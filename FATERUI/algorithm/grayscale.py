#!/usr/bin/env python2.7
# coding=utf-8

'''
@date = '17/4/7'
@author = 'chenliang'
@email = 'chenliang2380@cvte.com'
'''

import numpy as np

from FATERUI.paramwidget import *
from collections import OrderedDict
import FATERUI.editwidget
import cv2
from keras.models import load_model

import sys

class GrayScale(ParamWidget):

    def __init__(self, param=''):
        super(GrayScale, self).__init__(param)
        self.setWindowTitle("GrayScale Image")
        # self._model = None
        self.img_rows = 128
        self.img_cols = 20
        self._model = load_model('my_model.h5')

    def _initParamStr(self):
        self._parentName = 'Process'
        self._name = 'GrayScale'

        paramType = {'Method':['C',['CNN','Image']],
                     'CorrectLevel':['T',5,0,10,1],
                     'ROI': ['R']}

        paramList = ['Method','CorrectLevel','ROI']
        self._paramType = OrderedDict()
        for i in paramList:
            self._paramType[i] = paramType[i]
        self._param = {'Method':'CNN','CorrectLevel':5,'ROI':[0,0,50,50]}
        instr = ''
        print(sys.path)

    def run(self):
        image = self.getProcessImage()

        if image is None:
            return FATERUI.editwidget.StateNg

        # cv2ImageRGB = cv2.cvtColor(image, cv2.COLOR_BGRA2GRAY)
        roisx = int(self._param['ROI'][0])
        roisy = int(self._param['ROI'][1])
        roix = int(self._param['ROI'][0]+self._param['ROI'][2])
        roiy = int(self._param['ROI'][1] + self._param['ROI'][3])

        procesImg = image[roisy:roiy,roisx:roix]

        print(procesImg.shape)
        # model = load_model('my_model.h5')
        img = cv2.resize(procesImg, (self.img_rows,self.img_cols))

        # print img.shape
        imglist = []
        imglist.append(img)
        imgarray = np.array(imglist)
        res = self._model.predict(imgarray)

        res = [[0,0,0,1,0,0,0,0]]
        # del model
        print(res)
        def findidx():
            idx = 0
            maxval = 0
            maxidx = 0
            for i in res[0]:
                if i > maxval:
                    maxval = i
                    maxidx = idx
                idx += 1
            return maxidx
        if findidx() == self._param['CorrectLevel']:
            return FATERUI.editwidget.StatePass
        else:
            return FATERUI.editwidget.StateNg
