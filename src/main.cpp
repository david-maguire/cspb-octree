//  main.cpp
//      Menu style interface for interacting with the octree
//  Author: David Maguire
//  CU ID: dama7698
//  GitHub Username: david-maguire
//

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "../include/main.h"
#include "../include/file_functions.h"

int main() {
    octree myTree = octree(); // Initialize octree

    int choice;
    do {
        cout << "\n=== Octree Menu ===\n";
        cout << "1) Check for a point\n";
        cout << "2) Add a new point\n";
        cout << "3) Remove a point\n";
        cout << "4) Get point count\n";
        cout << "5) Get list of all points\n";
        cout << "6) Get depth\n";
        cout << "7) Set new depth\n";
        cout << "8) Read .OBJ file\n";
        cout << "9) Write .OBJ file\n";
        cout << "10) Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                checkForPoint(myTree);
                break;
            case 2:
                addNewPoint(myTree);
                break;
            case 3:
                removePoint(myTree);
                break;
            case 4:
                getPointCount(myTree);
                break;
            case 5:
                listAllPoints(myTree.getRoot());
                break;
            case 6:
                showDepth(myTree);
                break;
            case 7:
                setNewDepth(myTree);
                break;
            case 8:
                readFile(myTree);
                break;
            case 9:
                writeFile(myTree);
                break;
            case 10:
                closeProgram();
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }

    } while (choice != 10);

    return 0;
}





//Menu function definitions
//
//
// 1) Check if a point is present
void checkForPoint(octree &tree) {
    vertex queryPoint;
    cout << "Enter x, y, z coordinates: ";
    cin >> queryPoint.x >> queryPoint.y >> queryPoint.z;
    if(tree.contains(tree.getRoot(), queryPoint) == true){
        cout << "That point is in the tree.\n";
    }
    else{
        cout << "No such point exists.\n";
    }
}

// 2) Add a new point
void addNewPoint(octree &tree) {
    vertex newPoint;
    cout << "Enter a vertex ID: ";
    cin >> newPoint.vertID;
    cout << "Enter x, y, z coordinates: ";
    cin >> newPoint.x >> newPoint.y >> newPoint.z;

    tree.insert_data(tree.getRoot(), newPoint);
    cout << "Point added.\n";
}

// 3) Remove a point
void removePoint(octree &tree) {
    vertex doomedPoint;
    cout << "Enter x, y, z coordinates of point to remove: ";
    cin >> doomedPoint.x >> doomedPoint.y >> doomedPoint.z;
    if (tree.contains(tree.getRoot(), doomedPoint) == true){
        tree.remove_data(tree.getRoot(), doomedPoint);
        cout << "Point removed.\n";
    }
    else{
        cout << "No such point exists in this tree. \n";
    }
}

// 4) Get point count
void  getPointCount(octree &tree) {
    unsigned count = tree.countPoints(tree.getRoot());
    cout << "Total points in tree: " << count << "\n";
}
//5) List all points in the tree
void listAllPoints(oct_node* node) {
    if (node == NULL) {
        return;
    }

    // Print points in the current node
    for (int i = 0; i < node->points.size(); i++) {
        cout << "ID:" << node->points[i].vertID << " (" << node->points[i].x << ", " << node->points[i].y << ", " << node->points[i].z << ")\n";
    }
    // Recurse on children
    for (int i = 0; i < 8; ++i) {
        if (node->children[i] != NULL) {
            listAllPoints(node->children[i]);
        }
    }
}

//6) Get current depth of the tree
void showDepth(octree &tree) {
    cout << tree.getDepth() << "\n";
}

//7) Set a new depth for the tree
void setNewDepth(octree &tree){
    int newDepth = 0;
    cout << "Enter the desired depth: ";
    cin >> newDepth;
    tree.setDepth(newDepth);
    cout << "The depth has been set to: " << tree.getDepth() << "\n";
}

//8) Read OBJ with cubes
void readFile(octree &tree) {
    string readFile;
    cout << "Enter the file to be read: ";
    cin >> readFile;
    readFromOBJ(readFile, tree);
    cout << "Your file has successfully been read!\n";
}

//9) Write objects to an OBJ
void writeFile(octree &tree){
    string writeFile;
    cout << "Enter the name of the fie to be written (including the directory):\n";
    cin >> writeFile;
    exportOctreeOBJ(tree, writeFile);
    cout << "Your file has been written!\n";
}

// 10) Exit the program
void closeProgram() {
    cout << "Exiting the program.\n";
    exit(0);  // Exit immediately
}