"""Expo 데모 장치를 제어하는 관리자 모듈."""

from __future__ import annotations

import threading
import time

import serial

DEFAULT_SERIAL_BAUDRATE = 115200
DEFAULT_SERIAL_TIMEOUT = 1
DEFAULT_SERIAL_WRITE_TIMEOUT = 1

# --- 환경 기본값
DEFAULT_SYNC_INTERVAL = 10  # Minutes
DEFAULT_EARTHQUAKE_DURATION = 5  # Seconds


class DeviceManager:
    """시리얼 기반 Expo 장치를 묶어서 제어하는 헬퍼."""

    def __init__(self):
        self.obj_6: int | None = None
        self.obj_7: int | None = None
        self.obj_8: list[int] = []
        self.obj_9: int | None = None

        self.dot_matrix_current_state = ""

        self.lock = threading.Lock()
        # 지진 루틴이 진행 중인지 공유하기 위한 이벤트
        self.earthquake_active = threading.Event()

        self.port_obj: list[serial.Serial | None] = []

    def connect(self, ports: list[str]):
        """시리얼 포트를 열고 장치 핸들을 캐싱."""
        self.port_obj = [None] * len(ports)
        try:
            for i, port in enumerate(ports):
                ser = serial.Serial(
                    port=port,
                    baudrate=DEFAULT_SERIAL_BAUDRATE,
                    timeout=DEFAULT_SERIAL_TIMEOUT,
                    write_timeout=DEFAULT_SERIAL_WRITE_TIMEOUT,
                )
                # 최신 PySerial은 생성 시 곧바로 열릴 수 있으므로 다시 확인
                if not ser.is_open:
                    ser.open()
                self.port_obj[i] = ser
        except Exception:
            for serial_obj in self.port_obj:
                try:
                    if serial_obj and serial_obj.is_open:
                        serial_obj.close()
                except Exception:
                    pass
            raise

    def disconnect(self):
        """모든 장치와의 연결을 해제."""
        for i in range(len(self.port_obj)):
            if self.port_obj[i] and self.port_obj[i].is_open:
                self.port_obj[i].close()

    def activate(self):
        """장치마다 고유 코드를 읽어서 역할을 매핑."""
        time.sleep(3)
        for i in range(len(self.port_obj)):
            if not self.port_obj[i]:
                continue

            self.port_obj[i].write("q".encode())
            data = self.port_obj[i].read()
            time.sleep(1)
            if data == b"9":
                self.obj_9 = i
            elif data == b"8":
                self.obj_8.append(i)
            elif data == b"7":
                self.obj_7 = i
            elif data == b"6":
                self.obj_6 = i

        # 도트 매트릭스 내용이 바뀌었는지 주기적으로 확인
        self.dot_sync_thread = threading.Thread(target=self.thread_dot_matrix_sync)
        self.dot_sync_thread.daemon = True  # 데몬 스레드로 설정해 종료 시 자동 정리
        self.dot_sync_thread.start()

    # --- LED Strip related functions
    def set_led(self, color: str):
        """LED 스트립 상태를 세팅한다.

        Args:
            color (str): RED(빨강), GREEN(초록), OFF(끄기) 중 하나.
        """
        assert self.obj_6 is not None and self.obj_7 is not None, "LED 스트립 미할당"

        if color in ["RED", "GREEN"]:
            print(f"LED 스트립 상태 {color}로 설정")
        else:
            print(f"LED 스트립 OFF, 입력 문자: {color}")

        if color == "RED":  # 빨강
            self.port_obj[self.obj_6].write("r".encode())
            self.port_obj[self.obj_7].write("r".encode())
        elif color == "GREEN":  # 초록
            self.port_obj[self.obj_6].write("g".encode())
            self.port_obj[self.obj_7].write("g".encode())
        else:  # 끄기
            self.port_obj[self.obj_6].write("a".encode())
            self.port_obj[self.obj_7].write("a".encode())

    # --- Dot Matrix related functions
    def thread_dot_matrix_sync(self):
        """도트 매트릭스 표시 내용을 주기적으로 동기화."""
        while True:
            time.sleep(60 * DEFAULT_SYNC_INTERVAL)
            with self.lock:
                current_state_snapshot = self.dot_matrix_current_state

            if current_state_snapshot == "통신중":
                for i in range(len(self.obj_8)):
                    self.port_obj[self.obj_8[i]].write("c".encode())
            elif current_state_snapshot == "통신불가":
                for i in range(len(self.obj_8)):
                    self.port_obj[self.obj_8[i]].write("n".encode())

    def set_dot_matrix(self, text: str):
        """도트매트릭스 메시지를 변경한다.

        Args:
            text (str): '통신중', '복구중', '통신불가' 중 하나.
        """
        assert self.obj_8, "도트매트릭스 포트 미할당"

        if text == "통신중":
            for i in range(len(self.obj_8)):
                self.port_obj[self.obj_8[i]].write("c".encode())
            with self.lock:
                self.dot_matrix_current_state = "통신중"
        elif text == "복구중":
            for i in range(len(self.obj_8)):
                self.port_obj[self.obj_8[i]].write("v".encode())
            with self.lock:
                self.dot_matrix_current_state = "복구중"
        elif text == "통신불가":
            for i in range(len(self.obj_8)):
                self.port_obj[self.obj_8[i]].write("n".encode())
            with self.lock:
                self.dot_matrix_current_state = "통신불가"

    def toggle_animation(self):
        """도트매트릭스 애니메이션을 토글."""
        for i in range(len(self.obj_8)):
            self.port_obj[self.obj_8[i]].write("m".encode())

    # --- Earthquake related functions
    def set_earthquake(self, data: bool):
        """지진 장치에 온/오프 명령을 전달."""
        if data is True:
            self.port_obj[self.obj_7].write("1".encode())  # 지진 리니어 작동 시작
            time.sleep(0.01)  # 통신 안정화를 위한 딜레이
            self.port_obj[self.obj_7].write("2".encode())  # 건물 고정대 흔들림
        elif data is False:
            self.port_obj[self.obj_7].write("0".encode())  # 지진 리니어 작동 중지
            time.sleep(0.01)  # 통신 안정화를 위한 딜레이
            self.port_obj[self.obj_7].write("3".encode())  # 건물 고정대 안정

    def earthquake_sequence_thread(self):
        """지진 효과를 주기적으로 실행하고 자동 해제."""
        print("지진 발생")
        self.earthquake_active.set()  # 지진 활성 상태 공유
        self.set_earthquake(True)

        # 이벤트가 해제되면 즉시 멈추고, 아니면 기본 지속 시간만큼 유지
        stopped_manually = not self.earthquake_active.wait(DEFAULT_EARTHQUAKE_DURATION)

        if self.earthquake_active.is_set():  # 아직 이벤트가 설정되어 있다면(시간 초과 종료)
            self.earthquake_active.clear()  # 플래그 해제

        self.set_earthquake(False)

        if stopped_manually:
            print("지진 효과가 수동으로 중지되었습니다.")
        else:
            print("지진 효과가 자동으로 중지되었습니다.")

    def start_earthquake(self):
        """지진 효과를 별도 스레드에서 실행."""
        if not self.earthquake_active.is_set():
            # 지진 효과는 별도 스레드에서 실행해 메인 루프를 막지 않음
            self.earthquake_thread = threading.Thread(target=self.earthquake_sequence_thread)
            self.earthquake_thread.daemon = True
            self.earthquake_thread.start()
        else:
            print("이미 지진 효과가 진행 중입니다.")

    def stop_earthquake(self):
        """진행 중인 지진 효과 중지."""
        if self.earthquake_active.is_set():
            self.earthquake_active.clear()
        else:
            print("현재 지진 효과가 활성화되어 있지 않습니다.")

    # --- Environment related functions
    def reset_environment(self):
        """전체 환경을 초기 상태로 복원."""
        self.set_led("GREEN")
        self.set_dot_matrix("통신중")
        self.set_earthquake(False)

    # --- Demo related functions
    def start_demo_sequence(self):
        """전체 데모 시퀀스를 새 스레드에서 실행."""
        self.demonstration_thread = threading.Thread(target=self.demo_thread)
        self.demonstration_thread.start()

    def stop_demo_sequence(self):
        """데모 스레드 종료를 대기."""
        self.demonstration_thread.join()

    def demo_thread(self):
        """데모 시퀀스 본체."""
        assert self.obj_9 is not None, "센서(Q) 포트 미할당"

        self.start_earthquake()
        while True:
            # 지진이 끝날 때까지 대기
            if not self.earthquake_active.is_set():
                time.sleep(0.2)
                break
        self.set_dot_matrix("통신불가")
        self.set_led("RED")

        # RC Car start condition
        while True:
            data = self.port_obj[self.obj_9].read()
            if data == b"o":  # Something detected
                # (Optional) Filtering logic here
                break

        self.set_dot_matrix("복구중")
        time.sleep(3)  # Wait 3 seconds
        self.set_dot_matrix("통신중")
        self.set_led("GREEN")
