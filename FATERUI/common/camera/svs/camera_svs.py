#!/usr/bin/env python2.7
# coding=utf-8

import logging
import traceback
import time
import CameraSVS
from aoi.common import Camera, transform_image


class SVS(Camera):
    def __init__(self, camera_degree=0, is_trigger_mode=False):
        Camera.__init__(self)
        self.camera_name = 'SVS'
        self.camera_init_status = False
        self.__is_trigger_mode = is_trigger_mode
        self.__camera = CameraSVS.CameraSVS()
        self.camera_status = self.CAMERA_STATUS_UNCONNECTED
        self.__mark_as_open = False
        self.__flash_mode = True
        self.__old_time = 0.0
        self.__rotate_degree = camera_degree

    def get_camera_name(self):
        return self.camera_name

    def open(self):
        return self.__camera.open()

    def __del__(self):
        if self.get_camera_status() == self.CAMERA_STATUS_CONNECTED:
            self.__camera.release_camera()
            self.__mark_as_open = False
        del self.__camera

    def close(self):
        self.__camera.release_camera()
        self.__mark_as_open = False
        self.camera_status = self.CAMERA_STATUS_UNCONNECTED
        return True

    def __take_picture(self):
        if self.__mark_as_open:
            self.__mark_as_open = False
            self.__camera.open()
            logging.getLogger('pointgrey_camera').info('camera info:%s' % self.get_camera_description())
        img = self.__camera.get_image_in_numpy()

        return img

    def set_flash_mode(self, flash=True):
        self.__flash_mode = flash

    def take_picture(self, index=None):
        try:
            img = self.__take_picture()
            if img is not None and img.size > 100:
                if self.__is_trigger_mode:
                    if time.time() - self.__old_time < 1:
                        return None
                    self.__old_time = time.time()
                if abs(self.__rotate_degree) > 0.0001:
                    img = transform_image(img, angle=self.__rotate_degree, keep_all=True)
            elif img.size <= 100 and self.__is_trigger_mode:
                img = None
        except Exception as err:
            logging.getLogger('logger_system').exception('error in take_picture:%s' % traceback.print_exc())
            img = None
        return img

    def get_camera_description(self):
        desc = 'firmware:' + str(self.__camera.get_firmware_version()) +\
               '\nid:' + str(self.get_camera_id())
        return desc

    def get_camera_id(self):
        return self.__camera.get_camera_id()

    def get_camera_status(self):
        return self.camera_status

    def get_frame_count(self):
        return self.__camera.get_frame_count()

    def get_error_frame_count(self):
        return self.__camera.get_error_frame_count()

    def get_frame_rate(self):
        return self.__camera.get_frame_rate()

    def get_parameter(self):
        shutter_time = self.__camera.get_shutter()
        wb_red = self.__camera.get_white_balance_red()
        wb_green = self.__camera.get_white_balance_green()
        wb_blue = self.__camera.get_white_balance_blue()
        parameter = {'shutter': shutter_time,
                     'wb_red': wb_red,
                     'wb_green': wb_green,
                     'wb_blue': wb_blue,
                     'rotate': self.__rotate_degree}
        return parameter

    def set_parameter(self, **kwargs):
        flag = True
        shutter = kwargs.get('shutter', None)
        if shutter is not None:
            success = self.__camera.set_shutter(kwargs['shutter'])
            if not success:
                flag = False
        wb_red = kwargs.get('wb_red', None)
        if wb_red is not None:
            success = self.__camera.set_wb_red(wb_red)
            if not success:
                flag = False
        wb_green = kwargs.get('wb_green', None)
        if wb_green is not None:
            success = self.__camera.set_wb_green(wb_green)
            if not success:
                flag = False
        wb_blue = kwargs.get('wb_blue', None)
        if wb_blue is not None and wb_blue > 0:
            success = self.__camera.set_wb_blue(wb_blue)
            if not success:
                flag = False
        self.__rotate_degree = kwargs.get('rotate', self.__rotate_degree)
        return flag

    def get_picture_info(self):
        return 'Temperature:[%.2f]\t\tFrameRate:[%.2f]' % (self.get_camera_temperature(), self.get_frame_rate())

    def get_camera_temperature(self):
        return self.__camera.get_camera_temperature()

    def __is_connect(self):
        return self.__camera.is_connected()
