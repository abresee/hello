#ifndef BADFLOWEXCEPTION_H
#define BADFLOWEXCEPTION_H 
#include <exception>
#include <string>
class BadFlowException : public std::exception {
    std::string message_;
public:
    BadFlowException(const char * cstr);
    BadFlowException(const std::string& str);
    virtual const char * what();
};

#endif /* BADFLOWEXCEPTION_H */
