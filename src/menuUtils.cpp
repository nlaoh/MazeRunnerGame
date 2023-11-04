
#include "../include/menuUtils.h"
#include "../include/InputManager.h"
#include "../include/Agent.h"

/*
 * 🚨🚨The following functions handle menu functionality 
 * for each state in the terminal
 */

/*
 * Controls main menu state and handles user input, requires maze object to be passed
*/
static bool enhanced= false;
void mainMenuUserInput(States& curState, Maze& maze) {
    //set to -1 to avoid undefined behaviour
    int userInput = -1;
    //try reading maze
    std::cin >> userInput;
    //throw end of file error
    CIN_THROW_EOF_ERROR
    //ignore non-int input 
    CIN_CLEAR

    // ⚙️Generate maze
    if (userInput == 1) { 
        curState = ST_GetMaze;
    } 
    // ⚙️Build maze, 
    else if (userInput == 2) { 
        if (maze.getLength() > 0) {
            std::cout << "Building maze in minecraft..." << std::endl;
            //If a previous maze was already created, reset previous maze
            if (maze.inMinecraft == true && maze.prevEnhancedMaze == false) {
                std::cout << "Resetting previous maze" << std::endl;
                maze.resetPrevMinecraftMaze();
                //Reset previous maze foundations
                std::cout << "Resetting previous maze foundation" << std::endl;
                maze.ResetMazeFoundation();
            }
            else if (maze.inMinecraft == true && 
                maze.prevEnhancedMaze == true) {
                std::cout << "Resetting previous maze" << std::endl;
                maze.resetPrevMinecraftMazeEnhanced();
            }

            if (maze.newEnhancedMaze == false) {
                //Set new maze foundations
                std::cout << "Setting maze foundation" << std::endl;
                maze.setMazeFoundation();
                //Create maze foundation
                std::cout << "Creating maze foundation" << std::endl;
                maze.createMazeFoundation();
                std::cout << "Creating minecraft maze" << std::endl;
                maze.createMinecraftMaze();
                //set maze.inMinecraft to true as there is a maze in minecraft
                maze.inMinecraft = true;
            }
            else {
                std::cout << "Creating minecraft maze" << std::endl;
                maze.createMinecraftMazeEnhanced();
                maze.inMinecraft = true;
            }

            /* 
             * Save built maze as prevstructure 
             * if a new maze was generated in generate maze menu.
             */
            if (maze.newMaze == true) {
                maze.setPrevStructure();
                std::cout << "Prev structure saved" << std::endl;
                maze.newMaze = false;
            }
        } 
        else {
            throw std::runtime_error(
                "Please initialise a Maze first, using 'Generate Maze'.");
        }
    } 
    else if (userInput == 3) { 
        if(maze.inMinecraft == true) {
            curState = ST_SolveMaze;
        }
        else {
            throw std::runtime_error(
                "Please build a Maze first, using 'Build Maze'.");
        }
    } 
    // ⚙️Show team information
    else if (userInput == 4) { 
        curState = ST_Creators;
    } 
    // ⚙️Exit program
    else if (userInput == 5) { 
        curState = ST_Exit;
    } 
    else {
        throw std::runtime_error(
            "Input Error: Enter a number between 1 and 5 ....");
    }
    return;
}

/**
 * Controls generate maze state and handles user input, requires maze object to be passed
*/
void getMazeUserInput(States& curState, Maze& maze, bool enhancementsMode) {
    //set to -1 to avoid undefined behaviour
    int userInput = -1;
    std::cin >> userInput;
    //basic userInput checks
    CIN_THROW_EOF_ERROR
    CIN_CLEAR

    //The following are menu options
    // ⚙️Read maze from terminal
    if (userInput == 1) { 
        std::cout << "Enter the basePoint of the maze:" << std::endl;
        int baseX = 0, baseY = 0, baseZ = 0;
        int int_mazeLength = 0, int_mazeWidth = 0;
        //read maze properties
        try {
            readMazeProperties(baseX, baseY, baseZ, 
                int_mazeLength, int_mazeWidth);
        } 
        catch (const end_of_file_error&) {
            throw;
        }
        
        //convert int type to unsigned int type for maze class
        unsigned int mazeLength = static_cast<unsigned int>(int_mazeLength);
        unsigned int mazeWidth = static_cast<unsigned int>(int_mazeWidth);

        // given length and width are greater than 1 and both are odd numbers
        std::cout << "Length and width set to:" << std::endl;
        std::cout << mazeLength << " " << mazeWidth << std::endl;

        mcpp::Coordinate basePoint = mcpp::Coordinate(baseX, baseY, baseZ);
        maze.setBasePoint(basePoint);
        maze.setDimensions(mazeLength, mazeWidth);
        try {
            maze.setInitialStructure();
            maze.readInUserMaze();
            std::cout << "Maze read successfully" << std::endl;
            maze.printMaze(); 
        } 
        catch(const end_of_file_error&) {
            throw;
        }
        catch(const std::runtime_error&) {
            /* 
             * rethrow; error message depends on error encountered 
             * after reading maze
             */
            maze.newMaze = false;
            maze.removeStructure();
            throw;
        } 
    } 
    // ⚙️Generate random maze (odd numbers)
    else if (userInput == 2) { 
        std::cout << "Enter the basePoint of the maze:" << std::endl;
        int baseX = 0, baseY = 0, baseZ = 0;
        int int_mazeLength = 0, int_mazeWidth = 0;
        try {
            readMazeProperties(baseX, baseY, baseZ, 
                int_mazeLength, int_mazeWidth);
        } 
        catch (const end_of_file_error&) {
            throw;
        }
        //convert int type to unsigned int type for maze class
        unsigned int mazeLength = static_cast<unsigned int>(int_mazeLength);
        unsigned int mazeWidth = static_cast<unsigned int>(int_mazeWidth);
        // if length and width are greater than 1 and both are odd numbers
        std::cout << "Length and width set to:" << std::endl;
        std::cout << mazeLength << " " << mazeWidth << std::endl;

        std::cout << "Maze generated successfully" << std::endl;
        enhanced=false;
        mcpp::Coordinate basePoint = mcpp::Coordinate(baseX, baseY, baseZ);
        maze.setBasePoint(basePoint);
        maze.setDimensions(mazeLength, mazeWidth);
        maze.setInitialStructure();
        maze.generateRandomMaze(enhanced);
        maze.newEnhancedMaze = false;
        maze.printMaze();    
    }
    // ⚙️Back
    else if (userInput == 3 && !enhancementsMode) { 
        std::cout << "Returning to menu..." << std::endl;
        curState = ST_Main;
    }
    else if (userInput==3 && enhancementsMode){
        std::cout << "Enter the basePoint of the maze:" << std::endl;
        int baseX = 0, baseY = 0, baseZ = 0;
        int int_mazeLength = 0, int_mazeWidth = 0;
        try {
            readMazeProperties(baseX, baseY, baseZ, 
                int_mazeLength, int_mazeWidth);
        } 
        catch (const end_of_file_error&) {
            throw;
        }
        //convert int type to unsigned int type for maze class
        unsigned int mazeLength = static_cast<unsigned int>(int_mazeLength);
        unsigned int mazeWidth = static_cast<unsigned int>(int_mazeWidth);
        // if length and width are greater than 1 and both are odd numbers
        std::cout << "Length and width set to:" << std::endl;
        std::cout << mazeLength << " " << mazeWidth << std::endl;

        std::cout << "Reading terrain" << std::endl;
        mcpp::Coordinate basePoint = mcpp::Coordinate(baseX, baseY, baseZ);
        enhanced=true;
        maze.setBasePoint(basePoint);
        maze.setDimensions(mazeLength, mazeWidth);
        maze.getAreaHeight();
        maze.findMaxReachable();
        maze.setInitialStructureEnhanced();
        maze.generateRandomMaze(enhanced);
        maze.newEnhancedMaze = true;
        std::cout << "Maze generated" << std::endl;
        maze.printMaze(); 
    } 
    else if (userInput==4 && enhancementsMode){
        std::cout << "Returning to menu..." << std::endl;
        curState = ST_Main;
    }
    else { 
        if (!enhancementsMode){
        throw std::runtime_error(
            "Input Error: Enter a number between 1 and 3 ....");
    }
        else {
        throw std::runtime_error(
            "Input Error: Enter a number between 1 and 4 ....");
        }
    }
    return;
}

/**
 * Controls solve maze state and handles user input, requires maze object to be passed
*/
void solveMazeUserInput(States& curState, Maze &myMaze, 
    bool testingMode, bool enhancementsMode) {
    int userInput; 
    std::cin >> userInput;
    CIN_THROW_EOF_ERROR
    CIN_CLEAR
    static bool doneSet = false;
    bool playerSet=false;
    mcpp::Coordinate playerLoc = myMaze.getBasePoint();
    int mazeLength = myMaze.getLength();
    int mazeWidth = myMaze.getWidth();
    char** mazeArray = myMaze.getStructure();
    if (userInput == 1 && !testingMode) { 
        std::cout << "Solving maze manually"<<std::endl;
        while (!playerSet){
            int myRandX = rand()%(mazeLength);
            int myRandZ = rand()%(mazeWidth);
            int startX = myRandX;
            int startY = 0;
            int startZ = myRandZ;

            mcpp::Coordinate start = mcpp::Coordinate(startX, startY, startZ);
            mcpp::MinecraftConnection mc;
            playerLoc = playerLoc + start;
            if (mazeArray[myRandX][myRandZ]=='.'){
                playerLoc.y = mc.getHeight(playerLoc.x, playerLoc.z) + 1;
                mc.setPlayerPosition(playerLoc);
                doneSet=true; 
                playerSet=true;
                std::cout<< "Maze ready to Solve..."  <<std::endl; 
            }
            else{
            playerLoc = myMaze.getBasePoint();
            }
        }
    } 
    else if (userInput == 1 && testingMode) {
        for (int x = mazeLength - 1; x>=0 && !playerSet; --x) {
            for (int z = mazeWidth - 1; z>=0 && !playerSet ; --z) {
                playerLoc = myMaze.getBasePoint();
                mcpp::Coordinate lowerRight = mcpp::Coordinate(x, 0, z);
                playerLoc = playerLoc + lowerRight;
                mcpp::MinecraftConnection mc;
                if (mazeArray[x][z] == '.') {
                    playerLoc.y = mc.getHeight(playerLoc.x, playerLoc.z) + 1;
                    mc.setPlayerPosition(playerLoc);
                    doneSet=true; 
                    playerSet=true;
                    std::cout<< "Maze ready to Solve..."  <<std::endl;
                }
               
            }
        }
        playerSet=false;
    }
    // ⚙️Show escape route (wall follower algo, puts carpet on the floor)
    else if (userInput == 2 && doneSet) { 
        Agent myAgent(playerLoc);
        bool insideMaze = isInsideMaze(myMaze);
        if (insideMaze){
        myAgent.wallFollow(enhanced);
        myAgent.showPath(false);}
        else{
            std::cout<<"Please stay inside maze..."<<std::endl;
        }
    } 
    else if (userInput==2 && !doneSet){
        std::cout<< "Initialize player using Solve manually."<<std::endl;
    }
    // ⚙️Back
    else if (userInput == 3 && !enhancementsMode){
        std::cout << "Returning to menu..." << std::endl;
        curState=ST_Main;
    }
    else if (userInput == 3 && doneSet && enhancementsMode){
        bool insideMaze = isInsideMaze(myMaze);
        if (insideMaze){
        Agent myAgent(playerLoc);
        myAgent.BFS(enhanced);
        myAgent.showPath(true);
        }
        else{
            std::cout<<"Please stay inside maze..."<<std::endl;
        }
    }
    else if (userInput == 3 && !doneSet && enhancementsMode){
        std::cout<< "Initialize player using Solve manually."<<std::endl;
    }
    else if (userInput == 4 && enhancementsMode){
        std::cout << "Returning to menu..." << std::endl;
        curState=ST_Main;
    }
    else {
        if (!enhancementsMode) {
            throw std::runtime_error(
                "Input Error: Enter a number between 1 and 3 ....");
        }
        else {
            throw std::runtime_error(
                "Input Error: Enter a number between 1 and 4 ....");
        }
    }
    return;
}

bool isInsideMaze(Maze &myMaze){
    mcpp::Coordinate playerLoc = myMaze.getBasePoint();
    int mazeLength = myMaze.getLength();
    int mazeWidth = myMaze.getWidth();
    mcpp::MinecraftConnection mc;
    mcpp::Coordinate currLoc = mc.getPlayerPosition();
    bool isInsideMaze = false;
    if (enhanced){
        int mazeHeight = myMaze.getMaxReachableHeight();
        if (currLoc.x -playerLoc.x < mazeLength && 
            currLoc.x - playerLoc.x >= 0 && 
            mazeHeight + 3 - currLoc.y >= 0 && 
            currLoc.z-playerLoc.z <  mazeWidth 
            && currLoc.z - playerLoc.z >= 0){
            isInsideMaze = true;
        }
        else{
            isInsideMaze = false;
        }
    }
    else if (!enhanced){
        if (currLoc.x -playerLoc.x < mazeLength && 
            currLoc.x - playerLoc.x >= 0 && 
            (playerLoc.y + 3 - currLoc.y >= 0 || 
            currLoc.y - playerLoc.y < 0) && 
            currLoc.z-playerLoc.z <  mazeWidth && 
            currLoc.z - playerLoc.z >= 0){
            isInsideMaze = true;
        }
        else{
            isInsideMaze = false;
        
        }
    }
    return isInsideMaze;
}



/*
🚨🚨The following functions are for menu UI. 
*/
void printStartText(void){
    std::cout << std::endl;
    std::cout << "Welcome to MineCraft MazeRunner!" << std::endl;
    std::cout << "--------------------------------" << std::endl;
}

void printMainMenu(void){
    std::cout << std::endl;
    std::cout << "------------- MAIN MENU -------------" << std::endl;
    std::cout << "1) Generate Maze" << std::endl;
    std::cout << "2) Build Maze in MineCraft" << std::endl;
    std::cout << "3) Solve Maze" << std::endl;
    std::cout << "4) Show Team Information" << std::endl;
    std::cout << "5) Exit" << std::endl;
    std::cout << std::endl;
    std::cout << "Enter Menu item to continue:" << std::endl;
}

void printGenerateMazeMenu(bool enhancementsMode){
    std::cout << std::endl;
    std::cout << "------------- GENERATE MAZE -------------" << std::endl;
    std::cout << "1) Read Maze from terminal" << std::endl;
    std::cout << "2) Generate Random Maze" << std::endl;
    if (enhancementsMode) {
        std::cout << 
            "3) Generate Random Maze without modifying terrain (Enhancements)" 
            << std::endl;
        std::cout << "4) Back" << std::endl;
    }
    else {
        std::cout << "3) Back" << std::endl;
    }
    std::cout << std::endl;
    std::cout << "Enter Menu item to continue:" << std::endl;
}

void printSolveMazeMenu(bool enhancementsMode){
    std::cout << std::endl;
    std::cout << "------------- SOLVE MAZE -------------" << std::endl;
    std::cout << "1) Solve Manually" << std::endl;
    std::cout << "2) Show Escape Route" << std::endl;
    if (enhancementsMode) {
        std::cout << "3) Show Shortest Path (Enhancements)" << std::endl;
        std::cout << "4) Back" << std::endl;
    }
    else {
        std::cout << "3) Back" << std::endl;
    }
    std::cout << std::endl;
    std::cout << "Enter Menu item to continue:" << std::endl;
}



void printTeamInfo(void){
    std::cout << std::endl;
    std::cout << "Team members:" << std::endl;

    std::cout << 
        "\t [1] Owen Harris (s9809016@student.rmit.edu.au)" << std::endl;
    std::cout << 
        "\t [2] Boon Jia Xian (s4011455@student.rmit.edu.au)" << std::endl;
    std::cout << 
        "\t [3] Natchanon Laoharawee (s4006840@student.rmit.edu.au)" << 
        std::endl;
    std::cout << 
        "\t [4] Noah Bloomfield (s4012901@student.rmit.edu.au)" << std::endl;


    std::cout << std::endl;
}

void printExitMessage(void){
    std::cout << std::endl;
    std::cout << "The End!" << std::endl;
    std::cout << std::endl;
}

std::runtime_error printMenuErrorMessage(){
    return std::runtime_error("Invalid menu index, please enter another:");
}
