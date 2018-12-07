#! /bin/bash
swig -python -c++ ./CameraMindVision.i

g++ -fPIC -std=c++11 -c CameraMindVision.cpp CameraMindVision_wrap.cxx  -I/usr/local/include -I/usr/include -I/usr/include/python2.7

g++ -shared -std=c++11 CameraMindVision.o CameraMindVision_wrap.o -o _CameraMindVision.so -I/usr/include/python2.7 -I/usr/include -I/usr/local/include -L/usr/lib -L/lib -lMVSDK
