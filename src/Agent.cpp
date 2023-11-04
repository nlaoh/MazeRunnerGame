#include "../include/Agent.h"
#include <chrono>
#include <thread>
#include <iostream>

Agent::Agent(mcpp::Coordinate playerLoc)
{
}

Agent::~Agent()
{
}

void Agent::wallFollow(bool enhanced) {
    
        std::cout << "Finding Path..."<< std::endl;
    mcpp::Coordinate rightHand = mc.getPlayerPosition();
    mcpp::BlockType directionXPlus = mc.getBlock(rightHand + MOVE_ZPLUS);
    mcpp::BlockType directionZPlus = mc.getBlock(rightHand + MOVE_XMINUS);
    mcpp::BlockType directionXMinus = mc.getBlock(rightHand + MOVE_ZMINUS);
    mcpp::BlockType directionZMinus = mc.getBlock(rightHand + MOVE_XPLUS);
    //assume that player's right hand is touching wall
    if (!(directionXPlus == mcpp::Blocks::AIR)) {
       orientation = X_PLUS; 
    }
    else if (!(directionZPlus == mcpp::Blocks::AIR)) {
       orientation = Z_PLUS; 
    }
    else if (!(directionXMinus == mcpp::Blocks::AIR)) {
       orientation = X_MINUS; 
    }
    else if (!(directionZMinus == mcpp::Blocks::AIR)) {
       orientation = Z_MINUS; 
    }
    if (enhanced){
    rightHand.y = mc.getHeight (rightHand.x, rightHand.z) + WALL_HEIGHT;
    }
    while (mc.getBlock(rightHand + MOVE_XPLUS) == 
        mcpp::Blocks::ACACIA_WOOD_PLANK || 
        mc.getBlock(rightHand + MOVE_ZPLUS) == mcpp::Blocks::ACACIA_WOOD_PLANK || 
        mc.getBlock(rightHand + MOVE_XMINUS) == mcpp::Blocks::ACACIA_WOOD_PLANK ||
        mc.getBlock(rightHand + MOVE_ZMINUS) == mcpp::Blocks::ACACIA_WOOD_PLANK) {
        
        if (orientation == X_PLUS) {
            rightHand = rightHand + MOVE_ZPLUS;
        } else if (orientation == Z_PLUS) {
            rightHand = rightHand + MOVE_XMINUS;
        } else if (orientation == X_MINUS) {
            rightHand = rightHand + MOVE_ZMINUS;
        } else if (orientation == Z_MINUS) {
            rightHand = rightHand + MOVE_XPLUS;
        }
        mcpp::BlockType blockID = mc.getBlock(rightHand);

        if (!(blockID == mcpp::Blocks::AIR)) {
            // Turn left if there's a wall on the right
            if (orientation == X_PLUS) {
            rightHand = rightHand - MOVE_ZPLUS;
        } else if (orientation == Z_PLUS) {
            rightHand = rightHand - MOVE_XMINUS;
        } else if (orientation == X_MINUS) {
            rightHand = rightHand - MOVE_ZMINUS;
        } else if (orientation == Z_MINUS) {
            rightHand = rightHand - MOVE_XPLUS;
        }
            orientation = static_cast<AgentOrientation>((orientation + 3) % 4);
        } else {
            orientation = static_cast<AgentOrientation>((orientation + 1) % 4);
            path.push_back(rightHand);
        }
        if (enhanced){
    rightHand.y = mc.getHeight (rightHand.x, rightHand.z) + WALL_HEIGHT;
    }
    }
}


void Agent::BFS(bool enhanced){
    mcpp::Coordinate BFSstart = mc.getPlayerPosition();
    queue.push_back(std::make_pair(BFSstart, mcpp::Coordinate(0, 0, 0)));
    mcpp::Coordinate thisCoord = BFSstart;
    mcpp::Coordinate currDirection = mcpp::Coordinate(0, 0, 0);
    visited.push_back(std::make_pair(thisCoord, currDirection));
    bool victory = false;
    bool isVisited = false;
    int count = 0;
    mcpp::Coordinate exitCoord;
    int currHeight = 0;
    mcpp::Coordinate clone;
    mcpp::Coordinate trueDirection;
    //while the queue isn't empty
    if(enhanced == false){
    while(victory == false){
        if(mc.getBlock(thisCoord + MOVE_XPLUS) == 
            mcpp::Blocks::ACACIA_WOOD_PLANK){
        }
        else{
            isVisited = false;
            currDirection = MOVE_XPLUS;
            long unsigned int i;
            for(i = 0; i < visited.size(); ++i){
                if((thisCoord + MOVE_XPLUS) == visited[i].first){
                    /*
                     * std::cout << "Visited [" << i << "]: " 
                     * << visited[i].first.x << ", " << visited[i].first.y 
                     * << ", " << visited[i].first.z << std::endl << std::endl;
                     */
                    isVisited = true;
                }
            }
            if(isVisited == false){
                visited.push_back(std::make_pair(thisCoord + MOVE_XPLUS, 
                    currDirection));
                queue.push_back(std::make_pair(thisCoord + MOVE_XPLUS, 
                    currDirection));
            }
        }
        if(mc.getBlock(thisCoord + MOVE_XMINUS) == 
            mcpp::Blocks::ACACIA_WOOD_PLANK){
        }
        else{
            isVisited = false;
            currDirection = MOVE_XMINUS;
            long unsigned int i;
            for(i = 0; i < visited.size(); ++i){
                if((thisCoord + MOVE_XMINUS) == visited[i].first){
                    isVisited = true;
                }
            }
            if(isVisited == false){
                visited.push_back(std::make_pair(thisCoord + MOVE_XMINUS, 
                    currDirection));
                queue.push_back(std::make_pair(thisCoord + MOVE_XMINUS, 
                    currDirection));
            }
        }
        if(mc.getBlock(thisCoord + MOVE_ZPLUS) == 
            mcpp::Blocks::ACACIA_WOOD_PLANK){
        }
        else{
            isVisited = false;
            currDirection = MOVE_ZPLUS;
            long unsigned int i;
            for(i = 0; i < visited.size(); ++i){
                if((thisCoord + MOVE_ZPLUS) == visited[i].first){
                    isVisited = true;
                }
            }
            if(isVisited == false){
                visited.push_back(std::make_pair(thisCoord + 
                    MOVE_ZPLUS, currDirection));
                queue.push_back(std::make_pair(thisCoord + MOVE_ZPLUS, 
                    currDirection));
            }
        }
        if(mc.getBlock(thisCoord + MOVE_ZMINUS) == 
            mcpp::Blocks::ACACIA_WOOD_PLANK){
        }
        else{
            isVisited = false;
            currDirection = MOVE_ZMINUS;
            long unsigned int i;
            for(i = 0; i < visited.size(); ++i){
                if((thisCoord + MOVE_ZMINUS) == visited[i].first){
                    isVisited = true;
                }
            }
            if(isVisited == false){
                visited.push_back(std::make_pair(thisCoord + MOVE_ZMINUS, 
                    currDirection));
                queue.push_back(std::make_pair(thisCoord + MOVE_ZMINUS, 
                    currDirection));
            }
        }
        //check for exit
        if((mc.getBlock(thisCoord + MOVE_XPLUS) == 
            mcpp::Blocks::ACACIA_WOOD_PLANK) || 
            (mc.getBlock(thisCoord + MOVE_XMINUS) == 
            mcpp::Blocks::ACACIA_WOOD_PLANK) || 
            (mc.getBlock(thisCoord + MOVE_ZPLUS) == 
            mcpp::Blocks::ACACIA_WOOD_PLANK) || 
            (mc.getBlock(thisCoord + MOVE_ZMINUS) == 
            mcpp::Blocks::ACACIA_WOOD_PLANK)){
            //remove first in queue
            count += 1;
            pop_front(queue);
            //visit next in queue
            thisCoord = queue[0].first;
            currDirection = queue[0].second;
            std::cout << "Count: " << count << std::endl;
            std::cout << "This coord: " << thisCoord.x << ", " << thisCoord.y 
                << ", " << thisCoord.z << std::endl;
            std::cout << "This direction: " << currDirection.x << ", " 
                << currDirection.y << ", " << currDirection.z << std::endl;
        }
        else{
            victory = true;
            exitCoord = thisCoord;
            shortestPath.push_back(exitCoord);
            std::cout << "Exit: " << exitCoord.x << ", " << exitCoord.y << ", " 
                << exitCoord.z << std::endl;
        }
    }
    }
    else if(enhanced == true){
        while(victory == false){
            //x positive
        if((mc.getBlock(thisCoord + MOVE_XPLUS) == 
            mcpp::Blocks::AIR) && 
            !(mc.getBlock(thisCoord + MOVE_XPLUS + MOVE_DOWN)
            == mcpp::Blocks::AIR)) {
            isVisited = false;
            currDirection = MOVE_XPLUS;
            long unsigned int i;
            for(i = 0; i < visited.size(); ++i){
                if((thisCoord + MOVE_XPLUS) == visited[i].first){
                    /*
                     * std::cout << "Visited [" << i << "]: " 
                     * << visited[i].first.x << ", " << visited[i].first.y 
                     * << ", " << visited[i].first.z << std::endl << std::endl;
                     */
                    isVisited = true;
                }
            }
            if(isVisited == false){
                visited.push_back(std::make_pair(thisCoord + MOVE_XPLUS, 
                    currDirection));
                queue.push_back(std::make_pair(thisCoord + MOVE_XPLUS, 
                    currDirection));
            }
        }
            //x positive and up one block
        if(!(mc.getBlock(thisCoord + MOVE_XPLUS) == 
            mcpp::Blocks::AIR) && 
            (mc.getBlock(thisCoord + MOVE_XPLUS + MOVE_UP) 
            == mcpp::Blocks::AIR)){
            isVisited = false;
            currDirection = mcpp::Coordinate(1, 1, 0);
            long unsigned int i;
            for(i = 0; i < visited.size(); ++i){
                if((thisCoord + MOVE_XPLUS + MOVE_UP) == visited[i].first){
                    isVisited = true;
                }
            }
            if(isVisited == false){
                visited.push_back(std::make_pair(thisCoord + 
                    MOVE_XPLUS + MOVE_UP, currDirection));
                queue.push_back(std::make_pair(thisCoord + 
                    MOVE_XPLUS + MOVE_UP, currDirection));
            }
        }
            //x positive and down one block
        if((mc.getBlock(thisCoord + MOVE_XPLUS) == 
            mcpp::Blocks::AIR) && 
            (mc.getBlock(thisCoord + MOVE_XPLUS + MOVE_DOWN) 
            == mcpp::Blocks::AIR)){
            isVisited = false;
            currDirection = mcpp::Coordinate(1, -1, 0);
            long unsigned int i;
            for(i = 0; i < visited.size(); ++i){
                if((thisCoord + MOVE_XPLUS + MOVE_DOWN) == visited[i].first){
                    isVisited = true;
                }
            }
            if(isVisited == false){
                visited.push_back(std::make_pair(thisCoord + 
                    MOVE_XPLUS + MOVE_DOWN, currDirection));
                queue.push_back(std::make_pair(thisCoord + 
                    MOVE_XPLUS + MOVE_DOWN, currDirection));
            }
        }
            //x negative
        if((mc.getBlock(thisCoord + MOVE_XMINUS) == 
            mcpp::Blocks::AIR) && !(mc.getBlock(thisCoord + 
            MOVE_XMINUS + MOVE_DOWN) == mcpp::Blocks::AIR)){
            isVisited = false;
            currDirection = MOVE_XMINUS;
            long unsigned int i;
            for(i = 0; i < visited.size(); ++i){
                if((thisCoord + MOVE_XMINUS) == visited[i].first){
                    isVisited = true;
                }
            }
            if(isVisited == false){
                visited.push_back(std::make_pair(thisCoord + MOVE_XMINUS, 
                    currDirection));
                queue.push_back(std::make_pair(thisCoord + MOVE_XMINUS, 
                    currDirection));
            }
        }
            //x negative and up one block
        if(!(mc.getBlock(thisCoord + MOVE_XMINUS) == 
            mcpp::Blocks::AIR) && (mc.getBlock(thisCoord + 
            MOVE_XMINUS + MOVE_UP) == mcpp::Blocks::AIR)){
            isVisited = false;
            currDirection = mcpp::Coordinate(-1, 1, 0);
            long unsigned int i;
            for(i = 0; i < visited.size(); ++i){
                if((thisCoord + MOVE_XMINUS + MOVE_UP) == visited[i].first){
                    isVisited = true;
                }
            }
            if(isVisited == false){
                visited.push_back(std::make_pair(thisCoord + 
                    MOVE_XMINUS + MOVE_UP, currDirection));
                queue.push_back(std::make_pair(thisCoord + 
                    MOVE_XMINUS + MOVE_UP, currDirection));
            }
        }
            //x negative and down one block
        if((mc.getBlock(thisCoord + MOVE_XMINUS) == 
            mcpp::Blocks::AIR) && (mc.getBlock(thisCoord + 
            MOVE_XMINUS + MOVE_DOWN) == mcpp::Blocks::AIR)){
            isVisited = false;
            currDirection = mcpp::Coordinate(-1, -1, 0);
            long unsigned int i;
            for(i = 0; i < visited.size(); ++i){
                if((thisCoord + MOVE_XMINUS + MOVE_DOWN) == visited[i].first){
                    isVisited = true;
                }
            }
            if(isVisited == false){
                visited.push_back(std::make_pair(thisCoord + 
                    MOVE_XMINUS + MOVE_DOWN, currDirection));
                queue.push_back(std::make_pair(thisCoord + 
                    MOVE_XMINUS + MOVE_DOWN, currDirection));
            }
        }
            //z positive
        if((mc.getBlock(thisCoord + MOVE_ZPLUS) == 
            mcpp::Blocks::AIR) && !(mc.getBlock(thisCoord + 
            MOVE_ZPLUS + MOVE_DOWN) == mcpp::Blocks::AIR)){
            isVisited = false;
            currDirection = MOVE_ZPLUS;
            long unsigned int i;
            for(i = 0; i < visited.size(); ++i){
                if((thisCoord + MOVE_ZPLUS) == visited[i].first){
                    isVisited = true;
                }
            }
            if(isVisited == false){
                visited.push_back(std::make_pair(thisCoord + 
                    MOVE_ZPLUS, currDirection));
                queue.push_back(std::make_pair(thisCoord + MOVE_ZPLUS, 
                    currDirection));
            }
        }
            //z positive and up one block
        if(!(mc.getBlock(thisCoord + MOVE_ZPLUS) == 
            mcpp::Blocks::AIR) && (mc.getBlock(thisCoord + 
            MOVE_ZPLUS + MOVE_UP) == mcpp::Blocks::AIR)){
            isVisited = false;
            currDirection = mcpp::Coordinate(0, 1, 1);
            long unsigned int i;
            for(i = 0; i < visited.size(); ++i){
                if((thisCoord + MOVE_ZPLUS + MOVE_UP) == visited[i].first){
                    isVisited = true;
                }
            }
            if(isVisited == false){
                visited.push_back(std::make_pair(thisCoord + 
                    MOVE_ZPLUS + MOVE_UP, currDirection));
                queue.push_back(std::make_pair(thisCoord + 
                    MOVE_ZPLUS + MOVE_UP, currDirection));
            }
        }
            //z positive and down one block
        if((mc.getBlock(thisCoord + MOVE_ZPLUS) == 
            mcpp::Blocks::AIR) && (mc.getBlock(thisCoord + 
            MOVE_ZPLUS + MOVE_DOWN) == mcpp::Blocks::AIR)){
            isVisited = false;
            currDirection = mcpp::Coordinate(0, -1, 1);
            long unsigned int i;
            for(i = 0; i < visited.size(); ++i){
                if((thisCoord + MOVE_ZPLUS + MOVE_DOWN) == visited[i].first){
                    isVisited = true;
                }
            }
            if(isVisited == false){
                visited.push_back(std::make_pair(thisCoord + 
                    MOVE_ZPLUS + MOVE_DOWN, currDirection));
                queue.push_back(std::make_pair(thisCoord + 
                    MOVE_ZPLUS + MOVE_DOWN, currDirection));
            }
        }
            //z negative
        if((mc.getBlock(thisCoord + MOVE_ZMINUS) == 
            mcpp::Blocks::AIR) && !(mc.getBlock(thisCoord + 
            MOVE_ZMINUS + MOVE_DOWN) == mcpp::Blocks::AIR)){
            isVisited = false;
            currDirection = MOVE_ZMINUS;
            long unsigned int i;
            for(i = 0; i < visited.size(); ++i){
                if((thisCoord + MOVE_ZMINUS) == visited[i].first){
                    isVisited = true;
                }
            }
            if(isVisited == false){
                visited.push_back(std::make_pair(thisCoord + MOVE_ZMINUS, 
                    currDirection));
                queue.push_back(std::make_pair(thisCoord + MOVE_ZMINUS, 
                    currDirection));
            }
        }
            //z negative and up one block
        if(!(mc.getBlock(thisCoord + MOVE_ZMINUS) == 
            mcpp::Blocks::AIR) && (mc.getBlock(thisCoord + 
            MOVE_ZMINUS + MOVE_UP) == mcpp::Blocks::AIR)){
            isVisited = false;
            currDirection = mcpp::Coordinate(0, 1, -1);
            long unsigned int i;
            for(i = 0; i < visited.size(); ++i){
                if((thisCoord + MOVE_ZMINUS + MOVE_UP) == visited[i].first){
                    isVisited = true;
                }
            }
            if(isVisited == false){
                visited.push_back(std::make_pair(thisCoord + 
                    MOVE_ZMINUS + MOVE_UP, currDirection));
                queue.push_back(std::make_pair(thisCoord + 
                    MOVE_ZMINUS + MOVE_UP, currDirection));
            }
        }
            //z negative and down one block
        if((mc.getBlock(thisCoord + MOVE_ZMINUS) == 
            mcpp::Blocks::AIR) && (mc.getBlock(thisCoord + 
            MOVE_ZMINUS + MOVE_DOWN) == mcpp::Blocks::AIR)){
            isVisited = false;
            currDirection = mcpp::Coordinate(0, -1, -1);
            long unsigned int i;
            for(i = 0; i < visited.size(); ++i){
                if((thisCoord + MOVE_ZMINUS + MOVE_DOWN) == visited[i].first){
                    isVisited = true;
                }
            }
            if(isVisited == false){
                visited.push_back(std::make_pair(thisCoord + 
                    MOVE_ZMINUS + MOVE_DOWN, currDirection));
                queue.push_back(std::make_pair(thisCoord + 
                    MOVE_ZMINUS + MOVE_DOWN, currDirection));
            }
        }
        //check for exit
        currHeight = thisCoord.y;
        clone = thisCoord;
        clone.y = currHeight + WALL_HEIGHT_BFS;
        if((mc.getBlock(clone + MOVE_XPLUS) == 
            mcpp::Blocks::ACACIA_WOOD_PLANK) || 
            (mc.getBlock(clone + MOVE_XMINUS) == 
            mcpp::Blocks::ACACIA_WOOD_PLANK) || 
            (mc.getBlock(clone + MOVE_ZPLUS) == 
            mcpp::Blocks::ACACIA_WOOD_PLANK) || 
            (mc.getBlock(clone + MOVE_ZMINUS) == 
            mcpp::Blocks::ACACIA_WOOD_PLANK)){
            //remove first in queue
            count += 1;
            pop_front(queue);
            //visit next in queue
            thisCoord = queue[0].first;
            currDirection = queue[0].second;
            std::cout << "Count: " << count << std::endl;
            std::cout << "This coord: " << thisCoord.x << ", " << thisCoord.y 
                << ", " << thisCoord.z << std::endl;
            std::cout << "This direction: " << currDirection.x << ", " 
                << currDirection.y << ", " << currDirection.z << std::endl;
            trueDirection.x = currDirection.x;
            trueDirection.y = currDirection.y;
            trueDirection.z = currDirection.z;
        }
        else{
            victory = true;
            exitCoord = thisCoord;
            std::cout << "Exit: " << exitCoord.x << ", " << exitCoord.y << ", " 
                << exitCoord.z << std::endl;
            std::cout << "This direction: " << currDirection.x << ", "
                << currDirection.y << ", " << currDirection.z << std::endl;
            std::cout << "True direction: " << trueDirection.x << ", "
                << trueDirection.y << ", " << trueDirection.z << std::endl;
            shortestPath.push_back(exitCoord);
            currDirection.x = trueDirection.x;
            currDirection.y = trueDirection.y;
            currDirection.z = trueDirection.z;
        }
    }
    }
    victory = false;
    //shortestPath.push_back(exitCoord);
    while(victory == false){
        reverse(currDirection);
        thisCoord = thisCoord + currDirection;
        long unsigned int i;
        for(i = 0; i < visited.size(); ++i){
            if(thisCoord == visited[i].first){
                shortestPath.push_back(thisCoord);
                currDirection = visited[i].second;
            }
        }
        if(thisCoord == BFSstart){
            victory = true;
        }
    }
}



void Agent::showPath(bool enhanced){
    mcpp::Coordinate Y_PLUS = mcpp::Coordinate(0,1,0);
    mcpp::Coordinate Y_MINUS = mcpp::Coordinate (0,-1,0);
    if(enhanced == false){
        int i=0;
        mcpp::BlockType myBlock = mc.getBlock(path[path.size() - 1]);
        mcpp::Coordinate myCoord = path[path.size() - 1];
        
         
        for (auto &location : path) {
        static int prevLoc = location.y;
        location.y = prevLoc;
        mcpp::BlockType pathBlock = mc.getBlock(location) ;
        mcpp::Coordinate tempLoc = location+Y_MINUS;
        mcpp::BlockType pathBlockBelow =  mc.getBlock(tempLoc);
        int pathHeight = mc.getHeight(location.x, location.z)+1;
        mcpp::Coordinate placePath (location.x, pathHeight, location.z);
        if (pathBlock == mcpp::Blocks::AIR && 
            pathBlockBelow == mcpp::Blocks::AIR){
            
            mc.setBlock(placePath,mcpp::Blocks::LIME_CARPET);
            
        }
        //if theres a hole on path, 1 block max, for enhancements
        else if (pathBlock == mcpp::Blocks::AIR){
            mc.setBlock(placePath, mcpp::Blocks::LIME_CARPET);
        }
        //if theres a block on path, 1 block max, for enhancements
        else{
            location = location + Y_PLUS;
            mc.setBlock(placePath, mcpp::Blocks::LIME_CARPET);
        }
        //Delay
            std::cout << "Step[" << i + 1 << "]: " << "(" << placePath.x 
                << ", " << placePath.y << ", " << placePath.z << ")" 
                << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            if (pathBlock == mcpp::Blocks::AIR && 
                pathBlockBelow == mcpp::Blocks::AIR){
                mc.setBlock(placePath, mcpp::Blocks::AIR);    
            }
            else{
                mc.setBlock(placePath, mcpp::Blocks::AIR);
            }
            i++;
            prevLoc = location.y;
        }
        
        std::cout<< "Maze solved, restoring last tile."<<std::endl;
        //To restore the block back at the last tile
        std::this_thread::sleep_for(std::chrono::seconds(5));
        mc.setBlock(myCoord, myBlock);
    }
    else if(enhanced == true){
        long int i;
        int count = 1;
        std::cout << "Shortest Path!" << std::endl;
        mcpp::BlockType myBlock = mc.getBlock(shortestPath[0]);
        mcpp::Coordinate myCoord = shortestPath[0];
        
        
        for (i = (shortestPath.size() - 2); i >= 0; --i){
            static int prevLoc = shortestPath[i].y;
            shortestPath[i].y = prevLoc;
            mcpp::BlockType pathBlock = mc.getBlock(shortestPath[i]) ;
            mcpp::Coordinate tempLoc = shortestPath[i]+Y_MINUS;
            mcpp::BlockType pathBlockBelow =  mc.getBlock(tempLoc);
            int pathHeight = mc.getHeight(shortestPath[i].x, 
                shortestPath[i].z)+1;
            mcpp::Coordinate placePath (shortestPath[i].x, pathHeight, 
                shortestPath[i].z);
            if (pathBlock == mcpp::Blocks::AIR && 
                pathBlockBelow == mcpp::Blocks::AIR){
                mc.setBlock(placePath,mcpp::Blocks::LIME_CARPET);
                
            }
            //if theres a hole on path, 1 block max, for enhancements
            else if (pathBlock == mcpp::Blocks::AIR){
                mc.setBlock(placePath, mcpp::Blocks::LIME_CARPET);
            }
            //if theres a block on path, 1 block max, for enhancements
            else{
                shortestPath[i] = shortestPath[i] + Y_PLUS;
                mc.setBlock(placePath, mcpp::Blocks::LIME_CARPET);
            }
            
            //delay
            std::cout << "Step[" << count << "]: " << "(" << placePath.x 
                << ", " << placePath.y << ", " << placePath.z 
                << ")" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            if (pathBlock == mcpp::Blocks::AIR && 
                pathBlockBelow == mcpp::Blocks::AIR){
                mc.setBlock(placePath, mcpp::Blocks::AIR);    
            }
            else{
                mc.setBlock(placePath, mcpp::Blocks::AIR);
            }
            ++count;
            prevLoc = shortestPath[i].y;
        }
        //To restore the block back at the last tile
        std::cout<< "Maze solved, restoring last tile."<<std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(5));
        mc.setBlock(myCoord, myBlock);
    }
}

void Agent::pop_front(std::vector<std::pair<mcpp::Coordinate, 
    mcpp::Coordinate>> &myVector){
    if(myVector.size() > 0){
        myVector.erase(myVector.begin());
    }
    else{
        std::cout << "Vector is size 0, can't erase" << std::endl;
    }
}

void Agent::reverse(mcpp::Coordinate &myDirection){
    int x = myDirection.x;
    int z = myDirection.z;
    int y = myDirection.y;
    if(x == -1){
        myDirection.x = 1;
    }
    else if(x == 1){
        myDirection.x = -1;
    }

    if(z == 1){
        myDirection.z = -1;
    }
    else if(z == -1){
        myDirection.z = 1;
    }

    if(y == 1){
        myDirection.y = -1;
    }
    else if(y == -1){
        myDirection.y = 1;
    }
}

