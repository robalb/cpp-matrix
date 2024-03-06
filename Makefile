CXX = g++
CXX_TEST = g++
CXX_COV = g++

LINK_TARGET = main.exe
LINK_TARGET_TEST = main.test.exe
LINK_TARGET_COV = main.cov.exe

CXXFLAGS = -Wall
CXXFLAGS_TEST = -Wall -Wextra -g3 -O0 -fsanitize=address,undefined
CXXFLAGS_COV = -g3 -O0 -fprofile-arcs -ftest-coverage

CXXINCLUDES = .
CXXINCLUDES_TEST = .
CXXINCLUDES_COV = .

#------ production build ----------

$(LINK_TARGET): main.o 
	$(CXX) $(CXXFLAGS) -o $@ $^

main.o: main.cpp ograph.hpp
	$(CXX) $(CXXFLAGS) -I$(CXXINCLUDES) -o $@ -c main.cpp

#------- code coverage build ---------

$(LINK_TARGET_COV): main.cov.o 
	$(CXX_COV) $(CXXFLAGS_COV) -o $@ $^

main.cov.o: main.cpp ograph.hpp
	$(CXX_COV) $(CXXFLAGS_COV) -I$(CXXINCLUDES_COV) -o $@ -c main.cpp

#-------- asan test build --------

$(LINK_TARGET_TEST): main.test.o 
	$(CXX_TEST) $(CXXFLAGS_TEST) -o $@ $^

main.test.o: main.cpp ograph.hpp
	$(CXX_TEST) $(CXXFLAGS_TEST) -I$(CXXINCLUDES_TEST) -o $@ -c main.cpp

#----------------

.PHONY: coverage_lines
coverage_lines: $(LINK_TARGET_COV)
	./$(LINK_TARGET_COV)
	gcov main.cov.cpp -Hfmqr #-k
	echo "lines without coverage:"
	cat ograph.hpp.gcov | grep "#####"

.PHONY: coverage
coverage: coverage_lines
	grep --color=always '#####\|$$' ograph.hpp.gcov | less -R

.PHONY: test
test: $(LINK_TARGET_TEST)
	./$(LINK_TARGET_TEST)

.PHONY: debug
debug: $(LINK_TARGET)
	gdb ./$(LINK_TARGET)

#valgrind must run on LINK_TARGET, the build without asan
.PHONY: leaktest
leaktest: $(LINK_TARGET)
	valgrind --leak-check=full ./$(LINK_TARGET) 

.PHONY: clean
clean:
	rm *.o *.exe *.gcov *.gcda *.gcno

.PHONY: doc
doc:
	doxygen

.PHONY: all
all: main.exe doc
