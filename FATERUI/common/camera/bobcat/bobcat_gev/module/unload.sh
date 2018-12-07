#!/bin/sh

################################################################################
#
# unload.sh
# Pleora Technologies Inc. Copyright (c) 2002-2015
# for ebUniversalProForEthernet-x86_64.ko 5.1.4 build 3606
#
################################################################################

# Display the help for this script
DisplayHelp()
{
    echo ""
    echo "NAME"
    echo "    unload.sh - Unload the eBUS Universal Pro For Ethernet driver module "
    echo "                ebUniversalProForEthernet-x86_64.ko 5.1.4 build 3606 from the system"
    echo ""
    echo "SYNOPSIS"
    echo "    bash unload.sh [--help]"
    echo ""
    echo "DESCRIPTION"
    echo "    Unload the eBUS Universal Pro For Ethernet module and remove the configure"
	echo "    from the system to be ready to use"
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

# Print out the error and exit 
#  $1 Error message
#  $2 Exit code
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

# Ensure the module is in memory
EBUSUNIVERSALPRO_LOADED=`lsmod | grep -o ebUniversalProForEthernet`
if [ "$EBUSUNIVERSALPRO_LOADED" != "ebUniversalProForEthernet" ];then
	exit 0
fi

# Unload the module
echo "Unloading eBUS Universal Pro For Ethernet..."
/sbin/rmmod ./ebUniversalProForEthernet-x86_64.ko $* || exit 1

# Remove existing node if any
echo "Delete device node..."
rm -f /dev/ebUniversalProForEthernet /dev/ebUniversalProForEthernet0
