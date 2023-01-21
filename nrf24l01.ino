#include "pin.h"
#include "SPI.h"
#include "RF24.h"




RF24 radio(pin_NRF24_CE, pin_NRF24_CSN);

bool nrf24l01_started = false;
bool nrf24l01_connexion = false;

uint8_t nrf_address[][6] = { "1Node", "2Node" };
bool nrf_radioNumber = 0;
float nrf_payload = 0.0;

uint32_t toSend[7] = {65000,65000,65000,65000,65000,65000,65000};


void nrf24l01_start() {
  #if is_test_mode == true
    serial_FTDI.println("Start NRF24L01");
  #endif
  while (!radio.begin()) {
    buzzer(200);
  }
  radio.setAutoAck(false);

  nrf24l01_started = true;

  radio.setPALevel(RF24_PA_MAX);

  radio.setPayloadSize(sizeof(nrf_payload));
  radio.openWritingPipe(nrf_address[nrf_radioNumber]);
  radio.openReadingPipe(1, nrf_address[!nrf_radioNumber]);

}

bool nrf24l01_send(uint8_t type, uint16_t value) {
    radio.stopListening();
    nrf_payload = float(value) + (float(type) / 10);
    flag = radio.write(&nrf_payload, sizeof(float));
    radio.startListening();
    return flag;
}

bool nrf24l01_s(uint8_t type, uint16_t value) {
    nrf_payload = float(value) + (float(type) / 10);
    flag = radio.write(&nrf_payload, sizeof(float));
    return flag;
}

void stopl() {
    radio.stopListening();
}

void startl() {
    radio.startListening();
}

long lastSentMsgDelay = 0;
int delayId = 0;
uint8_t bytes;
uint32_t temp32;

void nrf24l01_update() {
  uint8_t pipe;
  while(radio.available(&pipe)) {
    uint8_t bytes = radio.getPayloadSize();
    radio.read(&nrf_payload, bytes);
    temp32 = nrf_payload; // value
    temp8 = (nrf_payload - temp32 + 0.01)*10; // type
    #if is_test_mode == true
      serial_FTDI.println(String(temp8)+"-"+String(temp32));
    #endif
    switch(temp8) {
      case 0:
        if(delayId == temp32) {
          #if is_test_mode == true
            serial_FTDI.println("response"+String(millis()-lastSentMsgDelay));
          #endif
          if(!nrf24l01_connexion) {
            nrf24l01_connexion = true;
            digitalWrite(PC13, HIGH);
            delay(200);
            digitalWrite(PC13, LOW);
          }
        }
        break;
      case 1:
        if(temp32 < 10) {
          speed = "0"+String(temp32);
        } else {
          speed = String(temp32);
        }
        toSend[0] = temp32;
        checkToSend();
        break;
      case 2:
        toSend[1] = temp32;
        if(temp32 < 10) {
          tempe = "0"+String(temp32)+"C";
        } else {
          tempe = String(temp32)+"C";
        }
        checkToSend();
        break;
      case 3:
        if(temp32 < 10) {
          amp = "0"+String(temp32)+"A";
        } else {
          amp = String(temp32)+"A";
        }
        toSend[2] = temp32;
        checkToSend();
        break;
      case 4:
        toSend[3] = temp32;
        checkToSend();
        break;
      case 5:
        if(temp32 < 100) {
          if(temp32 < 10) {
            tens = "00"+String(temp32)+"V";
          } else {
            tens = "0"+String(temp32)+"V";
          }
        } else {
          tens = String(temp32)+"V";
        }
        toSend[4] = temp32;
        checkToSend();
        break;
      case 6:
        toSend[5] = temp32;
        checkToSend();
        break;
      case 7:
        toSend[6] = temp32;
        checkToSend();
        break;

    }
  }

  if(lastSentMsgDelay + 100 < millis()) {
    #if is_test_mode == true
      serial_FTDI.println("send update");
    #endif
    lastSentMsgDelay = millis();
    delayId++;
    if(delayId >=100) {
      delayId = 0;
    }
    nrf_payload = float(delayId);
    radio.stopListening();
    bool Ret = radio.write(&nrf_payload, sizeof(float));
    radio.startListening();
  }
 
}

bool nrf_connected() {
  return nrf24l01_connexion;
}

void checkToSend() {
  long start = millis();
  bool flag = true;
  for(uint8_t i = 0; i<7; i++) {
    if(toSend[i] == 65000) {
      flag = false;
      break;
    }
  }
  if(flag) {
    if(useBle) {
      serial_HC.println(String(toSend[0])+"-"+String(toSend[1])+"-"+String(toSend[2])+"-"+String(toSend[4])+"-"+String(toSend[3])+"-"+String(toSend[5])+"-"+String(toSend[6]));
    } else {

    }
    for(uint8_t i = 0; i<7; i++) {
      toSend[i] = 65000;
    }
    serial_FTDI.println("SEND DATA "+String(millis() - start));
  }
}
