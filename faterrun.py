#!/usr/bin/env python2.7
# coding=utf-8

'''
@date = '17/4/7'
@author = 'chenliang'
@email = 'chenliang2380@cvte.cn'
'''

import sys

from PyQt5.QtWidgets import *
from PyQt5 import QtGui
from PyQt5.QtCore import QFile
from FATERUI import faterwin
import cv2
if __name__ == '__main__':

    app = QApplication(sys.argv)

    # style = QFile("./FATERUI/qss/psblack.qss")
    # style.open(QFile.ReadOnly)
    # styleSheet = style.readAll()
    # styleSheet = unicode(styleSheet, encoding='utf8')
    # app.setStyle(styleSheet)
    # style.close()

    print(( cv2.__version__ ))
    app.setStyle('Fusion')
    fater_win = faterwin.FaterWin()
    icon = QtGui.QIcon()
    icon.addPixmap(QtGui.QPixmap(":/ico/eye.ico"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
    fater_win.setWindowIcon(icon)
    fater_win.showMaximized()
    app.exec_()




