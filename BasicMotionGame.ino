
/*
  Ultrasonic/LED Game
  by L. Savage, February 2023
  
  Basic game: LED turns yellow, user must activate corresponding sensor before time runs out.
  
  GAME STATES  
 State 0: Idle (LED off, sensor not active)
 State 1: Play (Yellow LED, sensor waits for user input for X seconds)
 State 2: Win (If Input = True, LED green, 1 point awarded)
 State 3: Loose (If Input = False, LED red, no point awarded)
 
*/

// Neo Pixel setup
#include <Adafruit_NeoPixel.h>
#include "ultrasonicClass.h" 
#define N_LEDS 4

// Define Pins
const int ledPin = 3;
Ultrasonic* ultrasonicSensor0 = new Ultrasonic(4,5);// (Trig PIN,Echo PIN)
Ultrasonic* ultrasonicSensor1 = new Ultrasonic(6,7);// (Trig PIN,Echo PIN)
Ultrasonic* ultrasonicSensor2 = new Ultrasonic(8,9);// (Trig PIN,Echo PIN)
Ultrasonic* ultrasonicSensor3 = new Ultrasonic(10,11);// (Trig PIN,Echo PIN)  

Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, ledPin, NEO_GRB + NEO_KHZ800);

//Range variable in cm. LEDs turn on when object is within range of sensor
int objectRange = 30;   

// Keeping score
int points = 0;

//Delays for difficulty settings
int easySpeed = 750;
int mediumSpeed = 500;
int hardSpeed = 250; 
int longDelay = 1000;
int shortDelay = 500;


//Define pins as input or output, start serial
void setup() {
  Serial.begin(9600);
  strip.begin();
  pinMode(ledPin, OUTPUT); 
}

void loop() {
  
 Serial.println("\nSelect Difficulty: ");
 Serial.println("1 - EASY");
 Serial.println("2 - MEDIUM");
 Serial.println("3 - HARD");
 Serial.println("0 - Hardware Test");

  while (Serial.available() == 0) {
  }

  int menuChoice = Serial.parseInt();

  switch (menuChoice) {
      case 1:
        Serial.println("\nEASY");
        countdown();
        easyMode();
        break;
  
      case 2:
        Serial.println("\nMEDIUM"); 
        countdown();
        mediumMode();
        break;
  
      case 3:
        Serial.println("\nHARD"); 
        countdown();
        hardMode();
        break;
      
      case 0:
       Serial.println("\nHardware Test");
       countdown();
       testGame();
       break;  
  
      default:
        Serial.println("Please choose a valid selection");
    }
     
}


//GAME FUNTIONS 
 
void countdown(){
  Serial.println("\nGame Starting in...");  
  Serial.println("3..."); 
  delay(shortDelay);
  Serial.println("2...");
  delay(shortDelay);
  Serial.println("1...");
  delay(shortDelay);
  Serial.println("GO!");
  delay(shortDelay);
}
  

//Game States
void GameIdle(int i) {
  setSingleLEDcolor(i, strip.Color(0, 0, 0));
}

void GamePlay(int i) {
  setSingleLEDcolor(i, strip.Color(7, 7, 0));
}

int GameWin(int i) {
  setSingleLEDcolor(i,strip.Color(0, 7, 0));
  Serial.println("\n+1 Point!");
  delay(longDelay);
  points++;
  return points;
}

void GameLoose(int i) {
  setSingleLEDcolor(i, strip.Color(7, 0, 0));
  Serial.println("\nMiss");
  delay(longDelay);
}


void motionGame(Ultrasonic* UltrasonicSensor, int i, int gameSpeed){
  
  GamePlay(i);
  delay(gameSpeed);
  
  int distance = UltrasonicSensor->Ranging(CM);
  
   if (distance >= objectRange){
    GameLoose(i);
  }

  else if (distance < objectRange) {
    GameWin(i);
  }

  else {
    GameIdle(i);
  }
  
  setLEDstrip(strip.Color(0, 0, 0));
  delay(gameSpeed);
  
}

// Plays 10 games on random LEDs
void randomizeGames(int gameSpeed){
    
  for (int n = 0; n < 10; n++) { 
    int randNum = random(0,4);
    switch (randNum) {
      
      Serial.println(randNum);
      case 0:
       motionGame(ultrasonicSensor0, 0, gameSpeed);
       break;
  
      case 1:
       motionGame(ultrasonicSensor1, 1, gameSpeed);
       break;
  
      case 2:
       motionGame(ultrasonicSensor2, 2, gameSpeed);
       break;
      
      case 3:
       motionGame(ultrasonicSensor3, 3, gameSpeed);
       break;  
  
      default:
        break; 
    }
    }
}

// Turns on LEDs one at a time for testing
void testGame(){
  points = 0;
  motionGame(ultrasonicSensor0, 0, easySpeed);
  motionGame(ultrasonicSensor1, 1, easySpeed);
  motionGame(ultrasonicSensor2, 2, easySpeed);
  motionGame(ultrasonicSensor3, 3, easySpeed);  
  Serial.println("\nTOTAL POINTS: ");
  Serial.println(points);
}


void easyMode(){
  points = 0;
  randomizeGames(easySpeed);
  Serial.println("\nTOTAL POINTS: ");
  Serial.println(points);
}

void mediumMode(){
  points = 0;
  randomizeGames(mediumSpeed);
  Serial.println("\nTOTAL POINTS: ");
  Serial.println(points);
}

void hardMode(){
  points = 0;
  randomizeGames(hardSpeed);
  Serial.println("\nTOTAL POINTS: ");
  Serial.println(points);
}


// Sets all LEDs one color
static void setLEDstrip(uint32_t c) {
     for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
    }
}


// Sets single LED one color. i is the LED number
static void setSingleLEDcolor(int i, uint32_t c) {
  strip.setPixelColor(i, c);
  strip.show();
}

