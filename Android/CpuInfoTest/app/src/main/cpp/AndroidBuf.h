//By Navy Zhang 710605420@qq.com 
//
// Created by xd on 7/23/2024.
//

#ifndef CPUINFOTEST_ANDROIDBUF_H
#define CPUINFOTEST_ANDROIDBUF_H

#include <iostream>
#include <streambuf>
#include <android/log.h>

class AndroidBuf : public std::streambuf {
    enum {
        BUFFER_SIZE = 255,
    };

public:
    AndroidBuf();

    ~AndroidBuf();

protected:
    virtual int_type overflow(int_type c) {
        if (c != EOF) {
            *pptr() = c;
            pbump(1);
        }
        flush_buffer();
        return c;
    }

    virtual int sync() {
        flush_buffer();
        return 0;
    }

private:
    int flush_buffer();

private:
    char buffer_[BUFFER_SIZE + 1];
};

#endif //CPUINFOTEST_ANDROIDBUF_H
