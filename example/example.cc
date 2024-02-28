#include "log/log.h"

void testLogLevel() {
  SET_TRACE_LOG(true);
  SET_LOG_LEVEL(LogLevel::INFO__);
  DEBUG_LOG_I("LogLevel test Start:");

  SET_LOG_LEVEL(LogLevel::VERBOSE__);
  DEBUG_LOG_V("====setlevel:%d====", s_logLevel);
  DEBUG_LOG_D("====setlevel:%d====", s_logLevel);
  DEBUG_LOG_I("====setlevel:%d====", s_logLevel);
  DEBUG_LOG_W("====setlevel:%d====", s_logLevel);
  DEBUG_LOG_E("====setlevel:%d====", s_logLevel);

  SET_LOG_LEVEL(LogLevel::DEBUG__);
  DEBUG_LOG_V("====setlevel:%d====", s_logLevel);
  DEBUG_LOG_D("====setlevel:%d====", s_logLevel);
  DEBUG_LOG_I("====setlevel:%d====", s_logLevel);
  DEBUG_LOG_W("====setlevel:%d====", s_logLevel);
  DEBUG_LOG_E("====setlevel:%d====", s_logLevel);

  SET_LOG_LEVEL(LogLevel::INFO__);
  DEBUG_LOG_V("====setlevel:%d====", s_logLevel);
  DEBUG_LOG_D("====setlevel:%d====", s_logLevel);
  DEBUG_LOG_I("====setlevel:%d====", s_logLevel);
  DEBUG_LOG_W("====setlevel:%d====", s_logLevel);
  DEBUG_LOG_E("====setlevel:%d====", s_logLevel);

  SET_LOG_LEVEL(LogLevel::WARNING__);
  DEBUG_LOG_V("====setlevel:%d====", s_logLevel);
  DEBUG_LOG_D("====setlevel:%d====", s_logLevel);
  DEBUG_LOG_I("====setlevel:%d====", s_logLevel);
  DEBUG_LOG_W("====setlevel:%d====", s_logLevel);
  DEBUG_LOG_E("====setlevel:%d====", s_logLevel);

  SET_LOG_LEVEL(LogLevel::ERROR__);
  DEBUG_LOG_V("====setlevel:%d====", s_logLevel);
  DEBUG_LOG_D("====setlevel:%d====", s_logLevel);
  DEBUG_LOG_I("====setlevel:%d====", s_logLevel);
  DEBUG_LOG_W("====setlevel:%d====", s_logLevel);
  DEBUG_LOG_E("====setlevel:%d====", s_logLevel);

  SET_LOG_LEVEL(LogLevel::DISABLE__);
  DEBUG_LOG_V("====setlevel:%d====", s_logLevel);
  DEBUG_LOG_D("====setlevel:%d====", s_logLevel);
  DEBUG_LOG_I("====setlevel:%d====", s_logLevel);
  DEBUG_LOG_W("====setlevel:%d====", s_logLevel);
  DEBUG_LOG_E("====setlevel:%d====", s_logLevel);

  SET_LOG_LEVEL(LogLevel::INFO__);
  DEBUG_LOG_I("LogLevel test Finish!");
}

void testLogTrace() {
  DEBUG_LOG_I("LogTrace test Start:");

  SET_TRACE_LOG(true);
  DEBUG_LOG_I("SET_TRACE_LOG:%d", s_traceOpen);
  SET_TRACE_LOG(false);
  DEBUG_LOG_I("SET_TRACE_LOG:%d", s_traceOpen);

  SET_TRACE_FILE(true);
  DEBUG_LOG_I("SET_TRACE_FILE:%d", s_traceFile);
  SET_TRACE_FILE(false);
  DEBUG_LOG_I("SET_TRACE_FILE:%d", s_traceFile);

  SET_TRACE_TIME(true);
  DEBUG_LOG_I("SET_TRACE_TIME:%d", s_traceTime);
  SET_TRACE_TIME(false);
  DEBUG_LOG_I("SET_TRACE_TIME:%d", s_traceTime);

  DEBUG_LOG_I("LogTrace test Finish!");
}

int LogCallback(int level, const char* context) {
  printf("this is from callback log:%s", context);
  return 0;
}

void testLogCallback() {
  DEBUG_LOG_I("LogCallback test Start:");

  SET_LOG_CALLBACK(LogCallback);
  DEBUG_LOG_I("SET_LOG_CALLBACK Test!");
  SET_LOG_CALLBACK(nullptr);

  DEBUG_LOG_I("LogCallback test Finish!");
}

void testLogFile() {
  DEBUG_LOG_I("LogFile test Start:");

  SET_FILE_LOG("./client_log_file");
  DEBUG_LOG_I("LogFile test!");
  CLOSE_FILE_LOG();

  DEBUG_LOG_I("LogFile test Finish!");
}

int main() {
  testLogLevel();

  testLogTrace();

  testLogFile();

  testLogCallback();

  return 0;
}