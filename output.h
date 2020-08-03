#pragma once
#include <unordered_map>
#include <vector>

using namespace std;

class Output {
	int num_cache;
	// maps video ids stored in each cache server to cache server id
	unordered_map<int, vector<int>> stored_videos;
};
