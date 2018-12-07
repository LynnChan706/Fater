
// *****************************************************************************
//
//     Copyright (c) 2009, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __OS_LOCK_TAG_H__
#define __OS_LOCK_TAG_H__

#include "OS_Types.h"

typedef OS_UINT32 OS_TAG;

#define OS_TAG_NOTAG                      0

#define OS_TAG_LOCKINCREASEINT32          1
#define OS_TAG_LOCKDECREASEINT32          2
#define OS_TAG_RWLOCKREADACQUIRE          3
#define OS_TAG_RWLOCKREADRELEASE          4
#define OS_TAG_SHAREMEMORYINIT            5
#define OS_TAG_SHAREMEMORYFREE            6

#define OS_TAG_GDRSETABORTEDSTATE             1000  
#define OS_TAG_GDRPUSHREQUEST                 1001
#define OS_TAG_GDRWAITREQUESTS1               1002
#define OS_TAG_GDRWAITREQUESTS2               1003
#define OS_TAG_GDRWAITREQUESTS3               1004
#define OS_TAG_GDRWAITDONE                    1005
#define OS_TAG_GDRRESET1                      1006
#define OS_TAG_GDRRESET2                      1007
#define OS_TAG_GDRSETCONFIG                   1008
#define OS_TAG_GDRGETCONFIG                   1009
#define OS_TAG_GDRSETTIMEOUTS                 1010
#define OS_TAG_GDRGETTIMEOUTS                 1011
#define OS_TAG_GDRPUSHSINGLE1                 1012
#define OS_TAG_GDRPUSHSINGLE2                 1013
#define OS_TAG_GDRPUSHSINGLE3                 1014
#define OS_TAG_GDRPUSHCONTINUOUS1             1015
#define OS_TAG_GDRPUSHCONTINUOUS2             1016
#define OS_TAG_GDRPUSHCONTINUOUS3             1017
#define OS_TAG_GDRPUSHCONTINUOUS4             1018
#define OS_TAG_GDRPUSHCONTINUOUS5             1019
#define OS_TAG_GDRPUSHCONTINUOUS6             1020
#define OS_TAG_GDRPUSHCONTINUOUS7             1021
#define OS_TAG_GDRPUSHCONTINUOUS8             1022
#define OS_TAG_GDRREQUESTINIT                 1023
#define OS_TAG_GDRINTERNALABORT               1024
#define OS_TAG_GDRINCREASEPACKETSIGNORED      1025
#define OS_TAG_GDRPROCESSNONINITIALIAZED1     1026
#define OS_TAG_GDRPROCESSNONINITIALIAZED2     1027
#define OS_TAG_GDRAUTORESETRING               1028
#define OS_TAG_GDRINTERNALRESET               1029
#define OS_TAG_GDRMAKEMAINTENANCENOLOCK1      1030
#define OS_TAG_GDRMAKEMAINTENANCENOLOCK2      1031
#define OS_TAG_GDRMAKEMAINTENANCENOLOCK3      1032
#define OS_TAG_GDRMAKEMAINTENANCENOLOCK4      1033
#define OS_TAG_GDRMAKEMAINTENANCENOLOCK5      1034
#define OS_TAG_GDRMAKEMAINTENANCE             1035
#define OS_TAG_GDROPENRESOURCE                1036
#define OS_TAG_GDRCLOSERESOURCE               1037
#define OS_TAG_GDRGETRESENDGROUP              1038
#define OS_TAG_GDRRELEASERESENDGROUP          1039
#define OS_TAG_GDRCHANGESIZEOFRESENDGROUP     1040
#define OS_TAG_GDRFLUSH1                      1041
#define OS_TAG_GDRFLUSH2                      1042
#define OS_TAG_GDRREQUESTCOPYQUEUEDPACKET1    1043
#define OS_TAG_GDRREQUESTCOPYQUEUEDPACKET2    1044
#define OS_TAG_GDRREQUESTCOPYQUEUEDPACKET3    1045
#define OS_TAG_GDRADDPACKETTOQUEUE1           1046
#define OS_TAG_GDRADDPACKETTOQUEUE2           1047
#define OS_TAG_GDRABORT1                      1048
#define OS_TAG_GDRABORT2                      1049
#define OS_TAG_GDRSTOREBACKUPPACKET           1050
#define OS_TAG_GDRCANCEL1                     1051
#define OS_TAG_GDRCANCEL2                     1052
#define OS_TAG_GDRWAITREQUEST                 1053

#define OS_TAG_NETRXPACKETMANAGERHOLDPACKET     2000
#define OS_TAG_NETRXPACKETMANAGERCREATEPACKET1  2001
#define OS_TAG_NETRXPACKETMANAGERCREATEPACKET2  2002
#define OS_TAG_NETRXPACKETMANAGERDESTROYPACKET1 2003
#define OS_TAG_NETRXPACKETMANAGERDESTROYPACKET2 2004

#define OS_TAG_WTF6IOCONTROLINITPLUGIN1                3000
#define OS_TAG_WTF6IOCONTROLINITPLUGIN2                3001
#define OS_TAG_WTF6FILTERATTACH                        3003
#define OS_TAG_WFT6FILTERPAUSE1                        3004
#define OS_TAG_WFT6FILTERPAUSE2                        3005
#define OS_TAG_WFT6FILTERPAUSE3                        3006
#define OS_TAG_WFT6FILTERPAUSE4                        3007
#define OS_TAG_WTF6FILTERDETACH1                       3008
#define OS_TAG_WTF6FILTERDETACH2                       3009
#define OS_TAG_WFT6FILTERSENDNETBUFFERLISTSCOMPLETE    3010
#define OS_TAG_WFT6SEND1                               3014
#define OS_TAG_WFT6SEND2                               3015
#define OS_TAG_WFT6SEND3                               3016
#define OS_TAG_WFT6SEND4                               3017
#define OS_TAG_WFT6SEND5                               3018
#define OS_TAG_WTF6FILTERRESTART1                      3019
#define OS_TAG_WTF6FILTERRESTART2                      3020
#define OS_TAG_WFT6FILTERCANCELOIDREQUEST              3021
#define OS_TAG_WFT6FILTEROIDREQUESTCOMPLETE            3022
#define OS_TAG_WFT6FILTERREFERENCE                     3023
#define OS_TAG_WFT6FILTERDEREFERENCE                   3024
#define OS_TAG_WFT6FILTERRECEIVENETBUFFEFLISTS         3025
#define OS_TAG_WFT6RUNOFFLOADDPC1                      3028
#define OS_TAG_WFT6RUNOFFLOADDPC2                      3029
#define OS_TAG_WFT6WAITFORALLDPC1                      3030
#define OS_TAG_WFT6WAITFORALLDPC2                      3031
#define OS_TAG_WFT6RUNCPUUSAGEDPC                      3032
#define OS_TAG_WFT6CPUUSAGEREAD                        3033
#define OS_TAG_FREEFILTER                              3034
#define OS_TAG_WTF6IOCONTROLINITPCAPFILTER1            3035
#define OS_TAG_WTF6IOCONTROLINITPCAPFILTER2            3036
#define OS_TAG_WFT6FILTERRECEIVENETBUFFERLISTS         3037
#define OS_TAG_WFT6FILTERSENDNETBUFFERLISTS            3038
#define OS_TAG_WFT6FREEFILTER                          3039
#define OS_TAG_WFT6CLOSEPLUGIN1                        3040
#define OS_TAG_WFT6CLOSEPLUGIN2                        3041
#define OS_TAG_WFT6CLOSEPLUGIN3                        3042
#define OS_TAG_WFT6DISPATCHIOCONTROLCANCEL1            3043
#define OS_TAG_WFT6DISPATCHIOCONTROLCANCEL2            3044
#define OS_TAG_WFT6DISPATCHIOCONTROLCANCEL3            3045
#define OS_TAG_WFT6DISPATCHIOCONTROLCANCEL4            3046
#define OS_TAG_WFT6DISPATCHIOCONTROL1                  3047
#define OS_TAG_WFT6DISPATCHIOCONTROL2                  3048
#define OS_TAG_WFT6DISPATCHIOCONTROL3                  3049
#define OS_TAG_WFT6DISPATCHIOCONTROL4                  3050
#define OS_TAG_WFT6DISPATCHIOCONTROL5                  3051
#define OS_TAG_WTF6IOCONTROLISENABLED				   3052

#define OS_TAG_WFT5REGISTERDEVICE1                                  4000
#define OS_TAG_WFT5REGISTERDEVICE2                                  4001
#define OS_TAG_WFT5REGISTERDEVICE3                                  4002
#define OS_TAG_WFT5DEREGISTERDEVICE1                                4003
#define OS_TAG_WFT5DEREGISTERDEVICE2                                4004
#define OS_TAG_WFT5IOCONTROLINITPLUGIN1                             4005
#define OS_TAG_WFT5IOCONTROLINITPLUGIN2                             4006
#define OS_TAG_WFT5UNBINDADAPTER                                    4007
#define OS_TAG_WFT5PNPNETEVENTSETPOWER1                             4008
#define OS_TAG_WFT5PNPNETEVENTSETPOWER2                             4009
#define OS_TAG_WFT5QUEUERECEIVEDPACKET                              4010
#define OS_TAG_WFT5FLUSHRECEIVEQUEUE                                4011
#define OS_TAG_WFT5QUERYINFORMATION                                 4012
#define OS_TAG_WFT5FILTERREFERENCE                                  4013
#define OS_TAG_WFT5FILTERDEREFERENCE                                4014
#define OS_TAG_WFT5MPSEND1                                          4015
#define OS_TAG_WFT5MPSEND2                                          4016
#define OS_TAG_WFT5MPSEND3                                          4017
#define OS_TAG_WFT5MPSEND4                                          4018
#define OS_TAG_WFT5MPSEND5                                          4019
#define OS_TAG_WFT5SEND1                                            4020
#define OS_TAG_WFT5SEND2                                            4021
#define OS_TAG_WFT5SENDCOMPLETE                                     4022
#define OS_TAG_WFT5MPINITIALIZE                                     4023
#define OS_TAG_WFT5SENDPACKETS1                                     4026
#define OS_TAG_WFT5SENDPACKETS2                                     4027
#define OS_TAG_WFT5SENDPACKETS3                                     4028
#define OS_TAG_WFT5SENDPACKETS4                                     4029
#define OS_TAG_WFT5SENDPACKETS5                                     4030
#define OS_TAG_WFT5SENDPACKETS6                                     4031
#define OS_TAG_WFT5MPQUERYINFORMATION1                              4032
#define OS_TAG_WFT5MPQUERYINFORMATION2                              4033
#define OS_TAG_WFT5MPSETINFORMATION1                                4034
#define OS_TAG_WFT5MPSETINFORMATION2                                4035
#define OS_TAG_WFT5MPHALT                                           4036
#define OS_TAG_WFT5RECEIVE                                          4037
#define OS_TAG_WFT5DISPATCHIOCONTROLCANCEL1                         4038
#define OS_TAG_WFT5DISPATCHIOCONTROLCANCEL2                         4039
#define OS_TAG_WFT5DISPATCHIOCONTROLCANCEL3                         4040
#define OS_TAG_WFT5DISPATCHIOCONTROLCANCEL4                         4041
#define OS_TAG_WFT5DISPATCHIOCONTROL1                               4042
#define OS_TAG_WFT5DISPATCHIOCONTROL2                               4043
#define OS_TAG_WFT5DISPATCHIOCONTROL3                               4044
#define OS_TAG_WFT5DISPATCHIOCONTROL4                               4045
#define OS_TAG_WFT5DISPATCHIOCONTROL5                               4046
#define OS_TAG_WFT5CLOSEPLUGIN1                                     4053
#define OS_TAG_WFT5CLOSEPLUGIN2                                     4054
#define OS_TAG_WFT5CLOSEPLUGIN3                                     4055
#define OS_TAG_WTF5IOCONTROLISENABLED				                4056

#define OS_TAG_LOGWRITE                    5000
#define OS_TAG_LOGREAD                     5001

#define OS_TAG_PIGCLOSE1                   6000
#define OS_TAG_PIGCLOSE2                   6001
#define OS_TAG_PIGCANCEL1                  6002
#define OS_TAG_PIGCANCEL2                  6003
#define OS_TAG_PIGUNMAPONERROR             6004
#define OS_TAG_PIGIOCONTROLPUSHREQUESTS1   6005
#define OS_TAG_PIGIOCONTROLPUSHREQUESTS2   6006
#define OS_TAG_PIGIOCONTROLPUSHREQUESTS3   6007
#define OS_TAG_PIGIOCONTROLPUSHREQUESTS4   6008
#define OS_TAG_PIGIOCONTROLWAITREQUESTS    6009
#define OS_TAG_PIGRESEND1                  6010
#define OS_TAG_PIGRESEND2                  6011
#define OS_TAG_PIGRESEND3                  6012
#define OS_TAG_PIGRESEND4                  6013
#define OS_TAG_PIGRESEND5                  6014
#define OS_TAG_PIGRESENDDONE               6015
#define OS_TAG_PIGPROCESSRESEND1           6016
#define OS_TAG_PIGPROCESSRESEND2           6017

#define OS_TAG_DISINITPLUGIN               7000
#define OS_TAG_DISDELETEPLUGIN             7001
#define OS_TAG_DISPCAPPROCESSPACKET        7002

#define OS_TAG_LFTFILTERCLOSE              8000
#define OS_TAG_LFTFILTERSEND1              8004
#define OS_TAG_LFTFILTERSEND2              8005
#define OS_TAG_LFTFILTERSENDTHREAD1        8007

#define OS_TAG_PCAPSTOP					   9000
#define OS_TAG_PCAPSTART				   9001
#define OS_TAG_PCAPPUSHBUFFER              9002
#define OS_TAG_PCAPWAITNEXTBUFFER1         9003
#define OS_TAG_PCAPWAITNEXTBUFFER2         9004
#define OS_TAG_PCAPWAITNEXTBUFFER3         9005
#define OS_TAG_PCAPABORT                   9006
#define OS_TAG_PCAPCANCEL                  9007

#define OS_TAG_GTRPUSHBUFFERS1            10000
#define OS_TAG_GTRPUSHBUFFERS2            10001
#define OS_TAG_GTRPUSHBUFFERS3            10002
#define OS_TAG_GTRPUSHBUFFERS4            10004
#define OS_TAG_GTRWAITREQUESTS1           10005
#define OS_TAG_GTRWAITREQUESTS2           10006
#define OS_TAG_GTRWAITREQUESTS3           10007
#define OS_TAG_GTRWAITREQUESTS4           10008
#define OS_TAG_GTRWAITREQUESTS5           10009
#define OS_TAG_GTRWAITREQUESTS6           10010
#define OS_TAG_PREPAREWAITBUFFER          10011
#define OS_TAG_GTRWAITFORPENDINGPACKETS   10012
#define OS_TAG_GTRABORT                   10013
#define OS_TAG_GTRGETNEXTBUFFER           10014
#define OS_TAG_GTRRUNWORK1                10015
#define OS_TAG_GTRRUNWORK2                10016
#define OS_TAG_GTRWAITFORBANDWIDTH        10017
#define OS_TAG_GTRNEXTBUFFER              10018
#define OS_TAG_GTRSETBUFFERDONE           10019
#define OS_TAG_GTRPROCESSABORT            10020
#define OS_TAG_GTRSENDDONE1               10021
#define OS_TAG_GTRSENDDONE2               10022
#define OS_TAG_GTRABORTOLDERPUSHEDBUFFER  10021
#define OS_TAG_GTRPROCESSCANCELLING1      10022
#define OS_TAG_GTRPROCESSCANCELLING2      10023
#define OS_TAG_GTRWAITFORQUEUEDBUFFER     10024

#define OS_TAG_PITCLOSE1                  11000
#define OS_TAG_PITCLOSE2                  11001
#define OS_TAG_PITCANCEL1                 11002
#define OS_TAG_PITCANCEL2                 11003
#define OS_TAG_PITUNMAPONERROR            11004
#define OS_TAG_PITIOCONTROLPUSHREQUESTS1  11005
#define OS_TAG_PITIOCONTROLPUSHREQUESTS2  11006
#define OS_TAG_PITIOCONTROLPUSHREQUESTS3  11007
#define OS_TAG_PITIOCONTROLPUSHREQUESTS4  11008
#define OS_TAG_PITIOCONTROLWAITBUFFERS    11009
#define OS_TAG_PITRESEND1                 11010
#define OS_TAG_PITRESEND2                 11011
#define OS_TAG_PITRESEND3                 11012
#define OS_TAG_PITRESENDDONE              11013
#define OS_TAG_PITFREEBUFFERINFO          11014
#define OS_TAG_PITQUEUESEND1              11015
#define OS_TAG_PITQUEUESEND2              11016

#endif // __OS_TAG_TAG_H__
