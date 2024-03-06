/**
 * @file testframework.hpp
 * @brief single-file implementation of assert_throw
 * 
 * Example usage:
 *  <code>
 *  int main() {
 *      M_ASSERT_THROW(
 *          throw std::invalid_argument("Invalid argument"),
 *          std::invalid_argument
 *      );
 *  }
 *  </code>
 */

#include <iostream>

/**
 * @brief macro implementation of assert for exceptions
 *
 */
#define M_ASSERT_THROW(__expr, __type) {          \
  testFramework_caught = assertCaught::none;      \
  try{                                            \
    (__expr);                                     \
  }                                               \
  catch(const __type &e){                         \
    testFramework_caught = assertCaught::correct; \
  }                                               \
  catch(...){                                     \
    testFramework_caught = assertCaught::wrong;   \
  }                                               \
  assert(testFramework_assert_throw(              \
      testFramework_caught,                       \
      #__expr,                                    \
      #__type                                     \
      ));                                         \
}


/**
 * assert_throw internal data structure
 */
enum class assertCaught {none, wrong, correct};

/**
 * assert_throw global, do not use directly
 */
assertCaught testFramework_caught = assertCaught::none;

/**
 * test framework assert throw, use via macro expansion
 */
inline bool testFramework_assert_throw(assertCaught result, std::string expr, std::string type){
  if(result != assertCaught::correct){
    std::cout << "---------------------------" << std::endl;
    std::cout << "FAILED: " << expr << std::endl;
    std::cout << "    expected " << type << " exception, got ";
    if(result == assertCaught::wrong)
      std::cout << "a different one" << std::endl;
    else
      std::cout << "none" << std::endl;
  }
  return result == assertCaught::correct;
}

