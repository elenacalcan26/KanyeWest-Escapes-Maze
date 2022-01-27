#include<stdlib.h>

#include "Maze.h"

using namespace std;

Maze::Maze()
{
}

Maze::~Maze()
{
}

void Maze::generateMaze() {
    srand(time(NULL));
    Node node;
    node.x = rand() % MAZE_DIM;
    node.y = rand() % MAZE_DIM;

    stk.push(node);
    while (!stk.empty())
    {
        Node nextNode = stk.top();
        stk.pop();
        
        if (validNextNode(nextNode)) {
            maze[nextNode.y][nextNode.x] = 1;
            vector<Node> neighs = findNeighbors(nextNode);
            randomlyAddNodesToStack(neighs);
        }
    }

    maze[node.y][node.x] = 0;
    
}


bool Maze::validNextNode(Node node) {
    int numOnesNeighs = 0;
    for (int y =  node.y - 1; y < node.y + 2; y++) {
        for (int x = node.x - 1; x < node.x + 2; x++) {
            if (pointOnGrid(x, y) && pointNotNode(node, x, y) && maze[y][x] == 1) {
                numOnesNeighs++;    
            }
        }
    }
    return (numOnesNeighs < 3) && maze[node.y][node.x] != 1;
}

vector<Node> Maze::findNeighbors(Node node) {
    vector<Node> neighs;
    for (int y = node.y - 1; y < node.y + 2; y++) {
        for (int x = node.x - 1; x < node.x + 2; x++) {
            if (pointOnGrid(x, y) && pointNotCorner(node, x, y) && pointNotNode(node, x, y)) {
                Node newNode;
                newNode.x = x;
                newNode.y = y;
                neighs.push_back(newNode);
            }
        }
    }
    return neighs;
}

void Maze::randomlyAddNodesToStack(vector<Node> neighs) {
    srand(time(NULL));
    int targetIndex;
    while (!neighs.empty())
    {
        targetIndex = rand() % neighs.size();
        stk.push(neighs.at(targetIndex));
        
        
        neighs.erase(neighs.begin() + targetIndex);
    }

}

bool Maze::pointOnGrid(int x, int y) {
   return y < MAZE_DIM && x < MAZE_DIM && x >= 0 && y >= 0;
}

bool Maze::pointNotCorner(Node node, int x, int y) {
    return (x == node.x || y == node.y);
}

bool Maze::pointNotNode(Node node, int x, int y) {
    return !(x == node.x && y == node.y);
}


