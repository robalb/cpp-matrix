/**
 * @file ograph.hpp
 * @brief header file implementing the oriented graph class
 *
 */

#ifndef OGRAPH_HPP
#define OGRAPH_HPP

#include <algorithm> // std::swap
#include <iostream>  // std::ostream
#include <iterator>  // std::forward_iterator_tag
#include <cstddef>   // std::ptrdiff_t
#include <exception> // std::exception

/**
 * @brief The node provided is not valid
 *
 * The node was not found, or duplicate
 */
class invalidNodeException: public std::exception {
  virtual const char* what() {
    return "Invalid node";
  }
};

/**
 * @brief The edge provided is not valid
 *
 * The edge was not found, or duplicate
 */
class invalidEdgeException: public std::exception {
  virtual const char* what() {
    return "Invalid edge";
  }
};

/**
 * @brief an oriented graph
 *
 * The class implements a generic ordered graph.
 * Label for the graph are of generic type T.
 * Each label is unique. There cannot be two nodes that are equal,
 *   equality is checked using the provided E functor.
 *
 * @tparam T type for the node labels
 * @tparam E functor used for node comparison
 */
template <typename T, typename E>
class oriented_graph {
  //traits
  public:
    typedef unsigned int size_type;
    typedef T value_type;

  //internal attributes
  private:

    /**
     * @brief the amount of nodes in the graph
     *
     */
    size_type _size;

    /**
     * @brief an indexed list of all nodes
     *
     * the index of a node in this list will be used
     * as key in the adjacency matrix
     *
     */
    T* _nodes;

    /**
     * @brief adjacency matrix for the graph
     *
     */
    int** _matrix;

    /**
     * @brief functor for the equality check between data of type T
     *
     */
    E _eql;

  //internal utilities
  private:

    /**
     * @brief delete the dynamic memory of a matrix
     *
     * @param matrix the matrix to delete
     * @param size the matrix size
     */
    void _delete_matrix(int **matrix, size_type size){
      if(matrix != nullptr)
        for(size_type i=0; i<size; i++)
          delete[] matrix[i];
      delete[] matrix;
    }

    /**
     * @brief initialize all the rows of a matrix to nullptrs
     *
     * call this when initializing a matrix, between the initialization
     * of the rows and the initialization of the columns.
     * The purpose of this method is to allow _delete_matrix to work
     * in the case of allocation errors on the columns
     *
     * Do not call this method on a fully initialized matrix
     * (This warning is the telltale sign that matrix should have been implemented in a standalone class)
     *
     * @param matrix a matrix where only the rows have been allocated
     * @param size
     */
    void _init_matrix(int **matrix, size_type size){
      if(matrix != nullptr)
        for(size_type i=0; i<size; i++)
          matrix[i] = nullptr;
    }

    /**
     * @brief delete all the internal data of the graph
     *
     * @post _size = 0
     * @post _nodes = nullptr
     * @post _matrix = nullptr
     */
    void _clear(){
      delete[] _nodes;
      _delete_matrix(_matrix, _size);
      _nodes = nullptr;
      _matrix = nullptr;
      _size = 0;
    }

    /**
     * @brief find the index position of a node in the _nodes list
     *
     * @param node the node to search
     * @return the index position of the given node in _nodes
     */
    int _index(const T &node) const{
      for(size_type i=0; i<_size; i++)
        if(_eql(_nodes[i], node))
          return i;
      return -1;
    }

  //special members
  public:

    /**
     * @brief Default constructor
     *
     * @post _size = 0
     * @post _nodes = nullptr
     * @post _matrix = nullptr
    */
    oriented_graph() : _size(0), _nodes(nullptr), _matrix(nullptr) {
      #ifndef NDEBUG 
      std::cout<<"oriented_graph()"<<std::endl;
      #endif
    }

    /**
     * @brief Secondary constructor
     *
     * @param nodes a list of nodes that are part of the graph
     * @param size the nodes list size
     *
     * @throw std::bad_alloc 
     * @post _size = size
     * @post _nodes != nullptr
     * @post _matrix != nullptr
     */
    oriented_graph(T* nodes, size_type size) : _size(0), _nodes(nullptr), _matrix(nullptr) {
      for(size_type i=0; i<size; i++){
        addNode(nodes[i]);
      }
      #ifndef NDEBUG 
      std::cout<<"oriented_graph(nodes, size)"<<std::endl;
      #endif
    }

    /**
     * @brief Destructor
     *
     * @post _size = 0
     * @post _nodes = nullptr
     * @post _matrix = nullptr
     */
    ~oriented_graph(){
      _clear();
      #ifndef NDEBUG 
      std::cout<<"~oriented_graph()"<<std::endl;
      #endif   
    }

    /**
     * @brief swap the state of the current instance with the given instance
     *
     * @param other the instance to swap with
     * @post _size != _size
     * @post _nodes != _nodes
     * @post _matrix != _matrix
     */
    void swap(oriented_graph &other) {
      std::swap(_size,other._size);
      std::swap(_nodes,other._nodes);
      std::swap(_matrix,other._matrix);
    }

    /**
     * @brief copy constructor
     *
     * @param other object to copy
     * @throw std::bad_alloc 
     * @post _size = size
     * @post _nodes != nullptr
     * @post _matrix != nullptr
     */
    oriented_graph(const oriented_graph &other): _size(0), _nodes(nullptr), _matrix(nullptr) {
      #ifndef NDEBUG 
      std::cout<<"oriented_graph(&oriented_graph)"<<std::endl;
      #endif   
      //copy without edges
      for(size_type i=0; i<other._size; i++){
        addNode(other._nodes[i]);
      }
      //copy edges
      for(size_type i=0; i<other._size; i++)
        for(size_type j=0; j<other._size; j++)
          _matrix[i][j] = other._matrix[i][j];
    }

    /**
     * @brief copy assignment
     *
     * @param other the element to copy
     * @return reference to current graph, post-copy
     * @throw std::bad_alloc 
     * @post _size != _size
     * @post _nodes != _nodes
     * @post _matrix != _matrix
     */
    oriented_graph& operator=(const oriented_graph &other){
      if (&other != this) {
        oriented_graph tmp(other);
        this->swap(tmp); 
      }
      return *this;
    }

  //public interface
  public:

    /**
     * @brief graph nodes size getter
     *
     * @return the amount of nodes in the graph
     */
    size_type nodes() const{
      return _size;
    }

    /**
     * @brief graph edges size getter
     *
     * @return the amount of edges in the graph
     */
    int edges() const{
      int count = 0;

      for(size_type i=0; i<_size; i++)
        for(size_type j=0; j<_size; j++)
          count += _matrix[i][j];

      return count;
    }

    /**
     * @brief print the graph edges to stdout
     *
     */
    void print() const{
      for(size_type i=0; i<_size; i++){
        for(size_type j=0; j<_size; j++){
          std::cout << _matrix[i][j] << " ";
        }
        std::cout << std::endl;
      }
    }

    /**
     * @brief check if a node is part of the graph
     *
     * @returns true the node exists
     * @returns false the node does not exist
     */
    bool existsNode(const T &node) const{
      return (_index(node) != -1);
    }

    /**
     * @brief check if an edge is part of the graph
     *
     * This implementation considers invalid nodes to be a valid query,
     * therefore providing an invalid node will not raies any exception
     *  
     * @param nodeFrom the start node
     * @param nodeTo the end node
     * @returns true the edge exist
     * @returns false the edge does not exist, or one of the two nodes does not exist
     */
    bool existsEdge(const T &nodeFrom, const T &nodeTo) const{
      int iFrom = _index(nodeFrom);
      int iTo = _index(nodeTo);
      if(iFrom == -1 || iTo == -1)
        return false;
      return (_matrix[iFrom][iTo] != 0);
    }

    /**
     * @brief add a node to the graph
     *
     * @param node node to add to the graph
     * @throw invalidNodeException the provided node already exist
     * @throw std::bad_alloc 
     * @post _size = _size+1
     * @post _nodes != _nodes
     * @post _matrix != _matrix
     */
    void addNode(const T &node){
      if(existsNode(node))
        throw invalidNodeException();

      //create new, larger data structures
      size_type new_size = _size+1;
      T* new_nodes = nullptr;
      int** new_matrix = nullptr;
      try{
        new_nodes = new T[new_size];
        new_matrix = new int*[new_size];
        _init_matrix(new_matrix, new_size);
        // throw std::bad_alloc(); //TODO:remove. decomment to simulate malloc issue
        for(size_type i=0; i<new_size; i++)
          new_matrix[i] = new int[new_size];
      }
      catch(...){
        #ifndef NDEBUG 
        std::cout<<"exception in addNode()"<<std::endl;
        #endif   
        delete[] new_nodes;
        _delete_matrix(new_matrix, new_size);
        throw;
      }

      //copy old data structures to new data structures
      for(size_type i=0; i<_size; i++)
        new_nodes[i] = _nodes[i];
      for(size_type i=0; i<_size; i++)
        for(size_type j=0; j<_size; j++)
          new_matrix[i][j] = _matrix[i][j];

      //add the new data
      new_nodes[new_size-1] = node;
      for(size_type i=0; i<new_size; i++){
        new_matrix[i][new_size-1] = 0;
        new_matrix[new_size-1][i] = 0;
      }

      //delete old data structures
      _clear();

      //commit
      _size = new_size;
      _nodes = new_nodes;
      _matrix = new_matrix;

      //TODO: should i std::swap and _then_ delete the new data?
    }

    /**
     * @brief remove a node from the graph
     *
     * @param node node to remove from the graph
     * @throw invalidNodeException the provided node does not exist
     * @throw std::bad_alloc
     * @post _size = _size-1
     * @post _nodes != _nodes
     * @post _matrix != _matrix
     */
    void removeNode(const T &node){
      if(!existsNode(node))
        throw invalidNodeException();

      //create new, smaller data structures
      const size_type new_size = _size-1;
      T* new_nodes = nullptr;
      int** new_matrix = nullptr;
      try{
        new_nodes = new T[new_size];
        new_matrix = new int*[new_size];
        _init_matrix(new_matrix, new_size);
        for(size_type i=0; i<new_size; i++)
          new_matrix[i] = new int[new_size];
      }
      catch(...){
        #ifndef NDEBUG 
        std::cout<<"exception in removeNode()"<<std::endl;
        #endif   
        delete[] new_nodes;
        _delete_matrix(new_matrix, new_size);
        throw;
      }

      //copy old data structures to new data structures
      const size_type skip_index = _index(node);
      for(size_type i=0, new_i=0; i<_size; i++){
        if(i == skip_index)
          continue;
        new_nodes[new_i] = _nodes[i];
        for(size_type j=0, new_j=0; j<_size; j++){
          if(j == skip_index)
            continue;
          new_matrix[new_i][new_j] = _matrix[i][j];
          new_j++;
        }
        new_i++;
      }

      //delete old data structures
      _clear();

      //commit
      _size = new_size;
      _nodes = new_nodes;
      _matrix = new_matrix;
    }

    /**
     * @brief add a direct edge between two nodes
     *
     * @param nodeFrom the start node
     * @param nodeTo the destination node
     * @throw invalidEdgeException the edge already exists
     * @throw invalidNodeException the provided nodes do not exist
     * @post _matrix[i][j] != _matrix[i][j]
     */
    void addEdge(const T &nodeFrom, const T &nodeTo){
      if(!existsNode(nodeFrom) || !existsNode(nodeTo))
        throw invalidNodeException();
      if(existsEdge(nodeFrom, nodeTo))
        throw invalidEdgeException();

      int weight = 1;
      int iFrom = _index(nodeFrom);
      int iTo = _index(nodeTo);
      _matrix[iFrom][iTo] = weight;
    }

    /**
     * @brief remove an existing direct edge between two nodes
     *
     * @param nodeFrom the start node
     * @param nodeTo the destination node
     * @throw invalidEdgeException the edge does not exist
     * @post _matrix[i][j] != _matrix[i][j]
     */
    void removeEdge(T nodeFrom, T nodeTo){
      if(!existsEdge(nodeFrom, nodeTo))
        throw invalidEdgeException();

      int iFrom = _index(nodeFrom);
      int iTo = _index(nodeTo);
      _matrix[iFrom][iTo] = 0;
    }

  //iterator implementation
  public:

    /**
     * @brief const forward iterator on the graph nodes
     *
     */
    class const_iterator {
      //traits:
    public:
      typedef std::forward_iterator_tag iterator_category;
      typedef T                         value_type;
      typedef ptrdiff_t                 difference_type;
      typedef const T*                  pointer;
      typedef const T&                  reference;

    
      /**
       * @brief base constructor
       */
      const_iterator() : n(nullptr) {}
      
      /**
       * @brief copy constructor
       */
      const_iterator(const const_iterator &other) : n(other.n) {}

      /**
       * @brief copy assignment
       */
      const_iterator& operator=(const const_iterator &other) {
        n = other.n;
        return *this;
      }

      /**
       * @brief iterator destructor
       */
      ~const_iterator() {}

      /**
       * @return a reference to the data pointed by the iterator
       */
      reference operator*() const {
        return *n;
      }

      /**
       * @return the address of the data pointed by the iterator
       */
      pointer operator->() const {
        return n;
      }
      
      /**
       * @brief post increment operator
       */
      const_iterator operator++(int) {
        const_iterator tmp(*this);
        n++;
        return tmp;
      }

      /**
       * @brief pre increment operator
       */
      const_iterator& operator++() {
        n++;
        return *this;
      }

      /**
       * @brief equality operator
       */
      bool operator==(const const_iterator &other) const {
        return (n == other.n);
      }
      
      /**
       * @brief inequality operator
       */
      bool operator!=(const const_iterator &other) const {
        return (n != other.n);
      }

    private:
      const T *n;

      friend class oriented_graph; 

      /**
       * @brief iterator constructor
       *
       * this constructor can only be called by oriented_graph, that has
       * been set as friend
       *
       * @param nn pointer to the graph nodes internal data structure
       */
      const_iterator(const T *nn) : n(nn) { }
      
    }; //class const_iterator
    


    /**
     * @brief return an iterator pointing to the first node in the graph
     *
     * The iterator yelds all nodes inserted in the graph, there is no rule
     * for the order in which they are returned.
     * Consider this an iterator over a list of the graph nodes, in a shuffled order
     * @return const_iterator iterator begin
     */
    const_iterator begin() const {
      return const_iterator(_nodes);
    }
    
    /**
     * @brief return an iterator pointing to the end boundary of the graph
     *
     * The iterator yelds all nodes inserted in the graph, there is no rule
     * for the order in which they are returned.
     * The end iterator can only be used as a marker for the end of an iteration.
     * Attempts to dereference it will cause undefined behaviour
     *
     * @return const_iterator iterator end
     */
    const_iterator end() const {
      return const_iterator(_nodes+_size);
    }

};


#endif
