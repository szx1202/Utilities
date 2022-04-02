//allows to increment or decrement the Servo rotation by 2 button for increment and decrement
// data are shown in an OLED 128x32
// tested with Arduino nano

#include <Servo.h>
#include <Bounce2.h>
#include <Streaming.h> // to use Serial to print string and Value
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SERVO_PIN 8
#define BUTTON1_PIN 2 
#define BUTTON2_PIN 3
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// INSTANTIATE A Button OBJECT FROM THE Bounce2 NAMESPACE
Bounce2::Button button1 = Bounce2::Button();
Bounce2::Button button2 = Bounce2::Button();

const int PRESS_TIME = 2000; // 2000 milliseconds

// Variables will change:
int lastState = LOW;  // the previous state from the input pin
int currentState;     // the current reading from the input pin
unsigned long pressedTimeStamp  = 0;
unsigned long releasedTimeStamp = 0;
unsigned long elaps=0;

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int servoClose = 10; // variable to store the servo position initial
int servoThrow = 170; // variable to store the servo position turned
int servoStep=5; // increment decrement value
int servoPos; // actual position value

void setup() {
  
  Serial.begin(9600);
    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds
  
  myservo.attach(SERVO_PIN);  // attaches the servo on pin 9 to the servo object
  button1.attach( BUTTON1_PIN, INPUT ); // decrease USE EXTERNAL PULL-UP 10K resistor
  button2.attach( BUTTON2_PIN, INPUT ); // increae USE EXTERNAL PULL-UP 10K resistor

  // DEBOUNCE INTERVAL IN MILLISECONDS
  button1.interval(5); 
  button2.interval(5);

  // INDICATE THAT THE LOW STATE CORRESPONDS TO PHYSICALLY PRESSING THE BUTTON
  button1.setPressedState(LOW); 
  button2.setPressedState(LOW);
  myservo.write(servoClose);
  servoPos=servoClose;

Prn_Oled();
}


void loop() {
   
  button1.update(); // UPDATE THE BUTTON YOU MUST CALL THIS EVERY LOOP
  button2.update(); // UPDATE THE BUTTON YOU MUST CALL THIS EVERY LOOP

  if ( button1.rose()  ) {  
   pressedTimeStamp = millis();
    Serial.println(pressedTimeStamp);
}
  if ( button1.fell()) {
      releasedTimeStamp = millis();
      Serial.println(releasedTimeStamp);
      elaps=releasedTimeStamp-pressedTimeStamp;
      
      if((releasedTimeStamp>pressedTimeStamp+PRESS_TIME)){
        servoPos=servoClose;
        Serial.println("reset servo position");
      }
  }
  
  if ( button1.pressed() ) { //increase the value 
    if ((servoPos-servoStep)>=servoClose){ //enough to decrease
      myservo.write(servoPos-servoStep);
      servoPos=servoPos-servoStep;      
      Serial << "ServoPos " << servoPos <<endl;
    }
    else {
      myservo.write(servoClose);
      servoPos=servoClose;
      Serial << "button1 " << servoPos <<endl;
   }
  }
  
  if ( button2.pressed() ) { //increase the value
    if ((servoPos+servoStep)<=servoThrow){
      myservo.write(servoPos+servoStep);
      servoPos=servoPos+servoStep;
      Serial << "button2 " << servoPos <<endl;  
      }
    else{
      myservo.write(servoThrow);
      servoPos=servoThrow;
      Serial << "button2 " << servoPos <<endl;
    }
  }
  
  Prn_Oled();
}

void Prn_Oled(){
  display.clearDisplay();
    // text display tests
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.print("servoClose= ");
  display.println(servoClose);
  display.print("servoThrow= ");
  display.println(servoThrow);
  display.print("servoStep= ");
  display.println(servoStep);
  display.print("servoPos= ");
  display.println(servoPos);
  display.display(); // actually display all of the above  
 }
