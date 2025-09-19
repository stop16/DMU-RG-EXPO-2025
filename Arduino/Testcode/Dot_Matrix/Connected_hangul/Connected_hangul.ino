#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/io.h>

// 핀 정의 (기존과 동일)
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

#define CHAR_C  0x0000, 0x0000, 0x0000, 0x0080, 0x7c9c, 0x54a2, 0x54a2, 0x54a2, 0x57a2, 0x54a2, 0x54a2, 0x54a2, 0x009c, 0x0000, 0x0000, 0x0000 //통
#define CHAR_O  0x0000, 0x0000, 0x0080, 0x01be, 0x0302, 0x0602, 0x7c02, 0x0602, 0x0302, 0x0182, 0x0082, 0x0002, 0x7f82, 0x0000, 0x0000, 0x0000 //신 
#define CHAR_N  0x0000, 0x0000, 0x0080, 0x408c, 0x4292, 0x4692, 0x4c92, 0x78f2, 0x78f2, 0x4c92, 0x4692, 0x4292, 0x408c, 0x0080, 0x0000, 0x0000 //중



// 1. "Connected" 16xN 폰트 데이터 (PROGMEM에 저장)
// 각 값은 16픽셀 높이의 세로 한 줄을 의미합니다.
const uint16_t connected_font[] PROGMEM = {
  // 공백 (16열)
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  // C (8열)
  CHAR_C,
  // o (8열)
  CHAR_O,
  // n (8열)
  CHAR_N,
  // n (8열)

  // 공백 (16열)
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
};
const int FONT_TOTAL_COLUMNS = sizeof(connected_font) / sizeof(uint16_t);

// 2. 전역 변수 선언
uint16_t display_buffer_left[16];   // 왼쪽 16x16 매트릭스의 가로 데이터 버퍼
uint16_t display_buffer_right[16];  // 오른쪽 16x16 매트릭스의 가로 데이터 버퍼
volatile int scroll_offset = 0;     // 스크롤 위치
volatile bool needs_update = true;  // 버퍼 업데이트 필요 여부 플래그

// 3. 인터럽트 서비스 루틴 (ISR) 수정
ISR(TIMER2_OVF_vect) {
  static int tick_count = 0;
  const int SCROLL_INTERVAL = 500; // 0.1ms * 2000 = 200ms = 0.2초

  tick_count++;
  if (tick_count >= SCROLL_INTERVAL) {
    tick_count = 0;
    scroll_offset++;
    // 스크롤이 끝나면 처음으로
    if (scroll_offset >= FONT_TOTAL_COLUMNS) {
      scroll_offset = 0;
    }
    needs_update = true; // 버퍼를 업데이트해야 한다고 메인 루프에 알림
  }
  TCNT2 = 0xe7; // 타이머 초기값 재설정
}

// 4. 데이터 변환(Transposition) 함수 추가
void update_display_buffers() {
  // 버퍼 초기화
  memset(display_buffer_left, 0, sizeof(display_buffer_left));
  memset(display_buffer_right, 0, sizeof(display_buffer_right));

  // 32개의 세로 폰트 데이터를 가로 버퍼 데이터로 변환
  for (int col = 0; col < 32; ++col) {
    int font_col_index = scroll_offset + col;
    if (font_col_index >= FONT_TOTAL_COLUMNS) {
      font_col_index -= FONT_TOTAL_COLUMNS; // 폰트 데이터 순환
    }
    
    uint16_t font_column_data = pgm_read_word(&connected_font[font_col_index]);

    // 16개의 픽셀(비트)을 각각의 가로줄 버퍼에 배치
    for (int row = 0; row < 16; ++row) {
      if ((font_column_data >> row) & 1) {
        if (col < 16) { // 화면의 왼쪽 16열
          display_buffer_left[row] |= (1 << (15 - col));
        } else { // 화면의 오른쪽 16열
          display_buffer_right[row] |= (1 << (15 - (col - 16)));
        }
      }
    }
  }
}

// 5. 화면 출력(POV) 함수 수정
void refresh_display() {
  for (int row = 0; row < 16; ++row) {
    row_dynamic(row); // 행 선택

    uint16_t left_data = display_buffer_left[row];
    uint16_t right_data = display_buffer_right[row];

    // 중요: 오른쪽 매트릭스 데이터 먼저 전송, 그 다음 왼쪽 매트릭스 데이터 전송
    // 만약 스크롤 방향이 반대이면 아래 두 줄의 순서를 바꾸세요.
    shift_Register2((right_data >> 8), (right_data & 0xFF), row);
    shift_Register2((left_data >> 8), (left_data & 0xFF), row);
    
    ActivePulse(); // 데이터 래치 및 출력
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
  sei(); // 전역 인터럽트 허용
}

void loop() {
  if (needs_update) {
    update_display_buffers(); // 데이터 버퍼 업데이트 (필요할 때만)
    needs_update = false;
  }
  refresh_display(); // POV 디스플레이는 계속 실행
}


// --- 아래 함수들은 기존과 동일 (수정 없음) ---

void row_dynamic(uint8_t i) {
  // 코드를 간결하게 수정했지만 기능은 동일합니다.
  uint8_t row_select = i % 8;
  if (row_select & 0b001) A_High; else A_Low;
  if (row_select & 0b010) B_High; else B_Low;
  if (row_select & 0b100) C_High; else C_Low;
}

void shift_Register2(unsigned char high, unsigned char low, unsigned char cnt_row) {
  uint16_t buff = ((uint16_t)high << 8) | low;
  uint8_t BCD_Num = 2; // Pink 색상으로 고정

  for (int clk = 0; clk < 16; clk++) {
    if ((buff >> clk) & 1) {
       if (cnt_row < 8) {
         R1_ON; B1_ON; G1_OFF;
         R2_OFF; B2_OFF; G2_OFF;
       } else {
         R2_ON; B2_ON; G2_OFF;
         R1_OFF; B1_OFF; G1_OFF;
       }
    } else {
      R1_OFF; G1_OFF; B1_OFF;
      R2_OFF; G2_OFF; B2_OFF;
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
