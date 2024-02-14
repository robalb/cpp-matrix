/**
 * @file main.cpp
 * @brief usage test for the oriented_graph class
 */

#include <iostream>
#include <cassert>
#include "ograph.hpp"

/**
 * @brief functor for integer equality
 */
struct equal_int {
  bool operator()(int a, int b) const {
    return a==b;
  }
};

int main(){
  //empty class
  oriented_graph<int, equal_int> og;
  assert(og.nodes() == 0);
  assert(og.edges() == 0);
  assert(og.existsNode(1) == false);
  assert(og.existsNode(0) == false);
  assert(og.existsNode(99) == false);
  // TODO: test exceptions
  // assert(og.existsEdge(0, 0) == false);
  // assert(og.existsEdge(0, 1) == false);
  // assert(og.existsEdge(99, 1) == false);

  //1 node added post initialization
  og.addNode(10);
  assert(og.nodes() == 1);
  assert(og.edges() == 0);
  assert(og.existsNode(10) == true);
  assert(og.existsNode(0) == false);
  og.print();

  //several nodes added post initialization
  og.addNode(11);
  og.print();
  og.addNode(12);
  og.print();
  og.addNode(13);
  og.print();
  int m[4][4] = {
    {1,2,3,4},
    {1,2,3,4},
    {1,2,3,4},
    {1,2,3,4}
  };
  og.debug_setmatrix(&m[0][0]);
  og.print();
  og.addNode(14);
  og.print();
  //remove existing nodes
  og.removeNode(11);
  og.print();
  og.removeNode(10);
  og.print();
  og.removeNode(14);
  og.print();

  //-----------
  //arches
  //-----------

  //add graph with no edges
  int nodes[] = {0,1,2,3,4,5};
  oriented_graph<int, equal_int> og1(nodes, sizeof(nodes)/sizeof(nodes[0]));
  og1.print();
  assert(og1.nodes() == 6);
  assert(og1.edges() == 0);

  //add edges
  og1.addEdge(0,2);
  og1.addEdge(3,3);
  assert(og1.existsEdge(0,2));
  assert(!og1.existsEdge(2,0));
  assert(og1.existsEdge(3,3));
  assert(og1.edges() == 2);

  //add node, then edges in that node
  og1.addNode(6);
  og1.addEdge(1,6);
  og1.addEdge(6,1);
  og1.print();
  assert(og1.nodes() == 7);
  assert(og1.edges() == 4);
  assert(og1.existsEdge(0,2));
  assert(og1.existsEdge(3,3));
  assert(og1.existsEdge(1,6));
  assert(og1.existsEdge(6,1));

  assert(!og1.existsEdge(2,0));
  assert(!og1.existsEdge(2,2));
  assert(!og1.existsEdge(1,1));
  assert(!og1.existsEdge(1,5));
  assert(!og1.existsEdge(0,5));
  assert(!og1.existsEdge(2,6));

  //node removal
  og1.removeNode(1);
  og1.print();
  assert(og1.edges() == 2);
  assert(og1.existsEdge(0,2));
  assert(og1.existsEdge(3,3));

  assert(!og1.existsEdge(2,0));
  assert(!og1.existsEdge(2,2));
  assert(!og1.existsEdge(0,5));
  assert(!og1.existsEdge(2,6));

  
  //TODO: test all kinds of errors
  //TODO: repeat all tests, but for a custom class

}
