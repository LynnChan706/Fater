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
import Image
import numpy as np
import pytesseract
import re
from PyQt5.QtCore import QRect,QRectF
import uuid

class OcrUi(ParamWidget):

    def __init__(self,param=''):
        super(OcrUi,self).__init__(param)
        self.setWindowTitle("Ocr Image")

    def _initParamStr(self):
        self._parentName = 'Process'
        self._name = 'Ocr'

        paramType = {'Method':['C',['OCR','Template match']],
                     'Threshold':['T',80,0,100,1],
                     'ErrorText':['E',''],
                     'Correct':['E',''],
                     'TextMatching':['E','*'],
                     'ROI': ['R']}

        paramList = ['Method','Threshold','ErrorText','Correct','TextMatching','ROI']
        self._paramType = OrderedDict()
        for i in paramList:
            self._paramType[i] = paramType[i]
        self._param = {'Method':'OCR','Threshold':80,'ErrorText':'','Correct':'','TextMatching':'*','ROI':[0,0,50,50]}

    def run(self):

        image = self.getProcessImage()

        if image is None:
            return FATERUI.editwidget.StateNg

        cv2ImageRGB = cv2.cvtColor(image, cv2.COLOR_BGRA2GRAY)
        roisx=int( self._param['ROI'][0])
        roisy =int( self._param['ROI'][1])
        roix=int(self._param['ROI'][0]+self._param['ROI'][2])
        roiy = int(self._param['ROI'][1] + self._param['ROI'][3])

        procesImg = cv2ImageRGB[roisy:roiy,roisx:roix]
        if self._param['Method'] == 'OCR':
            pimg = Image.fromarray(np.array(procesImg))
            ocrStr = pytesseract.image_to_string(pimg)
            # ocrStr = ocrStr.replace(' ','')
            scrRes = self.correctStr(ocrStr)
            fonts = cv2.FONT_HERSHEY_SIMPLEX
            image[roisy:roiy, roisx:roix] = cv2.cvtColor(procesImg, cv2.COLOR_GRAY2BGR)
            cv2.putText(image,scrRes,(20,roiy),fonts,3,(0,230,50),10)
            # showimg = cv2.cvtColor(image, cv2.COLOR_BGRA2RGB)
            self.setProcessImage(image)
            self.setShowImage(image)
            # scrRes = self.correctStr(ocrStr)
            print('ocrres:', ' org:', ocrStr, ' correct:', scrRes)
            if self._param['ErrorText'] != '':
                rightStr = self.correctStr(self._param['ErrorText'])
                if scrRes == rightStr:
                    return FATERUI.editwidget.StateNg
                else:
                    return FATERUI.editwidget.StatePass
            elif self._param['Correct'] != '':
                rightStr = self.correctStr(self._param['Correct'])
                if scrRes == rightStr:
                    return FATERUI.editwidget.StatePass
                else:
                    return FATERUI.editwidget.StateNg
            else:
                if (self._param['TextMatching'] != '*' or self._param['TextMatching'] != '') and ocrStr is not '':
                    rightStr = self.correctStr(self._param['TextMatching'])
                    try:
                        if re.search(self._param['TextMatching'],scrRes):
                            return FATERUI.editwidget.StatePass
                        else:
                            return FATERUI.editwidget.StateNg
                    except Exception as e:
                        print(Exception,"error:",e)
                        return FATERUI.editwidget.StateNg
                else:
                    return FATERUI.editwidget.StatePass
        else:
            imageTmpl = self.getTempImage()

            if imageTmpl is None:
                return FATERUI.editwidget.StateNg

            tmplRect = QRectF(0, 0, image.shape[1],image.shape[0])

            prcoRect = QRectF(self._param['ROI'][0]-20,self._param['ROI'][1]-20,
                              self._param['ROI'][2]+40,self._param['ROI'][3]+40)

            roirect = tmplRect.intersected(prcoRect)

            print(roirect.x(),roirect.y(),roirect.bottomRight().x(),roirect.bottomRight().y())

            procesroiImg = image[int(roirect.y()):int(roirect.bottomRight().y()),
                           int(roirect.x()):int(roirect.bottomRight().x())]

            # procesroiImg=[[123,132],[123,321]]
            print(procesroiImg.shape[0],procesroiImg.shape[1])

            tmplroiImg = imageTmpl[roisy:roiy, roisx:roix]
            print(tmplroiImg.shape[0], tmplroiImg.shape[1])
            res = cv2.matchTemplate(procesroiImg,tmplroiImg, cv2.TM_CCOEFF_NORMED)
            cv2.imwrite('./img/procesroiImg.jpg',procesroiImg)
            cv2.imwrite('./img/lroiImg.jpg',tmplroiImg)
            min_val, max_val, min_loc, max_loc = cv2.minMaxLoc(res)

            fonts = cv2.FONT_HERSHEY_SIMPLEX

            print('temlate match res',max_val,min_val)
            if max_val > (float(self._param['Threshold'])/100):
                cv2.putText(image, 'Value: ' + str(int(max_val * 100)), (20, roiy), fonts, 3, (0, 230, 50), 10)
                self.setProcessImage(image)
                self.setShowImage(image)
                return FATERUI.editwidget.StatePass
            else:
                cv2.putText(image, 'Value: ' + str(int(max_val * 100)), (20, roiy), fonts, 3, (0, 50, 230), 10)
                self.setProcessImage(image)
                self.setShowImage(image)
                return FATERUI.editwidget.StateNg
        return FATERUI.editwidget.StatePass

    def gettemptestres(self):

        image = self.getProcessImage()

        if image is None:
            return FATERUI.editwidget.StateNg

        roisx=int( self._param['ROI'][0])
        roisy =int( self._param['ROI'][1])
        roix=int(self._param['ROI'][0]+self._param['ROI'][2])
        roiy = int(self._param['ROI'][1] + self._param['ROI'][3])

        procesImg = image[roisy:roiy,roisx:roix]

        imageTmpl = self.getTempImage()
        if imageTmpl is None:
            return FATERUI.editwidget.StateNg

        tmplroiImg = imageTmpl[roisy:roiy, roisx:roix]

        difimg = cv2.absdiff(tmplroiImg,procesImg)

        insmean,insdev = cv2.meanStdDev(difimg)

        _feature = cv2.SURF(1000)


        # procesImg = cv2.cvtColor(procesImg,cv2.COLOR_BGR2GRAY)

        idp=str(uuid.uuid4())[:6]

        # cv2.imwrite('img/'+str(idp)+'org.bmp', procesImg)
        # procesImg = cv2.equalizeHist(procesImg)

        # cv2.imwrite('img/'+str(idp) +'.bmp', procesImg)
        procesImg = self.adaptiveBrightness(procesImg, tmplroiImg)

        kp1, des1 = _feature.detectAndCompute(tmplroiImg, None)
        kp2, des2 = _feature.detectAndCompute(procesImg, None)

        res = cv2.matchTemplate(procesImg, tmplroiImg, cv2.TM_CCOEFF_NORMED)
        min_val, max_val, min_loc, max_loc = cv2.minMaxLoc(res)

        print('tmplimg key value: ',len(kp1),' process key value: ',len(kp2),'match res',max_val)
        if len(kp1)>10:
            kp1size=len(kp1)
        else:
            kp1size=10

        if(len(kp2)>kp1size):
            score=1.0
        else:
            score=float(len(kp2))/float(kp1size)

        score=score*max_val*100
        cv2.imwrite('img/__' + str(int(score)) + '__' + idp + 'org.bmp', procesImg)
        print('end res socket---------------------- :',score)
        print('std cv mean :', insmean[0],insdev[0])

    def adaptiveBrightness(self,imgorg, imgtemp):
        orghsv = cv2.cvtColor(imgorg, cv2.COLOR_BGR2HSV)
        temphsv = cv2.cvtColor(imgtemp, cv2.COLOR_BGR2HSV)

        orgmean, orgdev = cv2.meanStdDev(orghsv)
        tempmean, tempdev = cv2.meanStdDev(temphsv)

        print('v valse:', orgmean[2], tempmean[2], tempmean[2] / orgmean[2])
        f = tempmean[2] / orgmean[2]

        w = imgorg.shape[1]
        h = imgorg.shape[0]
        if f > 1:
            for xi in range(0, w):
                for xj in range(0, h):
                    if int(orghsv[xj, xi, 2] * f) < 255:
                        orghsv[xj, xi, 2] = int(orghsv[xj, xi, 2] * f)
                    else:
                        orghsv[xj, xi, 2] = 255
        img = cv2.cvtColor(orghsv, cv2.COLOR_HSV2BGR)
        return img

    def correctStr(self,str):
        res = str.upper()
        fomart = 'ABCDEFGHIGKLMNOPQRSTUVWXYZ0123456789*'
        for c in res:
            if not c in fomart:
                res = res.replace(c, '')
        res = res.replace('O', '0').replace('I', '1').replace('L', '1')
        return res

