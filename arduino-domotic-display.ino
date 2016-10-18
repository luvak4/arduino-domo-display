#include <VirtualWire.h>   // load virtual-wire library
#include <LiquidCrystal.h> // load LCM library
////////////////////////////////
// pins
////////////////////////////////
const int led_pin_rx = 13;  // led pin
const int receive_pin = 11; // radio rx pin
const int MSG_LEN = 7;      // radio max message lenght
const int RS = 9;           // LCM pin RS
const int Enable = 10;      // LCM pin enable
const int D4 = 5;           // LCM pin D4
const int D5 = 6;           // LCM pin D5
const int D6 = 7;           // LCM pin D6
const int D7 = 8;           // LCM pin D7
////////////////////////////////
// indirizzi radio
////////////////////////////////
#define indirMAESTROdisplay 1236 //rx
////////////////////////////////
// trasmissione radio a display
////////////////////////////////
#define DISPLAYindirizzoLSB    0
#define DISPLAYindirizzoMSB    1
#define DISPLAYcolonna         2
#define DISPLAYriga            3
#define DISPLAYnCaratteri      4
#define DISPLAYinizioTesto     5
#define VELOCITAhi          2000
//
byte BYTEradioDisplay[VW_MAX_MESSAGE_LEN];
String caratteri;
//
#define SIMBluceON             1
#define SIMBluceOFF            0
//
////////////////////////////////
// varie
////////////////////////////////
byte CIFR[]={223,205,228,240,43,146,241,//
	     87,213,48,235,131,6,81,26,//
	     70,34,74,224,27,111,150,22,//
	     138,239,200,179,222,231,212};
const unsigned long mask=0x0000FFFF;
int INDIRIZZO=0;
LiquidCrystal lcd(RS, Enable, D4, D5, D6, D7); 
byte luceON[8] = {
  B00100,
  B01010,
  B10001,
  B10101,
  B01110,
  B01110,
  B00100
};
byte luceOFF[8] = {
  B00100,
  B01110,
  B11111,
  B11111,
  B01110,
  B01110,
  B00100
};

////////////////////////////////
// setup
////////////////////////////////
void setup() {
  pinMode(led_pin_rx,OUTPUT);
  digitalWrite(led_pin_rx,LOW);
  vw_set_rx_pin(receive_pin); 
  vw_setup(VELOCITAhi);       
  vw_rx_start();
  lcd.createChar(SIMBluceON, luceON);
  lcd.createChar(SIMBluceOFF, luceOFF);  
  lcd.begin(20, 4);           // LCM initialize 20 cols 4 rows
  lcd.setCursor(0,0);         // LCM first row, first col
  lcd.print("Valk domotica 2016"); // LCM message
  lcd.write(byte(SIMBluceON));
  lcd.write(byte(SIMBluceOFF));
  //Serial.begin(9600); // debug
}

void loop() {
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;
  //
  if (vw_get_message(BYTEradioDisplay, &buflen)){
    vw_rx_stop();
    // decifratura messaggio XOR
    for (byte n=0; n<27;n++){
      BYTEradioDisplay[n]=BYTEradioDisplay[n] ^ CIFR[n];
    }
    // indirizzo
    INDIRIZZO = BYTEradioDisplay[DISPLAYindirizzoMSB];
    INDIRIZZO = INDIRIZZO << 8;
    INDIRIZZO = INDIRIZZO + BYTEradioDisplay[DISPLAYindirizzoLSB];
    // se indirizzo corretto
    switch (INDIRIZZO){
    case indirMAESTROdisplay:
      digitalWrite(led_pin_rx,HIGH);
      // assembla messaggio
      String msg="";
      for (int n=0; n<BYTEradioDisplay[DISPLAYnCaratteri];n++){
	char t=BYTEradioDisplay[n+DISPLAYinizioTesto];
	msg=msg+t;
      }
      // su display
      lcd.setCursor(BYTEradioDisplay[DISPLAYcolonna],BYTEradioDisplay[DISPLAYriga]);
      lcd.print(msg);
      delay(100);
      digitalWrite(led_pin_rx,LOW);    
      break;
    }
    vw_rx_start();
  }
}
