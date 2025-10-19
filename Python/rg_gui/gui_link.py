from PySide6.QtWidgets import QMainWindow, QPushButton, QLabel, QComboBox, QRadioButton, QFileDialog, QLineEdit
from PySide6.QtCore import Slot, Signal
from PySide6.QtGui import QPixmap

import expo_device_manager as edm

class MainWindow(QMainWindow):
    def __init__(self, main_window_instance):
        super().__init__(main_window_instance)
        self.main_window = main_window_instance
        self.device_manager = edm.DeviceManager()
        self.port_info = ['','','','','','']

        self.connection_status = False

        self._connect_signals()

    def _connect_signals(self):
        self.main_window.findChild(QPushButton, 'PB_Setting_Connect').clicked.connect(self._callback_PB_Setting_Connect)
        self.main_window.findChild(QPushButton, 'PB_Control_Start').clicked.connect(self._callback_PB_Control_Start)
        self.main_window.findChild(QPushButton, 'PB_Control_Reset').clicked.connect(self._callback_PB_Control_Reset)

        self.main_window.findChild(QPushButton, 'pushButton_check_port').clicked.connect(self._callback_check_port)

        self.device_manager.log_msg.connect(self.update_log_label)

        for i in range(1,6):
            self.main_window.findChild(QLineEdit, f'lineEdit_Port_{i}').editingFinished.connect(self._callback_Port_Info)
    
    def _callback_PB_Setting_Connect(self):
        if self.connection_status == False:
            self.device_manager.connect(self.port_info)
            self.device_manager.activate()
            self.device_manager.reset_environment()
            self.connection_status = True
        else:
            self.connection_status = False
            self.device_manager.reset_environment()
            self.device_manager.disconnect()

    def _callback_PB_Control_Start(self):
        self.device_manager.start_demo_sequence()
    
    def _callback_PB_Control_Reset(self):
        self.device_manager.reset_environment()

    def _callback_check_port(self):
        for i in range(1,7):
            text = self.main_window.findChild(QLineEdit, f'lineEdit_Port_{i}').text() # Getting All Port info from lineEdit
            self.port_info[i-1] = text
        self.main_window.findChild(QLabel, 'label_port_list_check').setText(f"{self.port_info}")

    def _callback_Port_Info(self):
        for i in range(1,7):
            text = self.main_window.findChild(QLineEdit, f'lineEdit_Port_{i}').text() # Getting All Port info from lineEdit
            self.port_info[i-1] = text

    def update_log_label(self, data:str):
        self.main_window.findChild(QLabel, 'label_log').setText(data)