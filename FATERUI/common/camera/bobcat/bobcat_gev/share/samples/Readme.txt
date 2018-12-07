
-e Copyright (c) 2002-2015

====================================
 Qt SDK
====================================

The eBUS SDK is build using the Qt SDK 4.8.6.

The sample Makefile is assuming that the development package is installed and that the
qmake location is in the system PATH.

====================================
 Cross-compiling the samples
====================================

Before cross-compiling the sample, the user must ensure that all the required environment for 
its toolschain is configure properly. It must also define the environment variables CC, CXX and LD
to point to you gcc, g++ and ld version of the toolschains.
The bin/set_puregev_env.sh must be sourced to define the configuration / location of the SDK as well.

