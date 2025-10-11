"""Expo 장치 자동 데모 시퀀스를 실행하는 간단한 스크립트."""

import expo_device_manager as edm

# 연결할 시리얼 포트 목록
PORT_LIST = ["COM45", "COM46", "COM47"]

# DeviceManager 인스턴스 생성
device_manager = edm.DeviceManager()

# 연결 및 장치 매핑(초기 준비 시간 때문에 약간의 대기 포함)
device_manager.connect(PORT_LIST)
device_manager.activate()

# 초기 상태 구성
device_manager.reset_environment()

# 데모 시퀀스 시작
device_manager.start_demo_sequence()

# 종료 루틴: 데모 이후 장치를 초기화하고 연결 해제
device_manager.reset_environment()
device_manager.disconnect()

print("Automated demo finished")
