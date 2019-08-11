/*
  CBM/Atari Paddle to volume control converter

  For Leonardo, Pro Micro and Due boards only.

  Convert the classic rotary dial phone to an webbrowser controller
  Allowing the user to dial an IP address

  This project does require some small tricks to be pulled mostly based on keyboard shortcuts available in the browser and Windows OS
  for more info about the keyboard key names: https://www.arduino.cc/en/Reference/KeyboardModifiers
  for more info about keyboard shortcuts in webbrowsers: https://www.howtogeek.com/114518/47-keyboard-shortcuts-that-work-in-all-web-browsers/

*/

#include "Keyboard.h"
#include "phone_enums.h"

/*IO-defintions*/
#define mode_select   15 /*switch used to select the mode (allow easy disabling of sometimes anoying mouse, usefull for debugging*/
#define ph_button     8  /*the white button on the front of the phone*/
#define ph_hookpulse  9  /*the hook of the rotary dial phone*/


/*defines*/
#define DELAY_LONG     250   /*response delay, in ms*/
#define PULSE_TIMEOUTVAL 25 /*.. times 10msec*/

/*-----------------------------------------------------------------------*/

/*routines*/
unsigned char PollPhone(void);
unsigned char CountDialPulses(void);
void OpenWebbrowser(void);
void CloseWebbrowser(void);
void OpenCalculator(void);
void CloseCalculator(void);
void SpeedDial(unsigned char mem);
void Sound_DialUpModem(void);
void Sound_Tone(unsigned int period, unsigned int duration);
void EasterEgg(String str);
void Backspace(unsigned char lp);

/*globals*/
//unsigned char lp = 0;
boolean WebbrowserOpen = false;     /*this flag is used to keep track of the status of the webbrowser, if it isn't open, it should not be closed (otherwise we might be closing the wrong app*/
boolean CalculatorOpen = false;     /*this flag is used to keep track of the status of the calculator, if it isn't open, it should not be closed (otherwise we might be closing the wrong app*/

/*-----------------------------------------------------------------------*/

void setup()
{
  pinMode(mode_select, INPUT_PULLUP);   /*initialize IO*/
  pinMode(ph_button, INPUT_PULLUP);
  pinMode(ph_hookpulse, INPUT_PULLUP);

  Serial.begin(115200);     /*open a serial connection, for debugging purposes only*/
  
  //while (!Serial) ;         /*to make sure we miss nothing printed to the virtual COM-port, keep looping until the serial stream is opened*/
  /*But beware, this means the application WILL stay here until a COM is opened, meaning that an application (arduino IDE or terminal) must be running!!!*/
  
  Serial.println("Hello this is debugging information");
  Serial.println("Rotary dial IP dialing");

  Keyboard.begin();
}

/*-----------------------------------------------------------------------*/

void loop()
{
  unsigned char value;
  static String dial_string = "";
  while (digitalRead(mode_select) == 0) /*for debugging purposes, it is very usefull to disable the projects functionaly... considering it is always connected and may not always behave as expected/desired during development*/
  {
    delay(1000); /*do nothing*/
    Serial.println("Project disabled...");
  }

  value = PollPhone();
  switch(value)
  {
    case PHONE_OFF_HOOK:
    {
      dial_string = ""; /*reset the dial string*/
      break;
    }

    case PHONE_SPEEDDIAL_HANG_UP:
    case PHONE_URL_HANG_UP:    
    {
      CloseWebbrowser();
      break;
    }    

    case PHONE_CALCULATOR_HANG_UP:
    {
      CloseCalculator();
      break;
    }    

    case PHONE_SPEEDDIAL_1:
    case PHONE_SPEEDDIAL_2:
    case PHONE_SPEEDDIAL_3:
    case PHONE_SPEEDDIAL_4:
    case PHONE_SPEEDDIAL_5:
    case PHONE_SPEEDDIAL_6:
    case PHONE_SPEEDDIAL_7:
    case PHONE_SPEEDDIAL_8:    
    {
      OpenWebbrowser();            
      value = value - PHONE_SPEEDDIAL_1 + 1; /*remove the offset from the enum to get a value from 1-8, that will be parsed to the SpeedDial routine that opens the ..th bookmark fromthe bookmark list*/
      SpeedDial(value); 
      Sound_DialUpModem();              
      break;                  
    }

    case PHONE_URL:
    {
      OpenWebbrowser();      
      break;
    }

    case PHONE_URL_0:
    case PHONE_URL_1:
    case PHONE_URL_2:
    case PHONE_URL_3:
    case PHONE_URL_4:
    case PHONE_URL_5:
    case PHONE_URL_6:
    case PHONE_URL_7:
    case PHONE_URL_8:
    case PHONE_URL_9:
    {
      value = value - PHONE_URL_0;        /*remove the offset from the enum to get the value from 0-9*/
      Keyboard.print(value);              /*the print function converts the value (of the conveniently chosen enum) to the ASCII character(s) representing of that value*/
      dial_string = dial_string + value;
      EasterEgg(dial_string);             /*check if the currently dialed number equals an easter egg and handle accordingly*/
      break;                  
    }

    case PHONE_CALCULATOR:
    {
      OpenCalculator();       
      break;
    }
       
    case PHONE_CALC_0:
    case PHONE_CALC_1:
    case PHONE_CALC_2:
    case PHONE_CALC_3:
    case PHONE_CALC_4:    
    case PHONE_CALC_5:
    case PHONE_CALC_6:
    case PHONE_CALC_7:
    case PHONE_CALC_8:
    case PHONE_CALC_9:
    {
      value = value - PHONE_CALC_0;       /*remove the offset from the enum to get the value from 0-9*/
      Keyboard.print(value);              /*the print function converts the value (of the conveniently chosen enum) to the ASCII character(s) representing of that value*/
      dial_string = dial_string + value;
      break;                  
    }

    case PHONE_CALC_DELETE:
    case PHONE_URL_DELETE:
    {
      Backspace(1);                                 /*do a single backspace*/
      dial_string.remove(dial_string.length()-1);   // Remove last char
      break;                  
    }

    case PHONE_CALC_DOT:
    case PHONE_URL_DOT:
    {
      Keyboard.press('.');
      Keyboard.releaseAll();
      dial_string = dial_string + ".";
      break;
    }

    case PHONE_CALC_DOTCOM:
    {
      break;    
    }
    
    case PHONE_URL_DOTCOM:
    {
      Keyboard.print(".com");
      Keyboard.press(KEY_RETURN);
      Keyboard.releaseAll();
      dial_string = dial_string + ".com";
      Sound_DialUpModem();                    
      break;
    }

    case PHONE_CALC_DOTCN:
    {
      break;    
    }
    
    case PHONE_URL_DOTCN:
     {
      Keyboard.print(".cn");
      Keyboard.press(KEY_RETURN);
      Keyboard.releaseAll();
      dial_string = dial_string + ".cn";
      Sound_DialUpModem();                    
      break;
    }

    case PHONE_CALC_MINUS:
    case PHONE_URL_MINUS:
    {
      Keyboard.press('-');
      Keyboard.releaseAll();
      dial_string = dial_string + "-";
      break;
    }

    case PHONE_CALC_PLUS:
    case PHONE_URL_PLUS:
    {
      Keyboard.press('+');
      Keyboard.releaseAll();
      dial_string = dial_string + "+";
      break;
    }

    case PHONE_CALC_STAR:    
    case PHONE_URL_STAR:
    {
      Keyboard.press('*');
      Keyboard.releaseAll();
      dial_string = dial_string + "*";
      break;
    }

    case PHONE_CALC_SLASH:    
    case PHONE_URL_SLASH:
    {
      Keyboard.press('/');
      Keyboard.releaseAll();
      dial_string = dial_string + "/";
      break;
    }

    case PHONE_CALC_EQUAL:    
    case PHONE_URL_EQUAL:
    {
      Keyboard.press('=');
      Keyboard.releaseAll();
      dial_string = dial_string + "=";
      break;
    }

    case PHONE_CALC_RETURN:
    {
      Keyboard.press(KEY_RETURN);
      Keyboard.releaseAll();
      break;
    }    
    
    case PHONE_URL_RETURN:
    {
      Keyboard.press(KEY_RETURN);
      Keyboard.releaseAll();
      Sound_DialUpModem();        
      break;
    }    
   
    case PHONE_IDLE:  /*when the phone is on the hook*/
    default:
    {
      break;
    }
  }

}


/*-----------------------------------------------------------------*/


/*this routine will poll the phone for user input*/
/*it responds with an enumerated event value*/
unsigned char PollPhone(void)
{   
  static unsigned char phone_state = STATE_HOOK_DOWN;   /*the statemachine of the phones behaviour*/
  static unsigned char phone_mode = MODE_SPEEDDIAL;     /*the mode in which we are using the phone*/
  static unsigned char shift = false;                   /*the shift function allows double functions on the dial*/
  unsigned char dial_val = 255;
  unsigned int tone_delay = 0;
  unsigned char ret_val = PHONE_IDLE; 

  switch(phone_state)
  {   
    case STATE_HOOK_PICK_UP:
    { /*check for activity, hook or pulse, sound a dial tone to indicate the system is ready for input*/      
      pinMode(ph_hookpulse, INPUT_PULLUP);    /*set IO-pin to input (if not already) pickup is easily detected by watching if the pin goes low*/      
      while(digitalRead(ph_hookpulse) == 0)   /*a while loop is the only way to get a glitchfree dial-tone, PWM doesn't sound good enough as the input pin testing results in unwanted noise*/
      {                                       /*though the blocking character of the while loop is not a big issue during this state, because now we have full control over the produced wave and can do IO-readings when the pin is at a known state*/
        Sound_Tone(2500, 1);                  /*generate a single cyle of the desired tone*/
      }     
      phone_state = STATE_DIAL;
      ret_val = PHONE_IDLE;        
      break;
    }     

    case STATE_DIAL:
    {
      dial_val = CountDialPulses();
      if(dial_val == 255)
      {
        phone_state = STATE_HOOK_DOWN;    /*oops, the user aborted by hanging the handpiece up on the hook*/
        switch(phone_mode)
        {
          case MODE_SPEEDDIAL:  {ret_val = PHONE_SPEEDDIAL_HANG_UP; break;}
          case MODE_CALCULATOR: {ret_val = PHONE_CALCULATOR_HANG_UP; break;}
          case MODE_URL:        {ret_val = PHONE_URL_HANG_UP; break;}
          default:              {ret_val = PHONE_IDLE; break;}
        }
      }
      else
      {
        phone_state = STATE_WAIT_FOR_INPUT;
        switch(phone_mode)
        {
          case MODE_SPEEDDIAL:
          {            
            switch(dial_val)
            {
              case 9:   {ret_val = PHONE_CALCULATOR; phone_mode = MODE_CALCULATOR; break;}
              case 0:   {ret_val = PHONE_URL; phone_mode = MODE_URL; break;}              
              default:  {ret_val = PHONE_SPEEDDIAL_1 + (dial_val-1); break;}            
            }
            break;
          }

          case MODE_CALCULATOR:
          {
            if(shift == true) {ret_val = PHONE_CALC_RETURN + dial_val;} /*convert the value of pulse_count to a value represented by the enum definition*/                                            
            else              {ret_val = PHONE_CALC_0 + dial_val;}      /*convert the value of pulse_count to a value represented by the enum definition*/                              
            shift = false;  /*reset shift*/
            break;
          }

          case MODE_URL:
          {
            if(shift == true) {ret_val = PHONE_URL_RETURN + dial_val;}  /*convert the value of pulse_count to a value represented by the enum definition*/                                            
            else              {ret_val = PHONE_URL_0 + dial_val;}       /*convert the value of pulse_count to a value represented by the enum definition*/                              
            shift = false;  /*reset shift*/
            break;
          }

          default:
          {
            break;
          }
        }
      }      
      break;
    }

    case STATE_WAIT_FOR_INPUT:
    {
      if(digitalRead(ph_button) == 0)       /*check for button activity*/
      {
        while(digitalRead(ph_button) == 0)  /*keep looping until released, produce a tone while pressing the button*/
        {
            Sound_Tone(8000, 1);      /*confirm button press with audible tone*/
            shift = true;             /*the button was pressed, this means that shift is active*/
        }
        pinMode(ph_hookpulse, INPUT_PULLUP);    /*set IO-pin to input, this way the phone makes no sound and draws no unwanted DC-current*/      
        Serial.println("Button press detected");  
        phone_state = STATE_WAIT_FOR_INPUT;
        break;      
      }
      
      if(digitalRead(ph_hookpulse) == 1)    /*check for pulse activity*/
      {
        phone_state = STATE_DIAL;            /*go to the dial (pulse decoding) state*/
      }      
      ret_val = PHONE_IDLE;              
      break;
    }

    case STATE_HOOK_DOWN:
    default:
    {
      pinMode(ph_hookpulse, INPUT_PULLUP);  /*set IO-pin to input, this way the phone makes no sound and hook pickup is easily detected, by wathcing if the pin goes low*/
      if(digitalRead(ph_hookpulse) == 0)    /*check if hook is picked up*/
      {
        Serial.println("Hook is picked up");
        delay(DELAY_LONG);                    /*delay to wait out the noisy signals created by the hook switch*/
        phone_state = STATE_HOOK_PICK_UP;     /*new state is hook pick up*/
        phone_mode = MODE_SPEEDDIAL;          /*default mode is speed dial mode*/        
        shift = false;                        /*default shift is not pressed*/
        ret_val = PHONE_OFF_HOOK;             /*signal this event to the caller of this routine*/
      }
      else
      {
        ret_val = PHONE_IDLE;
      }
      break;
    }
  }  

  return(ret_val);
}

/*this routine decodes the signal on the hook/pulse line and counts pulses or detects hang-up (receiver put back onto the hook)*/
unsigned char CountDialPulses(void)
{
  bool cur_pulse_state = HIGH;
  bool prev_pulse_state = HIGH; 
  unsigned char pulse_count = 0;
  unsigned char pulse_timeout = 0;
  
  pulse_timeout = PULSE_TIMEOUTVAL; /*reset timeout counter*/
  while(pulse_timeout > 0)  /*if the signal is too high for too long, then this isn't a valid pulse (user hangup or user pressing hookswitch)*/
  {
    prev_pulse_state = cur_pulse_state;
    cur_pulse_state = digitalRead(ph_hookpulse);       
    if((cur_pulse_state == LOW) && (prev_pulse_state == HIGH))        
    {
      pulse_count++;          
      pulse_timeout = PULSE_TIMEOUTVAL; /*reset timeout counter*/          
    }
    else
    {
      delay(10);
      pulse_timeout--;          
    }
  }

  if(digitalRead(ph_hookpulse) == LOW)  /*check current state of hook/pulse signal to determine whether we are hung up or done dialing*/
  {
    Serial.print("detected pulses=");
    Serial.print(pulse_count);
    Serial.print(", this means the user dialed the value ");
    if(pulse_count > 9) {pulse_count = 0;};       /*when 10 pulses are counted this means the value 0 is dialed, se here we correct for that to get the correct numerical value*/
    Serial.println(pulse_count);
    return(pulse_count);
  }
  else
  {
    Serial.println("phone hung up");
    return(255);    /*255=error value for "hung up"*/
  }      
}
    
/*this routine sends keyboard shortcuts to open a webbrowser*/
/*make sure that your desired webbrowser is placed as the 1st item on your taskbar (so that it can be started with the key combo: WINDOWSKEY+SHIFT+1)*/
/*the code below could have been written in loop-style reading an array of actions to save space, but this was easier to code AND easier to read back (due to comments), which might be usefull for other when reading and modifying it*/
void OpenWebbrowser(void)
{    
    /*WINDOWSKEY+1 = start 1st item on the taskbar (which should be firefox or any other webbroswer)*/
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press(KEY_LEFT_SHIFT); /*Open a new instance of the application located on the first position of the taskbar*/
    Keyboard.press('1');
    Keyboard.releaseAll();
    delay(2500);  /*allow some time for the app to start*/

    /*CONTROL+L = focus the address bar (and selects all within) so you can begin typing.*/
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press('l');
    Keyboard.releaseAll();
    delay(250);

    /*clear everything that is already present in the URL filed, if there is nothing thennothing will change, if there was something it will be gone*/
    Keyboard.press(KEY_DELETE);
    Keyboard.releaseAll();
    delay(250);   

    WebbrowserOpen = true;  /*raise flag, so that we know that the webbrowser is opened*/
}

/*the opposite to the openwebbrowser routine, this routine expects the webbrowser to be on top and have the focus*/
void CloseWebbrowser(void)
{ 
  if(WebbrowserOpen == true)   
  {
    WebbrowserOpen = false;
    Serial.println("Closing webbrowser");
    
    Keyboard.press(KEY_LEFT_GUI);     /*set focus to or "open" the taskbar*/
    Keyboard.press('t');    
    Keyboard.releaseAll();
    delay(250);   

    Keyboard.press(KEY_HOME);         /*make sure we are at the start*/
    Keyboard.releaseAll();
    delay(100);   

    Keyboard.press(KEY_UP_ARROW);     /*arrow up allows 'the user" to make a choice from the list of currently opened instances of this program*/
    Keyboard.releaseAll();
    delay(100);   
     
    Keyboard.press(KEY_END);          /*choose the last instance of the application*/
    Keyboard.releaseAll();
    delay(100);   
     
    Keyboard.press(KEY_RETURN);       /*confirm the currently chosen selection from the (possible) list of items)*/
    Keyboard.releaseAll();
    delay(100);   
    
    Keyboard.press(KEY_LEFT_ALT);     /*the close command*/
    Keyboard.press(KEY_F4);    
    Keyboard.releaseAll();
    delay(100);   
  }
  else
  {
    Serial.println("Webbrowser could not be closed because it wasn't opened");
  }
}

/*this routine sends keyboard shortcuts to open a calculator, make sure that the calculator is the 2nd item on the taskbar (so that it can be started with the key combo: WINDOWSKEY+SHIFT+2)*/
void OpenCalculator(void)
{
  Keyboard.press(KEY_LEFT_GUI);
  Keyboard.press(KEY_LEFT_SHIFT); /*Open a new instance of the application located on the second position of the taskbar*/
  Keyboard.press('2');
  Keyboard.releaseAll();
  delay(250);   
      
  CalculatorOpen = true;          /*raise flag, so that we know that the calculator is opened*/
}

/*the opposite to the opencalculator routine*/
void CloseCalculator(void)
{
  if(CalculatorOpen == true)   
  {
    CalculatorOpen = false;
    Serial.println("Closing calculator");
        
    Keyboard.press(KEY_LEFT_GUI);     /*set focus to or "open" the taskbar*/
    Keyboard.press('t');    
    Keyboard.releaseAll();
    delay(250);   

    Keyboard.press(KEY_HOME);         /*make sure we are at the start*/
    Keyboard.releaseAll();
    delay(100);   

    Keyboard.press(KEY_RIGHT_ARROW);     /*arrow right brings us to the second item on the taskbar*/
    Keyboard.releaseAll();
    delay(100);   

    Keyboard.press(KEY_UP_ARROW);     /*arrow up allows 'the user" ta make a choice from the list of currently opened instances of this program*/
    Keyboard.releaseAll();
    delay(100);   
     
    Keyboard.press(KEY_END);          /*choose the last instance of the application*/
    Keyboard.releaseAll();
    delay(100);   
     
    Keyboard.press(KEY_RETURN);       /*confirm the currently chosen selection from the (possible) list of items)*/
    Keyboard.releaseAll();

    Keyboard.press(KEY_LEFT_ALT);     /*the close command*/
    Keyboard.press(KEY_F4);    
    Keyboard.releaseAll();
    delay(100);
  } 
  else
  {
    Serial.println("Calculator could not be closed because it wasn't opened");
  }
}

/*this routine will browse through the bookmarks of firefox and selects one of the top 10 bookmarks*/
/*this way the speeddial is easily configured without the need for hardcoding anything into the firmware*/
void SpeedDial(unsigned char mem)
{
  unsigned char lp;

  /*open the bookmarks menu (this works for English OS only, because in English OS is the B (Bookmarks), but in Dutch OS, the A is used (blAdwijzers)*/
//  Keyboard.press(KEY_LEFT_ALT);
//  Keyboard.releaseAll();
//  delay(250);    
//  Keyboard.press('b');
//  Keyboard.releaseAll();
//  delay(100);    

  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press(KEY_LEFT_SHIFT);
  Keyboard.press('b');
  Keyboard.releaseAll();
  delay(100);    


  if(mem==0)  
  {
    mem=10; /*the value 0 represents the 10th item on the list of items in the bookmarks menu*/
  }

  mem = mem-1;  /*the desired position requires one less arrow down action, because we already start at item 1*/
  
  /*scroll down to the desired location on the list (add .. extra down actions because the menu might have an offset*/  
  for(lp=mem+0; lp>0; lp--)
  {
    Keyboard.press(KEY_DOWN_ARROW);
    Keyboard.releaseAll();
    delay(100);      
  }

  Keyboard.press(KEY_RETURN); /*finalize the selection by opening the selected link*/
  Keyboard.releaseAll();
  delay(100);       

  /*unfortunately, the bookmarks screen is still open, which will cause problems later on, so we must close it now*/
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press(KEY_LEFT_SHIFT);
  Keyboard.press('b');
  Keyboard.releaseAll();
  delay(100);    
  
  Keyboard.press(KEY_LEFT_ALT);     /*the close command*/
  Keyboard.press(KEY_F4);    
  Keyboard.releaseAll();
  delay(100);  
}

void Sound_DialUpModem(void)
{
  unsigned int lp;
  
  Sound_Tone(400, 1500);
  delay(250);
  for(lp=1250;lp>0;lp--)
  { 
    Sound_Tone((200 * random(1, 4)), 8);
  }   
}

void Sound_Tone(unsigned int period, unsigned int duration)
{
  unsigned int lp;
  
  period = period/2;
  for(lp=duration;lp>0;lp--)
  {
    pinMode(ph_hookpulse, OUTPUT);        /*set IO-pin to input, this way the phone makes no sound and hook pickup is easily detected, by watching if the pin goes low*/        
    digitalWrite(ph_hookpulse, HIGH);     /*produce current for the speaker*/
    delayMicroseconds(period);            /*delay to achieve the desired frequency*/  
    pinMode(ph_hookpulse, INPUT_PULLUP);  /*set IO-pin to input, this way the phone makes no sound and hook pickup is easily detected, by watching if the pin goes low*/
    delayMicroseconds(period);            /*delay to achieve the desired frequency*/  
  }
}


/*This routines check if the user has entered a dutch famous value for old telephones, number that were dialed many times before the dawn of the internet*/
/*When this number is dialed by this device,m an appropriate website is being shown*/
void EasterEgg(String str)
{  
  Serial.print("str=");
  Serial.println(str);
  if(str == "002")  /*this is the old Dutch number for the time*/
  {
    Backspace(4);  /*do a specific number of backspaces to delete the entered text*/
    Keyboard.print("https://time.is/nl/");
    Keyboard.press(KEY_RETURN);
    Keyboard.releaseAll();  
  }
  else
  if(str == "003")  /*this is the old Dutch number for the time*/
  {
    Backspace(4);  /*do a specific number of backspaces to delete the entered text*/
    Keyboard.print("https://www.knmi.nl/home");
    Keyboard.press(KEY_RETURN);
    Keyboard.releaseAll();  
  }
  else
    if(str == "008")  /*this is the old Dutch number for the time*/
  {
    Backspace(4);  /*do a specific number of backspaces to delete the entered text*/
    Keyboard.print("https://nl.wikipedia.org/wiki/Nummerinformatiedienst_(Nederland)");
    Keyboard.press(KEY_RETURN);
    Keyboard.releaseAll();  
  } 
  else
    if(str == "3.14159")  /*the magical number on which our whole society is build*/
  {
    Backspace(8);  /*do a specific number of backspaces to delete the entered text*/
    Keyboard.print("https://en.wikipedia.org/wiki/Pi");
    Keyboard.press(KEY_RETURN);
    Keyboard.releaseAll();  
  } 
  else
    if(str == "1.1.2.3.5.8.13.21")  /*the magical number on which our whole society is build*/
  {
    Backspace(18);  /*do a specific number of backspaces to delete the entered text*/   
    Keyboard.print("https://en.wikipedia.org/wiki/Fibonacci_number");
    Keyboard.press(KEY_RETURN);
    Keyboard.releaseAll();  
  } 

}

/*this routines is usefull for situations where multiple backspaces are required*/
void Backspace(unsigned char lp)
{
  unsigned char i;

  for(i=0; i<lp; i++)
  {
    Keyboard.press(KEY_BACKSPACE);        /*undo the entered numbers*/    
    Keyboard.releaseAll();  
    delay(100);    
  }
}
