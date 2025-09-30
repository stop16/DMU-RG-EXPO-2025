/**
 * 2025 RG EXPO Code (Merge ver) Type 1
 * Components
 * 1. 2x LED Strip

 * Scenario
 * 1. Send '7' when received 'q' (Query for MCU type)
 * 2. Turn all LED strip to RED when receive 'r'
 * 3. Turn all LED strip to GREEN when recive 'g'
 * 4. Turn all LED strip off when receive 'a'
 */

#include <FastLED.h>

#define MY_TYPE 6

// Pin Configuration
// TODO: FIX THIS PINOUT TO REAL PIN
#define DUMMY_PIN   999
#define PIN_LED_STRIP_1 9
#define PIN_LED_STRIP_2 10

// LED Strip Related
#define NUM_LEDS_STRIP_1        160     // LED 개수
#define NUM_LEDS_STRIP_2        120     // LED 개수
#define NUM_LEDS_TOTAL          (NUM_LEDS_STRIP_1 + NUM_LEDS_STRIP_2)

#define LED_TYPE                WS2812B // LED 타입
#define COLOR_ORDER             GRB     // 색상 순서

#define LED_DEFAULT_BRIGHTNESS  100

#define LED_COLOR_OFF           0
#define LED_COLOR_RED           1
#define LED_COLOR_GREEN         2
CRGB leds[NUM_LEDS_TOTAL];

void led_set_color(uint8_t color, uint8_t brightness);

void setup() {
  // LED Strip Setup
  FastLED.addLeds<LED_TYPE, PIN_LED_STRIP_1, COLOR_ORDER>(leds, NUM_LEDS_STRIP_1);
  FastLED.addLeds<LED_TYPE, PIN_LED_STRIP_2, COLOR_ORDER>(leds, NUM_LEDS_STRIP_2);
  // UART Setup
  Serial.begin(115200);
  Serial.println(MY_TYPE);
}

void loop() {
  char input;
  if(Serial.available()) {
    input = Serial.read();
    switch (input) {
      case 'r': // LED Strip to RED
        led_set_color(LED_COLOR_RED, LED_DEFAULT_BRIGHTNESS);
        break;
      case 'g': // LED Strip to Green
        led_set_color(LED_COLOR_GREEN, LED_DEFAULT_BRIGHTNESS);
        break;
      case 'a': // LED Strip off
        led_set_color(LED_COLOR_OFF, LED_DEFAULT_BRIGHTNESS);
        break;
      case 'q': // Type query -> Send type number
        Serial.println(MY_TYPE);
        break;
      default:
        break;
    }
  }
}

/**
 * Led color update
 */
void led_set_color(uint8_t color, uint8_t brightness)
{
  FastLED.setBrightness(brightness);
  switch (color) {
    case LED_COLOR_RED:
      for(uint16_t i = 0; i < NUM_LEDS_TOTAL; i++) {
        leds[i] = CRGB::Red;
      }
      break;
    case LED_COLOR_GREEN:
      for(uint16_t i = 0; i < NUM_LEDS_TOTAL; i++) {
        leds[i] = CRGB::Green;
      }
      break;
    case LED_COLOR_OFF:
      for(uint16_t i = 0; i < NUM_LEDS_TOTAL; i++) {
        leds[i] = CRGB::Black;
      }
      break;
    default:
      break;
  }
  FastLED.show();
}