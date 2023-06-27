#include <SoftwareSerial.h>
#include <Servo.h>
#include <string.h>

#include <stdio.h>


SoftwareSerial bt(8, 9); // 2 - tx, 3 - rxd


Servo servo1; // Servo de la base -> a
Servo servo2; // Servo del hombro -> b
Servo servo3; // Servo del rotor pinza -> c
Servo servo4; // Servo para ang de muñeca -> d
Servo servo5; // Servo para el codo -> e
Servo servo6; // Servo para cerrar la muñeca -> f, max: 105

int in1 = 11; // -> Motor 1
int in2 = 10; // -> Motor 1
int in3 = 13; // -> Motor 2
int in4 = 12; // -> Motor 2

int en1 = 17; // -> Direccional del motor 1
int en2 = 18; // -> Direccional del motor 2


// Variables para guardar valores por defecto en el movimiento
int s1[50], s2[50], s3[50], s4[50], s5[50], s6[50]; 
int positionsActual[6]; 

int velocidad = 15;
int index = 0;


int convertirLetrasANumeros(char letra1) {
  int numero1 = letra1 - 96;
  return numero1 ;
}



void setup() {
  Serial.begin(9600);
  // Serial.begin(57600);
  Serial.println("Listo");
  
  // Comunicacion con bluetooth
  bt.begin(9600);
  // bt.begin(38400);


  // Definiendo pines de salida
  // pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(en1, OUTPUT);
  pinMode(en2, OUTPUT);


  servo1.attach(2);
  servo2.attach(3);
  servo3.attach(4);
  servo4.attach(5);
  servo5.attach(6);
  servo6.attach(7);

  reset_values();
}

void loop() { // run over and over
  if (bt.available()) {
    Serial.write(bt.read());
  }
  
  if (Serial.available()) {
    bt.write(Serial.read());
  }


  //char value = bt.readString()[0];
  String value = bt.readStringUntil('\n');
  value.trim(); 

  Serial.println(value);
  
  if (value != '\0') {
    calc_move(value);
  }
}



void reset_values() {
  move_servo(160, servo1, 'a');
  move_servo(50, servo2, 'b');
  move_servo(0, servo3, 'c');
  move_servo(180, servo4, 'd');
  move_servo(45, servo5, 'e');
  move_servo(0, servo6, 'f');  
}

// Calcular que servo mover
void calc_move(String message) {
  char letter;
  int number;
  sscanf(message.c_str(), "%c-%d", &letter, &number);

  Serial.println(letter);
  switch (letter) {
    case 'a': return move_servo(number, servo1, 'a'); 
    case 'b': return move_servo(number, servo2, 'b'); 
    case 'c': return move_servo(number, servo3, 'c'); 
    case 'd': return move_servo(number, servo4, 'd'); 
    case 'e': return move_servo(number, servo5, 'e'); 
    case 'f': return move_servo(number, servo6, 'f'); 

    case 'g': return move_forward();
    case 'h': return move_back();

    case 'save': return save_position_act();
    // case 'reset': return 
  }
}


void move_servo(int ang, Servo servo, char letter) { 
  int numberLetter = 0;
  int position = positionsActual[numberLetter];
  
  move_position(ang, servo);
  // if (position > ang) {
  //   for (int j = position; j >= ang; j--) {
  //     move_position(j, servo);
  //   }
  // } 
  
  // else {
  //   for (int j = ang; j <= ang; j++) {
  //     move_position(j, servo);
  //   }
  // }

  // position[numberLetter] = ang;
}

void move_position(int ang, Servo servo) {
  servo.write(ang);
  delay(velocidad);
} 


bool isToMoveForward = false;
bool isToMoveBack = false;
void stop_motor() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);

  analogWrite(en1, 0);
  
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);

  analogWrite(en2, 0);

  isToMoveForward = false;
  isToMoveBack = false;
}

void move_forward() {
  if (isToMoveForward) return stop_motor();
  isToMoveForward = true;

  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);

  analogWrite(en1, 255);
  
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);

  analogWrite(en2, 255);
}

void move_back() {
  if (isToMoveBack) return stop_motor();
  isToMoveBack = true;

  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);

  analogWrite(en1, 255);
  
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);

  analogWrite(en2, 255);
}


void save_position_act() {
  s1[index] = servo1.read();
  s2[index] = servo2.read();
  s3[index] = servo3.read();
  s4[index] = servo4.read();
  s5[index] = servo5.read();
  s6[index] = servo6.read();

  index++;
}

void reset_position_act() {
  s1[index] = 0;
  s2[index] = 0;
  s3[index] = 0;
  s4[index] = 0;
  s5[index] = 0;
  s6[index] = 0;

  index = 0;
  reset_values();
}

void run_position_act() {
  
}