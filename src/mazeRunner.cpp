#include <iostream>
#include <cstring>
#include <mcpp/mcpp.h>

#include "../include/menuUtils.h"
#include "../include/InputManager.h"
 
//This file handles menu logic
int main(int argc, char** argv) {
    bool testingMode = false;
    bool enhancementsMode = false;
    // Checks if program was run with argument "-testmode"
    if ((argc == 2 && strcmp(argv[1], "-testmode") == 0) || 
        (argc == 3 && (strcmp(argv[1], "-testmode") == 0 ||
        strcmp(argv[2], "-testmode") == 0))) {
        testingMode = true;
    }
    else {
        // Normal mode
        srand(time(NULL)); 
    }

    if ((argc == 2 && strcmp(argv[1], "-enhancements") == 0) || 
        (argc == 3 && (strcmp(argv[1], "-enhancements")  == 0 ||
        strcmp(argv[2], "-enhancements")  == 0))) {
        enhancementsMode = true;
    }
    
    mcpp::MinecraftConnection mc; 
    mc.doCommand("time set day"); 

    // initial arguments for maze_foundation
    mcpp::Coordinate basepoint(0, 0, 0); 
    int xlen = 0;
    int zlen = 0;

    //Maze object
    Maze maze = Maze(basepoint, static_cast<unsigned int>(xlen), 
        static_cast<unsigned int>(zlen), testingMode, enhancementsMode);

    //Always begin in main
    States curState = ST_Main; 

    //State machine for menu
    printStartText();
    try {
        while (curState != ST_Exit) { 
            //Try getting input from user
            try {
                if (curState == ST_Main) {
                    printMainMenu();
                    mainMenuUserInput(curState, maze);
                } else if (curState == ST_GetMaze) {
                    printGenerateMazeMenu(enhancementsMode);
                    getMazeUserInput(curState, maze, enhancementsMode);
                } else if (curState == ST_SolveMaze) {
                    printSolveMazeMenu(enhancementsMode);
                    solveMazeUserInput(curState, maze, testingMode, 
                        enhancementsMode);
                } else if (curState == ST_Creators) {
                    printTeamInfo();
                    curState = ST_Main;
                }
            } 
            /* 
             * Incorrect input is thrown and caught here, 
             * resume menu loop at curState 
             */
            catch(const std::runtime_error& e) {
                std::cout << e.what() << std::endl;
            }
            catch (const end_of_file_error&) {
                //rethrow to exit loop
                throw;
            }
        }
    } 
    //catch when end of file is input
    catch (const end_of_file_error& e) {
        std::cout << std::endl << e.what() << std::endl;
    }

    std::cout << std::endl << "Exiting..." << std::endl;

    //When exiting, reset minecraft world to normal
    if (maze.inMinecraft == true) {
        std::cout << std::endl 
            << "Returning minecraft world to normal and "
            << "deleting generated mazes..." 
            << std::endl;
        std::cout << "Please wait 1-3 minutes." <<  std::endl;

        if (maze.newEnhancedMaze == false) {
            maze.resetMinecraftMaze();
            maze.ResetMazeFoundation();
        }
        else {
            maze.resetMinecraftMazeEnhanced();
        }
    }

    //Error message
    printExitMessage();
    return 0;
}