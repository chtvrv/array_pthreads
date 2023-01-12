CC = gcc
CXX = g++

C_FLAGS = \
	-std=c99 \
	-Wall \
	-Wextra \
	-Werror

CXX_FLAGS = \
	-std=c++20 \
	-Wall \
	-Wextra \
	-Werror

LINK_FLAGS = \
	-pthread \
	-lthread_realization \
	-L. \
	-lsingle_realization

TEST_LINK_FLAGS = \
	-pthread \
	-lthread_realization \
	-L. \
	-Wl,-rpath,. \
	-lsingle_realization \
	-lgtest_main \
	-lgtest

TEST_TARGET = test.out

HDRS = \
	project/include/

LIBRARIES = \
	libthread_realization.so \
	libsingle_realization.a

TEST_OBJECTS = \
	objects/preparation.o \
	objects/test.o

test: $(TEST_OBJECTS) $(LIBRARIES)
	$(CXX) $(TEST_OBJECTS) -o $(TEST_TARGET) $(TEST_LINK_FLAGS)

install: $(LIBRARIES)
	cp libthread_realization.so /usr/local/lib/

objects/test.o: project/src/test.cpp
	$(CXX) $(CXX_FLAGS) -I $(HDRS) -c $^ -o $@

objects/preparation.o: project/src/preparation.c
	$(CC) $(C_FLAGS) -I $(HDRS) -c $^ -o $@

objects/thread_realization.o: project/src/thread_realization.c
	$(CC) $(C_FLAGS) -fPIC -I $(HDRS) -c $^ -o $@

libthread_realization.so: objects/thread_realization.o
	$(CC) -shared $^ -o $@

libsingle_realization.a: objects/single_realization.o
	ar r $@ $^

objects/single_realization.o: project/src/single_realization.c
	$(CC) $(C_FLAGS) -I $(HDRS) -c $^ -o $@

clean:
	rm -rf $(TARGET) $(TEST_TARGET) objects/*.o *.bin *.so *.a
