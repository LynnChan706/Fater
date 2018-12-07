#!/usr/bin/env python2.7
# coding=utf-8

'''
@date = '17/4/7'
@author = 'chenliang'
@email = 'chenliang2380@cvte.com'
'''

from PyQt5.QtWidgets import *
from PyQt5 import QtCore
from PyQt5 import QtGui
from . import ui_typewidget

from FATERUI.algorithm import _TypeStr

_translate = QtCore.QCoreApplication.translate
type_col = 0
info_col = 1
check_col = 2


class TypeWidget(QWidget,ui_typewidget.Ui_typeWidget):

    selectsignal = QtCore.pyqtSignal(str,str)

    def __init__(self,father='Source',select='Camera'):
        super(TypeWidget,self).__init__()
        self.setupUi(self)
        self._typeStr = _TypeStr
        self.typeTree.setSelectionMode(QAbstractItemView.SingleSelection)
        self._typeDict = eval(self._typeStr)
        self._fatherType = 'Source'
        self._selectType = 'Camera'

        if select!='' and father !='':
            self._fatherType = father
            self._selectType = select
        self._typeList = []
        self._init_itemWidget()

        self.typeTree.itemDoubleClicked.connect(self._item_clicked)

    @property
    def selectType(self):
        return self._selectType

    @property
    def fatherType(self):
        return self._fatherType

    def getTypeInfo(self,typeStr):
        icon = QtGui.QIcon()
        icon.addPixmap(QtGui.QPixmap(":/ico/"+typeStr+".ico"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        return icon

    def _init_itemWidget(self):
        for i in self._typeDict:
            item_0 = QTreeWidgetItem(self.typeTree)
            item_0.setText(type_col,i)
            self._set_itemico(i, item_0)
            if type(self._typeDict[i]) == dict:

                self._init_topItem(self._typeDict[i], item_0,i==self._fatherType)
            else:
                checkPbtn = QRadioButton()
                checkPbtn.setChecked(False)
                if i==self._selectType:
                    checkPbtn.setChecked(True)
                checkPbtn.setEnabled(False)
                self.typeTree.setItemWidget(item_0,check_col,checkPbtn)
                item_0.setText(info_col,self._typeDict[i])

    def _set_itemico(self,iconame,item):
        icon = QtGui.QIcon()

        icon.addPixmap(QtGui.QPixmap(":/ico/"+iconame+".ico"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        if icon.isNull():
            print(iconame+".ico does not exist")
            icon.addPixmap(QtGui.QPixmap(":/ico/noexist.ico"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        item.setIcon(0, icon)

    def _init_topItem(self,type_dict,items,select):
        for k,v in list(type_dict.items()):
            item_0 = QTreeWidgetItem(items)
            item_0.setText(type_col, k)
            self._set_itemico(k, item_0)
            if not type(v)==dict:
                item_0.setText(info_col, v)
                checkPbtn = QRadioButton()
                checkPbtn.setChecked(False)
                if k == self._selectType and select:
                    checkPbtn.setChecked(True)
                checkPbtn.setEnabled(False)
                self.typeTree.setItemWidget(item_0, check_col, checkPbtn)
            else:
                self._init_topItem(v,item_0)

    def _item_clicked(self):
        checkPbtn=self.typeTree.itemWidget(self.typeTree.currentItem(),check_col)
        if type(checkPbtn)==QRadioButton:
            checkPbtn.setChecked(True)

            self._selectType=self.typeTree.currentItem().text(type_col)
            self._fatherType=self.typeTree.currentItem().parent().text(type_col)
            self.selectsignal.emit(self._selectType,self._fatherType)
            print(self._selectType)
