#ifndef COMPAT_RANDOM_H
#define COMPAT_RANDOM_H

#include <cstdint>
#include "Timer.h"

#ifdef ARDUINO
#include <Arduino.h>
#else
#include <random>
#endif //ARDUINO

namespace compat_rand{

	#ifdef ARDUINO
	inline void randomSeedOnce() {
		static bool seeded = false;
		if(!seeded) {
			randomSeed(analogRead(A0)); //Assumes A0 pin is floating
			seeded = true;
		}
	}

	inline void randomSeedOnce(uint32_t entropy) {
		static bool seeded = false;
		if(!seeded) {
			randomSeed(entropy);
			seeded = true;
		}
	}

	#else //Pure C++ fallback (Linux+ systems)

	inline std::mt19937& getGenerator() {
		static std::random_device rd;
		static std::mt19937 gen(rd());
		return gen;
	}

	inline void randomSeed(uint32_t seed) {
		getGenerator().seed(seed);
	}

	inline void randomSeedOnce() {
		static bool seeded = false;
		if(! seeded) {
			randomSeed(static_cast<uint32_t>(millis()));
			seeded = true;
		}
	}

	inline void randomSeedOnce(uint32_t seed){
		static bool seeded = false;
		if(!seeded) {
			randomSeed(seed);
			seeded = true;
		}
	}

	inline long random(long upper) {
		randomSeedOnce();
		std::uniform_int_distribution<long> dist(0, upper -1);
		return dist(getGenerator());
	}

	inline long random(long lower, long upper) {
		randomSeedOnce();
		std::uniform_int_distribution<long> dist(lower, upper -1);
		return dist(getGenerator());
	}

#endif //ARDUINO
} //namespace compat_rand
#endif //COMPAT_RANDOM_H
