# Parameters
# SRC_CS: The source C files to compie
# SRC_CPPS: The source CPP files to compile
# EXEC: The executable name

ifeq ($(SRC_CS) $(SRC_CPPS),)
  $(error No source files specified)
endif

ifeq ($(EXEC),)
  $(error No executable file specified)
endif

CC                  ?= gcc
CXX                 ?= g++

PUREGEV_ROOT        ?= ../../..
PV_LIBRARY_PATH      =$(PUREGEV_ROOT)/lib

CFLAGS              += -I$(PUREGEV_ROOT)/include -I.
CPPFLAGS            += -I$(PUREGEV_ROOT)/include -I. 
ifdef _DEBUG
    CFLAGS    += -g -D_DEBUG
    CPPFLAGS  += -g -D_DEBUG
else
    CFLAGS    += -O3
    CPPFLAGS  += -O3
endif
CFLAGS    += -D_UNIX_ -D_LINUX_ -DLINUX
CPPFLAGS  += -D_UNIX_ -D_LINUX_ -DLINUX

LDFLAGS             += -L$(PUREGEV_ROOT)/lib         \
                        -lPvBase                     \
                        -lPvDevice                   \
                        -lPvBuffer                   \
                        -lPvGenICam                  \
                        -lPvStream                   \
                        -lPvTransmitter              \
                        -lPvVirtualDevice	         \
		                -lPvAppUtils                 \
                        -lPvPersistence              \
                        -lPvSerial                   \
                        -lipxtruesense



# Conditional linking and usage of the GUI on the sample only when available
ifeq ($(wildcard $(PUREGEV_ROOT)/lib/libPvGUI.so),)
    CFLAGS    += -DPV_GUI_NOT_AVAILABLE
    CPPFLAGS  += -DPV_GUI_NOT_AVAILABLE
else
    LDFLAGS   += -lPvGUI
endif 

# Add simple imaging lib to the linker options only if available
ifneq ($(wildcard $(PUREGEV_ROOT)/lib/libSimpleImagingLib.so),)
    LDFLAGS   += -lSimpleImagingLib
endif 

# Configure Genicam
GEN_LIB_PATH = $(PUREGEV_ROOT)/lib/genicam/bin/Linux64_x64
LDFLAGS      += -L$(GEN_LIB_PATH)


# Configure Qt compilation if any
SRC_MOC              =
MOC			         =
RCC					 =
FILES_QTGUI          = $(shell grep -l QtGui *)
ifneq ($(FILES_QTGUI),)
	# This is a sample compiling Qt code
    HAVE_QT=$(shell which qmake-qt4 &>/dev/null ; echo $?)
    ifeq ($(HAVE_QT),1)
		# We cannot compile the sample without the Qt SDK!
 		$(error The sample $(EXEC) requires the Qt SDK to be compiled. See share/samples/Readme.txt for more details)
    endif

	# Query qmake to find out the folder required to compile
	QT_SDK_BIN        = $(shell qmake-qt4 -query QT_INSTALL_BINS)
	QT_SDK_LIB        = $(shell qmake-qt4 -query QT_INSTALL_LIBS)
	QT_SDK_INC        = $(shell qmake-qt4 -query QT_INSTALL_HEADERS)
	
	# We have a full Qt SDK installed, so we can compile the sample
	CFLAGS 	         += -I$(QT_SDK_INC) -I$(QT_SDK_INC)/QtGui -I$(QT_SDK_INC)/QtCore
	CPPFLAGS         += -I$(QT_SDK_INC) -I$(QT_SDK_INC)/QtGui -I$(QT_SDK_INC)/QtCore
	LDFLAGS          += -L$(QT_SDK_LIB) -lQtCore -lQtGui

    QT_LIBRARY_PATH   = $(QT_SDK_LIB)

    FILES_MOC            = $(shell grep -l Q_OBJECT *)
    ifneq ($(FILES_MOC),)
	    SRC_MOC           = $(FILES_MOC:%h=moc_%cxx)
	    FILES_QRC         = $(shell ls *.qrc)
	    SRC_QRC           = $(FILES_QRC:%qrc=qrc_%cxx)

	    OBJS             += $(SRC_MOC:%.cxx=%.o)
	    OBJS		     += $(SRC_QRC:%.cxx=%.o)

        MOC               = $(QT_SDK_BIN)/moc
  	    RCC               = $(QT_SDK_BIN)/rcc
    endif
endif

LD_LIBRARY_PATH       = $(PV_LIBRARY_PATH):$(QT_LIBRARY_PATH):$(GEN_LIB_PATH)
export LD_LIBRARY_PATH

OBJS      += $(SRC_CPPS:%.cpp=%.o)
OBJS      += $(SRC_CS:%.c=%.o)

all: $(EXEC)

clean:
	rm -rf $(OBJS) $(EXEC) $(SRC_MOC) $(SRC_QRC)

moc_%.cxx: %.h
	$(MOC) $< -o $@ 

qrc_%.cxx: %.qrc
	$(RCC) $< -o $@

%.o: %.cxx
	$(CXX) -fopenmp -c $(CPPFLAGS) -o $@ $<

%.o: %.cpp
	$(CXX) -fopenmp -c $(CPPFLAGS) -o $@ $<

%.o: %.c
	$(CC) -fopenmp -c $(CFLAGS) -o $@ $<

$(EXEC): $(OBJS)
	$(CXX) -fopenmp $(OBJS) -o $@ $(LDFLAGS) 

.PHONY: all clean
