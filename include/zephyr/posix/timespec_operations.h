#ifndef ZEPHYR_INCLUDE_POSIX_TIMESPEC_OPERATIONS_H_
#define ZEPHYR_INCLUDE_POSIX_TIMESPEC_OPERATIONS_H_

#include <zephyr/posix/time.h>

typedef uint64_t hwtime_t;

struct timespec_hwtime {
	hwtime_t hwtime;
};

// CONSTANTES Y DEFINICIONES VALIDAS PARA STM32F4_DISCO
#define HWTIME_HZ 168000000 // 5000 //   
#define NS_IN_S   1000000000

/*TIMESPEC MACROS*/
#define TS(s, ns) {s, ns}
#define TS_ADD(a, b, c)                                                                            \
	(a).tv_nsec = (b).tv_nsec + (c).tv_nsec;                                                   \
	(a).tv_sec = (b).tv_sec + (c).tv_sec;                                                      \
	if ((a).tv_nsec >= NS_IN_S) {                                                              \
		(a).tv_nsec = (a).tv_nsec - NS_IN_S;                                               \
		(a).tv_sec = (a).tv_sec + 1;                                                       \
	}
#define TS_INC(a, b)                                                                               \
	(a).tv_nsec += (b).tv_nsec;                                                                \
	(a).tv_sec += (b).tv_sec;                                                                  \
	if ((a).tv_nsec >= NS_IN_S) {                                                              \
		(a).tv_nsec = (a).tv_nsec - NS_IN_S;                                               \
		(a).tv_sec = (a).tv_sec + 1;                                                       \
	}
#define TS_SEC(ts)  ((ts.)tv_sec)
#define TS_NSEC(ts) ((ts.)tv_nsec)

// MACROS PARA CONVERTIR HWTIME A TIMESPEC
// #define TS(s, ns)       {.hwtime = ((hwtime_t)(s)) * HWTIME_HZ + (hwtime_t)(ns) * HWTIME_HZ /
// NS_IN_S} 
// #define TS_ADD(a, b, c) (a).hwtime = (b).hwtime + (c).hwtime 
// #define TS_SUB(a, b, c)
// (a).hwtime = (b).hwtime - (c).hwtime 
// #define TS_INC(a, b)    (a).hwtime += (b).hwtime 
// #define // TS_SEC(ts)      ((ts).hwtime / HWTIME_HZ) #define TS_NSEC(ts)     (((ts).hwtime % HWTIME_HZ) *
// NS_IN_S / HWTIME_HZ)

// TIMESPEC MACROS & HWTIME USE
#define HWT2TS(ts, hwt)                                                                            \
	(ts).tv_sec = (hwt) / HWTIME_HZ;                                                           \
	(ts).tv_nsec = (hwtime_t)((hwt) % HWTIME_HZ) * NS_IN_S / HWTIME_HZ

#define TS2HWT(ts) ((hwtime_t) (ts).tv_sec * HWTIME_HZ + \
               (hwtime_t) (ts).tv_nsec * HWTIME_HZ / NS_IN_S)


static inline int smaller_timespec(const struct timespec *t1, const struct timespec *t2)
{
	return t1->tv_sec < t2->tv_sec || (t1->tv_sec == t2->tv_sec && t1->tv_nsec < t2->tv_nsec);
}

static inline int smaller_or_equal_timespec(const struct timespec *t1, const struct timespec *t2)
{
	return t1->tv_sec < t2->tv_sec || (t1->tv_sec == t2->tv_sec && t1->tv_nsec <= t2->tv_nsec);
}

static inline void incr_timespec(struct timespec *t1, const struct timespec *t2)
{
	t1->tv_sec += t2->tv_sec;
	t1->tv_nsec += t2->tv_nsec;
	if (t1->tv_nsec >= 1000000000) {
		t1->tv_sec++;
		t1->tv_nsec -= 1000000000;
	}
}

static inline void decr_timespec(struct timespec *t1, const struct timespec *t2)
{
	if (t1->tv_nsec < t2->tv_nsec) {
		t1->tv_sec -= t2->tv_sec + 1;
		t1->tv_nsec = t1->tv_nsec + 1000000000 - t2->tv_nsec;
	} else {
		t1->tv_sec -= t2->tv_sec;
		t1->tv_nsec -= t2->tv_nsec;
	}
}

static inline void add_timespec(struct timespec *s, const struct timespec *t1,
				const struct timespec *t2)
{
	s->tv_sec = t1->tv_sec + t2->tv_sec;
	s->tv_nsec = t1->tv_nsec + t2->tv_nsec;
	if (s->tv_nsec >= 1000000000) {
		s->tv_sec++;
		s->tv_nsec -= 1000000000;
	}
}

static inline void div_timespec(struct timespec *s, const struct timespec *dividend,
				const struct timespec *divisor)
{
	long long ldividend = dividend->tv_sec * 1000000000 + dividend->tv_nsec;
	long long ldivisor = divisor->tv_sec * 1000000000 + divisor->tv_nsec;
	long long result;

	result = ldividend / ldivisor;

	s->tv_sec = result / 1000000000;
	s->tv_nsec = result % 1000000000;
}

static inline void mult_timespec(struct timespec *s, const struct timespec *t1,
				 const struct timespec *t2)
{
	long long lt1 = t1->tv_sec * 1000000000 + t1->tv_nsec;
	long long lt2 = t2->tv_sec * 1000000000 + t2->tv_nsec;
	long long result;

	result = lt1 / lt2;

	s->tv_sec = result / 1000000000;
	s->tv_nsec = result % 1000000000;
}

typedef long long unsigned nanosecs_t;
char str_timespec_s[40];
static inline char *show_timespec_s(struct timespec *ts)
{
	nanosecs_t ns = (nanosecs_t)(ts->tv_sec) * 1000000000 + ts->tv_nsec;
	nanosecs_t s = ns / 1000000000;
	nanosecs_t ms = ns % 1000000000;
	nanosecs_t us = ns % 1000000;
	ns = ns % 1000;
	printf(str_timespec_s, "%3lus%3lums%3luus%3luns", (unsigned long)s,
	       (unsigned long)(ms / 1000000), (unsigned long)(us / 1000), (unsigned long)ns);
	return str_timespec_s;
}

//--------------------//
// timespec_to_double //
//--------------------//

static inline double timespec_to_double(const struct timespec *time)
{
	return time->tv_nsec * 0.000000001 + (double)time->tv_sec;
}

//--------------------//
// double_to_timespec //
//--------------------//

static inline void double_to_timespec(double time, struct timespec *ts)
{
	ts->tv_sec = (long)time;
	ts->tv_nsec = (long)((time - (double)ts->tv_sec) * 1000000000);
}

// FIXME
static inline void cycles_to_timespec(uint64_t cycles, struct timespec *ts)
{
	// USAMOS MACROS PARA CONVERTIR CICLOS A TIMESPEC

	uint64_t ns = k_cyc_to_ns_floor64(cycles);
	ts->tv_sec = ns / NSEC_PER_SEC;
	ts->tv_nsec = ns % NSEC_PER_SEC;

	printf("cycles_to_timespec: %lld cycles = %lld ns = %lld s %ld ns\n", cycles, ns,
	       ts->tv_sec, ts->tv_nsec);
}

#endif /* ZEPHYR_INCLUDE_POSIX_TIMESPEC_OPERATIONS_H_ */