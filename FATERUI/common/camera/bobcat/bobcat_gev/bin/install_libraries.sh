#!/bin/sh
# ***************************************************************************************
#     Pleora Technologies Inc. Copyright (c) 2002-2015
# ***************************************************************************************

DisplayHelp()
{
    echo ""
    echo "NAME"
    echo "    install_libraries.sh - Installs or removes the libraries from the path."
    echo ""
    echo "SYNOPSIS"
    echo "    install_libraries.sh [ --install ][ --uninstall ][ --help ]"
    echo ""
    echo "DESCRIPTION"
    echo "    This script is used to add the libraries to the ldconfig application."
    echo ""
    echo ""
    echo "COPYRIGHT"
    echo "    Pleora Technologies Inc. Copyright (c) 2002-2015"
    echo ""
    echo "VERSION"
    echo "    4.1.4.3606"
    echo ""
}

Install()
{
    echo ""
    if [ -f "/etc/ld.so.conf.d/eBUS_SDK.conf" ]; then
        echo "Removing /etc/ld.so.conf.d/eBUS_SDK.conf"
        rm -f /etc/ld.so.conf.d/eBUS_SDK.conf
    fi

    echo "Creating /etc/ld.so.conf.d/eBUS_SDK.conf"

    SCRIPT_DIR=`readlink -f $0`
    SDK_BIN_DIR=`dirname $SCRIPT_DIR`
    SDK_DIR=`cd $SDK_BIN_DIR/..; pwd`
    SDK_LIB_DIR="$SDK_DIR/lib"

    GENICAM_ENUM_LIBRARIES=`find $SDK_LIB_DIR/genicam/bin -type d`

> /etc/ld.so.conf.d/eBUS_SDK.conf
cat > /etc/ld.so.conf.d/eBUS_SDK.conf <<__END__
$SDK_LIB_DIR
$GENICAM_ENUM_LIBRARIES
__END__

    echo "Reloading library path"
    /sbin/ldconfig
        
    echo "" 
}

Uninstall()
{
    if [ -f "/etc/ld.so.conf.d/eBUS_SDK.conf" ]; then
        echo ""
        
        echo "Removing /etc/ld.so.conf.d/eBUS_SDK.conf"
        rm -f /etc/ld.so.conf.d/eBUS_SDK.conf
        
        echo "Reloading library path"
        /sbin/ldconfig

        echo "" 
    fi
}

ACTION=""

# Parse the input arguments
for i in $*
do
    case $i in        
        --install)
            ACTION="INSTALL"
            ;; 
        --uninstall)
            ACTION="UNINSTALL"
            ;; 
        --help)
            DisplayHelp
            exit 0
        ;;
        *)
        # unknown option
        DisplayHelp
        exit 1
        ;;
    esac
done

# Default variables
USER_ID=`id -u`

# Check required priviledge
if [ "$USER_ID" != "0" ]; then
  echo "You need to run this script as superuser (root account)."
  exit 1
fi

if [ -z "$ACTION" ]; then
    echo ""
    echo "The following operations can be performed by this script:"
    echo ""
    echo "0 - Add libraries to the path."
    echo "1 - Remove libraries from the path."
    echo ""
    ANSWER="not set"
    until [ "$ANSWER" = "0" -o "$ANSWER" = "1" ]; do
        echo -n "Enter your selection [0|1]. Default is 0. "
        read ANSWER
        if [ -z "$ANSWER" ]; then
            ANSWER="0"
        fi
    done

    # Convert the selection into usable variables
    if [ "$ANSWER" = "0" ]; then
        ACTION="INSTALL"
    elif [ "$ANSWER" = "1" ]; then
        ACTION="UNINSTALL"
    fi
fi

# Now we can install if this was the action to perform
if [ "$ACTION" = "INSTALL" ]; then
    Install
elif [ "$ACTION" = "UNINSTALL" ]; then
    Uninstall
fi

echo ""



