#include "CameraPointgrey.h"
#include <stdlib.h>

using namespace FlyCapture2;
using namespace std;

CameraPointgrey::CameraPointgrey(int _mode, bool _single_mode, float _grabTimeout, bool _strobe_enable, float _trigger_delay, unsigned int _packetSize, unsigned int _interPacketDelay, int _intp_method, bool _debug, bool _is_hardware_trigger)
{
    FlyCapture2::BusManager busMgr;
    packetSize = _packetSize; // Packet size, in bytes.
    interPacketDelay = _interPacketDelay;
    grabTimeout = _grabTimeout;
    mode = _mode;
    single_mode = _single_mode;
    strobe_enable = _strobe_enable;

    debug = _debug;
    hardware_trigger = _is_hardware_trigger;
    pixelFormat = PIXEL_FORMAT_RAW8; //PIXEL_FORMAT_RGB8

    frame_count = 0;
    error_frame_count = 0;
    trigger_delay_time = _trigger_delay;
    Image::SetDefaultColorProcessing((ColorProcessingAlgorithm)_intp_method);

    m_pCamera = NULL;
    Error error = busMgr.GetCameraFromIndex(0, &guid);
    if (error == PGRERROR_OK){
        error = busMgr.GetInterfaceTypeFromGuid(&guid, &ifType);
        if (error == PGRERROR_OK){
            if (ifType == INTERFACE_GIGE) {
                m_pCamera = new FlyCapture2::GigECamera;
            } else{
                m_pCamera = new FlyCapture2::Camera;
            }
        }
    }
    if (m_pCamera == NULL){
        m_pCamera = new FlyCapture2::GigECamera;
    }

    error = m_pCamera->RestoreFromMemoryChannel(2);//restore from channel 2
    if (error != PGRERROR_OK){
            if(debug){
                cout<<"[CameraPointgrey] RestoreFromMemoryChannel..."<<error.GetDescription()<<endl;
            }
    }

    m_frame_rate = 0;
}

CameraPointgrey::~CameraPointgrey()
{

    release_camera();

}

bool CameraPointgrey::release_camera(){
    Error error;
    bool flag = true;

    if(m_pCamera != NULL){
        if(debug){
            cout<<"[CameraPointgrey] release camera start"<<endl;
        }
        if(m_pCamera->IsConnected()) {
            error = m_pCamera->StopCapture();
            if (error != PGRERROR_OK){
                if(debug){
                    cout<<"[CameraPointgrey] Error in StopCapture, "<<error.GetDescription()<<endl;
                }
                delete m_pCamera;
                m_pCamera = NULL;
                flag = false;
            }
            //before save, must StopCapture
            Error error = m_pCamera->SaveToMemoryChannel(2); //save to channel 2
            if (error != PGRERROR_OK){
                if(debug){
                    cout<<"[CameraPointgrey] SaveToMemoryChannel..."<<error.GetDescription()<<endl;
                }
            }
            // Disconnect the camera
            error = m_pCamera->Disconnect();
            if (error != PGRERROR_OK){
                cerr<<"[CameraPointgrey] Error in Disconnect, "<<error.GetDescription()<<endl;
                delete m_pCamera;
                m_pCamera = NULL;
                flag = false;
            }
        }

        delete m_pCamera;
        m_pCamera = NULL;

        if(debug){
            cout<<"[CameraPointgrey] release camera end"<<endl;
        }
    }
    return true;
}

bool CameraPointgrey::set_params(){
    Error error;

    if (single_mode == true){
        // Get current trigger settings
        TriggerMode triggerMode;
        error = m_pCamera->GetTriggerMode( &triggerMode );
        if (error != PGRERROR_OK)
        {
            cerr<<"[CameraPointgrey] Error in GetTriggerMode, "<<error.GetDescription()<<endl;
            return false;
        }

        // Set camera to trigger mode 0
        triggerMode.onOff = true;
        triggerMode.mode = 0;
        triggerMode.parameter = 0;

        // A source of 7 means software trigger
//        triggerMode.source = 7;
        triggerMode.source = 0;

        error = m_pCamera->SetTriggerMode( &triggerMode );
        if (error != PGRERROR_OK)
        {
            cerr<<"[CameraPointgrey] Error in SetTriggerMode, "<<error.GetDescription()<<endl;
            return false;
        }
    }else{
        TriggerMode triggerMode;
        error = m_pCamera->GetTriggerMode( &triggerMode );
        if (error != PGRERROR_OK)
        {
            cerr<<"[CameraPointgrey] Error in GetTriggerMode, "<<error.GetDescription()<<endl;
            return false;
        }

        // Set camera to trigger mode 0
        triggerMode.onOff = false;
        // triggerMode.mode = 0;

        error = m_pCamera->SetTriggerMode( &triggerMode );
        if (error != PGRERROR_OK)
        {
            cerr<<"[CameraPointgrey] Error in SetTriggerMode, "<<error.GetDescription()<<endl;
            return false;
        }
    }

    if (strobe_enable){
        StrobeControl mStrobe;
        mStrobe.source = 1;
        // mStrobe.parameter = 0;
        mStrobe.onOff = true;
        mStrobe.polarity = 2;    //rising edge
        mStrobe.delay = 0.0f;    // the strobe signal begin 1ms after the shutter opens
        error = m_pCamera->SetStrobe(&mStrobe);
        if ( error != PGRERROR_OK ){
            cerr<<"[CameraPointgrey] Error in setting StrobeControl, "<<error.GetDescription()<<endl;
            return false;
        }
        TriggerDelay trigger_delay;
        trigger_delay.onOff = true;
        trigger_delay.absControl = true;
        trigger_delay.absValue = trigger_delay_time;
        error = m_pCamera->SetTriggerDelay(&trigger_delay);
        if ( error != PGRERROR_OK ){
            cerr<<"[CameraPointgrey] Error in setting trigger_prop, "<<error.GetDescription()<<endl;
            return false;
        }
    }


    if(ifType == INTERFACE_GIGE) {
        cout<<"[CameraPointgrey] INTERFACE_GIGE"<<endl;
        FlyCapture2::GigECamera* gigeCam = static_cast<FlyCapture2::GigECamera*>(m_pCamera);
        error = gigeCam->SetGigEImagingMode ((Mode)mode);
        if ( error != PGRERROR_OK ){
            cerr<<"[CameraPointgrey] Error in setting ImagingMode, "<<error.GetDescription()<<endl;
            return false;
        }

        // Cache the current settings
        GigEImageSettings currImageSettings;
        error = gigeCam->GetGigEImageSettings( &currImageSettings );
        if ( error != PGRERROR_OK ){
            cerr<<"[CameraPointgrey] Error in GetGigEImageSettings, "<<error.GetDescription()<<endl;
            return false;
        }

        GigEImageSettingsInfo imageSettingsInfo;
        error = gigeCam->GetGigEImageSettingsInfo(&imageSettingsInfo);

        if (error != PGRERROR_OK){
            cerr<<"[CameraPointgrey] Error in GetGigEImageSettingsInfo, "<<error.GetDescription()<<endl;
            return false;
        }

        height = imageSettingsInfo.maxHeight;
        width = imageSettingsInfo.maxWidth;

        GigEImageSettings imageSettings;
        imageSettings.offsetX = 0;
        imageSettings.offsetY = 0;
        imageSettings.height = height;
        imageSettings.width = width;
        imageSettings.pixelFormat = pixelFormat;

        error = gigeCam->SetGigEImageSettings( &imageSettings );
        if (error != PGRERROR_OK){
            cerr<<"[CameraPointgrey] Error in SetGigEImageSettings, "<<error.GetDescription()<<endl;
            // Set the camera back to original state
            gigeCam->SetGigEImageSettings( &currImageSettings );
            return false;
        }

        // Set the packet size and delay to the camera
        GigEProperty packetSizeProp;
        packetSizeProp.propType = PACKET_SIZE;
        packetSizeProp.value = packetSize;
        error = gigeCam->SetGigEProperty(&packetSizeProp);
        if ( error != PGRERROR_OK ){
            cerr<<"[CameraPointgrey] There was an error setting the GigE packet size, attempting to revert to previous settings, "<<error.GetDescription()<<endl;
            gigeCam->SetGigEImageSettings( &currImageSettings );
            return false;
        }

        GigEProperty packetDelayProp;
        packetDelayProp.propType = PACKET_DELAY;
        packetDelayProp.value = interPacketDelay;
        error = gigeCam->SetGigEProperty(&packetDelayProp);
        if ( error != PGRERROR_OK ){
            cerr<<"[CameraPointgrey] There was an error setting the GigE packet delay, attempting to revert to previous settings, "<<error.GetDescription()<<endl;
            gigeCam->SetGigEImageSettings( &currImageSettings );
            return false;
        }
    }
    else{
        cout<<"[CameraPointgrey] USB"<<endl;
        FlyCapture2::Camera* p_cam = static_cast<FlyCapture2::Camera*>(m_pCamera);

        // Cache the current settings
        FlyCapture2::Format7ImageSettings curr_image_settings;
        unsigned int packet_size;
        float percentage;
        error = p_cam->GetFormat7Configuration(&curr_image_settings, &packet_size, &percentage);
        if ( error != PGRERROR_OK ){
            cerr<<"[CameraPointgrey] Error in GetFormat7Configuration, "<<error.GetDescription()<<endl;
            return false;
        }

        FlyCapture2::Format7Info image_settings_info;
        bool supported;
        error = p_cam->GetFormat7Info(&image_settings_info, &supported);
        if (error != PGRERROR_OK){
            cerr<<"[CameraPointgrey] Error in GetFormat7Info, "<<error.GetDescription()<<endl;
            return false;
        }
        height = image_settings_info.maxHeight;
        width = image_settings_info.maxWidth;

        FlyCapture2::Format7ImageSettings image_settings;
        image_settings.offsetX = 0;
        image_settings.offsetY = 0;
        image_settings.height = height;
        image_settings.width = width;
        image_settings.pixelFormat = pixelFormat;
        error = p_cam->SetFormat7Configuration(&image_settings, packet_size);
        if (error != PGRERROR_OK){
            cerr<<"[CameraPointgrey] Error in SetFormat7Configuration, "<<error.GetDescription()<<endl;
            // Set the camera back to original state
            p_cam->SetFormat7Configuration( &curr_image_settings, packet_size );
            return false;
        }

    }

    // close the gain
    Property prop_gain;
    prop_gain.type = GAIN;
    prop_gain.onOff = true;
    // Ensure auto-adjust mode is off.
    prop_gain.autoManualMode = false;
    // Ensure the property is set up to use absolute value control.
    prop_gain.absControl = true;
    prop_gain.absValue = 0;

    error = m_pCamera->SetProperty( &prop_gain );

    if ( error != PGRERROR_OK ){
        cerr<<"[CameraPointgrey] Error in setting gain, "<<error.GetDescription()<<endl;
        return false;
    }

    // Get the camera configuration
    FC2Config config;
    error = m_pCamera->GetConfiguration(&config);
    if (error != PGRERROR_OK)
    {
        cerr<<"[CameraPointgrey] Error in PGRERROR_OK, "<<error.GetDescription()<<endl;
        return false;
    }

    // Set the grab timeout to 5 seconds
    config.grabTimeout = grabTimeout;
    config.grabMode = DROP_FRAMES;
 //   config.numBuffers = 1;
    //config.highPerformanceRetrieveBuffer = true;

    // Set the camera configuration
    error = m_pCamera->SetConfiguration( &config );
    if (error != PGRERROR_OK)
    {
        cerr<<"[CameraPointgrey] Error in SetConfiguration, "<<error.GetDescription()<<endl;
        return false;
    }


    // Check for external trigger support
    // TriggerModeInfo triggerModeInfo;
    // error = m_pCamera->GetTriggerModeInfo( &triggerModeInfo );
    // if (error != PGRERROR_OK)
    // {
    //     cerr<<"[CameraPointgrey] Error in setting triggerModeInfo, "<<error.GetDescription()<<endl;
    //     return -1;
    // }

    // if ( triggerModeInfo.present != true )
    // {
    //     cout << "[CameraPointgrey] Camera does not support external trigger! Exiting..." << endl;
    //     return false;
    // }

    // VideoMode currVideoMode;
    // FrameRate currFrameRate;
    // error = m_pCamera->GetVideoModeAndFrameRate( &currVideoMode, &currFrameRate );
    // cout<<currVideoMode<<endl;
    // cout<<currFrameRate<<endl;


    // Mode mode;
    // error = m_pCamera->GetGigEImagingMode(&mode);

    // cout<< "mode"<<mode << endl;

    // bool supported = false;
	// mode = (Mode)4;
	// error = m_pCamera->QueryGigEImagingMode(mode, &supported);

	// cout<< supported << endl;

    return true;



    /*
    unsigned int numStreamChannels = 0;
    error = m_pCamera->GetNumStreamChannels( &numStreamChannels );
    if (error != PGRERROR_OK)
    {
        cerr<<"[CameraPointgrey] Error in GetNumStreamChannels, "<<error.GetDescription()<<endl;
        return false;
    }
    */
    /*
    GigEStreamChannel streamChannel;
    error = m_pCamera->GetGigEStreamChannelInfo( 0, &streamChannel );
    if (error != PGRERROR_OK){
        cerr<<"[CameraPointgrey] Error in GetGigEStreamChannelInfo, "<<error.GetDescription()<<endl;
        return false;
    }
    */

    // GigE image setting information

    //cout<<numStreamChannels<<endl;
}

bool CameraPointgrey::camera_discovery(){
    FlyCapture2::BusManager busMgr;
    Error error;

    int k = 0;

    unsigned int numCameras = 0;
    error = busMgr.GetNumOfCameras(&numCameras);

    if (error != PGRERROR_OK){
        error.PrintErrorTrace();
        return false;
    }

    if(numCameras==0){
        cout<<"[CameraPointgrey] Forcing IP addresses..."<<endl;
        error = BusManager::ForceAllIPAddressesAutomatically();
        if (error != PGRERROR_OK)
        {
            cerr<<"[CameraPointgrey] Error forcing IP addresses, "<<error.GetDescription()<<endl;
            return false;
        }

        while(k<2){
            cout<<"[CameraPointgrey] Waiting... "<<endl;
            sleep(3);

            error = busMgr.RescanBus();
            if (error != PGRERROR_OK)
            {
                cerr<<"[CameraPointgrey] Error rescanning bus, "<<error.GetDescription()<<endl;
                return false;
            }

            error = busMgr.GetNumOfCameras(&numCameras);

            if (error != PGRERROR_OK){
                error.PrintErrorTrace();
                return false;
            }

            if(numCameras>0){
                break;
            }

            k++;
        }
    }

    if(k>=5){
        cout<<"[CameraPointgrey] Time out in finding camera."<<endl;
        return false;
    }

    error = busMgr.GetCameraFromIndex(0, &guid);

    if (error != PGRERROR_OK){
        cerr<<"[CameraPointgrey] Error in GetCameraFromIndex, "<<error.GetDescription()<<endl;
        return false;
    }

    error = busMgr.GetInterfaceTypeFromGuid(&guid, &ifType);
    if (error == PGRERROR_OK){
        if (ifType == INTERFACE_GIGE) {
            m_pCamera = new FlyCapture2::GigECamera;
            cout<<"[CameraPointgrey] GigECamera"<<endl;
        } else{
            m_pCamera = new FlyCapture2::Camera;
            cout<<"[CameraPointgrey] Camera"<<endl;
        }
    }
    else{
        return false;
        cerr<<"[CameraPointgrey] Can't get interface type from guid."<<endl;
    }

    error = m_pCamera->Connect(&guid);
    if (error != PGRERROR_OK){
        cerr<<"[CameraPointgrey] Can't connect to the camera, "<<error.GetDescription()<<endl;
        return false;
    }

    cout<<"[CameraPointgrey] GetCameraInfo..."<<endl;
    error = m_pCamera->GetCameraInfo(&camInfo);

    if (error != PGRERROR_OK){
        cerr<<"[CameraPointgrey] Can't get the camera's information, "<<error.GetDescription()<<endl;
        return false;
    }

    if(debug){
        print_camera_info(&camInfo);
    }

    return true;
}

bool CameraPointgrey::open(){
    cout<<endl<<"====================================================="<<endl;
    cout<<"=   You are trying to connect a PointGrey camera.   ="<<endl;
    cout<<"====================================================="<<endl<<endl;

    Error error;

    unsigned int retries = 0;
    bool flag = false;

    release_camera();
    cout<<"[CameraPointgrey] "<<"camera_discovery"<<endl;
    flag = camera_discovery();
    if (flag)
    {
        cout<<"[CameraPointgrey] "<<"set_params"<<endl;
        flag = set_params();
    }

    if(!flag){
        return false;
    }

    cout<<"[CameraPointgrey] "<<"Start capture..."<<endl;
    error = m_pCamera->StartCapture();
    if (error != PGRERROR_OK && error != PGRERROR_ISOCH_ALREADY_STARTED){
        cerr<<"[CameraPointgrey] Can't start capture, "<<error.GetDescription()<<endl;
        return false;
    }

    usleep(100000);

    cout<<"[CameraPointgrey] Ready to capture the image..."<<"."<<endl;

    m_frame_time.clear();

    return true;
}

bool CameraPointgrey::reboot_camera(){
    cout<<"[CameraPointgrey] Some error in init camera, trying to reboot..."<<endl;

    Error error;
    const unsigned int k_cameraPower = 0x610;
    const unsigned int k_powerVal_on = 0x80000000;
    const unsigned int k_powerVal_off = 0x0;

    error  = m_pCamera->WriteRegister( k_cameraPower, k_powerVal_off );
    if (error != PGRERROR_OK)
    {
        cerr<<"[CameraPointgrey] Can't power off the camera, "<<error.GetDescription()<<endl;
        return false;
    }

    unsigned int regVal = 0;
    unsigned int retries = 10;

    // Wait for camera to complete power-off
    do
    {
        cout<<"[CameraPointgrey] Waiting for power off the camera..."<<endl;
        usleep(100000);
        error = m_pCamera->ReadRegister(k_cameraPower, &regVal);
        if (error == PGRERROR_TIMEOUT)
        {
            // ignore timeout errors, camera may not be responding to
            // register reads during power-up
        }
        else if (error != PGRERROR_OK)
        {
            cerr<<"[CameraPointgrey] Can't read the Register when power-off, "<<error.GetDescription()<<endl;
            return false;
        }

        retries--;
    } while ((regVal & k_powerVal_on) == 0 && retries > 0);

    if (error != PGRERROR_OK){
        cerr<<"[CameraPointgrey] Can't power off the camera, "<<error.GetDescription()<<endl;
        return false;
    }

    error  = m_pCamera->WriteRegister( k_cameraPower, k_powerVal_on );
    if (error != PGRERROR_OK)
    {
        cerr<<"[CameraPointgrey] Can't power on the camera, "<<error.GetDescription()<<endl;
        return false;
    }

    regVal = 0;
    retries = 10;
    do
    {
        cout<<"[CameraPointgrey] Waiting for power on the camera..."<<endl;
        usleep(100000);
        error = m_pCamera->ReadRegister(k_cameraPower, &regVal);
        if (error == PGRERROR_TIMEOUT)
        {
            // ignore timeout errors, camera may not be responding to
            // register reads during power-up
        }
        else if (error != PGRERROR_OK)
        {
            cerr<<"[CameraPointgrey] Can't read the Register when power-on, "<<error.GetDescription()<<endl;
            return false;
        }

        retries--;
    } while ((regVal & k_powerVal_off) == 0 && retries > 0);

    if (error != PGRERROR_OK){
        cerr<<"[CameraPointgrey] Can't power on the camera, "<<error.GetDescription()<<endl;
        return false;
    }

    return true;
}


void CameraPointgrey::print_camera_info( CameraInfo* pCamInfo ){
    char macAddress[64];
    sprintf(macAddress,
            "%02X:%02X:%02X:%02X:%02X:%02X",
            pCamInfo->macAddress.octets[0],
            pCamInfo->macAddress.octets[1],
            pCamInfo->macAddress.octets[2],
            pCamInfo->macAddress.octets[3],
            pCamInfo->macAddress.octets[4],
            pCamInfo->macAddress.octets[5]);

    char ipAddress[32];
    sprintf(ipAddress,
            "%u.%u.%u.%u",
            pCamInfo->ipAddress.octets[0],
            pCamInfo->ipAddress.octets[1],
            pCamInfo->ipAddress.octets[2],
            pCamInfo->ipAddress.octets[3]);


    char subnetMask[32];
    sprintf(subnetMask,
            "%u.%u.%u.%u",
            pCamInfo->subnetMask.octets[0],
            pCamInfo->subnetMask.octets[1],
            pCamInfo->subnetMask.octets[2],
            pCamInfo->subnetMask.octets[3]);

    char defaultGateway[32];
    sprintf(defaultGateway,
            "%u.%u.%u.%u",
            pCamInfo->defaultGateway.octets[0],
            pCamInfo->defaultGateway.octets[1],
            pCamInfo->defaultGateway.octets[2],
            pCamInfo->defaultGateway.octets[3]);


    cout << endl;
	cout << "*** CAMERA INFORMATION ***" << endl;
	cout << "Serial number - " << pCamInfo->serialNumber << endl;
    cout << "Camera model - " << pCamInfo->modelName << endl;
    cout << "Camera vendor - " << pCamInfo->vendorName << endl;
    cout << "Sensor - " << pCamInfo->sensorInfo << endl;
    cout << "Resolution - " << pCamInfo->sensorResolution << endl;
    cout << "Firmware version - " << pCamInfo->firmwareVersion << endl;
    cout << "Firmware build time - " << pCamInfo->firmwareBuildTime << endl;
    cout << "GigE version - " << pCamInfo->gigEMajorVersion << "." << pCamInfo->gigEMinorVersion << endl;
    cout << "User defined name - " << pCamInfo->userDefinedName << endl;
    cout << "XML URL 1 - " << pCamInfo->xmlURL1 << endl;
    cout << "XML URL 2 - " << pCamInfo->xmlURL2 << endl;
    cout << "MAC address - " << macAddress << endl;
    cout << "IP address - " << ipAddress << endl;
    cout << "Subnet mask - " << subnetMask << endl;
    cout << "Default gateway - " << defaultGateway << endl << endl;

    cout << "*** CAMERA PARAMETERS ***" << endl;
    cout << "mode - " << mode <<endl;
    cout << "packetSize - " << packetSize << endl; // Packet size, in bytes.
    cout << "interPacketDelay - " <<  interPacketDelay << endl;
    cout << "grabTimeout - " << grabTimeout << endl;
    cout << "single_mode - " << single_mode << endl;
    cout << "trigger_delay - " << trigger_delay_time << endl;
    cout << "strobe_enable - " << strobe_enable << endl;
    cout << "shutterTime - " << shutterTime << endl;
    cout << "pixelFormat - PIXEL_FORMAT_RAW8 " << endl << endl;
}

unsigned char* CameraPointgrey::capture_one_frame(){
    Error error;
    int k =0;

    if (single_mode == true && hardware_trigger==false)
    {
        error = m_pCamera->FireSoftwareTrigger();
        if ( error != PGRERROR_OK ){
            cerr<<"[CameraPointgrey] Error in FireSoftwareTrigger, "<<error.GetDescription()<<endl;
            return NULL;
        }
    }
    error = m_pCamera->RetrieveBuffer( &rawImage );
    if (error != PGRERROR_OK){
        error_frame_count++;
        if(debug){
            cout<<"[CameraPointgrey] "<<error.GetDescription()<<endl;
        }
        return NULL;
    }
    frame_count++;
    // Convert the raw image

    int size = height*width*3;
    unsigned char* buffer = new unsigned char[size];
    convertedImage.SetData(buffer, size);

    error = rawImage.Convert( PIXEL_FORMAT_BGR, &convertedImage );
    if ( error != PGRERROR_OK ){
        cerr<<"[CameraPointgrey] Error in rawImage.Convert, "<<error.GetDescription()<<endl;
        free(buffer);
        return NULL;
    }

    unsigned int len = static_cast<unsigned int>(m_frame_time.size());
    if(len > 60) {
        m_frame_time.pop_front();
    }
    m_frame_time.push_back(std::chrono::high_resolution_clock::now());
    if (len > 1) {
        std::chrono::duration<double> time_span;
        time_span = std::chrono::duration_cast<std::chrono::duration<double>>(m_frame_time[len-1] - m_frame_time[0]);
        m_frame_rate = 1.0 / ((time_span.count() + 0.000001) / (len-1));
    }

    return convertedImage.GetData();
}

bool CameraPointgrey::is_connected(){
    return m_pCamera->IsConnected();
}

int CameraPointgrey::get_frame_count() {
    return frame_count;
}

int CameraPointgrey::get_error_frame_count() {
    return error_frame_count;
}

double CameraPointgrey::get_frame_rate() {
    return m_frame_rate;
}

/**
    add by Bill.xie
**/

bool CameraPointgrey::set_wb_red(int red) {
    if (red < 0 || red > 1023) {
        return false;
    }
    //White balance
    Property prop_wb;
    //Define the property to adjust.
    prop_wb.type = WHITE_BALANCE;
    m_pCamera->GetProperty(&prop_wb);
    //Ensure the property is on.
    prop_wb.onOff = true;
    //Ensure auto-adjust mode is off.
    prop_wb.autoManualMode = false;
    prop_wb.valueA = red;
    //Set the property.
    Error error;
    bool is_connected = m_pCamera->IsConnected();
    if(is_connected == false) {
        m_pCamera->Connect();
    }

    error = m_pCamera->SetProperty( &prop_wb );
    usleep(500);
    if (is_connected == false) {
        m_pCamera->Disconnect();
    }
    if ( error != PGRERROR_OK ){
         cerr<<"[CameraPointgrey] Error in setting wb, "<<error.GetDescription()<<endl;
         return false;
    }
    return true;
}

bool CameraPointgrey::set_wb_blue(int blue) {
    if (blue < 0 || blue > 1023) {
        return false;
    }
    //White balance
    Property prop_wb;
    //Define the property to adjust.
    prop_wb.type = WHITE_BALANCE;
    m_pCamera->GetProperty(&prop_wb);
    //Ensure the property is on.
    prop_wb.onOff = true;
    //Ensure auto-adjust mode is off.
    prop_wb.autoManualMode = false;
    prop_wb.valueB = blue;
    //Set the property.
    Error error;
    bool is_connected = m_pCamera->IsConnected();
    if(is_connected == false) {
        m_pCamera->Connect();
    }

    error = m_pCamera->SetProperty( &prop_wb );
    usleep(500);
    if (is_connected == false) {
        m_pCamera->Disconnect();
    }
    if ( error != PGRERROR_OK ){
         cerr<<"[CameraPointgrey] Error in setting wb, "<<error.GetDescription()<<endl;
         return false;
    } else {
        cout << "set wb blue" << blue << endl;
    }
    return true;
}

bool CameraPointgrey::set_wb(int red, int blue) {
    if (red < 0 || red > 1023) {
        return false;
    }

    if (blue < 0 || blue > 1023) {
        return false;
    }
    //White balance
    Property prop_wb;
    //Define the property to adjust.
    prop_wb.type = WHITE_BALANCE;
    //Ensure the property is on.
    prop_wb.onOff = true;
    //Ensure auto-adjust mode is off.
    prop_wb.autoManualMode = false;
    //Set the white balance red channel to 500.

    prop_wb.valueA = red;
    //Set the white balance red channel to 850.
    prop_wb.valueB = blue;
    //Set the property.
    Error error;
    bool is_connected = m_pCamera->IsConnected();
    usleep(500);
    if (is_connected == false) {
        m_pCamera->Connect();
    }

    error = m_pCamera->SetProperty( &prop_wb );
    if (is_connected == false) {
        m_pCamera->Disconnect();
    }
    if ( error != PGRERROR_OK ){
         cerr<<"[CameraPointgrey] Error in setting wb, "<<error.GetDescription()<<endl;
         return false;
    }
    return true;
}

bool CameraPointgrey::set_shutter(float _shutter) {
    //shutter
    Property prop_shutter;
    //Define the property to adjust.
    prop_shutter.type = SHUTTER;
    //Ensure the property is on.
    prop_shutter.onOff = true;
    //Ensure auto-adjust mode is off.
    prop_shutter.autoManualMode = false;
    //Ensure the property is set up to use absolute value control.
    prop_shutter.absControl = true;
    //Set the absolute value of shutter to xx ms.
    prop_shutter.absValue = _shutter;
    //Set the property.
    Error error;
    bool is_connected = m_pCamera->IsConnected();
    usleep(500);
    if (is_connected == false) {
        m_pCamera->Connect();
    }

    error = m_pCamera->SetProperty( &prop_shutter );
    if (is_connected == false) {
        m_pCamera->Disconnect();
    }
    if ( error != PGRERROR_OK ){
        cerr<<"[CameraPointgrey] Error in setting shutter, "<<error.GetDescription()<<endl;
        return false;
    }
    return true;
}

float CameraPointgrey::get_shutter() {
    //shutter
    Property prop_shutter;
    //Define the property to adjust.
    prop_shutter.type = SHUTTER;

    bool is_connected = m_pCamera->IsConnected();
    if (is_connected == false) {
        m_pCamera->Connect();
    }
    Error error = m_pCamera->GetProperty(&prop_shutter);
    if (is_connected == false) {
        m_pCamera->Disconnect();
    }
    if ( error != PGRERROR_OK ){
        cerr<<"[CameraPointgrey] Error in get shutter, "<<error.GetDescription()<<endl;
        return -1;
    }
    float shutter = prop_shutter.absValue;
    return shutter;
}

int CameraPointgrey::get_white_balance_red() {
    //shutter
    Property prop_wb;
    //Define the property to adjust.
    prop_wb.type = WHITE_BALANCE;

    bool is_connected = m_pCamera->IsConnected();
    if (is_connected == false) {
        m_pCamera->Connect();
    }
    Error error = m_pCamera->GetProperty(&prop_wb);
    if (is_connected == false) {
        m_pCamera->Disconnect();
    }
    if ( error != PGRERROR_OK ){
        cerr<<"[CameraPointgrey] Error in get wb red, "<<error.GetDescription()<<endl;
        return -1;
    }
    int red = prop_wb.valueA;
    return red;
}

int CameraPointgrey::get_white_balance_blue() {
    //shutter
    Property prop_wb;
    //Define the property to adjust.
    prop_wb.type = WHITE_BALANCE;
    bool is_connected = m_pCamera->IsConnected();
    if (is_connected == false) {
        m_pCamera->Connect();
    }
    Error error = m_pCamera->GetProperty(&prop_wb);
    if (is_connected == false) {
        m_pCamera->Disconnect();
    }
    if ( error != PGRERROR_OK ){
        cerr<<"[CameraPointgrey] Error in get wb blue, "<<error.GetDescription()<<endl;
        return -1;
    }
    int blue = prop_wb.valueB;
    return blue;
}


char* CameraPointgrey::get_firmware_version(){
    bool is_connected = m_pCamera->IsConnected();
    if (is_connected == false) {
        m_pCamera->Connect();
    }
    Error error = m_pCamera->GetCameraInfo(&camInfo);
    if (is_connected == false) {
        m_pCamera->Disconnect();
    }
    if (error != PGRERROR_OK){
        cerr<<"[CameraPointgrey] Can't get the camera's information, "<<error.GetDescription()<<endl;
        return NULL;
    }
    return camInfo.firmwareVersion;
}

int CameraPointgrey::get_camera_id(){
    bool is_connected = m_pCamera->IsConnected();
    if (is_connected == false) {
        m_pCamera->Connect();
    }
    Error error = m_pCamera->GetCameraInfo(&camInfo);
    if (is_connected == false) {
        m_pCamera->Disconnect();
    }

    if (error != PGRERROR_OK){
        cerr<<"[CameraPointgrey] Can't get the camera's information, "<<error.GetDescription()<<endl;
        return -1;
    }
    return camInfo.serialNumber;
}


float CameraPointgrey::get_camera_temperature(){
    Property prop_temperature;
    prop_temperature.type = TEMPERATURE;
    bool is_connected = m_pCamera->IsConnected();
    if (is_connected == false) {
        m_pCamera->Connect();
    }
    Error error = m_pCamera->GetProperty(&prop_temperature);
    if (is_connected == false) {
        m_pCamera->Disconnect();
    }

    if (error != PGRERROR_OK){
        cerr<<"[CameraPointgrey] Can't get the camera's property info, "<<error.GetDescription()<<endl;
        return -273.15f;
    }
    float temperature = prop_temperature.valueA / 10.0f - 273.15f;
    return temperature;
}
