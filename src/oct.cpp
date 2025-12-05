//  oct.cpp
//      Homemade octree data structure for 3D coordinate points
//  Author: David Maguire
//  CU ID: dama7698
//  GitHub Username: david-maguire
//

#include "../include/oct.h"
#include "../include/file_functions.h"
#include <algorithm>

octree::octree() {
    root = new oct_node();
    root->nodeCenter = vector<float> (3, 0.0);
    root->halfSize = 1.0;
    root->parent = NULL;
    setDepth(3);
    for (int i = 0; i < 8; i++){
      root->children[i] = NULL;
    }
}

octree::octree(unsigned inputDepth) {
    root = new oct_node();
    root->nodeCenter = vector<float> (3, 0.0);
    root->halfSize = 1.0;
    root->parent = NULL;
    setDepth(inputDepth);
    for (int i = 0; i < 8; i++){
      root->children[i] = NULL;
    }
}

octree::~octree(){
    if (root != nullptr) {
      destroySubtree(root);
      root = NULL;
    }
}

void octree::subdivide(oct_node* parent) {
    float newHalf = parent->halfSize * 0.5;

    for (int i = 0; i < 8; i++) {
        parent->children[i] = new oct_node();
        parent->children[i]->parent = parent;
        parent->children[i]->halfSize = newHalf;
        parent->children[i]->nodeCenter = {parent->nodeCenter[0] + octree::NODE_OFFSETS[i][0] * newHalf, parent->nodeCenter[1] + octree::NODE_OFFSETS[i][1] * newHalf, parent->nodeCenter[2] + octree::NODE_OFFSETS[i][2] * newHalf};

        for (int j = 0; j < 8; j++){
          parent->children[i]->children[j] = NULL;
        }
    }

    
}

void octree::insert_data(oct_node* targetNode, vertex newPoint, unsigned current_depth){
  //Create new root if necessary
  if (root == NULL) {
    root = new oct_node();
    root->nodeCenter = vector<float> (3, 0.0);
    if (newPoint.x >= newPoint.y && newPoint.x >= newPoint.z){
        root->halfSize = newPoint.x;
      }
      else if (newPoint.y >= newPoint.x && newPoint.y >= newPoint.z){
        root->halfSize = newPoint.y;
      }
      else {
        root->halfSize = newPoint.z;
      }
      setDepth(3);

      for (int i = 0; i < 8; i++){
        root->children[i] = NULL;
      }
  }
  //Safety check on targetNode
  if (targetNode == NULL){
    targetNode = root;
  }

  //Resize existing octree if necessary
  if (newPoint.x >= root->halfSize || newPoint.y >= root->halfSize || newPoint.z >= root->halfSize) {
    float newHalfSize = max({newPoint.x, newPoint.y,newPoint.z});
    resize_node(root, newHalfSize, 1);
  }

  //check depth, if done stop
  if (current_depth == this->depth) {
    targetNode->points.push_back(newPoint);
    return;
  }
  //Divide current node if not subdivided
  if (targetNode->children[0] == NULL) {
    subdivide(targetNode);
  }
  //check what octant the vertex should be sorted into
  unsigned targetOctant = findTargetOctant(targetNode, newPoint);

  insert_data(targetNode->children[targetOctant], newPoint, current_depth + 1);

  //Calculate new densities
  targetNode->density = this->countPoints(targetNode) / (targetNode->halfSize * 8);

  return;    
}

unsigned octree::findTargetOctant(oct_node* targetNode, vertex point){
  int location[3] = {0,0,0};
    if (point.x >= targetNode->nodeCenter[0]){
      location[0] = 1;
    }
    else {
      location[0] = -1;
    }
    
    if (point.y >= targetNode->nodeCenter[1]) { 
      location[1] = 1;
    }
    else {
      location[1] = -1;
    }
    
    if (point.z >= targetNode->nodeCenter[2]) {
      location[2] = 1;
    }
    else {
      location[2] = -1;
    }
   for (int i = 0; i < 8; i++) {
        if (location[0] == NODE_OFFSETS[i][0] && location[1] == NODE_OFFSETS[i][1] && location[2] == NODE_OFFSETS[i][2]) {
          return i;
        }
  }
  return 0;
}

void octree::resize_node(oct_node* node, float newHalfSize, unsigned currentDepth){
  if (node == NULL){
    return;
  }

    node->halfSize = newHalfSize;

    for (int i = 0; i < 8; i++) {
        if (node->children[i]) {
            // Recompute child centers
            float childHalf = newHalfSize / 2.0;
            node->children[i]->nodeCenter = {
                node->nodeCenter[0] + NODE_OFFSETS[i][0] * childHalf,
                node->nodeCenter[1] + NODE_OFFSETS[i][1] * childHalf,
                node->nodeCenter[2] + NODE_OFFSETS[i][2] * childHalf
            };

            // Recursively resize child nodes
            resize_node(node->children[i], childHalf, currentDepth + 1);
        }
    }

    // Reorganize points within this node
    reorganizeData(node, currentDepth);

  return;

  
}

void octree::reorganizeData(oct_node* node, unsigned currentDepth) {
  if (node == NULL) {
    return;
  }

  //Only subdivide if:
  // 1) we're not at the bottom
  // 2) Have no children yet
  // 3) Have points in our node
  if (node->points.empty() == false && currentDepth < this->depth && node->children[0] == NULL) {
    subdivide(node);
  }

  vector<vertex> oldPoints;

  if (currentDepth < this->depth){
    oldPoints = node->points;
    node->points.clear();
  }


for (int i = 0; i < oldPoints.size(); i++) {
    unsigned newLocation = findTargetOctant(node, oldPoints[i]);
    if (node->children[newLocation] != NULL){
      node->children[newLocation]->points.push_back(oldPoints[i]);
    }
}

  //Recursively do the same for each child node
    for (int i = 0; i < 8; i++) {
        if (node->children[i] != NULL) {
            reorganizeData(node->children[i], currentDepth + 1);
        }
    }

  //Update density
  node->density = this->countPoints(node) / (node->halfSize * 8);

  return;
}

void octree::remove_data(oct_node* node, vertex point){
  oct_node* targetLocation = find(node, point);
  if(targetLocation == NULL){
    return;
  }

    for (int i = targetLocation->points.size() -1; i >= 0; i--) {
      if (comparePoints(targetLocation->points[i], point)){
        targetLocation->points.erase(targetLocation->points.begin() + i);
      }
    }
  
    //Update Density
    targetLocation->density = countPoints(targetLocation) / (targetLocation->halfSize * 8);

    // Recursively clean up empty nodes
    cleanupNode(targetLocation);

  return;
}

void octree::cleanupNode(oct_node* node) {
  if (node == NULL) {
    return;
  }

  if (node->points.empty() == false){
    return;
  }

    //Check current node for non-empty children
    for (int i = 0; i < 8; i++) {
      if (node->children[i] != NULL) {
        return;
      }
    }

    //Node is empty, not root, and has no children: delete it from parent
    oct_node* parent = node->parent;
    if (parent != NULL){
      for (int i = 0; i < 8; i++){
        if (parent->children[i] == node) {
          parent->children[i] = NULL;
          break;
        }
      }
      
      delete node;
      cleanupNode(parent);
    }
    
    else {
      //Node is root
      node->points.clear();
      for (int i = 0; i < 8; i++){
        node->children[i] = NULL;
      }
    }
}

bool octree::contains(oct_node* searchStart, vertex targetPoint){
  if (searchStart == NULL) {
    return false;
  }
  //local node
  for (int i = 0; i < searchStart->points.size(); i++) {
    if (comparePoints(searchStart->points[i], targetPoint)) {
      return true;
    }
  }
  
  unsigned testLocation = findTargetOctant(searchStart, targetPoint);
  if (searchStart->children[testLocation] == NULL){
    return false;
  }

  //child nodes
  return contains(searchStart->children[testLocation], targetPoint);
}

oct_node* octree::find(oct_node* searchStart, vertex targetPoint){
  if (searchStart == NULL) {
    return NULL;
  }

  //local node
  for (int i = 0; i < searchStart->points.size(); i++) {
    if (comparePoints(searchStart->points[i], targetPoint)) {
      return searchStart;
    }
  }
  
  unsigned testLocation = findTargetOctant(searchStart, targetPoint);
  if (searchStart->children[testLocation] == NULL){
    return NULL;
  }

  //child nodes
  return find(searchStart->children[testLocation], targetPoint);
}

unsigned octree::countPoints(oct_node* node) {
  unsigned count = node->points.size();
  for (int i = 0; i < 8; i++) {
    if(node->children[i] != NULL) {
      count = count + countPoints(node->children[i]);
    }
  }
  return count;
}

unsigned octree::getDepth(){
  return this->depth;
}

void octree::setDepth(unsigned newDepth){
  unsigned oldDepth = this->depth;
  this->depth = newDepth;
  if (newDepth > oldDepth) { 
    growTree(root, 1);
  }
  else if (newDepth < oldDepth){
    shrinkTree(root, 1);
  }
  reorganizeData(root, 1);
  return;
}

void octree::growTree(oct_node* node, unsigned currentLevel) {
    if (node == NULL) return;

    if (currentLevel >= this->depth) {
      return;
    }

    if (node->children[0] == NULL) {
        subdivide(node);
    }

    // Recursively grow each child if needed
    for (int i = 0; i < 8; i++) {
        growTree(node->children[i], currentLevel + 1);
    }
}

void octree::shrinkTree(oct_node* node, unsigned currentLevel) {
  if (node == NULL) {
    return;
  }

  // At max depth, collect child points to current node
  if (currentLevel < this->depth) {
    for (int i = 0; i < 8; i++) {
      if (node->children[i] != NULL) {
        shrinkTree(node->children[i], currentLevel + 1);

        // Move points from the child up to this node
        node->points.insert(node->points.end(), node->children[i]->points.begin(), node->children[i]->points.end());

        // Delete the now empty child
        delete node->children[i];
        node->children[i] = NULL;
      }
    }

  }
}

bool octree::comparePoints(vertex a, vertex b){
  return (abs(a.x - b.x) < (1e-5) && abs(a.y - b.y) < (1e-5) && abs(a.z - b.z) < (1e-5));
}

oct_node* octree::getRoot() {
    return root;
}

  void octree::destroySubtree(oct_node* node) {
    if (node == nullptr) return;
    // delete children first
    for (int i = 0; i < 8; ++i) {
      if (node->children[i] != nullptr) {
        destroySubtree(node->children[i]);
        node->children[i] = nullptr;
      }
    }
    // At this point children are deleted; safe to delete this node
    delete node;
}
;