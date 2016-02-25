#include "rotary_encoder.h"

RotaryEncoder::RotaryEncoder(byte pin1, byte pin2) : encoderPin1(pin1), encoderPin2(pin2), lastEncoded(0), encoderValue(0), lastencoderValue(0), lastMSB(0), lastLSB(0)
{   
    // pin1 2,pin2 3
    pinMode(encoderPin1, INPUT);
    pinMode(encoderPin2, INPUT);
    digitalWrite(encoderPin1, HIGH);
    digitalWrite(encoderPin2, HIGH);
    attachInterrupt(0, updateEncoder, CHANGE); 
    attachInterrupt(1, updateEncoder, CHANGE);
};

void RotaryEncoder::updateEncoder(){
  int MSB = digitalRead(encoderPin1); //MSB = most significant bit
  int LSB = digitalRead(encoderPin2); //LSB = least significant bit

  int encoded = (MSB << 1) |LSB; //converting the 2 pin value to single number
  int sum  = (lastEncoded << 2) | encoded; //adding it to the previous encoded value

  if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderValue ++;
  if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderValue --;

  lastEncoded = encoded; //store this value for next time
};

long RotaryEncoder::value(){
    return encoderValue
}