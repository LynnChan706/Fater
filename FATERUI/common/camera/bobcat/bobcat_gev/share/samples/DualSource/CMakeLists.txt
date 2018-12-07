PV_ENABLE_QT( FALSE )

set (BASELIB_DIR $ENV{BASELIB_DIR})
set (PUREGEV_DIR $ENV{PUREGEV_DIR})

IF(WIN32)
    string(REPLACE \\ / BASELIB_DIR ${BASELIB_DIR} )
    set(TGT_DUALSOURCESAMPLE DualSource)
ENDIF(WIN32)
IF(UNIX)
    set(TGT_DUALSOURCESAMPLE DualSource.bin)
ENDIF(UNIX)

set ( MOC_CLASSES
    dualsource.h
    sourcewidget.h
)

IF(QT_WRAP_CPP)
  QT_WRAP_CPP(${TGT_DUALSOURCESAMPLE} MOC_SOURCES ${MOC_CLASSES})
ENDIF(QT_WRAP_CPP)

SET(QtApp_RCCS dualsource.qrc)
QT4_ADD_RESOURCES(QtApp_RCC_SRCS ${QtApp_RCCS})

add_definitions( -DPV_NO_GEV1X_PIXEL_TYPES -DPV_NO_DEPRECATED_PIXEL_TYPES )

#------------------------------------------------------------------------------
#  Define source - add headers for convenience of editing code in the IDE
#-----------------------------------------------------------------------------
set(SOURCES
	dualsource.cpp
	sourcewidget.cpp
	source.cpp
	main.cpp
	displaythread.cpp
)

#------------------------------------------------------------------------------
# Build our Executable!
#------------------------------------------------------------------------------
set( SOURCES ${SOURCES} ${MOC_SOURCES} ${QtApp_RCC_SRCS} )
PV_ADD_FILES_TO_EXECUTABLE( ${TGT_DUALSOURCESAMPLE} " " "${SOURCES}" )
IF(WIN32)
	set_target_properties(${TGT_DUALSOURCESAMPLE} PROPERTIES VS_KEYWORD "Qt4VSv1.0")
	set_target_properties(${TGT_DUALSOURCESAMPLE} PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
	set_target_properties(${TGT_DUALSOURCESAMPLE} PROPERTIES OUTPUT_NAME "../${TGT_DUALSOURCESAMPLE}")
ENDIF(WIN32)

#------------------------------------------------------------------------------
# Add linker dependencies for this library
#------------------------------------------------------------------------------
IF(WIN32)
	set(TARGETS ${QT_LIBRARIES} optimized qtmain.lib debug qtmaind.lib )
	add_dependencies( ${TGT_DUALSOURCESAMPLE} PvDevice PvStream PvPersistence PvBuffer PvGenICam PvGUI PvSerial PvAppUtils )
ENDIF(WIN32)
IF(UNIX)
	set( TARGETS ${QT_LIBRARIES} PvBase PvDevice PvStream PvPersistence PvBuffer PvGenICam PvGUI PvSerial PvAppUtils )
ENDIF(UNIX)

PV_ADD_SHARED_LINKER_DEPENDENCIES( ${TGT_DUALSOURCESAMPLE} "${TARGETS}" )

#------------------------------------------------------------------------------
# Copy the appropriates files to the build folder
#------------------------------------------------------------------------------
PV_COPY_OUTPUT_EXE_TO_BUILD( ${TGT_DUALSOURCESAMPLE} shared )

IF(UNIX)
    execute_process( COMMAND 
        ${PUREGEV_DIR}/Software/Applications/pv1s01b1/eBUSPlayer/createlaunchscript.sh 
            ${PUREGEV_OUTPUT_BIN_DIR} 
            DualSource )
    execute_process( COMMAND 
        ${PUREGEV_DIR}/Software/Applications/pv1s01b1/eBUSPlayer/createlaunchscript.sh 
            ${PUREGEV_INSTALL_IMG_BIN_DIR}
            DualSource )
ENDIF()


