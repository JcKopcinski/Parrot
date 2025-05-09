#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include <functional>
#include <thread>
#include <atomic>
#include <vector>
#include <memory>
#include <mutex>

template<typename T = uint32_t>
class Timer {
	public:
		Timer() : running(true) {}

		~Timer() {
			stop();
		}

		//Schedule a function to be called every interval_ms milliseconds
		void every(T interval_ms, std::function<void()> callback) {
			std::lock_guard<std::mutex> lock(mutex);
			threads.emplace_back([=]() {
				while(running) {
					std::this_thread::sleep_for(std::chrono::milliseconds(interval_ms));
					if(running) callback();
				}
			});
		}

		void stop() {
			running = false;
			std::lock_guard<std::mutex> lock(mutex);
			for (auto& t : threads) {
				if(t.joinable()) {t.join();}
			}
			threads.clear();
		}
	private:
		std::atomic<bool> running;
		std::vector<std::thread> threads;
		std::mutex mutex;
};
inline std::shared_ptr<Timer<>> timer_create_default() {
	return std::make_shared<Timer<>>();
}

inline Timer<>& getTimer() {
	static Timer<> defaultTimer;
	return defaultTimer;
}

inline unsigned long millis() {
	static const auto start_time = std::chrono::steady_clock::now();
	auto now = std::chrono::steady_clock::now();
	return std::chrono::duration_cast<std::chrono::milliseconds>(now - start_time).count();
}
#endif //TIMER_H
