/*
 * RC카 2-아두이노 시스템: 통신용 마스터 코드 (I2C 통신)
 */
#include <SPI.h>
#include <RH_RF95.h>
#include <Wire.h> 

#define SLAVE_ADDRESS 8 
#define SFD_VALUE 0xAB

struct rg_expo_packet_controller {
  uint8_t SFD;
  uint8_t Payload1_GP1[2];
  uint8_t Payload2_Throttle;
  uint8_t Payload3_Brake;
  uint8_t Payload4_Steering;
  uint8_t Payload5_GP2[2];
  uint8_t CRC8;
};

struct rg_expo_packet_car {
  uint8_t SFD;
  uint8_t Payload1_GP1[2];
  uint8_t Payload2_Battery;
  uint8_t Payload3_RSSI;
  uint8_t Payload4_GP2[4];
  uint8_t CRC8;
};

RH_RF95 rf95;

uint8_t calculate_crc8(const uint8_t *data, size_t len) {
  uint8_t crc = 0;
  for (size_t i = 0; i < len; i++) {
    crc ^= data[i];
    for (int j = 0; j < 8; j++) {
      if (crc & 0x80) crc = (crc << 1) ^ 0xD5; else crc <<= 1;
    }
  }
  return crc;
}

void setup() {
  Serial.begin(9600);
  Wire.begin(); 
  rf95.init();
  rf95.setModemConfig(RH_RF95::Bw500Cr45Sf128);
  rf95.setTxPower(20, false);
  rf95.setModeRx();
  Serial.println("Master (I2C Mode) Ready.");
}

void loop() {
  if (rf95.available()) {
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);

    if (rf95.recv(buf, &len)) {
      if (len != sizeof(rg_expo_packet_controller) || calculate_crc8(buf, sizeof(rg_expo_packet_controller)) != 0) return;
      rg_expo_packet_controller* p = (rg_expo_packet_controller*)buf;
      if (p->SFD != SFD_VALUE) return;

      Wire.beginTransmission(SLAVE_ADDRESS);
      Wire.write(p->Payload2_Throttle);
      Wire.write(p->Payload3_Brake);
      Wire.write(p->Payload4_Steering);
      Wire.endTransmission();

      rg_expo_packet_car car_packet;
      car_packet.SFD = SFD_VALUE;
      car_packet.Payload2_Battery = 218;
      car_packet.Payload3_RSSI = rf95.lastRssi();
      car_packet.CRC8 = 0;
      car_packet.CRC8 = calculate_crc8((uint8_t*)&car_packet, sizeof(car_packet) - 1);
      
      rf95.send((uint8_t*)&car_packet, sizeof(car_packet));
      rf95.waitPacketSent();
      rf95.setModeRx();
    }
  }
}
