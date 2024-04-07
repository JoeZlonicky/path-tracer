#pragma once

#include <functional>
#include <mutex>
#include <queue>

// Based off: https://stackoverflow.com/questions/15752659/thread-pooling-in-c11
class ThreadPool {
public:
	ThreadPool();
	~ThreadPool();

	ThreadPool(const ThreadPool&) = delete;
	ThreadPool& operator=(const ThreadPool&) = delete;

	void queue_task(std::function<void()> task);
	int get_n_remaining_tasks();

private:
	void work();

	std::vector<std::thread> threads{};
	std::queue<std::function<void()>> task_queue{};
	static std::mutex queue_mutex;
	std::condition_variable cv;
	bool should_terminate = false;
};