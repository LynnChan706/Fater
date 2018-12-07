#!/usr/bin/env python2.7
# coding=utf-8

'''
@date = '17/4/7'
@author = 'chenliang'
@email = 'chenliang2380@cvte.com'
'''

import time
import FATERUI.editwidget
from FATERUI.paramwidget import *
from collections import OrderedDict


class WaitingProcess(ParamWidget):

    def __init__(self, param=''):
        super(WaitingProcess,self).__init__(param)
        self.setWindowTitle("Waiting Process")

    def _initParamStr(self):
        self._parentName='Log'
        self._name='Waiting'
        paramType = {'WaitS': ['T', 0, 0, 2000, 1]}
        paramList = ['WaitS']
        self._paramType = OrderedDict()
        for i in paramList:
            self._paramType[i] = paramType[i]
        self._param = {'WaitS': 1}

    def run(self):
        time.sleep(self._param['WaitS'])
        return FATERUI.editwidget.StatePass
