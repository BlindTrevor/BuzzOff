#include <LiquidCrystal_I2C.h>
#include "pitches.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);
const int wireLoopPin = 2;
const int winPin = 3;
const int button = 4;
const int buttonLedR = 5;
const int buttonLedG = 6;
const int buttonLedB = 7;
const int speakerPin = 8;
const int buttonLedC = 9;
const int resultLedA = 10;
const int resultLedC = 11;
int wireLoop, win, buttonPress;
unsigned long startTime, endTime, duration, LcdUpdateMillis;
byte timerRunning;

void setup(void){
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  pinMode(buttonLedR, OUTPUT);
  pinMode(buttonLedG, OUTPUT);
  pinMode(buttonLedB, OUTPUT);
  pinMode(buttonLedC, OUTPUT);
  pinMode(resultLedA, OUTPUT);
  pinMode(resultLedC, OUTPUT);
  digitalWrite(buttonLedC, HIGH);
  digitalWrite(buttonLedR, HIGH);
  digitalWrite(buttonLedG, HIGH);
  digitalWrite(buttonLedB, HIGH);
  digitalWrite(resultLedA, HIGH);
  digitalWrite(resultLedC, HIGH);
  pinMode(wireLoopPin, INPUT_PULLUP);
  pinMode(winPin, INPUT_PULLUP);
  pinMode(button, INPUT_PULLUP);
}

void loop(void){  
  wireLoop = digitalRead(wireLoopPin);
  win = digitalRead(winPin);

  Serial.print("Loop:");
  Serial.print(wireLoop);
  Serial.print(",");
  Serial.print("Win:");
  Serial.println(win);

  if(wireLoop == LOW && timerRunning == 1 ){ //Lose
    digitalWrite(buttonLedR, LOW);
    digitalWrite(buttonLedG, HIGH);
    digitalWrite(buttonLedB, HIGH);
    digitalWrite(resultLedA, HIGH);
    digitalWrite(resultLedC, LOW);
    lcd.setCursor(0,0);
    lcd.print("   Try Again!   ");
    winOrLose();
    loseMusic();
  }else if(win == LOW && timerRunning == 1 ){ //Win
    digitalWrite(buttonLedR, HIGH);
    digitalWrite(buttonLedG, LOW);
    digitalWrite(buttonLedB, HIGH);
    digitalWrite(resultLedA, LOW);
    digitalWrite(resultLedC, HIGH);
    lcd.setCursor(0,0);
    lcd.print("   Good Work!   ");
    winOrLose();
    winMusic();
  }else{
    if (timerRunning == 0){ //Waiting to start
      digitalWrite(buttonLedR, HIGH);
      digitalWrite(buttonLedG, HIGH);
      digitalWrite(buttonLedB, LOW);
      lcd.setCursor(0,0);
      lcd.print("  Next Player!  ");
    }else{ //Game in progress
      digitalWrite(buttonLedR, HIGH);
      digitalWrite(buttonLedG, LOW);
      digitalWrite(buttonLedB, LOW);
      if (millis() - LcdUpdateMillis >= 125){
        LcdUpdateMillis = millis();
        lcd.setCursor(0,0);
        lcd.print("   GO GO GO!!   ");
        duration = millis() - startTime;
        lcd.setCursor(0,1);
        lcd.print(printDuration());
      }
    }
    if (timerRunning == 0 && digitalRead(button) == LOW){ //Start Sequence
      digitalWrite(resultLedA, HIGH);
      digitalWrite(resultLedC, HIGH);
      lcd.setCursor(0,0);
      lcd.print("   Get Ready!   ");
      lcd.setCursor(0,1);
      lcd.print("                ");
      digitalWrite(buttonLedR, LOW);
      digitalWrite(buttonLedG, LOW);
      digitalWrite(buttonLedB, LOW);
      delay(1000);

      digitalWrite(buttonLedR, LOW);
      digitalWrite(buttonLedG, HIGH);
      digitalWrite(buttonLedB, LOW);
      lcd.setCursor(0,1);
      lcd.print("  3             ");
      tone(speakerPin, NOTE_C4, 250);
      delay(500);

      digitalWrite(buttonLedR, HIGH);
      digitalWrite(buttonLedG, HIGH);
      digitalWrite(buttonLedB, HIGH);
      delay(500);
      digitalWrite(buttonLedR, LOW);
      digitalWrite(buttonLedG, HIGH);
      digitalWrite(buttonLedB, LOW);
      lcd.setCursor(0,1);
      lcd.print("  3    2        ");
      tone(speakerPin, NOTE_C4, 250);
      delay(500);

      digitalWrite(buttonLedR, HIGH);
      digitalWrite(buttonLedG, HIGH);
      digitalWrite(buttonLedB, HIGH);
      delay(500);
      digitalWrite(buttonLedR, LOW);
      digitalWrite(buttonLedG, HIGH);
      digitalWrite(buttonLedB, LOW);
      lcd.setCursor(0,1);
      lcd.print("  3    2    1   ");
      tone(speakerPin, NOTE_C4, 250);
      delay(1000);

      digitalWrite(buttonLedR, HIGH);
      digitalWrite(buttonLedG, HIGH);
      digitalWrite(buttonLedB, HIGH);
      tone(speakerPin, NOTE_C5, 500);
      
      startTime = millis();
      timerRunning = 1;
    }
  }
}

String printDuration(){
      unsigned long milisecs = duration; //unsigned long
      unsigned long seconds = duration / 1000; //unsigned long
      unsigned long minutes = seconds / 60;         //unsigned long
      unsigned long hours = minutes / 60;           //unsigned long
      unsigned long days = hours / 24;              //unsigned long
      milisecs %= 1000;
      seconds %= 60;
      minutes %= 60;
      hours %= 24;
      String retStr = "";
      if(hours > 0){
        retStr.concat(hours);
        retStr.concat("h ");
      }else{
        retStr.concat("   ");
      }
      if(minutes > 0){
        retStr.concat(minutes);
        retStr.concat("m ");
      }
      retStr.concat(seconds);
      retStr.concat("s ");
      retStr.concat(milisecs);
      retStr.concat("ms     ");
      return(retStr);
}

void winOrLose(){
  if(timerRunning == 1){
    endTime = millis();
    timerRunning = 0;
    duration = endTime - startTime;
    lcd.setCursor(0,1);
    lcd.print(printDuration());
  }
}

void winMusic(){

  // notes in the melody:
    int melody[] = {
      NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
    };

    // note durations: 4 = quarter note, 8 = eighth note, etc.:
    int noteDurations[] = {
      4, 8, 8, 4, 4, 4, 4, 4
    };
  for (int thisNote = 0; thisNote < 8; thisNote++) {

    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(speakerPin, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(speakerPin);
  }
}

void loseMusic(){

  // notes in the melody:
    int melody[] = {
      NOTE_C4, NOTE_B3, NOTE_AS3, NOTE_A3, NOTE_GS3
    };

    // note durations: 4 = quarter note, 8 = eighth note, etc.:
    int noteDurations[] = {
      8, 8, 8, 8, 1
    };
  for (int thisNote = 0; thisNote < 5; thisNote++) {

    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(speakerPin, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(speakerPin);
  }
}
