#ifndef COORDINATEBLOCK_H
#define COORDINATEBLOCK_H

#include <mcpp/mcpp.h>

namespace mcpp {
    class CoordinateBlock : public Coordinate {
        public: 
            CoordinateBlock(int x = 0, int y = 0, int z = 0, 
                BlockType type = Blocks::AIR) : 
                coordinate(mcpp::Coordinate(x, y, z)), type(type) {};
            CoordinateBlock(mcpp::Coordinate coordinate, 
                BlockType type = Blocks::AIR) : 
                coordinate(coordinate), type(type) {};
            ~CoordinateBlock() {}; 
            BlockType getBlockType() {return this->type;}
            mcpp::Coordinate getCoordinate() {return this->coordinate;}
            
        private: 
            mcpp::Coordinate coordinate;
            BlockType type;
    };
}

#endif