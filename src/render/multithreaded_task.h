#pragma once

#include <atomic>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>
#include <vector>

class MultithreadedTask {
public:
	MultithreadedTask(std::function<void(int i)> func);  // func needs to be thread-safe
	~MultithreadedTask();

	MultithreadedTask(const MultithreadedTask&) = delete;
	MultithreadedTask& operator=(const MultithreadedTask&) = delete;

	void start(int n);
	void stop();
	[[nodiscard]] int get_n_remaining();  // Returns -1 if ended prematurely

private:
	void work(int start, int end);

	std::function<void(int i)> _func;

	std::vector<std::thread> _threads{};
	std::atomic_int _n_remaining = 0;
	std::atomic_bool _should_terminate = false;
};