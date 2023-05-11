#include "P1Cam.h"

#include "PlayerOneCamera.h"

void POACamPropToCamProp(const POACameraProperties &PCamProp, P1Cam::CameraProperties &camProp)
{
    camProp.cameraID = PCamProp.cameraID;
    camProp.cameraName = std::string(PCamProp.cameraModelName);
    camProp.userCustomName = std::string(PCamProp.userCustomID);
    camProp.pID = PCamProp.pID;
    camProp.maxWidth = PCamProp.maxWidth;
    camProp.maxHeight = PCamProp.maxHeight;
    camProp.bitDepth = PCamProp.bitDepth;
    camProp.isColorCamera = (PCamProp.isColorCamera == POA_TRUE); //if not(xx == POA_TRUE), performmance warning???
    camProp.isHasST4Port = (PCamProp.isHasST4Port == POA_TRUE);
    camProp.isHasCooler = (PCamProp.isHasCooler == POA_TRUE);
    camProp.isUSB3Speed = (PCamProp.isUSB3Speed == POA_TRUE);
    camProp.isSupportHardBin = (PCamProp.isSupportHardBin == POA_TRUE);
    camProp.bayerPattern = static_cast<P1Cam::BayerPattern>(PCamProp.bayerPattern);
    camProp.pixelSize = PCamProp.pixelSize;
    camProp.SN = std::string(PCamProp.SN);
    camProp.sensorModelName = std::string(PCamProp.sensorModelName);
    camProp.localPath = std::string(PCamProp.localPath);

    camProp.bins.clear();
    for(int i = 0; i < 8; i++)
    {
        if(PCamProp.bins[i] == 0)
        { break; }

        camProp.bins.push_back(PCamProp.bins[i]);
    }

    camProp.imgFormats.clear();
    for(int i = 0; i < 8; i++)
    {
        if(PCamProp.imgFormats[i] == POA_END)
        { break; }

        camProp.imgFormats.push_back(static_cast<P1Cam::ImgFormat>(PCamProp.imgFormats[i]));
    }
}

void P1Cam::GetAllCameraProperties(std::vector<CameraProperties> &allCamProps)
{
    allCamProps.clear();

    int cameraCount = POAGetCameraCount();
    POACameraProperties PCamProp;
    for(int ii = 0; ii < cameraCount; ii++)
    {
        POAErrors error = POAGetCameraProperties(ii, &PCamProp);
        if(error != POA_OK)
        { continue; }

        CameraProperties camProp;

        POACamPropToCamProp(PCamProp, camProp);

        allCamProps.push_back(camProp);
    }
}

P1Cam::Errors P1Cam::GetCameraPropertiesByID(int cameraID, CameraProperties &camProp)
{
    POACameraProperties PCamProp;

    POAErrors error = POAGetCameraPropertiesByID(cameraID, &PCamProp);

    if(error != POA_OK)
    {
        return static_cast<Errors>(error);
    }

    POACamPropToCamProp(PCamProp, camProp);

    return OK;
}

P1Cam::Errors P1Cam::OpenCamera(int cameraID)
{
    POAErrors error = POAOpenCamera(cameraID);

    if(error != POA_OK)
    {
        return static_cast<Errors>(error);
    }

    error = POAInitCamera(cameraID);

    return static_cast<Errors>(error);
}

P1Cam::Errors P1Cam::CloseCamera(int cameraID)
{
    POAErrors error = POACloseCamera(cameraID);

    return static_cast<Errors>(error);
}

P1Cam::Errors P1Cam::GetCameraTemp(int cameraID, double &temp)
{
    POAConfigValue PCfgVal;
    POABool isAuto;
    POAErrors error =  POAGetConfig(cameraID, POA_TEMPERATURE, &PCfgVal, &isAuto);

    if(error != POA_OK)
    {
        return static_cast<Errors>(error);
    }

    temp = PCfgVal.floatValue;

    return OK;
}

P1Cam::Errors P1Cam::GetCameraAllConfigs(int cameraID, std::vector<CameraConfigs> &allCamCfgs)
{
    int cfgCount = 0;
    POAErrors error = POAGetConfigsCount(cameraID, &cfgCount);

    if(error != POA_OK)
    {
        return static_cast<Errors>(error);
    }

    allCamCfgs.clear();

    POAConfigAttributes cfgAttr;
    for(int i = 0; i < cfgCount; i++)
    {
        POAGetConfigAttributes(cameraID, i, &cfgAttr);

        if(cfgAttr.configID == POA_GUIDE_NORTH ||
           cfgAttr.configID == POA_GUIDE_SOUTH ||
           cfgAttr.configID == POA_GUIDE_EAST  ||
           cfgAttr.configID == POA_GUIDE_WEST  ||
           cfgAttr.configID == POA_FLIP_NONE   ||
           cfgAttr.configID == POA_FLIP_HORI   ||
           cfgAttr.configID == POA_FLIP_VERT   ||
           cfgAttr.configID == POA_FLIP_BOTH   ||
           cfgAttr.configID == POA_TEMPERATURE ||
           cfgAttr.configID == POA_EGAIN)
        {
            continue;
        }

        allCamCfgs.push_back(static_cast<CameraConfigs>(cfgAttr.configID));
    }

    return OK;
}

P1Cam::Errors P1Cam::GetConfigAttributes(int cameraID, CameraConfigs cfgID, ConfigsAttributes &cfgAttr)
{
    POAConfig PCfgID = static_cast<POAConfig>(cfgID); //CameraConfigs is a subset of POAConfig, CameraConfigs cast to POAConfig is OK

    POAConfigAttributes PCfgAttr;
    POAErrors error = POAGetConfigAttributesByConfigID(cameraID, PCfgID, &PCfgAttr);

    if(error != POA_OK)
    {
        return static_cast<Errors>(error);
    }

    cfgAttr.configID = cfgID;
    cfgAttr.isSupportAuto = (PCfgAttr.isSupportAuto == POA_TRUE);
    cfgAttr.isWritable = (PCfgAttr.isWritable == POA_TRUE);
    cfgAttr.isReadable = (PCfgAttr.isReadable == POA_TRUE);
    cfgAttr.maxValue = PCfgAttr.maxValue.intValue;
    cfgAttr.minValue = PCfgAttr.minValue.intValue;
    cfgAttr.defaultValue = PCfgAttr.defaultValue.intValue;
    cfgAttr.confName = std::string(PCfgAttr.szConfName);
    cfgAttr.confDescription = std::string(PCfgAttr.szDescription);

    return OK;
}

P1Cam::Errors P1Cam::GetConfig(int cameraID, CameraConfigs cfgID, long &cfgValue, bool &isAuto)
{
    POAConfig PCfgID = static_cast<POAConfig>(cfgID);

    POABool PIsAuto = POA_FALSE;
    POAConfigValue PCfgVal;
    POAErrors error = POAGetConfig(cameraID, PCfgID, &PCfgVal, &PIsAuto);

    if(error != POA_OK)
    {
        return static_cast<Errors>(error);
    }
    cfgValue = PCfgVal.intValue;
    isAuto = (PIsAuto == POA_TRUE);

    return OK;
}

P1Cam::Errors P1Cam::SetConfig(int cameraID, CameraConfigs cfgID, long cfgValue, bool isAuto)
{
    POAConfig PCfgID = static_cast<POAConfig>(cfgID);

    POAConfigValue PCfgVal;
    PCfgVal.intValue = cfgValue;

    POAErrors error = POASetConfig(cameraID, PCfgID, PCfgVal, isAuto ? POA_TRUE : POA_FALSE);

    return static_cast<Errors>(error);
}

P1Cam::Errors P1Cam::GetImageStartPos(int cameraID, int &startX, int &startY)
{
    POAErrors error = POAGetImageStartPos(cameraID, &startX, &startY);

    return static_cast<Errors>(error);
}

P1Cam::Errors P1Cam::SetImageStartPos(int cameraID, int startX, int startY)
{
    POAErrors error = POASetImageStartPos(cameraID, startX, startY);

    return static_cast<Errors>(error);
}

P1Cam::Errors P1Cam::GetImageSize(int cameraID, int &imgWidth, int &imgHeight)
{
    POAErrors error = POAGetImageSize(cameraID, &imgWidth, &imgHeight);

    return static_cast<Errors>(error);
}

P1Cam::Errors P1Cam::SetImageSize(int cameraID, int imgWidth, int imgHeight)
{
    POAErrors error = POASetImageSize(cameraID, imgWidth, imgHeight);

    return static_cast<Errors>(error);
}

P1Cam::Errors P1Cam::GetImageBin(int cameraID, int &bin)
{
    POAErrors error = POAGetImageBin(cameraID, &bin);

    return static_cast<Errors>(error);
}

P1Cam::Errors P1Cam::SetImageBin(int cameraID, int bin)
{
    POAErrors error = POASetImageBin(cameraID, bin);

    return static_cast<Errors>(error);
}

P1Cam::Errors P1Cam::GetImageFormat(int cameraID, ImgFormat &imgFormat)
{
    POAImgFormat PImgFmt;
    POAErrors error = POAGetImageFormat(cameraID, &PImgFmt);

    if(error != POA_OK)
    {
        return static_cast<Errors>(error);
    }

    imgFormat = static_cast<ImgFormat>(PImgFmt);

    return OK;
}

P1Cam::Errors P1Cam::SetImageFormat(int cameraID, ImgFormat imgFormat)
{
    POAImgFormat PImgFmt = static_cast<POAImgFormat>(imgFormat);

    POAErrors error = POASetImageFormat(cameraID, PImgFmt);

    return static_cast<Errors>(error);
}

P1Cam::Errors P1Cam::GetImageFlip(int cameraID, bool &isFlipHori, bool &isFlipVert)
{
    POAConfigValue confValue;
    POABool boolValue;
    POAErrors error = POAGetConfig(cameraID, POA_FLIP_BOTH, &confValue, &boolValue);

    if(error != POA_OK)
    { return static_cast<Errors>(error); }

    if(confValue.boolValue) //is both flip
    {
        isFlipHori = true;
        isFlipVert = true;

        return OK;
    }

    confValue.boolValue = POA_FALSE;
    error = POAGetConfig(cameraID, POA_FLIP_VERT, &confValue, &boolValue);

    if(error != POA_OK)
    { return static_cast<Errors>(error); }

    if(confValue.boolValue) // is vert flip
    {
        isFlipHori = false;
        isFlipVert = true;

        return OK;
    }

    confValue.boolValue = POA_FALSE;
    error = POAGetConfig(cameraID, POA_FLIP_HORI, &confValue, &boolValue);

    if(error != POA_OK)
    { return static_cast<Errors>(error); }

    if(confValue.boolValue) // is hori flip
    {
        isFlipHori = true;
        isFlipVert = false;

        return OK;
    }

    isFlipHori = false;
    isFlipVert = false;

    return OK;
}

P1Cam::Errors P1Cam::SetImageFlip(int cameraID, bool isFlipHori, bool isFlipVert)
{
    POAConfig confID;
    if(isFlipHori && isFlipVert) //both flip
    {
        confID = POA_FLIP_BOTH;
    }
    else if(!isFlipHori && isFlipVert) //vert flip
    {
        confID = POA_FLIP_VERT;
    }
    else if(isFlipHori && !isFlipVert) //hori flip
    {
        confID = POA_FLIP_HORI;
    }
    else                              //none flip
    {
        confID = POA_FLIP_NONE;
    }

    POAConfigValue confValue;
    confValue.boolValue = POA_TRUE;
    POAErrors error = POASetConfig(cameraID, confID, confValue, POA_FALSE);

    return static_cast<Errors>(error);
}

P1Cam::Errors P1Cam::StartExposure(int cameraID, bool isSingleFrame)
{
    POAErrors error = POAStartExposure(cameraID, isSingleFrame ? POA_TRUE : POA_FALSE);

    return static_cast<Errors>(error);
}

P1Cam::Errors P1Cam::StopExposure(int cameraID)
{
    POAErrors error = POAStopExposure(cameraID);

    return static_cast<Errors>(error);
}

P1Cam::Errors P1Cam::GetCameraState(int cameraID, CameraState &cameraState)
{
    POACameraState PCamState;
    POAErrors error = POAGetCameraState(cameraID, &PCamState);

    if(error != POA_OK)
    {
        return static_cast<Errors>(error);
    }

    cameraState = static_cast<CameraState>(PCamState);

    return OK;
}

P1Cam::Errors P1Cam::ImageDataReady(int cameraID, bool &isReady)
{
    POABool PIsReady = POA_FALSE;
    POAErrors error = POAImageReady(cameraID, &PIsReady);

    if(error != POA_OK)
    {
        return static_cast<Errors>(error);
    }

    isReady = (PIsReady == POA_TRUE);

    return OK;
}

P1Cam::Errors P1Cam::GetImageData(int cameraID, unsigned char *pBuf, long bufSize, int timeout_ms)
{
    POAErrors error = POAGetImageData(cameraID, pBuf, bufSize, timeout_ms);

    return static_cast<Errors>(error);
}

P1Cam::Errors P1Cam::GetDroppedImagesCount(int cameraID, int &droppedCount)
{
    POAErrors error = POAGetDroppedImagesCount(cameraID, &droppedCount);

    return static_cast<Errors>(error);
}

P1Cam::Errors P1Cam::SetGuideST4(int cameraID, GuideDirection guideDirection, bool OnOff)
{
    POAConfig confID;
    switch (guideDirection)
    {
    case GUIDE_NORTH:
        confID = POA_GUIDE_NORTH;
        break;
    case GUIDE_SOUTH:
        confID = POA_GUIDE_SOUTH;
        break;
    case GUIDE_EAST:
        confID = POA_GUIDE_EAST;
        break;
    case GUIDE_WEST:
        confID = POA_GUIDE_WEST;
        break;
    default:
        return ERROR_INVALID_ARGU;
    }

    POAConfigValue confValue;
    confValue.boolValue = OnOff ? POA_TRUE : POA_FALSE;
    POAErrors error = POASetConfig(cameraID, confID, confValue, POA_FALSE);

    return static_cast<Errors>(error);
}

P1Cam::Errors P1Cam::GetAllSensorModeInfo(int cameraID, std::vector<SensorModeInfo> &allSenInfo)
{
    int senModeCount;

    POAErrors error = POAGetSensorModeCount(cameraID, &senModeCount);

    if(error != POA_OK)
    {
        return static_cast<Errors>(error);
    }

    allSenInfo.clear();

    if(senModeCount == 0) //don't support sensor mode selection
    { return OK; }

    POASensorModeInfo PSenModeInfo;
    for(int i = 0; i < senModeCount; i++)
    {
        POAGetSensorModeInfo(cameraID, i, &PSenModeInfo);

        SensorModeInfo senModeInfo;
        senModeInfo.modeID = i;
        senModeInfo.name = std::string(PSenModeInfo.name);
        senModeInfo.desc = std::string(PSenModeInfo.desc);

        allSenInfo.push_back(senModeInfo);
    }

    return OK;
}

P1Cam::Errors P1Cam::GetSensorMode(int cameraID, int &modeID)
{
    POAErrors error = POAGetSensorMode(cameraID, &modeID);

    return static_cast<Errors>(error);
}

P1Cam::Errors P1Cam::SetSensorMode(int cameraID, int modeID)
{
    POAErrors error = POASetSensorMode(cameraID, modeID);

    return static_cast<Errors>(error);
}

P1Cam::Errors P1Cam::GetCameraEGain(int cameraID, double &eGain)
{
    POAConfigValue PCfgVal;
    POABool isAuto;
    POAErrors error =  POAGetConfig(cameraID, POA_EGAIN, &PCfgVal, &isAuto);

    if(error != POA_OK)
    {
        return static_cast<Errors>(error);
    }

    eGain = PCfgVal.floatValue;

    return OK;
}

P1Cam::Errors P1Cam::GetGainsAndOffsets(int cameraID, GainsAndOffsets &gainsOffsets)
{
    POAErrors error = POAGetGainsAndOffsets(cameraID, &gainsOffsets.gainHighestDR, &gainsOffsets.HCGain, &gainsOffsets.unityGain, &gainsOffsets.gainLowestRN,
                                            &gainsOffsets.offsetHighestDR, &gainsOffsets.offsetHCGain, &gainsOffsets.offsetUnityGain, &gainsOffsets.offsetLowestRN);

    return static_cast<Errors>(error);
}

P1Cam::Errors P1Cam::SetUserCustomName(int cameraID, const std::string &customName)
{
    POAErrors error = POASetUserCustomID(cameraID, customName.c_str(), customName.length());

    return static_cast<Errors>(error);
}

std::string P1Cam::GetErrorString(Errors err)
{
    return std::string(POAGetErrorString(static_cast<POAErrors>(err)));
}

int P1Cam::GetAPIVersion()
{
    return POAGetAPIVersion();;
}

std::string P1Cam::GetSDKVersion()
{
    return std::string(POAGetSDKVersion());
}



/*
class Matrix
{
public:
    Matrix() = default;

    Matrix(int rows, int cols, int pixel_byte, int channels = 1) : m_rows(rows), m_cols(cols), m_channels(channels) 
    {
        m_size = (std::size_t)rows * cols * pixel_byte * channels;

        m_data = new unsigned char[m_size]; 
    }

    ~Matrix()
    {
        if(m_data)
        {
            delete [] m_data;
            m_data = nullptr;
        }
    }
public:
    void set_ndim(int rows, int cols, int pixel_byte, int channels = 1)
    {
        std::size_t buf_size = (std::size_t)rows * cols * pixel_byte * channels;
        bool is_realloc_buf = true;

        if(m_data)
        {
            if(buf_size == m_size)
            { 
                std::memset(m_data, 0, buf_size);
                is_realloc_buf = false; 
            }
            else
            {
                delete [] m_data;
                m_data = nullptr;
            }
        }   

        if(is_realloc_buf)
        {
            m_size = buf_size;
            m_data = new unsigned char[m_size];
        }
    }
    unsigned char *data() { return m_data; }
    int rows() const { return m_rows; }
    int cols() const { return m_cols; }
    int pixel_byte() const { return m_pixel_byte; }
    int channels() const { return m_channels; }
    std::size_t size(){ return m_size; }
private:
    int m_rows = 0, m_cols = 0;
    int m_channels = 0;
    int m_pixel_byte = 0;
    std::size_t m_size = 0;
    unsigned char *m_data = nullptr;
};
*/