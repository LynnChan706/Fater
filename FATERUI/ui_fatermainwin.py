# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'fatermainwin.ui'
#
# Created by: PyQt5 UI code generator 5.5.1
#
# WARNING! All changes made in this file will be lost!

from PyQt5 import QtCore, QtGui, QtWidgets

class Ui_FaterMainWin(object):
    def setupUi(self, FaterMainWin):
        FaterMainWin.setObjectName("FaterMainWin")
        FaterMainWin.resize(914, 765)
        self.centralwidget = QtWidgets.QWidget(FaterMainWin)
        self.centralwidget.setObjectName("centralwidget")
        self.horizontalLayout_2 = QtWidgets.QHBoxLayout(self.centralwidget)
        self.horizontalLayout_2.setContentsMargins(0, 0, 0, 0)
        self.horizontalLayout_2.setSpacing(0)
        self.horizontalLayout_2.setObjectName("horizontalLayout_2")
        self.frame = QtWidgets.QFrame(self.centralwidget)
        self.frame.setFrameShape(QtWidgets.QFrame.StyledPanel)
        self.frame.setFrameShadow(QtWidgets.QFrame.Raised)
        self.frame.setObjectName("frame")
        self.horizontalLayout = QtWidgets.QHBoxLayout(self.frame)
        self.horizontalLayout.setContentsMargins(0, 0, 0, 0)
        self.horizontalLayout.setSpacing(0)
        self.horizontalLayout.setObjectName("horizontalLayout")
        self.mdiview = QtWidgets.QMdiArea(self.frame)
        self.mdiview.setStyleSheet("QMdiArea\n"
"{\n"
"    border: 2px  solid  rgba(0,0,0,0%);\n"
"}\n"
"QMdiSubWindow::QWidget\n"
"{\n"
"    border: 3px  solid  rgba(0,0,0,0%);\n"
"}")
        brush = QtGui.QBrush(QtGui.QColor(205, 204, 202))
        brush.setStyle(QtCore.Qt.SolidPattern)
        self.mdiview.setBackground(brush)
        self.mdiview.setTabShape(QtWidgets.QTabWidget.Triangular)
        self.mdiview.setObjectName("mdiview")
        self.horizontalLayout.addWidget(self.mdiview)
        self.horizontalLayout_2.addWidget(self.frame)
        FaterMainWin.setCentralWidget(self.centralwidget)
        self.menubar = QtWidgets.QMenuBar(FaterMainWin)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 914, 28))
        self.menubar.setObjectName("menubar")
        self.menuFile = QtWidgets.QMenu(self.menubar)
        icon = QtGui.QIcon.fromTheme("File")
        self.menuFile.setIcon(icon)
        self.menuFile.setObjectName("menuFile")
        self.menuView = QtWidgets.QMenu(self.menubar)
        self.menuView.setObjectName("menuView")
        self.menuRun = QtWidgets.QMenu(self.menubar)
        self.menuRun.setObjectName("menuRun")
        self.menuSetting = QtWidgets.QMenu(self.menubar)
        self.menuSetting.setObjectName("menuSetting")
        FaterMainWin.setMenuBar(self.menubar)
        self.statusbar = QtWidgets.QStatusBar(FaterMainWin)
        self.statusbar.setStyleSheet("border-color: rgb(18, 10, 10);")
        self.statusbar.setObjectName("statusbar")
        FaterMainWin.setStatusBar(self.statusbar)
        self._editdock = QtWidgets.QDockWidget(FaterMainWin)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Expanding)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self._editdock.sizePolicy().hasHeightForWidth())
        self._editdock.setSizePolicy(sizePolicy)
        self._editdock.setMinimumSize(QtCore.QSize(275, 123))
        self._editdock.setAllowedAreas(QtCore.Qt.LeftDockWidgetArea|QtCore.Qt.RightDockWidgetArea)
        self._editdock.setObjectName("_editdock")
        self.dockWidgetContents = QtWidgets.QWidget()
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Preferred, QtWidgets.QSizePolicy.Expanding)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.dockWidgetContents.sizePolicy().hasHeightForWidth())
        self.dockWidgetContents.setSizePolicy(sizePolicy)
        self.dockWidgetContents.setObjectName("dockWidgetContents")
        self.verticalLayout = QtWidgets.QVBoxLayout(self.dockWidgetContents)
        self.verticalLayout.setContentsMargins(0, 0, 0, 0)
        self.verticalLayout.setSpacing(0)
        self.verticalLayout.setObjectName("verticalLayout")
        self.editscrollarea = QtWidgets.QScrollArea(self.dockWidgetContents)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Preferred)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.editscrollarea.sizePolicy().hasHeightForWidth())
        self.editscrollarea.setSizePolicy(sizePolicy)
        self.editscrollarea.setVerticalScrollBarPolicy(QtCore.Qt.ScrollBarAsNeeded)
        self.editscrollarea.setSizeAdjustPolicy(QtWidgets.QAbstractScrollArea.AdjustToContents)
        self.editscrollarea.setWidgetResizable(True)
        self.editscrollarea.setObjectName("editscrollarea")
        self.scrollAreaWidgetContents_2 = QtWidgets.QWidget()
        self.scrollAreaWidgetContents_2.setGeometry(QtCore.QRect(0, 0, 273, 648))
        self.scrollAreaWidgetContents_2.setObjectName("scrollAreaWidgetContents_2")
        self.verticalLayout_3 = QtWidgets.QVBoxLayout(self.scrollAreaWidgetContents_2)
        self.verticalLayout_3.setContentsMargins(0, 0, 0, 0)
        self.verticalLayout_3.setSpacing(0)
        self.verticalLayout_3.setObjectName("verticalLayout_3")
        self.editlayout1 = QtWidgets.QVBoxLayout()
        self.editlayout1.setSizeConstraint(QtWidgets.QLayout.SetDefaultConstraint)
        self.editlayout1.setSpacing(3)
        self.editlayout1.setObjectName("editlayout1")
        self.verticalLayout_3.addLayout(self.editlayout1)
        self.widget = QtWidgets.QWidget(self.scrollAreaWidgetContents_2)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Preferred, QtWidgets.QSizePolicy.Expanding)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.widget.sizePolicy().hasHeightForWidth())
        self.widget.setSizePolicy(sizePolicy)
        self.widget.setObjectName("widget")
        self.verticalLayout_4 = QtWidgets.QVBoxLayout(self.widget)
        self.verticalLayout_4.setContentsMargins(1, 2, 1, 1)
        self.verticalLayout_4.setSpacing(0)
        self.verticalLayout_4.setObjectName("verticalLayout_4")
        self.frame_2 = QtWidgets.QFrame(self.widget)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Preferred, QtWidgets.QSizePolicy.Preferred)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.frame_2.sizePolicy().hasHeightForWidth())
        self.frame_2.setSizePolicy(sizePolicy)
        self.frame_2.setMinimumSize(QtCore.QSize(0, 66))
        self.frame_2.setStyleSheet("QFrame#frame_2\n"
"{\n"
"    border-left: 0px solid rgb(205, 204, 202);\n"
"    border-right: 0px solid rgb(205, 204, 202);\n"
"    border-top: 2px solidrgb(205, 204, 202);\n"
"    border-bottom: 1px solid rgb(205, 204, 202);\n"
"}")
        self.frame_2.setFrameShape(QtWidgets.QFrame.Panel)
        self.frame_2.setFrameShadow(QtWidgets.QFrame.Plain)
        self.frame_2.setMidLineWidth(1)
        self.frame_2.setObjectName("frame_2")
        self.horizontalLayout_5 = QtWidgets.QHBoxLayout(self.frame_2)
        self.horizontalLayout_5.setContentsMargins(1, 2, 1, 0)
        self.horizontalLayout_5.setSpacing(2)
        self.horizontalLayout_5.setObjectName("horizontalLayout_5")
        self.addbtn = QtWidgets.QPushButton(self.frame_2)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Minimum, QtWidgets.QSizePolicy.Preferred)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.addbtn.sizePolicy().hasHeightForWidth())
        self.addbtn.setSizePolicy(sizePolicy)
        self.addbtn.setMinimumSize(QtCore.QSize(0, 60))
        self.addbtn.setMaximumSize(QtCore.QSize(1000, 60))
        self.addbtn.setStyleSheet("background-color:rgb(239, 235, 231);\n"
"")
        self.addbtn.setText("")
        icon = QtGui.QIcon()
        icon.addPixmap(QtGui.QPixmap(":/ico/Add.ico"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.addbtn.setIcon(icon)
        self.addbtn.setIconSize(QtCore.QSize(35, 35))
        self.addbtn.setObjectName("addbtn")
        self.horizontalLayout_5.addWidget(self.addbtn)
        self.runendbtn = QtWidgets.QPushButton(self.frame_2)
        self.runendbtn.setMinimumSize(QtCore.QSize(0, 60))
        self.runendbtn.setMaximumSize(QtCore.QSize(16777215, 60))
        self.runendbtn.setStyleSheet("background-color: rgb(244, 242, 240);")
        self.runendbtn.setText("")
        icon1 = QtGui.QIcon()
        icon1.addPixmap(QtGui.QPixmap(":/ico/Run.ico"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.runendbtn.setIcon(icon1)
        self.runendbtn.setIconSize(QtCore.QSize(35, 35))
        self.runendbtn.setObjectName("runendbtn")
        self.horizontalLayout_5.addWidget(self.runendbtn)
        self.runcirclebtn = QtWidgets.QPushButton(self.frame_2)
        self.runcirclebtn.setMinimumSize(QtCore.QSize(0, 60))
        self.runcirclebtn.setMaximumSize(QtCore.QSize(16777215, 60))
        self.runcirclebtn.setStyleSheet("background-color: rgb(244, 242, 240);")
        self.runcirclebtn.setText("")
        icon2 = QtGui.QIcon()
        icon2.addPixmap(QtGui.QPixmap(":/ico/End.ico"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.runcirclebtn.setIcon(icon2)
        self.runcirclebtn.setIconSize(QtCore.QSize(35, 35))
        self.runcirclebtn.setObjectName("runcirclebtn")
        self.horizontalLayout_5.addWidget(self.runcirclebtn)
        self.verticalLayout_4.addWidget(self.frame_2)
        spacerItem = QtWidgets.QSpacerItem(20, 574, QtWidgets.QSizePolicy.Minimum, QtWidgets.QSizePolicy.Expanding)
        self.verticalLayout_4.addItem(spacerItem)
        self.verticalLayout_3.addWidget(self.widget)
        self.editscrollarea.setWidget(self.scrollAreaWidgetContents_2)
        self.verticalLayout.addWidget(self.editscrollarea)
        self._editdock.setWidget(self.dockWidgetContents)
        FaterMainWin.addDockWidget(QtCore.Qt.DockWidgetArea(2), self._editdock)
        self.stadock = QtWidgets.QDockWidget(FaterMainWin)
        self.stadock.setMinimumSize(QtCore.QSize(250, 41))
        self.stadock.setStyleSheet("border-color: rgb(19, 22, 6);")
        self.stadock.setAllowedAreas(QtCore.Qt.LeftDockWidgetArea|QtCore.Qt.RightDockWidgetArea)
        self.stadock.setObjectName("stadock")
        self.dockWidgetContents_2 = QtWidgets.QWidget()
        self.dockWidgetContents_2.setObjectName("dockWidgetContents_2")
        self.stadock.setWidget(self.dockWidgetContents_2)
        FaterMainWin.addDockWidget(QtCore.Qt.DockWidgetArea(1), self.stadock)
        self.FIleTool = QtWidgets.QToolBar(FaterMainWin)
        self.FIleTool.setObjectName("FIleTool")
        FaterMainWin.addToolBar(QtCore.Qt.TopToolBarArea, self.FIleTool)
        self.EditTool = QtWidgets.QToolBar(FaterMainWin)
        self.EditTool.setObjectName("EditTool")
        FaterMainWin.addToolBar(QtCore.Qt.TopToolBarArea, self.EditTool)
        self.RunTool = QtWidgets.QToolBar(FaterMainWin)
        self.RunTool.setObjectName("RunTool")
        FaterMainWin.addToolBar(QtCore.Qt.TopToolBarArea, self.RunTool)
        self.runDock = QtWidgets.QDockWidget(FaterMainWin)
        self.runDock.setMinimumSize(QtCore.QSize(250, 163))
        self.runDock.setMaximumSize(QtCore.QSize(524287, 200))
        self.runDock.setObjectName("runDock")
        self.dockWidgetContents_4 = QtWidgets.QWidget()
        self.dockWidgetContents_4.setObjectName("dockWidgetContents_4")
        self.verticalLayout_2 = QtWidgets.QVBoxLayout(self.dockWidgetContents_4)
        self.verticalLayout_2.setObjectName("verticalLayout_2")
        self.horizontalLayout_3 = QtWidgets.QHBoxLayout()
        self.horizontalLayout_3.setObjectName("horizontalLayout_3")
        self.label = QtWidgets.QLabel(self.dockWidgetContents_4)
        self.label.setMinimumSize(QtCore.QSize(0, 60))
        font = QtGui.QFont()
        font.setPointSize(26)
        self.label.setFont(font)
        self.label.setStyleSheet("color: rgb(70, 128, 57);")
        self.label.setObjectName("label")
        self.horizontalLayout_3.addWidget(self.label)
        self.label_2 = QtWidgets.QLabel(self.dockWidgetContents_4)
        self.label_2.setMinimumSize(QtCore.QSize(0, 60))
        font = QtGui.QFont()
        font.setPointSize(26)
        self.label_2.setFont(font)
        self.label_2.setStyleSheet("color: rgb(222, 64, 47);")
        self.label_2.setObjectName("label_2")
        self.horizontalLayout_3.addWidget(self.label_2)
        self.verticalLayout_2.addLayout(self.horizontalLayout_3)
        self.horizontalLayout_4 = QtWidgets.QHBoxLayout()
        self.horizontalLayout_4.setObjectName("horizontalLayout_4")
        self.pushButton = QtWidgets.QPushButton(self.dockWidgetContents_4)
        self.pushButton.setMinimumSize(QtCore.QSize(0, 50))
        self.pushButton.setStyleSheet("background-color: rgb(141, 192, 107);")
        self.pushButton.setObjectName("pushButton")
        self.horizontalLayout_4.addWidget(self.pushButton)
        spacerItem1 = QtWidgets.QSpacerItem(40, 20, QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Minimum)
        self.horizontalLayout_4.addItem(spacerItem1)
        self.pushButton_2 = QtWidgets.QPushButton(self.dockWidgetContents_4)
        self.pushButton_2.setMinimumSize(QtCore.QSize(0, 50))
        self.pushButton_2.setStyleSheet("background-color: rgb(220, 85, 38);")
        self.pushButton_2.setObjectName("pushButton_2")
        self.horizontalLayout_4.addWidget(self.pushButton_2)
        self.verticalLayout_2.addLayout(self.horizontalLayout_4)
        self.runDock.setWidget(self.dockWidgetContents_4)
        FaterMainWin.addDockWidget(QtCore.Qt.DockWidgetArea(1), self.runDock)
        self.actionOpen = QtWidgets.QAction(FaterMainWin)
        icon3 = QtGui.QIcon()
        icon3.addPixmap(QtGui.QPixmap(":/ico/Open.ico"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.actionOpen.setIcon(icon3)
        self.actionOpen.setObjectName("actionOpen")
        self.actionSave = QtWidgets.QAction(FaterMainWin)
        icon4 = QtGui.QIcon()
        icon4.addPixmap(QtGui.QPixmap(":/ico/Save.ico"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.actionSave.setIcon(icon4)
        self.actionSave.setObjectName("actionSave")
        self.actionSave_as = QtWidgets.QAction(FaterMainWin)
        icon5 = QtGui.QIcon()
        icon5.addPixmap(QtGui.QPixmap(":/ico/Saveas.ico"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.actionSave_as.setIcon(icon5)
        self.actionSave_as.setObjectName("actionSave_as")
        self.actionEdit = QtWidgets.QAction(FaterMainWin)
        icon6 = QtGui.QIcon()
        icon6.addPixmap(QtGui.QPixmap(":/ico/Edit.ico"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.actionEdit.setIcon(icon6)
        self.actionEdit.setObjectName("actionEdit")
        self.actionStatistics = QtWidgets.QAction(FaterMainWin)
        icon7 = QtGui.QIcon()
        icon7.addPixmap(QtGui.QPixmap(":/ico/Statistics.ico"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.actionStatistics.setIcon(icon7)
        self.actionStatistics.setObjectName("actionStatistics")
        self.actionTo_select_line = QtWidgets.QAction(FaterMainWin)
        self.actionTo_select_line.setIcon(icon1)
        self.actionTo_select_line.setObjectName("actionTo_select_line")
        self.actionTo_end = QtWidgets.QAction(FaterMainWin)
        self.actionTo_end.setIcon(icon2)
        self.actionTo_end.setObjectName("actionTo_end")
        self.actionSetting = QtWidgets.QAction(FaterMainWin)
        icon8 = QtGui.QIcon()
        icon8.addPixmap(QtGui.QPixmap(":/ico/settings.ico"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.actionSetting.setIcon(icon8)
        self.actionSetting.setObjectName("actionSetting")
        self.menuFile.addAction(self.actionOpen)
        self.menuFile.addAction(self.actionSave)
        self.menuFile.addAction(self.actionSave_as)
        self.menuView.addAction(self.actionEdit)
        self.menuView.addAction(self.actionStatistics)
        self.menuRun.addAction(self.actionTo_select_line)
        self.menuRun.addAction(self.actionTo_end)
        self.menuSetting.addAction(self.actionSetting)
        self.menubar.addAction(self.menuFile.menuAction())
        self.menubar.addAction(self.menuView.menuAction())
        self.menubar.addAction(self.menuRun.menuAction())
        self.menubar.addAction(self.menuSetting.menuAction())
        self.FIleTool.addAction(self.actionOpen)
        self.FIleTool.addSeparator()
        self.FIleTool.addAction(self.actionSave)
        self.FIleTool.addSeparator()
        self.FIleTool.addAction(self.actionSave_as)
        self.EditTool.addAction(self.actionEdit)
        self.EditTool.addSeparator()
        self.EditTool.addAction(self.actionStatistics)
        self.RunTool.addAction(self.actionTo_select_line)
        self.RunTool.addSeparator()
        self.RunTool.addAction(self.actionTo_end)
        self.RunTool.addSeparator()
        self.RunTool.addAction(self.actionSetting)

        self.retranslateUi(FaterMainWin)
        QtCore.QMetaObject.connectSlotsByName(FaterMainWin)

    def retranslateUi(self, FaterMainWin):
        _translate = QtCore.QCoreApplication.translate
        FaterMainWin.setWindowTitle(_translate("FaterMainWin", "FATER"))
        self.menuFile.setTitle(_translate("FaterMainWin", "File"))
        self.menuView.setTitle(_translate("FaterMainWin", "View"))
        self.menuRun.setTitle(_translate("FaterMainWin", "Run"))
        self.menuSetting.setTitle(_translate("FaterMainWin", "setting"))
        self._editdock.setWindowTitle(_translate("FaterMainWin", "Edit"))
        self.stadock.setWindowTitle(_translate("FaterMainWin", "Statistics"))
        self.FIleTool.setWindowTitle(_translate("FaterMainWin", "File tools"))
        self.EditTool.setWindowTitle(_translate("FaterMainWin", "Edit tools"))
        self.RunTool.setWindowTitle(_translate("FaterMainWin", "Run tools"))
        self.runDock.setWindowTitle(_translate("FaterMainWin", "Run"))
        self.label.setText(_translate("FaterMainWin", "203.1"))
        self.label_2.setText(_translate("FaterMainWin", "NG"))
        self.pushButton.setText(_translate("FaterMainWin", "startbtn"))
        self.pushButton_2.setText(_translate("FaterMainWin", "stopbtn"))
        self.actionOpen.setText(_translate("FaterMainWin", "Open"))
        self.actionSave.setText(_translate("FaterMainWin", "Save"))
        self.actionSave_as.setText(_translate("FaterMainWin", "Save as..."))
        self.actionEdit.setText(_translate("FaterMainWin", "Edit"))
        self.actionStatistics.setText(_translate("FaterMainWin", "Statistics"))
        self.actionTo_select_line.setText(_translate("FaterMainWin", "To select line"))
        self.actionTo_end.setText(_translate("FaterMainWin", "To end"))
        self.actionSetting.setText(_translate("FaterMainWin", "setting page"))

import uires_rc
