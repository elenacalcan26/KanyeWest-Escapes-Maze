#pragma once

#include<iostream>
#include<stack>
#include<vector>

#define MAZE_DIM 15

using namespace std;


struct Node
{
    int x, y;
};


class Maze
{
    
public:
    Maze();
    ~Maze();

    void generateMaze();
    bool validNextNode(Node node);
    void randomlyAddNodesToStack(vector<Node> neighs);
    vector<Node> findNeighbors(Node node);
    bool pointOnGrid(int x, int y);
    bool pointNotCorner(Node node, int x, int y);
    bool pointNotNode(Node node, int x, int y);

    stack<Node> stk;
    int maze[MAZE_DIM][MAZE_DIM] = {0};

};
