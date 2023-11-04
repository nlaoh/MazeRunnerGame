#ifndef MENU_UTILS_H
#define MENU_UTILS_H

#include "Maze.h"
#include <iostream>

//possible modes for running the program
#define NORMAL_MODE 0
#define TESTING_MODE 1
//Possible states for main menu
enum States{
    ST_Main,
    ST_GetMaze,
    ST_SolveMaze,
    ST_Creators,
    ST_Exit
};

/*
 * 🚨🚨The following functions handle menu functionality 
 * for each state in the terminal
 */
void mainMenuUserInput(States& curState, Maze& maze);
void getMazeUserInput(States& curState, Maze& maze, bool enhancementsMode);
void solveMazeUserInput(States& curState, Maze &myMaze, 
    bool testingMode, bool enhancementsMode);
bool isInsideMaze(Maze &myMaze);


/*
 * 🚨🚨The following functions are for output.
 */
void printStartText(void);
void printMainMenu(void);
void printGenerateMazeMenu(bool enhancementsMode);
void printSolveMazeMenu(bool enhancementsMode);
void printTeamInfo(void);
void printExitMessage(void);
std::runtime_error printMenuErrorMessage();

#endif