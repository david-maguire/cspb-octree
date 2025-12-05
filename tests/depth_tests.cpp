#include <iostream>
#include <vector>
#include "../include/oct.h"

using namespace std;

int main() {
    cout << "=== Octree Depth Change Test ===" << endl;

    // Create octree with depth 2
    octree tree(2);
    oct_node* root = tree.getRoot();

    // Insert initial points
    vertex v1 = {1, 0.1f, 0.1f, 0.1f};
    vertex v2 = {2, -0.4f, 0.3f, 0.2f};
    vertex v3 = {3, 0.5f, -0.25f, 0.7f};
    vertex v4 = {4, -0.9f, -0.9f, -0.4f};

    tree.insert_data(root, v1);
    tree.insert_data(root, v2);
    tree.insert_data(root, v3);
    tree.insert_data(root, v4);

    cout << "Inserted 4 points." << endl;

    // Confirm initial containment
    cout << "Contains v1? " << tree.contains(root, v1) << endl;
    cout << "Contains v2? " << tree.contains(root, v2) << endl;
    cout << "Contains v3? " << tree.contains(root, v3) << endl;
    cout << "Contains v4? " << tree.contains(root, v4) << endl;

    // Grow tree depth
    tree.setDepth(3);
    cout << "Increased depth to 3." << endl;

    // Check containment after grow
    cout << "[After grow] Contains v1? " << tree.contains(root, v1) << endl;
    cout << "[After grow] Contains v2? " << tree.contains(root, v2) << endl;
    cout << "[After grow] Contains v3? " << tree.contains(root, v3) << endl;
    cout << "[After grow] Contains v4? " << tree.contains(root, v4) << endl;

    // Shrink tree depth
    tree.setDepth(1);
    cout << "Shrunk depth to 1." << endl;

    // Check containment after shrink
    cout << "[After shrink] Contains v1? " << tree.contains(root, v1) << endl;
    cout << "[After shrink] Contains v2? " << tree.contains(root, v2) << endl;
    cout << "[After shrink] Contains v3? " << tree.contains(root, v3) << endl;
    cout << "[After shrink] Contains v4? " << tree.contains(root, v4) << endl;

    // Insert a new point after depth change
    vertex v5 = {5, 0.6f, 0.6f, 0.6f};
    tree.insert_data(root, v5);
    cout << "Inserted v5 after depth change. Contains v5? " << tree.contains(root, v5) << endl;

    // Remove all points and check containment
    tree.remove_data(root, v1);
    tree.remove_data(root, v2);
    tree.remove_data(root, v3);
    tree.remove_data(root, v4);
    tree.remove_data(root, v5);

    cout << "Removed all points." << endl;
    cout << "Total points now: " << tree.countPoints(root) << endl;

    cout << "=== Depth Change Containment Test Completed ===" << endl;
    return 0;
}