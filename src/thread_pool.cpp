#include "thread_pool.h"

#include <mutex>

std::mutex GridThreadPool::queue_mutex;

GridThreadPool::GridThreadPool(std::function<void(int x, int y)> function) : _func(function)
{
	auto n_threads = std::thread::hardware_concurrency();
	for (unsigned int i = 0; i < n_threads; ++i) {
		_threads.emplace_back(&GridThreadPool::work, this);
	}
}

GridThreadPool::~GridThreadPool()
{
	{
		std::unique_lock<std::mutex> lock{ queue_mutex };
		_should_terminate = true;
	}

	cv.notify_all();
	for (auto& thread : _threads) {
		thread.join();
	}
}

void GridThreadPool::queue_task(std::pair<int, int> coord)
{
	{
		std::unique_lock<std::mutex> lock{ queue_mutex };
		_coord_queue.push(coord);
	}
	cv.notify_one();
}

int GridThreadPool::get_n_remaining_tasks()
{
	std::unique_lock<std::mutex> lock{ queue_mutex };
	return _coord_queue.size();
}

void GridThreadPool::work()
{
	while (true) {
		std::pair<int, int> coord;
		{
			std::unique_lock<std::mutex> lock{ queue_mutex };
			cv.wait(lock, [this] { return !_coord_queue.empty() || _should_terminate; });

			if (_should_terminate) return;

			coord = _coord_queue.front();
			_coord_queue.pop();
		}
		_func(coord.first, coord.second);
	}
}