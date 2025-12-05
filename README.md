# cspb-octree
CPSB 2270: Data Structures Final Project, 
OCTREE for organizing and storing 3-dimensional data
(with demo visualization export!)

# Octree Data Structure with .OBJ Space Visualization Demo

## Introduction
An octree is a tree whose nodes each have 8 children. It's used commonly to represent
sparse 3D data for applications like pathfinding in games or robots. It breaks a space
up into eight "octants" (like quadrants, but, well, you get the idea) which then are
further broken up into octants. The trick is that we only add children to a node if
there are 3D points stored within it's space. In this way, we can effectively model
sparse point data without needing to model the entire space.

## Features
- Core data structure operations:
    Insert points
    Query (contains)
    Remove points
    Reorganize points/fix nodes
    ResizeTree/Nodes
    Set new octree depth
    Count points
    Subdivide nodes
    
- Features of the demo application
    Octree manipulation
        Add and remove points by coordinate
        Check for existing points
        List existing points
        Count existing points
        Get and set tree depth
        Import points (.OBJ)
        Export points and node representations (.OBJ)

## How the Data Structure Works
The data structure, as mentioned above, is a system of nodes with eight children.
Each node is linked via pointers to their eight children, as well as one back to
their parent.

Every node has a size and center relative to its parent, which we then sort our 
data points into. To insert, we look for which octant of a node the point belongs in,
and if we are not at our maximum depth yet we subdivide that node (if children don't
already exist) and proceed into the proper octant to check again.

A choice I made was to keep all 8 octants of a node as long as one of them contained a
point (or descendents that contain a point). This is not always the case, as it is popular
to "prune" the tree of empty nodes. This may be worth exploring for memory reasons, but I
felt that the clarity of all 8 nodes existing together was beneficial.

The depth of a tree is critically important, it defines how many times we subdivide. Growing
and shrinking the tree requires reorganizing all of the points as we give and take away levels.

We can traverse the tree by using the same look up techniques we use to insert a point: picking
the proper octant and descending to our maximum depth.

The root is itself a node with eight children, same as all the rest.

### Time and Space Complexity
| Operation | Avg  | Worst |
|-----------|--------------|------------|
| Insert    | O(N) | O(N)  |
| Query     | O(N) | O(N)  |
| Remove    | O(N) | O(N)  |
| Reorganize| O(N) | O(N)  |
| Resize    | O(N) | O(N)  |
| Set depth | O(N) | O(N)  |
| Count     | O(N) | O(N)  |
| Subdivide | O(1) | O(1)  |

I have not written a very efficient structure...

## File Structure
```
/data # Data files
    readme images
    obj_import_test.obj
    export_test.obj
/src # Source code
    Main.cpp - Demo application
    oct.cpp - Primary data structure class and functions
    file_functions.cpp - Reading and writing of OBJ files
/include # Header files
    main.h
    oct.h
    file_functions.h
/tests # Test cases
    depth_tests.cpp
    test.cpp
```
## Installation / Setup
I was not able to get cMake working, so we'll have to run this the old fashioned way:
```bash
git clone https://github.com/david-maguire/cspb-octree.git
cd cspb-octree
g++ oct.cpp main.cpp file_functions.cpp -o ../bin/octree
../bin/octree
```
## Usage
When prompted at the menu, enter a number corresponding tothe action you'd like to take.
For example, here the user has typed in "1" to indicate they wish to check for
a point in the tree. When prompted, they have typed "1 4 5," and the program has
returned text indicating that it was not found.

Unfortunately, the menu is not very stable and it is very easy to accidentally enter
invalid input. A lot of time was spent on file streaming and not as much on making
the user interface robust.

When prompted for coordinates, always enter them as discrete numbers separated by
a space. For accessing files, input the the entire string. For example, to load
the obj_import_test.obj file, when prompted enter "../data/obj_import_test.obj."

## Application Demo
The application allows you to manipulate an octree that starts out empty.
You are able to check parameters like depth, or for the inclusion of points
at specific coordinates, as well as add, remove, and modify the parameters.

![Query example](../data/readme_images/query.jpg)

Additionally, you are able to read and write .OBJ Files. At the moment, these
are fairly limited, designed to import the location of cubes, and export each node
as a cube and each point as a vertex. Note: Import will ONLY work if you are importing
cubes.

If you have access to Blender, these .OBJ files can be checked simply by importing
them into a scene! For example, these cubes are the items that were exported in the
obj_import_test.obj file:

![importTest](../data/readme_images/import_test.jpg)


And here is the subsequent export of the octree (depth 3), showing the increased density at the
locations of the nodes:

![exportTest](../data/readme_images/export_test.jpg)


Adding and removing points is quite easy, and it is simple to check on the points contained in
the tree. Play around with editing the obj_import_test before exporting again, and see how the
nodes react! Setting the viewport to "X-Ray: 0.2" in Blender is a great way to see how the nodes
are broken up and how depth changes affect things.

## Testing
Testing was done on each of the major functions, via files in the /tests directory. I replaced
my main.cpp function with these and ran it as normal in order to ensure my major functions were
working, as well as checking for any complications with depth changes.

These files are still available if you'd like to run them yourself or even try harder ones.

## References
A major breakthrough was the discovery of the offset array (Thank you Paul Nettle!):
https://flipcode.com/archives/Octree_Implementation.shtml

For understanding .OBJs:
https://paulbourke.net/dataformats/obj/

For data streaming, a big thanks to w3schools for their many, many pages on the different
parts!

### Future Improvements
The original intent was to make a system capable of functioning with a pathfinding algorithm.
To that end, each node already has a density calculated to assist in choosing paths of least
resistance/weighted routes.

With that in mind, things for the future are:
1) Need to make it more efficient! More research necessary.
2) Make interface and file streams safer and more robust
3a) Expand upon .OBJ export and import, reading and writing more object types
3b) Enhance visualization with density color coding
4) Write a pathfinding algorithm!
5) A video walkthrough would be nice, just ran out of time
6) Increase useability by using vertex IDs for queries and operations
7) Clean up and standardize commenting and format, hard to keep track of
consistency over so much code

### License
MIT License
