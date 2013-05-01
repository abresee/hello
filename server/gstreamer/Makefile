PYTHONCXXFLAGS = -I/usr/include/python3.3m -I/usr/include/python3.3m -Wno-unused-result -DDYNAMIC_ANNOTATIONS_ENABLED=1 -DNDEBUG -g -fwrapv -O3 -Wall -march=x86-64 -mtune=generic -O2 -pipe -fstack-protector --param=ssp-buffer-size=4 -D_FORTIFY_SOURCE=2 -march=x86-64 -mtune=generic -O2 -pipe -fstack-protector --param=ssp-buffer-size=4 -D_FORTIFY_SOURCE=2 -fPIC
PYTHONLDFLAGS = -lpthread -ldl -lutil -lm -lpython3.3m -Xlinker -export-dynamic 

GSTREAMERCXXFLAGS = -pthread -I/usr/include/gstreamer-1.0 -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include 
GSTREAMERLDFLAGS = -lgstreamer-1.0 -lgobject-2.0 -lgmodule-2.0 -pthread -lgthread-2.0 -pthread -lglib-2.0 

CXXFLAGS = $(PYTHONCXXFLAGS) $(GSTREAMERCXXFLAGS)

LDFLAGS = $(PYTHONLDFLAGS) $(GSTREAMERLDFLAGS)

SOFLAGS = -shared -Wl,-soname,"$@",--export-dynamic -lboost_python3 

CXX = g++ 

NAMES = hello_gstreamer
SOBJ = $(addsuffix .so, $(NAMES))

test: $(SOBJ)

%.so : %.o
	$(CXX) $(SOFLAGS) $(LDFLAGS) $< -o $@ 

.PHONY: clean
clean:
	rm -rf $(SOBJ)


