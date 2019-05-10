#ifndef _STR_UTIL_H_
#define _STR_UTIL_H_

#include <string>

// trim from left
inline std::string& ltrim(std::string& s, const char* t = " \t\n\r\f\v")
{
    s.erase(0, s.find_first_not_of(t));
    return s;
}

// trim from right
inline std::string& rtrim(std::string& s, const char* t = " \t\n\r\f\v")
{
    s.erase(s.find_last_not_of(t) + 1);
    return s;
}

// trim from left & right
inline std::string& trim(std::string& s, const char* t = " \t\n\r\f\v")
{
    return ltrim(rtrim(s, t), t);
}

template<class Iter>
Iter strsplit(const std::string &s, const std::string &delim, Iter out)
{
    if (delim.empty()) {
        *out++ = s;
        return out;
    }
    size_t a = 0, b = s.find(delim);
    for ( ; b != std::string::npos;
          a = b + delim.length(), b = s.find(delim, a))
    {
        *out++ = std::move(s.substr(a, b - a));
    }
    *out++ = std::move(s.substr(a, s.length() - a));
    return out;
}

#endif
