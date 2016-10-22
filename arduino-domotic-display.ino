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
// indirizzi radio RX
////////////////////////////////
#define MASTRdisplay  100 // info to display
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
String CARATTERI;
////////////////////////////////
// LCM
////////////////////////////////
// CARATTERI personalizzati
#define SIMBluce  0
#define SIMBtermo 1
#define SIMBlivB  2
#define SIMBlivC  3
#define SIMBlivD  4
#define SIMBlivE  5
#define SIMBlivF  6
#define SIMBgiu   7
// CARATTERI interni
#define SIMBsu    B01011110
#define SIMBlivA  B01011111
#define SIMBon    B01101111
#define SIMBoff   'x'
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
byte luce[8]   = {B00100, B01010, B10001, B10101, B01110, B01110, B00100};
byte termo[8]  = {B00100, B01010, B01010, B01010, B11011, B11111, B01110};
byte livB[8]   = {B00000, B00000, B00000, B00000, B00000, B11111, B11111};
byte livC[8]   = {B00000, B00000, B00000, B00000, B11111, B11111, B11111};
byte livD[8]   = {B00000, B00000, B00000, B11111, B11111, B11111, B11111};
byte livE[8]   = {B00000, B00000, B11111, B11111, B11111, B11111, B11111};
byte livF[8]   = {B00000, B11111, B11111, B11111, B11111, B11111, B11111};
byte giu[8]    = {B00000, B00000, B00000, B00000, B10001, B01010, B00100};

  
////////////////////////////////
// setup
////////////////////////////////
void setup() {
  pinMode(led_pin_rx,OUTPUT);
  digitalWrite(led_pin_rx,LOW);
  vw_set_rx_pin(receive_pin); 
  vw_setup(VELOCITAhi);       
  vw_rx_start();
  lcd.createChar(SIMBluce, luce);
  lcd.createChar(SIMBtermo, termo);
  lcd.createChar(SIMBlivB, livB);
  lcd.createChar(SIMBlivC, livC);
  lcd.createChar(SIMBlivD, livD);
  lcd.createChar(SIMBlivE, livE);
  lcd.createChar(SIMBlivF, livF);
  lcd.createChar(SIMBgiu, giu);

  lcd.begin(20, 4);           // LCM initialize 20 cols 4 rows
  lcd.setCursor(0,0);         // LCM first row, first col
  lcd.print("Valk 16 "); // LCM message
  lcd.write(byte(SIMBluce));
  lcd.write(byte(SIMBtermo));
  lcd.write(byte(SIMBlivA));
  lcd.write(byte(SIMBlivB));
  lcd.write(byte(SIMBlivC));
  lcd.write(byte(SIMBlivD));
  lcd.write(byte(SIMBlivE));
  lcd.write(byte(SIMBlivF));
  lcd.write(byte(SIMBsu));
  lcd.write(byte(SIMBgiu));
  lcd.write(byte(SIMBon));
  lcd.write(byte(SIMBoff));
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
    case MASTRdisplay:
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
