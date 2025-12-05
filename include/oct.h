#include <memory>
#include <string>
#include <vector>
#pragma once

using namespace std;


// The points in 3d space we will be tracking in the octree
// Includes a vert ID for follow on implementation, not used currently
struct vertex{
  int vertID;
  float x;
  float y;
  float z;
};

// Octree nodes
// Defines the center and size of each node, the list of included points (if the bottom
// level), and points to any lower nodes
struct oct_node {
  oct_node* parent;
  float density;
  vector<float> nodeCenter;
  float halfSize;
  vector<vertex> points;
  oct_node* children[8];
};

// Octree:
//
// Each node has up to 8 children, defining the 3d space of its volume.
// Up to the depth of the tree, each point belongs to a node (or octant) 
// which is then (and only then) divided into relevant nodes to contain
// the points. 

class octree {
public:
  // Constructors, creates an initial node (I see no reason to have an empty structure)
  // If depth is undefined, default is 3.
  // Default dimensons of 2.0 x 2.0 x 2.0.
  octree();
  octree(unsigned startDepth);

  // deconstructor - use this to clean up all memory that the octree has allocated
  // but not returned with the 'delete' keyword.
  ~octree();
  
  //Creates child nodes if the current node is empty
  void subdivide(oct_node* currentNode);

  // Places a new vertex in the appropriate container, creating new octants if
  // necessary
  void insert_data(oct_node* targetNode, vertex newPoint, unsigned currentDepth = 0);

  //Finds target octant for child using the offset array
  unsigned findTargetOctant(oct_node* targetNode, vertex point);

  //resize nodes based on changes to parent size and reassign vertices
  void resize_node(oct_node* node, float newHalfSize, unsigned currentDepth);

  //Reorganize vertices, adding subdivisions if needed to match depth
  void reorganizeData(oct_node* node, unsigned currentDepth);

  // Removes any point that exists at the given location
  void remove_data(oct_node* node, vertex point);

  // Removes all children of a node if they are empty
  void cleanupNode(oct_node* node);

  //Returns true if any node below the point contains the desired vertex
  bool contains(oct_node* node, vertex point);

  //Returns the node containing a target point
  oct_node* find(oct_node* searchStart, vertex targetPoint);

  // Counts how many points are within a node and it's children
  // Used to calculate a node's density
  unsigned countPoints(oct_node* node);

  unsigned getDepth();

  void setDepth(unsigned newDepth);
  
  //Used to expand the tree after changing depth
  void growTree(oct_node* node, unsigned currentLevel);
  //Used to shrink the tree after changing depth
  void shrinkTree(oct_node* node, unsigned currentLevel);
  
  bool comparePoints(vertex a, vertex b);

  oct_node* getRoot();
  
private:
  oct_node* root;
  unsigned depth;

  //Used to create each sub-node. Each new sub-octant maps to one of the
  // 3-arrays and allows us to offset the center of the new node
  // in the right direction
  static constexpr int NODE_OFFSETS[8][3] = {
        {-1,-1,-1}, {1,-1,-1},
        {-1,1,-1}, {1,1,-1},
        {-1,-1,1}, {1,-1,1},
        {-1,1,1}, {1,1,1}
    };

  // Recursively delete a subtree to assist in destructor
  void destroySubtree(oct_node* node);

};