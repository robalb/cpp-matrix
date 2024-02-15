/**
 * @file main.cpp
 * @brief usage test for the oriented_graph class
 */

#include <iostream>
#include <cassert>
#include "ograph.hpp"
#include "testframework.hpp"

/**
 * @brief functor for integer equality
 */
struct equal_int {
  bool operator()(int a, int b) const {
    return a==b;
  }
};

void test_basic_behaviour(){
  //-----------
  //nodes
  //-----------
  
  //empty graph
  oriented_graph<int, equal_int> og;
  assert(og.nodes() == 0);
  assert(og.edges() == 0);
  assert(og.existsNode(1) == false);
  assert(og.existsNode(0) == false);
  assert(og.existsNode(99) == false);
  assert(og.existsEdge(0, 0) == false);
  assert(og.existsEdge(0, 1) == false);
  assert(og.existsEdge(99, 1) == false);

  //1 node added post initialization
  og.addNode(10);
  assert(og.nodes() == 1);
  assert(og.edges() == 0);
  assert(og.existsNode(10) == true);
  assert(og.existsNode(0) == false);
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
  assert(!og1.existsEdge(99,6));

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
  assert(!og1.existsEdge(99,6));

}

void test_exceptions(){
  int nodes[] = {0,1,2,3,4,5};
  oriented_graph<int, equal_int> og(nodes, sizeof(nodes)/sizeof(nodes[0]));

  M_ASSERT_THROW(
      og.addNode(1),
      invalidNodeException
      );
  M_ASSERT_THROW(
      og.addNode(200),
      invalidNodeException
      );
  M_ASSERT_THROW(
      og.addNode(300),
      invalidNodeException
      );

}

int main(){
  
  //TODO: test all kinds of errors
  //TODO: repeat all tests, but for a custom class
  //
  // test_basic_behaviour();
  // test_exceptions();


  // testFramework_summary();
  // oriented_graph<int, equal_int> og;
  // assert(og.nodes() == 0);
  // assert(og.edges() == 0);
  //
  //



    // Test boolean assertions
    M_ASSERT(2 + 2 == 4);
    M_ASSERT(5 * 3 == 14);

    testFramework::caught = testFramework::caughtType::none;

    // Test throw assertions
    M_ASSERT_THROW(throw std::invalid_argument("Invalid argument"), std::invalid_argument);
    M_ASSERT_THROW(throw std::logic_error("Logic error"), std::invalid_argument);

    // Print test summary
    testFramework::summary();
}
