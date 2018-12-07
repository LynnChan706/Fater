
PWD = `pwd`
KERNEL_FOLDER := /lib/modules/$(shell uname -r)/build

EXTRA_CFLAGS += -D_UNIX_ -D_PT_KERNEL_ 

obj-m += ebUniversalProForEthernet-x86_64.o
ebUniversalProForEthernet-x86_64-y := 			\
	LFT_Module.o				\
	NET_KMLRXPacket.o			\
	NET_KMLUtil.o				\
	OS_KMLEvent.o				\
    OS_KMLGCCCompiler.o         \
	OS_KMLInit.o				\
	OS_KMLLock.o				\
	OS_KMLMemory.o				\
	OS_KMLMemoryMap.o			\
	OS_KMLNetwork.o				\
	OS_KMLString.o				\
	OS_KMLSystem.o				\
	OS_KMLRWLock.o				\
	OS_KMLTime.o				\
	OS_KMLTimer.o    			\
	OS_KMLWorkQueue.o			\
    libebTransportLayer-x86_64.a

all:
	@make -C $(KERNEL_FOLDER) M=$(PWD) modules

clean:
	@rm -rf *.o .??* ebUniversalProForEthernet-x86_64.mod.c Module.markers Module.symvers modules.order


