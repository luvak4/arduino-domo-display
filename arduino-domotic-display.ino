#include <VirtualWire.h>   // load virtual-wire library
#include <LiquidCrystal.h> // load LCM library
const int led_pin = 13;    // led pin
const int receive_pin = 2; // radio rx pin
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

////////////////////////////////
// loop
////////////////////////////////
void loop(){    
  uint8_t buf[MSG_LEN]={0,0,0,0,0,0,0}; // empty buffer
  uint8_t buflen = MSG_LEN;             // lenght of buffer
  if (vw_get_message(buf, &buflen)){    // received a message?
    vw_rx_stop();                       // stop radio rx
    Serial.print(buf[0],HEX);
    Serial.print("-");
    Serial.print(buf[1],HEX);
    Serial.print("-");
    Serial.print(buf[2],HEX);
    Serial.print(buf[3],HEX);
    Serial.print(buf[4],HEX);
    Serial.print(buf[5],HEX);
    Serial.println(buf[6],HEX);
    //
    if (buf[0]==0xAD){
      // attuatore
      if (buf[1]==0x01){
	// rele ON
	lcd.setCursor(0,2);  
	lcd.print("                    "); 
	delay(200); 
	lcd.setCursor(0,2);
	///////////12345678901234567890
	lcd.print("ON                  ");	  
      }
      if (buf[1]==0x02){
	// rele OFF
	// rele ON
	lcd.setCursor(0,2);  
	lcd.print("                    "); 
	delay(200); 
	lcd.setCursor(0,2);  
	lcd.print("OFF                 ");	  	  
      }
      if (buf[1]==0x03){
	// valore analogico
	char temp[2];
	temp[0]=buf[2];
	temp[1]=buf[3];
	temp[2]=buf[4];
	//
	lcd.setCursor(0,1);
	lcd.print("                    "); 
	delay(200);
	lcd.setCursor(0,1);        
	lcd.print(atoi(temp));	  
      }
    }
    //
    vw_rx_start();
  }
}
