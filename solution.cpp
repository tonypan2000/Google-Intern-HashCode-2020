#include "process_data.h"
#include <iostream>
#include <string>


void get_header(istream& fin, Data& input) {
	int num_videos;
	int num_endpoints;
	int num_request_descriptions;
	int num_cache;
	int capacity_mb;
	fin >> num_videos >> num_endpoints >> num_request_descriptions >> num_cache >> capacity_mb;
	input.set_header(num_videos, num_endpoints, num_request_descriptions, num_cache, capacity_mb);
	input.reserve_vectors();
}

void get_video_size(istream& fin, Data& input) {
	for (int i = 0; i < input.get_num_videos(); ++i) {
		int size;
		fin >> size;
		input.set_video_size(size);
	}
}

void get_endpoints(istream& fin, Data& input) {
	for (int i = 0; i < input.get_num_endpoints(); ++i) {
		int latency_to_center_ms, num_connected_cache;
		fin >> latency_to_center_ms >> num_connected_cache;
		Data::Endpoint endpoint = Data::Endpoint(latency_to_center_ms, num_connected_cache);
		for (int j = 0; j < num_connected_cache; ++j) {
			int id, lat;
			fin >> id >> lat;
			endpoint.add_latency_entry(id, lat);
		}
		input.set_endpoint_info(endpoint);
	}
}

void get_requests(istream& fin, Data& input) {
	for (int i = 0; i < input.get_num_request_descriptions(); ++i) {
		int video_id, endpoint_id, num_requests;
		fin >> video_id >> endpoint_id >> num_requests;
		input.set_request_description(video_id, endpoint_id, num_requests);
	}
}

void Process::read_input(istream &fin, Data &input) {
	get_header(fin, input);
	get_video_size(fin, input);
	get_endpoints(fin, input);
	get_requests(fin, input);
}

ifstream open_file(string filename) {
	ifstream fin;
	fin.open(filename);
	if (!fin.is_open()) {
		cerr << "Error opening " << filename << endl;
		exit(1);
	}
	return fin;
}

void Process::process_data(Data& input, Output& result) {
	/*
	Idea A. cache server oriented (brute force)
	1. find all combinations of which videos to put into which cache servers
	2. calculate the time saved by each combinatiobn
	3. find the most optimal one
	*/

	/*
	Idea B. look at requests (naive)
	1. iterate through each endpoint request
	2. store each video that is connected to a cache server (without duplicates) until a cache is full
	3. return result
	*/
	int num_cache = input.get_num_cache_servers();
	result.reserve_output_size(num_cache);
	for (int i = 0; i < num_cache; ++i) {
		result.set_cache_info(input.get_cache_capacity());
	}
	for (auto r : input.get_requests()) {
		int video_id = r.video_id;
		int endpoint_id = r.endpoint_id;
		int video_size = input.get_video_size(video_id);

		// find the cache closest to endpoint (with capacity) to store video
		Data::Endpoint ep = input.get_endpoint(endpoint_id);
		if (!ep.num_connected_cache) {
			continue;
		}
		int cache_id = ep.get_closest_cache_id();
		while (!result.enough_capacity(cache_id, video_size) && cache_id != -1) {
			cache_id = ep.get_next_closest_cache_id();
			if (cache_id == -1) {
				break;
			}
		}
		if (cache_id == -1) {
			continue;
		}

		// store the video there
		result.store_video(cache_id, video_id, video_size);
	}

	result.count_num_cache();
}


int main(int argc, char** argv) {
	ios_base::sync_with_stdio(false);
	ifstream fin = open_file("example.in");
	Process solver;
	Data input;
	Output result;
	solver.read_input(fin, input);
	solver.process_data(input, result);
	//solver.print_results(result);
	return 0;
}
