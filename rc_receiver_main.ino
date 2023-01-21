#include "pin.h"
uint16_t allBauds[8] = {1200,2400,4800,9600,19200,38400,57600};

HardwareSerial Serial3(PB11, PB10);
HardwareSerial Serial2(PA3, PA2);
uint8_t temp8;
int16_t ntemp16;
uint16_t temp16;
bool flag;
uint8_t potDirSpe = 0;

float opts[6] = {1,0.5,0.5,0.5,0.5,1};

void setup() {
  pinMode(pin_BUZZER, OUTPUT);
  pinMode(PC13, OUTPUT);

  buzzer(100);

  #if is_test_mode == true
    digitalWrite(PC13, HIGH);
    serial_FTDI.begin(115200);
    delay(500);
    serial_FTDI.println("start debug");
    digitalWrite(PC13, LOW);
  #endif

  IO_start();
  STP16_start();
  nrf24l01_start();
  HC05_start();
  lcd_start();
  
}

void loop() {
  nrf24l01_update();
  STP16_update();
  lcd_update();
  IO_update();
  HC05_update();

}

long bipTime = 0;

void buzzer(uint16_t time) {
  bipTime = millis();
  while(bipTime + time > millis()) {
    digitalWrite(pin_BUZZER, HIGH);
    delayMicroseconds(500);
    digitalWrite(pin_BUZZER, LOW);
    delayMicroseconds(500);
  }
}


