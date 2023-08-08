#ifndef RX_EXCEPTION_EXCEPTION_H
#define RX_EXCEPTION_EXCEPTION_H

#include <exception>
#include <string>
#include <functional>

namespace rx {
namespace exception {

class Exception : public std::exception
{
private:
    std::string _what;

public:
    Exception(const std::string& what_ = "") : _what(what_) {}

    virtual const char* what() const noexcept override {
        return _what.c_str();
    }
};

class NotImplementedException : public std::exception
{
public:
    virtual const char* what() const noexcept override {
        return "Function not implemented!"; 
    }
};

class DisposedException : public std::exception
{
public:
    virtual const char* what() const noexcept override {
        return "Tried to access disposed element!"; 
    }
};

class WouldBlockException : public std::exception
{
public:
    virtual const char* what() const noexcept override {
        return "Tried to schedule blocking work!"; 
    }
};

class BadArgumentException : public std::exception
{
private:
    std::string _what;

public:
    BadArgumentException(const std::string& what_) : _what(what_) {}

    virtual const char* what() const noexcept override {
        std::string res = "A function argument has a bad value: " + _what;
        return res.c_str();
    }
};

} // END namespace exception

typedef std::function<bool(const std::exception& e)> handler_t;

} // END namepsace rx

#endif // RX_EXCEPTION_EXCEPTION_H