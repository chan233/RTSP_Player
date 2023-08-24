# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file '/home/chan/Desktop/RTSP_Player/viedoframe.ui'
#
# Created by: PyQt5 UI code generator 5.11.3
#
# WARNING! All changes made in this file will be lost!

from PyQt5 import QtCore, QtGui, QtWidgets

class Ui_ViedoFrame(object):
    def setupUi(self, ViedoFrame):
        ViedoFrame.setObjectName("ViedoFrame")
        ViedoFrame.resize(800, 600)
        self.centralwidget = QtWidgets.QWidget(ViedoFrame)
        self.centralwidget.setObjectName("centralwidget")
        self.verticalLayout = QtWidgets.QVBoxLayout(self.centralwidget)
        self.verticalLayout.setObjectName("verticalLayout")
        self.frame = QtWidgets.QFrame(self.centralwidget)
        self.frame.setFrameShape(QtWidgets.QFrame.StyledPanel)
        self.frame.setFrameShadow(QtWidgets.QFrame.Raised)
        self.frame.setObjectName("frame")
        self.verticalLayout.addWidget(self.frame)
        self.horizontalLayout = QtWidgets.QHBoxLayout()
        self.horizontalLayout.setObjectName("horizontalLayout")
        self.label_url = QtWidgets.QLabel(self.centralwidget)
        self.label_url.setObjectName("label_url")
        self.horizontalLayout.addWidget(self.label_url)
        self.lineEdit_url = QtWidgets.QLineEdit(self.centralwidget)
        self.lineEdit_url.setObjectName("lineEdit_url")
        self.horizontalLayout.addWidget(self.lineEdit_url)
        self.pushButton_play = QtWidgets.QPushButton(self.centralwidget)
        self.pushButton_play.setObjectName("pushButton_play")
        self.horizontalLayout.addWidget(self.pushButton_play)
        self.pushButton_record = QtWidgets.QPushButton(self.centralwidget)
        self.pushButton_record.setObjectName("pushButton_record")
        self.horizontalLayout.addWidget(self.pushButton_record)
        self.verticalLayout.addLayout(self.horizontalLayout)
        self.verticalLayout.setStretch(0, 9)
        self.verticalLayout.setStretch(1, 1)
        ViedoFrame.setCentralWidget(self.centralwidget)
        self.menubar = QtWidgets.QMenuBar(ViedoFrame)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 800, 23))
        self.menubar.setObjectName("menubar")
        ViedoFrame.setMenuBar(self.menubar)
        self.statusbar = QtWidgets.QStatusBar(ViedoFrame)
        self.statusbar.setObjectName("statusbar")
        ViedoFrame.setStatusBar(self.statusbar)

        self.retranslateUi(ViedoFrame)
        QtCore.QMetaObject.connectSlotsByName(ViedoFrame)

    def retranslateUi(self, ViedoFrame):
        _translate = QtCore.QCoreApplication.translate
        ViedoFrame.setWindowTitle(_translate("ViedoFrame", "MainWindow"))
        self.label_url.setText(_translate("ViedoFrame", "URL:"))
        self.lineEdit_url.setText(_translate("ViedoFrame", "rtsp://127.0.0.1:8554/123"))
        self.pushButton_play.setText(_translate("ViedoFrame", "play"))
        self.pushButton_record.setText(_translate("ViedoFrame", "record"))

