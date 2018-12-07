#ifndef CameraBobcat_H
#define CameraBobcat_H

#include <iostream>
#include <unistd.h>
#include <deque>
#include <chrono>

#include <PvDevice.h>
#include <PvDeviceGEV.h>
#include <PvPipeline.h>
#include <PvBuffer.h>
#include <PvStream.h>
#include <PvStreamGEV.h>
#include <PvStreamU3V.h>
#include <PvDeviceInfoGEV.h>
#include <PvDeviceInfoU3V.h>
#include <PvSystem.h>
#include <ImagingBuffer.h>
#include <ImagingFilterRGB.h>
#include <PvConfigurationWriter.h>
#include <PvConfigurationReader.h>
#include "PvBufferConverter.h"

#include <list>

#define CAPTURE_RETRY_TIME 1
#define BUFFER_COUNT ( 4 )

#define DEVICE_CONFIGURATION_TAG ( "DeviceConfiguration" )
#define STREAM_CONFIGURAITON_TAG ( "StreamConfiguration" )
#define STRING_INFORMATION_TAG ( "StringInformation" )

class CameraBobcat : protected PvDeviceEventSink
{
    public:
        int64_t width;
        int64_t height;

        CameraBobcat( int _mode = 0, bool _single_mode = true, float _grabTimeout = 5000, bool _strobe_enable = true, float _trigger_delay = 0.018f, unsigned int _packetSize = 9000, unsigned int _interPacketDelay = 1000, int _intp_method = 7,bool _debug = false, bool _is_hardware_trigger=false);
        ~CameraBobcat();

        bool open();
        unsigned char* capture_one_frame();
        bool is_connected();
        bool release_camera();
        bool reboot_camera();
        int get_frame_count();
        int get_error_frame_count();
        double get_frame_rate();

        bool set_wb(int red, int green, int blue);
        bool set_wb_red(int red);
        bool set_wb_blue(int blue);
        bool set_wb_green(int green);
        bool set_shutter(float shutter);
        float get_shutter();
        int get_white_balance_red();
        int get_white_balance_blue();
        int get_white_balance_green();
        const char* get_firmware_version();
        const char* get_camera_id();
        float get_camera_temperature();

        bool store_configuration(const char* file_path);
        bool restore_configuration(const char* file_path);

    private:
        std::deque<std::chrono::high_resolution_clock::time_point> m_frame_time;
        double m_frame_rate;
        bool single;
        float shutterTime;
        int wb_red;
        int wb_blue;
        float grabTimeout;
        float trigger_delay_time;

        bool single_mode;
        bool debug;
        bool hardware_trigger;
        bool strobe_enable;
        unsigned int packetSize; // Packet size, in bytes.
        unsigned int interPacketDelay; //Inter packet delay, in timestamp counter units.
        unsigned int gain;
        unsigned int frame_count;
        unsigned int error_frame_count;
        int mode;

        PvString mConnectionID;
        PvDevice* mDevice;
        PvStream* mStream;
        PvPipeline* mPipeline;
        bool mConnectionLost;
        PvBuffer* mPvBuffers;
        uint32_t mBufferCount;
        SimpleImagingLib::ImagingBuffer* mImagingBuffers;

        PvGenInteger *mBlockCount;
        PvGenFloat *mFrameRate;
        PvGenFloat *mBandwidth;

        bool camera_discovery();
        bool set_params();
        bool GetFrameDimensions( PvGenParameterArray * aDeviceParams, PvDevice * aDevice );
};

#endif
