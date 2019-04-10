//Firstly, get a piezo hooked up, and use it as a trigger to toggle guitar signal on and off!

//Implement a button to turn a filter on and off, and add a pot to control said filter. This filter can affect both signals sure, fuck it like 

#include <stdlib.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <SLIPEncodedSerial.h>

SLIPEncodedSerial SLIPSerial(Serial);

class Button
{
  public:
  Button(int i)
  {
    ip = i;
    prevButtonVal = 1;
  }
  bool handleButton()
  {
    buttonVal = digitalRead(ip);
    if(buttonVal != prevButtonVal)
    {
      if(buttonVal == 0)
      {
      prevButtonVal = buttonVal; 
      return true;
      }
      else
      {
        prevButtonVal = buttonVal;
        return false;
      }
    }

    else
    {
      prevButtonVal = buttonVal;
      return false;
    }
  }

private:
  int ip;
  int prevButtonVal;
  int buttonVal;
  
};

class BassControl
{
    public:
        BassControl()
        {
            prevGtrState = false;
        }
//=======================================================================
    void gtrOnOff()
    {
      gtrState = gtrToggle->handleButton();
      if(gtrState == true)
      {
        if(prevGtrState = false)
        {
          gtx = 1;
        }
        else
        {
          gtx = 0;
        }
        prevGtrState = gtrState;
        txGtrState(gtx);
      }
    }
//=======================================================================

    void txGtrState(int gState)
    {
      OSCMessage gStateMsg("/GTR");
      gStateMsg.add(gState);
      SLIPSerial.beginPacket();
      gStateMsg.send(SLIPSerial);
      SLIPSerial.endPacket();
    }
//========================================================================

    private:
       Button* gtrToggle = new Button(12);
       bool gtrState;
       bool prevGtrState;

       int gtx = 1;
};

void setup()
{
  SLIPSerial.begin(115200);
pinMode(12, INPUT_PULLUP);
pinMode(13, OUTPUT);
}

BassControl controller;
void loop()
{
    controller.gtrOnOff();
}
