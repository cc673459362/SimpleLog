#include "time/time_util.h"
#if defined (WINDOWS) || defined(_WINDOWS) || defined(WIN32)
#include <chrono>
#else
#include <stddef.h>
#include <sys/time.h>
#endif

static GetTimeHook g_get_time_ = nullptr;

// Attention, in armeabi system, base time could be 5000s early
static int64_t time_init() {
#ifndef OS_WIN
  return 0;
#else
  std::chrono::nanoseconds now = std::chrono::system_clock::now().time_since_epoch();
  std::chrono::nanoseconds now_steady = std::chrono::steady_clock::now().time_since_epoch();
  return now.count() - now_steady.count();
#endif
}

#if defined (WINDOWS) || defined(_WINDOWS) || defined(WIN32)
static int64_t g_time_base = time_init();
#endif

#if defined (WINDOWS) || defined(_WINDOWS) || defined(WIN32)
#else
int64_t getSystemTimeMs() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  int64_t value = ((int64_t)tv.tv_sec * 1000 + tv.tv_usec / 1000);
  return value;
}
#endif

int32_t getLocalTimeS() {
  return getLocalTimeMs() / 1000;
}

void setTimeHook(GetTimeHook getTime) {
    g_get_time_ = getTime;
}

int64_t getLocalTimeMs() {
  uint64_t nowMs = 0;
  if (g_get_time_) {
    return g_get_time_();
  }
#if defined (WINDOWS) || defined(_WINDOWS) || defined(WIN32)
  std::chrono::nanoseconds now = std::chrono::steady_clock::now().time_since_epoch();
  nowMs = (now.count() + g_time_base) / 1000000;
#else
  nowMs = getSystemTimeMs();
#endif
  return nowMs;
}


int64_t getSystemLocalMs() {
  uint64_t nowMs = 0;
#if defined (WINDOWS) || defined(_WINDOWS) || defined(WIN32)
  std::chrono::nanoseconds now = std::chrono::steady_clock::now().time_since_epoch();
  nowMs = (now.count() + g_time_base) / 1000000;
#else
  nowMs = getSystemTimeMs();
#endif
  return nowMs;
}

bool getCurrentDate(std::string &outDate) {
  time_t tt = time(0);
  char tmp[32] = {0};
  strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S", localtime(&tt));
  outDate = tmp;
  return true;
}