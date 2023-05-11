#ifndef P1CAM_H
#define P1CAM_H

#include <string>
#include <vector>

namespace P1Cam {

enum BayerPattern   ///< Bayer Pattern Definition
{
    BAYER_RG = 0,       ///< RGGB
    BAYER_BG,           ///< BGGR
    BAYER_GR,           ///< GRBG
    BAYER_GB,           ///< GBRG
    BAYER_MONO = -1     ///< Monochrome, the mono camera with this
};


enum ImgFormat      ///< Image Data Format Definition
{
    RAW8 = 0,           ///< 8bit raw data, 1 pixel 1 byte, value range[0, 255]
    RAW16,              ///< 16bit raw data, 1 pixel 2 bytes, value range[0, 65535]
    RGB24,              ///< RGB888 color data, 1 pixel 3 bytes, value range[0, 255] (only color camera)
    MONO8               ///< 8bit monochrome data, convert the Bayer Filter Array to monochrome data. 1 pixel 1 byte, value range[0, 255] (only color camera)
};


enum Errors              ///< Return Error Code Definition
{
    OK = 0,                         ///< operation successful
    ERROR_INVALID_INDEX,            ///< invalid index, means the index is < 0 or >= the count( camera or config)
    ERROR_INVALID_ID,               ///< invalid camera ID
    ERROR_INVALID_CONFIG,           ///< invalid camera config
    ERROR_INVALID_ARGU,             ///< invalid argument(parameter)
    ERROR_NOT_OPENED,               ///< camera not opened
    ERROR_DEVICE_NOT_FOUND,         ///< camera not found, may be removed
    ERROR_OUT_OF_LIMIT,             ///< the value out of limit
    ERROR_EXPOSURE_FAILED,          ///< camera exposure failed
    ERROR_TIMEOUT,                  ///< timeout
    ERROR_SIZE_LESS,                ///< the data buffer size is not enough
    ERROR_EXPOSING,                 ///< camera is exposing. some operation, must stop exposure first
    ERROR_POINTER,                  ///< invalid pointer, when get some value, do not pass the NULL pointer to the function
    ERROR_CONF_CANNOT_WRITE,        ///< the camera config is not writable
    ERROR_CONF_CANNOT_READ,         ///< the camera config is not readable
    ERROR_ACCESS_DENIED,            ///< access denied
    ERROR_OPERATION_FAILED,         ///< operation failed, maybe the camera is disconnected suddenly
    ERROR_MEMORY_FAILED             ///< memory allocation failed
};


enum CameraState        ///< Camera State Definition
{
    STATE_CLOSED = 0,       ///< camera was closed
    STATE_OPENED,           ///< camera was opened, but not exposing
    STATE_EXPOSING          ///< camera is exposing
};


enum GuideDirection ///< Guide Direction Definition
{
    GUIDE_NORTH = 0,          ///< ST4 guide north, generally,it's DEC+ on the mount
    GUIDE_SOUTH,              ///< ST4 guide south, generally,it's DEC- on the mount
    GUIDE_EAST,               ///< ST4 guide east, generally,it's RA+ on the mount
    GUIDE_WEST                ///< ST4 guide west, generally,it's RA- on the mount
};


enum CameraConfigs              ///< Camera Config Definition
{
    EXPOSURE = 0,                   ///< exposure time(unit: us), read-write, value type is long
    GAIN = 1,                       ///< gain, read-write, value type is long
    HARDWARE_BIN = 2,               ///< hardware bin, read-write, value type is bool
    WB_R = 4,                       ///< red pixels coefficient of white balance, read-write, value type is long
    WB_G = 5,                       ///< green pixels coefficient of white balance, read-write, value type is long
    WB_B = 6,                       ///< blue pixels coefficient of white balance, read-write, value type is long
    OFFSET = 7,                     ///< camera offset, read-write, value type is long
    AUTOEXPO_MAX_GAIN = 8,          ///< maximum gain when auto-adjust, read-write, value type is long
    AUTOEXPO_MAX_EXPOSURE = 9,      ///< maximum exposure when auto-adjust(uint: ms), read-write, value type is long
    AUTOEXPO_BRIGHTNESS = 10,       ///< target brightness when auto-adjust, read-write, value type is long
    COOLER_POWER = 16,              ///< cooler power percentage[0-100%](only cool camera), read-only, value type is long
    TARGET_TEMP = 17,               ///< camera target temperature(uint: C), read-write, value type is long
    COOLER_ON = 18,                 ///< turn cooler(and fan) on or off, read-write, value type is bool
    HEATER_ON = 19,                 ///< (deprecated)get state of lens heater(on or off), read-only, value type is bool
    HEATER_POWER = 20,              ///< lens heater power percentage[0-100%], read-write, value type is long
    FAN_POWER = 21,                 ///< radiator fan power percentage[0-100%], read-write, value type is long
    FRAME_LIMIT = 26,               ///< frame rate limit, the range:[0, 2000], 0 means no limit, read-write, value type is long
    HQI = 27,                       ///< High Quality Image, for those without DDR camera(guide camera), if set to 'true', this will reduce the waviness and stripe of the image,
                                    ///< but frame rate may go down, note: this config has no effect on those cameras that with DDR. read-write, value type is bool
    USB_BANDWIDTH_LIMIT = 28,       ///< USB bandwidth limit, read-write, value type is long
    PIXEL_BIN_SUM = 29,             ///< take the sum of pixels after binning, 'true' is sum and 'false' is average, default is false, read-write, value type is bool
};


struct CameraProperties         ///< Camera Properties Definition
{
    int cameraID;                       ///< it's unique,camera can be controlled and set by the cameraID
    std::string cameraName;             ///< the camera name
    std::string userCustomName;         ///< user custom name, it will be will be added after the camera name, max len 16 bytes,like:Mars-C [Juno], default is empty
    int pID;                            ///< product ID
    int maxWidth;                       ///< max width of the image
    int maxHeight;                      ///< max height of the image
    int bitDepth;                       ///< ADC depth of CMOS sensor
    bool isColorCamera;                 ///< is a color camera or not
    bool isHasST4Port;                  ///< does the camera have ST4 port, if not, camera don't support ST4 guide
    bool isHasCooler;                   ///< does the camera have cooler, the cooled camera with cooler
    bool isUSB3Speed;                   ///< is usb3.0 speed
    bool isSupportHardBin;              ///< whether the camera supports hardware bin
    BayerPattern bayerPattern;          ///< the bayer filter pattern of camera
    double pixelSize;                   ///< camera pixel size(unit: um)
    std::string SN;                     ///< the serial number of camera,it's unique
    std::string sensorModelName;        ///< the sersor model(name) of camera, eg: IMX462
    std::string localPath;              ///< the path of the camera in the computer host
    std::vector<int> bins;              ///< bins supported by the camera, 1 == bin1, 2 == bin2,...
    std::vector<ImgFormat> imgFormats;  ///< image data format supported by the camera
};


struct ConfigsAttributes            ///< Camera Configs Attributes Definition(every CameraConfigs has a ConfigsAttributes)
{
    bool isSupportAuto;                 ///< is support auto?
    bool isWritable;                    ///< is writable?
    bool isReadable;                    ///< is readable?
    CameraConfigs configID;             ///< config ID, eg: EXPOSURE
    long maxValue;                      ///< maximum value
    long minValue;                      ///< minimum value
    long defaultValue;                  ///< default value
    std::string confName;               ///< POAConfig name, eg: POA_EXPOSURE: "Exposure", POA_TARGET_TEMP: "TargetTemp"
    std::string confDescription;        ///< a brief description about this one POAConfig
};


struct SensorModeInfo    ///< Sensor mode information
{
    int modeID;             ///< sensor mode ID
    std::string name;       ///< sensor mode name, can be used to display on the UI (eg: Combobox)
    std::string desc;      ///< sensor mode description, may be useful for tooltip
};


struct GainsAndOffsets ///< Some preset special gains and offsets
{
    int gainHighestDR;      ///< gain at highest dynamic range, in most cases, this gain is 0
    int HCGain;             ///< gain at HCG Mode(High Conversion Gain)
    int unityGain;          ///< unity gain(or standard gain), with this gain, eGain(e/ADU) will be 1
    int gainLowestRN;       ///< aka Maximum Analog Gain, gain at lowest read noise
    int offsetHighestDR;    ///< offset at highest dynamic range
    int offsetHCGain;       ///< offset at HCG Mode
    int offsetUnityGain;    ///< offset at unity gain
    int offsetLowestRN;     ///< offset at lowest read noise
};


/*****************************************Functions****************************************************/

void GetAllCameraProperties (std::vector<CameraProperties> &allCamProps);

Errors GetCameraPropertiesByID(int cameraID, CameraProperties &camProp);


Errors OpenCamera(int cameraID);

Errors CloseCamera(int cameraID);


Errors GetCameraTemp(int cameraID, double &temp);


Errors GetCameraAllConfigs(int cameraID, std::vector<CameraConfigs> &allCamCfgs);

Errors GetConfigAttributes(int cameraID, CameraConfigs cfgID, ConfigsAttributes &cfgAttr);


Errors GetConfig(int cameraID, CameraConfigs cfgID, long &cfgValue, bool &isAuto);

Errors SetConfig(int cameraID, CameraConfigs cfgID, long cfgValue, bool isAuto);


Errors GetImageStartPos(int cameraID, int &startX, int &startY);

Errors SetImageStartPos(int cameraID, int startX, int startY);


Errors GetImageSize(int cameraID, int &imgWidth, int &imgHeight);

Errors SetImageSize(int cameraID, int imgWidth, int imgHeight);


Errors GetImageBin(int cameraID, int &bin);

Errors SetImageBin(int cameraID, int bin);


Errors GetImageFormat(int cameraID, ImgFormat &imgFormat);

Errors SetImageFormat(int cameraID, ImgFormat imgFormat);


Errors GetImageFlip(int cameraID, bool &isFlipHori, bool &isFlipVert);

Errors SetImageFlip(int cameraID, bool isFlipHori, bool isFlipVert);


Errors StartExposure(int cameraID, bool isSingleFrame);

Errors StopExposure(int cameraID);


Errors GetCameraState(int cameraID, CameraState &cameraState);

Errors ImageDataReady(int cameraID, bool &isReady);

Errors GetImageData(int cameraID, unsigned char *pBuf, long bufSize, int timeout_ms);

Errors GetDroppedImagesCount(int cameraID, int &droppedCount);


Errors SetGuideST4(int cameraID, GuideDirection guideDirection, bool OnOff);


Errors GetAllSensorModeInfo(int cameraID, std::vector<SensorModeInfo> &allSenInfo);

Errors GetSensorMode(int cameraID, int &modeID);

Errors SetSensorMode(int cameraID, int modeID);


Errors GetCameraEGain(int cameraID, double &eGain);

Errors GetGainsAndOffsets(int cameraID, GainsAndOffsets &gainsOffsets);

Errors SetUserCustomName(int cameraID, const std::string &customName);

std::string GetErrorString(Errors err);


int GetAPIVersion();

std::string GetSDKVersion();

} // namespace P1Cam


#endif // P1CAM_H
