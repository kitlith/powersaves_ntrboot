CXXFLAGS := -pipe -O2 -Wall -Wextra -std=c++11 \
	-Iexternal/powerslaves -Iexternal/flashcart_core \
	-Isrc
LDLIBS := -lhidapi-libusb

SOURCE_FILES := \
	$(wildcard src/*.cpp) \
	$(wildcard external/flashcart_core/devices/*.cpp) \
	external/flashcart_core/device.cpp

OBJECT_FILES := $(SOURCE_FILES:.cpp=.o) external/powerslaves/libpowerslaves.a

all: ak2itool

external/powerslaves:
	git submodule init
	git submodule update

external/flashcart_core:
	git submodule init
	git submodule update

external/powerslaves/libpowerslaves.a: external/powerslaves
	make -C external/powerslaves/ libpowerslaves.a

ak2itool: $(OBJECT_FILES)
	$(CXX) $(CXXFLAGS) $^ $(LDLIBS) -o $@

clean:
	rm -f ak2itool src/*.o
	rm -f external/flashcart_core/*.o external/flashcart_core/devices/*.o
	make -C external/powerslaves/ clean
