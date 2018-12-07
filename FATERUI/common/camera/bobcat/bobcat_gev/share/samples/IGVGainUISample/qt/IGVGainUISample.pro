TEMPLATE = app
TARGET = IGVGainUISampleDlg
CONFIG += debug_and_release
    
HEADERS += \
	../IGVGainUISampleDlg.h \
	../pvmessagebox.h \

SOURCES +=   \
        ../displaythread.cpp \
        ../main.cpp \
        ../IGVGainUISampleDlg.cpp

RESOURCES += ../IGVGainUISampleDlg.qrc

OTHER_FILES +=
	
LIBS += -L$(PUREGEV_ROOT)/lib/ -lPvBuffer -lPvBase -lEbTransportLayerLib -lPvDevice -lPvGUI -lPvPersistence -lPvGenICam -lPvStream -lPvVirtualDevice -lPvSerial -lPvTransmitter -lPvAppUtils

INCLUDEPATH += $(PUREGEV_ROOT)/include
   
CONFIG(debug, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT PV_NO_GEV1X_PIXEL_TYPES PV_NO_DEPRECATED_PIXEL_TYPES _UNIX_ _LINUX_
