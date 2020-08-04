#pragma once
#include <unordered_set>

using namespace std;

class Output {
public:
	// https://stackoverflow.com/questions/15160889/how-can-i-make-an-unordered-set-of-pairs-of-integers-in-c
	struct pair_hash {
		inline std::size_t operator()(const std::pair<int, int>& v) const {
			return v.first * 31 + v.second;
		}
	};

	struct Cache {
		// set of video id and endpoint id to remove duplicates
		unordered_set<pair<int, int>, pair_hash> cached_video_ids;
		int capacity;

		Cache(int capacity_in)
			: capacity(capacity_in) { }
	};

	void reserve_output_size(int size) {
		stored_videos.reserve(size);
	}

	void set_cache_info(int capacity) {
		stored_videos.push_back(Cache(capacity));
	}

	void count_num_cache() {
		num_cache = 0;
		for (Cache c : stored_videos) {
			if (!c.cached_video_ids.empty()) {
				++num_cache;
			}
		}
	}

	bool enough_capacity(int id, int size) {
		return stored_videos[id].capacity >= size;
	}

	void store_video(int cache_id, int video_id, int video_size, int endpoint_id) {
		stored_videos[cache_id].cached_video_ids.insert(make_pair(video_id, endpoint_id));
		stored_videos[cache_id].capacity -= video_size;
	}

	int get_num_cache() {
		return num_cache;
	}

	vector<Cache> get_videos_in_cache() {
		return stored_videos;
	}

	bool duplicate_exist(int video_id, int endpoint_id) {
		for (Cache c : stored_videos) {
			if (c.cached_video_ids.find(make_pair(video_id, endpoint_id)) != c.cached_video_ids.end()) {
				return true;
			}
		}
		return false;
	}

private:
	int num_cache;
	vector<Cache> stored_videos;
};
