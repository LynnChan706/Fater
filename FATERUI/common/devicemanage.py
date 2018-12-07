#!/usr/bin/env python2.7
# coding=utf-8

'''
@date = '17/4/7'
@author = 'chenliang'
@email = 'chenliang2380@cvte.com'
'''

from .camera import cameramanage

class DeviceManage(object):
    def __init__(self):
        self._devdict = {}

        self._camlist = []

        self._camMan = cameramanage.CameraManage()

        self._virCam = None

        pass

    def getDeviceDict(self):
        return self._devdict

    def initdevice(self):
        self._camMan.initCamera()





