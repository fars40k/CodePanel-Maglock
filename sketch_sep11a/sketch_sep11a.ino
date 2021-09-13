#include <TimerMs.h>
#define FullCondition (digitalRead(8) == LOW)||(digitalRead(9) == LOW)||(digitalRead(10) == LOW) 

// MasterPassword: 9687
// GuestPassword: 2357


TimerMs tmr(1000,1,0);
int AccessLevel = 0; // 0-No 1-Guest 2-Master
bool CodeEntered = false;
int BlipHold = 0;
int HornPin = 12;
int LockPin = 11;
int codeGuestStep = 0;
int codeMasterStep = 0;
long codeResetCountdown = 1000000;

void setup() {
pinMode (13, OUTPUT); 
pinMode (0, OUTPUT);
pinMode (1, OUTPUT);
// Columns and Lines key figure:
//   5  6  7
//8  +  +  +     1  2  3
//9  +  +  +     4  5  6
//10 +  +  +     7  8  9
pinMode (5, OUTPUT);
pinMode (6, OUTPUT);
pinMode (7, OUTPUT);
pinMode (8, INPUT_PULLUP);
pinMode (9, INPUT_PULLUP);
pinMode (10, INPUT_PULLUP);
//
analogWrite(A4,0);
}

void loop() {
AccessLevel = 0;
status_blink();
AccessLevel = scan_keyboard_guest();
if (codeGuestStep == 0)
  {
    AccessLevel = scan_keyboard_master();
  }
if (AccessLevel == 1) 
  {
    lock_5sec_guest();
  } 
if (AccessLevel == 2) 
  {
    lock_5sec_master();
  }
if ((codeGuestStep != 0)||(codeMasterStep != 0)) 
  {
    codeResetCountdown--;
  }  
if ((codeResetCountdown <= 0)) 
  {
    codeGuestStep = 0;
    codeMasterStep = 0;
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

int scan_keyboard_guest() {
    if (codeGuestStep == 4)
  {
    codeGuestStep = 0;
    return(1);
  }
  //Master Scan Column 1
  digitalWrite(5,LOW);
  // 7:
  if ((codeGuestStep==3)&&(digitalRead(10) == LOW))
  {
    codeGuestStep++;
    while (digitalRead(10) == LOW)
      {
        
      }
    delay(100);
    return(0);
  }  
  else if (FullCondition)
  {
    codeGuestStep=0;
    return(0);
  }
  digitalWrite(5,HIGH);
  // Master Scan Column 2
  digitalWrite(6,LOW);
  // 2:
    if ((codeGuestStep==0)&&(digitalRead(8) == LOW))
  {
    codeMasterStep=0;
    codeGuestStep++;
    while (digitalRead(8) == LOW)
      {
        
      }
    delay(100);
    return(0);
  }
   // 5:
    else if ((codeGuestStep==2)&&(digitalRead(9) == LOW))
  {
    codeGuestStep++;
    while (digitalRead(9) == LOW)
      {
        
      }
    delay(100);
    return(0);
  }
  else if (FullCondition)
  {
    codeGuestStep=0;
    return(0);
  }
  digitalWrite(6,HIGH);
  // Master Scan Column 3
  digitalWrite(7,LOW);
  // 3:
  if ((codeGuestStep==1)&&(digitalRead(8) == LOW))
  {
    codeGuestStep++;
    while (digitalRead(8) == LOW)
      {
        
      }
    delay(100);
    return(0);
  }
 else if (FullCondition)
  {
    codeGuestStep=0;
    return(0);
  }
  digitalWrite(7,HIGH);
  //
  return(0);
}

int scan_keyboard_master() {
  if (codeMasterStep == 4)
  {
    codeMasterStep = 0;
    return(2);
  }
  //Master Scan Column 1
  digitalWrite(5,LOW);
  // 7:
  if ((codeMasterStep==3)&&(digitalRead(10) == LOW))
  {
    codeMasterStep++;
    while (digitalRead(10) == LOW)
      {
        
      }
    delay(100);
    return(0);
  }
  else if (FullCondition)
  {
    codeMasterStep=0;
    return(0);
  }
  digitalWrite(5,HIGH);
  // Master Scan Column 2
  digitalWrite(6,LOW);
  // 8:
    if ((codeMasterStep==2)&&(digitalRead(10) == LOW))
  {
    codeMasterStep++;
    while (digitalRead(10) == LOW)
      {
        
      }
    delay(100);
    return(0);
  }
  else if (FullCondition)
  {
    codeMasterStep=0;
    return(0);
  }
  digitalWrite(6,HIGH);
  // Master Scan Column 3
  digitalWrite(7,LOW);
  // 9:
  if ((codeMasterStep==0)&&(digitalRead(10) == LOW))
  {
    codeGuestStep=0;
    codeMasterStep++;
    while (digitalRead(10) == LOW)
      {
        
      }
    delay(100);
    return(0);
  }
  // 6:
  else if ((codeMasterStep==1)&&(digitalRead(9) == LOW))
  {
    codeMasterStep++;
    while (digitalRead(9) == LOW)
      {
        
      }
    delay(100);
    return(0);
  }
  else if (FullCondition)
  {
    codeMasterStep=0;
    return(0);
  }
  digitalWrite(7,HIGH);
  //
  return(0);
}
