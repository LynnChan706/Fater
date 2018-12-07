#ifndef __CAMERAMINDVISION_H
#define __CAMERAMINDVISION_H

#include <iostream>
#include <unistd.h>
#include <deque>
#include <chrono>
#include "CameraApi.h"
#include "CameraDefine.h"
#include "CameraStatus.h"

#define CAPTURE_RETRY_TIME 3

class CameraMindVision
{
    public:
        CameraMindVision( int _mode = 0,
                        bool _single_mode = true, 
                        float _grabTimeout = 5000,
                        bool _strobe_enable = true,
                        float _trigger_delay = 0.018f,
                        unsigned int _packetSize = 9000,
                        unsigned int _interPacketDelay = 1000,
                        int _intp_method = 7,
                        bool _debug = false,
                        bool _is_hardware_trigger=false);
        ~CameraMindVision();

        int init_SDK();
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
        bool set_wb_green(int green);
        bool set_wb_blue(int blue);
        bool set_shutter(float shutter);
        float get_shutter();
        int get_white_balance_red();
        int get_white_balance_green();
        int get_white_balance_blue();
        char* get_firmware_version();
        int get_camera_id();
        float get_camera_temperature();
        void get_frame_w_h(int* width, int* height);
        void save_parmeter();
        void load_parmeter();

    private:
        int                     g_hCamera;     //设备句柄
        unsigned char           * g_pRawBuffer;     //raw数据
        tSdkFrameHead           g_tFrameHead;       //图像帧头信息
        tSdkCameraCapbility     g_tCapability;      //设备描述信息

        bool g_fshutter_nonsync_flag;
        double g_fshutter_nonsync_value;

        std::deque<std::chrono::high_resolution_clock::time_point> m_frame_time;
        double m_frame_rate;
        unsigned int frame_count;
        unsigned int error_frame_count;

        int camera_discovery();
        bool set_params();
};

#endif
