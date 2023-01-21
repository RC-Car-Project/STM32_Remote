#include "pin.h"

bool HC05found_baud=false;

void HC05_start() {
  #if is_test_mode == true
    serial_FTDI.println("Start HC-05");
  #endif
    serial_HC.begin(115200);
    delay(1000);
    /*serial_HC.print("AT+VERSION");
    long st = millis();
    HC05found_baud = true;
    bool hasSearch = false;
    while(!serial_HC.available()) {
      delay(10);
      if(st + 1500 < millis()) {
        serial_HC.end();
        #if is_test_mode == true
          serial_FTDI.println("try to found baud ...");
        #endif
        HC05found_baud = false;
        hasSearch = true;
        find_baud_rate();
        break;
      }
    }
    if(HC05found_baud & !hasSearch) {
      #if is_test_mode == true
        serial_FTDI.println("HC05 VERSION: "+serial_HC.readString());
      #endif
    } */

}

void find_baud_rate(){
  HC05found_baud=false;
  int8_t baud_index=-1;
  while(baud_index<6){
    baud_index++;
    serial_HC.begin(allBauds[baud_index]); //Initialise software serial

    #if is_test_mode == true
      serial_FTDI.println("Testing "+String(allBauds[baud_index])+" Baud ...");
    #endif

    delay(1000);
    serial_HC.print("AT+VERSION");
    delay(2500);
    if (serial_HC.available()){
      HC05found_baud=true;

      #if is_test_mode == true
        serial_FTDI.println(serial_HC.readString()+" at "+String(allBauds[baud_index])+" Baud");
      #endif

      serial_HC.print("AT+BAUD8");
      delay(100);
      serial_HC.end();
      delay(100);
      serial_HC.begin(115200);
      break;
    }
    if (!HC05found_baud)  { serial_HC.end(); }
  }
  #if is_test_mode == true
   serial_FTDI.println("end");
  #endif
  if (!HC05found_baud) { 
    while(true){
      buzzer(50);
      buzzer(50);
      buzzer(50);
      delay(300);
    }
  }
}


void HC05_update() {
  while(serial_HC.available()) {
    serial_FTDI.print(serial_HC.read());
  }
}