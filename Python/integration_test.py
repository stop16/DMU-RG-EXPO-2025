import serial
import time

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
        port_obj[obj_7].write('r'.encode())
        port_obj[obj_8].write('r'.encode())
    elif color == 'GREEN':
        port_obj[obj_7].write('g'.encode())
        port_obj[obj_8].write('g'.encode())
    else:
        port_obj[obj_7].write('a'.encode())
        port_obj[obj_8].write('a'.encode())

def set_dot_matrix(text:str):
    """도트매트릭스 텍스트 설정

    Args:
        text (str): 통신중, 통신불가 중 하나 입력하면 설정됨
    """
    if text == '통신중':
        for i in range(len(obj_9)):
            port_obj[obj_9[i]].write('c'.encode())
    elif text == '통신불가':
        for i in range(len(obj_9)):
            port_obj[obj_9[i]].write('n'.encode())


# TODO: 실제 Port 이름으로 변경
port_obj = ['/dev/tty.usbmodem12301', '/dev/tty.usbmodem124101', '/dev/tty.usbmodem124201', '/dev/tty.usbmodem124401', '/dev/tty.usbserial-12430']

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

obj_7 = ""
obj_8 = ""
obj_9 = []

print("안정화 중...")

# 아두이노 부팅 시간 대기해야 함
# 테스트 결과: 5초면 확실히 동작함
# 줄여보면서 테스트 권장
time.sleep(5)

# Query해서 아두이노 펌웨어 타입 확인
for i in range(len(port_obj)):
    port_obj[i].write('q'.encode())
    data = port_obj[i].read()
    time.sleep(1)
    if data == b'9':
        obj_9.append(i)
        print(obj_9)
    elif data == b'8':
        obj_8 = i
    elif data == b'7':
        obj_7 = i

print("안정화 끝!")

# 동작 시작 전 대기, 지워도 됨
time.sleep(3)

# 우선 통신 단절 먼저 표현
set_led('RED')
set_dot_matrix('통신불가')

# 3초 대기
time.sleep(3)

# 통신중 표현
set_led('GREEN')
set_dot_matrix('통신중')

# 3초 대기
time.sleep(3)

# 포트 닫기
for i in range(1,len(port_obj)):
    port_obj[i].close()

print("Finish!")