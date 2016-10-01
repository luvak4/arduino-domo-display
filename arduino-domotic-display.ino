#include <VirtualWire.h>
#include <LiquidCrystal.h>
const int led_pin = 13;
const int receive_pin = 2;
// max lenght of my message
const int MSG_LEN = 7;
// display pins
const int RS = 9;
const int Enable = 10;
const int D4 = 5;
const int D5 = 6;
const int D6 = 7;
const int D7 = 8;
LiquidCrystal lcd(RS, Enable, D4, D5, D6, D7);

void setup() {
  // led
  pinMode(led_pin, OUTPUT);
  //radio rx
  vw_set_rx_pin(receive_pin);  
  vw_setup(2000);      
  vw_rx_start();
  // display 20x4
  lcd.begin(20, 4);
  ///////////12345678901234567890
  lcd.setCursor(0,0);
  lcd.print("Valk domotica 2016  ");
  Serial.begin(9600);
}

void loop(){    
    uint8_t buf[MSG_LEN]={0,0,0,0,0,0,0};
    uint8_t buflen = MSG_LEN;
    if (vw_get_message(buf, &buflen)){
      vw_rx_stop();
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
