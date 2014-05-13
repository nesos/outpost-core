/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#ifndef COBC_RTOS_POSIX_PERIODIC_TASK_MANAGER_H
#define COBC_RTOS_POSIX_PERIODIC_TASK_MANAGER_H

#include <time.h>

#include <cobc/time/duration.h>

#include "../mutex.h"
#include "../failure_handler.h"

namespace cobc
{
	namespace rtos
	{
		/**
		 * Helper class for Rate-Monotonic Scheduling (RMS).
		 *
		 * The POSIX implementation builds upon the clock_nanosleep() method
		 * with CLOCK_MONOTONIC as clock source.
		 *
		 * @author	Fabian Greif
		 * @ingroup	rtos
		 */
		class PeriodicTaskManager
		{
		public:
			struct Status
			{
				enum Type
				{
					/// Period has not been started
					idle,

					/// Period is currently running
					running,

					/// Period has expired
					timeout
				};
			};

			PeriodicTaskManager();

			~PeriodicTaskManager();

			/**
			 * Start next period.
			 *
			 * If the PeriodicTaskManager is running, the calling thread will
			 * be blocked for the remainder of the outstanding period and,
			 * upon completion of that period, the period will be reinitialized
			 * with the specified period.
			 *
			 * If the PeriodicTaskManager is not currently running and has
			 * not expired, it is initiated with a length of period ticks and
			 * the calling task returns immediately.
			 *
			 * If the PeriodicTaskManager has expired before the thread invokes
			 * the `nextPeriod` method, the period will be initiated with a
			 * length of *period* and the calling task returns immediately with
			 * a timeout error status.
			 *
			 * @param  period
			 *     Length of the next period. Can be different from the
			 *     previous one.
			 *
			 * @retval	Status::running
			 *     Period is currently running.
			 * @retval  Status::timeout
			 *     Last period was missed, this may require some different
			 *     handling from the user.
			 */
			Status::Type
			nextPeriod(time::Duration period);

			/**
			 * Check the status of the current period.
			 *
			 * @retval  Status::idle
			 *     Period has not been started.
			 * @retval	Status::running
			 *     Period is currently running.
			 * @retval  Status::timeout
			 *     Last period was missed, this may require some different
			 *     handling from the user.
			 */
			Status::Type
			status();

			/**
			 * Period measurement is stopped.
			 *
			 * Can be restarted with the invocation of `nextPeriod`.
			 */
			void
			cancel();

		private:
			Mutex mutex;
			bool timerRunning;
			struct timespec nextWakeTime;
		};
	}
}

#endif // COBC_RTOS_POSIX_PERIODIC_TASK_MANAGER_H