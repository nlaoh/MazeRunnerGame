#include "../include/Maze.h"
#include "../include/InputManager.h"

#include <iostream>
#include <string>
#include <random>
#include <vector>
#include <chrono>
#include <thread>

using std::vector;
/**
 * Maze constructor, make sure that xlen and zlen passed is an unsigned int 
 * and the respective modes if the program handles it.
*/
Maze::Maze(mcpp::Coordinate basePoint, unsigned int xlen, unsigned int zlen, 
    bool testingMode, bool enhancementsMode) {
    this->basePoint = basePoint.clone();
    this->xlen = xlen;
    this->zlen = zlen;
    this->prevBasePoint = basePoint.clone();
    this->prevXLen = xlen;
    this->prevZLen = zlen;
    this->testingMode = testingMode;
    this->enhancementsMode = enhancementsMode;
    sentinelLastMultiOptionPtr->initializeAsSentinel();
    sentinelReachablePtr->initializeAsSentinel();
}

/**
 * Deletes objects on heap.
*/
Maze::~Maze()
{
    //current structure
    if (structure != nullptr) {
        for (unsigned int i = 0; i < this->xlen; ++i) {
            delete[] structure[i];
            this->structure[i] = nullptr;
        }
        delete[] structure;
        this->structure = nullptr;
    }
    //prevStructure
    if (prevStructure != nullptr) {
        for (unsigned int i = 0; i < this->prevXLen; ++i) {
            delete[] prevStructure[i];
            this->prevStructure[i] = nullptr;
        }
        delete[] prevStructure;
        this->prevStructure = nullptr; 
    }

    //enhancedHeights
    if (enhancedHeights != nullptr) {
        for (unsigned int i = 0; i < this->xlen + 2; ++i) {
            delete[] enhancedHeights[i];
            this->enhancedHeights[i] = nullptr;
        }
        delete[] enhancedHeights;
        this->enhancedHeights = nullptr; 
    }

    //prevEnhancedHeights
    if (prevEnhancedHeights != nullptr) {
        for (unsigned int i = 0; i < this->xlen + 2; ++i) {
            delete[] prevEnhancedHeights[i];
            this->prevEnhancedHeights[i] = nullptr;
        }
        delete[] prevEnhancedHeights;
        this->prevEnhancedHeights = nullptr; 
    }

    this->sentinelLastMultiOptionPtr->deleteAll();
    this->sentinelLastMultiOptionPtr = nullptr;
    this->sentinelReachablePtr->deleteAll();
    this->sentinelReachablePtr = nullptr;
}

/**
 * Prints maze 
 * e.g.
 * 
 * **Printing Maze**
    BasePoint: ([x], [y], [z])
    Structure:
    xxxxx
    ....x
    xxxxx
    **End Printing Maze**
*/
void Maze::printMaze() const {
    std::cout << "**Printing Maze**" << std::endl; 
    std::cout << "BasePoint: (" << this->basePoint.x << ", " 
        << this->basePoint.y << ", " << basePoint.z << ")" << std::endl;
    std::cout << "Structure:" << std::endl;
    for (unsigned int i = 0; i < this->xlen; i++) {
        for (unsigned int j = 0; j < this->zlen; j++) {
            std::cout << this->structure[i][j];
        }
        std::cout << std::endl;
    }
    
    std::cout << "**End Printing Maze**" << std::endl;
    return;
}

/**
 * Sets the dimensions for a new maze based on passed arguments.
 * Note: This does not create a new maze structure, this is done
 * in 'setInitialStructure()'.
*/
void Maze::setDimensions(unsigned int xlen, unsigned int zlen) {
    // Deletes structure and enhancedHeights first if muliple mazes generated.
    if (structure != nullptr) {
        for (unsigned int i = 0; i < this->xlen; ++i) {
            delete[] structure[i];
            this->structure[i] = nullptr;
        }
        delete[] structure;
        this->structure = nullptr;
    }

    if (enhancedHeights != nullptr) {
        for (unsigned int i = 0; i < this->xlen + 2; ++i) {
            delete[] enhancedHeights[i];
            this->enhancedHeights[i] = nullptr;
        }
        delete[] enhancedHeights;
        this->enhancedHeights = nullptr; 
    }

    this->xlen = xlen;
    this->zlen = zlen;
}

void Maze::setBasePoint(mcpp::Coordinate& coordinate) {
    this->basePoint = coordinate.clone();
}

mcpp::Coordinate Maze::getBasePoint() const {
    return basePoint;
}

mcpp::Coordinate Maze::getPrevBasePoint() const {
    return prevBasePoint;
}

char** Maze::getStructure() const {
    return structure;
}

char** Maze::getPrevStructure() const {
    return this->prevStructure;
}

/**
 * Contract: 
 * -Dimensions of current maze object set with 'setDimensions()'
 * -Dimensions of current maze object 
 *      are valid for a maze (greater than 1 and odd numbers)
 * 
 * This creates a new structure on the heap and 
 * sets the 'newMaze' bool to true.
*/
void Maze::setInitialStructure() {
    this->newMaze = true;

    //Creating new structure
    structure = new char*[this->xlen];
    for (unsigned int i = 0; i < this->xlen; ++i) {
        structure[i] = new char[this->zlen];
        for (unsigned int j = 0; j < this->zlen; ++j) {
            if (i % 2 == 0) {
                structure[i][j] = 'x';
            }
            else {
                if (j % 2 == 0) {
                    structure[i][j] = 'x';
                }
                else {
                    structure[i][j] = ' ';
                }
            }
        }
    }

    // Chooses a random exit point or a fixed one if in testingMode.
    int sidesOrNot = rand() % 2;

    if (!testingMode) {
        if (sidesOrNot == 1) {
            int leftOrRight = rand() % 2;
            if (leftOrRight == 0) {
                creationStartCol = 1;
                creationStartRow = 2 * 
                    (rand() % (static_cast<int>(xlen) / 2)) + 1;
                structure[creationStartRow][creationStartCol - 1] = '.';
            }
            else {
                creationStartCol = static_cast<int>(zlen) - 2;
                creationStartRow = 2 * 
                    (rand() % (static_cast<int>(xlen) / 2)) + 1;
                structure[creationStartRow][creationStartCol + 1] = '.';
            }  
        }
        else {
            int topOrBottom = rand() % 2;
            if (topOrBottom == 0) {
                creationStartRow = 1;
                creationStartCol = 2 * 
                    (rand() % (static_cast<int>(zlen) / 2)) + 1;
                structure[creationStartRow - 1][creationStartCol] = '.';
            }
            else {
                creationStartRow = static_cast<int>(xlen) - 2;
                creationStartCol = 2 * 
                    (rand() % (static_cast<int>(zlen) / 2)) + 1;
                structure[creationStartRow + 1][creationStartCol] = '.';
            }  
        }
    }
    else {
        creationStartRow = 1;
        creationStartCol = 1;
        structure[1][0] = '.';
    }

    creationCurrRow = creationStartRow;
    creationCurrCol = creationStartCol;
    structure[creationCurrRow][creationCurrCol] = '.';
    // Sets the first point at which recursive algorithm will break down walls.
    Node* firstMultiOptionPtr = new Node(sentinelLastMultiOptionPtr);
    firstMultiOptionPtr->setDataRow(creationStartRow);
    firstMultiOptionPtr->setDataCol(creationStartCol);
    firstMultiOptionPtr->insertAtFront();
}

unsigned int Maze::getLength() const {
    return this->xlen;
}

unsigned int Maze::getWidth() const {
    return this->zlen;
}

unsigned int Maze::getPrevLength() const {
    return this->prevXLen;
}
unsigned int Maze::getPrevWidth() const {
    return this->prevZLen;
}

/*
 * Contract:
 * -Do not call this function without first calling 
 * setDimensions() and setBasePoint() and setInitialStructure()
 * 
 * Creates a random maze according to a set length and width 
 * and sets it as the current structure.
 * Is a recursive function.
 */
void Maze::generateRandomMaze(bool enhanced) {
    std::vector<std::string> validOptions(4);
    int optionCount = 0;

    // Checks which walls can be broken from up, right, down & left.
    if (!enhanced) {
        if ((creationCurrRow - 2) >= 0) {
            if (structure[creationCurrRow - 2][creationCurrCol] == ' ') {
                validOptions.at(optionCount) = "Up";
                ++optionCount;
            }
        }

        if ((creationCurrCol + 2) <= (static_cast<int>(zlen) - 1)) {
            if (structure[creationCurrRow][creationCurrCol + 2] == ' ') {
                validOptions.at(optionCount) = "Right";
                ++optionCount;
            }
        }

        if ((creationCurrRow + 2) <= (static_cast<int>(xlen) - 1)) {
            if (structure[creationCurrRow + 2][creationCurrCol] == ' ') {
                validOptions.at(optionCount) = "Down";
                ++optionCount;
            }
        }

        if ((creationCurrCol - 2) >= 0) {
            if (structure[creationCurrRow][creationCurrCol - 2] == ' ') {
                validOptions.at(optionCount) = "Left";
                ++optionCount;
            }
        }
    }
    // This also checks whether the points are reachable by heights of 1 block.
    else {
        if ((creationCurrRow - 2) >= 0) {
            if ((structure[creationCurrRow - 2][creationCurrCol] == ' ') &&
                ((enhancedHeights[creationCurrRow][creationCurrCol + 1] <= 
                enhancedHeights[creationCurrRow + 1][creationCurrCol + 1] + 1) &&
                (enhancedHeights[creationCurrRow][creationCurrCol + 1] >= 
                enhancedHeights[creationCurrRow + 1][creationCurrCol + 1] - 1)) && 
                ((enhancedHeights[creationCurrRow - 1][creationCurrCol + 1] <= 
                enhancedHeights[creationCurrRow][creationCurrCol + 1] + 1) &&
                (enhancedHeights[creationCurrRow - 1][creationCurrCol + 1] >= 
                enhancedHeights[creationCurrRow][creationCurrCol + 1] - 1))) {
                validOptions.at(optionCount) = "Up";
                ++optionCount;
            }
        }

        if ((creationCurrCol + 2) <= (static_cast<int>(zlen) - 1)) {
            if ((structure[creationCurrRow][creationCurrCol + 2] == ' ') &&
                ((enhancedHeights[creationCurrRow + 1][creationCurrCol + 2] <= 
                enhancedHeights[creationCurrRow + 1][creationCurrCol + 1] + 1) &&
                (enhancedHeights[creationCurrRow + 1][creationCurrCol + 2] >= 
                enhancedHeights[creationCurrRow + 1][creationCurrCol + 1] - 1)) && 
                ((enhancedHeights[creationCurrRow + 1][creationCurrCol + 3] <= 
                enhancedHeights[creationCurrRow + 1][creationCurrCol + 2] + 1) &&
                (enhancedHeights[creationCurrRow + 1][creationCurrCol + 3] >= 
                enhancedHeights[creationCurrRow + 1][creationCurrCol + 2] - 1))) {
                validOptions.at(optionCount) = "Right";
                ++optionCount;
            }
        }

        if ((creationCurrRow + 2) <= (static_cast<int>(xlen) - 1)) {
            if ((structure[creationCurrRow + 2][creationCurrCol] == ' ') &&
                ((enhancedHeights[creationCurrRow + 2][creationCurrCol + 1] <= 
                enhancedHeights[creationCurrRow + 1][creationCurrCol + 1] + 1) &&
                (enhancedHeights[creationCurrRow + 2][creationCurrCol + 1] >= 
                enhancedHeights[creationCurrRow + 1][creationCurrCol + 1] - 1)) && 
                ((enhancedHeights[creationCurrRow + 3][creationCurrCol + 1] <= 
                enhancedHeights[creationCurrRow + 2][creationCurrCol + 1] + 1) &&
                (enhancedHeights[creationCurrRow + 3][creationCurrCol + 1] >= 
                enhancedHeights[creationCurrRow + 2][creationCurrCol + 1] - 1))) {
                validOptions.at(optionCount) = "Down";
                ++optionCount;
            }
        }

        if ((creationCurrCol - 2) >= 0) {
            if ((structure[creationCurrRow][creationCurrCol - 2] == ' ') &&
                ((enhancedHeights[creationCurrRow + 1][creationCurrCol] <= 
                enhancedHeights[creationCurrRow + 1][creationCurrCol + 1] + 1) &&
                (enhancedHeights[creationCurrRow + 1][creationCurrCol] >= 
                enhancedHeights[creationCurrRow + 1][creationCurrCol + 1] - 1)) && 
                ((enhancedHeights[creationCurrRow + 1][creationCurrCol - 1] <= 
                enhancedHeights[creationCurrRow + 1][creationCurrCol] + 1) &&
                (enhancedHeights[creationCurrRow + 1][creationCurrCol - 1] >= 
                enhancedHeights[creationCurrRow + 1][creationCurrCol] - 1))) {
                validOptions.at(optionCount) = "Left";
                ++optionCount;
            }
        }
    }

    if (optionCount > 1) {
        Node* newMultiOptionRow = new Node(sentinelLastMultiOptionPtr);
        newMultiOptionRow->setDataRow(creationCurrRow);
        newMultiOptionRow->setDataCol(creationCurrCol);
        newMultiOptionRow->insertAtFront();
    }

    if (optionCount > 0) {
        int choice;

        if (!testingMode) {
            choice = rand() % optionCount;
        }
        else {
            choice = 0;
        }

        if (validOptions.at(choice) == "Down") {
            structure[creationCurrRow + 1][creationCurrCol] = '.';
            structure[creationCurrRow + 2][creationCurrCol] = '.';
            creationCurrRow += 2;
        } 
        else if (validOptions.at(choice) == "Up") {
            structure[creationCurrRow - 1][creationCurrCol] = '.';
            structure[creationCurrRow - 2][creationCurrCol] = '.';
            creationCurrRow -= 2;
        }
        else if (validOptions.at(choice) == "Right") {
            structure[creationCurrRow][creationCurrCol + 1] = '.';
            structure[creationCurrRow][creationCurrCol + 2] = '.';
            creationCurrCol += 2;
        }
        else {
            structure[creationCurrRow][creationCurrCol - 1] = '.';
            structure[creationCurrRow][creationCurrCol - 2] = '.';
            creationCurrCol -= 2;
        }

        generateRandomMaze(enhanced);
    }
    // Backtracks to the last multiple pathway option.
    else if (optionCount == 0 && !sentinelLastMultiOptionPtr->isEmpty()) {
        creationCurrRow = 
            sentinelLastMultiOptionPtr->getFrontPtr()->getDataRow();
        creationCurrCol = 
            sentinelLastMultiOptionPtr->getFrontPtr()->getDataCol();
        sentinelLastMultiOptionPtr->removeFront();

        generateRandomMaze(enhanced);
    }

    return; 
}

/**
 * Contract:
 * - setInitialStructure() and setBasePoint() and setDimensions() called first
 * 
 * This function reads in a user maze using characters 'x' and '.'.
 * When encountering an error, this function will throw a 
 * std::runtime_error().
*/
void Maze::readInUserMaze() {
    char curr_char;
    unsigned int i = 0;
    unsigned int j = 0;
    bool flag = false;
    long tileTotal = xlen * zlen;
    long xCount = 0;
    long iCount = 0;

    std::cout << "Enter the maze structure ('0' to exit):" << std::endl;
    for (i = 0; i < xlen; ++i) {
        for (j = 0; j < zlen; ++j) {
            std::cin >> curr_char;
            CIN_THROW_EOF_ERROR
            if (std::cin.fail()) {
                CIN_CLEAR
            }
            else if (curr_char == 'x') {
                structure[i][j] = curr_char;
                xCount += 1;
            }
            else if (curr_char == '.') {
                structure[i][j] = curr_char;
                iCount += 1;
            }
            /* exit case for when user accidently 
             * puts too large a length or width.
             */
            else if (curr_char == '0') {
                throw std::runtime_error(
                    "User entered '0', returning to menu.");
            }
            else {
                flag = true;
            }   
        }
    }

    j = 0;
    int oneExit = 0;
    bool noExit = true;

    for (i = 0; i < xlen; ++i){
        curr_char = structure[i][j];
        if (curr_char == '.') {
            oneExit += 1;
            noExit = false;
        }
    }

    j = zlen - 1;

    for (i = 0; i < xlen; ++i) {
        curr_char = structure[i][j];
        if (curr_char == '.'){
            oneExit += 1;
            noExit = false;
        }
    }

    i = 0;

    for (j = 1; j < zlen - 1; ++j) {
        curr_char = structure[i][j];
        if (curr_char == '.'){
            oneExit += 1;
            noExit = false;
        }
    }

    i = xlen - 1;

    for(j = 1; j < zlen - 1; ++j) {
        curr_char = structure[i][j];
        if (curr_char == '.'){
            oneExit += 1;
            noExit = false;
        }
    }

    // Check for loops in structure
    bool hasLoop = false;

    if (xCount != tileTotal && iCount != tileTotal) {
        Node* sentinelVisitedBricksPtr = new Node();
        sentinelVisitedBricksPtr->initializeAsSentinel();
        int sizeOfVisitedBricks = 0;
        bool foundInitialBrick = false;
        for (int i = 0; i < static_cast<int>(xlen); ++i) {
            for (int j = 0; j < static_cast<int>(zlen); ++j) {
                if (structure[i][j] == 'x' && !foundInitialBrick) {
                    Node* initialBrickPtr = new Node(sentinelVisitedBricksPtr);
                    initialBrickPtr->setDataRow(i);
                    initialBrickPtr->setDataCol(j);
                    initialBrickPtr->insertAtFront();
                    ++sizeOfVisitedBricks;
                    foundInitialBrick = true;
                }
            }
        }
        
        int pushCount = 1;
        Node* sentinelTempBricksPtr = new Node();
        sentinelTempBricksPtr->initializeAsSentinel();
        Node* currVisitedBricksPtr;
        Node* currTempBricksPtr;
        Node* innerVisitedBricksPtr;
        Node* innerTempBricksPtr;

        while (pushCount > 0) {
            pushCount = 0;
            currVisitedBricksPtr = sentinelVisitedBricksPtr->nextPtr;

            while (currVisitedBricksPtr != sentinelVisitedBricksPtr) {
                if ((currVisitedBricksPtr->getDataRow() - 1) >= 0) {
                    bool upInVisited = false;
                    bool upInTemp = false;
                    innerVisitedBricksPtr = sentinelVisitedBricksPtr->nextPtr;

                    while (innerVisitedBricksPtr != sentinelVisitedBricksPtr) {
                        if ((currVisitedBricksPtr->getDataRow() - 1 == 
                            innerVisitedBricksPtr->getDataRow()) &&
                            (currVisitedBricksPtr->getDataCol() == 
                            innerVisitedBricksPtr->getDataCol())) {
                            upInVisited = true;
                        }
                        innerVisitedBricksPtr = innerVisitedBricksPtr->nextPtr;
                    }

                    innerTempBricksPtr = sentinelTempBricksPtr->nextPtr;

                    while (innerTempBricksPtr != sentinelTempBricksPtr) {
                        if ((currVisitedBricksPtr->getDataRow() - 1 == 
                            innerTempBricksPtr->getDataRow()) &&
                            (currVisitedBricksPtr->getDataCol() == 
                            innerTempBricksPtr->getDataCol())) {
                            upInTemp = true;
                        }
                        innerTempBricksPtr = innerTempBricksPtr->nextPtr;
                    }

                    if ((structure[currVisitedBricksPtr->getDataRow() - 1][
                        currVisitedBricksPtr->getDataCol()] == 'x') &&
                        !upInVisited && !upInTemp) {
                        Node* upTempPtr = new Node(sentinelTempBricksPtr);
                        upTempPtr->setDataRow(
                            currVisitedBricksPtr->getDataRow() - 1);
                        upTempPtr->setDataCol(
                            currVisitedBricksPtr->getDataCol());
                        upTempPtr->insertAtFront();
                        ++pushCount;
                    }
                }

                if ((currVisitedBricksPtr->getDataRow() + 1) <= 
                    (static_cast<int>(xlen) - 1)) {
                    bool downInVisited = false;
                    bool downInTemp = false;
                    innerVisitedBricksPtr = sentinelVisitedBricksPtr->nextPtr;

                    while (innerVisitedBricksPtr != sentinelVisitedBricksPtr) {
                        if ((currVisitedBricksPtr->getDataRow() + 1 == 
                            innerVisitedBricksPtr->getDataRow()) &&
                            (currVisitedBricksPtr->getDataCol() == 
                            innerVisitedBricksPtr->getDataCol())) {
                            downInVisited = true;
                        }
                        innerVisitedBricksPtr = innerVisitedBricksPtr->nextPtr;
                    }

                    innerTempBricksPtr = sentinelTempBricksPtr->nextPtr;

                    while (innerTempBricksPtr != sentinelTempBricksPtr) {
                        if ((currVisitedBricksPtr->getDataRow() + 1 == 
                            innerTempBricksPtr->getDataRow()) &&
                            (currVisitedBricksPtr->getDataCol() == 
                            innerTempBricksPtr->getDataCol())) {
                            downInTemp = true;
                        }
                        innerTempBricksPtr = innerTempBricksPtr->nextPtr;
                    }

                    if ((structure[currVisitedBricksPtr->getDataRow() + 1][
                        currVisitedBricksPtr->getDataCol()] == 'x') &&
                        !downInVisited && !downInTemp) {
                        Node* downTempPtr = new Node(sentinelTempBricksPtr);
                        downTempPtr->setDataRow(
                            currVisitedBricksPtr->getDataRow() + 1);
                        downTempPtr->setDataCol(
                            currVisitedBricksPtr->getDataCol());
                        downTempPtr->insertAtFront();
                        ++pushCount;
                    }
                }

                if ((currVisitedBricksPtr->getDataCol() - 1) >= 0) {
                    bool leftInVisited = false;
                    bool leftInTemp = false;
                    innerVisitedBricksPtr = sentinelVisitedBricksPtr->nextPtr;

                    while (innerVisitedBricksPtr != sentinelVisitedBricksPtr) {
                        if ((currVisitedBricksPtr->getDataRow() == 
                            innerVisitedBricksPtr->getDataRow()) &&
                            (currVisitedBricksPtr->getDataCol() - 1 == 
                            innerVisitedBricksPtr->getDataCol())) {
                            leftInVisited = true;
                        }
                        innerVisitedBricksPtr = innerVisitedBricksPtr->nextPtr;
                    }

                    innerTempBricksPtr = sentinelTempBricksPtr->nextPtr;

                    while (innerTempBricksPtr != sentinelTempBricksPtr) {
                        if ((currVisitedBricksPtr->getDataRow() == 
                            innerTempBricksPtr->getDataRow()) &&
                            (currVisitedBricksPtr->getDataCol() - 1 == 
                            innerTempBricksPtr->getDataCol())) {
                            leftInTemp = true;
                        }
                        innerTempBricksPtr = innerTempBricksPtr->nextPtr;
                    }

                    if ((structure[currVisitedBricksPtr->getDataRow()][
                        currVisitedBricksPtr->getDataCol() - 1] == 'x') &&
                        !leftInVisited && !leftInTemp) {
                        Node* leftTempPtr = new Node(sentinelTempBricksPtr);
                        leftTempPtr->setDataRow(
                            currVisitedBricksPtr->getDataRow());
                        leftTempPtr->setDataCol(
                            currVisitedBricksPtr->getDataCol() - 1);
                        leftTempPtr->insertAtFront();
                        ++pushCount;
                    }
                }

                if ((currVisitedBricksPtr->getDataCol() + 1) <= 
                    (static_cast<int>(zlen) - 1)) {
                    bool rightInVisited = false;
                    bool rightInTemp = false;
                    innerVisitedBricksPtr = sentinelVisitedBricksPtr->nextPtr;

                    while (innerVisitedBricksPtr != sentinelVisitedBricksPtr) {
                        if ((currVisitedBricksPtr->getDataRow() == 
                            innerVisitedBricksPtr->getDataRow()) &&
                            (currVisitedBricksPtr->getDataCol() + 1 == 
                            innerVisitedBricksPtr->getDataCol())) {
                            rightInVisited = true;
                        }
                        innerVisitedBricksPtr = innerVisitedBricksPtr->nextPtr;
                    }

                    innerTempBricksPtr = sentinelTempBricksPtr->nextPtr;

                    while (innerTempBricksPtr != sentinelTempBricksPtr) {
                        if ((currVisitedBricksPtr->getDataRow() == 
                            innerTempBricksPtr->getDataRow()) &&
                            (currVisitedBricksPtr->getDataCol() + 1 == 
                            innerTempBricksPtr->getDataCol())) {
                            rightInTemp = true;
                        }
                        innerTempBricksPtr = innerTempBricksPtr->nextPtr;
                    }

                    if ((structure[currVisitedBricksPtr->getDataRow()][
                        currVisitedBricksPtr->getDataCol() + 1] == 'x') &&
                        !rightInVisited && !rightInTemp) {
                        Node* rightTempPtr = new Node(sentinelTempBricksPtr);
                        rightTempPtr->setDataRow(
                            currVisitedBricksPtr->getDataRow());
                        rightTempPtr->setDataCol(
                            currVisitedBricksPtr->getDataCol() + 1);
                        rightTempPtr->insertAtFront();
                        ++pushCount;
                    }
                }

                currVisitedBricksPtr = currVisitedBricksPtr->nextPtr;
            }

            if (pushCount > 0) {
                currTempBricksPtr = sentinelTempBricksPtr->nextPtr;

                while (currTempBricksPtr != sentinelTempBricksPtr) {
                    Node* newVisitedBricksPtr = 
                        new Node(sentinelVisitedBricksPtr);
                    newVisitedBricksPtr->setDataRow(
                        currTempBricksPtr->getDataRow());
                    newVisitedBricksPtr->setDataCol(
                        currTempBricksPtr->getDataCol());
                    newVisitedBricksPtr->insertAtFront();
                    currTempBricksPtr = currTempBricksPtr->nextPtr;
                    currTempBricksPtr->removeFront();
                    ++sizeOfVisitedBricks;
                }
            }
        }

        if ((sizeOfVisitedBricks + iCount) != (xlen * zlen)) {
            hasLoop = true;
        }

        sentinelVisitedBricksPtr->deleteAll();
        sentinelTempBricksPtr->deleteAll();
    }

    // Checking that all passage ways are connected
    bool passageDisconnected = false;

    if (xCount != tileTotal && iCount != tileTotal) {
        Node* sentinelVisitedAirPtr = new Node();
        sentinelVisitedAirPtr->initializeAsSentinel();
        int sizeOfVisitedAir = 0;
        bool foundInitialAir = false;
        for (int i = 0; i < static_cast<int>(xlen); ++i) {
            for (int j = 0; j < static_cast<int>(zlen); ++j) {
                if (structure[i][j] == '.' && !foundInitialAir) {
                    Node* initialAirPtr = new Node(sentinelVisitedAirPtr);
                    initialAirPtr->setDataRow(i);
                    initialAirPtr->setDataCol(j);
                    initialAirPtr->insertAtFront();
                    ++sizeOfVisitedAir;
                    foundInitialAir = true;
                }
            }
        }
        
        int pushCount = 1;
        Node* sentinelTempAirPtr = new Node();
        sentinelTempAirPtr->initializeAsSentinel();
        Node* currVisitedAirPtr;
        Node* currTempAirPtr;
        Node* innerVisitedAirPtr;
        Node* innerTempAirPtr;

        while (pushCount > 0) {
            pushCount = 0;
            currVisitedAirPtr = sentinelVisitedAirPtr->nextPtr;

            while (currVisitedAirPtr != sentinelVisitedAirPtr) {
                if ((currVisitedAirPtr->getDataRow() - 1) >= 0) {
                    bool upInVisited = false;
                    bool upInTemp = false;
                    innerVisitedAirPtr = sentinelVisitedAirPtr->nextPtr;

                    while (innerVisitedAirPtr != sentinelVisitedAirPtr) {
                        if ((currVisitedAirPtr->getDataRow() - 1 == 
                            innerVisitedAirPtr->getDataRow()) &&
                            (currVisitedAirPtr->getDataCol() == 
                            innerVisitedAirPtr->getDataCol())) {
                            upInVisited = true;
                        }
                        innerVisitedAirPtr = innerVisitedAirPtr->nextPtr;
                    }

                    innerTempAirPtr = sentinelTempAirPtr->nextPtr;

                    while (innerTempAirPtr != sentinelTempAirPtr) {
                        if ((currVisitedAirPtr->getDataRow() - 1 == 
                            innerTempAirPtr->getDataRow()) &&
                            (currVisitedAirPtr->getDataCol() == 
                            innerTempAirPtr->getDataCol())) {
                            upInTemp = true;
                        }
                        innerTempAirPtr = innerTempAirPtr->nextPtr;
                    }

                    if ((structure[currVisitedAirPtr->getDataRow() - 1][
                        currVisitedAirPtr->getDataCol()] == '.') &&
                        !upInVisited && !upInTemp) {
                        Node* upTempPtr = new Node(sentinelTempAirPtr);
                        upTempPtr->setDataRow(
                            currVisitedAirPtr->getDataRow() - 1);
                        upTempPtr->setDataCol(
                            currVisitedAirPtr->getDataCol());
                        upTempPtr->insertAtFront();
                        ++pushCount;
                    }
                }

                if ((currVisitedAirPtr->getDataRow() + 1) <= 
                    (static_cast<int>(xlen) - 1)) {
                    bool downInVisited = false;
                    bool downInTemp = false;
                    innerVisitedAirPtr = sentinelVisitedAirPtr->nextPtr;

                    while (innerVisitedAirPtr != sentinelVisitedAirPtr) {
                        if ((currVisitedAirPtr->getDataRow() + 1 == 
                            innerVisitedAirPtr->getDataRow()) &&
                            (currVisitedAirPtr->getDataCol() == 
                            innerVisitedAirPtr->getDataCol())) {
                            downInVisited = true;
                        }
                        innerVisitedAirPtr = innerVisitedAirPtr->nextPtr;
                    }

                    innerTempAirPtr = sentinelTempAirPtr->nextPtr;

                    while (innerTempAirPtr != sentinelTempAirPtr) {
                        if ((currVisitedAirPtr->getDataRow() + 1 == 
                            innerTempAirPtr->getDataRow()) &&
                            (currVisitedAirPtr->getDataCol() == 
                            innerTempAirPtr->getDataCol())) {
                            downInTemp = true;
                        }
                        innerTempAirPtr = innerTempAirPtr->nextPtr;
                    }

                    if ((structure[currVisitedAirPtr->getDataRow() + 1][
                        currVisitedAirPtr->getDataCol()] == '.') &&
                        !downInVisited && !downInTemp) {
                        Node* downTempPtr = new Node(sentinelTempAirPtr);
                        downTempPtr->setDataRow(
                            currVisitedAirPtr->getDataRow() + 1);
                        downTempPtr->setDataCol(
                            currVisitedAirPtr->getDataCol());
                        downTempPtr->insertAtFront();
                        ++pushCount;
                    }
                }

                if ((currVisitedAirPtr->getDataCol() - 1) >= 0) {
                    bool leftInVisited = false;
                    bool leftInTemp = false;
                    innerVisitedAirPtr = sentinelVisitedAirPtr->nextPtr;

                    while (innerVisitedAirPtr != sentinelVisitedAirPtr) {
                        if ((currVisitedAirPtr->getDataRow() == 
                            innerVisitedAirPtr->getDataRow()) &&
                            (currVisitedAirPtr->getDataCol() - 1 == 
                            innerVisitedAirPtr->getDataCol())) {
                            leftInVisited = true;
                        }
                        innerVisitedAirPtr = innerVisitedAirPtr->nextPtr;
                    }

                    innerTempAirPtr = sentinelTempAirPtr->nextPtr;

                    while (innerTempAirPtr != sentinelTempAirPtr) {
                        if ((currVisitedAirPtr->getDataRow() == 
                            innerTempAirPtr->getDataRow()) &&
                            (currVisitedAirPtr->getDataCol() - 1 == 
                            innerTempAirPtr->getDataCol())) {
                            leftInTemp = true;
                        }
                        innerTempAirPtr = innerTempAirPtr->nextPtr;
                    }

                    if ((structure[currVisitedAirPtr->getDataRow()][
                        currVisitedAirPtr->getDataCol() - 1] == '.') &&
                        !leftInVisited && !leftInTemp) {
                        Node* leftTempPtr = new Node(sentinelTempAirPtr);
                        leftTempPtr->setDataRow(currVisitedAirPtr->getDataRow());
                        leftTempPtr->setDataCol(currVisitedAirPtr->getDataCol() - 1);
                        leftTempPtr->insertAtFront();
                        ++pushCount;
                    }
                }

                if ((currVisitedAirPtr->getDataCol() + 1) <= 
                    (static_cast<int>(zlen) - 1)) {
                    bool rightInVisited = false;
                    bool rightInTemp = false;
                    innerVisitedAirPtr = sentinelVisitedAirPtr->nextPtr;

                    while (innerVisitedAirPtr != sentinelVisitedAirPtr) {
                        if ((currVisitedAirPtr->getDataRow() == 
                            innerVisitedAirPtr->getDataRow()) &&
                            (currVisitedAirPtr->getDataCol() + 1 == 
                            innerVisitedAirPtr->getDataCol())) {
                            rightInVisited = true;
                        }
                        innerVisitedAirPtr = innerVisitedAirPtr->nextPtr;
                    }

                    innerTempAirPtr = sentinelTempAirPtr->nextPtr;

                    while (innerTempAirPtr != sentinelTempAirPtr) {
                        if ((currVisitedAirPtr->getDataRow() == 
                            innerTempAirPtr->getDataRow()) &&
                            (currVisitedAirPtr->getDataCol() + 1 == 
                            innerTempAirPtr->getDataCol())) {
                            rightInTemp = true;
                        }
                        innerTempAirPtr = innerTempAirPtr->nextPtr;
                    }

                    if ((structure[currVisitedAirPtr->getDataRow()][
                        currVisitedAirPtr->getDataCol() + 1] == '.') &&
                        !rightInVisited && !rightInTemp) {
                        Node* rightTempPtr = new Node(sentinelTempAirPtr);
                        rightTempPtr->setDataRow(
                            currVisitedAirPtr->getDataRow());
                        rightTempPtr->setDataCol(
                            currVisitedAirPtr->getDataCol() + 1);
                        rightTempPtr->insertAtFront();
                        ++pushCount;
                    }
                }

                currVisitedAirPtr = currVisitedAirPtr->nextPtr;
            }

            if (pushCount > 0) {
                currTempAirPtr = sentinelTempAirPtr->nextPtr;

                while (currTempAirPtr != sentinelTempAirPtr) {
                    Node* newVisitedAirPtr = new Node(sentinelVisitedAirPtr);
                    newVisitedAirPtr->setDataRow(currTempAirPtr->getDataRow());
                    newVisitedAirPtr->setDataCol(currTempAirPtr->getDataCol());
                    newVisitedAirPtr->insertAtFront();
                    currTempAirPtr = currTempAirPtr->nextPtr;
                    currTempAirPtr->removeFront();
                    ++sizeOfVisitedAir;
                }
            }
        }

        if ((sizeOfVisitedAir + xCount) != (xlen * zlen)) {
            passageDisconnected = true;
        }

        sentinelVisitedAirPtr->deleteAll();
        sentinelTempAirPtr->deleteAll();
    }

    // Checking for air block passages of width greater than 1
    bool passageWide = false;
    for (int i = 0; i < static_cast<int>(xlen) - 1; ++i) {
        for (int j = 0; j < static_cast<int>(zlen) - 1; ++j) {
            if (structure[i][j] == '.' && structure[i + 1][j] == '.' &&
                structure[i][j + 1] == '.' && structure[i + 1][j + 1] == '.') {
                passageWide = true;
            }
        }
    }

    if (flag == true) {
        throw std::runtime_error(
            "Invalid input, please only enter a maze consisting of 'x' and '.'."
            );
    }
    else if (xCount == tileTotal) {
        throw std::runtime_error("Invalid maze, consists of all 'x'.");
    }
    else if (iCount == tileTotal) {
        throw std::runtime_error("Invalid maze, consists of all '.'.");
    }
    else if (noExit == true) {
        throw std::runtime_error("Invalid maze, maze has no exit.");
    }
    else if (oneExit > 1) {
        std::string invalidExitMessage = 
            "Invalid maze, maze can only have one exit, currently has: " + 
            std::to_string(oneExit) + ".";
        throw std::runtime_error(invalidExitMessage.c_str());
    }
    else if (hasLoop == true) {
        throw std::runtime_error(
            "Invalid maze, inner walls must be connected to outer walls.");
    }
    else if (passageDisconnected == true) {
        throw std::runtime_error(
            "Invalid maze, all passage ways must be connected.");
    }
    else if (passageWide == true) {
        throw std::runtime_error(
            "Invalid maze, passage ways can only have a width of 1.");
    }
    return;
}

/**
 * Contract:
 * - setDimensions() and setBasePoint() and setInitialStructure() and 
 * generateRandomMaze() or readInUserMaze() called first
 * 
 * Create a maze in minecraft according to the
 * saved maze structure in this Maze object.
*/
void Maze::createMinecraftMaze() const {
    mcpp::SocketConnection connection("localhost", 4711);
    mcpp::MinecraftConnection minecraft;
    //Initialised to 0 as air block
    mcpp::BlockType currBlock(0);
    for (int i = 0; i < static_cast<int>(this->xlen); i++) {
        for (int j = 0; j < static_cast<int>(this->zlen); j++) { 
            if (structure[i][j] == 'x') {
                currBlock = wallID;
                for (int k = 0; k < static_cast<int>(this->height); k++) {
                    minecraft.setBlock(basePoint + mcpp::Coordinate(i, k, j), 
                        currBlock);
                    //50ms delay
                    std::this_thread::sleep_for(std::chrono::milliseconds(50));
                }
            } else if (structure[i][j] == '.') {
                currBlock = airID;
                //50ms delay
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            } else {
                //Throw error when char in structure is unexpected
                throw end_of_file_error(
                    "Unexpected char within maze structure");                    
            }
        }
    }
    return;
}

/**
 * Contract:
 * - setDimensions() and setBasePoint() and setInitialStructure() and 
 * generateRandomMaze() or readInUserMaze() called first
 * 
 * Reset a maze in minecraft according to the
 * current saved maze structure in this Maze object.
*/
void Maze::resetMinecraftMaze() const {
    mcpp::SocketConnection connection("localhost", 4711);
    mcpp::MinecraftConnection minecraft;
    vector<vector<int>> heights = minecraft.getHeights(
        this->prevBasePoint, this->prevBasePoint + 
        mcpp::Coordinate(this->prevXLen, 0, this->prevZLen));
    for (int i = 0; i < static_cast<int>(this->prevXLen); i++) {
        for (int j = 0; j < static_cast<int>(this->prevZLen); j++) {
            if (this->prevStructure[i][j] == 'x') {
                int currHeight = heights[i][j];
                for (int k = 0; k < this->height; k++) {
                    minecraft.setBlock(mcpp::Coordinate(prevBasePoint.x + i, 
                        currHeight - k, prevBasePoint.z + j), this->airID);
                    //50ms delay
                    std::this_thread::sleep_for(std::chrono::milliseconds(50));
                }
            } 
            else if (this->prevStructure[i][j] == '.') {
                //50ms delay
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
            else {
                throw end_of_file_error(
                    "Unexpected char within maze structure");                                       
            }
        }
    }
    return;
}

/**
 * Contract:
 * - setPrevStructure() called first
 * 
 * Reset a maze in minecraft according to the
 * previous saved maze structure in this Maze object.
*/
void Maze::resetPrevMinecraftMaze() const {
    mcpp::SocketConnection connection("localhost", 4711);
    mcpp::MinecraftConnection minecraft;
    vector<vector<int>> heights = minecraft.getHeights(
        this->prevBasePoint, this->prevBasePoint + 
        mcpp::Coordinate(this->prevXLen, 0, this->prevZLen));
    for (int i = 0; i < static_cast<int>(this->prevXLen); i++) {
        for (int j = 0; j < static_cast<int>(this->prevZLen); j++) {
            if (this->prevStructure[i][j] == 'x') {
                int currHeight = heights[i][j];
                for (int k = 0; k < this->height; k++) {
                    minecraft.setBlock(mcpp::Coordinate(prevBasePoint.x + i, 
                        currHeight - k, prevBasePoint.z + j), this->airID);
                    //50ms delay
                    std::this_thread::sleep_for(std::chrono::milliseconds(50));
                }
            } 
            else if (this->prevStructure[i][j] == '.') {
                //50ms delay
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
            else {
                throw end_of_file_error(
                    "Unexpected char within maze structure");                    
            }
        }
    }
    return;
}

/**
 * Contract:
 * - setDimensions() and setBasePoint() and setInitialStructure() and 
 * generateRandomMaze() or readInUserMaze() called first
 * 
 * Copies current maze structure as the previous maze structure.
*/
void Maze::setPrevStructure() {
    //delete prevStructure if not empty
    if (prevStructure != nullptr) {
        for (unsigned int i = 0; i < this->prevXLen; ++i) {
            delete[] prevStructure[i];
            this->prevStructure[i] = nullptr;
        }
        delete[] prevStructure;
        this->prevStructure = nullptr; 
    }

    //deep copy prevStructure
    prevStructure = new char*[this->xlen];
    for (unsigned int i = 0; i < this->xlen; ++i) {
        prevStructure[i] = new char[this->zlen];
        for (unsigned int j = 0; j < this->zlen; ++j) {
            prevStructure[i][j] = structure[i][j];
        }
    }

    //delete prevEnhancedHeights if not empty
    if (prevEnhancedHeights != nullptr) {
        for (unsigned int i = 0; i < this->prevXLen + 2; ++i) {
            delete[] prevEnhancedHeights[i];
            this->prevEnhancedHeights[i] = nullptr;
        }
        delete[] prevEnhancedHeights;
        this->prevEnhancedHeights = nullptr; 
    }

    //deep copy prevEnhancedHeights
    if (enhancedHeights != nullptr) {
        prevEnhancedHeights = new int*[this->xlen + 2];
        for (unsigned int i = 0; i < this->xlen + 2; ++i) {
            prevEnhancedHeights[i] = new int[this->zlen + 2];
            for (unsigned int j = 0; j < this->zlen + 2; ++j) {
                prevEnhancedHeights[i][j] = enhancedHeights[i][j];
            }
        }
    }

    this->prevBasePoint = this->basePoint.clone();
    this->prevXLen = this->xlen;
    this->prevZLen = this->zlen;
    this->prevMaxReachableHeight = this->maxReachableHeight;
    this->prevEnhancedMaze = this->newEnhancedMaze;
    
    return;
}

bool Maze::getTestingMode() const {
    return this->testingMode;
}

bool Maze::getEnhancementsMode() const {
    return this->enhancementsMode;
}

/**
 * Contract: 
 * - setDimensions() and setBasePoint() called first
 * 
 * Changes area of the maze foundation 
 * based on current structure and creates 
 * respective data types to hold foundation data
*/
void Maze::setMazeFoundation() {
    //clear envID
    if (!envID.empty()) {
        std::cout << "Clearing envID" << std::endl;
        for (unsigned int i = 0; i < this->prevXLen; i++) {
            for (unsigned int j = 0; j < this->prevZLen; j++) {
                envID[i][j].clear();
            }
            envID[i].clear();
        }
        envID.clear();
    }
    //resize envID by xlen and zlen
    std::cout << "Resizing envID" << std::endl;
    envID.resize(this->xlen, deque<deque<mcpp::CoordinateBlock>>(this->zlen));
}

/**
 * Contract:
 * - setDimensions() and setBasePoint() and setMazeFoundation() called first
 * 
 * Flattens maze terrain, creates floor just under the basePoint.
 * Deleted blocks are stored in this object.
*/
void Maze::createMazeFoundation() {
    mcpp::SocketConnection connection("localhost", 4711);
    mcpp::MinecraftConnection minecraft;
    //Initiate player position 10 units above and facing maze
    mcpp::Coordinate viewPoint(basePoint.x, basePoint.y+10, basePoint.z);
    minecraft.setPlayerPosition(viewPoint);
    minecraft.doCommand("tp @s ~ ~ ~ -45 45");
    //Loop variables
    mcpp::Coordinate basePoint = this->basePoint + mcpp::Coordinate(0, -1, 0); 
    mcpp::BlockType currBlock(0);
    mcpp::Coordinate currCoordinate(0,0,0);
    mcpp::CoordinateBlock currCoordinateBlock(0,0,0);
    int currHeight = 0;
    vector<vector<int>> heights = minecraft.getHeights(this->basePoint, 
        this->basePoint + mcpp::Coordinate(this->xlen, 0, this->zlen));
    for (unsigned int i = 0; i < this->xlen; i++) {
        for (unsigned int j = 0; j < this->zlen; j++) {
            currHeight = heights[i][j];
            if (currHeight < basePoint.y) {
                for (int k = currHeight + 1; k < basePoint.y + 1; k++) {
                    if (k == basePoint.y) {
                        currCoordinate = mcpp::Coordinate(
                            basePoint.x + i, k, basePoint.z + j
                            );
                        currBlock = minecraft.getBlock(currCoordinate);
                        currCoordinateBlock = mcpp::CoordinateBlock(
                            currCoordinate, currBlock
                            );
                        envID[i][j].push_front(currCoordinateBlock);
                        minecraft.setBlock(currCoordinate, this->floorID);
                    }
                    else {
                        currCoordinate = mcpp::Coordinate(
                            basePoint.x + i, k, basePoint.z + j
                            );
                        currBlock = minecraft.getBlock(currCoordinate);
                        currCoordinateBlock = mcpp::CoordinateBlock(
                            currCoordinate, currBlock
                            );
                        envID[i][j].push_front(currCoordinateBlock);
                        minecraft.setBlock(currCoordinate, this->fillID);
                    }
                }
            }
            //if currHeight is higher or equal to basepoint
            else {
                //keep decrementing until currheight is basepoint
                for (int k = currHeight; k >= basePoint.y; k--) {
                    currCoordinate = mcpp::Coordinate(
                        basePoint.x + i, k, basePoint.z + j
                        );
                    currBlock = minecraft.getBlock(currCoordinate);
                    //Check if basepoint
                    if (k == basePoint.y) {
                        currCoordinateBlock = mcpp::CoordinateBlock(
                            currCoordinate, currBlock
                            );
                        envID[i][j].push_front(currCoordinateBlock);
                        //if block beneath floor is not air:
                        mcpp::Coordinate innerCurrCoordinate = (
                            currCoordinate + mcpp::Coordinate(0, -1, 0)
                            );
                        while (minecraft.getBlock(
                            innerCurrCoordinate) 
                            == this->airID) 
                            {
                            currBlock = minecraft.getBlock(
                                innerCurrCoordinate);
                            currCoordinateBlock = mcpp::CoordinateBlock(
                                innerCurrCoordinate, currBlock
                                );
                            envID[i][j].push_front(currCoordinateBlock);
                            minecraft.setBlock(innerCurrCoordinate, 
                                this->fillID);
                            //decrement innerCurrCoordinate
                            innerCurrCoordinate = (
                            innerCurrCoordinate + mcpp::Coordinate(0, -1, 0)
                            );
                        }
                        //Place floor if not already floor
                        if (!(minecraft.getBlock(currCoordinate) == 
                            this->floorID)) {
                            minecraft.setBlock(currCoordinate, this->floorID);
                        }
                    }
                    else {
                        currCoordinateBlock = mcpp::CoordinateBlock(
                            currCoordinate, currBlock);
                        envID[i][j].push_front(currCoordinateBlock);
                        minecraft.setBlock(currCoordinate, this->airID);
                    }
                }

            }

        }
    }
}


/**
 * Contract:
 * - setPrevStructure() and CreateMazeFoundation() called first
 * 
 * Resets minecraft world to normal after building a maze foundation.
*/
void Maze::ResetMazeFoundation() {
    mcpp::SocketConnection connection("localhost", 4711);
    mcpp::MinecraftConnection minecraft; 
    //mcpp::BlockType currBlock(0); //Initialised to 0 as air block
    for (unsigned int i = 0; i < this->prevXLen; i++) {
        for (unsigned int j = 0; j < this->prevZLen; j++) {
            for (int k = 0; 
                k < static_cast<int>(envID.at(i).at(j).size()); k++) {
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
                minecraft.setBlock(
                    envID[i][j][k].getCoordinate(), 
                    envID[i][j][k].getBlockType()
                    );
            }
        }
    }
}

// Starting from the base point finds a list of points reachable by 1 block.
void Maze::findMaxReachable() {
    Node* basePointPtr = new Node(sentinelReachablePtr);
    basePointPtr->setDataRow(1);
    basePointPtr->setDataCol(1);
    basePointPtr->insertAtFront();
    Node* sentinelTempPtr = new Node();
    sentinelTempPtr->initializeAsSentinel();
    Node* currReachablePtr;
    Node* currTempPtr;
    Node* innerReachablePtr;
    Node* innerTempPtr;
    int pushCount = 1;

    while (pushCount > 0) {
        pushCount = 0;
        currReachablePtr = sentinelReachablePtr->nextPtr;
        while (currReachablePtr != sentinelReachablePtr) {
            if (currReachablePtr->getDataRow() - 1 >= 0) {
                bool upInReachable = false;
                bool upInTemp = false;
                innerReachablePtr = sentinelReachablePtr->nextPtr;

                while (innerReachablePtr != sentinelReachablePtr) {
                    if ((currReachablePtr->getDataRow() - 1 == 
                        innerReachablePtr->getDataRow()) &&
                        (currReachablePtr->getDataCol() == 
                        innerReachablePtr->getDataCol())) {
                        upInReachable = true;
                    }
                    innerReachablePtr = innerReachablePtr->nextPtr;
                }

                innerTempPtr = sentinelTempPtr->nextPtr;

                while (innerTempPtr != sentinelTempPtr) {
                    if ((currReachablePtr->getDataRow() - 1 == 
                        innerTempPtr->getDataRow()) &&
                        (currReachablePtr->getDataCol() == 
                        innerTempPtr->getDataCol())) {
                        upInTemp = true;
                    }
                    innerTempPtr = innerTempPtr->nextPtr;
                }

                if ((enhancedHeights[currReachablePtr->getDataRow() - 1][
                    currReachablePtr->getDataCol()] <= 
                    enhancedHeights[currReachablePtr->getDataRow()][
                    currReachablePtr->getDataCol()] + 1) && 
                    (enhancedHeights[currReachablePtr->getDataRow() - 1][
                    currReachablePtr->getDataCol()] >= 
                    enhancedHeights[currReachablePtr->getDataRow()][
                    currReachablePtr->getDataCol()] - 1) &&
                    !upInReachable && !upInTemp) {
                    Node* upTempPtr = new Node(sentinelTempPtr);
                    upTempPtr->setDataRow(
                        currReachablePtr->getDataRow() - 1);
                    upTempPtr->setDataCol(
                        currReachablePtr->getDataCol());
                    upTempPtr->insertAtFront();
                    ++pushCount;
                }
            }

            if (currReachablePtr->getDataRow() + 1 <= 
                static_cast<int>(xlen) + 1) {
                bool downInReachable = false;
                bool downInTemp = false;
                innerReachablePtr = sentinelReachablePtr->nextPtr;

                while (innerReachablePtr != sentinelReachablePtr) {
                    if ((currReachablePtr->getDataRow() + 1 == 
                        innerReachablePtr->getDataRow()) &&
                        (currReachablePtr->getDataCol() == 
                        innerReachablePtr->getDataCol())) {
                        downInReachable = true;
                    }
                    innerReachablePtr = innerReachablePtr->nextPtr;
                }

                innerTempPtr = sentinelTempPtr->nextPtr;

                while (innerTempPtr != sentinelTempPtr) {
                    if ((currReachablePtr->getDataRow() + 1 == 
                        innerTempPtr->getDataRow()) &&
                        (currReachablePtr->getDataCol() == 
                        innerTempPtr->getDataCol())) {
                        downInTemp = true;
                    }
                    innerTempPtr = innerTempPtr->nextPtr;
                }

                if ((enhancedHeights[currReachablePtr->getDataRow() + 1][
                    currReachablePtr->getDataCol()] <= 
                    enhancedHeights[currReachablePtr->getDataRow()][
                    currReachablePtr->getDataCol()] + 1) && 
                    (enhancedHeights[currReachablePtr->getDataRow() + 1][
                    currReachablePtr->getDataCol()] >= 
                    enhancedHeights[currReachablePtr->getDataRow()][
                    currReachablePtr->getDataCol()] - 1) &&
                    !downInReachable && !downInTemp) {
                    Node* downTempPtr = new Node(sentinelTempPtr);
                    downTempPtr->setDataRow(
                        currReachablePtr->getDataRow() + 1);
                    downTempPtr->setDataCol(
                        currReachablePtr->getDataCol());
                    downTempPtr->insertAtFront();
                    ++pushCount;
                }
            }

            if (currReachablePtr->getDataCol() - 1 >= 0) {
                bool leftInReachable = false;
                bool leftInTemp = false;
                innerReachablePtr = sentinelReachablePtr->nextPtr;

                while (innerReachablePtr != sentinelReachablePtr) {
                    if ((currReachablePtr->getDataRow() == 
                        innerReachablePtr->getDataRow()) &&
                        (currReachablePtr->getDataCol() - 1 == 
                        innerReachablePtr->getDataCol())) {
                        leftInReachable = true;
                    }
                    innerReachablePtr = innerReachablePtr->nextPtr;
                }

                innerTempPtr = sentinelTempPtr->nextPtr;

                while (innerTempPtr != sentinelTempPtr) {
                    if ((currReachablePtr->getDataRow() == 
                        innerTempPtr->getDataRow()) &&
                        (currReachablePtr->getDataCol() - 1 == 
                        innerTempPtr->getDataCol())) {
                        leftInTemp = true;
                    }
                    innerTempPtr = innerTempPtr->nextPtr;
                }

                if ((enhancedHeights[currReachablePtr->getDataRow()][
                    currReachablePtr->getDataCol() - 1] <= 
                    enhancedHeights[currReachablePtr->getDataRow()][
                    currReachablePtr->getDataCol()] + 1) && 
                    (enhancedHeights[currReachablePtr->getDataRow()][
                    currReachablePtr->getDataCol() - 1] >= 
                    enhancedHeights[currReachablePtr->getDataRow()][
                    currReachablePtr->getDataCol()] - 1) &&
                    !leftInReachable && !leftInTemp) {
                    Node* leftTempPtr = new Node(sentinelTempPtr);
                    leftTempPtr->setDataRow(
                        currReachablePtr->getDataRow());
                    leftTempPtr->setDataCol(
                        currReachablePtr->getDataCol() - 1);
                    leftTempPtr->insertAtFront();
                    ++pushCount;
                }
            }

            if (currReachablePtr->getDataCol() + 1 <= 
                static_cast<int>(zlen) + 1) {
                bool rightInReachable = false;
                bool rightInTemp = false;
                innerReachablePtr = sentinelReachablePtr->nextPtr;

                while (innerReachablePtr != sentinelReachablePtr) {
                    if ((currReachablePtr->getDataRow() == 
                        innerReachablePtr->getDataRow()) &&
                        (currReachablePtr->getDataCol() + 1 == 
                        innerReachablePtr->getDataCol())) {
                        rightInReachable = true;
                    }
                    innerReachablePtr = innerReachablePtr->nextPtr;
                }

                innerTempPtr = sentinelTempPtr->nextPtr;

                while (innerTempPtr != sentinelTempPtr) {
                    if ((currReachablePtr->getDataRow() == 
                        innerTempPtr->getDataRow()) &&
                        (currReachablePtr->getDataCol() + 1 == 
                        innerTempPtr->getDataCol())) {
                        rightInTemp = true;
                    }
                    innerTempPtr = innerTempPtr->nextPtr;
                }

                if ((enhancedHeights[currReachablePtr->getDataRow()][
                    currReachablePtr->getDataCol() + 1] <= 
                    enhancedHeights[currReachablePtr->getDataRow()][
                    currReachablePtr->getDataCol()] + 1) && 
                    (enhancedHeights[currReachablePtr->getDataRow()][
                    currReachablePtr->getDataCol() + 1] >= 
                    enhancedHeights[currReachablePtr->getDataRow()][
                    currReachablePtr->getDataCol()] - 1) &&
                    !rightInReachable && !rightInTemp) {
                    Node* rightTempPtr = new Node(sentinelTempPtr);
                    rightTempPtr->setDataRow(
                        currReachablePtr->getDataRow());
                    rightTempPtr->setDataCol(
                        currReachablePtr->getDataCol() + 1);
                    rightTempPtr->insertAtFront();
                    ++pushCount;
                }
            }
            currReachablePtr = currReachablePtr->nextPtr;
        }

        if (pushCount > 0) {
            currTempPtr = sentinelTempPtr->nextPtr;

            while (currTempPtr != sentinelTempPtr) {
                Node* newReachablePtr = new Node(sentinelReachablePtr);
                newReachablePtr->setDataRow(currTempPtr->getDataRow());
                newReachablePtr->setDataCol(currTempPtr->getDataCol());
                newReachablePtr->insertAtFront();
                currTempPtr = currTempPtr->nextPtr;
                currTempPtr->removeFront();
            }
        }
    }
    sentinelTempPtr->deleteAll();
    sentinelTempPtr = nullptr;
}

void Maze::removeStructure() {
    if (this->structure != nullptr) {
        for (unsigned int i = 0; i < this->xlen; ++i) {
            delete[] structure[i];
            this->structure[i] = nullptr;
        }
        delete[] structure;
        this->structure = nullptr;
    }
    this->xlen = 0;
    this->zlen = 0;
    this->basePoint = mcpp::Coordinate(0, 0, 0);
}

/* 
 * Reading the heights from Minecraft including perimeter rows and columns.
 */
void Maze::getAreaHeight(){
    
    mcpp::MinecraftConnection mc;
    mcpp::Coordinate startCoordinate = this->basePoint.clone();
    startCoordinate.x = startCoordinate.x - 1;
    startCoordinate.z = startCoordinate.z - 1;

    mcpp::Coordinate endCoordinate = this->basePoint.clone();
    endCoordinate.x = endCoordinate.x + static_cast<int>(this->xlen) + 1;
    endCoordinate.z = endCoordinate.z + static_cast<int>(this->zlen) + 1;

    std::vector<std::vector<int>> heightsVector = mc.getHeights(
        startCoordinate, endCoordinate); 

    enhancedHeights = new int*[static_cast<int>(this->xlen) + 2];
    for (int i = 0; i < static_cast<int>(this->xlen) + 2; ++i) {
        enhancedHeights[i] = new int[static_cast<int>(this->zlen) + 2];
        for (int j = 0; j < static_cast<int>(this->zlen) + 2; ++j) { 
            enhancedHeights[i][j] = heightsVector.at(i).at(j);
        }
    }
}

// Creates the initial maze grid with only potential paths that are reachable.  
void Maze::setInitialStructureEnhanced() {
    this->newMaze = true;

    //Creating structure including 'o' for obstacles and perimeter as 'p'.
    char** extendedStructure = new char*[this->xlen + 2];
    for (unsigned int i = 0; i < this->xlen + 2; ++i) {
        extendedStructure[i] = new char[this->zlen + 2];
        for (unsigned int j = 0; j < this->zlen + 2; ++j) {
            extendedStructure[i][j] = 'o';
        }    
    }

    Node* currReachablePtr = sentinelReachablePtr->nextPtr;

    while (currReachablePtr != sentinelReachablePtr) {
        if (currReachablePtr->getDataRow() == 0 || 
            currReachablePtr->getDataRow() == static_cast<int>(xlen) + 1 ||
            currReachablePtr->getDataCol() == 0 ||
            currReachablePtr->getDataCol() == static_cast<int>(zlen) + 1) {
            extendedStructure[currReachablePtr->getDataRow()][
                currReachablePtr->getDataCol()] = 'p';
        }
        else {
            extendedStructure[currReachablePtr->getDataRow()][
                currReachablePtr->getDataCol()] = ' ';
        }
        currReachablePtr = currReachablePtr->nextPtr;
        currReachablePtr->removeFront();
    }

    bool obstacleInFirstGrid = false;

    for (int i = 1; i <= 3; ++i) {
        for (int j = 1; j <= 3; ++j) {
            if (extendedStructure[i][j] == 'o') {
                obstacleInFirstGrid = true;
            }
        }
    }

    bool exitInFirstGrid = false;

    if (!testingMode) {    
        if (!obstacleInFirstGrid) {
            if (((enhancedHeights[2][1] <= enhancedHeights[2][2] + 1) &&
                (enhancedHeights[2][1] >= enhancedHeights[2][2] - 1)) && 
                ((enhancedHeights[2][0] <= enhancedHeights[2][1] + 1) &&
                (enhancedHeights[2][0] >= enhancedHeights[2][1] - 1)) &&
                (enhancedHeights[2][0] >= enhancedHeights[1][0]) &&
                (enhancedHeights[2][0] >= enhancedHeights[3][0]) &&
                (extendedStructure[2][0] == 'p')) {
                exitInFirstGrid = true;
            }
            else if (((enhancedHeights[1][2] <= enhancedHeights[2][2] + 1) &&
                (enhancedHeights[1][2] >= enhancedHeights[2][2] - 1)) && 
                ((enhancedHeights[0][2] <= enhancedHeights[1][2] + 1) &&
                (enhancedHeights[0][2] >= enhancedHeights[1][2] - 1)) &&
                (enhancedHeights[0][2] >= enhancedHeights[0][1]) &&
                (enhancedHeights[0][2] >= enhancedHeights[0][3]) &&
                (extendedStructure[0][2] == 'p')) {
                exitInFirstGrid = true;
            }
        }
    }
    else {
        if (!obstacleInFirstGrid) {
            if (((enhancedHeights[2][1] <= enhancedHeights[2][2] + 1) &&
                (enhancedHeights[2][1] >= enhancedHeights[2][2] - 1)) && 
                ((enhancedHeights[2][0] <= enhancedHeights[2][1] + 1) &&
                (enhancedHeights[2][0] >= enhancedHeights[2][1] - 1)) &&
                (enhancedHeights[2][0] >= enhancedHeights[1][0]) &&
                (enhancedHeights[2][0] >= enhancedHeights[3][0]) &&
                (extendedStructure[2][0] == 'p')) {
                exitInFirstGrid = true;
            }
        }
    }

    if (!obstacleInFirstGrid && exitInFirstGrid) {
        Node* sentinelGridPtr = new Node();
        sentinelGridPtr->initializeAsSentinel();
        Node* firstGridPtr = new Node(sentinelGridPtr);
        firstGridPtr->setDataRow(2);
        firstGridPtr->setDataCol(2);
        firstGridPtr->insertAtFront();
        Node* sentinelTempPtr = new Node();
        sentinelTempPtr->initializeAsSentinel();
        Node* currGridPtr;
        Node* currTempPtr;
        Node* innerGridPtr;
        Node* innerTempPtr;
        int pushCount = 1;

        // Checks up, down, left, right for reachable grid not aleady included.
        while (pushCount > 0) {
            pushCount = 0;
            currGridPtr = sentinelGridPtr->nextPtr;
        
            while (currGridPtr != sentinelGridPtr) {
                if (currGridPtr->getDataRow() - 2 >= 2) {
                    bool upInGrid = false;
                    bool upInTemp = false;
                    innerGridPtr = sentinelGridPtr->nextPtr;

                    while (innerGridPtr != sentinelGridPtr) {
                        if ((currGridPtr->getDataRow() - 2 == 
                            innerGridPtr->getDataRow()) &&
                            (currGridPtr->getDataCol() == 
                            innerGridPtr->getDataCol())) {
                            upInGrid = true;
                        }
                        innerGridPtr = innerGridPtr->nextPtr;
                    }

                    innerTempPtr = sentinelTempPtr->nextPtr;

                    while (innerTempPtr != sentinelTempPtr) {
                        if ((currGridPtr->getDataRow() - 2 == 
                            innerTempPtr->getDataRow()) &&
                            (currGridPtr->getDataCol() == 
                            innerTempPtr->getDataCol())) {
                            upInTemp = true;
                        }
                        innerTempPtr = innerTempPtr->nextPtr;
                    }

                    int obstacleInUpCount = 0;

                    for (int i = currGridPtr->getDataRow() - 3; 
                        i <= currGridPtr->getDataRow() + 1; ++i) {
                        for (int j = currGridPtr->getDataCol() - 1; 
                            j <= currGridPtr->getDataCol() + 1; ++j) {
                            if (extendedStructure[i][j] == 'o') {
                                ++obstacleInUpCount;
                            }
                        }
                    }

                    if (!upInGrid && !upInTemp && obstacleInUpCount == 0 &&
                        ((enhancedHeights[currGridPtr->getDataRow() - 1][
                        currGridPtr->getDataCol()] <= 
                        enhancedHeights[currGridPtr->getDataRow()][
                        currGridPtr->getDataCol()] + 1) &&
                        (enhancedHeights[currGridPtr->getDataRow() - 1][
                        currGridPtr->getDataCol()] >= 
                        enhancedHeights[currGridPtr->getDataRow()][
                        currGridPtr->getDataCol()] - 1)) && 
                        ((enhancedHeights[currGridPtr->getDataRow() - 2][
                        currGridPtr->getDataCol()] <= 
                        enhancedHeights[currGridPtr->getDataRow() - 1][
                        currGridPtr->getDataCol()] + 1) &&
                        (enhancedHeights[currGridPtr->getDataRow() - 2][
                        currGridPtr->getDataCol()] >= 
                        enhancedHeights[currGridPtr->getDataRow() - 1][
                        currGridPtr->getDataCol()] - 1))) {

                        Node* upTempPtr = new Node(sentinelTempPtr);
                        upTempPtr->setDataRow(currGridPtr->getDataRow() - 2);
                        upTempPtr->setDataCol(currGridPtr->getDataCol());
                        upTempPtr->insertAtFront();
                        ++pushCount;
                    }
                }

                if (currGridPtr->getDataRow() + 2 <= static_cast<int>(xlen)) {
                    bool downInGrid = false;
                    bool downInTemp = false;
                    innerGridPtr = sentinelGridPtr->nextPtr;

                    while (innerGridPtr != sentinelGridPtr) {
                        if ((currGridPtr->getDataRow() + 2 == 
                            innerGridPtr->getDataRow()) &&
                            (currGridPtr->getDataCol() == 
                            innerGridPtr->getDataCol())) {
                            downInGrid = true;
                        }
                        innerGridPtr = innerGridPtr->nextPtr;
                    }

                    innerTempPtr = sentinelTempPtr->nextPtr;

                    while (innerTempPtr != sentinelTempPtr) {
                        if ((currGridPtr->getDataRow() + 2 == 
                            innerTempPtr->getDataRow()) &&
                            (currGridPtr->getDataCol() == 
                            innerTempPtr->getDataCol())) {
                            downInTemp = true;
                        }
                        innerTempPtr = innerTempPtr->nextPtr;
                    }

                    int obstacleInDownCount = 0;

                    for (int i = currGridPtr->getDataRow() - 1; 
                        i <= currGridPtr->getDataRow() + 3; ++i) {
                        for (int j = currGridPtr->getDataCol() - 1; 
                            j <= currGridPtr->getDataCol() + 1; ++j) {
                            if (extendedStructure[i][j] == 'o') {
                                ++obstacleInDownCount;
                            }
                        }
                    }

                    if (!downInGrid && !downInTemp && 
                        obstacleInDownCount == 0 &&
                        ((enhancedHeights[currGridPtr->getDataRow() + 1][
                        currGridPtr->getDataCol()] <= 
                        enhancedHeights[currGridPtr->getDataRow()][
                        currGridPtr->getDataCol()] + 1) &&
                        (enhancedHeights[currGridPtr->getDataRow() + 1][
                        currGridPtr->getDataCol()] >= 
                        enhancedHeights[currGridPtr->getDataRow()][
                        currGridPtr->getDataCol()] - 1)) && 
                        ((enhancedHeights[currGridPtr->getDataRow() + 2][
                        currGridPtr->getDataCol()] <= 
                        enhancedHeights[currGridPtr->getDataRow() + 1][
                        currGridPtr->getDataCol()] + 1) &&
                        (enhancedHeights[currGridPtr->getDataRow() + 2][
                        currGridPtr->getDataCol()] >= 
                        enhancedHeights[currGridPtr->getDataRow() + 1][
                        currGridPtr->getDataCol()] - 1))) {

                        Node* downTempPtr = new Node(sentinelTempPtr);
                        downTempPtr->setDataRow(currGridPtr->getDataRow() + 2);
                        downTempPtr->setDataCol(currGridPtr->getDataCol());
                        downTempPtr->insertAtFront();
                        ++pushCount;
                    }
                }

                if (currGridPtr->getDataCol() - 2 >= 2) {
                    bool leftInGrid = false;
                    bool leftInTemp = false;
                    innerGridPtr = sentinelGridPtr->nextPtr;

                    while (innerGridPtr != sentinelGridPtr) {
                        if ((currGridPtr->getDataRow() == 
                            innerGridPtr->getDataRow()) &&
                            (currGridPtr->getDataCol() - 2 == 
                            innerGridPtr->getDataCol())) {
                            leftInGrid = true;
                        }
                        innerGridPtr = innerGridPtr->nextPtr;
                    }

                    innerTempPtr = sentinelTempPtr->nextPtr;

                    while (innerTempPtr != sentinelTempPtr) {
                        if ((currGridPtr->getDataRow() == 
                            innerTempPtr->getDataRow()) &&
                            (currGridPtr->getDataCol() - 2 == 
                            innerTempPtr->getDataCol())) {
                            leftInTemp = true;
                        }
                        innerTempPtr = innerTempPtr->nextPtr;
                    }

                    int obstacleInLeftCount = 0;

                    for (int i = currGridPtr->getDataRow() - 1; 
                        i <= currGridPtr->getDataRow() + 1; ++i) {
                        for (int j = currGridPtr->getDataCol() - 3; 
                            j <= currGridPtr->getDataCol() + 1; ++j) {
                            if (extendedStructure[i][j] == 'o') {
                                ++obstacleInLeftCount;
                            }
                        }
                    }

                    if (!leftInGrid && !leftInTemp && 
                        obstacleInLeftCount == 0 &&
                        ((enhancedHeights[currGridPtr->getDataRow()][
                        currGridPtr->getDataCol() - 1] <= 
                        enhancedHeights[currGridPtr->getDataRow()][
                        currGridPtr->getDataCol()] + 1) &&
                        (enhancedHeights[currGridPtr->getDataRow()][
                        currGridPtr->getDataCol() - 1] >= 
                        enhancedHeights[currGridPtr->getDataRow()][
                        currGridPtr->getDataCol()] - 1)) && 
                        ((enhancedHeights[currGridPtr->getDataRow()][
                        currGridPtr->getDataCol() - 2] <= 
                        enhancedHeights[currGridPtr->getDataRow()][
                        currGridPtr->getDataCol() - 1] + 1) &&
                        (enhancedHeights[currGridPtr->getDataRow()][
                        currGridPtr->getDataCol() - 2] >= 
                        enhancedHeights[currGridPtr->getDataRow()][
                        currGridPtr->getDataCol() - 1] - 1))) {

                        Node* leftTempPtr = new Node(sentinelTempPtr);
                        leftTempPtr->setDataRow(currGridPtr->getDataRow());
                        leftTempPtr->setDataCol(currGridPtr->getDataCol() - 2);
                        leftTempPtr->insertAtFront();
                        ++pushCount;
                    }
                }

                if (currGridPtr->getDataCol() + 2 <= static_cast<int>(zlen)) {
                    bool rightInGrid = false;
                    bool rightInTemp = false;
                    innerGridPtr = sentinelGridPtr->nextPtr;

                    while (innerGridPtr != sentinelGridPtr) {
                        if ((currGridPtr->getDataRow() == 
                            innerGridPtr->getDataRow()) &&
                            (currGridPtr->getDataCol() + 2 == 
                            innerGridPtr->getDataCol())) {
                            rightInGrid = true;
                        }
                        innerGridPtr = innerGridPtr->nextPtr;
                    }

                    innerTempPtr = sentinelTempPtr->nextPtr;

                    while (innerTempPtr != sentinelTempPtr) {
                        if ((currGridPtr->getDataRow() == 
                            innerTempPtr->getDataRow()) &&
                            (currGridPtr->getDataCol() + 2 == 
                            innerTempPtr->getDataCol())) {
                            rightInTemp = true;
                        }
                        innerTempPtr = innerTempPtr->nextPtr;
                    }

                    int obstacleInRightCount = 0;

                    for (int i = currGridPtr->getDataRow() - 1; 
                        i <= currGridPtr->getDataRow() + 1; ++i) {
                        for (int j = currGridPtr->getDataCol() - 1; 
                            j <= currGridPtr->getDataCol() + 3; ++j) {
                            if (extendedStructure[i][j] == 'o') {
                                ++obstacleInRightCount;
                            }
                        }
                    }

                    if (!rightInGrid && !rightInTemp && 
                        obstacleInRightCount == 0 &&
                        ((enhancedHeights[currGridPtr->getDataRow()][
                        currGridPtr->getDataCol() + 1] <= 
                        enhancedHeights[currGridPtr->getDataRow()][
                        currGridPtr->getDataCol()] + 1) &&
                        (enhancedHeights[currGridPtr->getDataRow()][
                        currGridPtr->getDataCol() + 1] >= 
                        enhancedHeights[currGridPtr->getDataRow()][
                        currGridPtr->getDataCol()] - 1)) && 
                        ((enhancedHeights[currGridPtr->getDataRow()][
                        currGridPtr->getDataCol() + 2] <= 
                        enhancedHeights[currGridPtr->getDataRow()][
                        currGridPtr->getDataCol() + 1] + 1) &&
                        (enhancedHeights[currGridPtr->getDataRow()][
                        currGridPtr->getDataCol() + 2] >= 
                        enhancedHeights[currGridPtr->getDataRow()][
                        currGridPtr->getDataCol() + 1] - 1))) {

                        Node* rightTempPtr = new Node(sentinelTempPtr);
                        rightTempPtr->setDataRow(currGridPtr->getDataRow());
                        rightTempPtr->setDataCol(
                            currGridPtr->getDataCol() + 2);
                        rightTempPtr->insertAtFront();
                        ++pushCount;
                    }
                }
                currGridPtr = currGridPtr->nextPtr;
            }

            if (pushCount > 0) {
                currTempPtr = sentinelTempPtr->nextPtr;

                while (currTempPtr != sentinelTempPtr) {
                    Node* newGridPtr = new Node(sentinelGridPtr);
                    newGridPtr->setDataRow(currTempPtr->getDataRow());
                    newGridPtr->setDataCol(currTempPtr->getDataCol());
                    newGridPtr->insertAtFront();
                    currTempPtr = currTempPtr->nextPtr;
                    currTempPtr->removeFront();
                }
            }
        }

        currGridPtr = sentinelGridPtr->nextPtr;

        // Setting wall chars as 'x' around reachable grid center points.
        while (currGridPtr != sentinelGridPtr) {
            for (int i = currGridPtr->getDataRow() - 1; 
                i <= currGridPtr->getDataRow() + 1; ++i) {
                for (int j = currGridPtr->getDataCol() - 1; 
                    j <= currGridPtr->getDataCol() + 1; ++j) {
                    if ((i == currGridPtr->getDataRow()) && 
                        (j == currGridPtr->getDataCol())) {
                        extendedStructure[i][j] = ' ';
                    }
                    else {
                        extendedStructure[i][j] = 'x';
                    }
                }
            }
            currGridPtr = currGridPtr->nextPtr;
        }

        // Filling remaining non-enclosed areas with 'o' for obstacles.
        for (int i = 0; i < static_cast<int>(xlen) + 2; ++i) {
            int j = 0;

            while ((j < static_cast<int>(zlen) + 2) &&
                extendedStructure[i][j] != 'x' && 
                extendedStructure[i][j] != '.') {
                if (extendedStructure[i][j] == ' ') {
                    extendedStructure[i][j] = 'o';
                }
                ++j;
            }
        }

        for (int i = 0; i < static_cast<int>(xlen) + 2; ++i) {
            int j = static_cast<int>(zlen) + 1;

            while ((j >= 0) &&
                extendedStructure[i][j] != 'x' && 
                extendedStructure[i][j] != '.') {
                if (extendedStructure[i][j] == ' ') {
                    extendedStructure[i][j] = 'o';
                }
                --j;
            }
        }

        for (int j = 0; j < static_cast<int>(zlen) + 2; ++j) {
            int i = 0;

            while ((i < static_cast<int>(xlen) + 2) &&
                extendedStructure[i][j] != 'x' && 
                extendedStructure[i][j] != '.') {
                if (extendedStructure[i][j] == ' ') {
                    extendedStructure[i][j] = 'o';
                }
                ++i;
            }
        }

        for (int j = 0; j < static_cast<int>(zlen) + 2; ++j) {
            int i = static_cast<int>(xlen) + 1;

            while ((i >= 0) &&
                extendedStructure[i][j] != 'x' && 
                extendedStructure[i][j] != '.') {
                if (extendedStructure[i][j] == ' ') {
                    extendedStructure[i][j] = 'o';
                }
                --i;
            }
        }

        // Filling remaining enclosed areas around obstacles with wall blocks.
        for (int i = 1; i < static_cast<int>(xlen) + 1; ++i) {
            for (int j = 1; j < static_cast<int>(zlen) + 1; ++j) {
                for (int k = i - 1; k <= i + 1; ++k) {
                    for (int l = j - 1; l <= j + 1; ++l) {
                        if (extendedStructure[i][j] == ' ' &&
                            !(i == k && j == l) && 
                            extendedStructure[k][l] != 'x') {
                            extendedStructure[i][j] = 'x';
                        }
                    }
                }
            }
        }

        bool exitFound = false;

        // Chooses an exit at random or a fixed one if in testing mode. 
        if (!testingMode) {
            while (!exitFound) {
                int potentialExitRow = 
                    (((rand() % (static_cast<int>(xlen) / 2)) + 1) * 2);
                int potentialExitCol = 
                    (((rand() % (static_cast<int>(zlen) / 2)) + 1) * 2);

                if (extendedStructure[potentialExitRow][potentialExitCol] == 
                    ' ') {
                    int upDownLeftRight = rand() % 4;

                    if (upDownLeftRight == 0 &&
                        ((enhancedHeights[potentialExitRow - 1][
                        potentialExitCol] <= 
                        enhancedHeights[potentialExitRow][
                        potentialExitCol] + 1) &&
                        (enhancedHeights[potentialExitRow - 1][
                        potentialExitCol] >= 
                        enhancedHeights[potentialExitRow][
                        potentialExitCol] - 1)) && 
                        ((enhancedHeights[potentialExitRow - 2][
                        potentialExitCol] <= 
                        enhancedHeights[potentialExitRow - 1][
                        potentialExitCol] + 1) &&
                        (enhancedHeights[potentialExitRow - 2][
                        potentialExitCol] >= 
                        enhancedHeights[potentialExitRow - 1][
                        potentialExitCol] - 1)) &&
                        (enhancedHeights[potentialExitRow - 2][
                        potentialExitCol] >= 
                        enhancedHeights[potentialExitRow - 2][
                        potentialExitCol - 1]) &&
                        (enhancedHeights[potentialExitRow - 2][
                        potentialExitCol] >= 
                        enhancedHeights[potentialExitRow - 2][
                        potentialExitCol + 1]) &&
                        (extendedStructure[potentialExitRow - 2][
                        potentialExitCol] == 'p')) { 

                        extendedStructure[potentialExitRow - 1][
                            potentialExitCol] = '.';
                        exitFound = true;
                    }
                    else if (upDownLeftRight == 1 &&
                        ((enhancedHeights[potentialExitRow + 1][
                        potentialExitCol] <= 
                        enhancedHeights[potentialExitRow][
                        potentialExitCol] + 1) &&
                        (enhancedHeights[potentialExitRow + 1][
                        potentialExitCol] >= 
                        enhancedHeights[potentialExitRow][
                        potentialExitCol] - 1)) && 
                        ((enhancedHeights[potentialExitRow + 2][
                        potentialExitCol] <= 
                        enhancedHeights[potentialExitRow + 1][
                        potentialExitCol] + 1) &&
                        (enhancedHeights[potentialExitRow + 2][
                        potentialExitCol] >= 
                        enhancedHeights[potentialExitRow + 1][
                        potentialExitCol] - 1)) &&
                        (enhancedHeights[potentialExitRow + 2][
                        potentialExitCol] >= 
                        enhancedHeights[potentialExitRow + 2][
                        potentialExitCol - 1]) &&
                        (enhancedHeights[potentialExitRow + 2][
                        potentialExitCol] >= 
                        enhancedHeights[potentialExitRow + 2][
                        potentialExitCol + 1]) &&
                        (extendedStructure[potentialExitRow + 2][
                        potentialExitCol] == 'p')) { 

                        extendedStructure[potentialExitRow + 1][
                            potentialExitCol] = '.';
                        exitFound = true;
                    }
                    else if (upDownLeftRight == 2 &&
                        ((enhancedHeights[potentialExitRow][
                        potentialExitCol - 1] <= 
                        enhancedHeights[potentialExitRow][
                        potentialExitCol] + 1) &&
                        (enhancedHeights[potentialExitRow][
                        potentialExitCol - 1] >= 
                        enhancedHeights[potentialExitRow][
                        potentialExitCol] - 1)) && 
                        ((enhancedHeights[potentialExitRow][
                        potentialExitCol - 2] <= 
                        enhancedHeights[potentialExitRow][
                        potentialExitCol - 1] + 1) &&
                        (enhancedHeights[potentialExitRow][
                        potentialExitCol - 2] >= 
                        enhancedHeights[potentialExitRow][
                        potentialExitCol - 1] - 1)) &&
                        (enhancedHeights[potentialExitRow][
                        potentialExitCol - 2] >= 
                        enhancedHeights[potentialExitRow - 1][
                        potentialExitCol - 2]) &&
                        (enhancedHeights[potentialExitRow][
                        potentialExitCol - 2] >= 
                        enhancedHeights[potentialExitRow + 1][
                        potentialExitCol - 2]) &&
                        (extendedStructure[potentialExitRow][
                        potentialExitCol - 2] == 'p')) {

                        extendedStructure[potentialExitRow][
                        potentialExitCol - 1] = '.';
                        exitFound = true;
                    }
                    else if (upDownLeftRight == 3 &&
                        ((enhancedHeights[potentialExitRow][
                        potentialExitCol + 1] <= 
                        enhancedHeights[potentialExitRow][
                        potentialExitCol] + 1) &&
                        (enhancedHeights[potentialExitRow][
                        potentialExitCol + 1] >= 
                        enhancedHeights[potentialExitRow][
                        potentialExitCol] - 1)) && 
                        ((enhancedHeights[potentialExitRow][
                        potentialExitCol + 2] <= 
                        enhancedHeights[potentialExitRow][
                        potentialExitCol + 1] + 1) &&
                        (enhancedHeights[potentialExitRow][
                        potentialExitCol + 2] >= 
                        enhancedHeights[potentialExitRow][
                        potentialExitCol + 1] - 1)) &&
                        (enhancedHeights[potentialExitRow][
                        potentialExitCol + 2] >= 
                        enhancedHeights[potentialExitRow - 1][
                        potentialExitCol + 2]) &&
                        (enhancedHeights[potentialExitRow][
                        potentialExitCol + 2] >= 
                        enhancedHeights[potentialExitRow + 1][
                        potentialExitCol + 2]) &&
                        (extendedStructure[potentialExitRow][
                        potentialExitCol + 2] == 'p')) { 

                        extendedStructure[potentialExitRow][
                            potentialExitCol + 1] = '.';
                        exitFound = true;
                    }
                    
                    if (exitFound) {
                        extendedStructure[potentialExitRow][
                            potentialExitCol] = '.';
                        creationStartRow = potentialExitRow - 1;
                        creationStartCol = potentialExitCol - 1;
                        creationCurrRow = creationStartRow;
                        creationCurrCol = creationStartCol;
                        Node* firstMultiOptionPtr = 
                            new Node(sentinelLastMultiOptionPtr);
                        firstMultiOptionPtr->setDataRow(creationStartRow);
                        firstMultiOptionPtr->setDataCol(creationStartCol);
                        firstMultiOptionPtr->insertAtFront();
                    }
                }
            }
        }
        else {
            extendedStructure[2][2] = '.';
            extendedStructure[2][1] = '.';
            creationStartRow = 1;
            creationStartCol = 1;
            creationCurrRow = creationStartRow;
            creationCurrCol = creationStartCol;
            Node* firstMultiOptionPtr = new Node(sentinelLastMultiOptionPtr);
            firstMultiOptionPtr->setDataRow(creationStartRow);
            firstMultiOptionPtr->setDataCol(creationStartCol);
            firstMultiOptionPtr->insertAtFront();
        }

        sentinelGridPtr->deleteAll();
        sentinelGridPtr = nullptr;
        sentinelTempPtr->deleteAll();
        sentinelTempPtr = nullptr;

        // Replacing extended structure with structure (perimeter now removed). 
        structure = new char*[this->xlen];
        for (unsigned int i = 0; i < this->xlen; ++i) {
            structure[i] = new char[this->zlen];   
        }

        for (unsigned int i = 1; i < this->xlen + 1; ++i) {
            for (unsigned int j = 1; j < this->zlen + 1; ++j) {
                structure[i - 1][j - 1] = extendedStructure[i][j];
            }   
        }

        if (extendedStructure != nullptr) {
            for (unsigned int i = 0; i < this->xlen + 2; ++i) {
                delete[] extendedStructure[i];
                extendedStructure[i] = nullptr;
            }

            delete[] extendedStructure;
            extendedStructure = nullptr;
        }
    
    }
    else {
        setDimensions(0, 0);
        throw std::runtime_error(
            "Invalid basePoint for maze, cannot build here due to obstacles.");
    }
}

void Maze::setMaxReachableHeight(int maxReachableHeight) {
    this->maxReachableHeight = maxReachableHeight;
}

int Maze::getMaxReachableHeight() {
    return this->maxReachableHeight;
}

void Maze::setPrevMaxReachableHeight(int prevMaxReachableHeight) {
    this->prevMaxReachableHeight = prevMaxReachableHeight;
}

int Maze::getPrevMaxReachableHeight() {
    return this->prevMaxReachableHeight;
}

void Maze::createMinecraftMazeEnhanced() {
    mcpp::SocketConnection connection("localhost", 4711);
    mcpp::MinecraftConnection minecraft;
    mcpp::Coordinate viewPoint(basePoint.x, basePoint.y+10, basePoint.z);
    minecraft.setPlayerPosition(viewPoint);
    minecraft.doCommand("tp @s ~ ~ ~ -45 45");
    // Initialised to 0 as air block
    mcpp::BlockType currBlock(0);
    this->setMaxReachableHeight(enhancedHeights[1][1]);

    // Find maximum reachable height within structure;
    for (int i = 0; i < static_cast<int>(this->xlen); i++) {
        for (int j = 0; j < static_cast<int>(this->zlen); j++) {
            if (structure[i][j] == '.' && 
                (enhancedHeights[i + 1][j + 1] > 
                this->getMaxReachableHeight())) {
                this->setMaxReachableHeight(enhancedHeights[i + 1][j + 1]);
            }
        }
    }

    for (int i = 0; i < static_cast<int>(this->xlen); i++) {
        for (int j = 0; j < static_cast<int>(this->zlen); j++) { 
            if (structure[i][j] == 'x') {
                currBlock = wallID;
                if (enhancedHeights[i + 1][j + 1] < 
                    this->getMaxReachableHeight() + 3) {
                    for (int k = this->getMaxReachableHeight() + 3; 
                        k > enhancedHeights[i + 1][j + 1]; --k) {
                        minecraft.setBlock(mcpp::Coordinate(basePoint.x + i, k, 
                            basePoint.z + j), currBlock);
                        //50ms delay
                        std::this_thread::sleep_for(std::chrono::milliseconds(50));
                    }
                }
                else {
                    minecraft.setBlock(mcpp::Coordinate(basePoint.x + i, 
                        enhancedHeights[i + 1][j + 1] + 1, basePoint.z + j), 
                        currBlock);
                    //50ms delay
                    std::this_thread::sleep_for(std::chrono::milliseconds(50));  
                }
            } else if (structure[i][j] == '.' || structure[i][j] == 'o') {
                currBlock = airID;
                //50ms delay
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            } else {
                //Throw error when char in structure is unexpected
                throw end_of_file_error(
                    "Unexpected char within maze structure");                    
            }
        }
    }
    return;
}

void Maze::resetMinecraftMazeEnhanced() {
    mcpp::SocketConnection connection("localhost", 4711);
    mcpp::MinecraftConnection minecraft;
    
    for (int i = 0; i < static_cast<int>(this->prevXLen); i++) {
        for (int j = 0; j < static_cast<int>(this->prevZLen); j++) {
            if (this->prevStructure[i][j] == 'x') {
                if (enhancedHeights[i + 1][j + 1] < 
                    this->getMaxReachableHeight() + 3) {
                    for (int k = this->getMaxReachableHeight() + 3; 
                        k > enhancedHeights[i + 1][j + 1]; --k) {
                        minecraft.setBlock(mcpp::Coordinate(
                            prevBasePoint.x + i, k, 
                            prevBasePoint.z + j), this->airID);
                        //50ms delay
                        std::this_thread::sleep_for(
                            std::chrono::milliseconds(50));
                    }
                }
                else {
                    minecraft.setBlock(mcpp::Coordinate(prevBasePoint.x + i, 
                        enhancedHeights[i + 1][j + 1] + 1, 
                        prevBasePoint.z + j), 
                        this->airID);
                    //50ms delay
                    std::this_thread::sleep_for(std::chrono::milliseconds(50));
                }
            } 
            else if (this->prevStructure[i][j] == '.' || 
                this->prevStructure[i][j] == 'o') {
                //50ms delay
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
            else {
                throw end_of_file_error(
                    "Unexpected char within maze structure");                    
            }
        }
    }
    return;
}

void Maze::resetPrevMinecraftMazeEnhanced() {
    mcpp::SocketConnection connection("localhost", 4711);
    mcpp::MinecraftConnection minecraft;
    
    for (int i = 0; i < static_cast<int>(this->prevXLen); i++) {
        for (int j = 0; j < static_cast<int>(this->prevZLen); j++) {
            if (this->prevStructure[i][j] == 'x') {
                if (prevEnhancedHeights[i + 1][j + 1] < 
                    this->getPrevMaxReachableHeight() + 3) {
                    for (int k = this->getPrevMaxReachableHeight() + 3; 
                        k > prevEnhancedHeights[i + 1][j + 1]; --k) {
                        minecraft.setBlock(mcpp::Coordinate(
                            prevBasePoint.x + i, k, 
                            prevBasePoint.z + j), this->airID);
                        //50ms delay
                        std::this_thread::sleep_for(
                            std::chrono::milliseconds(50));
                    }
                }
                else {
                    minecraft.setBlock(mcpp::Coordinate(prevBasePoint.x + i, 
                        prevEnhancedHeights[i + 1][j + 1] + 1, 
                        prevBasePoint.z + j), 
                        this->airID);
                    //50ms delay
                    std::this_thread::sleep_for(std::chrono::milliseconds(50));
                }
            } 
            else if (this->prevStructure[i][j] == '.' || 
                this->prevStructure[i][j] == 'o') {
                //50ms delay
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
            else {
                throw end_of_file_error(
                    "Unexpected char within maze structure");                    
            }
        }
    }
    return;
}