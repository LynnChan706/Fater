#!/usr/bin/env python2.7
# coding=utf-8

'''
@date = '17/4/7'
@author = 'chenliang'
@email = 'chenliang2380@cvte.cn'
'''


from PyQt5.QtWidgets import *
from PyQt5 import QtCore
from PyQt5 import QtGui

from . import ui_editwidget
from . import typewidget
from . import imageview
from FATERUI.algorithm import *
from FATERUI.algorithm import _TypeParamPage
ParamItems = 0
ParamSelect = 1
ParamInfo = 2

StateWait = 0
StatePass = 1
StateNg = 2
StateProcess = 3

class EditWidget(QWidget,ui_editwidget.Ui_editwidget):

    showSignal = QtCore.pyqtSignal(imageview.ImageView)
    addSingal = QtCore.pyqtSignal(QWidget)
    deleteSingal = QtCore.pyqtSignal(QWidget)

    def __init__(self,father='Source',name ='Camera',param=''):

        super(EditWidget,self).__init__()
        self.setupUi(self)
        self._split = QSplitter(self.scrollAreaWidgetContents)

        self.typeLabel = QLabel(self.scrollAreaWidgetContents)
        font = QtGui.QFont()
        font.setPointSize(14)
        font.setItalic(True)
        self.typeLabel.setFont(font)
        self.typeLabel.setFrameShape(QFrame.NoFrame)
        self.typeLabel.setObjectName("typeLabel")
        self.typeLabel.setMaximumHeight(26)
        self.typeLabel.setMinimumHeight(26)

        self.nameLabel.setText(name)
        self.parambtn.clicked.connect(self._show_proc)
        self.frame_2.setContextMenuPolicy(QtCore.Qt.CustomContextMenu)
        self.frame_2.customContextMenuRequested.connect(self._show_contextmenu)
        self.paramArea.setVisible(False)
        self.setMinimumSize(QtCore.QSize(250, 64))
        self.setMaximumSize(QtCore.QSize(1600, 64))

        self._menu = QMenu(self)
        self._init_menu()

        self._id = 0
        self._selectType = ''
        self._selectTypeFather=''

        self._typeWidget = typewidget.TypeWidget(father,name)
        # self.typeSelectLayout.addWidget(self._typeWidget)
        self._typeWidget.selectsignal.connect(self._init_param_view)

        self._imageWidget = imageview.ImageView()

        self._typeParamPage = _TypeParamPage

        self._stateDict = {StateWait:['NULL',"color: rgb(30,30,30);"],StatePass:['PASS',"color: rgb(0,240,0);"],
                          StateNg:['NG',"color: rgb(240,0,0);"],StateProcess:['PROCESS',"color: rgb(0,0,240);"]}

        self._state = StateWait
        self.setState(StateWait)

        self._paramPage = eval(self._typeParamPage[name]+'()')
        self._init_param_view(self._typeWidget.selectType, self._typeWidget.fatherType,param)
        self._paramPage.setImagewidget(self._imageWidget)

        #styleSheet
        self.frame_2.setFrameShadow(QFrame.Plain)
        self.frame_2.setLineWidth(2)
        self.frame_2.setStyleSheet("")

        self._split.setOrientation(QtCore.Qt.Vertical)
        self._split.addWidget(self._typeWidget)
        self._split.addWidget(self.typeLabel)
        self._split.addWidget(self._paramPage)
        self.typeSelectLayout.addWidget(self._split)

        self.setEditId(0)

    def getShowWidget(self):
        return self._imageWidget

    def getParamStr(self):
        return self._paramPage.getParamStr()

    def saveTmplImg(self,str):
        self._paramPage.saveTmplImg(str)

    def loadTmplImg(self,str):
        self._paramPage.loadTmplImg(str)

    def initProcess(self):
        self._paramPage.initProcess()

    def _init_menu(self):

        self._actAdd = QAction(QtGui.QIcon(":/ico/Add.ico"),"Insert pro",self,triggered=self._add_proc)
        self._actDelete = QAction(QtGui.QIcon(":/ico/Delete.ico"),"Delete pro",self,triggered=self._delete_proc)
        self._actShow = QAction(QtGui.QIcon(":/ico/Show.ico"),"Show|Hide", self,triggered=self._show_proc)
        self._actRunto = QAction(QtGui.QIcon(":/ico/RunCurrent.ico"),"Run to current",self,triggered=self._runto_proc)
        self._actDetial = QAction(QtGui.QIcon(":/ico/Detial.ico"),"Detials ", self,triggered=self._show_prarmpage)

        self._menu.addAction(self._actShow)
        self._menu.addSeparator()
        self._menu.addAction(self._actAdd)
        self._menu.addAction(self._actDelete)
        self._menu.addSeparator()

        self._menu.addAction(self._actRunto)
        self._menu.addAction(self._actDetial)

    def _init_param_view(self, selectType, selectTypeFather, param=''):
        idstr = ''
        if self._id != 0:
            idstr = '[ '+str(self._id)+' ] '

        self.typeLabel.setText(selectType)
        self.nameLabel.setText('[ '+selectTypeFather+' | '+selectType+' ]')

        self._selectType = selectType
        self._selectTypeFather = selectTypeFather

        pix = QtGui.QPixmap(":/ico/"+selectType+".ico")
        if pix.isNull():
            print(selectType + ".ico does not exist")
            pix = QtGui.QPixmap(":/ico/noexist.ico")
        self.icoLabel.setPixmap(pix)
        self._paramPage.close()
        self._paramPage.deleteLater()
        # self._split.removeWidget(self._paramPage)
        # self.otherParamLayout.setGeometry()
        self._imageWidget.setWindowTitle(idstr+'[ '+selectTypeFather+' | '+selectType+' ]')
        self._imageWidget.setId(self._id)

        if selectType in self._typeParamPage:
            self._paramPage = eval(self._typeParamPage[selectType]+'(param)')
            self._paramPage.setImagewidget(self._imageWidget)
            self._split.addWidget(self._paramPage)
        else:
            rect=self.otherParamLayout.geometry()
            # rect=QtCore.QRect
            rect.setHeight(1)
            self.otherParamLayout.setGeometry(rect)
            self._paramPage.setGeometry(rect)

        print("init view",selectType)

    def setEditId(self,idx = 0):
        if idx == 0:
            self._id=0
            self.idlabel.setText('')
            self._imageWidget.setWindowTitle('[ '+self._selectTypeFather + ' | ' + self._selectType+' ]')
            self._imageWidget.setId(0)
        else:
            self._id = idx
            idstr = '[ ' + str(self._id) + ' ] '
            self.idlabel.setText(idstr)
            self._imageWidget.setWindowTitle(idstr+'[ '+self._selectTypeFather + ' | ' + self._selectType+' ]')
            self._imageWidget.setId(self._id)

    def setTempImage(self,img):
        self._paramPage.setTempImage(img)

    def getTempImage(self):
        return self._paramPage.getTempImage()

    def mouseReleaseEvent(self, QMouseEvent):
        self._show_prarmpage()

    # def mouseDoubleClickEvent(self, QMouseEvent):
    #     self._show_prarmpage()

    def _show_prarmpage(self):
        if self.paramArea.isVisible():
            self.paramArea.setVisible(False)
            self.setMinimumSize(QtCore.QSize(250, 64))
            self.setMaximumSize(QtCore.QSize(1600, 64))
            self.frame_2.setFrameShape(QFrame.StyledPanel)
            self.frame_2.setStyleSheet("")

        else:
            self.paramArea.setVisible(True)
            self.setMinimumSize(QtCore.QSize(250, 560))
            self.setMaximumSize(QtCore.QSize(1600, 1600))
            self.frame_2.setFrameShape(QFrame.Box)
            self.frame_2.setStyleSheet("QFrame#frame_2\n"
                                       "{\n"
                                       "    border-left: 2px solid #467CB3;\n"
                                       "    border-right: 2px solid #467CB3;\n"
                                       "    border-top: 2px solid #467CB3;\n"
                                       "    border-bottom: 2px solid #467CB3;\n"
                                       "}")

    def _show_contextmenu(self,pos):
        self._menu.exec_(self.frame_2.mapToGlobal(pos))

    def _delete_proc(self,obj):
        self.deleteSingal.emit(self)

    def _add_proc(self,obj):
        self.addSingal.emit(self)

    def _show_proc(self, obj):
        self.showSignal.emit(self._paramPage.get_imageWidget())

    def _runto_proc(self, obj):
        mesbox = QMessageBox.information(self, "proc ", "Run")

    def setState(self,state):
        self._state = state
        self.resLabel.setText(self._stateDict[state][0])
        self.resLabel.setStyleSheet(self._stateDict[state][1])

    def run(self):
        # print 'prarm edit run'
        self.setState(StateProcess)
        state=self._paramPage.run()
        self.setState(state)

