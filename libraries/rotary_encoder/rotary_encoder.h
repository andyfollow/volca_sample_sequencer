#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

class RotaryEncoder
{
        private:
                  byte encoderPin1;
                  byte encoderPin2;
                  volatile int lastEncoded;
                  volatile long encoderValue;
                  long lastencoderValue;

                  int lastMSB;
                  int lastLSB;
        public:
                RotaryEncoder(byte pin1, byte pin2);
                long value();
};

#endif // #ifndef rotary_encoder_h