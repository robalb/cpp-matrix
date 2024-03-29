/**
 * @file main.cpp
 * @brief unit tests for the oriented_graph class
 */

#include <iostream>
#include <cassert>
#include "ograph.hpp"
#include "animal.hpp"
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

/**
 * @brief functor for custom class animal equality
 */
struct equal_animal {
  bool operator()(const animal &a, const animal &b) const {
    bool bothQuack = (a.doesQuack() == b.doesQuack());
    bool sameLegs = (a.getLegs() == b.getLegs());
    return bothQuack && sameLegs;
  }
};

void test_custom_class(){
  std::cout << "====== TEST_CUSTOM_CLASS ======" << std::endl;

  //-----------
  //empty graph 1
  //-----------

  oriented_graph<animal, equal_animal> og;

  animal duck1 = animal(true, 2);
  animal duck2 = animal(true, 2);
  animal dog1 = animal(false, 2);

  assert(og.nodes() == 0);
  assert(og.edges() == 0);
  assert(!og.existsNode(duck1));

  std::cout << "*** add node ***" << std::endl;
  og.addNode(duck1);
  std::cout << "*** end add node ***" << std::endl;

  assert(og.nodes() == 1);
  assert(og.edges() == 0);
  M_ASSERT_THROW(
      og.addNode(duck2),
      invalidNodeException
      )
  assert(og.existsNode(duck1));
  //test duck typing
  assert(og.existsNode(duck2));


  //-----------
  //non-empty graph 2
  //-----------
  animal list[] = {dog1};
  oriented_graph<animal, equal_animal> og2(list, 1);

  assert(og2.existsNode(dog1));
  assert(!og2.existsNode(duck1));
  assert(og2.nodes() == 1);
  assert(og2.edges() == 0);

  //add edge
  og2.addEdge(dog1, dog1);
  assert(og2.nodes() == 1);
  assert(og2.edges() == 1);
  assert(og2.existsEdge(dog1, dog1));
  M_ASSERT_THROW(
      og2.addEdge(dog1, dog1),
      invalidEdgeException
      )

  std::cout << "*** copy assignment ***" << std::endl;
  og2 = og;
  std::cout << "*** copy assignment end ***" << std::endl;

  assert(og.nodes() == 1);
  assert(og.edges() == 0);
  assert(og.existsNode(duck1));

  //remove node
  og2.removeNode(duck1);
  assert(og2.nodes() == 0);
  assert(og2.edges() == 0);
  assert(!og2.existsNode(duck1));
}

void test_custom_class_2(){
  std::cout << "====== TEST_CUSTOM_CLASS 2 ======" << std::endl;

  animal duck1 = animal(true, 2);
  animal duck2 = animal(true, 2);
  animal spider1 = animal(false, 8);
  animal spider2 = animal(false, 7);//born without one leg, but it's not affecting its life too much
  animal cat1 = animal(false, 4);

  animal invalid_animals[] = {duck1, duck2, cat1};
  animal valid_animals[] = {duck1, spider1, cat1};

  //test duplicate nodes in constructor
  oriented_graph<animal, equal_animal> *invalid_og = nullptr;
  M_ASSERT_THROW(
    (invalid_og = new oriented_graph<animal, equal_animal>(invalid_animals, 3)),
    invalidNodeException
  );
  delete invalid_og;

  //valid nodes in constructor
  oriented_graph<animal, equal_animal> og(valid_animals, 3);

  assert(og.nodes() == 3);
  assert(og.edges() == 0);
  assert(og.existsNode(duck1));
  assert(og.existsNode(spider1));
  assert(og.existsNode(cat1));

  og.addEdge(duck1, duck1);
  assert(og.edges() == 1);
  assert(og.existsEdge(duck1, duck1));
  assert(og.existsEdge(duck1, duck2));
  og.removeEdge(duck1, duck1);
  assert(og.edges() == 0);
  assert(!og.existsEdge(duck1, duck1));
  assert(!og.existsEdge(duck1, duck2));
  assert(!og.existsEdge(duck1, spider2));

  og.removeNode(duck1);
  assert(og.nodes() == 2);
  assert(og.edges() == 0);
  assert(!og.existsNode(duck1));
  assert(og.existsNode(spider1));
  assert(og.existsNode(cat1));

  og.removeNode(cat1);
  assert(og.nodes() == 1);
  assert(og.edges() == 0);
  assert(!og.existsNode(duck1));
  assert(og.existsNode(spider1));
  assert(!og.existsNode(cat1));

  og.removeNode(spider1);
  assert(og.nodes() == 0);
  assert(og.edges() == 0);
  assert(!og.existsNode(duck1));
  assert(!og.existsNode(spider1));
  assert(!og.existsNode(cat1));

}


void test_custom_class_iterator(){
  std::cout << "====== TEST_CUSTOM_CLASS_ITERATOR ======" << std::endl;

  oriented_graph<animal, equal_animal> og;
  animal duck1 = animal(true, 2);
  animal duck2 = animal(true, 2);
  animal dog1 = animal(false, 2);
  og.addNode(duck1);
  og.addNode(dog1);

  //iterator
  oriented_graph<animal, equal_animal>::const_iterator i = og.begin();

  //test access
  assert(i->doesQuack() == duck1.doesQuack());

  //empty iterator
  oriented_graph<animal, equal_animal> og1;
  assert(og1.begin() == og1.end());

  og1.addNode(duck1);
  assert(++og1.begin() == og1.end());

  //test basic usage
  og1.addNode(dog1);
  int sum = 0;
  for(oriented_graph<animal, equal_animal>::const_iterator i = og1.begin(); i != og1.end(); i++){
    sum += i->getLegs();
  }
  assert(sum == 4);

}

void pass_by_value(oriented_graph<int, equal_int> d) {
  d.addNode(10);
  std::cout << "*** leaving pass_by_value ***" << std::endl;
}

void pass_by_reference(oriented_graph<int, equal_int> &d) {
  d.addNode(11);
  std::cout << "*** leaving pass_by_reference ***" << std::endl;
}

void pass_by_pointer(oriented_graph<int, equal_int> *d) {
  d->addNode(12);
  std::cout << "*** leaving pass_by_pointer ***" << std::endl;
}

void test_class_behaviour(){
  std::cout << "====== TEST_CLASS_BEHAVIOUR ======" << std::endl;
  oriented_graph<int, equal_int> og;

  //we expect a call to the copy constructor here
  assert(og.nodes() == 0);
  std::cout << "*** calling pass_by_value ***" << std::endl;
  pass_by_value(og);
  assert(og.nodes() == 0);

  std::cout << "*** calling pass_by_reference ***" << std::endl;
  pass_by_reference(og);
  assert(og.nodes() == 1);

  std::cout << "*** calling pass_by_pointer ***" << std::endl;
  pass_by_pointer(&og);
  assert(og.nodes() == 2);

  //we expect 5 calls to the default constructor here
  std::cout << "*** default constructor ***" << std::endl;
  oriented_graph<int, equal_int> og1[5];

  //we expect 6 calls to the destructor here
  std::cout << "*** end of function ***" << std::endl;
}

void test_basic_behaviour(){
  std::cout << "====== TEST_BASIC_BEHAVIOUR ======" << std::endl;

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

void test_exception_in_constructor(){
  std::cout << "====== TEST_EXCEPTION_IN_CONSTRUCTOR ======" << std::endl;

  bool success = false;
  int invalid_list[] = {1,2,2,3};
  std::cout << "starting try block" << std::endl;
  try{
    oriented_graph<int, equal_int> og1(invalid_list, 4);
    std::cout << "never reached" << std::endl;
  }
  catch(invalidNodeException &e){
    success = true;
    std::cout << "caught invalidNodeException, as expected" << std::endl;
  }
  catch(...){
    std::cout << "caught other exc " << std::endl;
  }
  std::cout << "out of try block" << std::endl;
  assert(success);
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

  assert(og1.nodes() == 6);
  assert(og1.edges() == 0);
  assert(og2.nodes() == 2);
  assert(og2.edges() == 1);

  og1.swap(og2);
  og2.swap(og1);
  
  assert(og1.nodes() == 6);
  assert(og1.edges() == 0);
  assert(og2.nodes() == 2);
  assert(og2.edges() == 1);
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

  assert(og1.nodes() == 4);
  assert(og1.edges() == 3);
  assert(og1.existsEdge('a', 'a'));
  assert(og1.existsEdge('a', 'd'));
  assert(og1.existsEdge('c', 'c'));
  assert(!og1.existsEdge('d', 'a'));
  assert(!og1.existsEdge('e', 'e'));

  oriented_graph<char, equal_char> og2 = og1;

  //check that og1 is intact
  assert(og1.nodes() == 4);
  assert(og1.edges() == 3);
  assert(og1.existsEdge('a', 'a'));
  assert(og1.existsEdge('a', 'd'));
  assert(og1.existsEdge('c', 'c'));
  assert(!og1.existsEdge('d', 'a'));
  assert(!og1.existsEdge('e', 'e'));

  //check that og2 is identical to og1
  assert(og2.nodes() == 4);
  assert(og2.edges() == 3);
  assert(og2.existsEdge('a', 'a'));
  assert(og2.existsEdge('a', 'd'));
  assert(og2.existsEdge('c', 'c'));
  assert(!og2.existsEdge('d', 'a'));
  assert(!og2.existsEdge('e', 'e'));

  og1.removeNode('a');
  og1.removeNode('d');

  //check that og1 modified correctly
  assert(og1.nodes() == 2);
  assert(og1.edges() == 1);
  assert(og1.existsEdge('c', 'c'));
  assert(!og1.existsEdge('a', 'a'));
  assert(!og1.existsEdge('a', 'd'));
  assert(!og1.existsEdge('d', 'a'));
  assert(!og1.existsEdge('e', 'e'));

  //check that og2 is intact
  assert(og2.nodes() == 4);
  assert(og2.edges() == 3);
  assert(og2.existsEdge('a', 'a'));
  assert(og2.existsEdge('a', 'd'));
  assert(og2.existsEdge('c', 'c'));
  assert(!og2.existsEdge('d', 'a'));
  assert(!og2.existsEdge('e', 'e'));

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

  assert(og1.nodes() == 3);
  assert(og1.edges() == 4);
  assert(og1.existsEdge('x', 'x'));
  assert(og1.existsEdge('y', 'y'));
  assert(og1.existsEdge('x', 'y'));
  assert(og1.existsEdge('y', 'x'));

  char nodes2[] = {'a', 'b', 'c', 'd'};
  oriented_graph<char, equal_char> og2(nodes2, sizeof(nodes2)/sizeof(nodes2[0]));
  og2.addEdge('a', 'a');
  og2.addEdge('a', 'd');
  og2.addEdge('c', 'c');

  assert(og2.nodes() == 4);
  assert(og2.edges() == 3);
  assert(og2.existsEdge('a', 'a'));
  assert(og2.existsEdge('a', 'd'));
  assert(og2.existsEdge('c', 'c'));
  assert(!og2.existsEdge('d', 'a'));
  assert(!og2.existsEdge('e', 'e'));

  og2 = og1;

  //check that information was copied correctly
  assert(og2.nodes() == 3);
  assert(og2.edges() == 4);
  assert(og2.existsEdge('x', 'x'));
  assert(og2.existsEdge('y', 'y'));
  assert(og2.existsEdge('x', 'y'));
  assert(og2.existsEdge('y', 'x'));

  og2.removeNode('x');

  //check that og2 changed succesfully
  assert(og2.nodes() == 2);
  assert(og2.edges() == 1);
  assert(og2.existsEdge('y', 'y'));
  assert(!og2.existsEdge('z', 'z'));
  assert(!og2.existsEdge('x', 'x'));
  assert(!og2.existsEdge('x', 'y'));
  assert(!og2.existsEdge('y', 'x'));

  //check that changes in og2 didn't affect og1
  assert(og1.nodes() == 3);
  assert(og1.edges() == 4);
  assert(og1.existsEdge('x', 'x'));
  assert(og1.existsEdge('y', 'y'));
  assert(og1.existsEdge('x', 'y'));
  assert(og1.existsEdge('y', 'x'));

}

void test_iterator(){
  std::cout << "====== TEST_ITERATOR ======" << std::endl;

  char nodes[] = {'a', 'b', 'c'};
  oriented_graph<char, equal_char> og(nodes, sizeof(nodes)/sizeof(nodes[0]));
  oriented_graph<char, equal_char>::const_iterator a = og.begin();

  //test access
  assert(*a == 'a');

  //test post increment
  assert(*(a++) == 'a');
  assert(*a == 'b');

  //test pre increment
  assert(*(++a) == 'c');
  assert(*a == 'c');

  //test equality, and access boundaries
  a++;
  assert(a == og.end());

  //test assignment operator
  a = og.begin();
  assert(*a == 'a');

  //empty iterator
  oriented_graph<int, equal_int> og1;
  assert(og1.begin() == og1.end());

  og1.addNode(1);
  assert(++og1.begin() == og1.end());

  //test basic usage
  og1.addNode(2);
  int sum = 0;
  for(oriented_graph<int, equal_int>::const_iterator i = og1.begin(); i != og1.end(); i++){
    sum += *i;
  }
  assert(sum == 3);
}


int main(){
  test_custom_class();
  test_custom_class_2();
  test_custom_class_iterator();
  test_class_behaviour();
  test_basic_behaviour();
  test_exceptions();
  test_exception_in_constructor();
  test_swap();
  test_copy_constructor();
  test_copy_assignment();
  test_iterator();
}
