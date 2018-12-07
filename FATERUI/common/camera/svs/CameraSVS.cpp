#include "CameraSVS.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>

#include <iostream>
#include <fstream>

#include <cv.h>
#include <highgui.h>
#include <opencv2/imgproc/imgproc.hpp>

#include <signal.h>

using namespace std;
using namespace cv;

SVGigE_IMAGE* CameraSVS::g_signalImage;
char CameraSVS::g_camera_ip[16];
int CameraSVS::g_isread_success;

CameraSVS::CameraSVS()
{
    m_frame_rate = 0.0;
    frame_count = 0;
    error_frame_count = 0;

    g_image_width = 0;
    g_image_height = 0;

    g_isopen = false;
    g_isread_success = 0;

    g_wb_bgr[0] = 1.0;
    g_wb_bgr[1] = 1.0;
    g_wb_bgr[2] = 1.0;
    load_parameter();
}

CameraSVS::~CameraSVS()
{
    release_camera();
}

void CameraSVS::load_parameter()
{
    string str;
    ifstream ifile("svs_parameter");
    if(!ifile)
        return;
    for(int i = 0; i < 3; i++)
    {
        ifile >> str;
        g_wb_bgr[i] = atof(str.c_str());
    }
    ifile.close();
}

void CameraSVS::save_parameter()
{
    ofstream ofile("svs_parameter");
    ofile << g_wb_bgr[0] << endl << g_wb_bgr[1] << endl << g_wb_bgr[2];
    ofile.close();
}

bool CameraSVS::release_camera()
{
    Camera_setAcquisitionControl(g_camera, ACQUISITION_CONTROL_STOP);
    closeStream(g_stream);
    closeCamera(g_camera);
    g_isopen = false;
    g_isread_success = 0;
    return true;
}

SVGigE_RETURN CameraSVS::cam_callback_discover(SVGigE_SIGNAL *Signal, void* Context)
{
   Context = Context;
   if (Signal->SignalType == SVGigE_SIGNAL_CAMERA_FOUND)
   {
      SVGigE_CAMERA* camInfo = (SVGigE_CAMERA*) Signal->Data;
      printf("found camera mac: %s\n", camInfo->mac);
      printf("found camera ip: %s\n", camInfo->ip);
      memcpy(g_camera_ip, camInfo->ip, sizeof(g_camera_ip));
      return SVGigE_SUCCESS;
   }
   return SVGigE_ERROR;
}

int CameraSVS::camera_discovery()
{
    SVGigE_RETURN result;

    memset(g_camera_ip, 0, sizeof(g_camera_ip));

    unsigned int networks[16];
    size_t networkCount = 0;

    for( size_t i = 0 ; i < 16; i++ ){
        networks[i] = 0;
    }

    //find all local network interfaces
    result = findNetworkAdapters((unsigned int*) &networks, 16);

    if (result == SVGigE_SUCCESS)
    {
        for ( networkCount = 0; networkCount < 16; networkCount++)
        {
            if (networks[networkCount] == 0)
                break;
        }
    }
    else
    {
        return -1;
    }

    //perform device discovery on each local network interface
    for (unsigned int i = 0; i < networkCount; i++)
    {
        discoverCameras(networks[i], 1000, cam_callback_discover, NULL);
        if(g_camera_ip[0] != 0)
        {
            break;
        }
    }

    if(g_camera_ip[0] == 0)
        return -1;
    return 0;
}

SVGigE_RETURN CameraSVS::cam_callback_image(SVGigE_SIGNAL *signal, void *context)
{
    if(signal->SignalType == SVGigE_SIGNAL_FRAME_COMPLETED)
    {
        g_signalImage = (SVGigE_IMAGE *)signal->Data;
        g_isread_success = 1;
    }
    else
    {
        if(signal->SignalType == SVGigE_SIGNAL_FRAME_ABANDONED)
        {
            printf("camera: frame lost\n");
            fflush(stdout);
            g_isread_success = -1;
        }
        else
        {
            if(signal->SignalType == SVGigE_SIGNAL_START_OF_TRANSFER)
            {
                //printf("camera: SVGigE_SIGNAL_START_OF_TRANSFER\n");
            }
            else
            {
                printf("camera: invalid callback %d\n", signal->SignalType);
                fflush(stdout);
                g_isread_success = -1;
            }
        }
    }
    return SVGigE_SUCCESS;
}

bool CameraSVS::open()
{
    cout<<endl<<"====================================================="<<endl;
    cout<<"=   You are trying to open svs camera.   ="<<endl;
    cout<<"====================================================="<<endl<<endl;

    int error;
    unsigned int retries = 0;
    int flag = -1;

    release_camera();
    cout<< "[CameraSVS] " << "camera_discovery" << endl;
    flag = camera_discovery();

    while ( flag == -1 && retries < 3)
    {
        cout<<endl<<"[CameraSVS] ("<<retries<<") There is some error in init camera, waiting for retry..."<<endl<<endl;
        sleep(1);
        release_camera();
        flag = camera_discovery();
        retries++;
    }

    if(flag == -1)
    {
        printf("camera: error to find camera\n");
        return false;
    }

    // Now to open camera
    struct in_addr addr;
    if (inet_aton(g_camera_ip, &addr) == 0)
    {
        printf("camera: error ip\n");
        return false;
    }
    int result = openCamera(&g_camera, ntohl(addr.s_addr), 0, 3000, MULTICAST_MODE_NONE);
    if(result != SVGigE_SUCCESS)
    {
        printf("camera: openCamera failed\n");
        return false;
    }
    Camera_setAreaOfInterest(g_camera ,10000, 10000, 0, 0);

    unsigned int local_ip;
    unsigned short local_port;
    unsigned int bufsize;
    int bufcount = 10;
    int packetsize = 1500;

    Camera_getSizeX(g_camera, &g_image_width);
    printf("camera: g_image_width = %i\n", g_image_width);

    Camera_getSizeY(g_camera, &g_image_height);
    printf("camera: g_image_height = %i\n", g_image_height);

    result = Camera_getBufferSize(g_camera, &bufsize);
    if(result)
    {
        printf("camera: can't read buffersize\n");
        return false;
    }
    result = addStream(g_camera, &g_stream, &local_ip, &local_port, bufsize, bufcount, packetsize, 1000, cam_callback_image, NULL);
    if(result)
    {
        printf("camera: addStream failed\n");
        return false;
    }
    enableStream(g_stream, true);
    Camera_setAcquisitionMode(g_camera, ACQUISITION_MODE_SOFTWARE_TRIGGER, false);
    Camera_setAcquisitionControl(g_camera, ACQUISITION_CONTROL_START);
    g_isopen = true;

    cout<<"[CameraSVS] Ready to capture the image..."<<"."<<endl;

    m_frame_time.clear();
    return true;
}

bool CameraSVS::reboot_camera()
{
    if(true)
    {
        cerr << "[CameraSVS] Unsupport reboot. Please implement me. ";
        return false;
    }
    return true;
}

void CameraSVS::raw_to_bgr(const unsigned char *bayer, unsigned char *bgr, int sx, int sy, int tile)
{
    const int bayerStep = sx;
    const int bgrStep = 3 * sx;
    int width = sx;
    int height = sy;
    int blue = tile == GVSP_PIX_BAYBG8|| tile == GVSP_PIX_BAYGB8 ? -1 : 1;
    int start_with_green = tile == GVSP_PIX_BAYGB8 || tile == GVSP_PIX_BAYGR8;
    int i, imax, iinc;

    /* add black border */
    imax = sx * sy * 3;
    for (i = sx * (sy - 1) * 3; i < imax; i++) {
        bgr[i] = 0;
    }
    iinc = (sx - 1) * 3;
    for (i = (sx - 1) * 3; i < imax; i += iinc) {
        bgr[i++] = 0;
        bgr[i++] = 0;
        bgr[i++] = 0;
    }

    bgr += 1;
    width -= 1;
    height -= 1;

    for (; height--; bayer += bayerStep, bgr += bgrStep) {
        const unsigned char *bayerEnd = bayer + width;

        if (start_with_green) {
            bgr[-blue] = bayer[bayerStep];
            bgr[0] = (bayer[0] + bayer[bayerStep + 1] + 1) >> 1;
            bgr[blue] = bayer[1];
            bayer++;
            bgr += 3;
        }

        if (blue > 0) {
            for (; bayer <= bayerEnd - 2; bayer += 2, bgr += 6) {
                bgr[-1] = bayer[bayerStep + 1];
                bgr[0] = (bayer[1] + bayer[bayerStep] + 1) >> 1;
                bgr[1] = bayer[0];

                bgr[2] = bayer[bayerStep + 1];
                bgr[3] = (bayer[1] + bayer[bayerStep + 2] + 1) >> 1;
                bgr[4] = bayer[2];
            }
        } else {
            for (; bayer <= bayerEnd - 2; bayer += 2, bgr += 6) {
                bgr[1] = bayer[bayerStep + 1];
                bgr[0] = (bayer[1] + bayer[bayerStep] + 1) >> 1;
                bgr[-1] = bayer[0];

                bgr[4] = bayer[bayerStep + 1];
                bgr[3] = (bayer[1] + bayer[bayerStep + 2] + 1) >> 1;
                bgr[2] = bayer[2];
            }
        }

        if (bayer < bayerEnd) {
            bgr[-blue] = bayer[bayerStep + 1];
            bgr[0] = (bayer[1] + bayer[bayerStep] + 1) >> 1;
            bgr[blue] = bayer[0];
            bayer++;
            bgr += 3;
        }

        bayer -= width;
        bgr -= width * 3;

        blue = -blue;
        start_with_green = !start_with_green;
    }
}

void CameraSVS::do_whitebalance(unsigned char *im, const float *wb_bgr, int size)
{
    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            float val = (float)*im * wb_bgr[j];
            if(val > 255.0)
                val = 255.0;
            *im = (unsigned char)val;
            im += 1;
        }
    }
}

unsigned char* CameraSVS::capture_one_frame()
{
    int _try_time = 0;
    g_isread_success = 0;
    while(g_isread_success != 1 && _try_time < CAPTURE_RETRY_TIME)
    {
        g_isread_success = 0;
        int result = Camera_startAcquisitionCycle(g_camera);
        if (result != SVGigE_SUCCESS)
        {
            printf("camera: Error while using software trigger");
            continue;
        }
        unsigned int timeout = 10000;
        while (timeout > 0 && g_isread_success == 0)
        {
            usleep(100);
            timeout--;
        }
        if (g_isread_success == 1)
        {
            break;
        }
        _try_time++;
    }
    if(g_isread_success != 1)
    {
        printf("camera: read image error.\n");
        error_frame_count ++;
        return NULL;
    }

    // unsigned short block_id = g_signalImage->ImageID;
    unsigned char *img = g_signalImage->ImageData;
    // unsigned int s = g_signalImage->ImageSize;
    // unsigned int packetresend = g_signalImage->PacketResend;

    // Bayer to BGR
    unsigned char* bgr_buffer = new unsigned char[g_image_width * g_image_height * 3];
    // Mat img_tmp = Mat(g_image_height, g_image_width, CV_8UC1, img);
    Mat img_tmp2 = Mat(g_image_height, g_image_width, CV_8UC3, bgr_buffer);
    // cvtColor(img_tmp, img_tmp2, COLOR_BayerGR2BGR);
    raw_to_bgr(img, bgr_buffer, g_image_width, g_image_height, GVSP_PIX_BAYGR8);
    // Adjust white balance
    // do_whitebalance(bgr_buffer, g_wb_bgr, g_image_width*g_image_height);
    img_tmp2 = img_tmp2.mul(Scalar(g_wb_bgr[0], g_wb_bgr[1], g_wb_bgr[2]));
    // Release Mat but not bgr_buffer
    // img_tmp.release();
    img_tmp2.release();

    frame_count++;

    // 计算帧率
    unsigned int len = static_cast<unsigned int>(m_frame_time.size());
    if(len > 60)
    {
        m_frame_time.pop_front();
    }
    m_frame_time.push_back(std::chrono::high_resolution_clock::now());
    if (len > 1)
    {
        std::chrono::duration<double> time_span;
        time_span = std::chrono::duration_cast<std::chrono::duration<double>>(m_frame_time[len-1] - m_frame_time[0]);
        m_frame_rate = 1.0 / ((time_span.count() + 0.000001) / (len-1));
    }
    return  bgr_buffer;
}

bool CameraSVS::is_connected()
{
    return g_isopen;
}

int CameraSVS::get_frame_count()
{
    return frame_count;
}

int CameraSVS::get_error_frame_count()
{
    return error_frame_count;
}

double CameraSVS::get_frame_rate() 
{
    return m_frame_rate;
}

bool CameraSVS::set_wb_red(int red)
{
    g_wb_bgr[2] = red / 128.0;
    save_parameter();
    return true;
}

bool CameraSVS::set_wb_green(int green)
{
    g_wb_bgr[1] = green / 128.0;
    save_parameter();
    return true;
}

bool CameraSVS::set_wb_blue(int blue)
{
    g_wb_bgr[0] = blue / 128.0;
    save_parameter();
    return true;
}

bool CameraSVS::set_wb(int red, int green, int blue)
{
    g_wb_bgr[0] = blue / 128.0;
    g_wb_bgr[1] = green / 128.0;
    g_wb_bgr[2] = red / 128.0;
    save_parameter();
    return true;
}

bool CameraSVS::set_shutter(float _shutter)
{
    Camera_setExposureTime(g_camera, _shutter * 1000.0);
    Camera_writeEEPROM(g_camera);
    return true;
}

float CameraSVS::get_shutter()
{
    float _val = 0.0;
    Camera_getExposureTime(g_camera, &_val);
    return _val / 1000.0;
}

int CameraSVS::get_white_balance_red()
{
    return (int)(g_wb_bgr[2] * 128.0);
}

int CameraSVS::get_white_balance_green()
{
    return (int)(g_wb_bgr[1] * 128.0);
}

int CameraSVS::get_white_balance_blue()
{
    return (int)(g_wb_bgr[0] * 128.0);
}


char* CameraSVS::get_firmware_version()
{
    return NULL;
}

int CameraSVS::get_camera_id()
{
    return 1;
}

float CameraSVS::get_camera_temperature()
{
    unsigned int _val = 0;
    Camera_getSensorTemperature(g_camera, &_val);
    return (float)_val;
}

void CameraSVS::get_frame_w_h(int* width, int* height)
{
    *width = g_image_width;
    *height = g_image_height;
}
