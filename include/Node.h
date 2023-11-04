#ifndef NODE_H
#define NODE_H

#include <mcpp/mcpp.h>

class Node {
    public:
        Node(Node* sentinelPtr = nullptr, 
            mcpp::Coordinate dataCoordinate = mcpp::Coordinate(0, 50, 0), 
            mcpp::BlockType dataBlockType = mcpp::Blocks::AIR, 
            int dataRow = 0, int dataCol = 0);
        void initializeAsSentinel();
        void insertAtFront();
        void insertAtEnd();
        void insertBefore(Node* targetNodePtr);
        void insertAfter(Node* targetNodePtr);
        Node* getFrontPtr();
        Node* getEndPtr();
        bool isEmpty();
        void removeFront();
        void removeEnd();
        void deleteAll();
        void setDataCoordinate(mcpp::Coordinate dataCoordinate);
        void setDataBlockType(mcpp::BlockType dataBlockType);
        void setDataRow(int dataRow);
        void setDataCol(int dataCol);
        mcpp::Coordinate getDataCoordinate();
        mcpp::BlockType getDataBlockType();
        int getDataRow();
        int getDataCol();
        Node* sentinelPtr;
        Node* nextPtr;
        Node* prevPtr;

    private:
        mcpp::Coordinate dataCoordinate;
        mcpp::BlockType dataBlockType = mcpp::Blocks::AIR;
        int dataRow;
        int dataCol;
        
};

#endif //NODE_H