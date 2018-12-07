TEMPLATE = app
TARGET = IGVExposureUISampleDlg
CONFIG += debug_and_release
    
HEADERS += \
	../thread.h \
	../IGVExposureUISampleDlg.h \
	../pvmessagebox.h \
    ../displaythread.h

SOURCES +=   \
        ../displaythread.cpp \
        ../main.cpp \
        ../IGVExposureUISampleDlg.cpp

RESOURCES += ../IGVExposureUISampleDlg.qrc

OTHER_FILES +=
	
LIBS += -L$(PUREGEV_ROOT)/lib/ -lPvBuffer -lPvBase -lEbTransportLayerLib -lPvDevice -lPvGUI -lPvPersistence -lPvGenICam -lPvStream -lPvVirtualDevice -lPvSerial -lPvTransmitter -lPvAppUtils

INCLUDEPATH += $(PUREGEV_ROOT)/include
   
CONFIG(debug, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT PV_NO_GEV1X_PIXEL_TYPES PV_NO_DEPRECATED_PIXEL_TYPES _UNIX_ _LINUX_
