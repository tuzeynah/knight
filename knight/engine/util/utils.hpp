#pragma once
#include "math/geometry.h"
#include <chrono>
#include <random>

namespace util {
	namespace number {
		inline int get_random_int(const int min, const int max)
		{
			std::random_device rdd;
			std::mt19937 rnd(rdd());
			std::uniform_int_distribution<int> distribution(min, max);
			return distribution(rnd);
		}

		inline float get_random_float(float min, float max) {
			static std::random_device rd;
			static std::mt19937 rng(rd());
			std::uniform_real_distribution<float> dist(min, max);
			return dist(rng);
		}

		inline float noise_float(const float min, const float max)
		{
			static std::mt19937 mersenne(static_cast<std::mt19937::result_type>(std::time(nullptr)));
			std::uniform_real_distribution<float> distribution(min, max);
			return distribution(mersenne);
		}

		inline double get_time() {
			using namespace std::chrono;
			auto now = high_resolution_clock::now();
			auto ms = time_point_cast<milliseconds>(now);
			return ms.time_since_epoch().count() / 1000.0; 
		}

		inline void precise_timer_sleep(double f_seconds)
		{
			while (f_seconds > 5e-3)
			{
				auto start = std::chrono::high_resolution_clock::now();
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
				auto end = std::chrono::high_resolution_clock::now();

				auto observed = (end - start).count() / 1e9;
				f_seconds -= observed;
			}

			/* ~~ spin lock */
			auto start = std::chrono::high_resolution_clock::now();
			while ((std::chrono::high_resolution_clock::now() - start).count() / 1e9 < f_seconds);
		}

	}

	namespace extra {
		inline Vector3 from_hsb(float h, float s, float v)
		{
			float _r, _g, _b;

			float _h = h == 1.0f ? 0 : h * 6.0f;
			auto i = static_cast<int>(trunc(_h));
			float f = _h - i;

			const float p = v * (1.f - s);
			const float q = v * (1.f - s * f);
			const float t = v * (1.f - s * (1.f - f));

			switch (i) {
			case 0:
				_r = v;
				_g = t;
				_b = p;
				break;

			case 1:
				_r = q;
				_g = v;
				_b = p;
				break;

			case 2:
				_r = p;
				_g = v;
				_b = t;
				break;

			case 3:
				_r = p;
				_g = q;
				_b = v;
				break;

			case 4:
				_r = t;
				_g = p;
				_b = v;
				break;

			default:
				_r = v;
				_g = p;
				_b = q;
				break;
			}

			return Vector3((unsigned char)(_r * 255), (unsigned char)(_g * 255), (unsigned char)(_b * 255));
		}

		inline void sort_vec(std::vector<double>& v) 
		{

			for (size_t i = 0; i < v.size(); i++) {

				for (size_t j = i + 1; j < v.size(); j++) {

					if (v[i] > v[j]) {

						double x = v[i];
						v[i] = v[j];
						v[j] = x;
					}

				}

			}

		}

		inline bool in_range(double x, double y, double z) {
			if (y > z) {

				if (x > y) return false;
				if (x < z) return false;
				return true;

			}

			if (z > y) {

				if (x > z) return false;
				if (x < y) return false;
				return true;

			}

			return false;

		}
	}
}