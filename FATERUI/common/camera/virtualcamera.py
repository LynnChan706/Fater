#!/usr/bin/env python2.7
# coding=utf-8

'''
@date = '17/4/7'
@author = 'chenliang'
@email = 'chenliang2380@cvte.com'
'''

from . import camera
from os import listdir
import os
import cv2

class VirtualCamera(camera.Camera):

    def __init__(self):
        super(VirtualCamera,self).__init__()
        self._dir = None
        self._filelist=None
        self._numfile=0
        self._currIdx=0

    def set_img_dir(self,dir):
        if dir !='':
            self._dir = dir

    def get_camera_status(self):
        pass

    def open(self):
        if self._dir != None and os.path.exists(self._dir):
            self._filelist = listdir(self._dir)
            self._numfile = len(self._filelist)
            print('file image total number:',self._numfile)
            return True
        else:
            return False

    def take_picture(self, index=None):
        if self._numfile == 0:
            if not self.open():
                return None
        if self._currIdx < len(self._filelist):
            img = cv2.imread(self._dir+self._filelist[self._currIdx])
            self._currIdx += 1
            if self._currIdx == len(self._filelist):
                self._currIdx = 0
            return img
        return None



