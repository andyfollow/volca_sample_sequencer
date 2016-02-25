#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

const byte LEVEL = 7;
const byte PAN = 10;
const byte SAMPLE_START_POINT = 40;
const byte SAMPLE_LENGTH = 41;
const byte HI_CUT = 42;
const byte SPEED = 43;
const byte PITCH_EG_INT = 44;
const byte PITCH_EG_ATTACK = 45;
const byte PITCH_EG_DECAY = 46;
const byte AMP_EG_ATTACK = 47;
const byte AMP_EG_DECAY = 48;

class MidiSender
{
        private:


        public:
                MidiSender();
                void timing_clock(); //
                void timing_start(); //
                void timing_stop(); //
                void note_on(int speed)
};

#endif // #ifndef rotary_encoder_h