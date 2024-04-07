#include "thread_pool.h"

#include <mutex>

std::mutex ThreadPool::queue_mutex;

ThreadPool::ThreadPool()
{
	auto n_threads = std::thread::hardware_concurrency();
	for (unsigned int i = 0; i < n_threads; ++i) {
		threads.emplace_back(&ThreadPool::work, this);
	}
}

ThreadPool::~ThreadPool()
{
	{
		std::unique_lock<std::mutex> lock{ queue_mutex };
		should_terminate = true;
	}

	cv.notify_all();
	for (auto& thread : threads) {
		thread.join();
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

int ThreadPool::get_n_remaining_tasks()
{
	std::unique_lock<std::mutex> lock{ queue_mutex };
	return task_queue.size();
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