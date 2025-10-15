/**
 * 2025 RG EXPO Code (Merge ver) Type 2
 * Components
 * 1. 2x CR Servo
 * 2. 1x Servo
 * 3. 2x LED Strip

 * Scenario
 * 1. Send '7' when received 'q' (Query for MCU type)
 * 2. Turn all LED strip to RED when receive 'r'
 * 3. Turn all LED strip to GREEN when receive 'g'
 * 4. Start CR_Servo_1 when receive '1'
 * 5. Start CR_Servo_2 when receive '1'
 * 6. Move Servo_1 down when receive '2'
 * 7. Move Servo_1 up when receive '3'
 * 8. Stop all CR_Servo when receive '0'
 */
 
#include <FastLED.h>
#include <Servo.h>

#define MY_TYPE 7

// Pin Configuration
#define DUMMY_PIN 999
#define PIN_LED_STRIP_1 9
#define PIN_LED_STRIP_2 10
#define PIN_CR_SERVO_1 11
#define PIN_CR_SERVO_2 5
#define PIN_SERVO 3

// LED Strip Related
#define NUM_LEDS_STRIP_1 140
#define NUM_LEDS_STRIP_2 140
#define NUM_LEDS_TOTAL (NUM_LEDS_STRIP_1 + NUM_LEDS_STRIP_2)
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
#define LED_DEFAULT_BRIGHTNESS 100
#define LED_COLOR_OFF 0
#define LED_COLOR_RED 1
#define LED_COLOR_GREEN 2

CRGB leds[NUM_LEDS_TOTAL];

// Servo Related
#define CR_SERVO_OFF 0
#define CR_SERVO_ON 1
#define SERVO_DOWN 0
#define SERVO_UP 1
#define SERVO_STOP 2

Servo CR_Servo_1;
Servo CR_Servo_2;
Servo Servo_1;

// Communication Timeout & Earthquake Control
unsigned long lastCommTime = 0;
const unsigned long COMM_TIMEOUT = 5000; // 5초
bool earthquakeActive = false;
bool recoveryComplete = false;
bool commStarted = false;  // 첫 통신 수신 여부

void led_set_color(uint8_t color, uint8_t brightness);
void servo_set_position(uint8_t position);
void cr_servo_set_state(uint8_t state);

void setup() {
  // UART Setup
  Serial.begin(115200);
  
  // LED Strip Setup
  FastLED.addLeds<LED_TYPE, PIN_LED_STRIP_1, COLOR_ORDER>(leds, NUM_LEDS_STRIP_1);
  FastLED.addLeds<LED_TYPE, PIN_LED_STRIP_2, COLOR_ORDER>(leds + NUM_LEDS_STRIP_1, NUM_LEDS_STRIP_2);
  
  // Servo Setup
  CR_Servo_1.attach(PIN_CR_SERVO_1);
  CR_Servo_2.attach(PIN_CR_SERVO_2);
  Servo_1.attach(PIN_SERVO);
  
  CR_Servo_1.writeMicroseconds(1500);
  CR_Servo_2.writeMicroseconds(1500);
  Servo_1.write(0);
  
  Serial.println(MY_TYPE);
}

void loop() {
  char input;
  
  if (Serial.available()) {
    input = Serial.read();
    
    switch (input) {
      case '0': // CR_SERVO OFF
        cr_servo_set_state(CR_SERVO_OFF);
        servo_set_position(SERVO_UP);
        break;
      case '1': // CR_SERVO ON
        cr_servo_set_state(CR_SERVO_ON);
        servo_set_position(SERVO_DOWN);
        break;
      case '2': // SERVO Down
        servo_set_position(SERVO_DOWN);
        break;
      case '3': // SERVO Up
        servo_set_position(SERVO_UP);
        break;
      case 's': // System Reset
        cr_servo_set_state(CR_SERVO_OFF);
        servo_set_position(SERVO_UP);
        break;
      case 'r': // LED RED
        led_set_color(LED_COLOR_RED, LED_DEFAULT_BRIGHTNESS);
        break;
      case 'g': // LED GREEN
        led_set_color(LED_COLOR_GREEN, LED_DEFAULT_BRIGHTNESS);
        break;
      case 'a': // LED OFF
        led_set_color(LED_COLOR_OFF, LED_DEFAULT_BRIGHTNESS);
        break;
      case 'q': // Type query
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

/**
 * Servo control function
 */
void servo_set_position(uint8_t position)
{
  switch(position) {
    case SERVO_DOWN:
      Servo_1.write(170);
      break;
    case SERVO_UP:
      Servo_1.write(20);
      break;
    case SERVO_STOP:
      Servo_1.write(90);
      break;
    default:
      break;
  }
}

/**
 * CR Servo control function
 */
void cr_servo_set_state(uint8_t state)
{
  switch(state) {
    case CR_SERVO_ON:
      CR_Servo_1.writeMicroseconds(2218.5);
      CR_Servo_2.writeMicroseconds(3000);
      break;
    case CR_SERVO_OFF:
      CR_Servo_1.writeMicroseconds(1500);
      CR_Servo_2.writeMicroseconds(1500);
      break;
    default:
      break;
  }
}
