
#ifndef LOG_H_
#define LOG_H_

#include <stdarg.h>
#include <string.h>

#include <sstream>

#ifndef _LINUX
#include <thread>
#endif

enum LogLevel {
  VERBOSE__ = 0,
  DEBUG__ = 1,
  INFO__ = 2,
  WARNING__ = 3,
  ERROR__ = 4,
  DISABLE__ = 5,
};

// Attention, should be > 200
constexpr int k_maxLogSize = 512;
extern LogLevel s_logLevel;
extern bool s_traceOpen;
extern bool s_traceFile;
extern bool s_traceTime;

typedef int (*LogCallBack)(int prio, const char* text);

extern void SET_LOG_LEVEL(LogLevel level);
extern void SET_FILE_LOG(const char* dir);
extern void CLOSE_FILE_LOG();
extern void SET_TRACE_LOG(bool traceOpen);
extern void SET_TRACE_FILE(bool traceFile);
extern void SET_TRACE_TIME(bool traceTime);
extern void SET_LOG_CALLBACK(LogCallBack LogCallback);

// tansform log to Hex
extern std::string stringToHex(const char* str, const int len,
                               std::string separator = "");
extern void __printHex(const char* tag, char* buff, int buff_len);

#if defined(WINDOWS) || defined(_WINDOWS) || defined(WIN32)
extern unsigned long long GET_THREAD_ID();
#else
extern pid_t GET_THREAD_ID();
#endif

extern void PUSH_TO_LOG_QUEUE(LogLevel level, const char* fmt, ...);
extern void printProc(int level, char* log, int len);

#if defined(WINDOWS) || defined(_WINDOWS) || defined(WIN32)
#define __FILENAME__ \
  (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#define __PRINT_LOG__(level, fmt, ...)                                         \
  do {                                                                         \
    if (s_logLevel > level) break;                                             \
    if (s_traceOpen) {                                                         \
      PUSH_TO_LOG_QUEUE(level, "[%llu][%s:%d(%s)] " fmt "\n", GET_THREAD_ID(), \
                        __FILENAME__, __LINE__, __FUNCTION__, ##__VA_ARGS__);  \
    } else if (s_traceFile) {                                                  \
      PUSH_TO_LOG_QUEUE(level, "[%s]" fmt "\n", __FILENAME__, ##__VA_ARGS__);  \
    } else {                                                                   \
      PUSH_TO_LOG_QUEUE(level, fmt "\n", ##__VA_ARGS__);                       \
    }                                                                          \
  } while (false)
#else
#define __FILENAME__ \
  (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define __PRINT_LOG__(level, fmt, ...)                                        \
  do {                                                                        \
    if (s_logLevel > level) break;                                            \
    if (s_traceOpen) {                                                        \
      PUSH_TO_LOG_QUEUE(level, "[%d][%s:%d(%s)] " fmt "\n", GET_THREAD_ID(),  \
                        __FILENAME__, __LINE__, __FUNCTION__, ##__VA_ARGS__); \
    } else if (s_traceFile) {                                                 \
      PUSH_TO_LOG_QUEUE(level, "[%s]" fmt "\n", __FILENAME__, ##__VA_ARGS__); \
    } else {                                                                  \
      PUSH_TO_LOG_QUEUE(level, fmt "\n", ##__VA_ARGS__);                      \
    }                                                                         \
  } while (false)
#endif

#define DEBUG_LOG_V(fmt, ...) __PRINT_LOG__(VERBOSE__, fmt, ##__VA_ARGS__);

#define DEBUG_LOG_D(fmt, ...) __PRINT_LOG__(DEBUG__, fmt, ##__VA_ARGS__);

#define DEBUG_LOG_I(fmt, ...) __PRINT_LOG__(INFO__, fmt, ##__VA_ARGS__);

#define DEBUG_LOG_W(fmt, ...) __PRINT_LOG__(WARNING__, fmt, ##__VA_ARGS__);

#define DEBUG_LOG_E(fmt, ...) __PRINT_LOG__(ERROR__, fmt, ##__VA_ARGS__);

#endif  // LOG_H_
