/**
* Copyright(c) 2014, Phil Sampson (http://www.zamma.co.uk)
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#include "timing.h"
#include "platform.h"
#include <ctime>

#ifdef OS_WINDOWS
	#include <Windows.h>
	#include <iostream>
	static double g_freq;
	static bool g_timerInitialized = false;
#elif defined(OS_LINUX)
	#include <sys/time.h>
	static const long NANOSECONDS_PER_SECOND = 1000000000L;
#else
	#include <chrono>
	static std::chrono::system_clock::time_point m_epoch = std::chrono::high_resolution_clock::now();
#endif

double Time::GetTime()
{
	#ifdef OS_WINDOWS
		if (!g_timerInitialized)
		{
			LARGE_INTEGER li;
			if (!QueryPerformanceFrequency(&li))
			{
				std::cerr << "QueryPerformanceFrequency failed in timer initialization" << std::endl;
			}

			g_freq = double(li.QuadPart);
			g_timerInitialized = true;
		}

		LARGE_INTEGER li;
		if (!QueryPerformanceCounter(&li))
		{
			std::cerr << "QueryPerformanceCounter failed in get time!" << std::endl;
		}

		return double(li.QuadPart)/g_freq;

	#elif defined(OS_LINUX)
		timespec ts;
		clock_gettime(CLOCK_REALTIME, &ts);
		return (double)(((long)ts.tv_sec * NANOSECONDS_PER_SECOND) + ts.tv_nsec) / ((double)(NANOSECONDS_PER_SECOND));

	#else
		return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - m_epoch).count() / 1000000000.0;

	#endif
}
