#! /bin/bash
swig -python -c++ ./CameraSVS.i

g++ -fPIC -std=c++11 -c CameraSVS.cpp CameraSVS_wrap.cxx  -I/usr/local/include -I/usr/include -I/usr/include/python2.7 `pkg-config --cflags --libs opencv`

g++ -shared -std=c++11 CameraSVS.o CameraSVS_wrap.o -o _CameraSVS.so -I/usr/include/python2.7 -I/usr/include -I/usr/local/include -L/usr/lib -L/lib -lsvgige `pkg-config --cflags --libs opencv`
