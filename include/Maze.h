#ifndef ASSIGN_MAZE_H
#define ASSIGN_MAZE_H

#include "Node.h"
#include "CoordinateBlock.h"
#include <mcpp/mcpp.h>
#include <deque>
using std::deque;

class Maze
{
public:
    Maze(mcpp::Coordinate basePoint = mcpp::Coordinate(0, 0, 0), 
        unsigned int xlen = 0, unsigned int zlen = 0, 
        bool testingMode = 0, bool enhancementsMode = 0);
    ~Maze();
    mcpp::Coordinate getBasePoint() const;
    mcpp::Coordinate getPrevBasePoint() const;
    void setBasePoint(mcpp::Coordinate&);
    void setDimensions(unsigned int xlen, unsigned int zlen);
    char** getStructure() const;
    char** getPrevStructure() const;
    unsigned int getLength() const;
    unsigned int getWidth() const;
    unsigned int getPrevLength() const;
    unsigned int getPrevWidth() const;
    void printMaze() const;
    void setInitialStructure();
    void removeStructure();
    void generateRandomMaze(bool enhanced);
    void readInUserMaze();
    void createMinecraftMaze() const;
    void resetMinecraftMaze() const;
    bool getTestingMode() const;
    bool getEnhancementsMode() const;
    void resetPrevMinecraftMaze() const;
    void setPrevStructure();
    //This bool stores if a new maze has been generated
    bool newMaze = false;
    bool newEnhancedMaze = false;
    bool prevEnhancedMaze = false;
    bool inMinecraft = false;
    void createMazeFoundation();
    void setMazeFoundation();
    void ResetMazeFoundation();
    void getAreaHeight();
    void findMaxReachable();
    void setInitialStructureEnhanced();
    void createMinecraftMazeEnhanced();
    void setMaxReachableHeight(int maxReachableHeight);
    int getMaxReachableHeight();
    void setPrevMaxReachableHeight(int prevMaxReachableHeight);
    int getPrevMaxReachableHeight();
    void resetMinecraftMazeEnhanced();
    void resetPrevMinecraftMazeEnhanced();
    
private:
    mcpp::Coordinate basePoint; 
    mcpp::Coordinate prevBasePoint;
    // Structure is a 2D array of chars indicating walls, paths and obstacles.
    char** structure = nullptr;
    char** prevStructure = nullptr;
    unsigned int xlen;
    unsigned int zlen;
    unsigned int prevXLen;
    unsigned int prevZLen;
    bool testingMode;
    bool enhancementsMode;
    // These rows and columns relate to recursive maze generation.
    int creationStartRow;
    int creationStartCol;
    int creationCurrRow;
    int creationCurrCol;
    // This 2D array of ints contains heights read from the terrain.
    int** enhancedHeights = nullptr;
    int** prevEnhancedHeights = nullptr;
    // A queue of points within the maze where mulitple paths can be built. 
    Node* sentinelLastMultiOptionPtr = new Node();
    // Starting from the base point, a list of points reachable by 1 block.
    Node* sentinelReachablePtr = new Node();
    int maxReachableHeight;
    int prevMaxReachableHeight;
    const int height = 3;
    const mcpp::BlockType wallID = mcpp::Blocks::ACACIA_WOOD_PLANK;
    
    //Maze foundation members
    const mcpp::BlockType floorID = mcpp::Blocks::GRASS;
    const mcpp::BlockType fillID = mcpp::Blocks::DIRT;
    const mcpp::BlockType airID = mcpp::Blocks::AIR;
    deque<deque<deque<mcpp::CoordinateBlock>>> envID;
};




#endif //ASSIGN_MAZE_H
