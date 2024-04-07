#pragma once

#include <functional>
#include <queue>
#include <mutex>

// Based off: https://stackoverflow.com/questions/15752659/thread-pooling-in-c11
class ThreadPool {
public:
	ThreadPool();
	~ThreadPool() = default;

	ThreadPool(const ThreadPool&) = delete;
	ThreadPool& operator=(const ThreadPool&) = delete;

	void queue_task(std::function<void()> task);
	void wait_for_completion();

private:
	void work();

	std::vector<std::thread> threads{};
	std::queue<std::function<void()>> task_queue{};
	static std::mutex queue_mutex;
	std::condition_variable cv;
	bool should_terminate = false;
};