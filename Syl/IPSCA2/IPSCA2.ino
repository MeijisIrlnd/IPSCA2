#include <math.h>
#include <OSCMessage.h>
#include <SLIPEncodedSerial.h>

SLIPEncodedSerial SLIPSerial(Serial);


//===========================================================================================================================================
class ToggleButton
{
  public:
  ToggleButton::ToggleButton(int ip, int countLimit)
  {
    inputPin = ip;
    buttonReading = 0;
    previousState = 0;
    cLim = countLimit;
  
  }
int getState()
  {
    buttonReading = digitalRead(inputPin);
    if(buttonReading != previousState)
    {
      if(previousState == 0)
      {
        previousState = buttonReading;
        if(count > cLim)
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
  int cLim;
};

//=======================================================================================================================================

class ParameterReader
{
  public:
  //Remember this button is a pointer, so access functions with "->"
  ToggleButton* inputState = new ToggleButton(8, 1);
  ParameterReader::ParameterReader()
  {
    mNote = 110;
    prevHarmonicLevel = 0;
    prevReleaseLevel = 0;
  }
  
  //Common Functions
  //============================================================================================================================
  void txInputState()
  {
    OSCMessage ipMessage("/mode");
    ipMessage.add(inputState->getCount());
    SLIPSerial.beginPacket();
    ipMessage.send(SLIPSerial);
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

  //Midi Control Functions
  //====================================================================================================================
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

  
  void getRelease()
  {
    for(int i = 0; i < 200; i++)
    {
      rel = analogRead(18);
      avgRelease += rel;
    }
    avgRelease /= 200;

    if(int(avgRelease) != int(prevReleaseLevel))
    {
      prevReleaseLevel = avgRelease;
      avgRelease /= 1027;
      avgRelease = 1 - avgRelease;
      
      txRelease(avgRelease);
      delay(10);
    }
    prevReleaseLevel = avgRelease;
  }

  void txRelease(float aR)
  {
    OSCMessage relMessage("/release");
    relMessage.add(aR);
    SLIPSerial.beginPacket();
    relMessage.send(SLIPSerial);
    SLIPSerial.endPacket();
  }

  void setLEDKeyboard()
  {
    analogWrite(14, 1000);
    analogWrite(15, 0);
    analogWrite(16, 1000);
  }
//==========================================================================================================================================
//Guitar control functions
  void getOctave()
  {
    if(octaveSwitch->getState() == 1)
    {
      switch(octaveSwitch->getCount())
      {
        case 1:
        {
          txOctave(1);
          break;
        }

        case 2: 
        {
          txOctave(2);
          break;
        }

        case 3:
        {
          txOctave(3);
          break;
        }

        default:
        {
          txOctave(1);
          break;
        }
      }
    }
  }

  void txOctave(int oct)
  {
    OSCMessage oMessage("/octave");
    oMessage.add(oct);
    SLIPSerial.beginPacket();
    oMessage.send(SLIPSerial);
    SLIPSerial.endPacket();
  }

  void getGtrMode()
  {
    if(gtrMode->getState() == 1)
    {
      switch(gtrMode->getCount())
      {
        case 1:
        {
          txGtrMode(1);
          break;
        }

        case 2:
        {
          txGtrMode(2);
          break;
        }

        case 3:
        {
          txGtrMode(3);
        }

        default:
        {
          txGtrMode(1);
        }
      }
    }
  }

  void txGtrMode(int gM)
  {
    OSCMessage gMessage("/gtrMode");
    gMessage.add(gM);
    SLIPSerial.beginPacket();
    gMessage.send(SLIPSerial);
    SLIPSerial.endPacket();
  }

  void getSampleTrigger()
  {
    if(sampleTrigger->getState() == 1)
    {
      txSampleTrigger(1);
    }
  }

  void txSampleTrigger(int t)
  {
    OSCMessage trigMessage("/trigger");
    trigMessage.add(t);
    SLIPSerial.beginPacket();
    trigMessage.send(SLIPSerial);
    SLIPSerial.endPacket();
  }

  void getSampleSelect()
  {
    if(sampleSelect->getState() == 1)
    {
      switch(sampleSelect->getCount())
      {
        case 1:
        {
          txSampleSelect(1);
          break;
        }

        case 2: 
        {
          txSampleSelect(2);
          break;
        }

        case 3:
        {
          txSampleSelect(3);
          break;
        }

        case 4:
        {
          txSampleSelect(4);
          break;
        }

        default:
        {
          txSampleSelect(1);
        }
      }
    }
  }

  void txSampleSelect(int sS)
  {
    OSCMessage sampMessage("/sample");
    sampMessage.add(sS);
    SLIPSerial.beginPacket();
    sampMessage.send(SLIPSerial);
    SLIPSerial.endPacket();
  }
  void setLEDGuitar()
  {
    analogWrite(14, 0);
    analogWrite(15, 1000);
    analogWrite(16, 1000);
  }
  private:
  //Midi Control
  ToggleButton* midiOne = new ToggleButton(12, 1);
  ToggleButton* midiTwo = new ToggleButton(11, 1);
  ToggleButton* midiThree = new ToggleButton(10, 1);
  ToggleButton* midiFour = new ToggleButton(9, 1);
  int mNote;
  int noteOn;

  float harmonicLevel;
  float avgHarmonics;
  float prevHarmonicLevel;

  float rel;
  float avgRelease;
  float prevReleaseLevel;

  //Guitar Control
  ToggleButton* octaveSwitch = new ToggleButton(12, 2);
  ToggleButton* gtrMode = new ToggleButton(11, 2);
  ToggleButton* sampleTrigger = new ToggleButton(10, 1);
  ToggleButton* sampleSelect = new ToggleButton(9, 4);
  
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

  pinMode(18, INPUT);
}

//This is also a pointer, "->"
ParameterReader* paramReader = new ParameterReader();

void loop() 
{
  //txInputState() should only be called when the value has been changed.
  int st = paramReader->inputState->getState();
  if(paramReader->inputState->getCount() == 1)
  {
    paramReader->setLEDKeyboard();
    paramReader->txInputState();
    paramReader->getNoteVals();
    paramReader->getHarmonics();
    paramReader->getRelease();
  }

  else
  {
    paramReader->setLEDGuitar();
    paramReader->txInputState();
    paramReader->getOctave();
    paramReader->getGtrMode();
    paramReader->getSampleTrigger();
    paramReader->getSampleSelect();
    paramReader->getHarmonics();
    //So also need to set up guitar parameters, maybe lets just start with the keyboard and implementing that into PureData.
    //What are the buttons going to do for the guitar?
    //one can be an octave switcher 1)
    //one can be combo, so gtr, gtr + sub, sub 2)
    //Filter on off? idk lmao, because this would be handy to implement for both gtr and midi inputs. think guitar specific. 
    //Sample trigger 3)
    //sample selector 4)
    //What is the release potentiometer going to do for the guitar modifier? 
  }


}
