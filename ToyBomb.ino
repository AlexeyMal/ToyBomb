#include <MultiFuncShield.h>

/*

  For more information and help, please visit https://www.cohesivecomputing.co.uk/hackatronics/arduino-multi-function-shield/part-3/

  All our hackatronics projects are free for personal use, and there are many more
  in the pipeline. If you find our projects helpful or useful, please consider making
  a small donation to our hackatronics fund using the donate buttons on our web pages.
  Thank you.

*/

enum CountDownModeValues
{
  COUNTING_STOPPED,
  COUNTING
};

byte countDownMode = COUNTING_STOPPED;

byte tenths = 0;
char seconds = 0;
char minutes = 0;

void setup() {
  // put your setup code here, to run once:
  MFS.initialize();    // initialize multifunction shield library
  MFS.write(0);

  pinMode(5 , INPUT_PULLUP);
  pinMode(6 , INPUT_PULLUP);
  pinMode(9 , INPUT_PULLUP);
  pinMode(A5, INPUT_PULLUP);

  //Serial.begin(9600);
}


void loop() {
  // put your main code here, to run repeatedly:

  byte btn = MFS.getButton();

  switch (countDownMode)
  {
    case COUNTING_STOPPED:
      //MFS.blinkLeds(LED_ALL, OFF);
      //MFS.writeLeds(LED_ALL, ON);
      if (btn == BUTTON_1_SHORT_RELEASE && (minutes + seconds) > 0)
      {
        // start the timer
        countDownMode = COUNTING;
        MFS.writeLeds(LED_1, ON);
        MFS.blinkLeds(LED_1, ON);
      }
      else if (btn == BUTTON_1_LONG_PRESSED)
      {
        // reset the timer
        tenths = 0;
        seconds = 0;
        minutes = 0;
        MFS.write(minutes * 100 + seconds);
        MFS.blinkLeds(LED_ALL, OFF);
        MFS.writeLeds(LED_ALL, OFF);
      }
      else if (btn == BUTTON_2_PRESSED || btn == BUTTON_2_LONG_PRESSED)
      {
        minutes++;
        if (minutes > 99) //60)
        {
          minutes = 0;
        }
        MFS.write(minutes * 100 + seconds);
        MFS.writeLeds(LED_2, ON);
        MFS.blinkLeds(LED_2, ON);
      }
      else if (btn == BUTTON_3_PRESSED || btn == BUTTON_3_LONG_PRESSED)
      {
        seconds += 1; //10;
        if (seconds >= 60)
        {
          seconds = 0;
        }
        MFS.write(minutes * 100 + seconds);
        MFS.writeLeds(LED_3, ON);
        MFS.blinkLeds(LED_3, ON);
      }
      break;

    case COUNTING:
      if (btn == BUTTON_1_SHORT_RELEASE || btn == BUTTON_1_LONG_RELEASE)
      {
        MFS.blinkLeds(LED_4, OFF);
        MFS.writeLeds(LED_4, OFF);
        // stop the timer
        countDownMode = COUNTING_STOPPED;
      }
      //else
      else if (!digitalRead(A5))
      {
        MFS.writeLeds(LED_4, ON);
        MFS.blinkLeds(LED_4, ON);
        // continue counting down
        tenths++;
        if (digitalRead(9))
        { //count faster
          tenths += 4;
        }

        if (tenths >= 10)
        {
          tenths = 0;
          seconds--;
          //MFS.beep(5);  // beep once, 50 milliseconds
          if (!digitalRead(6)) 
            MFS.beep(2);

          if (seconds < 0 && minutes > 0)
          {
            seconds = 59;
            minutes--;
          }

          //MFS.writeLeds(LED_ALL, ON);
          //MFS.blinkLeds(LED_ALL, ON);

          if (digitalRead(5))
          { //finish immediately
            minutes = 0;
            seconds = 0;
          }

          if (minutes == 0 && seconds == 0)
          {
            // timer has reached 0, so sound the alarm
            MFS.blinkLeds(LED_ALL, OFF);
            MFS.writeLeds(LED_ALL, ON);
            MFS.beep(50, 50, 4);  // beep 4 times, 500 milliseconds on / 500 off
            countDownMode = COUNTING_STOPPED;
            //MFS.write("0000"); //AMA update display prior to delay
            //MFS.write("HAHA"); 
            MFS.write(0);
            delay(4000); //AMA
            MFS.beep(300);
            MFS.write("----");
            MFS.blinkDisplay(DIGIT_ALL, ON);
            delay(3000);
            MFS.writeLeds(LED_ALL, OFF);
            MFS.blinkDisplay(DIGIT_ALL, OFF);
          }

          MFS.write(minutes * 100 + seconds);
        }
        delay(100);
      }
      break;
  }
}
