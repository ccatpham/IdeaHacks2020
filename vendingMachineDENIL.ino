#include <WiFi.h>
#include <Keypad.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "esp32-hal-ledc.h"
//#include <AsyncTCP.h>
//#include <ESPAsyncWebServer.h>
#include <WebServer.h>
#include <Time.h>
#include <Arduino.h>

// Heart Initialization
unsigned long initialTime;
unsigned long finalTime;

// Initializing display values
#define COUNT_LOW 3600
#define COUNT_HIGH 6750
#define TIMER_WIDTH 16
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

String password1 = "1234";
String password2 = "4567";
const int maxDrink1 = 1;
const int maxDrink2 = 0;
const int maxSnack1 = 1;
const int maxSnack2 = 2;
int drinks1 = 0;
int drinks2 = 0;
int snacks1 = 0;
int snacks2 = 0;


String buff = "";
int passlen = 0;


int servo1 = 1;
int servo2 = 2;

const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'},
};

void yeet(int servo, int angle);
void yot(int servo);

void openaisle(int front, int back, int anglef, int angleb);

byte rowPins[ROWS] = {18, 5, 17, 16}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {4 , 23, 15}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
char getPassword(String &pass, int &passlen);
void printNew(String number, int size1 = 5);

WebServer server(80);

String userName = "Denil";
String denilHeart = "88";

// REPLACE WITH YOUR NETWORK CREDENTIALS
const char* ssid = "Samsung Galaxy S7 edge 7512";
const char* password = "IDEAHACK9876";

// Heart Rate Sensor Defintions
const int HR_RX = 34;
byte oldSample, sample;
int bpm = 0;
int beatCounter = 0;
int standardHeartRate = 90;
int cheatingRate = 30;

void calculateBPM() {
  Serial.print("Your BPM is ");
  Serial.println(bpm);
  //Serial.println('/');
}



void heartCalibration() {
  sample = digitalRead(HR_RX);  //Store signal output
  if (sample && (oldSample != sample)) {
    //Serial.println("Beat");
    beatCounter++;
  }
  oldSample = sample;           //Store last signal received
}

void setup() {
  // put your setup code here, to run once:

  
  
  Serial.begin(115200);
  pinMode (HR_RX, INPUT);  //Signal pin to input
 
  Serial.println("Connecting to ");
  Serial.println(ssid);

  //connect to your local wi-fi network
  WiFi.begin(ssid, password);

  //check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());

  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
  
  //setup for servos
  ledcSetup(1, 50, TIMER_WIDTH); // channel 1, 50 Hz, 16-bit width
  ledcAttachPin(13, 1);   // GPIO 13 assigned to channel

  ledcSetup(2, 50, TIMER_WIDTH);
  ledcAttachPin(12, 2);

  ledcSetup(3, 50, TIMER_WIDTH);
  ledcAttachPin(14, 3);

  ledcSetup(4, 50, TIMER_WIDTH);
  ledcAttachPin(27, 4);

  ledcSetup(5, 50, TIMER_WIDTH);
  ledcAttachPin(26, 5);

  ledcSetup(6, 50, TIMER_WIDTH);
  ledcAttachPin(25, 6);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
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

void loop() {
   server.handleClient();
  // put your main code here, to run repeatedly:
  if (getPassword(buff, passlen) == '#') {
    if (buff == password1)
    {
      delay(2000);
      printNew("Passcode  Verified", 2);
      buff = "";
      passlen = 0;
      delay(2000);
      //add servo movements

      selectAisle(maxDrink1, maxSnack1, drinks1, snacks1);
    }
    else if (buff == password2)
    {
      delay(2000);
      printNew("Passcode  Verified", 2);
      buff = "";
      passlen = 0;
      delay(2000);
      //add servo movements

      selectAisle(maxDrink2, maxSnack2, drinks2, snacks2);
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

//servo open/close
void yeet(int servo, int angle) {
  //  for (int i=COUNT_LOW ; i < COUNT_HIGH ; i=i+500)
  //   {
  //      ledcWrite(servo, i);       // sweep servo 1
  //      delay(200);
  //   }

  ledcWrite(servo, (COUNT_LOW - COUNT_HIGH) / 90 * angle + COUNT_HIGH);
}

void yot(int servo) {

  //for (int i= COUNT_HIGH; i > COUNT_LOW ; i=i-500)
  //   {
  //      ledcWrite(servo, i);       // sweep servo 1
  //      delay(200);
  //   }
  ledcWrite(servo, COUNT_HIGH);
}

char getPassword(String & pass, int &passlen)
{
  char key = keypad.getKey();

  if (key != NO_KEY) {
    display.clearDisplay();
    display.setTextSize(5);
    display.setTextColor(WHITE);
    display.setCursor(0, 10);
    if (key == '*') {
      pass = "";
      passlen = 0;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    else if (key == '#' && passlen == 4) {
      display.setTextSize(2);
      display.println("entered: ");
      display.display();
      delay(1000);
      display.setTextSize(5);
    }

    else if (key == '#') {}
    else if ( passlen < 4) {
      pass += key;
      passlen += 1;

    }
    display.println(pass);
    display.display();
  }
  return (key);
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

void selectAisle(int maxDrink, int maxSnack, int &drinks, int &snacks)
{
  printNew("Select \nAisle", 2);
  char key = NO_KEY;
  while (key == NO_KEY)
  {
    key = keypad.getKey();
  }
  switch (key) {
    case '*':
    case '#':
      printNew("Cancelled", 2);
      break;
    case '1': //coffee
      printNew("Checking HR, press\nany button to cancel", 1);
      if (drinks < maxDrink)
      {
        //add heart rate code
        //initiate heart calibration
        

        Serial.println("Waiting for heart beat...");
        char key = NO_KEY;
        while (!digitalRead(HR_RX)) {
          //Serial.println("Heart Beat Detected!");
          key = keypad.getKey();
          if (key != NO_KEY)
          {
            Serial.println("Funtion will return");
            endDisplay();
            return;}
        }
        printNew("Heart beat\ndetected!", 2);

        initialTime = millis();
        finalTime = initialTime;
        while (finalTime - initialTime < 30000) {
        Serial.println("Heart beat detected!");
          if (sample && (oldSample != sample)) {
            beatCounter++;
            Serial.println("Beat");
          }
          oldSample = sample;
          sample = digitalRead(HR_RX);
          finalTime = millis();
        }
        bpm = beatCounter * 2;
        calculateBPM();
        delay(500);
        if (bpm <= standardHeartRate && bpm > cheatingRate ) {
          Serial.println("Test bpm < rate");
          openaisle(1, 2, 90, 90);
          drinks++;
        }
        else if (bpm <= cheatingRate) {
          printNew("Stop\ncheating.", 2);
        }
        else if (bpm > standardHeartRate) {
          printNew("Not now.\nLocked 5m", 2);
        }
      }
      else {
        printNew("Exceed\nmax:" + String(maxDrink), 2)   ;
      }

      break;
    case '2': //rice krispies
      if (snacks < maxSnack) {
        printNew("2 chosen", 2);
        delay(500);
        openaisle(3, 4, 25, 20);
        snacks += 1;
      }
      else {
        printNew("Exceed\nmax:" + String(maxSnack), 2);
      }
      break;
    case '3': //reeses
      if (snacks < maxSnack) {
        printNew("3 chosen", 2);
        delay(500);
        openaisle(5, 6, 25, 20);
        snacks += 1;
      }
      else {
        printNew("Exceed\nmax:" + String(maxSnack), 2);
      }
      break;
    default:
      break;
  }
  endDisplay();
}

void openaisle(int front, int back, int anglef, int angleb)
{
  yeet(front, anglef);
  delay(2000);
  yot (front);
  delay(2000);

  yeet(back, angleb);
  delay(2000);
  yot(back);
  delay(2000);
}

void endDisplay()
{
  printNew("Have a\nnice day");
  printNew("");
}

void handle_OnConnect() {
 
  server.send(200, "text/html", SendHTML(userName,denilHeart)); 
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

String SendHTML(String userName, String denilHeart ){
  String ptr = "<!DOCTYPE html><html><head>";
ptr+="     <title>Amending with Vending</title> \n";
ptr+="     <style> h5 { margin-top: 0px; margin-bottom: 3px;} </style> \n";
ptr+=" </head> \n";
ptr+=" <body> \n";
ptr+="     <div class=\"container\"> \n";
ptr+="         <div class=\"row\" style=\"margin: 5px; padding: 1px; border: 1px solid black; border-radius: 10px;\"> \n";
ptr+="             <p style=\"text-align: center;\">Amending with Vending</p> \n";
ptr+="         </div> \n";
ptr+="         <div class=\"row\" style=\"margin: 5px; padding: 10px; border: 1px solid black; border-radius: 10px;\"> \n";
ptr+="             <h5>Name</h5> \n";
ptr+="             <h7>";
ptr+=                   userName;
ptr+="              </h7> \n";
ptr+="         </div> \n";
ptr+="         <div class=\"row\" style=\"margin: 5px; padding: 10px; border: 1px solid black; border-radius: 10px;\"> \n";
ptr+="             <h5>Heart Rate</h5> \n";
ptr+="             <h7> ";
ptr+=                  denilHeart;
ptr+="             </h7> \n";
ptr+="         </div> \n";
ptr+="         <div class=\"row\" style=\"margin: 5px; padding: 10px; padding-bottom: 10px; border: 1px solid black; border-radius: 10px;\"> \n";
ptr+="             <h5>Daily Maximum Snacks</h5> \n";
ptr+="             <progress value=\"1\" max=\"5\"></progress> <span>1</span> \n";
ptr+="         </div> \n";
ptr+="         <div class=\"row\" style=\"margin: 5px; padding: 10px; border: 1px solid black; border-radius: 10px;\"> \n";
ptr+="             <h5>Items Available</h5> \n";
ptr+="             <ul> \n";
ptr+="                 <li>Reese's Cup</li> \n";
ptr+="                 <li>Rice Krispie Treats</li> \n";
ptr+="                 <li>Energy Drink</li> \n";
ptr+="             </ul> \n";
ptr+="         </div> \n";
ptr+="         <div class=\"row\" style=\"margin: 5px; padding: 10px; border: 1px solid black; border-radius: 10px;\"> \n";
ptr+="             <h5>Heart Rate History</h5> \n";
ptr+="             <ul> \n";
ptr+="                 <li>Past History Here</li> \n";
ptr+="             </ul> \n";
ptr+="         </div> \n";
ptr+="     </div> \n";
ptr+=" </body> \n";
ptr+=" </html> \n";

  return ptr;
}
