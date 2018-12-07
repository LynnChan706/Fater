# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'paramwidget.ui'
#
# Created by: PyQt5 UI code generator 5.5.1
#
# WARNING! All changes made in this file will be lost!

from PyQt5 import QtCore, QtGui, QtWidgets

class Ui_paramwidget(object):
    def setupUi(self, paramwidget):
        paramwidget.setObjectName("paramwidget")
        paramwidget.resize(400, 300)
        self.verticalLayout_2 = QtWidgets.QVBoxLayout(paramwidget)
        self.verticalLayout_2.setContentsMargins(0, 3, 0, 3)
        self.verticalLayout_2.setSpacing(3)
        self.verticalLayout_2.setObjectName("verticalLayout_2")
        self.otherLayout = QtWidgets.QVBoxLayout()
        self.otherLayout.setObjectName("otherLayout")
        self.verticalLayout_2.addLayout(self.otherLayout)
        self.paramTable = QtWidgets.QTableWidget(paramwidget)
        self.paramTable.setStyleSheet("QPushButton\n"
"{\n"
"        background-color: rgb(205, 204, 202);    \n"
"        border-right:1px solid #616161;\n"
"        border-left:1px solid #616161;\n"
"}\n"
"QTableWidget\n"
"{\n"
"    background-color: rgb(205, 204, 202);    \n"
"    border-right:0px solid #616161;\n"
"    border-left:0px solid #616161;\n"
"}\n"
"QTableWidget::item\n"
"{  \n"
"    border-bottom:1px solid #616161;\n"
"    border-right:1px solid #616161;\n"
"\n"
"    margin:0px;  \n"
"}")
        self.paramTable.setObjectName("paramTable")
        self.paramTable.setColumnCount(0)
        self.paramTable.setRowCount(0)
        self.verticalLayout_2.addWidget(self.paramTable)

        self.retranslateUi(paramwidget)
        QtCore.QMetaObject.connectSlotsByName(paramwidget)

    def retranslateUi(self, paramwidget):
        _translate = QtCore.QCoreApplication.translate
        paramwidget.setWindowTitle(_translate("paramwidget", "Form"))

