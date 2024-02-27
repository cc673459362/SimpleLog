/*
 * @Author: jiafengchen
 * @Date: 2024-02-27 15:29:09
 * @LastEditTime: 2024-02-27 17:18:45
 * @Description: 
 * Copyright (c) 2024 by Tencent Inc, All Rights Reserved. 
 */
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