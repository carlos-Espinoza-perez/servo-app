#include <SoftwareSerial.h>

SoftwareSerial bt(5, 6);

int listPositions[10][7] { {0, 120, 0, 120, 90, 120}};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Listo");

  bt.begin(38400);
}

void loop() {
  //char value = bt.readString()[0];
  String value = bt.readStringUntil('\n');
  value.trim(); 

  Serial.println(value);
  
  if (value != '\0') {
    //calc_move(value);
  } 
}
