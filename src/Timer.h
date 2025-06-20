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

		//non copyable
		Timer(const Timer&) = delete;
		Timer& operator = (const Timer&) = delete;

		//Schedule a function to be called every interval_ms milliseconds
		void every(T interval_ms, std::function<bool(void*)> callback, void* arg = nullptr) {
			std::lock_guard<std::mutex> lock(mutex);
			threads.emplace_back([=]() {
				while(running.load()) {
					std::this_thread::sleep_for(std::chrono::milliseconds(interval_ms));
					if(running.load()) {
						bool keepRunning = callback(arg);
						if(!keepRunning) break;
					}
				}
			});
		}

		void stop() {
			running.store(false);
			std::lock_guard<std::mutex> lock(mutex);
			for (auto& t : threads) {
				if(t.joinable()) {
					t.join();
				}	
			}
			threads.clear();
		}
	private:
		std::atomic<bool> running;
		std::vector<std::thread> threads;
		std::mutex mutex;
};

inline Timer<>& timer_create_default() {
	static Timer<> defaultTimer;
	return defaultTimer;
}

inline Timer<>& getTimer() {
	return timer_create_default();
}

inline unsigned long millis() {
	static const auto start_time = std::chrono::steady_clock::now();
	auto now = std::chrono::steady_clock::now();
	return std::chrono::duration_cast<std::chrono::milliseconds>(now - start_time).count();
}

inline unsigned long micros() {
	static const auto start_time = std::chrono::steady_clock::now();
	auto now = std::chrono::steady_clock::now();
	return std::chrono::duration_cast<std::chrono::microseconds>(now - start_time).count();
}

#endif //TIMER_H
