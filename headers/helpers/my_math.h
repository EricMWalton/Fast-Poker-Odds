#ifndef ARRAY_HELPERS
#include "array_helpers.h"
#endif

#include <time.h>
#include <stdlib.h>
#include <cmath>

namespace my_math {


	// put in dest an array of n random integers from min to max (inclusive-exclusive) 
	template<typename T> void rand_unique(T * dest, T min, T max, unsigned int n)
		{
		for(unsigned int i = 0; i < n; ++i)
			{
			T r;
			do  {
				r = (T)rand() % (max - min) + min;
				} while(array_helpers::search<T>(r, dest, dest + i) != -1);
			assert(r < max && r >= min);
			dest[i] = r;   
			}
		}

	double defaultEps = 1e-3;

	bool isAlmostEqual(float a, float b, float eps = defaultEps)
		{
		   return (std::abs(a - b) < eps);
		}

	bool isAlmostEqual(double a, double b, float eps = defaultEps)
		{
		   return (std::abs(a - b) < eps);
		}

	}