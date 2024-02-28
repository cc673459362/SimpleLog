#include "log/log.h"

#include "log/file_log.h"
#include "time/time_util.h"

#if defined(WINDOWS) || defined(_WINDOWS) || defined(WIN32)
#include <ws2tcpip.h>
#else
#include <pthread.h>  // pthread_self()
#endif

#ifdef DEBUG
LogLevel s_logLevel = INFO__;
#else
LogLevel s_logLevel = WARNING__;
#endif

constexpr int kMaxLen = 1024;
bool s_traceOpen = false;
bool s_traceFile = false;
bool s_traceTime = false;
static LogCallBack g_logCallback = nullptr;

void CallbackLog(int prio, const char *buf) {
  if (g_logCallback != nullptr) {
    g_logCallback(prio, buf);
  }
}

void SET_LOG_LEVEL(LogLevel level) { s_logLevel = level; }

void SET_FILE_LOG(const char *dir) { init(dir); }

void CLOSE_FILE_LOG() { clean(); }

void SET_TRACE_LOG(bool traceOpen) { s_traceOpen = traceOpen; }

void SET_TRACE_FILE(bool traceFile) { s_traceFile = traceFile; }

void SET_TRACE_TIME(bool traceTime) { s_traceTime = traceTime; }

void SET_LOG_CALLBACK(LogCallBack LogCallback) { g_logCallback = LogCallback; }

void printProc(int level, char *log, int len) {
  if (g_logCallback != nullptr) {
    CallbackLog(level, log);
    return;
  }
#ifdef DEBUG
  printf("%s", log);
#endif
#if defined(WINDOWS) || defined(_WINDOWS) || defined(WIN32)
#ifdef _UNICODE
  if (g_logCallback == NULL) {
    const int maxWideCharLogSize = 512;
    TCHAR sOut[maxWideCharLogSize] = {0};
    if (MultiByteToWideChar(CP_ACP, 0, log, len, sOut, maxWideCharLogSize) >
        0) {
      OutputDebugString(sOut);
    }
  }
#else
  OutputDebugString(log);
#endif  // _UNICODE
#endif
#ifndef ANDROID
  write(log, len);
#endif
}

inline static int appendLogTime(char *buf, int bufLen) {
  time_t tt = getLocalTimeS();
  struct tm *ttime;
  ttime = localtime(&tt);
  int len = strftime(buf, bufLen, "%H:%M:%S", ttime);
  int ms = getLocalTimeMs() % 1000;
  len += snprintf(buf + len, bufLen, ".%.3d ", ms);
  return len;
}

inline static int appendLogTimestamp(char *buf, int bufLen) {
  int64_t tt = static_cast<int64_t>(getLocalTimeMs());
  int len = snprintf(buf, bufLen, "%lld ", tt);
  return len;
}

inline static int appendLevelTag(LogLevel level, char *buf, int bufLen) {
  const char *tag = nullptr;
  switch (level) {
    case VERBOSE__:
      tag = "[V]";
      break;
    case DEBUG__:
      tag = "[D]";
      break;
    case INFO__:
      tag = "[I]";
      break;
    case WARNING__:
      tag = "[W]";
      break;
    case ERROR__:
      tag = "[E]";
      break;
    case DISABLE__:
      return 0;
  }
  return snprintf(buf, bufLen, "%s ", tag);
}

void PUSH_TO_LOG_QUEUE(LogLevel level, const char *fmt, ...) {
  char line[k_maxLogSize] = {0};

  int offset = 0;
  if (s_traceTime) {
    offset += appendLogTime(line + offset, k_maxLogSize - offset);
  }
  offset += appendLevelTag(level, line + offset, k_maxLogSize - offset);
  if (s_traceTime) {
    offset += appendLogTimestamp(line + offset, k_maxLogSize - offset);
  }
  // protect offset is not valid.
  if (offset >= k_maxLogSize || offset <= 0) {
    return;
  }
  int vs_read = 0;
  va_list args;
  va_start(args, fmt);

  vs_read = vsnprintf(line + offset, k_maxLogSize - offset, fmt, args);

  va_end(args);

  if (vs_read > k_maxLogSize) {
    offset += k_maxLogSize - offset;
    line[offset - 4] = '.';
    line[offset - 3] = '.';
    line[offset - 2] = '\n';
    line[offset - 1] = '\0';
  } else {
    offset += vs_read;
  }

  printProc(level, line, offset);
}

#if defined(WINDOWS) || defined(_WINDOWS) || defined(WIN32)
unsigned long long GET_THREAD_ID() {
  std::ostringstream oss;
  oss << std::this_thread::get_id();
  std::string stid = oss.str();
  return std::stoull(stid);
}
#elif defined(__APPLE__)
pid_t GET_THREAD_ID() { return pthread_mach_thread_np(pthread_self()); }
#else
pid_t GET_THREAD_ID() { return pthread_self(); }
#endif

const int kPrintHexLen = 256;
std::string stringToHex(const char *str, const int len, std::string separator) {
  const std::string hex = "0123456789ABCDEF";
  std::stringstream ss;
  int hexLen = len;
  if (hexLen > kPrintHexLen) {
    hexLen = kPrintHexLen;
  }
  for (std::string::size_type i = 0; i < hexLen; ++i) {
    ss << hex[(unsigned char)str[i] >> 4] << hex[(unsigned char)str[i] & 0xf]
       << separator;
  }
  return ss.str();
}

void __printHex(const char *tag, char *buff, int buff_len) {
  if (buff_len > kMaxLen) {
    buff_len = kMaxLen;
  }
  char str[kMaxLen];
  int ret = 0;
  memset(str, '\0', kMaxLen);
  for (int i = 0; i < buff_len; i++) {
    if (ret >= kMaxLen) {
      break;
    }
    ret += snprintf(str + ret, kMaxLen - ret, "%02x", (uint8_t)buff[i]);
  }
  DEBUG_LOG_V("%s:%s", tag, str);
}
