#pragma once
#include <functional>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <memory>
#include <utility>

class ThreadPool
{
private:
	std::vector<std::thread> workers_;
	std::queue<std::function<void()>> tasks_;
	std::mutex queue_mutex_;
	std::condition_variable condition_variable_;
	bool stop_{false};
public:
	explicit ThreadPool(unsigned int size = std::thread::hardware_concurrency());
	~ThreadPool();

	template<class F, class... Args>
	auto Add(F &&f, Args  &&... args) -> std::future<typename std::result_of<F(Args...)>::type>;
};

template<class F, class... Args>
auto ThreadPool::Add(F &&f, Args &&... args) -> std::future<typename std::result_of<F(Args...)>::type>
{
	using return_type = typename std::result_of<F(Args...)>::type;
	auto task = std::make_shared<std::packaged_task<return_type()>>(
			std::bind(std::forward<F>(f), std::forward<Args>(args)...));
	std::future<return_type> res = task->get_future();
	{
		std::unique_lock<std::mutex> lock(queue_mutex_);

		// don't allow enqueueing after stopping the pool
		if(stop_)
			throw std::runtime_error("enqueue on stopped ThreadPool");
		tasks_.emplace([task]() { (*task)(); });
	}
	condition_variable_.notify_one();
	return res;
}

