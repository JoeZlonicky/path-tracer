#pragma once

#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>
#include <vector>

// Thread pool for performing a multithreaded task on a grid of coordinates
// Based off: https://stackoverflow.com/questions/15752659/thread-pooling-in-c11
class GridThreadPool {
public:
	GridThreadPool(std::function<void(int x, int y)> function);
	~GridThreadPool();

	GridThreadPool(const GridThreadPool&) = delete;
	GridThreadPool& operator=(const GridThreadPool&) = delete;

	void queue_task(std::pair<int, int> coord);
	int get_n_remaining_tasks();

private:
	void work();

	std::mutex _queue_mutex;

	std::vector<std::thread> _threads{};
	std::condition_variable _cv;
	bool _should_terminate = false;

	std::function<void(int x, int y)> _func;
	std::queue<std::pair<int, int>> _coord_queue{};
};