/**
 * @file animal.hpp
 * @brief header file implementing an animal class, used in main for class tests
 */

#ifndef ANIMAL_HPP
#define ANIMAL_HPP

#include <algorithm> // std::swap
#include <iostream>  // std::ostream

/**
 * @brief class for a generic animal
 */
class animal{
  private:
    bool quacks;
    int legs;

  public:
    /**
     * @brief default constructor
     */
    animal() : quacks(false), legs(0) {
      #ifndef NDEBUG 
      std::cout<<"animal()"<<std::endl;
      #endif
    }

    /**
     * @brief secondary constructor
     */
    animal(bool quacks, int legs) : quacks(quacks), legs(legs) {
      #ifndef NDEBUG 
      std::cout<<"animal()"<<std::endl;
      #endif
    }

    /**
     * @brief destructor
     */
    ~animal(){
      #ifndef NDEBUG 
      std::cout<<"~animal()"<<std::endl;
      #endif   
    }

    /**
     * @brief copy constructor
     *
     * @param other object to copy
     */
    animal(const animal &other) : quacks(false), legs(0) {
      #ifndef NDEBUG 
      std::cout<<"animal(&animal)"<<std::endl;
      #endif   
      quacks = other.quacks;
      legs = other.legs;
    }

    /**
     * @brief swap the state of the current instance with the given instance
     *
     * @param other the instance to swap with
     */
    void swap(animal &other){
      #ifndef NDEBUG 
      std::cout<<"animal::swap"<<std::endl;
      #endif   
      std::swap(quacks, other.quacks);
      std::swap(legs, other.legs);
    }

    /**
     * @brief copy assignment
     *
     * @param other the element to copy
     */
    animal& operator=(const animal &other){
      #ifndef NDEBUG 
      std::cout<<"animal::operator="<<std::endl;
      #endif   
      if (&other != this) {
        animal tmp(other);
        this->swap(tmp); 
      }
      return *this;
    }

    /**
     * @brief quacks getter
     */
    bool doesQuack() const{
      return quacks;
    }

    /**
     * @brief legs getter
     */
    int getLegs() const{
      return legs;
    }

};

#endif

