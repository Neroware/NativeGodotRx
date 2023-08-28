#ifndef RX_EXCEPTION_EXCEPTION_H
#define RX_EXCEPTION_EXCEPTION_H

#include <exception>
#include <string>
#include <functional>

namespace rx {
namespace exception {

class rx_exception : public std::exception
{
private:
    std::string _what;
    std::string _type;

public:
    rx_exception(const std::string& what_ = "", const std::string& type = "RxError") : _what(what_), _type(type) {}

    virtual const char* what() const noexcept override {
        return _what.c_str();
    }
    virtual const char* type() const noexcept {
        return this->_type.c_str();
    }
};

class NotImplementedException : public rx_exception
{
public:
    virtual const char* what() const noexcept override {
        return "Function not implemented!"; 
    }
    virtual const char* type() const noexcept override {
        return "NotImplementedError";
    }
};

class DisposedException : public rx_exception
{
public:
    virtual const char* what() const noexcept override {
        return "Tried to access disposed element!"; 
    }
    virtual const char* type() const noexcept override {
        return "DisposedError";
    }
};

class WouldBlockException : public rx_exception
{
public:
    virtual const char* what() const noexcept override {
        return "Tried to schedule blocking work!"; 
    }
    virtual const char* type() const noexcept override {
        return "WouldBlockError";
    }
};

class BadArgumentException : public rx_exception
{
private:
    std::string _what;

public:
    BadArgumentException(const std::string& what_) : _what(what_) {}

    virtual const char* what() const noexcept override {
        return _what.c_str();
    }
    virtual const char* type() const noexcept override {
        return "BadArgumentError";
    }
};

class BadCastException : public rx_exception
{

public:
    BadCastException(){}

    virtual const char* what() const noexcept override {
        return "Could not perform cast!";
    }
    virtual const char* type() const noexcept override {
        return "BadCastException";
    }
};

class SequenceContainsNoElementsException : public rx_exception
{

public:
    SequenceContainsNoElementsException(){}

    virtual const char* what() const noexcept override {
        return "The provided sequence does not contain an element!";
    }
    virtual const char* type() const noexcept override {
        return "SequenceContainsNoElementsException";
    }
};

} // END namespace exception

typedef std::function<bool(const std::exception_ptr& e)> handler_t;

} // END namepsace rx

#endif // RX_EXCEPTION_EXCEPTION_H