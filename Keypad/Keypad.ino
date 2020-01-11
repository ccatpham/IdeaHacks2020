#include <Servo.h>
#include <Keypad.h>

String password = "";
int passlen = 0;

const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'},
};
byte rowPins[ROWS] = {4, 7, 8, 12}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {13, 14, 15}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup(){
  Serial.begin(9600);
  Serial.print("");
}

void loop(){
  char key = keypad.getKey();

  if (key != NO_KEY){
    if (key == '*'){
      password = "";
      passlen = 0;
    }
    else if (key == '#' && password.length() == 4){
      Serial.print("entered: ");
    }
    else if (key == '#'){}
    else if ( passlen < 4){
      password+=key;
      passlen+=1;
    }
    
    Serial.println(password);
  }
}
