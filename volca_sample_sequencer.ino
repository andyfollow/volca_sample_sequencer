/* YourDuino.com Example Software Sketch
 16 character 2 line I2C Display
 Backpack Interface labelled "A0 A1 A2" at lower right.
 ..and
 Backpack Interface labelled "YwRobot Arduino LCM1602 IIC V1"
 MOST use address 0x27, a FEW use 0x3F
 terry@yourduino.com */

/*-----( Import needed libraries )-----*/
#include <Wire.h>  // Comes with Arduino IDE
// Get the LCD I2C Library here: 
// https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads
// Move any other LCD libraries to another folder or delete them
// See Library "Docs" folder for possible commands etc.
#include <LiquidCrystal_I2C.h>

/*-----( Declare Constants )-----*/
/*-----( Declare objects )-----*/
// set the LCD address to 0x27 for a 16 chars 2 line display
// A FEW use address 0x3F
// Set the pins on the I2C chip used for LCD connections:
//                    addr, en,rw,rs,d4,d5,d6,d7,bl,blpol
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

/*-----( Declare Variables )-----*/
//NONE

//these pins can not be changed 2/3 are special pins
int encoderPin1 = 2;
int encoderPin2 = 3;

volatile int lastEncoded = 0;
volatile long encoderValue = 0;

long lastencoderValue = 0;

int lastMSB = 0;
int lastLSB = 0;

void setup()   /*----( SETUP: RUNS ONCE )----*/
{
  //  Set MIDI baud rate:
  Serial.begin(31250);
  
  lcd.begin(20,4);   // initialize the lcd for 16 chars 2 lines, turn on backlight

  pinMode(encoderPin1, INPUT); 
  pinMode(encoderPin2, INPUT);

  digitalWrite(encoderPin1, HIGH); //turn pullup resistor on
  digitalWrite(encoderPin2, HIGH); //turn pullup resistor on
  //call updateEncoder() when any high/low changed seen
  //on interrupt 0 (pin 2), or interrupt 1 (pin 3) 
  attachInterrupt(0, updateEncoder, CHANGE); 
  attachInterrupt(1, updateEncoder, CHANGE);
// ------- Quick 3 blinks of backlight  -------------

  lcd.backlight(); // finish with backlight on  
}

int delay_value = 120;

byte PATTERN_LENGTH = 16;
byte CURRENT_POSITION = 0;
byte speed_pattern[] = {64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64};
boolean enable_pattern[] = {false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false};
char lcd_pattern[] = {'|','_','_','_'};
byte encoderMode = 0; // 0:note_on_off, 1:speed change, 2:tempo mode

void loop()   /*----( LOOP: RUNS CONSTANTLY )----*/
{ Serial.write(0xFA); // PLAY 
  for (byte play_position=0;play_position < PATTERN_LENGTH;play_position++){
      lcd.setCursor(play_position/16,play_position%16);
      lcd.print(">");
      
      if (enable_pattern[play_position] == true){
          noteOn(0xB0, 0x2B, speed_pattern[play_position]);
          noteOn(0x91, 0x3A, 0x45);
          
      };
      for (byte c=0;c<6;c++){
          Serial.write(0xF8); //sync
          delay(delay_value/6);
      };
      if (enable_pattern[play_position] == true){lcd.print("P");}else{lcd.print(lcd_pattern[play_position%4]);};
      
  };

}/* --(end main loop )-- */

void updateEncoder(){
  int MSB = digitalRead(encoderPin1); //MSB = most significant bit
  int LSB = digitalRead(encoderPin2); //LSB = least significant bit

  int encoded = (MSB << 1) |LSB; //converting the 2 pin value to single number
  int sum  = (lastEncoded << 2) | encoded; //adding it to the previous encoded value

  if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderValue ++;
  if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderValue --;
  
  if (abs(encoderValue) == 15){
    whenEncoder(encoderValue/15);
    encoderValue = 0;
  }
  
  lastEncoded = encoded; //store this value for next time
};

void whenEncoder(int value ){
    static byte prevPosition = 0;
    CURRENT_POSITION = CURRENT_POSITION + value;
    lcd.setCursor(CURRENT_POSITION/16,CURRENT_POSITION%16);
    lcd.print('E');
    lcd.setCursor(prevPosition/16,prevPosition%16);
    lcd.print(lcd_pattern[prevPosition%4]);
    prevPosition = CURRENT_POSITION;
}



void noteOn(int cmd, int pitch, int velocity) {
  Serial.write(cmd);
  Serial.write(pitch);
  Serial.write(velocity);
}

/* ( THE END ) */

