# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'statisticswidget.ui'
#
# Created by: PyQt5 UI code generator 5.5.1
#
# WARNING! All changes made in this file will be lost!

from PyQt5 import QtCore, QtGui, QtWidgets

class Ui_staticsdgt(object):
    def setupUi(self, staticsdgt):
        staticsdgt.setObjectName("staticsdgt")
        staticsdgt.resize(343, 570)
        self.verticalLayout = QtWidgets.QVBoxLayout(staticsdgt)
        self.verticalLayout.setObjectName("verticalLayout")
        self.horizontalLayout = QtWidgets.QHBoxLayout()
        self.horizontalLayout.setObjectName("horizontalLayout")
        self.lineEdit = QtWidgets.QLineEdit(staticsdgt)
        self.lineEdit.setObjectName("lineEdit")
        self.horizontalLayout.addWidget(self.lineEdit)
        self.pushButton = QtWidgets.QPushButton(staticsdgt)
        self.pushButton.setObjectName("pushButton")
        self.horizontalLayout.addWidget(self.pushButton)
        self.verticalLayout.addLayout(self.horizontalLayout)
        self.horizontalLayout_2 = QtWidgets.QHBoxLayout()
        self.horizontalLayout_2.setObjectName("horizontalLayout_2")
        self.dateTimeEdit = QtWidgets.QDateTimeEdit(staticsdgt)
        self.dateTimeEdit.setObjectName("dateTimeEdit")
        self.horizontalLayout_2.addWidget(self.dateTimeEdit)
        self.dateTimeEdit_2 = QtWidgets.QDateTimeEdit(staticsdgt)
        self.dateTimeEdit_2.setObjectName("dateTimeEdit_2")
        self.horizontalLayout_2.addWidget(self.dateTimeEdit_2)
        self.verticalLayout.addLayout(self.horizontalLayout_2)
        self.tableWidget = QtWidgets.QTableWidget(staticsdgt)
        self.tableWidget.setObjectName("tableWidget")
        self.tableWidget.setColumnCount(3)
        self.tableWidget.setRowCount(0)
        item = QtWidgets.QTableWidgetItem()
        self.tableWidget.setHorizontalHeaderItem(0, item)
        item = QtWidgets.QTableWidgetItem()
        self.tableWidget.setHorizontalHeaderItem(1, item)
        item = QtWidgets.QTableWidgetItem()
        self.tableWidget.setHorizontalHeaderItem(2, item)
        self.verticalLayout.addWidget(self.tableWidget)
        self.graphicsView_2 = QtWidgets.QGraphicsView(staticsdgt)
        self.graphicsView_2.setObjectName("graphicsView_2")
        self.verticalLayout.addWidget(self.graphicsView_2)

        self.retranslateUi(staticsdgt)
        QtCore.QMetaObject.connectSlotsByName(staticsdgt)

    def retranslateUi(self, staticsdgt):
        _translate = QtCore.QCoreApplication.translate
        staticsdgt.setWindowTitle(_translate("staticsdgt", "Form"))
        self.pushButton.setText(_translate("staticsdgt", "search"))
        item = self.tableWidget.horizontalHeaderItem(0)
        item.setText(_translate("staticsdgt", "TIME"))
        item = self.tableWidget.horizontalHeaderItem(1)
        item.setText(_translate("staticsdgt", "RESUALT"))
        item = self.tableWidget.horizontalHeaderItem(2)
        item.setText(_translate("staticsdgt", "INFO"))

