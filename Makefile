CXXFLAGS := -pipe -O2 -Wall -Wextra -Iexternal/powerslaves -std=c++11
LDLIBS := -lhidapi-libusb

SOURCE_FILES := src/ak2itool.cpp $(wildcard src/devices/*.cpp) src/device.cpp

OBJECT_FILES := $(SOURCE_FILES:.cpp=.o) external/powerslaves/libpowerslaves.a

all: ak2itool

external/powerslaves:
	git submodule init
	git submodule update

external/powerslaves/libpowerslaves.a: external/powerslaves
	make -C external/powerslaves/ libpowerslaves.a

ak2itool: $(OBJECT_FILES)
	$(CXX) $(CXXFLAGS) $^ $(LDLIBS) -o $@

clean:
	rm -f ak2itool src/*.o src/devices/*.o
	make -C external/powerslaves/ clean
