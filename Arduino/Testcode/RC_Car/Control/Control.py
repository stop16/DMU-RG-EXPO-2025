import keyboard
import serial
import time

PORT = 'COM10'
BAUDRATE = 9600

def connect_serial(port, baudrate):
    try:
        ser = serial.Serial(
            port=port,      
            baudrate=baudrate,
            timeout=0.1,
            write_timeout=0.2
        )
        if not ser.isOpen():
            ser.open()
        print(f"✅ 시리얼 포트 {port} 연결됨.")
    except serial.SerialException as e:
        print(f"❌ 오류: 포트 {port} 열 수 없음. 다른 장치가 점유 중일 수 있습니다.")
        print(f"에러 메시지: {e}")
        exit()

# 시리얼 포트 설정 (이 부분은 환경에 맞게 확인해주세요)
try:
    serialHandle = serial.Serial(
        port='COM10',
        baudrate=9600,
        timeout=0.1,
        write_timeout=0.2
    )
    if not serialHandle.isOpen():
        serialHandle.open()
except serial.SerialException as e:
    print(f"오류: 시리얼 포트('COM10')를 열 수 없습니다. 포트 번호가 맞는지, 다른 프로그램이 사용 중이지 않은지 확인해주세요.")
    print(f"에러 메시지: {e}")
    exit()

# 마지막으로 전송한 데이터를 저장할 변수 (최적화의 핵심)
last_data_str = ""

try:
    print("RC카 조종을 시작합니다. (w, a, s, d 키 사용, 종료z: Ctrl+C)")
    while True:
        # 1. 키보드 입력 값을 바탕으로 현재 상태 결정
        throttle = 100 if keyboard.is_pressed('w') else 0
        brake = 180 if keyboard.is_pressed('s') else 0

        if keyboard.is_pressed('a'):
            steering = 0  # 좌회전
        elif keyboard.is_pressed('d'):
            steering = 255 # 우회전
        else:
            steering = 127 # 중앙

        # 2. 현재 상태를 데이터 문자열로 조합
        # [STX, STX, throttle, brake, steering, ETX, ETX] 형식
        current_data_str = f"0,0,{throttle},{brake},{steering},0,0\n"

        # 3. ★★★ 핵심 최적화 로직 ★★★
        # 마지막으로 보낸 데이터와 현재 데이터가 다를 경우에만 전송
        if current_data_str != last_data_str:
            # 데이터를 바이트로 인코딩하여 시리얼 전송
            
            try:
                serialHandle.write(current_data_str.encode())
                serialHandle.flush()  # flush 위치 이동
                print(f"전송됨: {current_data_str.strip()}")
                last_data_str = current_data_str
            except serial.SerialTimeoutException:
                print("⚠️ write timeout 발생! 시리얼 포트 재연결 중...")
                try:
                    serialHandle.close()
                except:
                    pass
                serialHandle = connect_serial(PORT, BAUDRATE)
            except Exception as e:
                print(f"❌ 예외 발생: {e}")
                try:
                    serialHandle.close()
                except:
                    pass
                break  # 중대한 예외면 종료
            
            # 현재 전송한 데이터를 마지막 데이터로 기록
            last_data_str = current_data_str

        # 4. CPU 사용량을 줄이고 안정적인 통신 간격을 위한 대기

        # 루프가 매우 가벼워졌으므로 sleep 시간을 조금 줄여 반응성을 높일 수 있습니다.
        time.sleep(0.02) # 20ms (초당 50번 체크)
        serialHandle.flush()

except KeyboardInterrupt:
    # 프로그램 종료 시 안전하게 포트 닫기
    serialHandle.close()
    print("\n시리얼 포트를 닫았습니다. 프로그램을 종료합니다.")
except Exception as e:
    print(f"\n예상치 못한 오류 발생: {e}")
    if serialHandle.isOpen():
        serialHandle.close()