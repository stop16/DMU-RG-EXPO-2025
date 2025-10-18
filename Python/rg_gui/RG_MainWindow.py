# -*- coding: utf-8 -*-

################################################################################
## Form generated from reading UI file 'Window.ui'
##
## Created by: Qt User Interface Compiler version 6.10.0
##
## WARNING! All changes made in this file will be lost when recompiling UI file!
################################################################################

from PySide6.QtCore import (QCoreApplication, QDate, QDateTime, QLocale,
    QMetaObject, QObject, QPoint, QRect,
    QSize, QTime, QUrl, Qt)
from PySide6.QtGui import (QBrush, QColor, QConicalGradient, QCursor,
    QFont, QFontDatabase, QGradient, QIcon,
    QImage, QKeySequence, QLinearGradient, QPainter,
    QPalette, QPixmap, QRadialGradient, QTransform)
from PySide6.QtWidgets import (QApplication, QGridLayout, QGroupBox, QLabel,
    QLineEdit, QMainWindow, QPushButton, QSizePolicy,
    QSpacerItem, QVBoxLayout, QWidget)

class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        if not MainWindow.objectName():
            MainWindow.setObjectName(u"MainWindow")
        MainWindow.resize(800, 600)
        MainWindow.setMinimumSize(QSize(800, 600))
        self.centralwidget = QWidget(MainWindow)
        self.centralwidget.setObjectName(u"centralwidget")
        self.gridLayout_2 = QGridLayout(self.centralwidget)
        self.gridLayout_2.setObjectName(u"gridLayout_2")
        self.groupBox_2 = QGroupBox(self.centralwidget)
        self.groupBox_2.setObjectName(u"groupBox_2")
        self.verticalLayout = QVBoxLayout(self.groupBox_2)
        self.verticalLayout.setObjectName(u"verticalLayout")
        self.PB_Control_Start = QPushButton(self.groupBox_2)
        self.PB_Control_Start.setObjectName(u"PB_Control_Start")
        self.PB_Control_Start.setMinimumSize(QSize(100, 100))

        self.verticalLayout.addWidget(self.PB_Control_Start)

        self.PB_Control_Reset = QPushButton(self.groupBox_2)
        self.PB_Control_Reset.setObjectName(u"PB_Control_Reset")
        self.PB_Control_Reset.setMinimumSize(QSize(100, 100))

        self.verticalLayout.addWidget(self.PB_Control_Reset)


        self.gridLayout_2.addWidget(self.groupBox_2, 2, 1, 1, 1)

        self.groupBox = QGroupBox(self.centralwidget)
        self.groupBox.setObjectName(u"groupBox")
        self.gridLayout = QGridLayout(self.groupBox)
        self.gridLayout.setObjectName(u"gridLayout")
        self.label_2 = QLabel(self.groupBox)
        self.label_2.setObjectName(u"label_2")
        self.label_2.setAlignment(Qt.AlignmentFlag.AlignRight|Qt.AlignmentFlag.AlignTrailing|Qt.AlignmentFlag.AlignVCenter)

        self.gridLayout.addWidget(self.label_2, 3, 1, 1, 1)

        self.lineEdit_Port_3 = QLineEdit(self.groupBox)
        self.lineEdit_Port_3.setObjectName(u"lineEdit_Port_3")
        self.lineEdit_Port_3.setMaximumSize(QSize(200, 16777215))

        self.gridLayout.addWidget(self.lineEdit_Port_3, 5, 2, 1, 1)

        self.label_Setting_State_Arduino_1 = QLabel(self.groupBox)
        self.label_Setting_State_Arduino_1.setObjectName(u"label_Setting_State_Arduino_1")
        self.label_Setting_State_Arduino_1.setAlignment(Qt.AlignmentFlag.AlignCenter)

        self.gridLayout.addWidget(self.label_Setting_State_Arduino_1, 1, 3, 1, 1)

        self.lineEdit_Port_5 = QLineEdit(self.groupBox)
        self.lineEdit_Port_5.setObjectName(u"lineEdit_Port_5")
        self.lineEdit_Port_5.setMaximumSize(QSize(200, 16777215))

        self.gridLayout.addWidget(self.lineEdit_Port_5, 9, 2, 1, 1)

        self.label_7 = QLabel(self.groupBox)
        self.label_7.setObjectName(u"label_7")
        self.label_7.setMaximumSize(QSize(16777215, 30))
        self.label_7.setAlignment(Qt.AlignmentFlag.AlignCenter)

        self.gridLayout.addWidget(self.label_7, 0, 3, 1, 1)

        self.label_4 = QLabel(self.groupBox)
        self.label_4.setObjectName(u"label_4")
        self.label_4.setAlignment(Qt.AlignmentFlag.AlignRight|Qt.AlignmentFlag.AlignTrailing|Qt.AlignmentFlag.AlignVCenter)

        self.gridLayout.addWidget(self.label_4, 7, 1, 1, 1)

        self.lineEdit_Port_4 = QLineEdit(self.groupBox)
        self.lineEdit_Port_4.setObjectName(u"lineEdit_Port_4")
        self.lineEdit_Port_4.setMaximumSize(QSize(200, 16777215))

        self.gridLayout.addWidget(self.lineEdit_Port_4, 7, 2, 1, 1)

        self.label_Setting_State_Arduino_2 = QLabel(self.groupBox)
        self.label_Setting_State_Arduino_2.setObjectName(u"label_Setting_State_Arduino_2")
        self.label_Setting_State_Arduino_2.setAlignment(Qt.AlignmentFlag.AlignCenter)

        self.gridLayout.addWidget(self.label_Setting_State_Arduino_2, 3, 3, 1, 1)

        self.horizontalSpacer = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.gridLayout.addItem(self.horizontalSpacer, 1, 0, 1, 1)

        self.PB_Setting_Connect = QPushButton(self.groupBox)
        self.PB_Setting_Connect.setObjectName(u"PB_Setting_Connect")
        self.PB_Setting_Connect.setMinimumSize(QSize(100, 120))

        self.gridLayout.addWidget(self.PB_Setting_Connect, 11, 0, 1, 4)

        self.label_Setting_State_Arduino_3 = QLabel(self.groupBox)
        self.label_Setting_State_Arduino_3.setObjectName(u"label_Setting_State_Arduino_3")
        self.label_Setting_State_Arduino_3.setAlignment(Qt.AlignmentFlag.AlignCenter)

        self.gridLayout.addWidget(self.label_Setting_State_Arduino_3, 5, 3, 1, 1)

        self.lineEdit_Port_2 = QLineEdit(self.groupBox)
        self.lineEdit_Port_2.setObjectName(u"lineEdit_Port_2")
        self.lineEdit_Port_2.setMaximumSize(QSize(200, 16777215))

        self.gridLayout.addWidget(self.lineEdit_Port_2, 3, 2, 1, 1)

        self.lineEdit_Port_1 = QLineEdit(self.groupBox)
        self.lineEdit_Port_1.setObjectName(u"lineEdit_Port_1")
        self.lineEdit_Port_1.setMaximumSize(QSize(200, 16777215))

        self.gridLayout.addWidget(self.lineEdit_Port_1, 1, 2, 1, 1)

        self.label = QLabel(self.groupBox)
        self.label.setObjectName(u"label")
        self.label.setAlignment(Qt.AlignmentFlag.AlignRight|Qt.AlignmentFlag.AlignTrailing|Qt.AlignmentFlag.AlignVCenter)

        self.gridLayout.addWidget(self.label, 1, 1, 1, 1)

        self.label_5 = QLabel(self.groupBox)
        self.label_5.setObjectName(u"label_5")
        self.label_5.setAlignment(Qt.AlignmentFlag.AlignRight|Qt.AlignmentFlag.AlignTrailing|Qt.AlignmentFlag.AlignVCenter)

        self.gridLayout.addWidget(self.label_5, 9, 1, 1, 1)

        self.label_3 = QLabel(self.groupBox)
        self.label_3.setObjectName(u"label_3")
        self.label_3.setAlignment(Qt.AlignmentFlag.AlignRight|Qt.AlignmentFlag.AlignTrailing|Qt.AlignmentFlag.AlignVCenter)

        self.gridLayout.addWidget(self.label_3, 5, 1, 1, 1)

        self.label_Setting_State_Arduino_4 = QLabel(self.groupBox)
        self.label_Setting_State_Arduino_4.setObjectName(u"label_Setting_State_Arduino_4")
        self.label_Setting_State_Arduino_4.setAlignment(Qt.AlignmentFlag.AlignCenter)

        self.gridLayout.addWidget(self.label_Setting_State_Arduino_4, 7, 3, 1, 1)

        self.label_Setting_State_Arduino_5 = QLabel(self.groupBox)
        self.label_Setting_State_Arduino_5.setObjectName(u"label_Setting_State_Arduino_5")
        self.label_Setting_State_Arduino_5.setAlignment(Qt.AlignmentFlag.AlignCenter)

        self.gridLayout.addWidget(self.label_Setting_State_Arduino_5, 9, 3, 1, 1)

        self.label_6 = QLabel(self.groupBox)
        self.label_6.setObjectName(u"label_6")

        self.gridLayout.addWidget(self.label_6, 10, 1, 1, 1)

        self.lineEdit_Port_6 = QLineEdit(self.groupBox)
        self.lineEdit_Port_6.setObjectName(u"lineEdit_Port_6")
        self.lineEdit_Port_6.setMaximumSize(QSize(200, 16777215))

        self.gridLayout.addWidget(self.lineEdit_Port_6, 10, 2, 1, 1)

        self.label_Setting_State_Arduino_6 = QLabel(self.groupBox)
        self.label_Setting_State_Arduino_6.setObjectName(u"label_Setting_State_Arduino_6")
        self.label_Setting_State_Arduino_6.setAlignment(Qt.AlignmentFlag.AlignCenter)

        self.gridLayout.addWidget(self.label_Setting_State_Arduino_6, 10, 3, 1, 1)

        self.pushButton_check_port = QPushButton(self.groupBox)
        self.pushButton_check_port.setObjectName(u"pushButton_check_port")

        self.gridLayout.addWidget(self.pushButton_check_port, 9, 0, 1, 1)

        self.label_port_list_check = QLabel(self.groupBox)
        self.label_port_list_check.setObjectName(u"label_port_list_check")
        self.label_port_list_check.setAlignment(Qt.AlignmentFlag.AlignCenter)

        self.gridLayout.addWidget(self.label_port_list_check, 10, 0, 1, 1)

        self.label_log = QLabel(self.groupBox)
        self.label_log.setObjectName(u"label_log")

        self.gridLayout.addWidget(self.label_log, 7, 0, 1, 1)


        self.gridLayout_2.addWidget(self.groupBox, 2, 0, 1, 1)

        MainWindow.setCentralWidget(self.centralwidget)

        self.retranslateUi(MainWindow)

        QMetaObject.connectSlotsByName(MainWindow)
    # setupUi

    def retranslateUi(self, MainWindow):
        MainWindow.setWindowTitle(QCoreApplication.translate("MainWindow", u"MainWindow", None))
        self.groupBox_2.setTitle(QCoreApplication.translate("MainWindow", u"\uc81c\uc5b4", None))
        self.PB_Control_Start.setText(QCoreApplication.translate("MainWindow", u"\uc2dc\uc5f0 \uc2dc\uc791", None))
        self.PB_Control_Reset.setText(QCoreApplication.translate("MainWindow", u"\ucd08\uae30\ud654", None))
        self.groupBox.setTitle(QCoreApplication.translate("MainWindow", u"\uc5f0\uacb0 \uc124\uc815", None))
        self.label_2.setText(QCoreApplication.translate("MainWindow", u"\uc544\ub450\uc774\ub178 2", None))
        self.label_Setting_State_Arduino_1.setText(QCoreApplication.translate("MainWindow", u"\uc5f0\uacb0 \uc0c1\ud0dc", None))
        self.label_7.setText(QCoreApplication.translate("MainWindow", u"COM PORT", None))
        self.label_4.setText(QCoreApplication.translate("MainWindow", u"\uc544\ub450\uc774\ub178 4", None))
        self.label_Setting_State_Arduino_2.setText(QCoreApplication.translate("MainWindow", u"\uc5f0\uacb0 \uc0c1\ud0dc", None))
        self.PB_Setting_Connect.setText(QCoreApplication.translate("MainWindow", u"\uc5f0\uacb0 / \uc5f0\uacb0 \ud574\uc81c", None))
        self.label_Setting_State_Arduino_3.setText(QCoreApplication.translate("MainWindow", u"\uc5f0\uacb0 \uc0c1\ud0dc", None))
        self.label.setText(QCoreApplication.translate("MainWindow", u"\uc544\ub450\uc774\ub178 1", None))
        self.label_5.setText(QCoreApplication.translate("MainWindow", u"\uc544\ub450\uc774\ub178 5", None))
        self.label_3.setText(QCoreApplication.translate("MainWindow", u"\uc544\ub450\uc774\ub178 3", None))
        self.label_Setting_State_Arduino_4.setText(QCoreApplication.translate("MainWindow", u"\uc5f0\uacb0 \uc0c1\ud0dc", None))
        self.label_Setting_State_Arduino_5.setText(QCoreApplication.translate("MainWindow", u"\uc5f0\uacb0 \uc0c1\ud0dc", None))
        self.label_6.setText(QCoreApplication.translate("MainWindow", u"\uc544\ub450\uc774\ub178 6", None))
        self.label_Setting_State_Arduino_6.setText(QCoreApplication.translate("MainWindow", u"\uc5f0\uacb0 \uc0c1\ud0dc", None))
        self.pushButton_check_port.setText(QCoreApplication.translate("MainWindow", u"\ud3ec\ud2b8 \uc785\ub825 \uc644\ub8cc", None))
        self.label_port_list_check.setText("")
        self.label_log.setText("")
    # retranslateUi

