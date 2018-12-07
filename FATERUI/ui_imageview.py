# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'imageview.ui'
#
# Created by: PyQt5 UI code generator 5.5.1
#
# WARNING! All changes made in this file will be lost!

from PyQt5 import QtCore, QtGui, QtWidgets

class Ui_imageview(object):
    def setupUi(self, imageview):
        imageview.setObjectName("imageview")
        imageview.resize(435, 362)
        icon = QtGui.QIcon()
        icon.addPixmap(QtGui.QPixmap(":/ico/image.ico"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        imageview.setWindowIcon(icon)
        imageview.setStyleSheet("border-color: rgb(205, 204, 202);\n"
"border-color: rgb(205, 204, 202);\n"
"border-right-color: rgb(205, 204, 202);\n"
"gridline-color: rgb(205, 204, 202);")

        self.retranslateUi(imageview)
        QtCore.QMetaObject.connectSlotsByName(imageview)

    def retranslateUi(self, imageview):
        _translate = QtCore.QCoreApplication.translate
        imageview.setWindowTitle(_translate("imageview", "Image"))

import uires_rc
