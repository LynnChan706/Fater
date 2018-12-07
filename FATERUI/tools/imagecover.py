#!/usr/bin/env python2.7
# coding=utf-8

'''
@date = '17/4/7'
@author = 'chenliang'
@email = 'chenliang2380@cvte.cn'
'''


from PyQt5.QtGui import  QImage


import cv2

def arrayToQimage(img):
    # print img.shape
    if len(img.shape) == 3:
        img = cv2.cvtColor(img, cv2.COLOR_BGRA2RGB)
        return QImage(img.data,img.shape[1], img.shape[0],img.shape[1]*img.shape[2], QImage.Format_RGB888)
    elif len(img.shape) == 2:
        return QImage(img.data, img.shape[1], img.shape[0], img.shape[1], QImage.Format_Indexed8)
    else:
        return None