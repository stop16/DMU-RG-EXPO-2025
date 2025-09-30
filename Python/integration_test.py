import serial
import time
import threading

# Lock 객체
lock = threading.Lock()

# 지진 효과의 활성 상태를 관리하기 위한 Event 객체
earthquake_active = threading.Event()

# 도트매트릭스 동기화 명령 전송 주기
SYNC_INTERVAL = 10 # 단위: 분

# 지진 발생 시간
EARTHQUAKE_DURATION = 5 # 단위: 초

# 도트매트릭스 상태 저장
dot_matrix_current_state = ""

def thread_dot_matrix_sync():
    """도트 매트릭스 동기화 Thread
    """
    while True:
        time.sleep(60*SYNC_INTERVAL)
        with lock:
            current_state_snapshot = dot_matrix_current_state
        if current_state_snapshot == '통신중':
            for i in range(len(obj_8)):
                port_obj[obj_8[i]].write('c'.encode())
        elif current_state_snapshot == '통신불가':
            for i in range(len(obj_8)):
                port_obj[obj_8[i]].write('n'.encode())

def set_led(color:str):
    """LED 스트립 색상 설정

    Args:
        color (str): RED면 빨강, GREEN이면 초록, OFF면 LED 스트립 끔.
    """
    if color in ['RED', 'GREEN']:
        print(f"LED 스트립 색상 {color}로 설정")
    else:
        print(f"LED 스트립 OFF, 입력 인자: {color}")

    if color == 'RED':
        port_obj[obj_6].write('r'.encode())
        port_obj[obj_7].write('r'.encode())
    elif color == 'GREEN':
        port_obj[obj_6].write('g'.encode())
        port_obj[obj_7].write('g'.encode())
    else:
        port_obj[obj_6].write('a'.encode())
        port_obj[obj_7].write('a'.encode())

def set_dot_matrix(text:str):
    """도트매트릭스 텍스트 설정

    Args:
        text (str): 통신중, 통신불가 중 하나 입력하면 설정됨
    """
    global dot_matrix_current_state
    
    if text == '통신중':
        for i in range(len(obj_8)):
            port_obj[obj_8[i]].write('c'.encode())
        with lock:
            dot_matrix_current_state = "통신중"
    elif text == '통신불가':
        for i in range(len(obj_8)):
            port_obj[obj_8[i]].write('n'.encode())
        with lock:
            dot_matrix_current_state = "통신불가"

def toggle_animation():
    """도트매트릭스 애니메이션 토글
    """
    for i in range(len(obj_8)):
        port_obj[obj_8[i]].write('m'.encode())

def set_earthquake(data:bool):
    if data == True:
        port_obj[obj_7].write('1'.encode()) # 지진 리니어 동작 시작
        time.sleep(0.01) # Delay for communication
        port_obj[obj_7].write('2'.encode()) # 건물 고정핀 내림
        
    elif data == False:
        port_obj[obj_7].write('0'.encode()) # 지진 리니어 동작 중지
        time.sleep(0.01) # Delay for communication
        port_obj[obj_7].write('3'.encode()) # 건물 고정핀 올림

def earthquake_sequence_thread():
    """지진 발생 후 5초 뒤 또는 수동 중지 시 멈추는 스레드 함수
    """
    print("지진 발생")
    earthquake_active.set() # 지진 활성 상태로 플래그 설정
    set_earthquake(True)

    # 기다리되, earthquake_active 이벤트가 clear되면 즉시 중단
    stopped_manually = not earthquake_active.wait(EARTHQUAKE_DURATION)

    if earthquake_active.is_set(): # 아직 이벤트가 설정되어 있다면 (시간 초과로 종료)
        earthquake_active.clear() # 플래그 해제

    set_earthquake(False)
    
    if stopped_manually:
        print("지진 효과가 수동으로 중지되었습니다.")
    else:
        print("지진 효과가 자동으로 중지되었습니다.")

# TODO: 실제 Port 이름으로 변경
port_obj = ['/dev/tty.usbmodem11301', '/dev/tty.usbmodem114101', '/dev/tty.usbmodem114201', '/dev/tty.usbmodem114401', '/dev/tty.usbserial-11430']

# 포트 개방
for i in range(len(port_obj)):
    print(i)
    port_obj[i] = serial.Serial(
        f'{port_obj[i]}', 115200, timeout=1)
    
    if port_obj[i].isOpen():
        pass
    else:
        break

print("Done!")

obj_6 = ""
obj_7 = ""
obj_8 = []
obj_9 = ""

print("안정화 중...")

# 아두이노 부팅 시간 대기해야 함
# 테스트 결과: 5초면 확실히 동작함
# 줄여보면서 테스트 권장
time.sleep(3)

# Query해서 아두이노 펌웨어 타입 확인
for i in range(len(port_obj)):
    port_obj[i].write('q'.encode())
    data = port_obj[i].read()
    time.sleep(1)
    if data == b'9':
        obj_9 = i
    elif data == b'8':
        obj_8.append(i)
    elif data == b'7':
        obj_7 = i
    elif data == b'6':
        obj_6 = i

print("안정화 끝!")

# Thread 시작
dot_sync_thread = threading.Thread(target=thread_dot_matrix_sync)
dot_sync_thread.daemon = True  # 데몬 스레드로 설정하여 메인 프로그램 종료 시 자동 종료
dot_sync_thread.start()

while True:
    a = input(f"Action \n1) 통신중\t 2) 통신 불가\t 3) 애니메이션 변경\t 4) 지진 ON\t 5) 지진 OFF\t e) 종료\n 입력: ")
    if a == "1":
        set_led('GREEN')
        set_dot_matrix('통신중')

    elif a == "2":
        set_led('RED')
        set_dot_matrix('통신불가')
    
    elif a == "3":
        toggle_animation()

    elif a == "4":
        if not earthquake_active.is_set():
            # 지진 효과를 별도의 스레드에서 실행하여 메인 루프를 막지 않음
            earthquake_thread = threading.Thread(target=earthquake_sequence_thread)
            earthquake_thread.daemon = True
            earthquake_thread.start()
        else:
            print("이미 지진 효과가 진행 중입니다.")

    elif a == "5":
        if earthquake_active.is_set():
            earthquake_active.clear()
        else:
            print("현재 지진 효과가 활성화되어 있지 않습니다.")
    
    elif a == "e":
        print("종료")
        break

# 종료 전 모두 초기상태로 원상 복구
set_earthquake(False)
set_dot_matrix('통신중')
set_led('GREEN')

# 포트 닫기
for i in range(len(port_obj)):
    port_obj[i].close()

print("Finish!")
