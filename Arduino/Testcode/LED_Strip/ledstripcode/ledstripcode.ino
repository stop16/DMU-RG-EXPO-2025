#include <FastLED.h>

// LED 스트립 설정
#define LED_PIN 2          // 데이터 핀 (2번 포트)
#define NUM_LEDS 120       // LED 개수
#define LED_TYPE WS2812B   // LED 타입
#define COLOR_ORDER GRB    // 색상 순서

// LED 배열 선언
CRGB leds[NUM_LEDS];

void setup() {
  // FastLED 초기화
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(100); // 밝기 설정 (0-255)
  
  // 시리얼 모니터 초기화 (디버깅용)
  Serial.begin(9600);
  Serial.println("RGB LED Strip 제어 시작");
}

void loop() {
  // 적색으로 5초간 켜기
  Serial.println("LED 켜기 - 적색");
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Red;  // 적색으로 설정
  }
  FastLED.show();  // LED에 색상 적용
  delay(5000);     // 5초 대기
  
  // 3초간 끄기
  Serial.println("LED 끄기");
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Black;  // 검은색(꺼짐)으로 설정
  }
  FastLED.show();  // LED에 색상 적용
  delay(3000);     // 3초 대기

  // 색으로 5초간 켜기
  Serial.println("LED 켜기 - 녹색");
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Green;  // 녹색으로 설정
  }
  FastLED.show();  // LED에 색상 적용
  delay(5000);     // 5초 대기

  Serial.println("LED 끄기");
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Black;  // 검은색(꺼짐)으로 설정
  }
  FastLED.show();  // LED에 색상 적용
  delay(3000);     // 3초 대기
}