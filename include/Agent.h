#ifndef ASSIGN3_AGENT_H
#define ASSIGN3_AGENT_H

#include "Maze.h"
#include <mcpp/mcpp.h>

#define MOVE_XPLUS mcpp::Coordinate(1,0,0)
#define MOVE_XMINUS mcpp::Coordinate(-1,0,0)
#define MOVE_ZPLUS mcpp::Coordinate(0,0,1)
#define MOVE_ZMINUS mcpp::Coordinate(0,0,-1)
#define MOVE_UP mcpp::Coordinate (0, 1, 0)
#define MOVE_DOWN mcpp::Coordinate(0, -1, 0)
#define WALL_HEIGHT 3
#define WALL_HEIGHT_BFS 2

enum solveAlgorithm{
        RIGHT_HAND_FOLLOW,
        BREATH_FIRST_SEARCH,
};

enum AgentOrientation{
    X_PLUS,
    Z_PLUS,
    X_MINUS,
    Z_MINUS
};

class Agent
{

public:
    Agent(mcpp::Coordinate playerLoc);
    ~Agent();
    void showPath(bool enhanced);
    void wallFollow(bool enhanced);
    void BFS(bool enhanced);
    void pop_front(std::vector<std::pair<mcpp::Coordinate, 
        mcpp::Coordinate>> &myVector);
    void reverse(mcpp::Coordinate &myDirection);
    AgentOrientation getOrientation() const;
    


private:
    /* data */
    mcpp::MinecraftConnection mc;
    AgentOrientation orientation;
    std::vector<mcpp::Coordinate> path;
    std::vector<std::pair<mcpp::Coordinate, mcpp::Coordinate>> visited;
    std::vector<std::pair<mcpp::Coordinate, mcpp::Coordinate>> queue;
    std::vector<mcpp::Coordinate> shortestPath;
    
};



#endif //ASSIGN3_AGENT_H