#!/usr/bin/env python2.7
# coding=utf-8

'''
@date = '17/4/7'
@author = 'chenliang'
@email = 'chenliang2380@cvte.cn'
'''

from FATERUI.paramwidget import *
from collections import OrderedDict
import FATERUI.editwidget
import cv2


class ThresholdUi(ParamWidget):

    def __init__(self,param=''):
        super(ThresholdUi,self).__init__(param)
        self.setWindowTitle("Threshold Image")

    def _initParamStr(self):
        self._parentName = 'Process'
        self._name = 'Threshold'

        paramType = {'Method':['C',['adaptiveThreshold','threshold']],
                     'Type':['C',['THRESH_BINARY_INV','THRESH_BINARY','THRESH_TRUNC']],
                     'MaxVal':['T',201,0,255,1]}

        paramList = ['Method','Type','MaxVal']
        self._paramType = OrderedDict()
        for i in paramList:
            self._paramType[i]=paramType[i]
        self._param={'Method':'adaptiveThreshold','Type':'THRESH_TRUNC','MaxVal':120}

    def run(self):
        image = self.getProcessImage()
        cv2ImageRGB = cv2.cvtColor(image, cv2.COLOR_BGRA2GRAY)
        self.setProcessRes(cv2ImageRGB)
        return FATERUI.editwidget.StatePass
