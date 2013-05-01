CXXFLAGS = `pkg-config --cflags gstreamer-1.0 gstreamer-plugins-base-1.0`
LDFLAGS = `pkg-config --libs gstreamer-1.0 gstreamer-plugins-base-1.0`

hello_gstreamer: hello_gstreamer.o
	$(CXX) -o hello_gstreamer hello_gstreamer.o $(LDFLAGS)	
hello_gstreamer.o: hello_gstreamer.cpp
	$(CXX) -c hello_gstreamer.cpp $(CXXFLAGS)

.PHONY: clean
clean:
	rm -rf hello_gstreamer hello_gstreamer.o
