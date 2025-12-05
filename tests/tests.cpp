#include "../include/oct.h"
#include <iostream>

using namespace std;

int main() {
    cout << "=== Octree Test Suite ===" << endl;

    // Create octree
    octree tree(3);
    oct_node* root = tree.getRoot();

    // Define some points
    vertex v1 = {1, 0.1f, 0.1f, 0.1f};
    vertex v2 = {2, 0.4f, 0.3f, 0.2f};
    vertex v3 = {3, -0.6f, 0.25f, 0.5f};
    vertex v4 = {4, 0.9f, -0.9f, 0.4f};

    // Insert points
    tree.insert_data(root, v1);
    tree.insert_data(root, v2);
    tree.insert_data(root, v3);
    tree.insert_data(root, v4);

    // Check contains
    cout << "Contains v1? " << tree.contains(root, v1) << endl;
    cout << "Contains v2? " << tree.contains(root, v2) << endl;
    cout << "Contains v3? " << tree.contains(root, v3) << endl;
    cout << "Contains v4? " << tree.contains(root, v4) << endl;

    // Count points
    cout << "Total points: " << tree.countPoints(root) << endl;

    // Remove a point
    cout << "\nRemoving v2..." << endl;
    tree.remove_data(root, v2);

    cout << "Contains v2? " << tree.contains(root, v2) << endl;
    cout << "Total points: " << tree.countPoints(root) << endl;

    // Remove all points
    tree.remove_data(root, v1);
    tree.remove_data(root, v3);
    tree.remove_data(root, v4);

    cout << "\nAfter removing all points:" << endl;
    cout << "Contains v1? " << tree.contains(root, v1) << endl;
    cout << "Contains v3? " << tree.contains(root, v3) << endl;
    cout << "Contains v4? " << tree.contains(root, v4) << endl;
    cout << "Total points: " << tree.countPoints(root) << endl;

    // Test depth change
    cout << "\nSetting depth to 4..." << endl;
    tree.setDepth(4);
    cout << "Current depth: " << tree.getDepth() << endl;

    cout << "\nInserting new point v5 at (0.5, 0.5, 0.5)..." << endl;
    vertex v5 = {5, 0.5f, 0.5f, 0.5f};
    tree.insert_data(root, v5);

    cout << "Contains v5? " << tree.contains(root, v5) << endl;
    cout << "Total points: " << tree.countPoints(root) << endl;

    cout << "\n=== Octree Test Completed ===" << endl;
    return 0;
}