PYTHONCXXFLAGS = `python3.3-config --cflags`
PYTHONLDFLAGS = `python3.3-config --ldflags`

GSTREAMERCXXFLAGS = `pkg-config --cflags gstreamer-{,{,plugins-}base-}1.0`
GSTREAMERLDFLAGS = `pkg-config --libs gstreamer-{,base-}1.0`

CXXFLAGS = -std=gnu++11

CXXFLAGS += $(GSTREAMERCXXFLAGS) $(PYTHONCXXFLAGS)
LDFLAGS += $(GSTREAMERLDFLAGS) $(PYTHONLDFLAGS)

default: python_library

SOFLAGS = -shared -Wl,-soname,"$@",--export-dynamic -lboost_python3 

SNAMES = hello_gstreamer
SOBJ = $(addsuffix .so, $(SNAMES))
ONAMES = player
OBJ = $(addsuffix .o, $(ONAMES))
SRCS = $(addsuffix .cpp, $(SNAMES) $(ONAMES))

python_library: $(SOBJ) $(OBJ) 

test: $(OBJ) main.o 
	$(CXX) -o test $(OBJ) main.o $(GSTREAMERLDFLAGS)

%.so : %.o $(OBJ)
	$(CXX) $(SOFLAGS) $(LDFLAGS) $^ -o $@ 

.PHONY: clean
clean:
	rm -rf $(SOBJ) $(OBJ) main.o test

player.o: player.h
