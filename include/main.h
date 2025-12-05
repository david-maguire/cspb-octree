#include "../include/oct.h"
#include <vector>
#include <string>
#include <iostream>

//Menu options:
//1) Check for a point in tree
void checkForPoint(octree &tree);
// 2) Add a new point
void addNewPoint(octree &tree);

// 3) Remove a point
void removePoint(octree &tree);

// 4) Get point count
void getPointCount(octree &tree);

// 5) Get list of all points
void listAllPoints(oct_node* node);

// 6) Get depth
void showDepth(octree &tree);

// 7) Set new depth
void setNewDepth(octree &tree);

// 8) Read .OBJ file
void readFile(octree &tree);

// 9) Write .OBJ file
void writeFile(octree &tree);

// 10) Close program
void closeProgram();