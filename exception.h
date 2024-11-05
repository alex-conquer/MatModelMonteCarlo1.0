#pragma once
#include <exception>
#include <string>

class CoreExceptions: public std::exception {
public:
    explicit CoreExceptions(const std::string& m_msg): m_msg(m_msg) {}
    const char* what() const noexcept override {
        return m_msg.c_str();
    }

private:
    std::string m_msg;
};

class InvalidDisplayInput: public CoreExceptions {
public:
    InvalidDisplayInput()
        : CoreExceptions("Input error. Make sure that numeric values are entered.") {}
};

class IncorrectFigure: public CoreExceptions {
public:
    IncorrectFigure()
        : CoreExceptions("The shape is incorrectly constructed, check what you are missing") {}
};
