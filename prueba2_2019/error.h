#ifndef _ERROR_H
#define _ERROR_H

#include <sstream>
#include <string>

struct TError {
    TError() {}
    TError(const std::string& msg): ss(msg) {}

    std::string getMessage() { return ss.str(); }

    template <typename T>
    friend TError& operator << (TError&, const T &);
private:
    std::ostringstream ss;
};

template <typename T>
TError& operator << (TError &err, const T &val) {
    err.ss << val;
    return err;
}

#endif