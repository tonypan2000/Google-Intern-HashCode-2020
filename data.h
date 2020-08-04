#pragma once
#include <queue>
#include <vector>

using namespace std;


class Data {
public:
	void set_header(int num_videos_in, int num_endpoints_in, int num_request_descriptions_in, int num_cache_in, int capacity_mb_in) {
		num_videos = num_videos_in;
		num_endpoints = num_endpoints_in;
		num_request_descriptions = num_request_descriptions_in;
		num_cache = num_cache_in;
		capacity_mb = capacity_mb_in;
	}

	int get_num_videos() {
		return num_videos;
	}

	void reserve_vectors() {
		video_size_mb.reserve(num_videos);
		endpoints.reserve(num_endpoints);
	}

	void set_video_size(int size) {
		video_size_mb.push_back(size);
	}

	int get_video_size(int id) {
		return video_size_mb[id];
	}

	struct Endpoint {
		// latency from cache server to each endpoint
		// priority queue sorting latency in ms to cache server ID from low to high
		// pair id latency
		struct CompareCache {
			bool operator() (pair<int, int> const &l1, pair<int, int> const &l2) const {
				return l1.second > l2.second;
			}
		};

		priority_queue<pair<int, int>, vector<pair<int, int>>, CompareCache> latency_to_cache_ms;
		int latency_to_center_ms;
		int num_connected_cache;

		Endpoint(int latency_to_center_ms_in, int num_connected_cache_in)
			: latency_to_center_ms(latency_to_center_ms_in), num_connected_cache(num_connected_cache_in) { }

		void add_latency_entry(int id, int lat) {
			latency_to_cache_ms.emplace(make_pair(id, lat));
		}

		int get_closest_cache_id() {
			return latency_to_cache_ms.top().first;
		}

		int get_next_closest_cache_id() {
			if (!latency_to_cache_ms.empty()) {
				latency_to_cache_ms.pop();
			}
			if (!latency_to_cache_ms.empty()) {
				return latency_to_cache_ms.top().first;
			} else {
				return -1;
			}
		}
	};

	int get_num_endpoints() {
		return num_endpoints;
	}

	void set_endpoint_info(Endpoint &endpoint) {
		endpoints.push_back(endpoint);
	}

	Endpoint get_endpoint(int id) {
		return endpoints[id];
	}

	
	struct Request {
		int video_id;
		int endpoint_id;
		int num_requests;
		Request(int video_id_in, int endpoint_id_in, int num_requests_in)
			: video_id(video_id_in), endpoint_id(endpoint_id_in), num_requests(num_requests_in) { }
	};

	// rank each request information by the number of the same request made from the most to the least
	struct CompareRequest {
		bool operator() (Request const& r1, Request const& r2) const {
			return r1.num_requests < r2.num_requests;
		}
	};

	int get_num_request_descriptions() {
		return num_request_descriptions;
	}

	void set_request_description(int video_id, int endpoint_id, int num_requests) {
		requests.emplace(Request(video_id, endpoint_id, num_requests));
	}

	priority_queue<Request, vector<Request>, CompareRequest> get_requests() {
		return requests;
	}

	int get_num_cache_servers() {
		return num_cache;
	}

	int get_cache_capacity() {
		return capacity_mb;
	}
	
private:
	int num_videos;
	int num_endpoints;
	int num_request_descriptions;
	int num_cache;
	int capacity_mb;

	// size of each video in MB, indexed by video ID
	vector<int> video_size_mb;

	vector<Endpoint> endpoints;

	priority_queue<Request, vector<Request>, CompareRequest> requests;
};
