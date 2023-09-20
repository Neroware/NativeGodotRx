#ifndef RX_EXCEPTION_EXCEPTION_H
#define RX_EXCEPTION_EXCEPTION_H

#include <exception>
#include <string>
#include <functional>

namespace rx {
namespace exception {

class rx_exception : public std::exception
{
protected:
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
    NotImplementedException(const std::string& what_ = "Function not implemented") 
        : rx_exception(what_, "NotImplementedError") {}
};

class DisposedException : public rx_exception
{
public:
    DisposedException(const std::string& what_ = "Tried to access disposed element") 
        : rx_exception(what_, "DisposedError") {}
};

class WouldBlockException : public rx_exception
{
public:
    WouldBlockException(const std::string& what_ = "Tried to schedule blocking work") 
        : rx_exception(what_, "WouldBlockError") {}
};

class BadArgumentException : public rx_exception
{
public:
    BadArgumentException(const std::string& what_) 
        : rx_exception(what_, "BadArgumentError") {}
};

class BadCastException : public rx_exception
{
public:
    BadCastException(const std::string& what_ = "Could not perform cast") 
        : rx_exception(what_, "BadCastError") {}
};

class SequenceContainsNoElementsException : public rx_exception
{
public:
    SequenceContainsNoElementsException(const std::string& what_ = "The provided sequence does not contain an element") 
        : rx_exception(what_, "SequenceContainsNoElementsError") {}
};

class ArgumentOutOfRangeException : public rx_exception
{
public:
    ArgumentOutOfRangeException(const std::string& what_ = "The provided argument is out of range") 
        : rx_exception(what_, "ArgumentOutOfRangeError") {}
};

class HttpRequestFailedException : public rx_exception
{
public:
    HttpRequestFailedException(const std::string& what_ = "A HTTP request has failed") 
        : rx_exception(what_, "HttpRequestFailedException") {}
};

} // END namespace exception
} // END namepsace rx

#endif // RX_EXCEPTION_EXCEPTION_H