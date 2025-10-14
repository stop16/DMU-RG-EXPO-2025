"""Expo 장치 자동 데모 시퀀스를 실행하는 간단한 스크립트."""

import expo_device_manager as edm
import time

# 연결할 시리얼 포트 목록
PORT_LIST = ['COM11','COM16','COM26','COM20','COM22','COM25']

# DeviceManager 인스턴스 생성
device_manager = edm.DeviceManager()

# 연결 및 장치 매핑(초기 준비 시간 때문에 약간의 대기 포함)
device_manager.connect(PORT_LIST)
device_manager.activate()

# 초기 상태 구성
device_manager.reset_environment()

# 데모 시퀀스 시작
device_manager.start_demo_sequence()

# 데모 시퀀스가 완료될 때까지 대기
# start_demo_sequence()가 내부적으로 스레드를 사용한다면
# 충분한 시간 동안 메인 스레드를 블로킹해야 함
try:
    # 데모가 진행되는 동안 메인 스레드 유지
    # 예상 데모 시간 또는 무한 대기 (Ctrl+C로 중단 가능)
    print("데모 시퀀스 실행 중... (Ctrl+C로 중단 가능)")
    while True:
        time.sleep(1)
except KeyboardInterrupt:
    print("\n사용자에 의해 중단됨")

# 종료 루틴: 데모 이후 장치를 초기화하고 연결 해제
device_manager.reset_environment()
device_manager.disconnect()
print("Automated demo finished")
