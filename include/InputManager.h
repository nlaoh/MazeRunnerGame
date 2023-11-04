#ifndef ASSIGN_INPUT_MANAGER_H
#define ASSIGN_INPUT_MANAGER_H

#include <iostream>
#include <limits>
#include <mcpp/mcpp.h>

//Macro for commonly throwing an end of file input, to reduce typos
#define CIN_THROW_EOF_ERROR \
    if (std::cin.eof()) { \
        throw end_of_file_error(); \
    }

//Macro for commonly clearing cin input and/or error flags
#define CIN_CLEAR \
    std::cin.clear(); \
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

//Possible input states
enum InputState {
    INPUT_X,
    INPUT_Y,
    INPUT_Z,
    INPUT_XLEN,
    INPUT_ZLEN,
    INPUT_XLEN_AND_ZLEN,
    INPUT_EXIT
};

//subclass for throwing and catching end of file errors
class end_of_file_error : public std::exception {
    public:
        end_of_file_error(const char* errorMsg = 
            "End of file read from input, exiting program.") : 
            std::exception(), errorMsg(errorMsg) {}
        /*
         * noexcept means that the function is not allowed to throw 
         * an exception, not really needed
         */
        const char* what() const noexcept override {
            return this->errorMsg.c_str();
        }
    private:
        std::string errorMsg; 
};

class invalid_input_error : public std::exception {
public:
    invalid_input_error(const char* message) : message(message) {}

    const char* what() const noexcept override {
        return this->message.c_str();
    }

private:
    std::string message;
};

void readMainMenuInput(int& userInput);

void readMazeProperties(int& baseX, int& baseY, int& baseZ, 
    int& xlen, int& zlen);

#endif