#include "multithreaded_task.h"

#include <functional>
#include <mutex>
#include <utility>
#include <vector>

MultithreadedTask::MultithreadedTask(std::function<void(int i)> func) : _func(func), _n_remaining(0) {
}

MultithreadedTask::~MultithreadedTask() {
	_should_terminate = true;
	for (auto& thread : _threads) {
		thread.join();
	}
}

void MultithreadedTask::start(int n)
{
	_should_terminate = true;
	for (auto& thread : _threads) {
		thread.join();
	}
	_threads.clear();

	auto n_threads = std::thread::hardware_concurrency();
	auto n_per_thread = n / n_threads;
	_n_remaining = n;
	_should_terminate = false;

	for (unsigned int i = 0; i < n_threads; ++i) {
		auto start = i * n_per_thread;
		auto end = (i == n_threads - 1) ? n : (i + 1) * n_per_thread;

		_threads.emplace_back(&MultithreadedTask::work, this, start, end);
	}
}

void MultithreadedTask::stop()
{
	_should_terminate = true;
}

int MultithreadedTask::get_n_remaining() {
	if (_should_terminate) return -1;
	return _n_remaining;
}

void MultithreadedTask::work(int start, int end) {
	for (auto i = start; i < end; ++i) {
		_func(i);
		--_n_remaining;
		if (_should_terminate) return;
	}
}