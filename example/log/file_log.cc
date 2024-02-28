#include "file_log.h"

#include <stdio.h>
#include <string.h>
#include <time.h>

#include <iostream>
#include <string>

#if defined(WINDOWS) || defined(_WINDOWS) || defined(WIN32)
#include <direct.h>  // for mkdir rmdir
#include <io.h>      // for access
#else
#include <dirent.h>    //for DIR remove
#include <sys/stat.h>  //for access
#include <sys/types.h>
#include <unistd.h>  //for mkdir rmdir
#endif

#if defined(WINDOWS) || defined(_WINDOWS) || defined(WIN32)
#define ACCESS _access
#define MKDIR(a) _mkdir(a)
#else
#define ACCESS access
#define MKDIR(a) mkdir(a, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)
#endif

constexpr int k_maxPath = 260;

FILE* g_logFile = nullptr;

bool mkdir__(const std::string& strPath) {
  int i = 0;
  int nDirLen = strPath.length();
  if (nDirLen <= 0) return false;
  char* pDirTemp = new char[nDirLen + 4];
  strPath.copy(pDirTemp, nDirLen + 1, 0);  // +1 to copy '\0'
  pDirTemp[nDirLen] = '\0';
  // Add '/' into tail.
  if (pDirTemp[nDirLen - 1] != '\\' && pDirTemp[nDirLen - 1] != '/') {
    pDirTemp[nDirLen] = '/';
    pDirTemp[nDirLen + 1] = '\0';
    nDirLen++;
  }
  // Make dir.
  for (i = 0; i < nDirLen; i++) {
    if (pDirTemp[i] == '\\' || pDirTemp[i] == '/') {
      if (i == 0) continue;

      pDirTemp[i] = '\0';

      int statu;
      statu = ACCESS(pDirTemp, 0);
      if (statu != 0) {
        statu = MKDIR(pDirTemp);
        if (statu != 0) {
          return false;
        }
      }
      pDirTemp[i] = '/';
    }
  }
  delete[] pDirTemp;
  return true;
}

bool init(const std::string& dirPath) {
  clean();
  if (!mkdir__(dirPath)) {
    return false;
  }
  std::string filePath;
  filePath.append(dirPath);
  filePath.append("/log_");

  time_t rawtime;
  struct tm* ptminfo;
  time(&rawtime);
  ptminfo = localtime(&rawtime);
  char time_s[k_maxPath] = {0};
  snprintf(time_s, k_maxPath, "%02d%02d%02d%02d%02d%02d",
           ptminfo->tm_year + 1900, ptminfo->tm_mon + 1, ptminfo->tm_mday,
           ptminfo->tm_hour, ptminfo->tm_min, ptminfo->tm_sec);

  filePath.append(time_s);
  filePath.append(".log");
  g_logFile = fopen(filePath.c_str(), "wb+");
  return true;
}

void clean() {
  if (g_logFile) {
    fflush(g_logFile);
    fclose(g_logFile);
    g_logFile = nullptr;
  }
}

int write(char* data, int len) {
  if (g_logFile == nullptr) {
    return -1;
  }
  int w = fwrite(data, len, 1, g_logFile);
  fflush(g_logFile);
  return w;
}
