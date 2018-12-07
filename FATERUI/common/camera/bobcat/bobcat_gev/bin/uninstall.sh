#!/bin/sh
# ***************************************************************************************
#     Pleora Technologies Inc. Copyright (c) 2002-2015
# ***************************************************************************************

# Default variables
USR_LIB=/usr/lib

START_DIR=`dirname $0`
START_DIR=`cd $START_DIR/..; pwd`

INSTALL_ROOT=$START_DIR

ARCH=`uname -m | sed -e 's/i.86/i686/' -e 's/^armv.*/arm/'`

DisplayHelp()
{
    echo ""
    echo "NAME"
    echo "    uninstall.sh - Uninstalls the eBUS SDK."
    echo ""
    echo "SYNOPSIS"
    echo "    bash uninstall.sh [ --help ]"
    echo ""
    echo "DESCRIPTION"
    echo "   Uninstalls the eBUS SDK from the PC."
    echo "   This script can only used by the root or sudoer account."
    echo "    --help             Displays this help."
    echo ""
    echo "COPYRIGHT"
    echo "    Pleora Technologies Inc. Copyright (c) 2002-2015"
    echo ""
    echo "VERSION"
    echo "    4.1.4.3606"
    echo ""
}

# Parse the input arguments
for i in $*
do
    case $i in        
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

# Create the set_puregev_vars script
if [ "x86_64" = "x86_64" ]; then
  GENICAM_LIB_SUBDIR=Linux64_x64
elif [ "x86_64" = "i686" ]; then
  GENICAM_LIB_SUBDIR=Linux32_i86
elif [ "x86_64" = "arm" ]; then 
  GENICAM_LIB_SUBDIR=Linux32_ARM
elif [ "x86_64" = "ppc" ]; then 
  GENICAM_LIB_SUBDIR=Linux32_PPC  
fi

# Initial screen
clear
echo "eBUS_SDK 4.1.4.3606 for linux"
echo "  ( bobcat_gev x86_64 )"
echo "========================================"
echo ""

# Ensure permission...
if [ ! -w $INSTALL_ROOT ]; then
  echo "You do not have write access to uninstall '$INSTALL_ROOT'."
  echo "Run the script as superuser (root account)."
  exit 1
fi

# Ensure the client want to uninstall the SDK
ANSWER="not initialized"
until [ "$ANSWER" = "yes" -o "$ANSWER" = "no" -o "$ANSWER" = "" ]; do
echo -n "Remove eBUS_SDK installed in the '$INSTALL_ROOT' directory [yes | no]?"
read ANSWER
done
if [ "$ANSWER" = "" ]; then
    ANSWER="yes"
fi
if [ ! "$ANSWER" = "yes" ]; then
  echo "Uninstall canceled."
  exit;
fi

if [ "$ARCH" = "x86_64" ]; then

  # We only remove the driver if this one is in memory
  if [ -f "$INSTALL_ROOT/module/unload.sh" ]; then
    bash "$INSTALL_ROOT/module/unload.sh"
  fi

  # We remove the symbolic link
  if [ -f "./install_libraries.sh" ]; then
    ./install_libraries.sh --uninstall
  fi

  # We only remove the service configuration i
  if [ -f "./install_daemon.sh" ]; then
    ./install_daemon.sh --uninstall
  fi
fi

echo "Removing software on $INSTALL_ROOT."
if [ -d $INSTALL_ROOT ]; then
  EXCEPTION_FOLDER=""
  if [ -d "$INSTALL_ROOT/licenses" ]; then
    if [ -n "$( ls $INSTALL_ROOT/licenses/*.lic 2> /dev/null )" ]; then
        EXCEPTION_FOLDER="licenses"
    fi
  fi
  if [ -z "$EXCEPTION_FOLDER" ]; then
    rm -rf $INSTALL_ROOT
    rmdir --ignore-fail-on-non-empty $(readlink -m $INSTALL_ROOT/..) 
    rmdir --ignore-fail-on-non-empty $(readlink -m $INSTALL_ROOT/../..) 
    rmdir --ignore-fail-on-non-empty $(readlink -m $INSTALL_ROOT/../../..) 
  else
    ls -d $INSTALL_ROOT/* -1 | grep -v -E $EXCEPTION_FOLDER | xargs rm -rf
  fi
fi

echo ""
echo "eBUS SDK has been uninstalled."
echo ""
echo ""
