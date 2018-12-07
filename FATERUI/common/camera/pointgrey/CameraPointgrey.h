#ifndef CameraPointgrey_H
#define CameraPointgrey_H

#include <iostream>
#include "FlyCapture2.h"
#include <unistd.h>
#include <deque>
#include <chrono>

#define CAPTURE_RETRY_TIME 3

class CameraPointgrey
{
    public:
        unsigned int width;
        unsigned int height;

        CameraPointgrey( int _mode = 0, bool _single_mode = true, float _grabTimeout = 5000, bool _strobe_enable = true, float _trigger_delay = 0.018f, unsigned int _packetSize = 9000, unsigned int _interPacketDelay = 1000, int _intp_method = 7,bool _debug = false, bool _is_hardware_trigger=false);
        ~CameraPointgrey();

        bool open();
        unsigned char* capture_one_frame();
        bool is_connected();
        bool release_camera();
        bool reboot_camera();
        int get_frame_count();
        int get_error_frame_count();
        double get_frame_rate();

        bool set_wb(int red, int blue);
        bool set_wb_red(int red);
        bool set_wb_blue(int blue);
        bool set_shutter(float shutter);
        float get_shutter();
        int get_white_balance_red();
        int get_white_balance_blue();
        char* get_firmware_version();
        int get_camera_id();
        float get_camera_temperature();

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
        FlyCapture2::PixelFormat pixelFormat;
        FlyCapture2::PGRGuid guid;
        FlyCapture2::CameraBase *m_pCamera;
//        FlyCapture2::Camera m_Camera;
//        FlyCapture2::GigECamera m_GigECamra;
        FlyCapture2::CameraInfo camInfo;
        FlyCapture2::Image rawImage;
        FlyCapture2::Image convertedImage;
        FlyCapture2::InterfaceType ifType;

        bool camera_discovery();
        bool set_params();
        void print_camera_info(FlyCapture2::CameraInfo* pCamInfo);
};

#endif
