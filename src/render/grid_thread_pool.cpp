#include "grid_thread_pool.h"

#include <functional>
#include <mutex>
#include <utility>
#include <vector>

GridThreadPool::GridThreadPool(std::function<void(int x, int y)> func) : _func(func) {
	auto n_threads = std::thread::hardware_concurrency();
	for(unsigned int i = 0; i < n_threads; ++i) {
		_threads.emplace_back(&GridThreadPool::work, this);
	}
}

GridThreadPool::~GridThreadPool() {
	{
		std::unique_lock<std::mutex> lock{_queue_mutex};
		_should_terminate = true;
	}

	_cv.notify_all();
	for(auto& thread : _threads) {
		thread.join();
	}
}

void GridThreadPool::queue_task(std::pair<int, int> coord) {
	{
		std::unique_lock<std::mutex> lock{_queue_mutex};
		_coord_queue.push(coord);
	}
	_cv.notify_one();
}

int GridThreadPool::get_n_remaining_tasks() {
	std::unique_lock<std::mutex> lock{_queue_mutex};
	return static_cast<int>(_coord_queue.size());
}

void GridThreadPool::work() {
	while(true) {
		std::pair<int, int> coord;
		{
			std::unique_lock<std::mutex> lock{_queue_mutex};
			_cv.wait(lock, [this] { return !_coord_queue.empty() || _should_terminate; });

			if(_should_terminate) return;

			coord = _coord_queue.front();
			_coord_queue.pop();
		}
		_func(coord.first, coord.second);
	}
}