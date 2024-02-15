/**
 * @file main.cpp
 * @brief unit tests for the oriented_graph class
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

/**
 * @brief functor for char equality
 */
struct equal_char {
  bool operator()(char a, char b) const {
    return a==b;
  }
};

void test_basic_behaviour(){
  std::cout << "====== TEST_BASIC_BEHAVIOUR ======" << std::endl;

  //-----------
  //nodes
  //-----------
  
  //empty graph
  oriented_graph<int, equal_int> og;
  M_ASSERT(og.nodes() == 0);
  M_ASSERT(og.edges() == 0);
  M_ASSERT(og.existsNode(1) == false);
  M_ASSERT(og.existsNode(0) == false);
  M_ASSERT(og.existsNode(99) == false);
  M_ASSERT(og.existsEdge(0, 0) == false);
  M_ASSERT(og.existsEdge(0, 1) == false);
  M_ASSERT(og.existsEdge(99, 1) == false);

  //1 node added post initialization
  og.addNode(10);
  M_ASSERT(og.nodes() == 1);
  M_ASSERT(og.edges() == 0);
  M_ASSERT(og.existsNode(10) == true);
  M_ASSERT(og.existsNode(0) == false);
  og.print();

  //-----------
  //arches
  //-----------

  //add graph with no edges
  int nodes[] = {0,1,2,3,4,5};
  oriented_graph<int, equal_int> og1(nodes, sizeof(nodes)/sizeof(nodes[0]));
  og1.print();
  M_ASSERT(og1.nodes() == 6);
  M_ASSERT(og1.edges() == 0);

  //add edges
  og1.addEdge(0,2);
  og1.addEdge(3,3);
  M_ASSERT(og1.existsEdge(0,2));
  M_ASSERT(!og1.existsEdge(2,0));
  M_ASSERT(og1.existsEdge(3,3));
  M_ASSERT(og1.edges() == 2);

  //add node, then edges in that node
  og1.addNode(6);
  og1.addEdge(1,6);
  og1.addEdge(6,1);
  og1.print();
  M_ASSERT(og1.nodes() == 7);
  M_ASSERT(og1.edges() == 4);
  M_ASSERT(og1.existsEdge(0,2));
  M_ASSERT(og1.existsEdge(3,3));
  M_ASSERT(og1.existsEdge(1,6));
  M_ASSERT(og1.existsEdge(6,1));

  M_ASSERT(!og1.existsEdge(2,0));
  M_ASSERT(!og1.existsEdge(2,2));
  M_ASSERT(!og1.existsEdge(1,1));
  M_ASSERT(!og1.existsEdge(1,5));
  M_ASSERT(!og1.existsEdge(0,5));
  M_ASSERT(!og1.existsEdge(2,6));
  M_ASSERT(!og1.existsEdge(99,6));

  //node removal
  og1.removeNode(1);
  og1.print();
  M_ASSERT(og1.edges() == 2);
  M_ASSERT(og1.existsEdge(0,2));
  M_ASSERT(og1.existsEdge(3,3));

  M_ASSERT(!og1.existsEdge(2,0));
  M_ASSERT(!og1.existsEdge(2,2));
  M_ASSERT(!og1.existsEdge(0,5));
  M_ASSERT(!og1.existsEdge(2,6));
  M_ASSERT(!og1.existsEdge(99,6));

}

void test_exceptions(){
  std::cout << "====== TEST_EXCEPTIONS ======" << std::endl;

  int nodes[] = {0,1,2,3,4,5};
  oriented_graph<int, equal_int> og(nodes, sizeof(nodes)/sizeof(nodes[0]));

  M_ASSERT_THROW(
      og.addNode(1),
      invalidNodeException
      );
  M_ASSERT_THROW(
      og.addEdge(1,200),
      invalidNodeException
      );
  M_ASSERT_THROW(
      og.removeNode(300),
      invalidNodeException
      );
  M_ASSERT_THROW(
      og.removeEdge(1,1),
      invalidEdgeException
      );

  //duplicate edge
  og.addEdge(1,1);
  M_ASSERT_THROW(
      og.addEdge(1,1),
      invalidEdgeException
      );
}

void test_swap(){
  std::cout << "====== TEST_SWAP ======" << std::endl;

  //-----------
  //swap two classes
  //-----------

  int nodes[] = {0,1,2,3,4,5};
  oriented_graph<int, equal_int> og1(nodes, sizeof(nodes)/sizeof(nodes[0]));
  
  oriented_graph<int, equal_int> og2;
  og2.addNode(10);
  og2.addNode(11);
  og2.addEdge(10,11);

  M_ASSERT(og1.nodes() == 6);
  M_ASSERT(og1.edges() == 0);
  M_ASSERT(og2.nodes() == 2);
  M_ASSERT(og2.edges() == 1);

  og1.swap(og2);
  og2.swap(og1);
  
  M_ASSERT(og1.nodes() == 6);
  M_ASSERT(og1.edges() == 0);
  M_ASSERT(og2.nodes() == 2);
  M_ASSERT(og2.edges() == 1);
}

void test_copy_constructor(){
  std::cout << "====== TEST_COPY_CONSTRUCTOR ======" << std::endl;

  //-----------
  //copy constructor
  //-----------

  char nodes[] = {'a', 'b', 'c', 'd'};
  oriented_graph<char, equal_char> og1(nodes, sizeof(nodes)/sizeof(nodes[0]));
  og1.addEdge('a', 'a');
  og1.addEdge('a', 'd');
  og1.addEdge('c', 'c');

  M_ASSERT(og1.nodes() == 4);
  M_ASSERT(og1.edges() == 3);
  M_ASSERT(og1.existsEdge('a', 'a'));
  M_ASSERT(og1.existsEdge('a', 'd'));
  M_ASSERT(og1.existsEdge('c', 'c'));
  M_ASSERT(!og1.existsEdge('d', 'a'));
  M_ASSERT(!og1.existsEdge('e', 'e'));

  oriented_graph<char, equal_char> og2 = og1;

  //check that og1 is intact
  M_ASSERT(og1.nodes() == 4);
  M_ASSERT(og1.edges() == 3);
  M_ASSERT(og1.existsEdge('a', 'a'));
  M_ASSERT(og1.existsEdge('a', 'd'));
  M_ASSERT(og1.existsEdge('c', 'c'));
  M_ASSERT(!og1.existsEdge('d', 'a'));
  M_ASSERT(!og1.existsEdge('e', 'e'));

  //check that og2 is identical to og1
  M_ASSERT(og2.nodes() == 4);
  M_ASSERT(og2.edges() == 3);
  M_ASSERT(og2.existsEdge('a', 'a'));
  M_ASSERT(og2.existsEdge('a', 'd'));
  M_ASSERT(og2.existsEdge('c', 'c'));
  M_ASSERT(!og2.existsEdge('d', 'a'));
  M_ASSERT(!og2.existsEdge('e', 'e'));

  og1.removeNode('a');
  og1.removeNode('d');

  //check that og1 modified correctly
  M_ASSERT(og1.nodes() == 2);
  M_ASSERT(og1.edges() == 1);
  M_ASSERT(og1.existsEdge('c', 'c'));
  M_ASSERT(!og1.existsEdge('a', 'a'));
  M_ASSERT(!og1.existsEdge('a', 'd'));
  M_ASSERT(!og1.existsEdge('d', 'a'));
  M_ASSERT(!og1.existsEdge('e', 'e'));

  //check that og2 is intact
  M_ASSERT(og2.nodes() == 4);
  M_ASSERT(og2.edges() == 3);
  M_ASSERT(og2.existsEdge('a', 'a'));
  M_ASSERT(og2.existsEdge('a', 'd'));
  M_ASSERT(og2.existsEdge('c', 'c'));
  M_ASSERT(!og2.existsEdge('d', 'a'));
  M_ASSERT(!og2.existsEdge('e', 'e'));

}

void test_copy_assignment(){
  std::cout << "====== TEST_COPY_ASSIGNMENT ======" << std::endl;

  //-----------
  //copy assignment
  //-----------

  char nodes[] = {'x', 'y', 'z'};
  oriented_graph<char, equal_char> og1(nodes, sizeof(nodes)/sizeof(nodes[0]));
  og1.addEdge('x', 'x');
  og1.addEdge('y', 'y');
  og1.addEdge('x', 'y');
  og1.addEdge('y', 'x');

  M_ASSERT(og1.nodes() == 3);
  M_ASSERT(og1.edges() == 4);
  M_ASSERT(og1.existsEdge('x', 'x'));
  M_ASSERT(og1.existsEdge('y', 'y'));
  M_ASSERT(og1.existsEdge('x', 'y'));
  M_ASSERT(og1.existsEdge('y', 'x'));

  char nodes2[] = {'a', 'b', 'c', 'd'};
  oriented_graph<char, equal_char> og2(nodes2, sizeof(nodes2)/sizeof(nodes2[0]));
  og2.addEdge('a', 'a');
  og2.addEdge('a', 'd');
  og2.addEdge('c', 'c');

  M_ASSERT(og2.nodes() == 4);
  M_ASSERT(og2.edges() == 3);
  M_ASSERT(og2.existsEdge('a', 'a'));
  M_ASSERT(og2.existsEdge('a', 'd'));
  M_ASSERT(og2.existsEdge('c', 'c'));
  M_ASSERT(!og2.existsEdge('d', 'a'));
  M_ASSERT(!og2.existsEdge('e', 'e'));

  og2 = og1;

  //check that information was copied correctly
  M_ASSERT(og2.nodes() == 3);
  M_ASSERT(og2.edges() == 4);
  M_ASSERT(og2.existsEdge('x', 'x'));
  M_ASSERT(og2.existsEdge('y', 'y'));
  M_ASSERT(og2.existsEdge('x', 'y'));
  M_ASSERT(og2.existsEdge('y', 'x'));

  og2.removeNode('x');

  //check that og2 changed succesfully
  M_ASSERT(og2.nodes() == 2);
  M_ASSERT(og2.edges() == 1);
  M_ASSERT(og2.existsEdge('y', 'y'));
  M_ASSERT(!og2.existsEdge('z', 'z'));
  M_ASSERT(!og2.existsEdge('x', 'x'));
  M_ASSERT(!og2.existsEdge('x', 'y'));
  M_ASSERT(!og2.existsEdge('y', 'x'));

  //check that changes in og2 didn't affect og1
  M_ASSERT(og1.nodes() == 3);
  M_ASSERT(og1.edges() == 4);
  M_ASSERT(og1.existsEdge('x', 'x'));
  M_ASSERT(og1.existsEdge('y', 'y'));
  M_ASSERT(og1.existsEdge('x', 'y'));
  M_ASSERT(og1.existsEdge('y', 'x'));


}

int main(){
  test_basic_behaviour();
  test_exceptions();
  test_swap();
  test_copy_constructor();
  test_copy_assignment();

  // Print test summary
  testFramework::summary();
}
