#include "thread_pool.h"

#include <mutex>

std::mutex ThreadPool::queue_mutex;

ThreadPool::ThreadPool()
{
	auto n_threads = std::thread::hardware_concurrency();
	for (auto i = 0; i < n_threads; ++i) {
		threads.emplace_back(&ThreadPool::work, this);
	}
}

void ThreadPool::queue_task(std::function<void()> task)
{
	{
		std::unique_lock<std::mutex> lock{ queue_mutex };
		task_queue.push(task);
	}
	cv.notify_one();
}

void ThreadPool::wait_for_completion()
{
	while (true) {
		std::unique_lock<std::mutex> lock{ queue_mutex };
		if (task_queue.empty()) {
			should_terminate = true;
			break;
		}
	}

	cv.notify_all();
	for (auto& thread : threads) {
		thread.join();
	}
	threads.clear();
}

void ThreadPool::work()
{
	while (true) {
		std::function<void()> task;
		{
			std::unique_lock<std::mutex> lock{ queue_mutex };
			cv.wait(lock, [this] { return !task_queue.empty() || should_terminate; });

			if (should_terminate) return;

			task = task_queue.front();
			task_queue.pop();
		}
		task();
	}
}