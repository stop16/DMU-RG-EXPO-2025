/**
 * 2025 RG EXPO Code (Merge ver) Type 3
 * Components
 * 1. 1x Dot Matrix
 *
 * Scenario
 * 1. Set Dot Matrix text '통신중' when receive 'c'
 * 2. Set Dot Matrix text '통신불가' when receive 'n'
 * 3. Set Dot Matrix text '복구중' when receive 'v'
 * 4. Toggle compact/animation mode when receive 'm'
 */
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <string.h>

// I/O Pin Definitions (unchanged)
#define A_High PORTC|=0x01
#define A_Low PORTC&=~0x01
#define B_High PORTC|=0x02
#define B_Low PORTC&=~0x02
#define C_High PORTC|=0x04
#define C_Low PORTC&=~0x04
#define R1_ON PORTD|=0x04
#define R1_OFF PORTD&=~0x04
#define G1_ON PORTD|=0x08
#define G1_OFF PORTD&=~0x08
#define B1_ON PORTD|=0x10
#define B1_OFF PORTD&=~0x10
#define R2_ON PORTD|=0x20
#define R2_OFF PORTD&=~0x20
#define G2_ON PORTD|=0x40
#define G2_OFF PORTD&=~0x40
#define B2_ON PORTD|=0x80
#define B2_OFF PORTD&=~0x80
#define Clk_enable PORTB|=0x01
#define Clk_disable PORTB&=~0x01
#define OE_enable PORTB&=~0x02
#define OE_disable PORTB|=0x02
#define LE_enable PORTB|=0x04
#define LE_disable PORTB&=~0x04

#define MY_TYPE 8

// --- Korean Character Font Data (15 columns each) ---
// '통'
#define CHAR_TONG 0x0000, 0x0080, 0x7c9c, 0x54a2, 0x54a2, 0x57a2, 0x54a2, 0x54a2, 0x009c, 0x0000 //통
// '신'
#define CHAR_SIN 0x0080, 0x033e, 0x0602, 0x7c02, 0x0602, 0x0302, 0x0082, 0x0002, 0x7f82, 0x0000 //신
// '중'
#define CHAR_JOONG 0x0080, 0x408c, 0x4292, 0x4692, 0x4c92, 0x78f2, 0x78f2, 0x4c92, 0x4692, 0x4292, 0x408c, 0x0080, 0x0000 //중
// '불'
#define CHAR_BUL 0x0000, 0x0080, 0x7eae, 0x12aa, 0x12aa, 0x12aa, 0x12ea, 0x12aa, 0x12aa, 0x7eba, 0x0080, 0x0000 //불
// '가'
#define CHAR_GA 0x1000, 0x1004, 0x1004, 0x1008, 0x1010, 0x1060, 0x1180, 0x1e00, 0x0000, 0x0000, 0x7ffe, 0x0080, 0x0080, 0x0000 //가
// '복'
#define CHAR_BOK  0x0000, 0x0000, 0x4040, 0x4040, 0x4040, 0x7e00, 0x0000, 0x0000, 0x0000, 0x0080, 0x8080, 0x8080, 0xfe80, 0x8080, 0x8000, 0x0000 //복
// '구'
#define CHAR_GU 0x0000, 0x0000, 0x4040, 0x4040, 0x4040, 0x7e00, 0x0000, 0x0000, 0x0000, 0x0080, 0x8080, 0x8080, 0xfe80, 0x8080, 0x8000, 0x0000 //구

// --- Normal Fonts (with animation padding) ---
const uint16_t disconnected_font[] PROGMEM = {
  // 공백 (16열)
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  CHAR_TONG, CHAR_SIN, CHAR_BUL, CHAR_GA,
  // 공백 (16열)
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
};

const uint16_t con_font[] PROGMEM = {
  // 공백 (16열)
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  CHAR_TONG, CHAR_SIN, CHAR_JOONG,
  // 공백 (16열)
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
};

const uint16_t repair_font[] PROGMEM = {
  // 공백 (16열)
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  CHAR_BOK, CHAR_GU, CHAR_JOONG,
  // 공백 (16열)
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
}

// --- Compact Fonts (Minimal Spacing for static display) ---
const uint16_t disconnected_font_compact[] PROGMEM = {
  CHAR_TONG, CHAR_SIN, CHAR_BUL, CHAR_GA,
};

const uint16_t con_font_compact[] PROGMEM = {
  CHAR_TONG, CHAR_SIN, CHAR_JOONG,
};

const uint16_t repair_font_compact[] PROGMEM = {
  CHAR_BOK, CHAR_GU, CHAR_JOONG,
}

// 각 텍스트의 총 컬럼 수를 정의
const int DISCONNECTED_COLUMNS = sizeof(disconnected_font) / sizeof(disconnected_font[0]);
const int CONNECTED_COLUMNS = sizeof(con_font) / sizeof(con_font[0]);
const int REPAIR_COLUMNS = sizeof(repair_font) / sizeof(repair_font[0]);
const int DISCONNECTED_COLUMNS_COMPACT = sizeof(disconnected_font_compact) / sizeof(disconnected_font_compact[0]);
const int CONNECTED_COLUMNS_COMPACT = sizeof(con_font_compact) / sizeof(con_font_compact[0]);
const int REPAIR_COLUMNS_COMPACT = sizeof(repair_font_compact) / sizeof(repair_font_compact[0]);

// 모든 폰트 중 가장 큰 크기를 버퍼 크기로 정의
#define MAX_FONT_COLUMNS DISCONNECTED_COLUMNS

// --- Global Variables ---
uint16_t active_font_buffer[MAX_FONT_COLUMNS];
volatile int current_font_total_columns;
volatile int scroll_offset = 0;
volatile bool needs_update = true;
uint8_t g_color = 0;

// NEW: 모드 관리를 위한 상태 변수
volatile bool isCompactMode = false;
char currentTextState = 'n'; // 'n': 통신불가, 'c': 통신중, 'v': 복구중

// 함수 프로토타입 선언
void updateTextAndMode();

// 동적으로 폰트 데이터를 로드하는 함수
void load_font(const uint16_t *source_font, int total_columns) {
  for (int i = 0; i < total_columns; ++i) {
    active_font_buffer[i] = pgm_read_word(&source_font[i]);
  }
  // 나머지 버퍼는 0으로 채움
  for (int i = total_columns; i < MAX_FONT_COLUMNS; ++i) {
    active_font_buffer[i] = 0x0000;
  }
  current_font_total_columns = total_columns;
  scroll_offset = 0;
  needs_update = true;
}

uint16_t display_buffer_left[16];
uint16_t display_buffer_right[16];

// 스크롤 애니메이션을 위한 타이머 인터럽트
ISR(TIMER2_OVF_vect) {
  static int tick_count = 0;
  const int SCROLL_INTERVAL = 500;

  tick_count++;
  if (tick_count >= SCROLL_INTERVAL) {
    tick_count = 0;
    scroll_offset++;
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
    if (current_font_total_columns > 0) {
       // 애니메이션 모드일 때만 순환
      if (!isCompactMode) {
        font_col_index %= current_font_total_columns;
      }
    }
    
    if(font_col_index < current_font_total_columns) {
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

// NEW: 현재 상태에 맞춰 폰트와 애니메이션을 설정하는 함수
void updateTextAndMode() {
  if (isCompactMode) {
    // 압축 모드: 애니메이션 중지, 압축 폰트 로드
    TIMSK2 = 0x00; // 타이머 인터럽트 비활성화
    scroll_offset = 0; // 스크롤 위치 초기화
    if (currentTextState == 'c') {
      load_font(con_font_compact, CONNECTED_COLUMNS_COMPACT);
    } else if (currentTextState == 'n') {
      load_font(disconnected_font_compact, DISCONNECTED_COLUMNS_COMPACT);
    } else {
      load_font(repair_font_compact, REPAIR_COLUMNS_COMPACT); 
    }
  } else {
    // 일반 모드: 일반 폰트 로드 후 애니메이션 시작
    if (currentTextState == 'c') {
      load_font(con_font, CONNECTED_COLUMNS);
    } else if (currentTextState == 'n') {
      load_font(disconnected_font, DISCONNECTED_COLUMNS);
    } else {
      load_font(repair_font, REPAIR_COLUMNS);
    }
    TIMSK2 = 0x01; // 타이머 인터럽트 활성화
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

  // 초기 상태 설정 ('통신불가', 일반 모드)
  currentTextState = 'n';
  g_color = 1; // RED
  updateTextAndMode(); // 새 핸들러를 호출하여 디스플레이 초기화

  Serial.println(MY_TYPE);
}

void loop() {
  if (Serial.available()) {
    char input = Serial.read();
    switch (input) {
      case 'n':
        g_color = 1; // RED
        if (currentTextState != 'n') {
            currentTextState = 'n';
            updateTextAndMode();
        }
        break;
      case 'c':
        g_color = 2; // GREEN
        if (currentTextState != 'c') {
            currentTextState = 'c';
            updateTextAndMode();
        }
        break;
      case 'v':
        g_color = 1;
        if (currentTextState != 'v') {
          currentTextState = 'v';
          updateTextAndMode();
        }
        break;
      case 'm': // NEW: 모드 전환
        isCompactMode = !isCompactMode;
        updateTextAndMode();
        break;
      case 'q':
        Serial.println(MY_TYPE);
        break;
      default:
        break;
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
        case 0: R1_OFF; B1_OFF; G1_OFF; R2_OFF; B2_OFF; G2_OFF; break;
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
