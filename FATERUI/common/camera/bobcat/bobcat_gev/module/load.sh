#!/bin/sh

################################################################################
#
# load.sh
# Pleora Technologies Inc. Copyright (c) 2002-2015
# for ebUniversalProForEthernet-x86_64.ko 5.1.4 build 3606
#
################################################################################

# Variables
HOST_VERSION=`uname -r`
HOST_ARCH=`uname -m | sed -e 's/i.86/i686/' -e 's/^armv.*/arm/'`

#Display the help for this script
DisplayHelp()
{
    echo ""
    echo "NAME"
    echo "    load.sh - Load the eBUS Universal Pro For Ethernet driver module "
    echo "              ebUniversalProForEthernet-x86_64.ko 5.1.4 build 3606"
    echo ""
    echo "SYNOPSIS"
    echo "    bash load.sh [--help]"
    echo ""
    echo "DESCRIPTION"
    echo "    Load the eBUS Universal Pro For Ethernet module and configure the system to"
    echo "    be ready to use"
    echo "    This script can only used by root or sudoer"
    echo "    --help             Display this help"
    echo ""
    echo "COPYRIGHT"
    echo "    Pleora Technologies Inc. Copyright (c) 2002-2015"
    echo ""
    echo "VERSION"
    echo "    5.1.4 build 3606"
    echo ""
}

#Print out the error and exit 
# $1 Error message
# $2 Exit code
ErrorOut()
{
	echo ""
	echo "Error: $1"
	echo ""
	exit $2
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

# Check required priviledge
if [ `whoami` != root ]; then
	ErrorOut "This script can only be run by root user or sudoer" 1
fi

# Do not re-load if not needed
EBUSUNIVERSALPRO_LOADED=`lsmod | grep -o ebUniversalProForEthernet`
if [ "$EBUSUNIVERSALPRO_LOADED" = "ebUniversalProForEthernet" ];then
	exit 0
fi

# Sanity check
if [ "x86_64" != "$HOST_ARCH" ]; then
    ErrorOut "*** The module ebUniversalProForEthernet-x86_64.ko can only be load with a kernel x86_64 ***" 1
fi

# Check if this module can be loaded on this kernel
if command -v modinfo > /dev/null 2>&1; then
    VERMAGIC=`modinfo -F vermagic ebUniversalProForEthernet-x86_64.ko`
    if [ "$HOST_VERSION" != "${VERMAGIC%% *}" ]; then 
        ErrorOut "*** The module ebUniversalProForEthernet-x86_64.ko was not compiled agains this kernel. Use the build.sh script to rebuild it for this kernel ***" 1
    fi
fi    

# Load the module
echo "Loading eBUS Universal Pro For Ethernet for x86_64 ..."
/sbin/insmod ./ebUniversalProForEthernet-x86_64.ko $* || exit 1

# Remode existing node if any
echo "Delete old device node..."
rm -f /dev/ebUniversalProForEthernet0

# Recreate the nodes
echo "Create device node..."
major=$(awk "\$2==\"ebUniversalProForEthernet\" {print \$1}" /proc/devices)
mknod /dev/ebUniversalProForEthernet c $major 0

# Change permission
echo "Change permission on device node..."
group="staff"
grep -q '^staff:' /etc/group || group="wheel"
chgrp $group /dev/ebUniversalProForEthernet
chmod 777 /dev/ebUniversalProForEthernet

# Modify the socket configuration
echo "Adjust network settings..."
/sbin/sysctl -w net.core.rmem_max=10485760 > /dev/null
/sbin/sysctl -w net.core.wmem_max=10485760 > /dev/null

