#ifndef FILE_LOCKER_H_
#define FILE_LOCKER_H_
#include <string>
#include <stdio.h>
#include <atomic>

class FileLocker {
public:
    FileLocker(const std::string & file_path);
    virtual ~FileLocker();
    int lock();
    int unlock();
private:
    int fd_;
    std::atomic<bool> locked_;
    std::string file_path_;
};

#endif