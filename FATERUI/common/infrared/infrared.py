#!/usr/bin/env python2.7
# coding=utf-8

'''
@date = '17/4/7'
@author = 'chenliang'
@email = 'chenliang2380@cvte.com'
'''

import serial
import binascii
import subprocess
import os
# import pexpect
from time import sleep
from FATERUI.common.camera.singleton import Singleton


class Infrared(Singleton):

    def __init__(self):
        if not hasattr(self,'_initstate'):
            self.ser = None
            self._commandDict = {'MENU':'','RESET':'','UP':'','DOWN':'','LEFT':'','RIGHT':'','VERSION':''
                , 'F1': '','F2':'','F3':'','F4':''}
            self._initstate = False
            self._menustr = '88900E'
            self._exitstr = '88904A'
            self.retry = 1

    def loadCommandDict(self, dict):
        print(dict)
        self._commandDict = dict['cmdDict']
        if 'sername' in dict:
            self.openDevice(dict['sername'])

    def getcommandDict(self):
        cmdDict={}
        cmdDict['cmdDict']=self._commandDict
        if self.ser is not None:
             cmdDict['sername']=self.ser.name
        return cmdDict

    def insertDict(self, key, value):
        self._commandDict[key] = value
        print(self._commandDict)

    def openDevice(self, ser, passwd=''):
        # devicestr = ser
        # print('open  ser ', 'sudo chmod 777 ' + devicestr)
        # shell_cmd = "sudo chmod 777 "+devicestr
        # child = pexpect.spawn(shell_cmd)
        # index = child.expect(['password',pexpect.EOF,pexpect.TIMEOUT])
        # if index == 0:
        #     child.sendline('lin')
        #     index = child.expect(['password',pexpect.EOF,pexpect.TIMEOUT])
        #     if index == 0:
        #         self._initstate = False
        #         return False
        # try:
        #     self.ser = serial.Serial(devicestr, 9600,timeout=0.1)
        #     # self.senddata('88900E')
        #     self._initstate = True
        #     return True
        # except Exception as e:
        #     print(Exception, " init device error:", e)
        #     self._initstate = False
        return False

    def openSer(self):
        if self.ser is not None and not self.ser.isOpen():
            self.ser.open()

    def closeSer(self):
        if self.ser is not None and self.ser.isOpen():
            self.ser.close()

    def getinitState(self):
        return self._initstate

    def order_list(self,a):
        a_list = []
        for i in a.split():
            a_list.append(binascii.a2b_hex(i))
        return a_list

    def hexShow(self,data):
        result = ''
        hLen = len(data)
        for i in range(hLen):
            hvol = ord(data[i])
            hhex = '%02x' % hvol
            result += hhex + ' '
        print('hexShow:', result)
        return result

    def recv(self,ser):
        while True:
            data=''
            try:
                data = ser.read(3)
            except Exception as e:
                print(Exception,e)
                data = ''
                return data
            if data == '':
                continue
            else:
                break
            sleep(0.02)
        return data

    def recvdata(self):
        if self._initstate:
            data = self.recv(self.ser)
            return self.hexShow(data)
        else:
            return ''

    def senddata(self,a):
        self.openSer()
        a = a.replace(' ','')
        sendstr='A1F1'+a
        if self._initstate:
            print('Infrared Send Data:', self._initstate)
            try:
                list = sendstr.decode("hex")
                self.hexShow(list)
                self.ser.write(list)
                return True
            except Exception as e:
                print(Exception, " send str error:", e)
                return False
        else:
            return False

    def fmenu(self):
        for i in range(self.retry):
            self.senddata(self._menustr)

    def emenu(self):
        for i in range(self.retry):
            self.senddata(self._exitstr)