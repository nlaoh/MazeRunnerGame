#include "../include/InputManager.h"
#include <string>

/* 
 * Currently a small function and not used, 
 * but could be used for more edge case checking
 */
void readMainMenuInput(int& userInput) {
    std::cin >> userInput;
    //throw end of file error
    CIN_THROW_EOF_ERROR
    //ignore non-int input
    CIN_CLEAR
}

void readMazeProperties(int& baseX, int& baseY, int& baseZ, 
    int& xlen, int& zlen) {
    mcpp::MinecraftConnection mc;
    InputState curState = INPUT_X;
    std::string temp = "";
    //The while loop is used to follow through each state in order
    while (curState != INPUT_EXIT) {
        //try getting input
        try {
            //need separate fail checks as each coordinate can fail
            if (curState == INPUT_X) {
                std::cin >> temp;
                CIN_THROW_EOF_ERROR
                if (std::cin.fail()){
                    CIN_CLEAR
                    throw invalid_input_error(
                        "Invalid input for X coordinate, "
                        "try entering an integer."
                        );
                }
                try {
                    baseX = std::stoi(temp);
                    curState = INPUT_Y;
                }
                //catch error thrown by stoi when encounter integer overflow
                catch (const std::out_of_range& e) {
                    CIN_CLEAR
                    throw invalid_input_error(""
                        "Integer is too large or small for X coordinate, "
                            "try entering a different integer.");
                }
                //catch error thrown by stoi when string is not a valid integer
                catch (const std::invalid_argument& e) {
                    CIN_CLEAR
                    throw invalid_input_error(
                        "Invalid input for X coordinate, "
                        "try entering an integer."
                        );
                }
            }
            else if (curState == INPUT_Y) {
                std::cin >> temp;
                CIN_THROW_EOF_ERROR
                if (std::cin.fail()){
                    CIN_CLEAR
                    throw invalid_input_error(
                        "Invalid input for Y coordinate, "
                        "try entering an integer."
                        );
                }
                try {
                    baseY = std::stoi(temp);
                    curState = INPUT_Z;
                }
                //catch error thrown by stoi when encountern integer overflow
                catch (const std::out_of_range& e) {
                    CIN_CLEAR
                    throw invalid_input_error(""
                        "Integer is too large or small for Y coordinate, "
                            "try entering a different integer.");
                }
                //catch error thrown by stoi when string is not a valid integer
                catch (const std::invalid_argument& e) {
                    CIN_CLEAR
                    throw invalid_input_error(
                        "Invalid input for Y coordinate, "
                        "try entering an integer."
                        );
                }
            }
            else if (curState == INPUT_Z) {
                std::cin >> temp;
                CIN_THROW_EOF_ERROR
                if (std::cin.fail()){
                    CIN_CLEAR
                    throw invalid_input_error(
                        "Invalid input for Z coordinate, "
                        "try entering an integer."
                        );
                }
                try {
                    baseZ = std::stoi(temp);
                }
                //catch error thrown by stoi when encounter integer overflow
                catch (const std::out_of_range& e) {
                    CIN_CLEAR
                    throw invalid_input_error(""
                        "Integer is too large or small for Z coordinate, "
                            "try entering a different integer.");
                }
                //catch error thrown by stoi when string is not a valid integer
                catch (const std::invalid_argument& e) {
                    CIN_CLEAR
                    throw invalid_input_error(
                        "Invalid input for Z coordinate, "
                        "try entering an integer."
                        );
                }
                //Check basePoint.y is y + 1 in minecraft world. 
                int yHeight = mc.getHeight(baseX, baseZ);
                if (baseY != yHeight + 1) {
                    CIN_CLEAR
                    curState = INPUT_X;
                    std::string e = "Invalid basePoint, "
                        "Y coordinate must be 1 unit above ground.\n"
                        "The ideal Y coordinate for this location would be "
                        + std::to_string(mc.getHeight(baseX, baseZ) + 1) + ".\n"
                        "Please re-enter your basepoint:";
                    throw invalid_input_error(
                        e.c_str()
                    );   
                }
                //clear any remaining input in buffer
                CIN_CLEAR
                curState = INPUT_XLEN_AND_ZLEN;
            }
            /* 
             * 🚨🚨 Xlen and zlen are read together to try to adhere 
             * to assignment requirements
             */
            else if (curState == INPUT_XLEN_AND_ZLEN) {
                std::cout << "Enter the length and width of the maze:" 
                    << std::endl;
                //Get xlen
                std::cin >> temp;
                CIN_THROW_EOF_ERROR
                //if fail, negative int, 0, or odd
                if (std::cin.fail()) {
                    CIN_CLEAR
                    throw invalid_input_error(                    
                        "Invalid input for length, try entering only "
                        "integer values, and odd numbers greater than 1 "
                        "and less than or equal to 39."
                        );
                }
                try {
                    xlen = std::stoi(temp);
                    if (xlen < 3 || xlen % 2 == 0 || xlen > 39) {
                        CIN_CLEAR
                        throw invalid_input_error(                    
                            "Invalid input for length, try entering only "
                            "integer values, and odd numbers greater than 1 "
                            "and less than or equal to 39."
                            );
                    }
                }
                //catch error thrown by stoi when encounter integer overflow
                catch (const std::out_of_range& e) {
                    CIN_CLEAR
                    throw invalid_input_error(""
                        "Integer is too large or small for length, "
                        "try entering a different integer."
                        );
                }
                //catch error thrown by stoi when string is not a valid integer
                catch (const std::invalid_argument& e) {
                    CIN_CLEAR
                    throw invalid_input_error(                    
                        "Invalid input for length, try entering only "
                        "integer values, and odd numbers greater than 1 "
                        "and less than or equal to 39."
                        );
                }
                //Get zlen
                std::cin >> temp;
                //if fail, negative int, 0, or odd
                if (std::cin.fail()) {
                    CIN_CLEAR
                    throw invalid_input_error(                    
                        "Invalid input for width, try entering only "
                        "integer values, and odd numbers greater than 1 "
                        "and less than or equal to 39."
                        );
                }
                try {
                    zlen = std::stoi(temp);
                    if (zlen < 3 || zlen % 2 == 0 || zlen > 39) {
                        CIN_CLEAR
                        throw invalid_input_error(                    
                            "Invalid input for width, try entering only "
                            "integer values, and odd numbers greater than 1 "
                            "and less than or equal to 39."
                            );
                    }
                }
                //catch error thrown by stoi when encounter integer overflow
                catch (const std::out_of_range& e) {
                    CIN_CLEAR
                    throw invalid_input_error(""
                        "Integer is too large or small for width, "
                        "try entering a different integer."
                        );
                }
                //catch error thrown by stoi when string is not a valid integer
                catch (const std::invalid_argument& e) {
                    CIN_CLEAR
                    throw invalid_input_error(                    
                        "Invalid input for width, try entering only "
                        "integer values, and odd numbers greater than 1 "
                        "and less than or equal to 39."
                        );
                }
                curState = INPUT_EXIT;
            }
        } 
        //catch input errors
        catch(invalid_input_error& e) {
            std::cout << e.what() << std::endl;
        }
    }
    return;
}
