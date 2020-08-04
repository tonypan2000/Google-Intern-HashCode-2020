#pragma once
#include <unordered_set>

using namespace std;

class Output {
public:
	struct Cache {
		unordered_set<int> cached_video_ids;
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

	void store_video(int cache_id, int video_id, int video_size) {
		stored_videos[cache_id].cached_video_ids.insert(video_id);
		stored_videos[cache_id].capacity -= video_size;
	}

private:
	int num_cache;
	vector<Cache> stored_videos;
};
