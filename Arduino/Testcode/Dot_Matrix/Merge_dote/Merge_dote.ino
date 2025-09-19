#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <string.h>

// I/O Pin Definitions (unchanged)
#define A_High PORTC |= 0x01
#define A_Low PORTC &= ~0x01
#define B_High PORTC |= 0x02
#define B_Low PORTC &= ~0x02
#define C_High PORTC |= 0x04
#define C_Low PORTC &= ~0x04
#define R1_ON PORTD |= 0x04
#define R1_OFF PORTD &= ~0x04
#define G1_ON PORTD |= 0x08
#define G1_OFF PORTD &= ~0x08
#define B1_ON PORTD |= 0x10
#define B1_OFF PORTD &= ~0x10
#define R2_ON PORTD |= 0x20
#define R2_OFF PORTD &= ~0x20
#define G2_ON PORTD |= 0x40
#define G2_OFF PORTD &= ~0x40
#define B2_ON PORTD |= 0x80
#define B2_OFF PORTD &= ~0x80
#define Clk_enable PORTB |= 0x01
#define Clk_disable PORTB &= ~0x01
#define OE_enable PORTB &= ~0x02
#define OE_disable PORTB |= 0x02
#define LE_enable PORTB |= 0x04
#define LE_disable PORTB &= ~0x04

// Character Definitions (unchanged)
#define CHAR_D 0x0000, 0x1FFC, 0x1FFC, 0x180C, 0x180C, 0x180C, 0x180C, 0x180C, 0x1C1C, 0x0FF8, 0x07F0, 0x0000, 0x0000, 0x0000
#define CHAR_I 0x180c, 0x180c, 0x180c, 0x180c, 0x180c, 0x1ffc, 0x1ffc, 0x180c, 0x180c, 0x180c, 0x180c, 0x180c, 0x0000, 0x0000
#define CHAR_S 0x0000, 0x0710, 0x0f98, 0x189c, 0x188c, 0x188c, 0x188c, 0x188c, 0x188c, 0x0cf8, 0x0470, 0x0000, 0x0000, 0x0000
#define CHAR_C 0x0000, 0x0ff8, 0x1ffc, 0x180c, 0x180c, 0x180c, 0x180c, 0x180c, 0x180c, 0x180c, 0x180c, 0x0808, 0x0000, 0x0000
#define CHAR_O 0x0000, 0x0FF8, 0x1FFC, 0x180C, 0x180C, 0x180C, 0x180C, 0x180C, 0x180C, 0x1FFC, 0x0FF8, 0x0000, 0x0000, 0x0000
#define CHAR_N 0x0000, 0x1FFC, 0x1FFC, 0x1C00, 0x1F00, 0x07E0, 0x03F0, 0x007C, 0x001C, 0x1FFC, 0x1FFC, 0x0000, 0x0000, 0x0000
#define CHAR_E 0x1ffc, 0x1ffc, 0x1ffc, 0x19cc, 0x19cc, 0x19cc, 0x19cc, 0x19cc, 0x19cc, 0x19cc, 0x19cc, 0x0000, 0x0000, 0x0000
#define CHAR_T 0x0000, 0x1800, 0x1800, 0x1800, 0x1800, 0x1ffc, 0x1ffc, 0x1800, 0x1800, 0x1800, 0x1800, 0x0000, 0x0000, 0x0000
#define CHAR_CON_C 0x0000, 0x0ff8, 0x1ffc, 0x180c, 0x180c, 0x180c, 0x180c, 0x180c, 0x180c, 0x180c, 0x180c, 0x0808, 0x0000, 0x0000
#define CHAR_CON_D 0x0000, 0x1FFC, 0x1FFC, 0x180C, 0x180C, 0x180C, 0x180C, 0x180C, 0x1C1C, 0x0FF8, 0x07F0, 0x0000, 0x0000, 0x0000

// 각 텍스트의 총 컬럼 수를 정의
const int DISCONNECTED_COLUMNS = 200;
const int CONNECTED_COLUMNS = 158;

// "DISCONNECTED" 폰트 데이터 (PROGMEM에 저장)
const uint16_t disconnected_font[] PROGMEM = {
  // 공백 (16열)
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  // D (14열)
  CHAR_D,
  // I (14열)
  CHAR_I,
  // S (14열)
  CHAR_S,
  // C (14열)
  CHAR_1,
  // O (14열)
  CHAR_SHIN,
  // N (14열)
  CHAR_JOONG,
  // N (14열)
  CHAR_JOONG,
  // E (14열)
  CHAR_BUL,
  // C (14열)
  CHAR_1,
  // T (14열)
  CHAR_GA,
  // E (14열)
  CHAR_BUL,
  // D (14열)
  CHAR_D,
  // 공백 (16열)
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
};

// "CONNECTED" 폰트 데이터 (PROGMEM에 저장)
const uint16_t connected_font[] PROGMEM = {
  // 공백 (16열)
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  // C (14열)
  CHAR_CON_C,
  // o (14열)
  CHAR_SHIN,
  // n (14열)
  CHAR_JOONG,
  // n (14열)
  CHAR_JOONG,
  // e (14열)
  CHAR_BUL,
  // c (14열)
  CHAR_CON_C,
  // t (14열)
  CHAR_GA,
  // e (14열)
  CHAR_BUL,
  // d (14열)
  CHAR_CON_D,
  // 공백 (16열)
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
};

// SRAM에 저장될 동적 폰트 버퍼 (최대 크기)
uint16_t active_font_buffer[DISCONNECTED_COLUMNS];
// 현재 폰트의 총 컬럼 수를 저장하는 변수
volatile int current_font_total_columns;
volatile int scroll_offset = 0;
volatile bool needs_update = true;
uint8_t g_color = 0;

// 동적으로 폰트 데이터를 로드하는 함수
void load_font(const uint16_t *source_font, int total_columns) {
  for (int i = 0; i < total_columns; ++i) {
    active_font_buffer[i] = pgm_read_word(&source_font[i]);
  }
  // 나머지 버퍼는 0으로 채움
  for (int i = total_columns; i < DISCONNECTED_COLUMNS; ++i) {
    active_font_buffer[i] = 0x0000;
  }
  current_font_total_columns = total_columns;
  scroll_offset = 0;
  needs_update = true;
}

// --- 나머지 함수들은 변경된 변수명을 사용하도록 수정 ---

uint16_t display_buffer_left[16];
uint16_t display_buffer_right[16];

ISR(TIMER2_OVF_vect) {
  static int tick_count = 0;
  const int SCROLL_INTERVAL = 500;

  tick_count++;
  if (tick_count >= SCROLL_INTERVAL) {
    tick_count = 0;
    scroll_offset++;
    // 현재 폰트의 총 컬럼 수에 맞춰 스크롤 오프셋을 리셋
    if (scroll_offset >= current_font_total_columns) {
      scroll_offset = 0;
    }
    needs_update = true;
  }
  TCNT2 = 0xe7;
}

void update_display_buffers() {
  memset(display_buffer_left, 0, sizeof(display_buffer_left));
  memset(display_buffer_right, 0, sizeof(display_buffer_right));

  for (int col = 0; col < 32; ++col) {
    int font_col_index = scroll_offset + col;
    // 현재 폰트의 총 컬럼 수에 맞춰 인덱스 계산
    if (font_col_index >= current_font_total_columns) {
      font_col_index -= current_font_total_columns;
    }

    uint16_t font_column_data = active_font_buffer[font_col_index];

    for (int row = 0; row < 16; ++row) {
      if ((font_column_data >> row) & 1) {
        if (col < 16) {
          display_buffer_left[row] |= (1 << (15 - col));
        } else {
          display_buffer_right[row] |= (1 << (15 - (col - 16)));
        }
      }
    }
  }
}

void refresh_display() {
  for (int row = 0; row < 16; ++row) {
    row_dynamic(row);

    uint16_t left_data = display_buffer_left[row];
    uint16_t right_data = display_buffer_right[row];

    shift_Register2((right_data >> 8), (right_data & 0xFF), row, g_color);
    shift_Register2((left_data >> 8), (left_data & 0xFF), row, g_color);

    ActivePulse();
  }
}

void setup() {
  DDRB = 0xff;
  DDRC = 0xff;
  DDRD = 0xff;

  TCCR2A = 0x00;
  TCCR2B = 0x04;
  TIMSK2 = 0x01;
  TCNT2 = 0xe7;
  sei();
  Serial.begin(115200);

  // 초기 시작 시 "disconnected" 텍스트와 해당 컬럼 수로 로드
  load_font(disconnected_font, DISCONNECTED_COLUMNS);
}

void loop() {
  if (Serial.available()) {
    char ser = Serial.read();
    if (ser == '1') {
      g_color = 1; // RED
      load_font(disconnected_font, DISCONNECTED_COLUMNS);
    } else if (ser == '2') {
      g_color = 2; // GREEN
      load_font(connected_font, CONNECTED_COLUMNS);
    }
  }

  if (needs_update) {
    update_display_buffers();
    needs_update = false;
  }
  refresh_display();
}

// --- 기타 유틸리티 함수 (변경 없음) ---

void row_dynamic(uint8_t i) {
  uint8_t row_select = i % 8;
  if (row_select & 0b001) A_High; else A_Low;
  if (row_select & 0b010) B_High; else B_Low;
  if (row_select & 0b100) C_High; else C_Low;
}

void shift_Register2(unsigned char high, unsigned char low, unsigned char cnt_row, uint8_t BCD_Num) {
  uint16_t buff = ((uint16_t)high << 8) | low;

  for (int clk = 0; clk < 16; clk++) {
    if ((buff >> clk) & 1) {
      switch (BCD_Num) {
        case 0:
          R1_OFF; B1_OFF; G1_OFF; R2_OFF; B2_OFF; G2_OFF;
          break;
        case 1:
          if (cnt_row < 8) { R1_ON; B1_OFF; G1_OFF; R2_OFF; B2_OFF; G2_OFF; }
          else { R2_ON; B2_OFF; G2_OFF; R1_OFF; B1_OFF; G1_OFF; }
          break;
        case 2:
          if (cnt_row < 8) { R1_OFF; B1_OFF; G1_ON; R2_OFF; B2_OFF; G2_OFF; }
          else { R2_OFF; B2_OFF; G2_ON; R1_OFF; B1_OFF; G1_OFF; }
          break;
      }
    } else {
      R1_OFF; G1_OFF; B1_OFF; R2_OFF; G2_OFF; B2_OFF;
    }
    Clk_enable;
    Clk_disable;
  }
}

void ActivePulse() {
  LE_enable;
  delayMicroseconds(100);
  LE_disable;
  OE_enable;
  delayMicroseconds(600);
  OE_disable;
}