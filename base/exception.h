#ifndef EXCEPTION_H
#define EXCEPTION_H
#include <exception>
#include <string>
namespace yang{
class Exception {
public:
    Exception(const std::string& msg) : message_(msg) {}
    const std::string& what() const{
        return message_;
    }
private:
    std::string message_;
};
}
#endif