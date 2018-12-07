#include "CameraBobcat.h"
#include <stdlib.h>
#include <vector>
#include <string.h>
#include <iomanip>

using namespace std;

CameraBobcat::CameraBobcat(int _mode, bool _single_mode, float _grabTimeout, bool _strobe_enable,
                            float _trigger_delay, unsigned int _packetSize, unsigned int _interPacketDelay,
                            int _intp_method, bool _debug, bool _is_hardware_trigger)
{
    mDevice = NULL;
    mStream = NULL;
    mPipeline = NULL;
    mConnectionLost = false;

    m_frame_rate = 0;
}

CameraBobcat::~CameraBobcat()
{

    release_camera();

}

bool CameraBobcat::release_camera(){

    return true;
}

bool CameraBobcat::set_params(){
    return true;
}

bool CameraBobcat::camera_discovery(){

    return true;
}

bool CameraBobcat::GetFrameDimensions( PvGenParameterArray * aDeviceParams, PvDevice * aDevice )
{
    PvGenInteger* lFrameWidth = dynamic_cast<PvGenInteger*>( aDeviceParams->Get( "Width") );
    PvGenInteger* lFrameHeight = dynamic_cast<PvGenInteger*>( aDeviceParams->Get( "Height") );

    if ( !lFrameWidth )
    {
        cerr << "Could not get width" << endl;
        PvDevice::Free( aDevice );
        return false;
    }

    if ( !lFrameHeight )
    {
        cerr << "Could not get height" << endl;
        PvDevice::Free( aDevice );
        return false;
    }

    lFrameWidth->GetValue( width );
    lFrameHeight->GetValue( height );

    cout << "Frame dimensions are " << width << "x" << height << endl;

    return true;
}

bool CameraBobcat::open(){
    const PvDeviceInfo *lDeviceInfo = NULL;
    PvResult lResult = PvResult::Code::INVALID_PARAMETER;
    PvSystem lSystem;
    cout<<endl<<"====================================================="<<endl;
    cout<<"=   You are trying to connect a Bobcat camera.   ="<<endl;
    cout<<"====================================================="<<endl<<endl;


    lSystem.Find();

    for ( uint32_t i = 0; i < lSystem.GetInterfaceCount(); i++ )
    {
        const PvInterface *lInterface = dynamic_cast<const PvInterface *>( lSystem.GetInterface( i ) );
        if ( lInterface != NULL )
        {
            for ( uint32_t j = 0; j < lInterface->GetDeviceCount(); j++ )
            {
                lDeviceInfo = dynamic_cast<const PvDeviceInfo *>( lInterface->GetDeviceInfo( j ) );
                if ( lDeviceInfo != NULL )
                {
                    cout << lDeviceInfo->GetDisplayID().GetAscii() << endl;
                    break;
                }
            }
        }
    }

    if( lDeviceInfo == NULL )
    {
        cout << "No device found!" << endl;
        return false;
    }

    // Connect to the GigE Vision or USB3 Vision device
    cout << "Connecting to " << lDeviceInfo->GetDisplayID().GetAscii() << endl;
    mDevice = PvDevice::CreateAndConnect( lDeviceInfo, &lResult );
    if ( !lResult.IsOK() )
    {
        cout << "Unable to connect to " << lDeviceInfo->GetDisplayID().GetAscii()
             << "\tError:" << lResult.GetCodeString().GetAscii() << endl;
        PvDevice::Free( mDevice );
        return false;
    }

     // Get device parameters need to control streaming
    PvGenParameterArray *lDeviceParams = mDevice->GetParameters();
    PvGenCommand *lStart = dynamic_cast<PvGenCommand *>( lDeviceParams->Get( "AcquisitionStart" ) );
    PvGenCommand *lStop = dynamic_cast<PvGenCommand *>( lDeviceParams->Get( "AcquisitionStop" ) );

    // Get acquisition mode parameter
    PvGenEnum *lExpMode = mDevice->GetParameters()->GetEnum( "ExposureMode" );
    if ( lExpMode == NULL )
    {
        cerr << "Can't get ExposureMode Parameters." << endl;
       return false;
    }
    lResult = lExpMode->SetValue(1); // Set Timed Mode  0:Off, 1:Timed, 2:IOExposureControl
    if ( !lResult.IsOK() )
    {
        cerr << "Faile to set ExposureMode(Timed)." << endl;
        return false;
    }


    PvGenEnum *lWhiteBalanceMode = mDevice->GetParameters()->GetEnum( "WhiteBalanceMode" );
    if ( lWhiteBalanceMode == NULL )
    {
        cerr << "Can't get WhiteBalanceMode Parameters." << endl;
       return false;
    }
    // Set White Balance Mode
    // 0:Off, 1:Once, 2:Auto, 3:Manual
    lResult = lWhiteBalanceMode->SetValue(3);
    if ( !lResult.IsOK() )
    {
        cerr << "Faile to set White Balance Mode(Manual)." << endl;
        return false;
    }

    PvGenBoolean *lFlatFieldCorrection = mDevice->GetParameters()->GetBoolean ( "FlatFieldCorrection" );
    if ( lFlatFieldCorrection == NULL )
    {
        cerr << "Can't get FlatFieldCorrection Parameters." << endl;
        return false;
    }
    lResult = lFlatFieldCorrection->SetValue(true);
    if ( !lResult.IsOK() )
    {
        cerr << "Faile to set FlatFieldCorrection(true)." << endl;
        return false;
    }

    PvGenEnum *lDefectPixelCorrection = mDevice->GetParameters()->GetEnum( "DefectPixelCorrection" );
    if ( lDefectPixelCorrection == NULL )
    {
        cerr << "Can't get DefectPixelCorrection Parameters." << endl;
        return false;
    }
    lResult = lDefectPixelCorrection->SetValue(0);
    if ( !lResult.IsOK() )
    {
        cerr << "Faile to set DefectPixelCorrection(Off)." << endl;
        return false;
    }

    PvGenEnum *lHotPixelCorrection = mDevice->GetParameters()->GetEnum( "HotPixelCorrection" );
    if ( lHotPixelCorrection == NULL )
    {
        cerr << "Can't get HotPixelCorrection Parameters." << endl;
        return false;
    }
    lResult = lHotPixelCorrection->SetValue(0);
    if ( !lResult.IsOK() )
    {
        cerr << "Faile to set HotPixelCorrection(Off)." << endl;
        return false;
    }

    PvGenEnum *lPixelFormat = mDevice->GetParameters()->GetEnum( "PixelFormat" );
    if ( lPixelFormat == NULL )
    {
        cerr << "Can't get PixelFormat Parameters." << endl;
        return false;
    }
    lResult = lPixelFormat->SetValue("BayerGR12");
    if ( !lResult.IsOK() )
    {
        cerr << "Faile to set PixelFormat(BayerGR12)." << endl;
        return false;
    }

    PvGenEnum *lAcquisitionMode = mDevice->GetParameters()->GetEnum( "AcquisitionMode" );
    if ( lAcquisitionMode == NULL )
    {
        cerr << "Can't get AcquisitionMode Parameters." << endl;
        return false;
    }
    lResult = lAcquisitionMode->SetValue("Continuous");
    if ( !lResult.IsOK() )
    {
        cerr << "Faile to set AcquisitionMode(Continuous)." << endl;
        return false;
    }

    if ( !GetFrameDimensions( lDeviceParams, mDevice ) )
    {
        return false;
    }

    // Creates stream object
    cout << "Opening stream to device" << endl;
    mStream = PvStream::CreateAndOpen( lDeviceInfo->GetConnectionID(), &lResult );
    if ( ( mStream == NULL ) || !lResult.IsOK() )
    {
        cout << "Error creating and opening stream object" << endl;
        PvStream::Free( mStream );
        PvDevice::Free( mDevice );
        return false;
    }

    // Configure streaming for GigE Vision devices
    const PvDeviceInfoGEV* lDeviceInfoGEV = dynamic_cast<const PvDeviceInfoGEV *>( lDeviceInfo );
    if ( lDeviceInfoGEV != NULL )
    {
        PvStreamGEV *lStreamGEV = static_cast<PvStreamGEV *>( mStream );
        PvDeviceGEV *lDeviceGEV = static_cast<PvDeviceGEV *>( mDevice );

        // Negotiate packet size
        lDeviceGEV->NegotiatePacketSize();

        // Configure device streaming destination
        lDeviceGEV->SetStreamDestination( lStreamGEV->GetLocalIPAddress(), lStreamGEV->GetLocalPort() );
    }

    mPvBuffers = NULL;

    // Use min of BUFFER_COUNT and how many buffers can be queued in PvStream.
    mBufferCount = ( mStream->GetQueuedBufferMaximum() < BUFFER_COUNT ) ?
        mStream->GetQueuedBufferMaximum() :
        BUFFER_COUNT;

    // Create our image buffers which are holding the real memory buffers
    mImagingBuffers = new SimpleImagingLib::ImagingBuffer[ mBufferCount ];
    for ( uint32_t i = 0; i < mBufferCount; i++ )
    {
        mImagingBuffers[ i ].AllocateImage( static_cast<uint32_t>( width ), static_cast<uint32_t>( height ), 2 );
    }

    // Creates, eBUS SDK buffers, attach out image buffer memory
    mPvBuffers = new PvBuffer[ mBufferCount ];
    for ( uint32_t i = 0; i < mBufferCount; i++ )
    {
        // Attach the memory of our imaging buffer to a PvBuffer. The PvBuffer is used as a shell
        // that allows directly acquiring image data into the memory owned by our imaging buffer
        mPvBuffers[ i ].GetImage()->Attach( mImagingBuffers[ i ].GetTopPtr(),
            static_cast<uint32_t>( width ), static_cast<uint32_t>( height ), PvPixelBayerGR12 );

        // Set eBUS SDK buffer ID to the buffer/image index
        mPvBuffers[ i ].SetID( i );
    }

    // Queue all buffers in the stream
    for ( uint32_t i = 0; i < mBufferCount; i++ )
    {
        mStream->QueueBuffer( mPvBuffers + i );
    }

    // Get stream parameters/stats.
    PvGenParameterArray *lStreamParams = mStream->GetParameters();
    mBlockCount = dynamic_cast<PvGenInteger *>( lStreamParams->Get( "BlockCount" ) );
    mFrameRate = dynamic_cast<PvGenFloat *>( lStreamParams->Get( "AcquisitionRate" ) );
    mBandwidth = dynamic_cast<PvGenFloat *>( lStreamParams->Get( "Bandwidth" ) );

    // Enables stream before sending the AcquisitionStart command.
    cout << "Enable streaming on the controller." << endl;
    mDevice->StreamEnable();

    // The buffers are queued in the stream, we just have to tell the device
    // to start sending us images.
    cout << "Sending StartAcquisition command to device" << endl;
    lStart->Execute();

    return true;
}

bool CameraBobcat::reboot_camera(){
    return true;
}

unsigned char* CameraBobcat::capture_one_frame(){
    PvBuffer *lBuffer = NULL;
    PvResult lOperationResult;
    PvBufferConverter lBufferConverter;
    lBufferConverter.SetBayerFilter(PvBayerFilter3X3);
    unsigned char * buffer = NULL;
    // Retrieve next buffer
    PvResult lResult = mStream->RetrieveBuffer( &lBuffer, &lOperationResult, 1000 );
    if ( lResult.IsOK() )
    {

        if (lOperationResult.IsOK())
        {
            if ( lBuffer->GetPayloadType() == PvPayloadTypeImage )
            {
                // Get image specific buffer interface.
                PvImage *lImage = lBuffer->GetImage();
                PvBuffer *lBufferBGR = new PvBuffer();
                PvImage *lImageBRG = lBufferBGR->GetImage();
                lImageBRG->Alloc( lImage->GetWidth(), lImage->GetHeight(), PvPixelBGR8 );
                lBufferConverter.Convert(lBuffer, lBufferBGR);
                buffer = lImageBRG->GetDataPointer();
            }
        }

        // Re-queue the buffer in the stream object.
        mStream->QueueBuffer( lBuffer );
    }
    return buffer;
}

bool CameraBobcat::is_connected(){
    return mDevice != NULL && mDevice->IsConnected();
}

int CameraBobcat::get_frame_count() {
    return frame_count;
}

int CameraBobcat::get_error_frame_count() {
    return error_frame_count;
}

double CameraBobcat::get_frame_rate() {
    double rate = 0.0;
    PvResult res;
    if (mFrameRate)
    {
        res = mFrameRate->GetValue(rate);
    }
    return rate;
}

/**
    add by Bill.xie
**/

bool CameraBobcat::set_wb_red(int red) {
    PvResult res;
    // Get GenICam parameter object
    PvGenInteger *lInt = dynamic_cast<PvGenInteger *>( mDevice->GetParameters()->Get( "RedCoefficient" ) );
    if(lInt)
        res = lInt->SetValue(red);

    return lInt && res.IsOK();
}

bool CameraBobcat::set_wb_blue(int blue) {
    PvResult res;
    // Get GenICam parameter object
    PvGenInteger *lInt = dynamic_cast<PvGenInteger *>( mDevice->GetParameters()->Get( "BlueCoefficient" ) );
    if(lInt)
        res = lInt->SetValue(blue);

    return lInt && res.IsOK();
}

bool CameraBobcat::set_wb_green(int green){
    PvResult res;
    // Get GenICam parameter object
    PvGenInteger *lInt = dynamic_cast<PvGenInteger *>( mDevice->GetParameters()->Get( "GreenCoefficient" ) );
    if(lInt)
        res = lInt->SetValue(green);

    return lInt && res.IsOK();
}
bool CameraBobcat::set_wb(int red, int green, int blue) {
    return set_wb_red(red) && set_wb_blue(blue) && set_wb_green(green);
}

bool CameraBobcat::set_shutter(float _shutter) {
    PvResult res;
    int64_t val = (int64_t)(_shutter*1000);
    // Get GenICam parameter object
    PvGenInteger *lInt = dynamic_cast<PvGenInteger *>( mDevice->GetParameters()->Get( "ExposureTimeRaw" ) );
    if(lInt)
        res = lInt->SetValue(val);

    return lInt && res.IsOK();
}

float CameraBobcat::get_shutter() {
    PvResult res;
    int64_t val = -1;
    // Get GenICam parameter object
    PvGenInteger *lInt = dynamic_cast<PvGenInteger *>( mDevice->GetParameters()->Get( "ExposureTimeRaw" ) );
    if(lInt)
        res = lInt->GetValue(val);
    if( lInt && res.IsOK() )
    {
        return val/1000.0;
    }
    else{
        return -1;
    }
}

int CameraBobcat::get_white_balance_red() {
    PvResult res;
    int64_t val = -1;
    // Get GenICam parameter object
    PvGenInteger *lInt = dynamic_cast<PvGenInteger *>( mDevice->GetParameters()->Get( "CurrentRedCoefficient" ) );
    if(lInt)
        res = lInt->GetValue(val);
    if( lInt && res.IsOK() )
    {
        return val;
    }
    else{
        return -1;
    }
}

int CameraBobcat::get_white_balance_blue() {
    PvResult res;
    int64_t val = -1;
    // Get GenICam parameter object
    PvGenInteger *lInt = dynamic_cast<PvGenInteger *>( mDevice->GetParameters()->Get( "CurrentBlueCoefficient" ) );
    if(lInt)
        res = lInt->GetValue(val);
    if( lInt && res.IsOK() )
    {
        return val;
    }
    else{
        return -1;
    }
}

int CameraBobcat::get_white_balance_green() {
    PvResult res;
    int64_t val = -1;
    // Get GenICam parameter object
    PvGenInteger *lInt = dynamic_cast<PvGenInteger *>( mDevice->GetParameters()->Get( "CurrentGreenCoefficient" ) );
    if(lInt)
        res = lInt->GetValue(val);
    if( lInt && res.IsOK() )
    {
        return val;
    }
    else{
        return -1;
    }
}

const char* CameraBobcat::get_firmware_version(){
    PvResult res;
    PvString  val;
    // Get GenICam parameter object
    PvGenString *str = dynamic_cast<PvGenString *>( mDevice->GetParameters()->Get( "DeviceVersion" ) );
    if(str)
        res = str->GetValue(val);
    if( str && res.IsOK() )
    {
        return val.GetAscii();
    }
    else{
        return NULL;
    }
}

const char* CameraBobcat::get_camera_id(){
    PvResult res;
    PvString  val;
    // Get GenICam parameter object
    PvGenString *str = dynamic_cast<PvGenString *>( mDevice->GetParameters()->Get( "DeviceID" ) );
    if(str)
        res = str->GetValue(val);
    if( str && res.IsOK() )
    {
        return val.GetAscii();
    }
    else{
        return NULL;
    }
}


float CameraBobcat::get_camera_temperature(){
    PvResult res;
    int64_t val = -1;
    // Get GenICam parameter object
    PvGenInteger *lInt = dynamic_cast<PvGenInteger *>( mDevice->GetParameters()->Get( "CurrentTemperature" ) );
    if(lInt)
        res = lInt->GetValue(val);
    if( lInt && res.IsOK() )
    {
        return (float)val;
    }
    else{
        return -1;
    }
}

bool CameraBobcat::store_configuration(const char* file_path)
{
    if (mDevice == NULL || mStream == NULL)
    {
        return false;
    }

    const PvDeviceInfo *lDeviceInfo = NULL;
    PvResult lResult = PvResult::Code::INVALID_PARAMETER;
    PvConfigurationWriter lWriter;

    lWriter.Store(mDevice, DEVICE_CONFIGURATION_TAG);
    lWriter.Store(mStream, STREAM_CONFIGURAITON_TAG);
    lResult = lWriter.Save(file_path);
    if ( !lResult.IsOK() )
    {
        cout<<"Can't save configuration file."<<endl;
        return false;
    }

    return true;
}

bool CameraBobcat::restore_configuration(const char* file_path)
{
    if (mDevice == NULL || !mDevice->IsConnected())
    {
        return false;
    }

    PvConfigurationReader lReader;
    lReader.Load(file_path);
    PvResult lResult = lReader.Restore(DEVICE_CONFIGURATION_TAG, mDevice);
    if ( !lResult.IsOK() )
    {
        cout<<"Can't restore the device configuration."<<endl;
        return false;
    }

    lResult = lReader.Restore( STREAM_CONFIGURAITON_TAG, mStream );
    if ( !lResult.IsOK() )
    {
        cout<<"Can't restore the stream configuration."<<endl;
        return false;
    }

    return true;
}
