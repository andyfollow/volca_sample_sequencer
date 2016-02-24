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

//-------- Write characters on the display ------------------
// NOTE: Cursor Position: (CHAR, LINE) start at 0  
  //lcd.setCursor(0,0); //Start at character 4 on line 0
  //lcd.print("Hello, world!");
  //delay(1000);
  //lcd.setCursor(0,1);
  //lcd.print("HI!YourDuino.com");
  //delay(8000);  

// Wait and then tell user they can start the Serial Monitor and type in characters to
// Display. (Set Serial Monitor option to "No Line Ending")
  //lcd.clear();
  //lcd.setCursor(0,0); //Start at character 0 on line 0
  //lcd.print("Use Serial Mon");
  //lcd.setCursor(0,1);
  //lcd.print("Type to display");  
  

}/*--(end setup )---*/

int counter1 = 0;
int counter2 = 0;
int delay_value = 100;

void loop()   /*----( LOOP: RUNS CONSTANTLY )----*/
{  lcd.setCursor(counter1,counter2);
  lcd.print("#");
  
  for (int c=0;c<6;c++){
      Serial.write(0xF8);
      delay((delay_value+encoderValue)/6);
  }
  //delay(delay_value+encoderValue);
  delay_value = delay_value + encoderValue;
  encoderValue = 0;
  lcd.setCursor(counter1,counter2);
  if (counter1 ==0 && counter2==0){Serial.write(0xFA);}
  if ((counter1-4)%8==0){
     noteOn(0x91, 0x3A, 0x45);  
  }
  if ((16*counter2 + counter1)%3==0){
     noteOn(0xB0, 0x2B, 0x20+counter1-counter2);
     noteOn(0x90, 0x3A, 0x45);  
  }
  if (counter1%4==0){
     lcd.print("|");
  }else{
  lcd.print("_");
  };
  
  counter1 = counter1 + 1;
  lcd.setCursor(0,2);
  lcd.print(60000/delay_value/4);
  if (counter1 >15){
   counter1 = 0; 
   counter2 = counter2 + 1;
   if (counter2 == 2){
       counter2 = 0;
     };
  }
}/* --(end main loop )-- */

void updateEncoder(){
  int MSB = digitalRead(encoderPin1); //MSB = most significant bit
  int LSB = digitalRead(encoderPin2); //LSB = least significant bit

  int encoded = (MSB << 1) |LSB; //converting the 2 pin value to single number
  int sum  = (lastEncoded << 2) | encoded; //adding it to the previous encoded value

  if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderValue ++;
  if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderValue --;

  lastEncoded = encoded; //store this value for next time
};
//  plays a MIDI note.  Doesn't check to see that
//  cmd is greater than 127, or that data values are  less than 127:
void noteOn(int cmd, int pitch, int velocity) {
  Serial.write(cmd);
  Serial.write(pitch);
  Serial.write(velocity);
}

/* ( THE END ) */

