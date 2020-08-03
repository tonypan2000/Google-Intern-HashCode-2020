#pragma once
#include <unordered_map>
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
		requests.reserve(num_request_descriptions);
	}

	void set_video_size(int size) {
		video_size_mb.push_back(size);
	}

	struct Endpoint {
		// latency from cache server to each endpoint
		// maps latency in ms to cache server ID
		unordered_map<int, int> latency_to_cache_ms;
		int latency_to_center_ms;
		int num_connected_cache;
		Endpoint(int latency_to_center_ms_in, int num_connected_cache_in)
			: latency_to_center_ms(latency_to_center_ms_in), num_connected_cache(num_connected_cache_in) { }
		void add_latency_entry(int id, int lat) {
			latency_to_cache_ms.emplace(id, lat);
		}
	};

	int get_num_endpoints() {
		return num_endpoints;
	}

	void set_endpoint_info(Endpoint &endpoint) {
		endpoints.push_back(endpoint);
	}

	struct Request {
		int video_id;
		int endpoint_id;
		int num_requests;
		Request(int video_id_in, int endpoint_id_in, int num_requests_in)
			: video_id(video_id_in), endpoint_id(endpoint_id_in), num_requests(num_requests_in) { }
	};

	int get_num_request_descriptions() {
		return num_request_descriptions;
	}

	void set_request_description(int video_id, int endpoint_id, int num_requests) {
		requests.push_back(Request(video_id, endpoint_id, num_requests));
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

	vector<Request> requests;
};
