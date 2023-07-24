#ifndef RX_EXCEPTION_EXCEPTION_H
#define RX_EXCEPTION_EXCEPTION_H

#include <exception>
#include <string>

namespace rx::exception {

class Exception : public std::exception
{
private:
    std::string _what;

public:
    Exception(const std::string& what_ = "") : _what(what_) {}

    virtual char const* what() {
        return _what.c_str();
    }
};

class NotImplementedException : public std::exception
{
public:
    virtual char const* what() { 
        return "Function not implemented!"; 
    }
};

class DisposedException : public std::exception
{
public:
    virtual char const* what() { 
        return "Tried to access disposed element!"; 
    }
};

class WouldBlockException : public std::exception
{
public:
    virtual char const* what() { 
        return "Tried to schedule blocking work!"; 
    }
};

class BadArgumentException : public std::exception
{
private:
    std::string _what;

public:
    BadArgumentException(const std::string& what_) : _what(what_) {}

    virtual char const* what() {
        std::string res = "A function argument has a bad value: " + _what;
        return res.c_str();
    }
};

}; // END namepsace rx::exception

#endif // RX_EXCEPTION_EXCEPTION_H