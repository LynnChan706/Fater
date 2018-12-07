SO_TARGET=_CameraPointgrey.so

INCLUDE=-I/usr/include/flycapture/ -I/usr/local/include/ -I/usr/include/ -I/usr/include/python2.7/
LIB_DIR=-L/usr/lib -L/usr/local/lib
LIBS=-lflycapture

all: $(SO_TARGET)
CameraPointgrey_wrap.cxx:
	swig -python -c++ ./CameraPointgrey.i
CameraPointgrey.o: CameraPointgrey.cpp
	g++ -fPIC -c -o $@ $< $(INCLUDE)
CameraPointgrey_wrap.o: CameraPointgrey_wrap.cxx
	g++ -fPIC -c -o $@ $< $(INCLUDE)
${SO_TARGET}:CameraPointgrey.o CameraPointgrey_wrap.o
	g++ -shared -o $@ $< $(INCLUDE) $(LIB_DIR) $(LIBS)
	rm -rf CameraPointgrey.o CameraPointgrey_wrap.o CameraPointgrey_wrap.cxx
.PHONY: clean

clean:
	rm -rf CameraPointgrey.py* CameraPointgrey.o CameraPointgrey_wrap.o CameraPointgrey_wrap.cxx ${SO_TARGET}
