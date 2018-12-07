#include "CameraMindVision.h"
#include <stdlib.h>

using namespace std;

CameraMindVision::CameraMindVision(int _mode, bool _single_mode, float _grabTimeout, bool _strobe_enable, float _trigger_delay, unsigned int _packetSize, unsigned int _interPacketDelay, int _intp_method, bool _debug, bool _is_hardware_trigger)
{
    g_hCamera = -1;         //设备句柄
    g_pRawBuffer=NULL;      //raw数据

    m_frame_rate = 0.0;
    frame_count = 0;
    error_frame_count = 0;
}

CameraMindVision::~CameraMindVision()
{
    release_camera();
}

int CameraMindVision::init_SDK()
{
    int                     iCameraCounts = 4;
    int                     iStatus=-1;
    tSdkCameraDevInfo       tCameraEnumList[4];

    //sdk初始化  0 English 1中文
    CameraSdkInit(1);

    //枚举设备，并建立设备列表
    CameraEnumerateDevice(tCameraEnumList, &iCameraCounts);

    printf("MindVision device count:%d\n", iCameraCounts);

    //没有连接设备
    if(iCameraCounts==0)
    {
        return -1;
    }

    //相机初始化。初始化成功后，才能调用任何其他相机相关的操作接口
    iStatus = CameraInit(&tCameraEnumList[0], -1, -1, &g_hCamera);

    //初始化失败
    if(iStatus!=CAMERA_STATUS_SUCCESS)
    {
        return -1;
    }
    //获得相机的特性描述结构体。该结构体中包含了相机可设置的各种参数的范围信息。决定了相关函数的参数
    CameraGetCapability(g_hCamera,&g_tCapability);

    /*让SDK进入工作模式，开始接收来自相机发送的图像
    数据。如果当前相机是触发模式，则需要接收到
    触发帧以后才会更新图像。    */
    CameraPlay(g_hCamera);

    load_parmeter();

    CameraSetIspOutFormat(g_hCamera,CAMERA_MEDIA_TYPE_BGR8);//CAMERA_MEDIA_TYPE_BGR8

    // 停止自动曝光
    CameraSetAeState(g_hCamera, false);
    // 手动曝光 增益
    CameraSetAnalogGain(g_hCamera, 4);

    //如果是用U3相机，请加上这个和延迟，图像输出才正常
    // int RPos, GPos, BPos;
    // CameraGetGain(g_hCamera,&RPos,&GPos,&BPos);
    // CameraSetGain(g_hCamera,RPos,GPos,BPos);
    // usleep(100000);

    //设置预览的分辨率。
    tSdkImageResolution *pImageSizeDesc = g_tCapability.pImageSizeDesc;// 预设分辨率选择
    CameraSetImageResolution(g_hCamera,&(pImageSizeDesc[0]));
    //软触发模式
    // CameraSetTriggerMode(g_hCamera, 1);
    return 0;
}

bool CameraMindVision::release_camera()
{
    CameraSetTriggerMode(g_hCamera, 0);

    if(g_hCamera>0)
    {
        //相机反初始化。释放资源。
        CameraUnInit(g_hCamera);
        g_hCamera=-1;
    }
    return true;
}

bool CameraMindVision::set_params()
{

    return true;
}

int CameraMindVision::camera_discovery()
{
    return init_SDK();
}

bool CameraMindVision::open()
{
    cout<<endl<<"====================================================="<<endl;
    cout<<"=   You are trying to open MindVision camera.   ="<<endl;
    cout<<"====================================================="<<endl<<endl;

    int error;

    unsigned int retries = 0;
    int flag = -1;

    release_camera();
    cout<<"[CameraMindVision] "<<"camera_discovery"<<endl;
    flag = camera_discovery();

    while ( flag == -1 && retries < 0)
    {
        cout<<endl<<"[CameraMindVision] ("<<retries<<") There is some error in init camera, waiting for retry..."<<endl<<endl;
        sleep(1);
        release_camera();
        flag = camera_discovery();
        retries++;
    }

    if(flag == -1)
    {
        return false;
    }

    cout<<"[CameraMindVision] Ready to capture the image..."<<"."<<endl;

    m_frame_time.clear();
    return true;
}

bool CameraMindVision::reboot_camera()
{
    if(CameraReConnect(g_hCamera) != CAMERA_STATUS_SUCCESS)
    {
        cerr << "[CameraMindVision] Unsupport reboot. Please implement me. ";
        return false;
    }
    return true;
}

unsigned char* CameraMindVision::capture_one_frame()
{
    unsigned char* g_pRgbBuffer = NULL;
    if (CameraGetImageBuffer(g_hCamera, &g_tFrameHead, &g_pRawBuffer, 2000) == CAMERA_STATUS_SUCCESS)
    {
        // 如果是C++调用此接口，必须主动释放此内存。如果是python调用，会被自动释放
        g_pRgbBuffer = new unsigned char[g_tFrameHead.iWidth * g_tFrameHead.iHeight * 3];
        CameraImageProcess(g_hCamera, g_pRawBuffer, g_pRgbBuffer, &g_tFrameHead);
        CameraReleaseImageBuffer(g_hCamera, g_pRawBuffer);
        if(g_tFrameHead.uiMediaType == CAMERA_MEDIA_TYPE_MONO8)
        {
            cerr << "[CameraMindVision] Unsupport this type" << endl;
        }
    }
    else
    {
        cerr << "[CameraMindVision] Error in CameraGetImageBuffer, " << "timeout" << endl;
        error_frame_count ++;
        return NULL;
    }
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
    return g_pRgbBuffer;
}

bool CameraMindVision::is_connected()
{
    int res = CameraConnectTest(g_hCamera);
    if(res == CAMERA_STATUS_SUCCESS)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int CameraMindVision::get_frame_count()
{
    return frame_count;
}

int CameraMindVision::get_error_frame_count()
{
    return error_frame_count;
}

double CameraMindVision::get_frame_rate() 
{
    return m_frame_rate;
}

bool CameraMindVision::set_wb_red(int red)
{
    if(is_connected() == false)
        return false;
    if (red < 0 || red > 1023) {
        return false;
    }
    int RPos, GPos, BPos;
    CameraGetGain(g_hCamera, &RPos, &GPos, &BPos);
    CameraSetGain(g_hCamera, red, GPos, BPos);
    return true;
}

bool CameraMindVision::set_wb_green(int green)
{
    if(is_connected() == false)
        return false;
    if (green < 0 || green > 1023)
    {
        return false;
    }
    int RPos, GPos, BPos;
    CameraGetGain(g_hCamera, &RPos, &GPos, &BPos);
    CameraSetGain(g_hCamera, RPos, green, BPos);
    return true;
}

bool CameraMindVision::set_wb_blue(int blue)
{
    if(is_connected() == false)
        return false;
    if (blue < 0 || blue > 1023)
    {
        return false;
    }
    int RPos, GPos, BPos;
    CameraGetGain(g_hCamera, &RPos, &GPos, &BPos);
    CameraSetGain(g_hCamera, RPos, GPos, blue);
    return true;
}

bool CameraMindVision::set_wb(int red, int green, int blue)
{
    if(is_connected() == false)
        return false;
    if (red < 0 || red > 1023)
    {
        return false;
    }
    if (green < 0 || green > 1023)
    {
        return false;
    }
    if (blue < 0 || blue > 1023)
    {
        return false;
    }
    CameraSetGain(g_hCamera, red, green, blue);
    return true;
}

// bool CameraMindVision::set_once_wb()
// {
//     if(CameraSetOnceWB(g_hCamera) == CAMERA_STATUS_SUCCESS)
//         return true
//     else
//         return false
// }

bool CameraMindVision::set_shutter(float _shutter)
{
    if(is_connected() == false)
        return false;
    double m_fExpLineTime=0;//当前的行曝光时间，单位为us
    CameraGetExposureLineTime(g_hCamera, &m_fExpLineTime);
    _shutter = _shutter * m_fExpLineTime * 1000.0;//us = ms * 1000
    if(CameraSetExposureTime(g_hCamera, (double)_shutter) == CAMERA_STATUS_SUCCESS)
    {
        return true;
    }
    else
    {
        return false;
    }
}

float CameraMindVision::get_shutter()
{
    if(is_connected() == false)
        return 0.0;
    double pfExposureTime = 0.0;
    if(CameraGetExposureTime(g_hCamera, &pfExposureTime) == CAMERA_STATUS_SUCCESS)
    {
        double m_fExpLineTime = 0;//当前的行曝光时间，单位为us
        CameraGetExposureLineTime(g_hCamera, &m_fExpLineTime);
        return (float)(pfExposureTime / m_fExpLineTime / 1000.0);
    }
    else
    {
        return 0.0;
    }
}

int CameraMindVision::get_white_balance_red()
{
    if(is_connected() == false)
        return 0;
    int RPos, GPos, BPos;
    if(CameraGetGain(g_hCamera, &RPos, &GPos, &BPos) == CAMERA_STATUS_SUCCESS)
    {
        return RPos;
    }
    else
    {
        return 0;
    }
}

int CameraMindVision::get_white_balance_green()
{
    if(is_connected() == false)
        return 0;
    int RPos, GPos, BPos;
    if(CameraGetGain(g_hCamera, &RPos, &GPos, &BPos) == CAMERA_STATUS_SUCCESS)
    {
        return GPos;
    }
    else
    {
        return 0;
    }
}

int CameraMindVision::get_white_balance_blue()
{
    if(is_connected() == false)
        return 0;
    int RPos, GPos, BPos;
    if(CameraGetGain(g_hCamera, &RPos, &GPos, &BPos) == CAMERA_STATUS_SUCCESS)
    {
        return BPos;
    }
    else
    {
        return 0;
    }
}


char* CameraMindVision::get_firmware_version()
{
    // char* version = new char[32];
    // if(CameraGetFirmwareVision(g_hCamera, version) == CAMERA_STATUS_SUCCESS)
    //     return version;
    // else
    return NULL;
}

int CameraMindVision::get_camera_id()
{
    return 1;
}

float CameraMindVision::get_camera_temperature()
{
    return 0.0f;
}

void CameraMindVision::get_frame_w_h(int* width, int* height)
{
    *width = g_tFrameHead.iWidth;
    *height = g_tFrameHead.iHeight;
}

void CameraMindVision::save_parmeter()
{
    CameraSaveParameter(g_hCamera, PARAMETER_TEAM_A);
}

void CameraMindVision::load_parmeter()
{
    CameraLoadParameter(g_hCamera, PARAMETER_TEAM_A);
}
