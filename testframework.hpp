/**
 * @brief single-file test framework
 * 
 * Example usage:
 *  <code>
 *  int main() {
 *      M_ASSERT(2 + 2 == 4);
 *      M_ASSERT(5 * 3 == 14);
 *
 *      M_ASSERT_THROW(throw std::invalid_argument("Invalid argument"), std::invalid_argument);
 *      M_ASSERT_THROW(throw std::logic_error("Logic error"), std::invalid_argument);
 *
 *      testFramework::summary();
 *  }
 *  </code>
 */

#include <iostream>
#include <string>

#define M_ASSERT(__expr) testFramework::assertFunc(__expr, #__expr, __LINE__)

#define M_ASSERT_THROW(__expr, __type) {     \
    testFramework::assertThrow_setNone();    \
    try{                                     \
        (__expr);                            \
    }                                        \
    catch(const __type &e){                  \
    testFramework::assertThrow_setCorrect(); \
    }                                        \
    catch(...){                              \
    testFramework::assertThrow_setWrong();   \
    }                                        \
    testFramework::assertThrowFunc(          \
        #__expr,                             \
        #__type,                             \
        __LINE__                             \
        );                                   \
}


class testFramework {
public:
    enum class caughtType {none, wrong, correct};
    static int success;
    static int total;
    static caughtType caught;

    static void assertFunc(bool result, std::string expr, int line) {
        total++;
        if (result) {
            success++;
        } else {
            std::cout << "---------------------------" << std::endl;
            std::cout << "FAILED: line " << line << ": " << expr << std::endl;
            std::cout << "    returned false" << std::endl;
        }
    }

    static void assertThrow_setNone(){
      caught = caughtType::none;
    }
    static void assertThrow_setCorrect(){
      caught = caughtType::correct;
    }
    static void assertThrow_setWrong(){
      caught = caughtType::wrong;
    }

    static void assertThrowFunc(std::string expr, std::string type, int line) {
        total++;
        if (caught == caughtType::correct) {
            success++;
            return;
        }
        std::cout << "---------------------------" << std::endl;
        std::cout << "FAILED: line " << line << ": " << expr << std::endl;
        std::cout << "    expected " << type << " exception, got ";
        if (caught == caughtType::wrong)
            std::cout << "a different one" << std::endl;
        else
            std::cout << "none" << std::endl;
    }

    static void summary() {
        std::cout << "---------------------------" << std::endl;
        std::cout << "tests passed: " << success << "\\" << total << std::endl;
        std::cout << "tests failed: " << (total - success) << std::endl;
        std::cout << "---------------------------" << std::endl;
    }
};

int testFramework::success = 0;
int testFramework::total = 0;
testFramework::caughtType testFramework::caught = testFramework::caughtType::none;

