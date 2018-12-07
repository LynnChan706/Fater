INCLUDE ( ../../pv1s01s1/eBUSPlayerShared/eBUSPlayerShared.cmake ) 

PV_ENABLE_QT( FALSE )

set (BASELIB_DIR $ENV{BASELIB_DIR})
set (PUREGEV_DIR $ENV{PUREGEV_DIR})

IF(WIN32)
    string(REPLACE \\ / BASELIB_DIR ${BASELIB_DIR} )
    set(TGT_EBUSPLAYER eBUSPlayer)
ENDIF(WIN32)
IF(UNIX)
    set(TGT_EBUSPLAYER eBUSPlayer.bin)
ENDIF(UNIX)

include ( ${BASELIB_DIR}/Software/CMakeShared/third-party.Definitions.cmake )
add_definitions( -DPV_NO_GEV1X_PIXEL_TYPES -DPV_NO_DEPRECATED_PIXEL_TYPES )

set ( MOC_CLASSES
	ebusplayer.h
	progressdlg.h
	setupdlg.h
	eventmonitordlg.h
	imagesavedlg.h
	filteringdlg.h
	aboutbox.h
	displayoptionsdlg.h
	bufferoptionsdlg.h
	splashscreen.h
	errorsdlg.h
	warningdlg.h
	actioncommanddlg.h
	filetransferdlg.h
	histogramwidget.h
)

IF(QT_WRAP_CPP)
  QT_WRAP_CPP(${TGT_EBUSPLAYER} MOC_SOURCES ${MOC_CLASSES})
ENDIF(QT_WRAP_CPP)

SET(QtApp_RCCS ebusplayer.qrc)
QT4_ADD_RESOURCES(QtApp_RCC_SRCS ${QtApp_RCCS})

#------------------------------------------------------------------------------
#  Define source - add headers for convenience of editing code in the IDE
#-----------------------------------------------------------------------------
set(SOURCES ${SOURCES_MODEL}
	ebusplayer.cpp
	ebusplayerhandlers.cpp
	ebusplayerlayout.cpp
	ebusplayermenufile.cpp
	ebusplayermenuhelp.cpp
	ebusplayermenutools.cpp
	main.cpp
	progressdlg.cpp
	setupdlg.cpp
	eventmonitordlg.cpp
	imagesavedlg.cpp
	filteringdlg.cpp
	aboutbox.cpp
	displayoptionsdlg.cpp
	bufferoptionsdlg.cpp
	splashscreen.cpp
	errorsdlg.cpp
	warningdlg.cpp
	actioncommanddlg.cpp
	filetransferdlg.cpp
	histogramwidget.cpp
)

#------------------------------------------------------------------------------
# Build our Executable!
#------------------------------------------------------------------------------
set( SOURCES ${SOURCES} ${MOC_SOURCES} ${QtApp_RCC_SRCS} )
PV_ADD_FILES_TO_EXECUTABLE( ${TGT_EBUSPLAYER} " " "${SOURCES}" )
IF(WIN32)
	set_target_properties(${TGT_EBUSPLAYER} PROPERTIES VS_KEYWORD "Qt4VSv1.0")
	set_target_properties(${TGT_EBUSPLAYER} PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
	set_target_properties(${TGT_EBUSPLAYER} PROPERTIES OUTPUT_NAME "../${TGT_EBUSPLAYER}")
ENDIF(WIN32)

#------------------------------------------------------------------------------
# Add linker dependencies for this library
#------------------------------------------------------------------------------
IF(WIN32)
	set(TARGETS ${QT_LIBRARIES} optimized qtmain.lib debug qtmaind.lib )
	add_dependencies( ${TGT_EBUSPLAYER} PvDevice PvStream PvPersistence PvBuffer PvGenICam PvGUI PvSerial PvAppUtils )
ENDIF(WIN32)
IF(UNIX)
	set( TARGETS ${QT_LIBRARIES} PvDevice PvStream PvPersistence PvBuffer PvGenICam PvGUI PvSerial PvAppUtils )
    IF (PV_ENABLE_MP4)
        target_link_libraries( ${TGT_EBUSPLAYER} avformat avcodec avutil swscale)
    ENDIF(PV_ENABLE_MP4)
    link_directories ( ${LIBXML_LIB_DIR} )
ENDIF(UNIX)
PV_ADD_SHARED_LINKER_DEPENDENCIES( ${TGT_EBUSPLAYER} "${TARGETS}" )

#------------------------------------------------------------------------------
# Copy the appropriates files to the build folder
#------------------------------------------------------------------------------
PV_COPY_OUTPUT_EXE_TO_BUILD( ${TGT_EBUSPLAYER} shared )

IF(UNIX)
    execute_process( COMMAND 
        ${PUREGEV_DIR}/Software/Applications/pv1s01b1/eBUSPlayer/createlaunchscript.sh 
            ${PUREGEV_OUTPUT_BIN_DIR} 
            eBUSPlayer )
    execute_process( COMMAND 
        ${PUREGEV_DIR}/Software/Applications/pv1s01b1/eBUSPlayer/createlaunchscript.sh 
            ${PUREGEV_INSTALL_IMG_BIN_DIR}
            eBUSPlayer )
ENDIF()








