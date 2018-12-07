#!/bin/sh
# ***************************************************************************************
#     Pleora Technologies Inc. Copyright (c) 2002-2015
# ***************************************************************************************

DisplayHelp()
{
    echo ""
    echo "NAME"
    echo "    set_udev_rules.sh - Manages udev rules independently of the udev version."
    echo ""
    echo "SYNOPSIS"
    echo "    set_udev_rules.sh [ --add=<vendor-id> ][ --remove=<vendor-id> ][ --removeall ][ --help ]"
    echo ""
    echo "DESCRIPTION"
    echo "    This script is used to create and remove udev rules from the system."
    echo "    To allow access to USB3 Vision devices, you must add a rule based on"
    echo "    the device's vendor ID."
    echo ""
    echo "    Information about the device can be found in several ways:"
    echo ""
    echo "    Method 1" 
    echo "    The vendor ID is typically labeled on your device. It is the "
    echo "     first four characters of the USB3 Vision GUID."
    echo ""
    echo "    Method 2"     
    echo "    When using lsusb -v, the output contains multiple devices and the "
    echo "     device should be listed as: "    
    echo ""
    echo "        ..."
    echo "        Bus 010 Device 002: ID 28b7:0001"
    echo "        Device Descriptor:"
    echo "        bLength                18"
    echo "        bDescriptorType         1"
    echo "        bcdUSB               3.00"
    echo "        bDeviceClass          239 Miscellaneous Device"
    echo "        bDeviceSubClass         2 ?"
    echo "        bDeviceProtocol         1 Interface Association"
    echo "        bMaxPacketSize0         9"
    echo "        idVendor           0x28b7         <- Vendor id is here!"
    echo "        idProduct          0x0001"
    echo "        bcdDevice            0.00"
    echo "        iManufacturer           1 Pleora Technologies Inc."
    echo "        iProduct                2 iPORT-NTx-U3-PT03-PB0UP01-128xU"
    echo "        iSerial                 3 12345678"
    echo "        bNumConfigurations      1"
    echo "        ..."
    echo ""
    echo "    Method 3"     
    echo "    When using USB3 Vision devices, the output contains multiple devices and the "
    echo "     device should be listed as: "   
    echo ""
    echo "        ..."
    echo "        T:  Bus=10 Lev=01 Prnt=01 Port=00 Cnt=01 Dev#=  2 Spd=5000 MxCh= 0"
    echo "        D:  Ver= 3.00 Cls=ef(misc ) Sub=02 Prot=01 MxPS= 9 #Cfgs=  1"
    echo "        P:  Vendor=28b7 ProdID=0001 Rev=00.00    <- Vendor id is here!"
    echo "        S:  Manufacturer=Pleora Technologies Inc."
    echo "        S:  Product=iPORT-NTx-U3-PT03-PB0UP01-128xU"
    echo "        S:  SerialNumber=12345678"
    echo "        C:  #Ifs= 3 Cfg#= 1 Atr=80 MxPwr=896mA"
    echo "        I:  If#= 0 Alt= 0 #EPs= 2 Cls=ef(misc ) Sub=05 Prot=00 Driver=(none)"
    echo "        I:  If#= 1 Alt= 0 #EPs= 1 Cls=ef(misc ) Sub=05 Prot=01 Driver=(none)"
    echo "        I:  If#= 2 Alt= 0 #EPs= 1 Cls=ef(misc ) Sub=05 Prot=02 Driver=(none)"
    echo "        ..."
    echo ""
    echo "    This script can only be used by the root or sudoer account."
    echo "    --add=<vendor-id>     Creates a rule based on the device's vendor ID."
    echo "    --remove=<vendor-id>  Removes the rule associated with the specified vendor ID."
    echo "    --removeall           Removes all the 3rd vendor ID rules from the system."
    echo "    --help                Displays this help."
    echo ""
    echo "COPYRIGHT"
    echo "    Pleora Technologies Inc. Copyright (c) 2002-2015"
    echo ""
    echo "VERSION"
    echo "    4.1.4.3606"
    echo ""
}

IsValidVendorID()
{
    if echo "$1" | grep -qE ^[0-9A-Fa-f]{4}$; then
        return 1
    else
        return 0
    fi
}

MakeRuleName()
{
    local FILE

    FILE=`echo $1 | awk '{print tolower($0)}'`

    eval "$2='/etc/udev/rules.d/80-drivers-eBUSSDK-$FILE.rules'"
}

CreateRuleFile()
{
    # Depending on the version of udev, we need to generate different type of files for the rules @#$@#$@
    VERSION_UDEV=`udevadm --version`
    if [ $VERSION_UDEV -gt 147 ]; then

> $FILENAME
cat > $1 <<__END__
ACTION=="add", SUBSYSTEM=="usb", ENV{DEVTYPE}=="usb_device", ATTRS{idVendor}=="$2", MODE="0666", GROUP="plugdev"
__END__

    else

> $FILENAME
cat > $1 <<__END__
SUBSYSTEM=="usb", SYSFS{idVendor}=="$2", MODE="0666"
__END__

    fi
}

Add()
{
    local FILENAME

    if [ ! -d "/etc/udev/rules.d" ]; then
        echo "Unable to find the folder that contains the rules (/etc/udev/rules.d)."
        exit 3
    fi

    if ! which udevadm >/dev/null 2>&1; then
        echo "Unable to find the udevadm tool to retrieve the current version of udev that is"
        echo "installed on this distribution." 
        echo "You must add the rule manually."
        exit 3
    fi
 
    MakeRuleName "$1" FILENAME

    echo "Adding rules for vendor ID $1."

    # Delete any existing rules with this name first
    rm -f $FILENAME

    # Create the new rule file
    CreateRuleFile "$FILENAME" "$1"

    # Set the permission to the files
    chmod u=rw,g=r,o=r $FILENAME

    # Reload the rules for the usb subsystem
    udevadm trigger --action=add --subsystem-match=usb --attr-match idVendor="$1"
    echo "The $FILENAME rule has been created."
}

Remove()
{
    local FILENAME

    if [ -d "/etc/udev/rules.d" ]; then

        MakeRuleName "$1" FILENAME
        echo "Removing the rules for vendor ID $1."
        rm -f $FILENAME

        if which udevadm >/dev/null 2>&1; then
            udevadm trigger --action=change --subsystem-match=usb --attr-match idVendor="$1"
        fi

        echo "The rules for vendor ID $1 have been removed."
    fi
}

RemoveAll()
{
    local FILENAME

    if [ -d "/etc/udev/rules.d" ]; then

        echo "Removing all rules."
        MakeRuleName "????" FILENAME
        rm -f $FILENAME

        # Recreate the pleora rule because it should be permanent
        MakeRuleName "28b7" FILENAME
        CreateRuleFile "$FILENAME" "28b7"

        if which udevadm >/dev/null 2>&1; then
            udevadm trigger --action=change --subsystem-match=usb 
        fi

        echo "All rules have been removed."
    fi
}

VENDORID=""

# Parse the input arguments
for i in $*
do
    case $i in        
        --add_pleora)
            VENDORID=28b7 
            ACTION="add"
            ;; 
        --add=0x*)
            VENDORID=0x${i#*=} 
            ACTION="add"
            ;; 
        --add=0X*)
            VENDORID=0X${i#*=} 
            ACTION="add"
            ;; 
        --add=*)
            VENDORID=${i#*=} 
            ACTION="add"
            ;; 
        --add)
            VENDORID=""
            ACTION="add"
            ;; 
        --remove_pleora)
            VENDORID=28b7 
            ACTION="remove"
            ;; 
        --remove=0x*)
            VENDORID=0x${i#*=} 
            ACTION="remove"
            ;; 
        --remove=0X*)
            VENDORID=0X${i#*=} 
            ACTION="remove"
            ;; 
        --remove=*)
            VENDORID=${i#*=} 
            ACTION="remove"
            ;;  
        --remove)
            VENDORID=""
            ACTION="remove"
            ;;
        --removeall)
            VENDORID="" 
            ACTION="removeall"
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
  echo "Can only run this script as superuser (root account)."
  exit 1
fi

if [ -z "$ACTION" ]; then
    echo ""
    echo "The following operations can be performed by this script:"
    echo ""
    echo "0 - Adds a rule for a device."
    echo "1 - Removes a rule for a device."
    echo "2 - Removes all rules from the system."
    echo ""
    ANSWER="not set"
    until [ "$ANSWER" = "0" -o "$ANSWER" = "1" -o "$ANSWER" = "2" ]; do
        echo -n "Enter your selection [0|1|2]. Default is 0. "
        read ANSWER
        if [ -z "$ANSWER" ]; then
            ANSWER="0"
        fi
    done

    # Convert the selection into usable variables
    if [ "$ANSWER" = "0" ]; then
        ACTION="add"
    elif [ "$ANSWER" = "1" ]; then
        ACTION="remove"
    elif [ "$ANSWER" = "2" ]; then
        ACTION="removeall"
    fi
fi

# Ask for the vendor-id if required
if [ "$ACTION" != "removeall" ]; then
    IsValidVendorID "$VENDORID"
    if [ "$?" = "0" ] ; then
        ANSWER="not set"
        until [ "$ANSWER" != "not set" ]; do
            echo -n "Enter a valid vendor ID containing four hexadecimal digits: "
            read ANSWER
            if [ -z "$ANSWER" ]; then
                ANSWER="not set"
            else
                IsValidVendorID "$ANSWER"
                if [ "$?" = "0" ] ; then
                    ANSWER="not set"
                fi
            fi
        done
        VENDORID=$ANSWER
    fi
fi

# Now finally, we can process the information...
echo ""
if [ "$ACTION" = "add" ]; then
    Add $VENDORID
elif [ "$ACTION" = "remove" ]; then
    Remove $VENDORID
elif [ "$ACTION" = "removeall" ]; then
    RemoveAll
fi
echo ""



