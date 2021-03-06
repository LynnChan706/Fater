#!/usr/bin/env python2.7
# coding=utf-8

'''
@date = '17/4/7'
@author = 'chenliang'
@email = 'chenliang2380@cvte.com'
'''
from . import cameraUi
from . import thresholdUi
from . import waitingUi
from . import ocrUi
from . import infraredout
from . import featurelocation
from . import grayscale

_TypeParamPage = {'Camera': 'cameraUi.CameraUi',
                  'Threshold': 'thresholdUi.ThresholdUi',
                  'Waiting': 'waitingUi.WaitingProcess',
                  'Ocr': 'ocrUi.OcrUi',
                  'grayscale':'grayscale.GrayScale',
                  'Location': 'featurelocation.FeatureLocation',
                  'Infrared': 'infraredout.InfraredOut'}


# _TypeStr = "{'Source':{'Camera':'camera imagesource','File':'file image'}," \
#            "'In':{'Net':'a net','Serial':'serial in put'}," \
#            "'Process':{'Threshold':'binary image'," \
#            "'Ocr':'character recognition','Location':'SIFT Feature location'}," \
#            "'Log':{'Waiting':'Waiting for condition'}," \
#            "'Out':{'Net':'a net','Serial':'serial in put','Infrared':'Infrared out'}}"


_TypeStr = "{'Source':{'Camera':'camera imagesource'}," \
           "'Process':{'Threshold':'binary image'," \
           "'Ocr':'character recognition'," \
           "'grayscale':'gray level inspection'," \
           "'Location':'SIFT Feature location'}," \
           "'Log':{'Waiting':'Waiting for condition'}," \
           "'Out':{'Infrared':'Infrared out'}}"