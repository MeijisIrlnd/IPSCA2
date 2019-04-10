//Firstly, get a piezo hooked up, and use it as a trigger to toggle guitar signal on and off!

//Implement a button to turn a filter on and off, and add a pot to control said filter. This filter can affect both signals sure, fuck it like 


class BassControl
{
    public:
        BassControl::BassControl()
        {
            
        }

        void handlePiezo()
        {
            piezoVal = analogRead(14);
        }



    private:
        float piezoVal;
};

void setup()
{
pinMode(14, INPUT);
}

BassControl controller;
void loop()
{
    controller.handlePiezo();
}
