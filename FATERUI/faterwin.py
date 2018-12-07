#!/usr/bin/env python2.7
# coding=utf-8

'''
@date = '17/4/7'
@author = 'chenliang'
@email = 'chenliang2380@cvte.cn'
'''

import sys
from PyQt5.QtWidgets import *
from PyQt5.QtCore import *
from . import ui_fatermainwin
from . import editwidget
import os
import threading
from . import settingdlg
from . import statisticswidget
import common.camera.cameramanage

class FaterWin(QMainWindow,ui_fatermainwin.Ui_FaterMainWin):
    def __init__(self):
        super(FaterWin, self).__init__()
        self.setupUi(self)

        self._sta = statisticswidget.StatisticsWidget()
        self.stadock.setWidget(self._sta)
        self.actionOpen.triggered.connect(self._open_file_dialog)
        self.actionSave_as.triggered.connect(self._saveas_test_file)
        self.actionSave.triggered.connect(self._save_test_file)
        self.editscrollarea.setWidgetResizable(True)
        self.actionSetting.triggered.connect(self.openSettingDlg)
        self.settingpage = settingdlg.SettingDlg()
        self._widgetDict = {}
        self._processList = []
        self.add_subwidows()
        self.actionTo_end.triggered.connect(self.startProcess)
        self.addbtn.clicked.connect(self.addDefaultWideget)
        self.runendbtn.clicked.connect(self.startProcess)
        self._currentName = ''

        self.stadock.close()
        self.runDock.close()
        # self.mdiview.setMouseTracking(True)
        # self.mdiview.installEventFilter(self)

        # self.initCamera()

    def _open_file_dialog(self):
        openfile = QFileDialog(self,'Open file','./testfile')
        openfile.setFileMode(QFileDialog.DirectoryOnly)
        openfile.exec_()
        filedir = openfile.selectedFiles()

        if filedir != '' and os.path.exists(filedir[0] + '/setting.tes'):
            try:
                settingfile = open(filedir[0] + '/setting.tes', 'r')
                settingstr = settingfile.read()
                self.settingpage.loadSetting(eval(settingstr))
                settingfile.close()
            except Exception:
                print(Exception)

        if filedir!='' and os.path.exists(filedir[0]+'/fater.tes'):
            file = filedir[0].split('/')
            self._currentName=file[-1]
            configfile = open(filedir[0]+'/fater.tes', 'r')
            try:
                teststr = configfile.read()
                self.delAllEdit()
                editlist=eval(teststr)
                for edit in editlist:
                    self.addEditWidget(None,edit[0],edit[1],edit[2])
                print(editlist)

                for edit in self._processList:
                    edit.loadTmplImg(filedir[0]+'/')

            finally:
                configfile.close()
        else:
            QMessageBox.warning(self,'FATER','There is no test file in this file!')

    def _save_test_file(self):
        savestr='['
        for edit in self._processList:
            savestr=savestr+edit.getParamStr()+',\n'
        savestr=savestr[:-2]+']'
        saveSetting=self.settingpage.getSetting()
        if self._currentName !='':
            isExists = os.path.exists('./testfile/'+self._currentName)
            if not isExists:
                print('file not exists')
                self._saveas_test_file(False,self._currentName)
            else:
                savefilename='./testfile/'+self._currentName+'/fater.tes'
                outfile = open(savefilename, 'w+')
                outfile.write(savestr)
                outfile.close()
                saveSetname = './testfile/' + self._currentName + '/setting.tes'
                outsetfile = open(saveSetname, 'w+')
                outsetfile.write(saveSetting)
                outsetfile.close()
                for edit in self._processList:
                    edit.saveTmplImg('./testfile/'+self._currentName+'/')

                QMessageBox.information(self,'FATER','Save file OK!')
        else:
            self._saveas_test_file()

    def _saveas_test_file(self,trigger=False,filename=''):
        savestr='['
        for edit in self._processList:
            savestr=savestr+edit.getParamStr()+',\n'
        savestr=savestr[:-2]+']'
        saveSetting = self.settingpage.getSetting()
        if filename != '':
            path='./testfile/'+filename+'/fater.tes'
        else:
            path = QFileDialog.getSaveFileName(self,'Save File',"./testfile", "Test Files(*.tes)")
            if path[0] == '':
                return False
        isExists = os.path.exists(path[0])
        if not isExists:
            print(path[0] + 'creat ok')
            os.makedirs(path[0])
            savefilename = path[0] + '/fater.tes'
            saveSetname = path[0] + '/setting.tes'
            if savefilename != '':
                outfile = open(savefilename, 'w')
                outfile.write(savestr)
                outfile.close()
                outsetfile = open(saveSetname, 'w+')
                outsetfile.write(saveSetting)
                outsetfile.close()
                for edit in self._processList:
                    edit.saveTmplImg(path[0]+'/')
                QMessageBox.information(self, 'FATER', 'Save file OK!')
            return True
        else:
            return False

    def openSettingDlg(self):
        # self.settingpage = settingdlg.SettingDlg()
        self.settingpage.show()

    def add_subwidows(self):

        self.addEditWidget()
        print('curdir',os.path.abspath(os.curdir))

    def initCamera(self):
        common.camera.cameramanage.CameraManage.initCamera()

    def delAllEdit(self):
        print('processlist',self._processList)
        while len(self._processList)>0:
            edit=self._processList[-1]
            self.deleteEditWidget(edit)
        print('processlist', self._processList)

    def deleteEditWidget(self,widget):

        print(self._widgetDict)
        imageview=widget.getShowWidget()
        if imageview in self._widgetDict:
            self._widgetDict[imageview].hide()
            del self._widgetDict[imageview]
        if widget in self._processList:
            self._processList.remove(widget)
            self.editlayout1.removeWidget(widget)
            widget.close()
        self.refreshIdx()

    def addDefaultWideget(self):
        self.addEditWidget()

    def addEditWidget(self,ahead=None,father='Source',name='Camera',param=''):
        if ahead==None:
            edit = editwidget.EditWidget(father,name,param)
            self.editlayout1.addWidget(edit)
            edit.showSignal.connect(self._show_widget)
            edit.addSingal.connect(self.addEditWidget)
            edit.deleteSingal.connect(self.deleteEditWidget)
            self._processList.append(edit)
            self.refreshIdx()
            print(self._processList)
        else:
            edit = editwidget.EditWidget(father,name,param)
            edit.showSignal.connect(self._show_widget)
            edit.addSingal.connect(self.addEditWidget)
            edit.deleteSingal.connect(self.deleteEditWidget)
            idx = self._processList.index(ahead)+1
            self._processList.insert(idx,edit)
            self.editlayout1.insertWidget(idx,edit)
            self.refreshIdx()
            print(self._processList)

    def startProcess(self):
        #Todo peocess
        if hasattr(self,'_processThread') and self._processThread.isAlive():
            QMessageBox.warning(self, 'FATER', 'The process is running!')
        else:
            self._processThread = threading.Thread(target=self.run)
            self._processThread.start()

    def refreshIdx(self):
        idx = 1
        for edit in self._processList:
            edit.setEditId(idx)
            idx += 1

    def run(self):
        if self.initRun():
            for i in self._processList:
                i.run()

    def initRun(self):
        if len(self._processList)>0:
            self._processList[0].initProcess()
            return True
        return False
    def _show_widget(self,widget):
        print(self._widgetDict)
        # print 'iddd' ,id(widget)
        if self._widgetDict.get(widget):
            if self._widgetDict[widget].isHidden():
                self._widgetDict[widget].setWindowTitle(widget.windowTitle())
                self._widgetDict[widget].show()
            else:
                self._widgetDict[widget].hide()
        else:
            self._widgetDict[widget] = self.mdiview.addSubWindow(widget)

            flags=Qt.CustomizeWindowHint | Qt.WindowMinMaxButtonsHint | Qt.WindowTitleHint
            self._widgetDict[widget].setWindowFlags(flags)
            self._widgetDict[widget].setWindowTitle(widget.windowTitle())
            self._widgetDict[widget].setWindowIcon(widget.windowIcon())
            self._widgetDict[widget].show()

if __name__ == '__main__':

    app = QApplication(sys.argv)
    app.setStyle('Fusion')
    faterwin = FaterWin()
    faterwin.show()
    app.exec_()
