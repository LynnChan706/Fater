#ifndef __CAMERASVS_H
#define __CAMERASVS_H

#include <iostream>
#include <unistd.h>
#include <deque>
#include <chrono>
#include "svgige.h"

#define CAPTURE_RETRY_TIME 3

class CameraSVS
{
    public:
        CameraSVS();
        ~CameraSVS();

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

        void load_parameter();
        void save_parameter();
    private:
        Camera_handle g_camera;
        Stream_handle g_stream;

        unsigned int g_image_width;
        unsigned int g_image_height;
        bool g_isopen;
        static int g_isread_success;

        static SVGigE_IMAGE *g_signalImage;

        float g_wb_bgr[3];

        std::deque<std::chrono::high_resolution_clock::time_point> m_frame_time;
        double m_frame_rate;
        unsigned int frame_count;
        unsigned int error_frame_count;

        static char g_camera_ip[16];

        static SVGigE_RETURN cam_callback_discover(SVGigE_SIGNAL *Signal, void* Context);
        static SVGigE_RETURN cam_callback_image(SVGigE_SIGNAL *signal, void *context);
        int camera_discovery();
        void raw_to_bgr(const unsigned char *bayer, unsigned char *bgr, int sx, int sy, int tile);
        void do_whitebalance(unsigned char *im, const float *wb_bgr, int size);
};

#endif
