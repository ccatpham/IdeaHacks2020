#include <Keypad.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "esp32-hal-ledc.h"

#define COUNT_LOW 3600
#define COUNT_HIGH 6750
#define TIMER_WIDTH 16
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

String password1 = "1234";
String buff = "";
int passlen = 0;

int servo1 = 1;
int servo2 = 2;

const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'},
};

void yeet(int servo);
void yot(int servo);

void openaisle(int front,int back);

byte rowPins[ROWS] = {18, 5, 17, 16}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {4 , 23, 15}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
char getPassword(String &pass, int &passlen);
void printNew(String number, int size1 = 5);



void setup(){
  Serial.begin(115200);
   //setup for servos
   ledcSetup(1, 50, TIMER_WIDTH); // channel 1, 50 Hz, 16-bit width
   ledcAttachPin(13, 1);   // GPIO 13 assigned to channel 

   ledcSetup(2,50, TIMER_WIDTH);
   ledcAttachPin(12, 2);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  // Display static text
  display.println("Enter");
  display.println("Passcode:");
  display.display(); 
  display.setTextSize(3);
}

void loop(){
  if(getPassword(buff, passlen) == '#'){
    if(buff == password1)
    {
      delay(2000);
      printNew("Passcode  Verified", 2);
      buff = "";
      passlen = 0;
      delay(2000);
      //add servo movements

      selectAisle();


      
    }
    else
    {
      delay(2000);
      printNew("Invalid,\nTry Again", 2);
      buff = "";
      passlen = 0;
    }
  }
}

char getPassword(String &pass, int &passlen)
{
    char key = keypad.getKey();

  if (key != NO_KEY){
     display.clearDisplay();
     display.setTextSize(5);
     display.setTextColor(WHITE);
     display.setCursor(0, 10);
    if (key == '*'){
      pass = "";
      passlen = 0;
    }



    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    else if (key == '#' && passlen == 4){
      display.setTextSize(2);
      display.println("entered: ");
      display.display();
      delay(1000);
      display.setTextSize(5);
    }

    else if (key == '#'){}
    else if ( passlen < 4){
      pass+=key;
      passlen+=1;
      
    }


    display.println(pass);
    display.display();
  }
  return(key);
}

void printNew(String number, int size1)
{
     display.clearDisplay();
     display.setTextSize(size1);
     display.setTextColor(WHITE);
     display.setCursor(0, 10);
     display.println(number);
     display.display();
}

void selectAisle()
{
  printNew("Select \nAisle", 2);
  char key = NO_KEY;
  while(key == NO_KEY)
  {
    key = keypad.getKey();
  }
  switch (key) {
    case '*':
    case '#':
      printNew("Cancelled", 2);
      break;
    case '1':
      printNew("1 chosen", 2);
      delay(500);
      openaisle(1,2);
      break;
    case '2':
      printNew("2 chosen", 2);
      break;
    case '3':
      printNew("3 chosen", 2);
      break;
    default:
      break;
  }
  
}


//servo open/close 
void yeet(int servo){
//  for (int i=COUNT_LOW ; i < COUNT_HIGH ; i=i+500)
//   {
//      ledcWrite(servo, i);       // sweep servo 1
//      delay(200);
//   }

ledcWrite(servo,COUNT_HIGH);
}

void yot(int servo){
      
//for (int i= COUNT_HIGH; i > COUNT_LOW ; i=i-500)
//   {
//      ledcWrite(servo, i);       // sweep servo 1
//      delay(200);
//   }
ledcWrite(servo,COUNT_LOW);
}

void openaisle(int front, int back)
{
   yeet(front);
   delay(2000);
   yot (front);
   delay(2000);

   yeet(back);
   delay(5000);
   yot(back);
   delay(2000);
}
