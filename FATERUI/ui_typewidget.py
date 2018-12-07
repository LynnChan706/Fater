# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'typewidget.ui'
#
# Created by: PyQt5 UI code generator 5.5.1
#
# WARNING! All changes made in this file will be lost!

from PyQt5 import QtCore, QtGui, QtWidgets

class Ui_typeWidget(object):
    def setupUi(self, typeWidget):
        typeWidget.setObjectName("typeWidget")
        typeWidget.resize(439, 389)
        self.verticalLayout = QtWidgets.QVBoxLayout(typeWidget)
        self.verticalLayout.setContentsMargins(1, 5, 1, 1)
        self.verticalLayout.setObjectName("verticalLayout")
        self.horizontalLayout = QtWidgets.QHBoxLayout()
        self.horizontalLayout.setObjectName("horizontalLayout")
        self.searchEdit = QtWidgets.QLineEdit(typeWidget)
        self.searchEdit.setObjectName("searchEdit")
        self.horizontalLayout.addWidget(self.searchEdit)
        self.searchBtn = QtWidgets.QPushButton(typeWidget)
        self.searchBtn.setObjectName("searchBtn")
        self.horizontalLayout.addWidget(self.searchBtn)
        self.verticalLayout.addLayout(self.horizontalLayout)
        self.typeTree = QtWidgets.QTreeWidget(typeWidget)
        font = QtGui.QFont()
        font.setFamily("Umpush")
        font.setPointSize(14)
        self.typeTree.setFont(font)
        self.typeTree.setStyleSheet("QTreeWidget\n"
"{\n"
"    background-color: rgb(205, 204, 202);\n"
"}\n"
"QTreeWidget::item\n"
"{  \n"
"    border-bottom:1px solid #616161;\n"
"    margin:0px;  \n"
"}")
        self.typeTree.setObjectName("typeTree")
        self.verticalLayout.addWidget(self.typeTree)

        self.retranslateUi(typeWidget)
        QtCore.QMetaObject.connectSlotsByName(typeWidget)

    def retranslateUi(self, typeWidget):
        _translate = QtCore.QCoreApplication.translate
        typeWidget.setWindowTitle(_translate("typeWidget", "Form"))
        self.searchBtn.setText(_translate("typeWidget", "search"))
        self.typeTree.headerItem().setText(0, _translate("typeWidget", " Type"))
        self.typeTree.headerItem().setText(1, _translate("typeWidget", " Info"))
        self.typeTree.headerItem().setText(2, _translate("typeWidget", " State"))

