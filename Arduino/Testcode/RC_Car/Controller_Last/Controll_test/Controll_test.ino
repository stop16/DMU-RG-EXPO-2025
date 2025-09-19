/*
 * 조종기 컨트롤러 아두이노 코드 (안정성 개선 버전)
 * [변경점]
 * - PC로부터의 시리얼 수신과 LoRa 전송 로직을 분리하여 통신 병목 현상을 해결했습니다.
 * - 일정한 간격(SEND_INTERVAL)으로 LoRa 패킷을 전송하여 안정성을 높였습니다.
 */

#include <SPI.h>
#include <RH_RF95.h>

#define SFD_VALUE 0xAB // 패킷 시작 구분자
#define MAX_BUFFER_SIZE 150 
#define SEND_INTERVAL 100 // LoRa 전송 간격 (100ms = 0.1초)

// 라디오 드라이버의 싱글톤 인스턴스
RH_RF95 rf95;

// --- 전역 제어 변수 ---
// PC로부터 받은 최신 명령을 저장합니다.
volatile uint8_t g_throttle = 0;
volatile uint8_t g_brake = 0;
volatile uint8_t g_steering = 127;
// 기타 데이터 (필요시 사용)
volatile uint8_t g_dArr[7] = {0, 0, 0, 0, 127, 0, 0};

// Controller가 보내는 패킷
struct rg_expo_packet_controller {
  uint8_t SFD;
  uint8_t Payload1_GP1[2];
  uint8_t Payload2_Throttle;
  uint8_t Payload3_Brake;
  uint8_t Payload4_Steering;
  uint8_t Payload5_GP2[2];
  uint8_t CRC8;
};

// Car로부터 받을 패킷
struct rg_expo_packet_car {
  uint8_t SFD;
  uint8_t Payload1_GP1[2];
  uint8_t Payload2_Battery;
  uint8_t Payload3_RSSI;
  uint8_t Payload4_GP2[4];
  uint8_t CRC8;
};

// CRC-8/Maxim-Dow (Polynomial: 0xD5)
uint8_t calculate_crc8(const uint8_t *data, size_t len) {
  uint8_t crc = 0;
  for (size_t i = 0; i < len; i++) {
    crc ^= data[i];
    for (int j = 0; j < 8; j++) {
      if (crc & 0x80) {
        crc = (crc << 1) ^ 0xD5;
      } else {
        crc <<= 1;
      }
    }
  }
  return crc;
}

void setup() {
  Serial.begin(9600);
  // while (!Serial);

  Serial.println("Controller LoRa Initializing...");
  if (!rf95.init()) {
    Serial.println("LoRa init failed");
    while (1);
  }

  // 모뎀 설정(RC카와 동일하게)
  if (!rf95.setModemConfig(RH_RF95::Bw500Cr45Sf128)) {
    Serial.println("setModemConfig failed");
    while (1);
  }

  rf95.setTxPower(20, false); 
  Serial.println("Controller Ready.");
}

unsigned long last_send_time = 0;
char serialBuffer[MAX_BUFFER_SIZE]; 

void loop() {
  // --- Task 1: PC로부터 시리얼 데이터 수신 (항상 실행) ---
  // 들어온 데이터가 있으면, 파싱해서 전역 변수에 저장만 합니다.
  if (Serial.available()) {
    int bytesRead = Serial.readBytesUntil('\n', serialBuffer, MAX_BUFFER_SIZE - 1);
    serialBuffer[bytesRead] = '\0';

    int arrIndex = 0;
    int currentNumber = 0;
    for (int i = 0; i < bytesRead; i++) {
      char c = serialBuffer[i];
      if (c >= '0' && c <= '9') {
        currentNumber = (currentNumber * 10) + (c - '0');
      } else if (c == ',') {
        if (arrIndex < 7) g_dArr[arrIndex++] = currentNumber;
        currentNumber = 0;
      }
    }
    if (arrIndex < 7) g_dArr[arrIndex] = currentNumber;
  }

  // --- Task 2: 일정한 간격으로 LoRa 데이터 전송 ---
  // 마지막으로 전송한 시간으로부터 SEND_INTERVAL(100ms)이 지났으면 전송합니다.
  if (millis() - last_send_time > SEND_INTERVAL) {
    last_send_time = millis(); // 전송 시간 갱신

    // 패킷 데이터 탑재
    rg_expo_packet_controller controller_packet;
    controller_packet.SFD = SFD_VALUE;
    controller_packet.Payload1_GP1[0] = g_dArr[0];
    controller_packet.Payload1_GP1[1] = g_dArr[1];
    controller_packet.Payload2_Throttle = g_dArr[2];
    controller_packet.Payload3_Brake = g_dArr[3];
    controller_packet.Payload4_Steering = g_dArr[4];
    controller_packet.Payload5_GP2[0] = g_dArr[5];
    controller_packet.Payload5_GP2[1] = g_dArr[6];

    // CRC 계산
    controller_packet.CRC8 = 0;
    size_t crc_len = sizeof(controller_packet) - sizeof(controller_packet.CRC8);
    uint8_t crc = calculate_crc8((uint8_t*)&controller_packet, crc_len);
    controller_packet.CRC8 = crc;

    // 패킷 전송
    rf95.send((uint8_t*)&controller_packet, sizeof(controller_packet));
    rf95.waitPacketSent();

    // 응답 대기 및 처리 (이 부분은 그대로 유지)
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    if (rf95.waitAvailableTimeout(200)) { // 응답 대기 시간 넉넉하게
      if (rf95.recv(buf, &len)) {
        // 응답 패킷 검증 로직 (생략)
        rg_expo_packet_car* received_packet = (rg_expo_packet_car*)buf;
        Serial.print("Reply from Car <- Battery: ");
        Serial.print(received_packet->Payload2_Battery);
        Serial.print(", RSSI: -");
        Serial.println(received_packet->Payload3_RSSI);
      }
    } else {
      Serial.println("No reply from Car.");
    }
  }
}
