/*
###################################
#         CONNECTION              #
#     LCD       Arduino           #
#     D4          5               #
#     D5          4               #
#     D6          3               #
#     D7          2               #
#     E           11              #
#     RS          12              #
#     A           5V              #
#     K           GND             #
#     VDD         5V              #
#     VSS         GND             #
#     V0          6               #
#     RW          GND             #
###################################

*/

#include <LiquidCrystal.h>
int Contrast = 60;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

char DATA[50]= "";
void setup(){
  Serial.begin(9600);
  analogWrite(6, Contrast);
  lcd.begin(16, 2);
  lcdScanCard();
}

void loop(){
  byte n = Serial.available();
  if(n != 0){
    Serial.readBytesUntil('>',DATA,50);
    
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Welcome");
    
    delay(1500);
    lcdScanCard();
  }
  
}

void lcdScanCard(){
  lcd.setCursor(0, 0);
  lcd.print("Scan Card...");
}
