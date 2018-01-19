#include <IRremote.h>

#define PIN_IR 3
#define START 7
#define FINISH 8
#define STATUS 13

bool debug = false;

bool ledState = LOW;
unsigned long previousMillis = 0;
const long interval = 500; 

unsigned long startTime = 0;
unsigned long finishTime = 0;

bool startFlag = false;
bool finishFlag = false;

IRsend irsend;

void setup() {
  Serial.begin(9600);
  pinMode(START, INPUT);
  pinMode(FINISH, INPUT);
  pinMode(STATUS, OUTPUT);
  irsend.enableIROut(38);
  irsend.mark(0);
}


void loop() {

  //capture both sensors
  bool startBeam = digitalRead(START);
  bool finishBeam = digitalRead(FINISH);

  if (debug) {
    Serial.print("S: "); Serial.print(startBeam); Serial.print(" sf: "); Serial.print(startFlag); 
    Serial.print(" F: "); Serial.print(finishBeam); Serial.print(" ff: "); Serial.println(finishFlag);
    //Serial.print(" d: "); Serial.println(done);
  }

  // START beam triggered while both flags are lowered
  if (startBeam && !startFlag) {
    Serial.print("* ");
    startTime = millis();
    startFlag = true;
    digitalWrite(STATUS, HIGH);
  }
  
  // FINISH beam triggered while its flag was lowered and START flag is raised
  if (finishBeam && startFlag) {
    //Serial.println("finish beam");
    finishTime = millis();
    finishFlag = true;
    digitalWrite(STATUS, LOW);

    // print the elapsed time
    Serial.print(finishTime - startTime); Serial.println(" ms");
    
    //reset the flags
    startFlag = false; 
    finishFlag = false;
  }
  
  // nothing has happened with the beams: just blink the STATUS LED with no delay
  if (!startFlag && !finishFlag) { 
    // check to see if it's time to blink the LED; that is, if the difference
    // between the current time and last time you blinked the LED is bigger than
    // the interval at which you want to blink the LED.
    unsigned long currentMillis = millis();
    
    if (currentMillis - previousMillis >= interval) {
      // save the last time you blinked the LED
      previousMillis = currentMillis;
      
      // set the STATUS LED with the ledState of the variable:
      ledState = !ledState;
      digitalWrite(STATUS, ledState ? HIGH : LOW);
    }
  }
}
