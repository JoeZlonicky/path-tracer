#include "thread_pool.h"

#include <mutex>

ThreadPool::ThreadPool()
{
	int n_threads = std::thread::hardware_concurrency();
	for (int i = 0; i < n_threads; ++i) {
		threads.emplace_back(work);
	}
}

void ThreadPool::queue_task(std::function<void()> task)
{
	{
		std::unique_lock<std::mutex> lock(queue_mutex);
		task_queue.push(task);
	}
	cv.notify_one();
}

void ThreadPool::work()
{
	while (true) {
		std::function<void()> task;
		{
			std::unique_lock<std::mutex> lock(queue_mutex);
			cv.wait(lock, [this] { return !task_queue.empty(); });

			task = task_queue.front();
			task_queue.pop();
		}
		task();
	}
}

void ThreadPool::wait_for_completion()
{
	std::unique_lock<std::mutex> lock(queue_mutex);
	if (task_queue.empty()) return;
	cv.wait(lock, [this] { return task_queue.empty(); });
	return;
}
