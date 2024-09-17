#include <LiquidCrystal_I2C.h>
#include "pitches.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);
const int startPin = 2;
const int endPin = 3;
const int button = 4;
const int buttonLedR = 5;
const int buttonLedG = 6;
const int buttonLedB = 7;
const int speakerPin = 8;
const int buttonLedC = 9;
int start, end_, buttonPress;
unsigned long startTime, endTime, duration;
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
  digitalWrite(buttonLedC, HIGH);
  digitalWrite(buttonLedR, HIGH);
  digitalWrite(buttonLedG, HIGH);
  digitalWrite(buttonLedB, HIGH);
  pinMode(startPin, INPUT_PULLUP);
  pinMode(startPin, INPUT_PULLUP);
  pinMode(button, INPUT_PULLUP);
}

void loop(void){  
  start = digitalRead(startPin);
  end_ = digitalRead(endPin);
  unsigned long currentCount = 0;
  Serial.print(start);
  Serial.print("\n");
  Serial.print(end_);  
  if (start == LOW && end_== LOW ){
    if (timerRunning == 1){
      digitalWrite(buttonLedR, LOW);
      digitalWrite(buttonLedG, HIGH);
      digitalWrite(buttonLedB, HIGH);
      lcd.setCursor(0,0);
      lcd.print("Try Again");
      winOrLose();
      loseMusic();
      delay(3000);
    }
  }else if (start == HIGH && end_== LOW){
    if (timerRunning == 1){
      digitalWrite(buttonLedR, HIGH);
      digitalWrite(buttonLedG, LOW);
      digitalWrite(buttonLedB, HIGH);
      lcd.setCursor(0,0);
      lcd.print("Well Done");
      winOrLose();
      winMusic();
      delay(3000);
    }
  }else{
    lcd.setCursor(0,0);
    if (timerRunning == 0){
    digitalWrite(buttonLedR, HIGH);
    digitalWrite(buttonLedG, HIGH);
    digitalWrite(buttonLedB, LOW);
      lcd.print("Ready           ");
    }else{
    digitalWrite(buttonLedR, HIGH);
    digitalWrite(buttonLedG, LOW);
    digitalWrite(buttonLedB, LOW);
      lcd.print("Go           ");
      duration = millis() - startTime;
      lcd.setCursor(0,1);
      lcd.print(duration);lcd.print ("     ");
    }


    if (timerRunning == 0 && digitalRead(button) == LOW){
      startTime = millis();
      timerRunning = 1;
    }

    

  }
}

void winOrLose(){
  if(timerRunning == 1){
    endTime = millis();
    timerRunning = 0;
    duration = endTime - startTime;
    lcd.setCursor(0,1);
    lcd.print(duration);lcd.print ("     ");
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
    tone(8, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(8);
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