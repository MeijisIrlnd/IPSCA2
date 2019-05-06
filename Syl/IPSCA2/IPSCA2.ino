#include <math.h>
#include <OSCMessage.h>
#include <SLIPEncodedSerial.h>

SLIPEncodedSerial SLIPSerial(Serial);


//===========================================================================================================================================
class ToggleButton
{
  public:
  ToggleButton::ToggleButton(int ip)
  {
    inputPin = ip;
    buttonReading = 0;
    previousState = 0;
  
  }
int getState()
  {
    buttonReading = digitalRead(inputPin);
    if(buttonReading != previousState)
    {
      if(previousState == 0)
      {
        previousState = buttonReading;
        if(count > 1)
        {
          count = 0;
        }
        ++count;
        return 1;
      }

      else
      {
        previousState = buttonReading;
        return 0;
      }
    }

    else
    {
      previousState = buttonReading;
      return false;
    }
  }

  int getCount()
  {
    return count;
  }
  
  int buttonReading;
  int previousState;
  private:

  int count;
  int inputPin;
};



class ParameterReader
{
  public:
  //Remember this button is a pointer, so access functions with "->"
  ToggleButton* inputState = new ToggleButton(8);
  ParameterReader::ParameterReader()
  {
    mNote = 110;
    prevHarmonicLevel = 0;
  }

  void txInputState()
  {
    OSCMessage ipMessage("/mode");
    ipMessage.add(inputState->getCount());
    SLIPSerial.beginPacket();
    ipMessage.send(SLIPSerial);
    SLIPSerial.endPacket();
  }
  void getNoteVals()
  {
    if(midiOne->getState() == 1)
    {
      mNote = 57;
      txFrequency(mNote);
    }

    else if(midiTwo->getState() == 1)
    {
      mNote = 61;
      txFrequency(mNote);
    }

    else if(midiThree->getState() == 1)
    {
      mNote = 62;
      txFrequency(mNote);
    }

    else if(midiFour->getState() == 1)
    {
      mNote = 64;
      txFrequency(mNote);
    }

    else
    {
      mNote = 0;
    }
    //txFrequency(mNote);
  }

  //Could template this?
  void txFrequency(int f)
  {
    OSCMessage freqMessage("/frequency");
    freqMessage.add(f);
    SLIPSerial.beginPacket();
    freqMessage.send(SLIPSerial);
    SLIPSerial.endPacket();
  }

  void getHarmonics()
  {
    for(int i = 0; i < 200; i++){
          harmonicLevel = analogRead(17);
          avgHarmonics += harmonicLevel;
    }

    avgHarmonics /= 200;

    if(int(avgHarmonics) != int(prevHarmonicLevel))
    {
    prevHarmonicLevel = avgHarmonics;
    avgHarmonics /= 1027;
    avgHarmonics = 1 - avgHarmonics;
    txHarmonics(avgHarmonics);

    delay(10);
    }
    prevHarmonicLevel = avgHarmonics;
   
  }

  void txHarmonics(float hVal)
  {
    OSCMessage hMessage("/harmonics");
    hMessage.add(hVal);
    SLIPSerial.beginPacket();
    hMessage.send(SLIPSerial);
    SLIPSerial.endPacket();
  }
  
  void setLEDKeyboard()
  {
    analogWrite(14, 1000);
    analogWrite(15, 0);
    analogWrite(16, 1000);
  }

  void setLEDGuitar()
  {
    analogWrite(14, 0);
    analogWrite(15, 1000);
    analogWrite(16, 1000);
  }
  private:
  ToggleButton* midiOne = new ToggleButton(12);
  ToggleButton* midiTwo = new ToggleButton(11);
  ToggleButton* midiThree = new ToggleButton(10);
  ToggleButton* midiFour = new ToggleButton(9);
  int mNote;
  int noteOn;

  float harmonicLevel;
  float avgHarmonics;
  float prevHarmonicLevel;
  
};

void setup() {
  SLIPSerial.begin(115200);
  pinMode(9, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);
  
  pinMode(13, OUTPUT);
  
  pinMode(8, INPUT_PULLUP);
  pinMode(14, OUTPUT);
  pinMode(15, OUTPUT);
  pinMode(16, OUTPUT);

  pinMode(17, INPUT);
}

//This is also a pointer, "->"
ParameterReader* paramReader = new ParameterReader();

void loop() 
{
  int st = paramReader->inputState->getState();
  if(paramReader->inputState->getCount() == 1)
  {
    paramReader->setLEDKeyboard();
    paramReader->txInputState();
    paramReader->getNoteVals();
    paramReader->getHarmonics();
  }

  else
  {
    paramReader->setLEDGuitar();
    paramReader->txInputState();
    //So also need to set up guitar parameters, maybe lets just start with the keyboard and implementing that into PureData.
  }


}
