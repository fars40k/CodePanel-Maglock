#include <Keypad.h>
#include <TimerMs.h>

char MasterPassword[4] = {'9','6','8','7'};
char GuestPassword[4] = {'2','3','5','7'};

const byte ROWS = 3; 
const byte COLS = 3;
char Keys[ROWS][COLS] = {
{'1','2','3'}, 
{'4','5','6'},
{'7','8','9'}
};
byte rowPins[ROWS] = {8, 9, 10};
byte colPins[COLS] = {5, 6, 7};

Keypad KeyMap = Keypad( makeKeymap(Keys), rowPins, colPins, ROWS, COLS);

TimerMs tmr(1000,1,0);
int AccessLevel = 0; // 0-No 1-Guest 2-Master
int HornPin = 12;
int LockPin = 11;
int CodeStep = 0;
int CheckLock = 0;
long codeResetCountdown = 6000000;

void setup() {
pinMode (13, OUTPUT); 
pinMode (12, OUTPUT); 
pinMode (11, OUTPUT); 

//
analogWrite(A4,0);
//
KeyMap.addEventListener(keypadEvent);
KeyMap.setDebounceTime(20);
//
Serial.begin(9600);
while (!Serial)
{
  
}
}

void loop() {
status_blink();
if (AccessLevel == 1) 
  {
    Serial.print("GuestAccessConfirmed");
    lock_5sec_guest();
    AccessLevel = 0;
  } 
if (AccessLevel == 2) 
  {
    Serial.print("MasterAccessConfirmed");
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
    codeResetCountdown = 6000000;
    Serial.print(" Reset ");
  }
char key = KeyMap.getKey(); 
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
  Serial.print("Key scanned: ");
  Serial.println(key);
if ((key == MasterPassword[CodeStep])&&(CheckLock != 1))
{
  Serial.print("Master password step: ");
  Serial.println(1+CodeStep);  
  if (CodeStep == 0)
    {
      CheckLock = 2;
    }

  CodeStep++;
    if (CodeStep == 4)
    {
      AccessLevel = 2;
      CodeStep = 0;
    }
} else
if ((key == GuestPassword[CodeStep])&&(CheckLock != 2))
{
  Serial.print("Guest password step: ");
  Serial.println(1+CodeStep);
    if (CodeStep == 0)
    {
      CheckLock = 1;
    }

  CodeStep++;
    if (CodeStep == 4)
    {
      AccessLevel = 1;
      CodeStep = 0;
    }
} else
  { 
     if (((CheckLock == 1)&&(key != GuestPassword[CodeStep-1]))||((CheckLock == 2)&&(key != MasterPassword[CodeStep-1]))||(CheckLock == 0))
    {
      Serial.print("Wrong password char: ");
      Serial.println(key);
      CheckLock = 0;
      CodeStep = 0; 
    }
  } 
}
