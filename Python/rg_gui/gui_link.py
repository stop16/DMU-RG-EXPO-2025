from PySide6.QtWidgets import QMainWindow, QPushButton, QLabel, QComboBox, QRadioButton, QFileDialog, QLineEdit
from PySide6.QtCore import Slot, Signal
from PySide6.QtGui import QPixmap

from core_logic import app_manager

class MainWindow(QMainWindow):
    def __init__(self, main_window_instance):
        super().__init__(main_window_instance)
        self.main_window = main_window_instance
        self.app_manager = app_manager()

        self._connect_signals()

    def _connect_signals(self):
        self.main_window.findChild(QPushButton, 'PB_Setting_Connect').clicked.connect(self._callback_PB_Setting_Connect)
        self.main_window.findChild(QPushButton, 'PB_Control_Start').clicked.connect(self._callback_PB_Control_Start)
        self.main_window.findChild(QPushButton, 'PB_Control_Stop').clicked.connect(self._callback_PB_Control_Stop)
        self.main_window.findChild(QPushButton, 'PB_Control_Reset').clicked.connect(self._callback_PB_Control_Reset)

        for i in range(1,6):
            self.main_window.findChild(QLineEdit, f'lineEdit_Port_{i}').editingFinished.connect(self._callback_Port_Info)
    
    def _callback_PB_Setting_Connect(self):
        self.app_manager.serial_handler.open_port(self.app_manager.port_lists)
        print("Callback for Connection")

    def _callback_PB_Control_Start(self):
        self.app_manager.start_scene()
        print("Callback for Start")

    def _callback_PB_Control_Stop(self):
        self.app_manager.stop_scene()
        print("Callback for Control")
    
    def _callback_PB_Control_Reset(self):
        self.app_manager.reset_scene()
        print("Callback for Reset")

    def _callback_Port_Info(self):
        data = []
        for i in range(1,6):
            text = self.main_window.findChild(QLineEdit, f'lineEdit_Port_{i}').text() # Getting All Port info from lineEdit
            data.append(text)
        print(data)
        self.app_manager.signal_serial_port.emit(data)