#include "file_locker.h"
#include <sys/file.h>
 #include <unistd.h>
FileLocker::FileLocker(const std::string & file_path)
{
    file_path_ = file_path;
    locked_ = false;
    fd_ = -1;
}

int FileLocker::lock()
{
    if((fd_ = open(file_path_.c_str(), O_RDONLY)) == -1)
    {
        return -1;
    }

    int retry_count = 0;
    do {
        if(flock(fd_, LOCK_EX) == -1)
        {
            if(errno != EINTR && errno != EWOULDBLOCK)
            {
                return -2;
            }
        }
    } while(retry_count++ < 3);

    locked_ = true;
    return 0;
}

int FileLocker::unlock()
{
    if(!locked_)
    {
        return 0;
    }

    if(fd_ != -1)
    {
        flock(fd_, LOCK_UN);
    }

    close(fd_);
    locked_ = false;
    return 0;
}

FileLocker::~FileLocker()
{
    unlock();
}

