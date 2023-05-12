#include <string>
#include <vector>
#include <iostream>


#include "pybind11/pybind11.h"
#include "pybind11/stl.h"
#include "pybind11/stl_bind.h"
#include "pybind11/numpy.h"

#include "P1Cam.h"

using namespace P1Cam;

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

// https://pybind11.readthedocs.io/en/stable/advanced/cast/overview.html
// https://pybind11.readthedocs.io/en/stable/advanced/cast/stl.html#opaque 
PYBIND11_MAKE_OPAQUE(std::vector<CameraProperties>); 
PYBIND11_MAKE_OPAQUE(std::vector<CameraConfigs>);
PYBIND11_MAKE_OPAQUE(std::vector<SensorModeInfo>);

namespace py = pybind11;

PYBIND11_MODULE(player_one_camera, m) {
    m.doc() = R"pbdoc(
        Player One Camera SDK(python)
        -----------------------------

        .. currentmodule:: player_one_camera

        .. autosummary::
           :toctree: _generate

           BayerPattern
           ImgFormat
           Errors
           CameraState
           GuideDirection
           CameraConfigs
           CameraProperties
           ConfigsAttributes
           SensorModeInfo
           GainsAndOffsets
           ListCamProp
           ListCamCfg
           ListSenModeInfo
           GetAllCameraProperties
           GetCameraPropertiesByID
           OpenCamera
           CloseCamera
           GetCameraTemp
           GetCameraAllConfigs
           GetConfigAttributes
           GetConfig
           SetConfig
           GetImageStartPos
           SetImageStartPos
           GetImageSize
           SetImageSize
           GetImageBin
           SetImageBin
           GetImageFormat
           SetImageFormat
           GetImageFlip
           SetImageFlip
           StartExposure
           StopExposure
           GetCameraState
           ImageDataReady
           GetImageData
           GetDroppedImagesCount
           SetGuideST4
           GetAllSensorModeInfo
           GetSensorMode
           SetSensorMode
           GetCameraEGain
           GetGainsAndOffsets
           SetUserCustomName
           GetErrorString
           GetAPIVersion
           GetSDKVersion
    )pbdoc";

    py::bind_vector<std::vector<CameraProperties>>(m, "ListCamProp"); //, pybind11::module_local(false)
    py::bind_vector<std::vector<CameraConfigs>>(m, "ListCamCfg");
    py::bind_vector<std::vector<SensorModeInfo>>(m, "ListSenModeInfo");

    py::enum_<BayerPattern>(m, "BayerPattern", py::arithmetic(), "Bayer Pattern Definition")
        .value("BAYER_RG", BAYER_RG, "RGGB")
        .value("BAYER_BG", BAYER_BG, "BGGR")
        .value("BAYER_GR", BAYER_GR, "GRBG")
        .value("BAYER_GB", BAYER_GB, "GBRG")
        .value("BAYER_MONO", BAYER_MONO, "Monochrome, the mono camera with this")
        .export_values();
  
    py::enum_<ImgFormat>(m, "ImgFormat", py::arithmetic(), "Image Data Format Definition")
        .value("RAW8", RAW8, "8bit RAW data")
        .value("RAW16", RAW16, "16bit RAW data")
        .value("RGB24", RGB24, "BGR888 color data")
        .value("MONO8", MONO8, "8bit monochrome data")
        .export_values();

    py::enum_<Errors>(m, "Errors", py::arithmetic(), "Return Error Code Definition")
        .value("OK", OK, "operation successful")
        .value("ERROR_INVALID_INDEX", ERROR_INVALID_INDEX, "invalid index")
        .value("ERROR_INVALID_ID", ERROR_INVALID_ID, "invalid camera ID")
        .value("ERROR_INVALID_CONFIG", ERROR_INVALID_CONFIG, "invalid camera config")
        .value("ERROR_INVALID_ARGU", ERROR_INVALID_ARGU, "invalid argument")
        .value("ERROR_NOT_OPENED", ERROR_NOT_OPENED, "camera not opened")
        .value("ERROR_DEVICE_NOT_FOUND", ERROR_DEVICE_NOT_FOUND, "camera not found")
        .value("ERROR_OUT_OF_LIMIT", ERROR_OUT_OF_LIMIT, "the value out of limit")
        .value("ERROR_EXPOSURE_FAILED", ERROR_EXPOSURE_FAILED, "camera exposure failed")
        .value("ERROR_TIMEOUT", ERROR_TIMEOUT, "timeout")
        .value("ERROR_SIZE_LESS", ERROR_SIZE_LESS, "the data buffer size is not enough")
        .value("ERROR_EXPOSING", ERROR_EXPOSING, "camera is exposing")
        .value("ERROR_POINTER", ERROR_POINTER, "invalid pointer(NULL pointer)")
        .value("ERROR_CONF_CANNOT_WRITE", ERROR_CONF_CANNOT_WRITE, "the camera config is not writable")
        .value("ERROR_CONF_CANNOT_READ", ERROR_CONF_CANNOT_READ, "the camera config is not readable")
        .value("ERROR_ACCESS_DENIED", ERROR_ACCESS_DENIED, "access denied")
        .value("ERROR_OPERATION_FAILED", ERROR_OPERATION_FAILED, "operation failed")
        .value("ERROR_MEMORY_FAILED", ERROR_MEMORY_FAILED, "memory allocation failed")
        .export_values();
        
    py::enum_<CameraState>(m, "CameraState", py::arithmetic(), "Camera State Definition")
        .value("STATE_CLOSED", STATE_CLOSED, "camera was closed")
        .value("STATE_OPENED", STATE_OPENED, "camera was opened, but not exposing")
        .value("STATE_EXPOSING", STATE_EXPOSING, "camera is exposing")
        .export_values();
        
    py::enum_<GuideDirection>(m, "GuideDirection", py::arithmetic(), "Guide Direction Definition")
        .value("GUIDE_NORTH", GUIDE_NORTH, "ST4 guide north")
        .value("GUIDE_SOUTH", GUIDE_SOUTH, "ST4 guide south")
        .value("GUIDE_EAST", GUIDE_EAST, "ST4 guide east")
        .value("GUIDE_WEST", GUIDE_WEST, "ST4 guide west")
        .export_values();

    py::enum_<CameraConfigs>(m, "CameraConfigs", py::arithmetic(), "Camera Config Definition")
        .value("EXPOSURE", EXPOSURE, "exposure time(unit: us)")
        .value("GAIN", GAIN, "gain")
        .value("HARDWARE_BIN", HARDWARE_BIN, "hardware bin")
        .value("WB_R", WB_R, "red pixels coefficient of white balance")
        .value("WB_G", WB_G, "green pixels coefficient of white balance")
        .value("WB_B", WB_B, "blue pixels coefficient of white balance")
        .value("OFFSET", OFFSET, "camera offset")
        .value("AUTOEXPO_MAX_GAIN", AUTOEXPO_MAX_GAIN, "maximum gain when auto-adjust")
        .value("AUTOEXPO_MAX_EXPOSURE", AUTOEXPO_MAX_EXPOSURE, "maximum exposure when auto-adjust(uint: ms)")
        .value("AUTOEXPO_BRIGHTNESS", AUTOEXPO_BRIGHTNESS, "target brightness when auto-adjust")
        .value("COOLER_POWER", COOLER_POWER, "cooler power percentage[0-100%](only cool camera)")
        .value("TARGET_TEMP", TARGET_TEMP, "camera target temperature(uint: C)")
        .value("COOLER_ON", COOLER_ON, "turn cooler(and fan) on or off")
        .value("HEATER_ON", HEATER_ON, "(deprecated)get state of lens heater(on or off)")
        .value("HEATER_POWER", HEATER_POWER, "lens heater power percentage[0-100%]")
        .value("FAN_POWER", FAN_POWER, "radiator fan power percentage[0-100%]")
        .value("FRAME_LIMIT", FRAME_LIMIT, "frame rate limit, the range:[0, 2000], 0 means no limit")
        .value("HQI", HQI, "High Quality Image, just for guide camera(no DDR buffer)")
        .value("USB_BANDWIDTH_LIMIT", USB_BANDWIDTH_LIMIT, "USB bandwidth limit")
        .value("PIXEL_BIN_SUM", PIXEL_BIN_SUM, "take the sum of pixels after binning, default is average")
        .export_values();

    py::class_<CameraProperties>(m, "CameraProperties")
        .def(py::init<>())
        .def_readwrite("cameraID", &CameraProperties::cameraID, "it's unique, camera can be controlled and set by the cameraID")
        .def_readwrite("cameraName", &CameraProperties::cameraName, "the camera name")
        .def_readwrite("userCustomName", &CameraProperties::userCustomName, "user custom name, it will be will be added after the camera name, max len 16 bytes,like \'Mars-C [Juno]\', default is empty")
        .def_readwrite("pID", &CameraProperties::pID, "product ID")
        .def_readwrite("maxWidth", &CameraProperties::maxWidth, "max width of the image")
        .def_readwrite("maxHeight", &CameraProperties::maxHeight, "max height of the image")
        .def_readwrite("bitDepth", &CameraProperties::bitDepth, "ADC depth of CMOS sensor")
        .def_readwrite("isColorCamera", &CameraProperties::isColorCamera, "is a color camera or not")
        .def_readwrite("isHasST4Port", &CameraProperties::isHasST4Port, "does the camera have ST4 port, if not, camera don't support ST4 guide")
        .def_readwrite("isHasCooler", &CameraProperties::isHasCooler, "does the camera have cooler, the cooled camera with cooler")
        .def_readwrite("isUSB3Speed", &CameraProperties::isUSB3Speed, "is usb3.0 speed")
        .def_readwrite("isSupportHardBin", &CameraProperties::isSupportHardBin, "whether the camera supports hardware bin")
        .def_readwrite("bayerPattern", &CameraProperties::bayerPattern, "the bayer filter pattern of camera")
        .def_readwrite("pixelSize", &CameraProperties::pixelSize, "camera pixel size, unit is um")
        .def_readwrite("SN", &CameraProperties::SN, "the serial number of camera, it's unique")
        .def_readwrite("sensorModelName", &CameraProperties::sensorModelName, "the sersor model name of camera, eg IMX462")
        .def_readwrite("localPath", &CameraProperties::localPath, "the path of the camera in the computer host")              
        .def_readwrite("bins", &CameraProperties::bins, "bins supported by the camera, 1 == bin1, 2 == bin2, ...")
        .def_readwrite("imgFormats", &CameraProperties::imgFormats, "image data format supported by the camera");

    py::class_<ConfigsAttributes>(m, "ConfigsAttributes")
        .def(py::init<>())
        .def_readwrite("isSupportAuto", &ConfigsAttributes::isSupportAuto, "is support auto?")
        .def_readwrite("isWritable", &ConfigsAttributes::isWritable, "is writable?")
        .def_readwrite("isReadable", &ConfigsAttributes::isReadable, "is readable?")
        .def_readwrite("configID", &ConfigsAttributes::configID, "config ID, eg, EXPOSURE")
        .def_readwrite("maxValue", &ConfigsAttributes::maxValue, "maximum value")
        .def_readwrite("minValue", &ConfigsAttributes::minValue, "minimum value")
        .def_readwrite("defaultValue", &ConfigsAttributes::defaultValue, "default value")
        .def_readwrite("confName", &ConfigsAttributes::confName, "POAConfig name, eg, POA_EXPOSURE name is \"Exposure\"")
        .def_readwrite("confDescription", &ConfigsAttributes::confDescription, "a brief description about this one POAConfig");

    py::class_<SensorModeInfo>(m, "SensorModeInfo")
        .def(py::init<>())
        .def_readwrite("modeID", &SensorModeInfo::modeID, "it's unique, sensor mode ID")
        .def_readwrite("name", &SensorModeInfo::name, "sensor mode name, can be used to display on the UI (eg, Combobox)")
        .def_readwrite("desc", &SensorModeInfo::desc, "sensor mode description, may be useful for tooltip");

    py::class_<GainsAndOffsets>(m, "GainsAndOffsets")
        .def(py::init<>())
        .def_readwrite("gainHighestDR", &GainsAndOffsets::gainHighestDR, "gain at highest dynamic range, in most cases, this gain is 0")
        .def_readwrite("HCGain", &GainsAndOffsets::HCGain, "gain at HCG Mode(High Conversion Gain)")
        .def_readwrite("unityGain", &GainsAndOffsets::unityGain, "unity gain(or standard gain), with this gain, eGain(e/ADU) will be 1")
        .def_readwrite("gainLowestRN", &GainsAndOffsets::gainLowestRN, "aka Maximum Analog Gain, gain at lowest read noise")
        .def_readwrite("offsetHighestDR", &GainsAndOffsets::offsetHighestDR, "offset at highest dynamic range")
        .def_readwrite("offsetHCGain", &GainsAndOffsets::offsetHCGain, "offset at HCG Mode")
        .def_readwrite("offsetUnityGain", &GainsAndOffsets::offsetUnityGain, "offset at unity gain")
        .def_readwrite("offsetLowestRN", &GainsAndOffsets::offsetLowestRN, "offset at lowest read noise");

    m.def("GetAllCameraProperties", &GetAllCameraProperties, R"pbdoc(
        Get all camera properties.
        return: None.
    )pbdoc", py::arg("allCamProps"));

    m.def("GetCameraPropertiesByID", &GetCameraPropertiesByID, R"pbdoc(
        Get the camera properties by ID.
        return: Errors.
    )pbdoc", py::arg("cameraID"), py::arg("camProp"));

    m.def("OpenCamera", &OpenCamera, R"pbdoc(
        Open the camera.
        return: Errors.
    )pbdoc", py::arg("cameraID"));

    m.def("CloseCamera", &CloseCamera, R"pbdoc(
        Close the camera.
        return: Errors.
    )pbdoc", py::arg("cameraID"));

    m.def("GetCameraTemp", [](int cameraID) { double temp; Errors err = GetCameraTemp(cameraID, temp); return std::make_tuple(err, temp); }, R"pbdoc(
        Get the camera current temperature.
        return: Tuple(Errors, temp).
    )pbdoc", py::arg("cameraID"));

    m.def("GetCameraAllConfigs", &GetCameraAllConfigs, R"pbdoc(
        Get camera all configs.
        return: Errors.
    )pbdoc", py::arg("cameraID"), py::arg("allCamCfgs"));

    m.def("GetConfigAttributes", &GetConfigAttributes, R"pbdoc(
        Get the attributes of camera config.
        return: Errors.
    )pbdoc", py::arg("cameraID"), py::arg("cfgID"), py::arg("cfgAttr"));

    m.def("GetConfig", [](int cameraID, CameraConfigs cfgID) { long cfgValue; bool isAuto; Errors err = GetConfig(cameraID, cfgID, cfgValue, isAuto); return std::make_tuple(err, cfgValue, isAuto); }, R"pbdoc(
        Get the current value of camera config.
        return: Tuple(Errors, cfgValue, isAuto).
    )pbdoc", py::arg("cameraID"), py::arg("cfgID"));

    m.def("SetConfig", &SetConfig, R"pbdoc(
        Set camera config value.
        return: Errors.
    )pbdoc", py::arg("cameraID"), py::arg("cfgID"), py::arg("cfgValue"), py::arg("isAuto"));

    m.def("GetImageStartPos", [](int cameraID) { int startX, startY; Errors err = GetImageStartPos(cameraID, startX, startY); return std::make_tuple(err, startX, startY); }, R"pbdoc(
        Get the image start position of CMOS sensor.
        return: Tuple(Errors, startX, startY).
    )pbdoc", py::arg("cameraID"));

    m.def("SetImageStartPos", &SetImageStartPos, R"pbdoc(
        Set the image start position of CMOS sensor.
        return: Errors.
    )pbdoc", py::arg("cameraID"), py::arg("startX"), py::arg("startY"));

    m.def("GetImageSize", [](int cameraID) { int imgWidth, imgHeight; Errors err = GetImageSize(cameraID, imgWidth, imgHeight); return std::make_tuple(err, imgWidth, imgHeight); }, R"pbdoc(
        Get the image size.
        return: Tuple(Errors, imgWidth, imgHeight).
    )pbdoc", py::arg("cameraID"));

    m.def("SetImageSize", &SetImageSize, R"pbdoc(
        Set the image size.
        return: Errors.
    )pbdoc", py::arg("cameraID"), py::arg("imgWidth"), py::arg("imgHeight"));

    m.def("GetImageBin", [](int cameraID) { int bin; Errors err = GetImageBin(cameraID, bin); return std::make_tuple(err, bin); }, R"pbdoc(
        Get the image pixel binning.
        return: Tuple(Errors, bin).
    )pbdoc", py::arg("cameraID"));

    m.def("SetImageBin", &SetImageBin, R"pbdoc(
        Set the image pixel binning.
        return: Errors.
    )pbdoc", py::arg("cameraID"), py::arg("bin"));

    m.def("GetImageFormat", [](int cameraID) { ImgFormat imgFormat; Errors err = GetImageFormat(cameraID, imgFormat); return std::make_tuple(err, imgFormat); }, R"pbdoc(
        Get the image format.
        return: Tuple(Errors, imgFormat).
    )pbdoc", py::arg("cameraID"));

    m.def("SetImageFormat", &SetImageFormat, R"pbdoc(
        Set the image format.
        return: Errors.
    )pbdoc", py::arg("cameraID"), py::arg("imgFormat"));

    m.def("GetImageFlip", [](int cameraID) { bool isFlipHori, isFlipVert; Errors err = GetImageFlip(cameraID, isFlipHori, isFlipVert); return std::make_tuple(err, isFlipHori, isFlipVert); }, R"pbdoc(
        Get the image flip state.
        return: Tuple(Errors, isFlipHori, isFlipVert).
    )pbdoc", py::arg("cameraID"));

    m.def("SetImageFlip", &SetImageFlip, R"pbdoc(
        Set the image flip state.
        return: Errors.
    )pbdoc", py::arg("cameraID"), py::arg("isFlipHori"), py::arg("isFlipVert"));

    m.def("StartExposure", &StartExposure, R"pbdoc(
        Start exposure.
        return: Errors.
    )pbdoc", py::arg("cameraID"), py::arg("isSingleFrame"));

    m.def("StopExposure", &StopExposure, R"pbdoc(
        Stop exposure.
        return: Errors.
    )pbdoc", py::arg("cameraID"));

    m.def("GetCameraState", [](int cameraID) { CameraState cameraState = STATE_CLOSED; GetCameraState(cameraID, cameraState); return cameraState; }, R"pbdoc(
        Get camera state.
        return: cameraState.
    )pbdoc", py::arg("cameraID"));

    m.def("ImageDataReady", [](int cameraID) { bool isReady = false; ImageDataReady(cameraID, isReady); return isReady; }, R"pbdoc(
        Is the image data ready(available)?
        return: isReady.
    )pbdoc", py::arg("cameraID"));

    m.def("GetImageData", [](int cameraID, py::array_t<unsigned char, py::array::c_style> buf, int timeout_ms) {
                                auto buf_info = buf.request();

                                if ( buf_info.format !=  py::format_descriptor<unsigned char>::format() )
                                {return ERROR_INVALID_ARGU;}

                                py::ssize_t buf_size = buf_info.size;

                                unsigned char *pBuf = (unsigned char *)buf_info.ptr;

                                return GetImageData(cameraID, pBuf, (long)buf_size, timeout_ms);                              
                            }, 
    R"pbdoc(
        Get the image data.
        return: Errors.
    )pbdoc", py::arg("cameraID"), py::arg("buf"), py::arg("timeout_ms"));

    m.def("GetDroppedImagesCount", [](int cameraID) { int droppedCount = 0; Errors err = GetDroppedImagesCount(cameraID, droppedCount); return std::make_tuple(err, droppedCount); }, R"pbdoc(
        Get the number of dropped frames.
        return: Tuple(Errors, droppedImagesCount).
    )pbdoc", py::arg("cameraID"));

    m.def("SetGuideST4", &SetGuideST4, R"pbdoc(
        Set ST4 guide direction ON or OFF.
        return: Errors.
    )pbdoc", py::arg("cameraID"), py::arg("guideDirection"), py::arg("OnOff"));

    m.def("GetAllSensorModeInfo", &GetAllSensorModeInfo, R"pbdoc(
        Get all sensor mode info.
        return: Errors.
    )pbdoc", py::arg("cameraID"), py::arg("allSenModeInfo"));

    m.def("GetSensorMode", [](int cameraID) { int modeID = 0; Errors err = GetSensorMode(cameraID, modeID); return std::make_tuple(err, modeID); }, R"pbdoc(
        Get current sensor mode.
        return: Tuple(Errors, modeID).
    )pbdoc", py::arg("cameraID"));

    m.def("SetSensorMode", &SetSensorMode, R"pbdoc(
        Set sensor mode.
        return: Errors.
    )pbdoc", py::arg("cameraID"), py::arg("modeID"));

    m.def("GetCameraEGain", [](int cameraID) { double eGain = 0.0; Errors err = GetCameraEGain(cameraID, eGain); return std::make_tuple(err, eGain); }, R"pbdoc(
        Get the current eGain(e/ADU).
        return: Tuple(Errors, eGain).
    )pbdoc", py::arg("cameraID"));

    m.def("GetGainsAndOffsets", &GetGainsAndOffsets, R"pbdoc(
        Get the preset special gains and offsets.
        return: Errors.
    )pbdoc", py::arg("cameraID"), py::arg("gainsOffsets"));

    m.def("SetUserCustomName", &SetUserCustomName, R"pbdoc(
        Set user custom name that will be added after the camera name, like ' [custom_name]', max lenght is 16 bytes, if you set a empty string, the previous custom name will be cleared.
        return: Errors.
    )pbdoc", py::arg("cameraID"), py::arg("customName"));

    m.def("GetErrorString", &GetErrorString, R"pbdoc(
        Convert Errors enum to a string, useful for logging or displaying on UI.
        return: Errors.
    )pbdoc", py::arg("err"));

    m.def("GetAPIVersion", &GetAPIVersion, R"pbdoc(
        Get the API version, a integer, like: 20230511.
        return: apiNumber
    )pbdoc");

    m.def("GetSDKVersion", &GetSDKVersion, R"pbdoc(
        Get the sdk version, a sting, like: '3.3.0'
        return: SDKVer
    )pbdoc");

#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "0.0.0";
#endif
    m.attr("__email__") = "lei.zhang@player-one-astronomy.com";
}
