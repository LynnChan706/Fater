#!/bin/sh
# ***************************************************************************************
#
#     Copyright (c) 2009, Pleora Technologies Inc., All rights reserved.
#
# ***************************************************************************************

OS_NAME=`uname --`

if [ "$1" = "" ]; then
  SIZE=10485760
else
  SIZE=$1
fi

if [ "$OS_NAME" = "SunOS" ]; then
    USER=`/usr/xpg4/bin/id -u -n`
fi

echo "Setting socket maximum buffer size to $SIZE"

if [ "$OS_NAME" = "Linux" ]; then
  USER_ID=`id -u`
  PROC_ROOT=/proc/sys/net/core

  # Check required priviledge
  if [ "$USER_ID" != "0" ]; then
 	echo "This script can only be run by root user or sudoer"
    exit 1
  fi

  if [ ! -w $PROC_ROOT/wmem_max ]; then
    echo "Cannot write to $PROC_ROOT/wmem_max"
    exit 1
  fi

  if [ ! -w $PROC_ROOT/rmem_max ]; then
    echo "Cannot write to $PROC_ROOT/rmem_max"
    exit 1
  fi

  echo $SIZE > $PROC_ROOT/wmem_max
  echo $SIZE > $PROC_ROOT/rmem_max

  exit 0
fi

if [ "$OS_NAME" = "SunOS" ]; then
  NDD_BIN=/usr/sbin/ndd
  if [ ! -x $NDD_BIN ]; then
    echo "You don't have sufficient privileges to run this script. Please run as root"
    exit 1
  fi

  $NDD_BIN -set /dev/udp udp_max_buf $SIZE
  exit 0
fi

echo "Error: unknown platform: $OS_NAME"
exit 1

