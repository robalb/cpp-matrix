CXX = g++
CXX_TEST = g++

LINK_TARGET = main.exe
LINK_TARGET_TEST = main.test.exe

CXXFLAGS = -Wall
CXXFLAGS_TEST = -Wall -Wextra -g3 -O0 \
								-fsanitize=address,undefined

CXXINCLUDES = .
CXXINCLUDES_TEST = .

#----------------

.PHONY: test
test: $(LINK_TARGET_TEST)
	./$(LINK_TARGET_TEST)

.PHONY: debug
debug: $(LINK_TARGET_TEST)
	gdb ./$(LINK_TARGET_TEST)

#valgrind must run on LINK_TARGET, the build without asan
.PHONY: leaktest
leaktest: $(LINK_TARGET)
	valgrind --leak-check=full ./$(LINK_TARGET) 


$(LINK_TARGET_TEST): main.test.o 
	$(CXX_TEST) $(CXXFLAGS_TEST) -o $@ $^

main.test.o: main.cpp olist.hpp
	$(CXX_TEST) $(CXXFLAGS_TEST) -I$(CXXINCLUDES_TEST) -o $@ -c main.cpp

#----------------

$(LINK_TARGET): main.o 
	$(CXX) $(CXXFLAGS) -o $@ $^

main.o: main.cpp olist.hpp
	$(CXX) $(CXXFLAGS) -I$(CXXINCLUDES) -o $@ -c main.cpp

#----------------

.PHONY: clean
clean:
	rm *.o *.exe

.PHONY: doc
doc:
	doxygen

.PHONY: all
all: main.exe doc
