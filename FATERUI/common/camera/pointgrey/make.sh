#! /bin/bash
swig -python -c++ ./CameraPointgrey.i

g++ -fPIC -std=c++11 -c CameraPointgrey.cpp CameraPointgrey_wrap.cxx -I/usr/include/flycapture -I/usr/local/include -I/usr/include -I/usr/include/python2.7

g++ -shared -std=c++11 CameraPointgrey.o CameraPointgrey_wrap.o -o _CameraPointgrey.so -I/usr/include/python2.7 -I/usr/include -I/usr/local/include -I/usr/include/flycapture -L/usr/lib -L/usr/local/lib -lflycapture
