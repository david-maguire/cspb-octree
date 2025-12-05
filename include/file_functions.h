#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <functional>
#include "../include/oct.h"

#pragma once

//Read .OBJ file
void readFromOBJ(const string &filename, octree &tree);

void writeCube(ofstream &outFile, float cx, float cy, float cz, float h, unsigned &vertexOffset);

void writeObjects(oct_node* node, ofstream &objFile, unsigned &vertexOffset);

void exportOctreeOBJ(octree &tree, const string &filename);