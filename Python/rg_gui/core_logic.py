from PySide6.QtCore import QObject, QThread, Signal, Slot, QIODevice, QByteArray, QTimer
from PySide6.QtSerialPort import QSerialPort, QSerialPortInfo

ARDUINO_TYPE_1 = ''
ARDUINO_TYPE_2 = ''
ARDUINO_TYPE_3 = ['', '', '']

class serial_manager(QObject):

    status_changed = Signal(str)
    data_received = Signal(list)

    def __init__(self, parent = None):
        super().__init__(parent)
        self._serial_port = [QSerialPort(self) for _ in range(6)]
        for i in range(5):
            self._serial_port[i].readyRead.connect(lambda: QTimer.singleShot(10, self._on_ready_read))

        self.is_connected = False

    def open_port(self, port_name:list, baud_rate:int=115200) -> bool:
        """Serial 포트 열기

        Args:
            port_name (str): port 이름 (COM7, /dev/ttyAMA0, etc...)
            baud_rate (int, optional): 통신 속도 설정, Defaults to 115200.

        Returns:
            bool: 연결되었다면 True, 아니라면 False 반환
        """
        success_cnt = 0
        for i in range(5):
            self._serial_port[i].setPortName(port_name[i])
            self._serial_port[i].setBaudRate(baud_rate)
            if self._serial_port[i].open(QIODevice.ReadWrite):
                success_cnt = success_cnt + 1
            else:
                success_cnt = 0

        if success_cnt >= 6:
            self.is_connected = True
            self.status_changed.emit("Connected")

    def close_port(self):
        """Serial 포트 닫기
        """
        for i in range(5):
            if self._serial_port[i].isOpen():
                self._serial_port[i].close()
        
        self.status_changed.emit("Disconnected")

    @Slot()
    def _on_ready_read(self):
        data = [0,0,0,0,0]
        for i in range(5):
            if self._serial_port[i].bytesAvailable():
                data[i] = self._serial_port[i].readAll().data()
                data[i] = data[i].encode()
        self.data_received.emit(data)

    @Slot()
    def write_bytes_data(self, data:str, index:int):
        if not self._serial_port[index].isOpen():
            self.error_occured.emit("포트가 열려 있지 않습니다.")
            return
        byte_array_to_send = QByteArray(bytes(data))
        bytes_written = self._serial_port[index].write(byte_array_to_send)


class app_manager(QObject):

    signal_serial_port = Signal(list)

    def __init__(self, parent = None):
        super().__init__(parent)

        self.serial_handler = serial_manager()
        self.port_lists = []

        self.signal_serial_port.connect(self.update_signal_port)

    def update_signal_port(self, data:list):
        print(data)
        self.port_lists = data

    def start_scene(self):
        self.serial_handler.write_bytes_data()

    def stop_scene(self):
        pass

    def reset_scene(self):
        pass

class scene_thread(QThread):

    update_signal = Signal(list)

    def run(self):
        self.update_signal.emit(['o', ARDUINO_TYPE_1, 's'])

        while True:
            print()