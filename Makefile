VPATH = src test
BUILD_DIR = build
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Isrc -c
LFLAGS = -Wall 

OBJS = $(BUILD_DIR)/Buffer.o \
       $(BUILD_DIR)/LineBuffer.o \
       $(BUILD_DIR)/Utilities.o

all : create_build_dir gee test

$(BUILD_DIR)/main.o : main.cpp Platform.h
	$(CXX) $(CXXFLAGS) $< -o $@

$(BUILD_DIR)/Buffer.o : Buffer.cpp Buffer.h Platform.h
	$(CXX) $(CXXFLAGS) $< -o $@

$(BUILD_DIR)/LineBuffer.o : LineBuffer.cpp LineBuffer.h Buffer.h Utilities.h Platform.h
	$(CXX) $(CXXFLAGS) $< -o $@

$(BUILD_DIR)/Utilities.o : Utilities.cpp Utilities.h Platform.h
	$(CXX) $(CXXFLAGS) $< -o $@

GEE_OBJS = $(OBJS) $(BUILD_DIR)/main.o

gee : $(GEE_OBJS)
	$(CXX) $(LFLAGS) $(GEE_OBJS) -o $(BUILD_DIR)/gee

TEST_OBJS = $(OBJS) $(BUILD_DIR)/test.o \
					$(BUILD_DIR)/Buffer_Tests.o \
					$(BUILD_DIR)/LineBuffer_Tests.o

$(BUILD_DIR)/test.o : test.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

$(BUILD_DIR)/Buffer_Tests.o : Buffer_Tests.cpp Buffer.cpp Buffer.h
	$(CXX) $(CXXFLAGS) $< -o $@

$(BUILD_DIR)/LineBuffer_Tests.o : LineBuffer_Tests.cpp LineBuffer.cpp LineBuffer.h Utilities.h
	$(CXX) $(CXXFLAGS) $< -o $@

test : $(TEST_OBJS)
	$(CXX) $(LFLAGS) $(TEST_OBJS) -o $(BUILD_DIR)/test

.PHONY : clean create_build_dir

create_build_dir:
	mkdir -p $(BUILD_DIR)

clean:
	rm -fr $(BUILD_DIR)


