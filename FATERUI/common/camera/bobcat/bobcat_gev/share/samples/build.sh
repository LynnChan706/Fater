
SAMPLES=" \
  eBUSPlayer \
  DualSource \
  ConfigurationReader \
  ConnectionRecovery \
  DeviceFinder \
  GenICamParameters \
  MulticastMaster \
  MulticastSlave \
  MultiSource \
  PvPipelineSample \
  PvStreamSample \
  DeviceSerialPort \
  TransmitTestPattern \
  TransmitProcessedImage \
  ImageProcessing \
  IGVExposureUISample \
  IGVGainUISample \
  IGVTriggerUISample \
  IGVTrueSenseUISample
"

for SAMPLE in $SAMPLES; do
  make -C $SAMPLE
done

