# TOF 테스트 방법

## 단계
1. ToF 주소 확인 방법
  - ToF 주소 스캐너 동작해야 함
  - 스케치 새로 생성해서 아래 코드 붙여넣어 동작 시리얼 모니터에 출력되는 주소값 확인 (Baud Rate: 115200)
```C
#include <Wire.h>

void setup() {
  Wire.begin(); // Initialize the I2C bus
  Serial.begin(115200); // Initialize serial communication for output
  while (!Serial); // Wait for Serial Monitor to open (optional)
  Serial.println("\nI2C Scanner");
}

void loop() {
  byte error, address;
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;
  for (address = 1; address < 127; address++) { // Iterate through all possible 7-bit addresses
    // The i2c_scanner uses the return value of
    // the Wire.endTransmission to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16) {
        Serial.print("0"); // Add leading zero for single-digit addresses
      }
      Serial.print(address, HEX); // Print address in hexadecimal
      Serial.println(" !");
      nDevices++;
    } else if (error == 4) {
      Serial.print("Unknown error at address 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.println(address, HEX);
    }
  }

  if (nDevices == 0) {
    Serial.println("No I2C devices found\n");
  } else {
    Serial.println("done\n");
  }

  delay(5000); // Wait 5 seconds before the next scan
}
```
2. 동작 테스트 방법
  - [이 링크](https://github.com/stop16/DMU-RG-EXPO-2025/blob/main/Arduino/Testcode/ToF/ToF.ino)의 코드 업로드해서 동작 테스트 (Baud Rate: 115200)
  - 실행했을 때
    - 시리얼 모니터에서 거리에 해당하는 값이 뜬다면 성공
    - Failed to detect and initialize sensor! 또는 TIMEOUT이 뜬다면 실패
3. ToF 활성화 방법
- [Merged_Type2.ino](https://github.com/stop16/DMU-RG-EXPO-2025/blob/main/Arduino/Merge/Merged_Type2/Merged_Type2.ino)의 93번째 줄에서 while문 주석 해제
  - 해제하면 ToF 초기화되지 않았을 때 무한루프로 빠지게 됨 -> 동작 차단
```C
  if (!ToF_sensor.init())
  {
    // while (1) {}
  }
```
