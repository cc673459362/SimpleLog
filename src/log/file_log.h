#ifndef FILE_LOG_H_
#define FILE_LOG_H_
#include <string>

extern bool init(const std::string &dirPath);

void clean();

extern int write(char *data, int len);

#endif // FILE_LOG_H_