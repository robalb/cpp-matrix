#ifndef OGRAPH_HPP
#define OGRAPH_HPP

#include <algorithm> // std::swap
#include <iostream> // std::ostream
#include <iterator> // std::forward_iterator_tag
#include <cstddef>  // std::ptrdiff_t

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
     * @brief delete all the internal data of the matrix
     *
     * @post _size = 0
     * @post _nodes = nullptr
     * @post _matrix = nullptr
     */
    void _clear(){
      delete[] _nodes;

      for(size_type i=0; i<_size; i++)
        delete[] _matrix[i];
      delete[] _matrix;

      _nodes = nullptr;
      _matrix = nullptr;
      _size = 0;
    }

    void _init(){
      
    }

    /**
     * @brief find the index position of a node in the _nodes list
     *
     * @param node the node to search
     * @return the index position of the given node in _nodes
     */
    int _index(const T &node){
      for(size_type i=0; i<_size; i++)
        if(_eql(_nodes[i], node))
          return i;
      return -1;
    }

    const T &_nodeAt(const int index){

    }


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

    //TODO: swap method, assignment, copy


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
    size_type edges() const{
      size_type count = 0;

      for(int i=0; i<_size; i++)
        for(int j=0; j<_size; j++)
          count += _matrix[i][j];

      return count;
    }

    /**
     * @brief print a visual representation of the graph to stdout
     *
     */
    void print() const{
      for(int i=0; i<_size; i++){
        for(int j=0; j<_size; j++){
          std::cout << _matrix[i][j] << " ";
        }
        std::cout << std::endl;
      }
    }

    //TODO: remove after edge methods implementations
    void debug_setmatrix(int** data){
      for(int i=0; i<_size; i++){
        for(int j=0; j<_size; j++){
          _matrix[i][j] = data[i][j];
        }
      }
    }

    bool existsNode(T &node) const{
      return (_index(node) != -1);
    }

    bool existsEdge(T nodeFrom, T nodeTo) const{

    }

    void addNode(const T &node){
      if(existsNode(node))
        throw;//TODO

      //create new, larger data structures
      //TODO try
      size_type new_size = _size+1;
      T* new_nodes = new T[new_size];
      int** new_matrix = new int*[new_size];
      for(size_type i=0; i<new_size; i++)
        new_matrix[i] = new int[new_size];

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

    void removeNode(T node){
      if(!existsNode(node))
        throw;//TODO

      //create new, smaller data structures
      //TODO try
      const size_type new_size = _size-1;
      T* new_nodes = new T[new_size];
      int** new_matrix = new int*[new_size];
      for(size_type i=0; i<new_size; i++)
        new_matrix[i] = new int[new_size];

      //copy old data structures to new data structures
      const int skip_index = _index(node);
      for(size_type i=0, new_i=0; i<_size; i++){
        new_nodes[new_i] = _nodes[i];
        if(i != skip_index) new_i++;
      }
      for(size_type i=0, new_i=0; i<_size; i++)
        for(size_type j=0, new_j=0; j<_size; j++){
          new_matrix[new_i][new_j] = _matrix[i][j];
          if(i != skip_index) new_i++;
          if(j != skip_index) new_j++;
        }

      //delete old data structures
      _clear();

      //commit
      _size = new_size;
      _nodes = new_nodes;
      _matrix = new_matrix;

    }

    void addEdge(T nodeFrom, T nodeTo){

    }

    void removeEdge(T nodeFrom, T nodeTo){

    }




};


#endif
