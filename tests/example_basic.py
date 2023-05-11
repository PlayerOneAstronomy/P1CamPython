# -*- coding: utf-8 -*-

import player_one_camera as P1Cam
import numpy as np
import time

print('version: ' + P1Cam.__version__)
print('email: ' + P1Cam.__email__)
print()

def print_cam_prop(camProp : P1Cam.CameraProperties):
    print('\n ------list camera properties------')
    print('cameraID: {}'.format(camProp.cameraID))
    print('cameraName: {}'.format(camProp.cameraName))
    print('userCustomName: {}'.format(camProp.userCustomName))
    print('pID: {}'.format(camProp.pID))
    print('maxWidth: {}'.format(camProp.maxWidth))
    print('maxHeight: {}'.format(camProp.maxHeight))
    print('bitDepth: {}'.format(camProp.bitDepth))
    print('isColorCamera: {}'.format(camProp.isColorCamera))
    print('isHasST4Port: {}'.format(camProp.isHasST4Port))
    print('isUSB3Speed: {}'.format(camProp.isUSB3Speed))
    print('isSupportHardBin: {}'.format(camProp.isSupportHardBin))
    print('bayerPattern: {}'.format(camProp.bayerPattern))
    print('pixelSize: {}'.format(camProp.pixelSize))
    print('SN: {}'.format(camProp.SN))
    print('sensorModelName: {}'.format(camProp.sensorModelName))
    print('localPath: {}'.format(camProp.localPath))
    print('bins: {}'.format(camProp.bins))
    print('imgFormats: {}'.format(camProp.imgFormats))
    
    
def print_config_attr(cfg_attr : P1Cam.ConfigsAttributes):
    print('\n ------config attribute, config ID:{}------'.format(cfg_attr.configID))
    print('isSupportAuto: {}'.format(cfg_attr.isSupportAuto))
    print('isWritable: {}'.format(cfg_attr.isWritable))
    print('isReadable: {}'.format(cfg_attr.isReadable))
    print('maxValue: {}'.format(cfg_attr.maxValue))
    print('minValue: {}'.format(cfg_attr.minValue))
    print('defaultValue: {}'.format(cfg_attr.defaultValue))
    print('confName: {}'.format(cfg_attr.confName))
    print('confDescription: {}'.format(cfg_attr.confDescription))
    
    
print('Camera SDK Version: V{}, API Version: {} \n'.format(P1Cam.GetSDKVersion(), P1Cam.GetAPIVersion()))

list_cam_prop = P1Cam.ListCamProp()
P1Cam.GetAllCameraProperties(list_cam_prop) # get all camera properties 
camera_count = len(list_cam_prop)
print('camera count: {}'.format(camera_count)) 

if camera_count == 0:
    print('------no camera found------')
    exit()

for camProp in list_cam_prop:
    print_cam_prop(camProp)
    
# operate the first camera(index is 0)
print('\n ------operate the first camera------')
camera_ID = list_cam_prop[0].cameraID
print('------get the first camera properties------')
fisrtCamProp = P1Cam.CameraProperties()
error = P1Cam.GetCameraPropertiesByID(camera_ID, fisrtCamProp)
if error == P1Cam.Errors.OK:
    print_cam_prop(fisrtCamProp)

print('------open first camera------\n')
error = P1Cam.OpenCamera(camera_ID)
if error != P1Cam.Errors.OK:
    print('open camera failed, error string: {} \n'.format(P1Cam.GetErrorString(error)))
    exit()
    
temp_tuple = P1Cam.GetCameraTemp(camera_ID)
if temp_tuple[0] == P1Cam.Errors.OK:
    print('camera temperature: {:.1f}C, error string: {} \n'.format(temp_tuple[1], P1Cam.GetErrorString(temp_tuple[0])))
    
# get all camera configs and their attributes
list_cam_cfg = P1Cam.ListCamCfg()
P1Cam.GetCameraAllConfigs(camera_ID, list_cam_cfg)
print('the camera configs of first camera: ')
for cfg in list_cam_cfg:
    ca = P1Cam.ConfigsAttributes()
    error = P1Cam.GetConfigAttributes(camera_ID, cfg, ca)
    if error != P1Cam.Errors.OK:
        print('get {} attributes failed, error string: {}'.format(cfg, P1Cam.GetErrorString(error)))
        continue
    print_config_attr(ca)
    
# get config and set config
print('-------------------------------')
print('get config and set config: ')
exp_tuple = P1Cam.GetConfig(camera_ID, P1Cam.CameraConfigs.EXPOSURE) # get exposure
if exp_tuple[0] == P1Cam.Errors.OK:
    print('camera current exposure: {} us, isAuto: {}'.format(exp_tuple[1], exp_tuple[2]))
else:
    print('get exposure failed, error string: {}'.format(P1Cam.GetErrorString(exp_tuple[0])))
    
gain_tuple = P1Cam.GetConfig(camera_ID, P1Cam.CameraConfigs.GAIN) # get gain
if gain_tuple[0] == P1Cam.Errors.OK:
    print('camera current gain: {}, isAuto: {}'.format(gain_tuple[1], gain_tuple[2]))
else:
    print('get gain failed, error string: {}'.format(P1Cam.GetErrorString(gain_tuple[0])))

exp_val_us = 100000 #us
error = P1Cam.SetConfig(camera_ID, P1Cam.CameraConfigs.EXPOSURE, exp_val_us, False) # set exposure to 100ms
if error == P1Cam.Errors.OK:
    print('set exposure to {} us OK!'.format(exp_val_us))
else:
    print('set exposure failed, error string: {}'.format( P1Cam.GetErrorString(error)))

gain_val = 100
error = P1Cam.SetConfig(camera_ID, P1Cam.CameraConfigs.GAIN, gain_val, False) # set gain to 100
if error == P1Cam.Errors.OK:
    print('set gain to {} OK!'.format(gain_val))
else:
    print('set gain failed, error string: {}'.format( P1Cam.GetErrorString(error)))
    
# set the ON/OFF(bool) type config, 1 is ON(True), 0 is OFF(False), 
print('\n-------------------------------hard bin')
if fisrtCamProp.isSupportHardBin:
    hard_bin_tuple =  P1Cam.GetConfig(camera_ID, P1Cam.CameraConfigs.HARDWARE_BIN)
    print(hard_bin_tuple)
    is_hard_bin_enable = bool(hard_bin_tuple[1]) # convert int to bool, Of course, you don't have to convert.
    print('get is hard bin enbale: {}'.format(is_hard_bin_enable))
    
    print('disable hard bin')
    is_hard_bin_enable = False # it is recommended to disable hardbin
    P1Cam.SetConfig(camera_ID, P1Cam.CameraConfigs.HARDWARE_BIN, int(is_hard_bin_enable), False) # please convert bool to int, also, P1Cam.SetConfig(camera_ID, P1Cam.CameraConfigs.HARDWARE_BIN, 0, False)
else:
    print('this camera do not support hard bin')
    
# set cooled camera
print('\n------------------------------- cooled camera')
if fisrtCamProp.isHasCooler: # if camera is a cooled camera
    print('set cooled camera: ')
    
    target_temp = -10
    P1Cam.SetConfig(camera_ID, P1Cam.CameraConfigs.TARGET_TEMP, target_temp, False) # set target temperature to -10 C
    
    lens_heater_power = 20 # aka, anti_dew power
    P1Cam.SetConfig(camera_ID, P1Cam.CameraConfigs.HEATER_POWER, lens_heater_power, False) # set lens heater power to 20%
    
    fan_power = 90
    P1Cam.SetConfig(camera_ID, P1Cam.CameraConfigs.FAN_POWER, fan_power, False) # set fan power to 90%
    
    is_cooler_on = True
    P1Cam.SetConfig(camera_ID, P1Cam.CameraConfigs.COOLER_ON, int(is_cooler_on), False) # turn on cooler, also, P1Cam.SetConfig(camera_ID, P1Cam.CameraConfigs.COOLER_ON, 1, False)
    
    cooler_power = P1Cam.GetConfig(camera_ID, P1Cam.CameraConfigs.COOLER_POWER)[1] # get camera current cooler power
    temp = P1Cam.GetCameraTemp(camera_ID)[1] ## get camera current temperature
    print('----->camera temperature: {:.1f}, cooler power: {}'.format(temp, cooler_power))
else:
    print('this camera is not cooled camera')
    

print('-------------------------------')

# get and set image parameters
print('get and set image parameters: ')
start_pos_tuple = P1Cam.GetImageStartPos(camera_ID) # get image start position
if start_pos_tuple[0] == P1Cam.Errors.OK:
    print('image start position, X: {}, Y: {}'.format(start_pos_tuple[1], start_pos_tuple[2]))
else:
    print('get image start position failed, error string: {}'.format(P1Cam.GetErrorString(start_pos_tuple[0])))
    
error = P1Cam.SetImageStartPos(camera_ID, 0, 0) # set image start position to (0, 0)
if error == P1Cam.Errors.OK:
    print('set image start position (0, 0) OK!')
else:
    print('set image start positio failed, error string: {}'.format(P1Cam.GetErrorString(error)))


image_size_tuple = P1Cam.GetImageSize(camera_ID) #get iamge size
if image_size_tuple[0] == P1Cam.Errors.OK:
    print('image size, width: {}, height: {}'.format(image_size_tuple[1], image_size_tuple[2]))
else:
    print('get image size failed, error string: {}'.format(P1Cam.GetErrorString(image_size_tuple[0])))
 
width = fisrtCamProp.maxWidth
height = fisrtCamProp.maxHeight 
error = P1Cam.SetImageSize(camera_ID, width, height) # set image size to max, if camera is exposuring, please stop exposure first, before calling this function
if error == P1Cam.Errors.OK:
    print('set image size, width: {}, height: {} OK!'.format(width, height))
else:
    print('set image size failed, error string: {}'.format(P1Cam.GetErrorString(error)))
   
    
bin_tuple = P1Cam.GetImageBin(camera_ID) # get image bin
if bin_tuple[0] == P1Cam.Errors.OK:
    print('get image pixel binning, bin: {}'.format(bin_tuple[1]))
else:
    print('get image pixel binning failed, error string: {}'.format(P1Cam.GetErrorString(bin_tuple[0])))

bin = 1
error = P1Cam.SetImageBin(camera_ID, bin) # set image bin to 1, if camera is exposuring, please stop exposure first, before calling this function
if error == P1Cam.Errors.OK:
    print('set image pixel binning, bin: {} OK!'.format(bin))
else:
    print('set image pixel binning, error string: {}'.format(P1Cam.GetErrorString(error)))


image_fmt_tuple = P1Cam.GetImageFormat(camera_ID) # get image format
if image_fmt_tuple[0] == P1Cam.Errors.OK:
    print('get image format, format: {}'.format(image_fmt_tuple[1]))
else:
    print('get image format failed, error string: {}'.format(P1Cam.GetErrorString(image_fmt_tuple[0])))

img_fmt = P1Cam.ImgFormat.RAW16
error = P1Cam.SetImageFormat(camera_ID, img_fmt) # set image format to RAW16, if camera is exposuring, please stop exposure first, before calling this function
if error == P1Cam.Errors.OK:
    print('set image format, format: {} OK!'.format(img_fmt))
else:
    print('set image fromat, error string: {}'.format(P1Cam.GetErrorString(error)))
    

image_flip_tuple = P1Cam.GetImageFlip(camera_ID)
if image_flip_tuple[0] == P1Cam.Errors.OK:
    print('image flip state, is flip hori: {}, is flip vert: {}'.format(image_flip_tuple[1], image_flip_tuple[2]))
else:
    print('get image flip state failed, error string: {}'.format(P1Cam.GetErrorString(image_flip_tuple[0])))
    
is_flip_hori = True
is_flip_vert = False  
error = P1Cam.SetImageFlip(camera_ID, is_flip_hori, is_flip_vert) # set image flip horizontally
if error == P1Cam.Errors.OK:
    print('set image flip state, is flip hori: {}, is flip vert: {} OK!'.format(is_flip_hori, is_flip_vert))
else:
    print('set image flip state failed, error string: {}'.format(P1Cam.GetErrorString(error)))

# start exposure and get image data

# allocate image memory in advance
image_buf_size = width * height
if img_fmt == P1Cam.ImgFormat.RAW16:
    image_buf_size *= 2
elif img_fmt == P1Cam.ImgFormat.RGB24:
    image_buf_size *= 3
    
image_buf = np.zeros(image_buf_size, dtype = np.uint8)

# 1. single frame(Snap Mode)
print('single frame exposure (Snap Mode)')
exp_val_us = 1000000
P1Cam.SetConfig(camera_ID, P1Cam.CameraConfigs.EXPOSURE, exp_val_us, False) # set exposure to 1s

frame_count = 0
is_single_frame = True
while frame_count < 5:
    frame_count += 1
    print('frame number: {}'.format(frame_count))
    error = P1Cam.StartExposure(camera_ID, is_single_frame) # Snap Mode, each time you want to get the data, start exposure first
    if error != P1Cam.Errors.OK:
        print('start exposure(Snap Mode) failed, error string: {}'.format(P1Cam.GetErrorString(error)))
        continue
    cam_state = P1Cam.GetCameraState(camera_ID)
    while cam_state == P1Cam.CameraState.STATE_EXPOSING: # in Snap Mode, please check camera state first, wait for exposure finished
        #sleep(exposure_us / 10);
        # if(breakTrigger)
        # {
        #    break;
        # }
        cam_state = P1Cam.GetCameraState(camera_ID)
    
    is_image_ready = P1Cam.ImageDataReady(camera_ID)
    if is_image_ready:
        error = P1Cam.GetImageData(camera_ID, image_buf, exp_val_us//1000+500)
        if error != P1Cam.Errors.OK:
            print('get image data failed: {}'.format(P1Cam.GetErrorString(error)))
        else:
            file_name = 'snap_mode_image_data_{}_{}_{}_{}.npy'.format(width, height, img_fmt, frame_count)
            print('save image data, file name: ' + file_name)
            np.save(file_name, image_buf)
    else:
        print('image data is not ready!')
 
# 2. continuously exposure(Video Mode) 
print('continuously exposure (Video Mode)') 
exp_val_us = 100000
P1Cam.SetConfig(camera_ID, P1Cam.CameraConfigs.EXPOSURE, exp_val_us, False) # set exposure to 100ms

img_fmt = P1Cam.ImgFormat.RAW8
P1Cam.SetImageFormat(camera_ID, img_fmt) # set image fromat to RAW8
image_buf_size = width * height # raw8 buffer size

#print(image_buf.shape)
image_buf.resize(image_buf_size)
#print(image_buf.shape)

frame_count = 0
is_single_frame = False
error = P1Cam.StartExposure(camera_ID, is_single_frame) # Video Mode, start exposure once, and you can keep getting data
if error != P1Cam.Errors.OK:
    print('start exposure(Video Mode) failed, error string: {}'.format(P1Cam.GetErrorString(error)))
else:
    while frame_count < 10: # or while True,this is recommended to do in another thread
        frame_count += 1
        is_image_ready = P1Cam.ImageDataReady(camera_ID)
        while not is_image_ready:
            #sleep(exposure_us / 10);
            # if(breakTrigger)
            # {
            #    break;
            # }
            is_image_ready = P1Cam.ImageDataReady(camera_ID) # in Video Mode, NO need to check camera state 
            
        error = P1Cam.GetImageData(camera_ID, image_buf, exp_val_us//1000+500)     
        if error != P1Cam.Errors.OK:
            print('get image data failed: {}'.format(P1Cam.GetErrorString(error)))
        else:
            file_name = 'video_mode_image_data_{}_{}_{}_{}.npy'.format(width, height, img_fmt, frame_count)
            print('save image data, file name: ' + file_name)
            np.save(file_name, image_buf) 
                      
    # stop exposure (in Video Mode, must call this function for stopping exposure)
    P1Cam.StopExposure(camera_ID) # at any time you can stop the exposure, please remember to call this function
            
dropped_frames_count_tuple = P1Cam.GetDroppedImagesCount(camera_ID)
dropped_frames_count = dropped_frames_count_tuple[1] # dropped_frames_count = P1Cam.GetDroppedImagesCount(camera_ID)[1]
print('dropped frames count: {}'.format(dropped_frames_count))

image_buf = None

# ST4 guide test
if fisrtCamProp.isHasST4Port:
    print('test ST4 guide(On/Off):') # recommended to do this in a thread
    P1Cam.SetGuideST4(camera_ID, P1Cam.GuideDirection.GUIDE_NORTH, True)
    print('guide North start!')
    time.sleep(5) # guide north lasts for 5s
    P1Cam.SetGuideST4(camera_ID, P1Cam.GuideDirection.GUIDE_NORTH, False)
    print('guide North stop!')
else:
    print('this camera do not support ST4 guide')
        
# set sensor mode
list_sen_mode_info = P1Cam.ListSenModeInfo()
P1Cam.GetAllSensorModeInfo(camera_ID, list_sen_mode_info)
if len(list_sen_mode_info) == 0: # if the lenght of list is 0, means that this camera doesn't support sensor mode selection 
    print('this camera do not support sensor mode selection')
else:
    current_sen_mode_ID = P1Cam.GetSensorMode(camera_ID)[1] # get the current sensor mode ID, btw, P1Cam.GetSensorMode(camera_ID)[0] is the error
    
    for sen_mod_info in list_sen_mode_info:
        print('mode ID: {}, mode name: {}, mode description: {} \n'.format(sen_mod_info.modeID, sen_mod_info.name, sen_mod_info.desc))
        if current_sen_mode_ID == sen_mod_info.modeID:
            print('this is camera current sensor mode, mode ID {} \n'.format(current_sen_mode_ID))
    print('set 2nd mode to current sensor mode')
    error = P1Cam.SetSensorMode(camera_ID, list_sen_mode_info[1].modeID) # set sensor mode
    if error != P1Cam.Errors.OK:
        print('set sensor mode failed: {}'.format(P1Cam.GetErrorString(error)))
    else:
        print('set sensor mode OK. modeID: {}'.format(list_sen_mode_info[1].modeID))

# get eGain (e/ADU), eGain will change as the gain changes
eGain = P1Cam.GetCameraEGain(camera_ID)[1]
print('camera current eGain (e/ADU): {:.1f} \n'.format(eGain))

# get preset special gains and offsets
print('\n------------------------------- get preset special gains and offsets')
gains_offsets = P1Cam.GainsAndOffsets()
P1Cam.GetGainsAndOffsets(camera_ID, gains_offsets)
print('gainHighestDR: {}'.format(gains_offsets.gainHighestDR))
print('HCGain: {}'.format(gains_offsets.HCGain))
print('unityGain: {}'.format(gains_offsets.unityGain))
print('gainLowestRN: {}'.format(gains_offsets.gainLowestRN))
print('offsetHighestDR: {}'.format(gains_offsets.offsetHighestDR))
print('offsetHCGain: {}'.format(gains_offsets.offsetHCGain))
print('offsetUnityGain: {}'.format(gains_offsets.offsetUnityGain))
print('offsetLowestRN: {}'.format(gains_offsets.offsetLowestRN))

# set custom name(custom ID)
print('\n------------------------------- set custom name(custom ID)')
custom_name = 'my_cam'
error = P1Cam.SetUserCustomName(camera_ID, custom_name) # if camera is exposuring, please stop exposure first, before calling this function
if error != P1Cam.Errors.OK:
    print('set camera custom name failed: {}'.format(P1Cam.GetErrorString(error)))
else:
    P1Cam.GetCameraPropertiesByID(camera_ID, fisrtCamProp) # get the custom name
    print('camera name: {}, custom name: {}'.format(fisrtCamProp.cameraName, fisrtCamProp.userCustomName))

# clear the custom name
print('clear custom name')
P1Cam.SetUserCustomName(camera_ID, '')
P1Cam.GetCameraPropertiesByID(camera_ID, fisrtCamProp)
print('camera name: {}, custom name: {}'.format(fisrtCamProp.cameraName, fisrtCamProp.userCustomName))

if fisrtCamProp.isHasCooler: #check camera temperature and cooler power again
    print('\n please plug the 12v power supply into cooler camera')
    print(' please wait for 30s to check the temperature change of the cooled camera')
    time.sleep(30)
    cooler_power = P1Cam.GetConfig(camera_ID, P1Cam.CameraConfigs.COOLER_POWER)[1] # get camera current cooler power
    temp = P1Cam.GetCameraTemp(camera_ID)[1] ## get camera current temperature
    print('----->camera temperature: {:.1f}, cooler power: {}'.format(temp, cooler_power))

# close camera
print('\n------------------------------- close camera')
P1Cam.CloseCamera(camera_ID)