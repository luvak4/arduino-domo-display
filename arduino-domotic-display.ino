#include <VirtualWire.h>   // load virtual-wire library
#include <LiquidCrystal.h> // load LCM library
#define DISPLAYindirizzoLSB 0
#define DISPLAYindirizzoMSB   1
#define DISPLAYcolonnaMSB   2
#define DISPLAYriga      3
#define DISPLAYnCaratteri 4
#define DISPLAYinizioTesto 5
String caratteri;

byte BYTEradioDisplay[VW_MAX_MESSAGE_LEN];
#define PONTEsuGIU 1234
#define CIRC_CANTINA 1235
#define DISPLAY 1236
byte CIFR[]={223,205,228,240,43,146,241,87,213,48,235,131,6,81,26,70,34,74,224,27,111,150,22,138,239,200,179,222,231,212};
//
const unsigned long mask=0x0000FFFF;
//




const int led_pin = 13;    // led pin
const int receive_pin = 11; // radio rx pin
const int MSG_LEN = 7;     // raduio max message lenght
const int RS = 9;          // LCM pin RS
const int Enable = 10;     // LCM pin enable
const int D4 = 5;          // LCM pin D4
const int D5 = 6;          // LCM pin D5
const int D6 = 7;          // LCM pin D6
const int D7 = 8;          // LCM pin D7
LiquidCrystal lcd(RS, Enable, D4, D5, D6, D7); // LCM initialize lib
////////////////////////////////
// setup
////////////////////////////////
void setup() {
  pinMode(led_pin, OUTPUT);   // led set pin
  vw_set_rx_pin(receive_pin); // radio set rx pin
  vw_setup(2000);             // radio set speed
  vw_rx_start();              // radio rx ON
  lcd.begin(20, 4);           // LCM initialize 20 cols 4 rows
  lcd.setCursor(0,0);         // LCM first row, first col
  lcd.print("Valk domotica 2016  "); // LCM message
  Serial.begin(9600);         // serial begin and speed
}







void loop() {
  // put your main code here, to run repeatedly:
uint8_t buf[VW_MAX_MESSAGE_LEN];
    uint8_t buflen = VW_MAX_MESSAGE_LEN;

    if (vw_get_message(BYTEradioDisplay, &buflen)) // Non-blocking
    {
        // decifratura
  for (byte n=0; n<27;n++){
    BYTEradioDisplay[n]=BYTEradioDisplay[n] ^ CIFR[n];
  }
  byte col=BYTEradioDisplay[2];
  byte rig=BYTEradioDisplay[3];
  Serial.println(col);
  Serial.println(rig);
  Serial.println();
  String temp="";
  //String zero="";
  
  for (int n=0; n<BYTEradioDisplay[4];n++)
  {
    char t=BYTEradioDisplay[n+5];
    temp=temp+t;
    //zero=zero+" ";
  }
  //lcd.setCursor(col,rig);
  //lcd.print(zero);
  //delay(100);
  lcd.setCursor(col,rig);
  lcd.print(temp);
  
    }
}
