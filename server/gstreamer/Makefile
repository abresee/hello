CXXFLAGS = `pkg-config --cflags gstreamer-1.0 gstreamer-plugins-base-1.0`
LDFLAGS = `pkg-config --libs gstreamer-1.0 gstreamer-plugins-base-1.0`

all: hello_gstreamer.o
	$(CXX) -o hello_gstreamer hello_gstreamer.o $(LDFLAGS)	
hello_gstreamer.o:
	$(CXX) -c hello_gstreamer.cpp $(CXXFLAGS)

.PHONY: clean
clean:
	rm hello_gstreamer hello_gstreamer.o
