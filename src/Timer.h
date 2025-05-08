#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include <functional>
#include <thread>
#include <atomic>
#include <vector>
#include <memory>

template<typename T = uint32_t>
class Timer {
	public:
		Timer() : running(true) {}

		~Timer() {
			stop();
		}

		void every(T interval_ms, std::function<void()> callback) {
			threads.emplace_back([=]() {
				while(running) {
					std::this_thread::sleep_for(std::chrono::milliseconds(interval_ms));
					if(running) callback();
				}
			});
		}

		void stop() {
			running = false;
			for (auto& t : threads) {
				if(t.joinable()) t.join();
			}
			threads.clear();
		}
	private:
		std::atomic<bool> running;
		std::vector<std::thread> threads;
};
inline std::shared_ptr<Timer<>> timer_create_default() {
	return std::make_shared<Timer<>>();
}

inline Timer<>& getTimer() {
	static std::shared_ptr<Timer<>> defaultTimer = timer_create_default();
	return *defaultTimer;
}
		
#endif //TIMER_H
