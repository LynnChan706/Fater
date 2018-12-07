# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'editwidget.ui'
#
# Created by: PyQt5 UI code generator 5.5.1
#
# WARNING! All changes made in this file will be lost!

from PyQt5 import QtCore, QtGui, QtWidgets

class Ui_editwidget(object):
    def setupUi(self, editwidget):
        editwidget.setObjectName("editwidget")
        editwidget.resize(330, 666)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Preferred)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(editwidget.sizePolicy().hasHeightForWidth())
        editwidget.setSizePolicy(sizePolicy)
        editwidget.setMinimumSize(QtCore.QSize(260, 64))
        editwidget.setContextMenuPolicy(QtCore.Qt.CustomContextMenu)
        self.verticalLayout_4 = QtWidgets.QVBoxLayout(editwidget)
        self.verticalLayout_4.setContentsMargins(2, 0, 2, 0)
        self.verticalLayout_4.setSpacing(0)
        self.verticalLayout_4.setObjectName("verticalLayout_4")
        self.frame_2 = QtWidgets.QFrame(editwidget)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Preferred, QtWidgets.QSizePolicy.Expanding)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.frame_2.sizePolicy().hasHeightForWidth())
        self.frame_2.setSizePolicy(sizePolicy)
        self.frame_2.setMinimumSize(QtCore.QSize(0, 64))
        self.frame_2.setContextMenuPolicy(QtCore.Qt.CustomContextMenu)
        self.frame_2.setStyleSheet("QFrame#frame_2\n"
"{\n"
"    border-left: 4px solid #BFFFBF;\n"
"    border-right: 2px solid #BFFFBF;\n"
"    border-top: 2px solid #BFFFBF;\n"
"    border-bottom: 2px solid #BFFFBF;\n"
"}")
        self.frame_2.setFrameShape(QtWidgets.QFrame.StyledPanel)
        self.frame_2.setFrameShadow(QtWidgets.QFrame.Plain)
        self.frame_2.setLineWidth(5)
        self.frame_2.setObjectName("frame_2")
        self.verticalLayout_6 = QtWidgets.QVBoxLayout(self.frame_2)
        self.verticalLayout_6.setContentsMargins(2, 2, 2, 2)
        self.verticalLayout_6.setSpacing(0)
        self.verticalLayout_6.setObjectName("verticalLayout_6")
        self.frame = QtWidgets.QFrame(self.frame_2)
        self.frame.setMinimumSize(QtCore.QSize(0, 64))
        self.frame.setMaximumSize(QtCore.QSize(16777215, 64))
        self.frame.setStyleSheet("")
        self.frame.setObjectName("frame")
        self.horizontalLayout_2 = QtWidgets.QHBoxLayout(self.frame)
        self.horizontalLayout_2.setContentsMargins(1, 1, 1, 1)
        self.horizontalLayout_2.setSpacing(2)
        self.horizontalLayout_2.setObjectName("horizontalLayout_2")
        self.icoLabel = QtWidgets.QLabel(self.frame)
        self.icoLabel.setMinimumSize(QtCore.QSize(55, 55))
        self.icoLabel.setMaximumSize(QtCore.QSize(55, 55))
        self.icoLabel.setText("")
        self.icoLabel.setPixmap(QtGui.QPixmap(":/ico/Source.ico"))
        self.icoLabel.setScaledContents(True)
        self.icoLabel.setAlignment(QtCore.Qt.AlignCenter)
        self.icoLabel.setObjectName("icoLabel")
        self.horizontalLayout_2.addWidget(self.icoLabel)
        self.verticalLayout_2 = QtWidgets.QVBoxLayout()
        self.verticalLayout_2.setObjectName("verticalLayout_2")
        self.horizontalLayout = QtWidgets.QHBoxLayout()
        self.horizontalLayout.setObjectName("horizontalLayout")
        self.idlabel = QtWidgets.QLabel(self.frame)
        font = QtGui.QFont()
        font.setPointSize(13)
        self.idlabel.setFont(font)
        self.idlabel.setStyleSheet("color: rgb(14, 48, 101);")
        self.idlabel.setObjectName("idlabel")
        self.horizontalLayout.addWidget(self.idlabel)
        self.nameLabel = QtWidgets.QLabel(self.frame)
        self.nameLabel.setMinimumSize(QtCore.QSize(0, 0))
        font = QtGui.QFont()
        font.setPointSize(13)
        self.nameLabel.setFont(font)
        self.nameLabel.setStyleSheet("color: rgb(14, 48, 101);")
        self.nameLabel.setObjectName("nameLabel")
        self.horizontalLayout.addWidget(self.nameLabel)
        self.horizontalLayout.setStretch(1, 1)
        self.verticalLayout_2.addLayout(self.horizontalLayout)
        self.infoLabel = QtWidgets.QLabel(self.frame)
        self.infoLabel.setObjectName("infoLabel")
        self.verticalLayout_2.addWidget(self.infoLabel)
        self.horizontalLayout_2.addLayout(self.verticalLayout_2)
        self.verticalLayout_5 = QtWidgets.QVBoxLayout()
        self.verticalLayout_5.setContentsMargins(-1, -1, -1, 1)
        self.verticalLayout_5.setSpacing(0)
        self.verticalLayout_5.setObjectName("verticalLayout_5")
        self.resLabel = QtWidgets.QLabel(self.frame)
        self.resLabel.setMinimumSize(QtCore.QSize(0, 30))
        self.resLabel.setMaximumSize(QtCore.QSize(16777215, 30))
        font = QtGui.QFont()
        font.setPointSize(18)
        self.resLabel.setFont(font)
        self.resLabel.setStyleSheet("color: rgb(112, 178, 69);")
        self.resLabel.setAlignment(QtCore.Qt.AlignCenter)
        self.resLabel.setObjectName("resLabel")
        self.verticalLayout_5.addWidget(self.resLabel)
        self.parambtn = QtWidgets.QPushButton(self.frame)
        self.parambtn.setMinimumSize(QtCore.QSize(66, 26))
        self.parambtn.setMaximumSize(QtCore.QSize(16777215, 26))
        self.parambtn.setStyleSheet("QPushButton {\n"
"    \n"
"    background-color: rgb(234, 230, 225);\n"
"    border-style: outset;\n"
"    border-width: 2px;\n"
"    border-radius: 6px;\n"
"    border-color: beige;\n"
"    font: bold 10px;\n"
"    min-width: 5em;\n"
"    padding: 1px;\n"
"}\n"
"QPushButton:pressed \n"
"{\n"
"    \n"
"    background-color: rgb(205, 204, 202);\n"
"    border-style: inset;\n"
"}")
        icon = QtGui.QIcon()
        icon.addPixmap(QtGui.QPixmap(":/ico/Show.ico"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.parambtn.setIcon(icon)
        self.parambtn.setCheckable(False)
        self.parambtn.setChecked(False)
        self.parambtn.setObjectName("parambtn")
        self.verticalLayout_5.addWidget(self.parambtn)
        self.horizontalLayout_2.addLayout(self.verticalLayout_5)
        self.horizontalLayout_2.setStretch(1, 1)
        self.verticalLayout_6.addWidget(self.frame)
        self.paramArea = QtWidgets.QScrollArea(self.frame_2)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Preferred)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.paramArea.sizePolicy().hasHeightForWidth())
        self.paramArea.setSizePolicy(sizePolicy)
        self.paramArea.setMinimumSize(QtCore.QSize(0, 460))
        self.paramArea.setSizeAdjustPolicy(QtWidgets.QAbstractScrollArea.AdjustToContents)
        self.paramArea.setWidgetResizable(True)
        self.paramArea.setObjectName("paramArea")
        self.scrollAreaWidgetContents = QtWidgets.QWidget()
        self.scrollAreaWidgetContents.setGeometry(QtCore.QRect(0, 0, 314, 592))
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Preferred, QtWidgets.QSizePolicy.Expanding)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.scrollAreaWidgetContents.sizePolicy().hasHeightForWidth())
        self.scrollAreaWidgetContents.setSizePolicy(sizePolicy)
        self.scrollAreaWidgetContents.setMinimumSize(QtCore.QSize(0, 460))
        self.scrollAreaWidgetContents.setObjectName("scrollAreaWidgetContents")
        self.verticalLayout_3 = QtWidgets.QVBoxLayout(self.scrollAreaWidgetContents)
        self.verticalLayout_3.setContentsMargins(3, 3, 3, 3)
        self.verticalLayout_3.setObjectName("verticalLayout_3")
        self.typeSelectLayout = QtWidgets.QVBoxLayout()
        self.typeSelectLayout.setSizeConstraint(QtWidgets.QLayout.SetMaximumSize)
        self.typeSelectLayout.setObjectName("typeSelectLayout")
        self.verticalLayout_3.addLayout(self.typeSelectLayout)
        self.verticalLayout_3.setStretch(0, 1)
        self.paramArea.setWidget(self.scrollAreaWidgetContents)
        self.verticalLayout_6.addWidget(self.paramArea)
        self.verticalLayout_4.addWidget(self.frame_2)

        self.retranslateUi(editwidget)
        QtCore.QMetaObject.connectSlotsByName(editwidget)

    def retranslateUi(self, editwidget):
        _translate = QtCore.QCoreApplication.translate
        editwidget.setWindowTitle(_translate("editwidget", "Form"))
        self.idlabel.setText(_translate("editwidget", "0"))
        self.nameLabel.setText(_translate("editwidget", "MAC"))
        self.infoLabel.setText(_translate("editwidget", "info............"))
        self.resLabel.setText(_translate("editwidget", "PASS"))
        self.parambtn.setText(_translate("editwidget", "S|H"))

import uires_rc
