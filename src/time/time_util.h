#ifndef TIME_UTILS_H_
#define TIME_UTILS_H_

#include <cstdint>
#include <string>

typedef uint64_t (*GetTimeHook)();

extern void setTimeHook(GetTimeHook getTime);

extern int32_t getLocalTimeS();

extern int64_t getLocalTimeMs();

// get the system time ignore timehook
extern int64_t getSystemLocalMs();

// get system Data
extern bool getCurrentDate(std::string &outDate);

#endif  // TIME_UTILS_H_