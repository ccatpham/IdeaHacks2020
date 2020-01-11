import processing.serial.*;
import controlP5.*;

ControlP5 cp5;
Serial port;
int myColor = color(0,0,0);

int sliderTicks1 = 100;
int vendingMachineLimit = 4;
int sliderValue = 0;
boolean submitState = false;
PrintWriter output;
Slider abc;
Button submitButton;


void setup(){
  size(900,600);
  noStroke();
  cp5 = new ControlP5(this);
  //printArray(Serial.list());
  //port = new Serial(this, "COM8", 9600);
  output = createWriter("config.txt");
   
  abc = cp5.addSlider("sliderTicks2")
     .setPosition(20,25)
     .setWidth(400)
     .setRange(1, vendingMachineLimit) // values can range from big to small as well
     .setValue(1)
     .setNumberOfTickMarks(vendingMachineLimit)
     .setSliderMode(Slider.FLEXIBLE)
     ;

  // create a new button with name 'buttonA'
  submitButton = cp5.addButton("SUBMIT")
     .setValue(0)
     .setPosition(550,20)
     .setSize(200,19)
     ;

}

//void sliderTicks2(int theValue){
//  //port.write('a');
//  println("value", theValue);
//}

//void mousePressed(){
//  println(cp5.getValue("sliderTicks2"));
//  output.println("Vending Machine Limit: " + cp5.getValue("sliderTicks2"));
//}

void mousePressed(){
  if (mouseX >=550 && mouseX <= 550+200 && mouseY >= 20 && mouseY <= 20+19){
    output = createWriter("config.txt");
    output.println("Vending Machine Limit: " + cp5.getValue("sliderTicks2"));
    output.flush(); // writes data to the file
    output.close(); // closes the file
  }
}

void draw(){
  background(sliderTicks1);
}
