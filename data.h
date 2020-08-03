#pragma once
#include <unordered_map>
#include <vector>

using namespace std;


class Data {
	int num_videos;	
	int num_endpoints;
	int num_request_descriptions;
	int num_cache;
	int capacity_mb;

	// size of each video in MB, indexed by video ID
	vector<int> video_size_mb;

	struct Endpoint {
		// latency from cache server to each endpoint
		// maps latency in ms to cache server ID
		unordered_map<int, int> latency_to_cache_ms;
		int latency_to_center_ms;
		int num_connected_cache;
		Endpoint(unordered_map<int, int> latency_to_cache_ms_in, int latency_to_center_ms_in, int num_connected_cache_in)
			: latency_to_cache_ms(latency_to_cache_ms_in), latency_to_center_ms(latency_to_center_ms_in), num_connected_cache(num_connected_cache_in) { }
	};
	vector<Endpoint> endpoints;

	struct Request {
		int video_id;
		int endpoint_id;
		int num_requests;
		Request(int video_id_in, int endpoint_id_in, int num_requests_in)
			: video_id(video_id_in), endpoint_id(endpoint_id_in), num_requests(num_requests_in) { }
	};
	vector<Request> requests;
};
