# integration_test.py 실행 가이드

## 필요사항
- python3가 설치된 PC

## 구동 방법
- CMD에서 아래의 명령어 입력
```cmd
pip install pyserial
```
- 장치 관리자를 열고, 최하단의 포트(COM&LPT)에서 아두이노의 COM 포트 기록 (COMx 등)
- integration_test.py 내용 수정
  - 51번째 줄의 포트 이름을 예시와 같이 수정
    ```python
    port_obj = ['COM3', 'COM4', 'COM5', 'COM6', 'COM7']
    ```
- 파일 탐색기를 통해 integration_test.py가 있는 폴더로 이동
  - 빈 곳을 우클릭 - 터미널에서 열기 클릭
  - 아래의 명령어 입력
    ```cmd
    python integration_test.py
    ```
- "안정화 끝!" 이라는 명령어가 뜰 때까지 대기
  - 만약 그 전에 오류가 난다면 연결 관련 점검 필요
