#! /bin/bash
swig -python -c++ ./CameraBobcat.i



g++ -fPIC -std=c++11 -c CameraBobcat.cpp CameraBobcat_wrap.cxx \
    -I./bobcat_gev/include \
    -I/usr/local/include \
    -I/usr/include \
    -I/usr/include/python2.7

g++ -shared -std=c++11 CameraBobcat.o CameraBobcat_wrap.o -o _CameraBobcat.so \
    -I/usr/include/python2.7 \
    -I/usr/include \
    -I/usr/local/include \
    -I./bobcat_gev/include \
    -L/usr/lib \
    -L/usr/local/lib \
    -L./bobcat_gev/lib \
    -L./bobcat_gev/lib/genicam/bin/Linux64_x64 \
    -lPvBase \
    -lPvDevice \
    -lPvStream \
    -lPvBuffer \
    -lPvPersistence \
    -lSimpleImagingLib
