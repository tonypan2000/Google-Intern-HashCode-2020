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

ifstream open_input_file(string filename) {
	ifstream fin;
	fin.open(filename);
	if (!fin.is_open()) {
		cerr << "Error opening " << filename << endl;
		exit(1);
	}
	return fin;
}

void Process::read_input(string filename, Data &input) {
	ifstream fin = open_input_file(filename);
	get_header(fin, input);
	get_video_size(fin, input);
	get_endpoints(fin, input);
	get_requests(fin, input);
	fin.close();
}

/*
	Idea B. look at requests (naive)
	1. iterate through each endpoint request
	2. store each video that is connected to a cache server (without duplicates) until a cache is full
	3. return result
*/
void naive_solution(Data& input, Output& result) {
	int num_cache = input.get_num_cache_servers();
	result.reserve_output_size(num_cache);
	for (int i = 0; i < num_cache; ++i) {
		result.set_cache_info(input.get_cache_capacity());
	}

	priority_queue<Data::Request, vector<Data::Request>, Data::CompareRequest> pq = input.get_requests();
	while (!pq.empty()) {
		Data::Request r = pq.top();
		pq.pop();

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

void Process::process_data(Data& input, Output& result) {
	/*
	Idea A (TODO): cache server oriented (brute force)
	1. find all combinations of which videos to put into which cache servers
	2. calculate the time saved by each combinatiobn
	3. find the most optimal one
	*/

	naive_solution(input, result);
}

ofstream open_output_file(string filename) {
	ofstream out;
	out.open(filename);
	if (!out.is_open()) {
		cerr << "Error opening " << filename << endl;
		exit(1);
	}
	return out;
}

void Process::save_results(string filename, Output& result) {
	ofstream out = open_output_file(filename);
	out << result.get_num_cache() << '\n';
	vector<Output::Cache> videos_in_cache = result.get_videos_in_cache();
	for (int i = 0; i < videos_in_cache.size(); ++i) {
		if (!videos_in_cache[i].cached_video_ids.empty()) {
			out << i;
			for (int id : videos_in_cache[i].cached_video_ids) {
				out << ' ' << id;
			}
			out << '\n';
		}
	}
	out.close();
}


int main(int argc, char** argv) {
	ios_base::sync_with_stdio(false);
	
	string input_files[5] = {"example", "learning_cooking_from_youtube", "me_working_from_home", "music_videos_of_2020", "vloggers_of_the_world"};
	// string input_files[1] = { "learning_cooking_from_youtube" };

	for (string file : input_files) {
		Process solver;
		Data input;
		Output result;
		solver.read_input(file + ".in", input);
		solver.process_data(input, result);
		solver.save_results(file + ".out", result);
	}
	return 0;
}
