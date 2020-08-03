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

int main(int argc, char** argv) {
	ios_base::sync_with_stdio(false);
	ifstream fin = open_file("example.in");
	Process solver;
	Data input;
	solver.read_input(fin, input);
	//solver.process_data();
	//solver.print_results();
	return 0;
}
