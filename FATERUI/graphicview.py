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


class GraphicsView(QGraphicsView):

    showImage = QtCore.pyqtSignal(QtGui.QImage)
    roichanged = QtCore.pyqtSignal(QtCore.QRectF)

    def __init__(self):
        super(GraphicsView,self).__init__()
        self._sence = QGraphicsScene()
        self.setScene(self._sence)

        self._sence.setSceneRect(-3000,-3000,9000,9000)

        # pix = QtGui.QPixmap("./RES/grid1.png")
        # self.setBackgroundBrush(QtGui.QBrush(pix))
        # self.setStyleSheet("background: rgb(0, 0, 255);")
        icon = QtGui.QIcon()
        icon.addPixmap(QtGui.QPixmap(":/ico/image.ico"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.setWindowIcon(icon)
        self.setWindowTitle("Camera Image")
        self.setStyleSheet("QScrollBar{background:rgba(246,244,242,50);}")
        self._pen=QtGui.QPen()
        self._pen.setColor(QtGui.QColor.fromRgb(0,255,255))
        self._pen.setWidth(3)

        self._lpen=QtGui.QPen()
        self._lpen.setColor(QtGui.QColor.fromRgb(80,211,90))
        self._lpen.setWidth(2)

        self._PixmapItem = None
        self._selectItem = None

        self._xLineItem = None
        self._yLineItem = None
        # QGraphicsLineItem

        self._scale = 100.0
        self._roiItem = None

        self._editBut = QtCore.Qt.NoButton
        self._editButpos = QtCore.QPoint(-1, -1)
        self._roi = []
        self._img = QtGui.QImage
        self.setAttribute(QtCore.Qt.WA_DeleteOnClose, False)
        self.showImage.connect(self.set_displayImage)

        self._lastPos=QtCore.QPoint(0,0)
        self._lastscalePos=QtCore.QPoint(0,0)

        self.setMouseTracking(True)

        # self.setVerticalScrollBarPolicy(QtCore.Qt.ScrollBarAlwaysOff)
        # self.setHorizontalScrollBarPolicy(QtCore.Qt.ScrollBarAlwaysOff)

        self.setRenderHint(QtGui.QPainter.Antialiasing, False)
        self.setDragMode(QGraphicsView.RubberBandDrag)
        self.setOptimizationFlags(QGraphicsView.DontSavePainterState)
        self.setViewportUpdateMode(QGraphicsView.SmartViewportUpdate)
        # self.installEventFilter()

    def closeEvent(self, QCloseEvent):
        pass

    def showCapImage(self,img):
        self.showImage.emit(img)

    def set_displayImage(self,img):
        scence=self.scene()
        # scence = QGraphicsScene()
        if self._PixmapItem:
            scence.removeItem(self._PixmapItem)
        self._PixmapItem = scence.addPixmap(QtGui.QPixmap.fromImage(img))

    def getRoiRect(self):

        imgRect = self._PixmapItem.mapRectToScene(self._PixmapItem.boundingRect())
        # print 'imgrect',imgRect
        if self._roiItem is not None:
            roi = self._roiItem.mapRectToScene(self._roiItem.rect())
            # print self._roiItem.x(),self._roiItem.y()
            # print 'roirect', roi.x(), roi.y(), roi.width(), roi.height()
            roirect = imgRect.intersected(roi)
            w = roirect.width()
            h = roirect.height()
            roirect.setX(round(roirect.x()-imgRect.x(),2))
            roirect.setY(round(roirect.y()-imgRect.y(),2))
            roirect.setWidth(round(w,2))
            roirect.setHeight(round(h,2))
            # print 'roirect',roirect.x(),roirect.y(),roirect.width(),roirect.height()
            return roirect
        else:
            return QtCore.QRectF(0,0,0,0)

    def leaveEvent(self, QEvent):
        if self._xLineItem is not None:
            self.scene().removeItem(self._xLineItem)
            self._xLineItem = None
        if self._yLineItem is not None:
            self.scene().removeItem(self._yLineItem)
            self._yLineItem = None

    def wheelEvent(self, event):

        self.centerOn(self._lastscalePos)
        if event.angleDelta().y()>0 and self._scale >30:

            self.scale(0.9,0.9)
            self._scale *= 0.9
        elif event.angleDelta().y()<0 and self._scale <250:
            self.scale(1.1,1.1)
            self._scale *= 1.1
        psen = self.mapToScene(event.x(), event.y())
        self.drawCrossLine(psen.x(),psen.y())

    def drawCrossLine(self,x,y):
        if self._xLineItem is None:
            self._xLineItem = self._sence.addLine(x, -5000, x, 9000, self._lpen)
            self._xLineItem.setZValue(3)
        else:
            self._sence.removeItem(self._xLineItem)
            self._xLineItem = self._sence.addLine(x, -5000, x, 9000, self._lpen)
            self._xLineItem.setZValue(3)

        if self._yLineItem is None:
            self._yLineItem = self._sence.addLine(-5000,y,9000,y,self._lpen)
            self._yLineItem.setZValue(3)
        else:
            self._sence.removeItem(self._yLineItem)
            self._yLineItem = self._sence.addLine(-5000,y,9000,y,self._lpen)
            self._yLineItem.setZValue(3)

    def mouseMoveEvent(self, event):
        # print "mouse move 123",event.x(),' ',event.y()
        psen = self.mapToScene(event.x(), event.y())
        self.drawCrossLine(psen.x(),psen.y())
        self._lastscalePos = event.pos()
        if self._editBut == QtCore.Qt.MidButton:

            hBar =self.horizontalScrollBar()
            vBar = self.verticalScrollBar()
            delta = event.pos() - self._lastPos
            hBar.setValue(hBar.value() - delta.x())
            vBar.setValue(vBar.value() - delta.y())
            self._lastPos = event.pos()

        elif self._editBut == QtCore.Qt.LeftButton and self._editButpos.x() !=-1:
            # psen = self.mapToScene(event.x(), event.y())
            pitem = self._roiItem.mapFromScene(psen)
            itempos = psen - self._editButpos
            itemposRoi = psen - pitem
            self._roiItem.setX(itempos.x())
            self._roiItem.setY(itempos.y())
            self.roichanged.emit(self.getRoiRect())

        elif self._editBut == QtCore.Qt.RightButton:
            roi=self._roiItem.rect()
            # psen = self.mapToScene(event.x(), event.y())
            itempos = psen - self._editButpos
            if itempos.x()<0:
                roi.setX(psen.x())
            if itempos.y()<0:
                roi.setY(psen.y())
            roi.setWidth(abs(itempos.x()))
            roi.setHeight(abs(itempos.y()))
            self._roiItem.setRect(roi)
            self.roichanged.emit(self.getRoiRect())

    def mousePressEvent(self, event):
        # print 'widget', event.x(),event.y()
        psen = self.mapToScene(event.x(), event.y())
        pitem = self._PixmapItem.mapFromScene(psen)
        if self._roiItem:
            pitemroi = self._roiItem.mapFromScene(psen)

        self._editBut = event.button()

        if self._editBut == QtCore.Qt.MidButton:
            self._editButpos = pitem
            self.setCursor(QtCore.Qt.ClosedHandCursor)
            self._lastPos = event.pos()

        elif self._roiItem and self._editBut == QtCore.Qt.LeftButton and self._roiItem.contains(pitemroi):
            self._editButpos = pitemroi

        elif self._editBut == QtCore.Qt.RightButton:
            self.setCursor(QtCore.Qt.CrossCursor)
            if self._roiItem:
                self.scene().removeItem(self._roiItem)
            self._roiItem=self._addroiRect(psen.x(),psen.y(),10,10)
            self._editButpos = psen
        else:
            self._editButpos = QtCore.QPoint(-1, -1)

    def mouseReleaseEvent(self, event):
        self.setCursor(QtCore.Qt.ArrowCursor)
        self.setDragMode(QGraphicsView.RubberBandDrag)
        self.setInteractive(True)
        self._editBut = QtCore.Qt.NoButton
        self._editButpos = QtCore.QPoint(-1, -1)
        # self.setDragMode(QGraphicsView.RubberBandDrag)
        # self.setInteractive(True)
        if self._selectItem:
            self.scene().removeItem(self._selectItem)
            self._selectItem = None
        # self.scene().removeItem(self._selectItem)

    def addRect(self,x,y,w,h):
        scence=self.scene()
        if not self._selectItem:
            self._selectItem = scence.addRect(x, y, w, h, self._pen)
        else:
            scence.removeItem(self._selectItem)
            self._selectItem = scence.addRect(x, y, w, h, self._pen)
        scence = QGraphicsScene()
        scence.removeItem()

        self._selectItem.setZValue(1)

    def _addroiRect(self,x,y,w,h):
        scence = self.scene()
        item = scence.addRect(x, y, w, h, self._pen)
        item.setZValue(2)
        return item

