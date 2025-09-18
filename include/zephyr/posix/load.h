/*
 *
 * Functions to "eat" CPU-time.
 * The amount of time is measured using CPU-time clocks.
 *
 */

#ifndef ZEPHYR_INCLUDE_POSIX_LOAD_H__
#define ZEPHYR_INCLUDE_POSIX_LOAD_H__

#include <zephyr/posix/time.h>
#include <timespec_operations.h>


/*
 * eat_ts
 *
 * Executes during the interval of time 'cpu_time' 
 */
static inline void eat_ts(const struct timespec *cpu_time)
{
    struct timespec current_time, time_to_go;
    (void)clock_gettime(CLOCK_THREAD_CPUTIME_ID, &current_time);
    add_timespec(&time_to_go, &current_time, cpu_time);

    while (smaller_timespec(&current_time, &time_to_go)) {
        (void)clock_gettime(CLOCK_THREAD_CPUTIME_ID, &current_time);
    }
}

/*
 * eat
 *
 * Executes during the interval of time 'For_Seconds' 
 */
static inline void eat (float for_seconds)
{
  struct timespec time;
  double_to_timespec(for_seconds, &time);
  eat_ts(&time);
}

static inline void eat_cycles(uint64_t cycles) {
  struct timespec time; 
  //convert cycles to timespec and store it in time variable
  cycles_to_timespec(cycles, &time);  
  eat_ts(&time);
}

/*
 * adjust
 * 
 * Only for compatibility with load_loop.c
 */
static inline void adjust (void)
{
}

#endif // ZEPHYR_INCLUDE_POSIX_LOAD_H_