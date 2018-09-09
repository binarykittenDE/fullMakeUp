#include <Servo.h>

#include <Adafruit_NeoPixel.h>
#define PIXEL_PIN    5    // Digital IO pin connected to the NeoPixels.
#define PIXEL_COUNT 15

// Parameter 1 = number of pixels in strip,  neopixel stick has 8
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream, correct for neopixel stick
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip), correct for neopixel stick
Adafruit_NeoPixel ledStrip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

//Servos
// create servo objects to control the servos (twelve servo objects can be created on most boards)
Servo leftEyeShadowServo;  
Servo rightEyeShadowServo;
Servo lipStickServo;
Servo leftRougeServo; 
Servo rightRougeServo;

//Positions
int leftEyeShadowServoPosition = 0;
int rightEyeShadowServoPosition = 0;
int lipStickServoPosition = 0;  
int leftRougeServoPosition = 0; 
int rightRougeServoPosition = 0; 

//Pins
int leftEyeShadowServoPin = 10;
int rightEyeShadowServoPin = 8;
int lipStickServoPin = 9;
int leftRougeServoPin = 7;
int rightRougeServoPin = 11;

//Button
int buttonPin = 6;
int buttonState = 0;
int oldButtonState = 0;

void setup() {
  Serial.begin(115200);

  leftEyeShadowServo.attach(leftEyeShadowServoPin);
  rightEyeShadowServo.attach(rightEyeShadowServoPin);
  lipStickServo.attach(lipStickServoPin);
  leftRougeServo.attach(leftRougeServoPin);
  rightRougeServo.attach(rightRougeServoPin);

  pinMode(buttonPin, INPUT_PULLUP);
  setAllDefaultPositions();
  //Led Strip
  ledStrip.begin();
  ledStrip.show(); // Initialize all pixels to 'off'
}

void loop() { 
  //Buttonstate got changed!!
  buttonState = digitalRead(buttonPin);
  
    if(buttonState == LOW){
      doEyesAndLips();
      doRouge();

      paparazziLight(ledStrip.Color(127, 127, 127),50);
      
      delay(30000);
      setDefaultLipPosition();
    }
  
}


#define NUM_FRAMES 100

void doEyesAndLips(){
  for(int r = 0; r < 4; r++) {
    for(int f=0; f < NUM_FRAMES; f++) {
      float angle = 6.28f * ((float)f / (float)NUM_FRAMES);
      float v = cos(angle);
      //Mittelpunkt + (cast)(+-in die richtungen * v)
      leftEyeShadowServoPosition = 70 + (int)(20.0f * v);
      rightEyeShadowServoPosition = 110 - (int)(20.0f * v);
      lipStickServoPosition = 40 + (int)(20.0f * v);

      leftEyeShadowServo.write(leftEyeShadowServoPosition); 
      rightEyeShadowServo.write(rightEyeShadowServoPosition);
      lipStickServo.write(lipStickServoPosition);

      delay(10);
    }
  }
  lipStickServo.write(40);
  setDefaultEyeShadowPositions();
}

void doRouge(){
  Serial.println("doRouge()");
  leftRougeServo.write(170);
  rightRougeServo.write(10);
  delay(400);
  setRougeDefaultPositions();
}

void setDefaultEyeShadowPositions(){
  leftEyeShadowServo.write(110);
  rightEyeShadowServo.write(60);
}

void setDefaultLipPosition(){
   lipStickServo.write(0);
}

void setRougeDefaultPositions(){
  leftRougeServo.write(10);
  rightRougeServo.write(170);
}

void setAllDefaultPositions(){
  setDefaultEyeShadowPositions();
  setDefaultLipPosition();
  setRougeDefaultPositions();
}

void paparazziLight(uint32_t c, uint8_t wait) {
  int amountOfRoundsToDoThis = 50;
  for (int j=0; j<amountOfRoundsToDoThis; j++) {  
    for (int q=0; q < 3; q++) {
      for (int i=0; i < ledStrip.numPixels(); i=i+3) {
        ledStrip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      ledStrip.show();

      delay(wait);

      for (int i=0; i < ledStrip.numPixels(); i=i+3) {
        ledStrip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
  colorWipe(ledStrip.Color(0, 0, 0), 50); //Black. Means off
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<ledStrip.numPixels(); i++) {
    ledStrip.setPixelColor(i, c);
    ledStrip.show();
    delay(wait);
  }
}
