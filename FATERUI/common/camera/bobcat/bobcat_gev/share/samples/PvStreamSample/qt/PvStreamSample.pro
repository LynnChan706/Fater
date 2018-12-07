TEMPLATE = app
TARGET =  PvStreamSample
CONFIG += debug_and_release
    
HEADERS += \
        
SOURCES += ../PvStreamSample.cpp

LIBS += -L$(PUREGEV_ROOT)/lib/ -lPvBuffer -lPvBase -lEbTransportLayerLib -lPvDevice -lPvGUI -lPvPersistence -lPvGenICam -lPvStream -lPvVirtualDevice -lPvSerial

INCLUDEPATH += $(PUREGEV_ROOT)/include
   
CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT PV_NO_GEV1X_PIXEL_TYPES PV_NO_DEPRECATED_PIXEL_TYPES _UNIX_ _LINUX_
