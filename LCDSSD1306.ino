#include "pin.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET     -1 
#define SCREEN_ADDRESS 0x3C


#define refresh_Display 100


#if is_test_mode == true
#define NUMFLAKES     10
#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16
long lastDisplay = 0;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET,1000000,1000000);
String speed = "00";
String tempe = "0C";
String amp = "0A";
String tens = "000V";

#endif
void lcd_start() {
  #if is_test_mode == true
    serial_FTDI.println("Start LCDSS1306");
  #endif
  while(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    digitalWrite(pin_BUZZER, HIGH);
    delay(25);
    digitalWrite(pin_BUZZER, LOW);
    delay(25);
    digitalWrite(pin_BUZZER, HIGH);
    delay(25);
    digitalWrite(pin_BUZZER, LOW);
    delay(25);
    digitalWrite(pin_BUZZER, HIGH);
    delay(25);
    digitalWrite(pin_BUZZER, LOW);
    delay(300);
  }
  display.clearDisplay();
}

void lcd_update() {

  if(lastDisplay + refresh_Display < millis()) {
    #if is_test_mode == true
      long starttt = micros();
    #endif
    display.clearDisplay();
    lastDisplay = millis();
    uint8_t trotA = potDirSpe;
    if(trotA < 90) {
      trotA = 90 - trotA;
    } else {
      trotA -= 90;
    }
    trotA /= 2.8125;

    display.fillRect(0, display.height() - trotA , 16, display.height(), 1);
    display.setTextSize(3);
    display.setTextColor(WHITE);
    display.setCursor(30,5);
    display.println(speed);
    display.setTextSize(1);
    display.setCursor(65,19);
    display.println("Km/h");

    display.setCursor(110,2);
    display.println(tempe);

    display.setCursor(110,12);
    display.println(amp);

    display.setCursor(100,22);
    display.println(tens);
    display.display();


    #if is_test_mode == true
      serial_FTDI.println(String(micros() - starttt)+"us display");
    #endif
  }
}


