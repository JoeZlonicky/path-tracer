#pragma once

#include <functional>
#include <mutex>
#include <queue>
#include <thread>


class ThreadPool {
public:
	ThreadPool();

	void queue_task(std::function<void()> task);
	void work();
	void wait_for_completion();

private:
	std::vector<std::thread> threads;
	std::queue<std::function<void()>> task_queue;
	std::mutex queue_mutex;
	std::condition_variable cv;
};