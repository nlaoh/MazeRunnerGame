#include "../include/Node.h"

/* This class is an implementation of a doubly linked list.
 * A sentinel Node is like a dummy head and tail combined into one.
 * A sentinel Node can be created on its own but further Nodes 
 * added to a list need to point to a sentinel Node.
 */
Node::Node(Node* sentinelPtr, mcpp::Coordinate dataCoordinate, 
    mcpp::BlockType dataBlockType, int dataRow, int dataCol) {
    this->sentinelPtr = sentinelPtr;
    this->dataCoordinate = dataCoordinate.clone();
    this->dataBlockType = dataBlockType;
    this->dataRow = dataRow;
    this->dataCol = dataCol;
    nextPtr = nullptr;
    prevPtr = nullptr;
}

void Node::initializeAsSentinel() {
    sentinelPtr = this;
    nextPtr = this;
    prevPtr = this;
}

void Node::insertAtFront() {
    nextPtr = sentinelPtr->nextPtr;
    sentinelPtr->nextPtr = this;
    prevPtr = sentinelPtr;
    nextPtr->prevPtr = this; 
}

void Node::insertAtEnd() {
    prevPtr = sentinelPtr->prevPtr;
    sentinelPtr->prevPtr = this;
    nextPtr = sentinelPtr;
    prevPtr->nextPtr = this;
}

void Node::insertBefore(Node* targetNodePtr) {
    prevPtr = targetNodePtr->prevPtr;
    targetNodePtr->prevPtr = this;
    nextPtr = targetNodePtr;
    prevPtr->nextPtr = this;
}

void Node::insertAfter(Node* targetNodePtr) {
    nextPtr = targetNodePtr->nextPtr;
    targetNodePtr->nextPtr = this;
    prevPtr = targetNodePtr;
    nextPtr->prevPtr = this;
}

Node* Node::getFrontPtr() {
    return sentinelPtr->nextPtr;
}

Node* Node::getEndPtr() {
    return sentinelPtr->prevPtr;
}

bool Node::isEmpty() {
    bool isEmpty = false;
    if (sentinelPtr->nextPtr == sentinelPtr && 
        sentinelPtr->prevPtr == sentinelPtr) {
        isEmpty = true;
    }
    return isEmpty;
}

void Node::removeFront() {
    sentinelPtr->nextPtr = sentinelPtr->nextPtr->nextPtr;
    delete sentinelPtr->nextPtr->prevPtr;
    sentinelPtr->nextPtr->prevPtr = sentinelPtr;
}

void Node::removeEnd() {
    sentinelPtr->prevPtr = sentinelPtr->prevPtr->prevPtr;
    delete sentinelPtr->prevPtr->nextPtr;
    sentinelPtr->prevPtr->nextPtr = sentinelPtr;
}

void Node::deleteAll() {
    Node* currPtr = sentinelPtr->nextPtr;

    while (currPtr != sentinelPtr) {
        currPtr = currPtr->nextPtr;
        currPtr->removeFront();
    }
    delete sentinelPtr;
}

void Node::setDataCoordinate(mcpp::Coordinate dataCoordinate) {
    this->dataCoordinate = dataCoordinate.clone();
}

void Node::setDataBlockType(mcpp::BlockType dataBlockType) {
    this->dataBlockType = dataBlockType;
}

void Node::setDataRow(int dataRow) {
    this->dataRow = dataRow;
}

void Node::setDataCol(int dataCol) {
    this->dataCol = dataCol;
}

mcpp::Coordinate Node::getDataCoordinate() {
    return dataCoordinate;
}

mcpp::BlockType Node::getDataBlockType() {
    return dataBlockType;
}

int Node::getDataRow() {
    return dataRow;
}

int Node::getDataCol() {
    return dataCol;
}