from PySide6.QtCore import QObject, QThread, Signal, Slot, QIODevice, QByteArray, QTimer
from PySide6.QtSerialPort import QSerialPort, QSerialPortInfo


class app_manager(QObject):

    signal_serial_port = Signal(list)

    def __init__(self, parent = None):
        super().__init__(parent)