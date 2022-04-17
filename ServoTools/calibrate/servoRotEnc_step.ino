// https://dronebotworkshop.com/rotary-encoders-arduino/
// tested with Arduino UNO and Nano

// Create a Servo object
#include <Servo.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Rotary Encoder Inputs
#define INPUT_CLK 4
#define INPUT_DT 5
#define INPUT_SW 6 
#define SERVO_PIN 9
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

Servo myservo;
int servoClose = 10; // variable to store the servo position initial
int servoThrow = 170; // variable to store the servo position turned
int servoStep=5; // increment decrement value
int counter = 1; // Encoder step value
int servoPos= servoClose; // actual position value
int currentStateCLK;
int previousStateCLK; 
unsigned long lastButtonPress = 0;
 
void setup() { 
  
  // Set encoder pins as inputs  
  pinMode (INPUT_CLK,INPUT);
  pinMode (INPUT_DT,INPUT);
  pinMode (INPUT_SW,INPUT_PULLUP);
 
  Serial.begin (9600);
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  
  display.display();
  delay(2000); // Pause for 2 seconds
      // Clear the buffer
  display.clearDisplay();
    // text display tests
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  Prn_Oled();
  
  // Attach servo on pin 9 to the servo object
   myservo.attach(SERVO_PIN);
   myservo.write(servoPos);  
   // Read the initial state of INPUT_CLK
   // Assign to previousStateCLK variable
   previousStateCLK = digitalRead(INPUT_CLK); 
 } //End Setup 
 
void loop() {
  
  // Read the current state of INPUT_CLK
   currentStateCLK = digitalRead(INPUT_CLK);
    
   // If the previous and the current state of the INPUT_CLK are different then a pulse has occured
   if (currentStateCLK != previousStateCLK){ 
       
     // If the INPUT_DTstate is different than the INPUT_CLK state then 
     // the encoder is rotating counterclockwise
     if (digitalRead(INPUT_DT) != currentStateCLK) { 
       counter --;
       servoPos=servoPos-servoStep;
       if (servoPos<servoClose){
        counter=1;
        servoPos=servoClose;
       }
      
     } else {
       // Encoder is rotating clockwise
       counter ++;
       servoPos=servoPos+servoStep;
       if (servoPos>servoThrow){
        counter=servoThrow;
        servoPos=servoThrow;
       }
     }
     
     // Move the servo
     myservo.write(servoPos);
      
     Serial.print("Position: ");
     Serial.println(servoPos);
     Prn_Oled();
   } 
 
       // Read the button state
  int btnState = digitalRead(INPUT_SW);

  //If we detect LOW signal, button is pressed
  if (btnState == LOW) {
    //if 50ms have passed since last LOW pulse, it means that the
    //button has been pressed, released and pressed again
    if (millis() - lastButtonPress > 50) {
      Serial.println("Button pressed!");
      counter=servoClose;
      servoPos=counter;
      myservo.write(servoPos);
      Prn_Oled();
    }

    // Remember last button press event
    lastButtonPress = millis();
  }
   // Update previousStateCLK with the current state
   previousStateCLK = currentStateCLK; 
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
