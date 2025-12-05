//  file_functions.cpp
//      Functions dedicated specifically to dealing with .OBJ files
//  Author: David Maguire
//  CU ID: dama7698
//  GitHub Username: david-maguire
//

#include "../include/file_functions.h"

// Reads an OBJ file and inserts each cube as a vertex into the octree
void readFromOBJ(const string &filename, octree &tree) {
    ifstream file(filename);

    vector<vertex> cubeVertices;
    string line;
    int vertCounter = 0;  // Assign unique IDs to vertices

    while (getline(file, line)) {
        istringstream inputStream(line);
        string prefix;
        inputStream >> prefix;

        if (prefix == "v") {
            float x;
            float y;
            float z;
            inputStream >> x >> y >> z;
            cubeVertices.push_back(vertex{0, x, y, z});

            // If we have 8 vertices, cube is compplete
            if (cubeVertices.size() == 8) {
                // Compute center
                float cx = 0, cy = 0, cz = 0;
                for( int i = 0; i < cubeVertices.size(); i++){
                    cx += cubeVertices[i].x;
                    cy += cubeVertices[i].y;
                    cz += cubeVertices[i].z;
                }
                vertex cubeCenter{vertCounter++, cx / 8.0f, cy / 8.0f, cz / 8.0f};

                // Insert into octree
                tree.insert_data(tree.getRoot(), cubeCenter);

                // Clear for next cube
                cubeVertices.clear();
            }
        }
    }

    file.close();
    cout << "Finished inserting points from " << filename << "\n";
}

// Write a cube mesh to OBJ
void writeCube(ofstream &outFile, float cx, float cy, float cz, float h, unsigned &vertexOffset) {
    //Adding "h," the halfSize of the cube to the center point in each direction
    float x[8] = {cx - h, cx + h, cx + h, cx - h, cx - h, cx + h, cx + h, cx - h};
    float y[8] = {cy - h, cy - h, cy + h, cy + h, cy - h, cy - h, cy + h, cy + h};
    float z[8] = {cz - h, cz - h, cz - h, cz - h, cz + h, cz + h, cz + h, cz + h};

    //write out each vertex as a line
    for (int i = 0; i < 8; ++i)
        outFile << "v " << x[i] << " " << y[i] << " " << z[i] << "\n";

    //Building faces from vertex IDs
    int faces[6][4] = {
        {0,1,2,3}, {4,5,6,7}, {0,1,5,4},
        {2,3,7,6}, {0,3,7,4}, {1,2,6,5}
    };
    //write out each face as a line
    for (int i = 0; i < 6; ++i) {
        outFile << "f ";
        for (int j = 0; j < 4; ++j) {
            outFile << (faces[i][j] + vertexOffset) << " ";
        }
        outFile << "\n";
    }

    vertexOffset += 8; //Used to track discrete cubes, for some reason OBJs work based on continous vertex counts
}

// Recursively export octree nodes and points
void writeObjects(oct_node* node, ofstream &objFile, unsigned &vertexOffset) {
    if (node == NULL) return;

    // Write cube for this node
    writeCube(objFile, node->nodeCenter[0], node->nodeCenter[1], node->nodeCenter[2], node->halfSize, vertexOffset);

    //Write each point as a vertex
    for (int i = 0; i < node->points.size(); i++){
        objFile << "o point_" << node->points[i].vertID << "\n";
        objFile << "v " << node->points[i].x << " " << node->points[i].y << " " << node->points[i].z << "\n";
        vertexOffset ++;
    }

    // Recurse into children
    for (int i = 0; i < 8; i++) {
        if (node->children[i]) {
            writeObjects(node->children[i], objFile, vertexOffset);
        }
    }
}

// Main export function
void exportOctreeOBJ(octree &tree, const string &filename) {
    ofstream objFile(filename);
    if (!objFile.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    unsigned vertexOffset = 1; // OBJ indices start at 1
    writeObjects(tree.getRoot(), objFile, vertexOffset);

    objFile.close();
    cout << "OBJ export complete: " << filename << endl;
}