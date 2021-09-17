#include <Keypad.h>
#include <TimerMs.h>

char MasterPassword[4] = {'9','6','8','7'};
char GuestPassword[4] = {'2','3','5','7'};

const byte ROWS = 3; 
const byte COLS = 3;
char hexaKeys[ROWS][COLS] = {
{'1','2','3'}, 
{'4','5','6'},
{'7','8','9'}
};
byte rowPins[ROWS] = {8, 9, 10};
byte colPins[COLS] = {5, 6, 7};

Keypad Keys = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

TimerMs tmr(1000,1,0);
int AccessLevel = 0; // 0-No 1-Guest 2-Master
int HornPin = 12;
int LockPin = 11;
int CodeStep = 0;
int CheckLock = 0;
long codeResetCountdown = 1000000;

void setup() {
pinMode (13, OUTPUT); 
pinMode (12, OUTPUT); 
pinMode (11, OUTPUT); 

/*
 * pinMode (5, OUTPUT);
pinMode (6, OUTPUT);
pinMode (7, OUTPUT);
pinMode (8, INPUT_PULLUP);
pinMode (9, INPUT_PULLUP);
pinMode (10, INPUT_PULLUP);
 */
//
analogWrite(A4,0);
//
Keys.addEventListener(keypadEvent);
}

void loop() {
status_blink();
char key = Keys.getKey();
if (AccessLevel == 1) 
  {
    lock_5sec_guest();
    AccessLevel = 0;
  } 
if (AccessLevel == 2) 
  {
    lock_5sec_master();
    AccessLevel = 0;
  }
if (CodeStep > 0) 
  {
    codeResetCountdown--;
  }  
if ((codeResetCountdown <= 0)) 
  {
    CodeStep = 0;
    codeResetCountdown = 1000000;
  } 
}

void status_blink(void) {
 
  if (tmr.timeLeft() > 500) 
    {
    digitalWrite(13,LOW);
    }
   if (tmr.timeLeft() < 500) 
    {
    digitalWrite(13,HIGH);
    }
  if (tmr.tick()) 
   {
    tmr.start();
   }
}


void lock_5sec_master(void) {
  digitalWrite(13,HIGH);
  digitalWrite(LockPin,HIGH);
  delay(5000);
  digitalWrite(LockPin,LOW);
  digitalWrite(13,LOW);
}

void lock_5sec_guest(void) {
  digitalWrite(13,HIGH);
  digitalWrite(LockPin,HIGH);
  digitalWrite(HornPin,HIGH);
  delay(1000);
  digitalWrite(HornPin,LOW);
  delay(4000);
  digitalWrite(LockPin,LOW);
  digitalWrite(13,LOW);
}

void keypadEvent(KeypadEvent key)
{
if ((key == MasterPassword[CodeStep])&&(CheckLock != 1))
{
  if (CodeStep == 0)
  {
    CheckLock = 2;
  }
  CodeEnterSignal();
  CodeStep++;
  if (CodeStep == 4)
  {
    AccessLevel = 2;
    CodeStep = 0;
  }
} else
if ((key == GuestPassword[CodeStep])&&(CheckLock != 2))
{
  if (CodeStep == 0)
  {
    CheckLock = 1;
  }
  CodeEnterSignal();
  CodeStep++;
  if (CodeStep == 4)
  {
    AccessLevel = 1;
    CodeStep = 0;
  }
} else
{ 
  CheckLock = 0;
  CodeStep = 0; 
}
  
}


void CodeEnterSignal(void)
{
  if (CodeStep != 0) {
  for(int i; i<=CodeStep ; i++ )
  {
    analogWrite(A4,255);
    delay(400);
    analogWrite(A4,0);
  }
}
}
