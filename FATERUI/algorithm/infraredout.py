#!/usr/bin/env python2.7
# coding=utf-8

'''
@date = '17/4/7'
@author = 'chenliang'
@email = 'chenliang2380@cvte.com'
'''

import time
from FATERUI.paramwidget import *
from collections import OrderedDict
import FATERUI.editwidget
from FATERUI.common import Infrared


class InfraredOut(ParamWidget):

    def __init__(self,param=''):
        super(InfraredOut, self).__init__(param)
        self.setWindowTitle("Infrared")

    def _initParamStr(self):
        self._parentName='Out'
        self._name='Infrared'

        infrareddev = Infrared()
        cmddict=infrareddev.getcommandDict()

        cmdlist=[]
        for k,v in list(cmddict['cmdDict'].items()):
            cmdlist.append(k)

        paramType = {'SleepTime_S':['T',1,0,100,1],
                     'Command': ['C', cmdlist],
                     'Message':['E','']}

        paramList = ['Command','Message','SleepTime_S']
        self._paramType = OrderedDict()
        for i in paramList:
            self._paramType[i]=paramType[i]
        self._param={'SleepTime_S':1,'Message':'','Command':'MENU'}

    def run(self):
        try:
            infrareddev = Infrared()
            if infrareddev.senddata(self._param['Message']):
                time.sleep(self._param['SleepTime_S'])
                return FATERUI.editwidget.StatePass
            return FATERUI.editwidget.StateNg
        except Exception as e:
            print(Exception, ":", e)
            return FATERUI.editwidget.StateNg

    def _paramChangeEvent(self,key,value):
        if key is 'Command':
            cmddict = Infrared().getcommandDict()
            if value in cmddict['cmdDict']:
                cmdtext=cmddict['cmdDict'][value]
                self._param['Message'] = cmdtext
                self.refreshParam('Message')
