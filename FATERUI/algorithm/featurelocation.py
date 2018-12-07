#!/usr/bin/env python2.7
# coding=utf-8

'''
@date = '17/4/7'
@author = 'chenliang'
@email = 'chenliang2380@cvte.com'
'''



from FATERUI.paramwidget import *
from collections import OrderedDict
import FATERUI.editwidget
import cv2
import numpy as np
from datetime import datetime

class FeatureLocation(ParamWidget):

    def __init__(self,param=''):
        super(FeatureLocation,self).__init__(param)
        self.setWindowTitle("Feature Location")
        self._MIN_MATCH_COUNT = 10
        self._feature = cv2.SURF(5000)
        # cv2.SIFT()
    def _initParamStr(self):
        self._parentName = 'Process'
        self._name = 'Location'
        paramType = {'Method':['C',['SIFT']],'ROI': ['R']}
        paramList = ['Method','ROI']
        self._paramType = OrderedDict()
        for i in paramList:
            self._paramType[i]=paramType[i]
        self._param = {'Method':'SIFT', 'ROI':[0,0,50,50]}

    def run(self):
        if self.getTempImage() is not None:
            print('template ok!')

            tmplImg = self.getTempImage()
            procImg = self.getProcessImage()

            proImg=procImg.copy()

            proImg=self.adaptiveBrightness(proImg,tmplImg)

            if proImg is None:
                return FATERUI.editwidget.StateNg

            orgtime=datetime.now()
            kp1,des1 = self._feature.detectAndCompute(tmplImg, None)
            kp2,des2 = self._feature.detectAndCompute(proImg, None)

            FLANN_INDEX_KDTREE = 0
            index_params = dict(algorithm = FLANN_INDEX_KDTREE, trees=5)
            search_params = dict(checks=50)
            flann = cv2.FlannBasedMatcher(index_params, search_params)

            matches = flann.knnMatch(des1, des2, k=2)

            print('matches size',len(matches))

            # store all the good matches as per Lowe's ratio test.
            good = []
            for m, n in matches:
                if m.distance < 0.7 * n.distance:
                    good.append(m)

            if len(good) > self._MIN_MATCH_COUNT:
                src_pts = np.float32([kp1[m.queryIdx].pt for m in good]).reshape(-1, 1, 2)
                dst_pts = np.float32([kp2[m.trainIdx].pt for m in good]).reshape(-1, 1, 2)

                M, mask = cv2.findHomography(dst_pts,src_pts,cv2.RANSAC, 5.0)
                matchesMask = mask.ravel().tolist()

                print(tmplImg.shape)

                h = tmplImg.shape[0]
                w = tmplImg.shape[1]
                pts = np.float32([[0, 0], [0, h - 4], [w - 4, h - 4], [w - 4, 0]]).reshape(-1, 1, 2)
                dst = cv2.perspectiveTransform(pts, M)
                proImg = cv2.warpPerspective(procImg, M, (tmplImg.shape[1], tmplImg.shape[0]))
                print(dst)

                self.setProcessImage(proImg)
                # showimg = cv2.cvtColor(proImg, cv2.COLOR_BGRA2RGB)
                self.setShowImage(proImg)
                finishtime = datetime.now()
                print('locatios cost time:',(finishtime-orgtime).seconds,'s')
                return FATERUI.editwidget.StatePass
            return FATERUI.editwidget.StateNg
        else:
            print('template error!')
            return FATERUI.editwidget.StateNg

    def adaptiveBrightness(self,imgorg, imgtemp):
        orghsv = cv2.cvtColor(imgorg, cv2.COLOR_BGR2HSV)
        temphsv = cv2.cvtColor(imgtemp, cv2.COLOR_BGR2HSV)

        orgmean, orgdev = cv2.meanStdDev(orghsv)
        tempmean, tempdev = cv2.meanStdDev(temphsv)

        print('v valse:', orgmean[2], tempmean[2], tempmean[2] / orgmean[2])
        f = tempmean[2] / orgmean[2]
        w = imgorg.shape[1]
        h = imgorg.shape[0]
        if f>1:
            for xi in range(0, w):
                for xj in range(0, h):
                    if int(orghsv[xj, xi, 2] * f) < 255:
                        orghsv[xj, xi, 2] = int(orghsv[xj, xi, 2] * f)
                    else:
                        orghsv[xj, xi, 2] = 255
        img = cv2.cvtColor(orghsv, cv2.COLOR_HSV2BGR)
        return img
