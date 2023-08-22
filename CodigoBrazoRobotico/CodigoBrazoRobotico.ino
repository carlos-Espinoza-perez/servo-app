#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>


// Definicion de variables iniciales
Adafruit_PWMServoDriver listServos = Adafruit_PWMServoDriver(0x40);
unsigned int pos0 = 204;
unsigned int pos180 = 409;


SoftwareSerial bt(5, 6);



// Guardado de posiciones
int positionsInitial[6] = { 0, 0, 0, 1, 3, 4 };

bool savingPositions = false;
int listPositions[100][2];  // { n_servo, ang }

int indexPosition = 0;

// Pines para la configuracion para los motores
int speedMotor1 = 10;
int speedMotor2 = 11;

int dir1Motor1 = 8;
int dir1Motor2 = 12;

int dir2Motor1 = 9;
int dir2Motor2 = 13;



void setup() {
  bt.begin(38400);

  // Configuracion de los servo
  listServos.begin();
  listServos.setPWMFreq(50);

  // Configuracion de los motores
  pinMode(speedMotor1, OUTPUT);
  pinMode(speedMotor2, OUTPUT);

  pinMode(dir1Motor1, OUTPUT);
  pinMode(dir1Motor2, OUTPUT);

  pinMode(dir2Motor1, OUTPUT);
  pinMode(dir2Motor2, OUTPUT);


  defaultPositionServo();
}

void loop() {
  //char value = bt.readString()[0];
  String value = bt.readStringUntil('\n');
  value.trim();

  Serial.println(value);

  if (value != "\0") {
    calc_move(value);
  }
}










/*
  Lista de acciones por los numeros recibidos
  0 al 6 -> Servo motores

  7-> Borrar ultimo movimiento
  8 -> Guardar movimientos
  9 -> Cancelar automatizacion
  10 -> Iniciar movimientos

  11 -> Adelante
  12 -> Atras
  13 -> Izquierda
  14 -> Derecha
*/


void calc_move(String message) {
  uint8_t n_action;
  int ang;
  sscanf(message.c_str(), "%d-%d", &n_action, &ang);

  // Validando que son los 6 servos los que se estan moviendo
  if (n_action <= 6) {
    moveServo(n_action, ang);

    return;
  }


  switch (n_action) {
    case 7: return deleteLastMoveSaved();
    case 8: return saveMovements();
    case 9: return cancelAutomatization();
    case 10: return initAutomaization();
    
    case 11: return moveForward();
    case 12: return moveBack();
    case 13: return moveRight();
    case 14: return moveLeft();
  }
}


// Numero 1 al 6 - Mover el brazo
void moveServo(uint8_t n_servo, int ang) {
  setServo(n_servo, ang);

  if (savingPositions) {
    listPositions[indexPosition][0] = n_servo;
    listPositions[indexPosition][1] = ang;
    indexPosition += 1;
  }
}

// Numero 7 - Borrar ulitmo movimiento
void deleteLastMoveSaved() {
  indexPosition--;

  listPositions[indexPosition + 1][0] = -1;
  listPositions[indexPosition + 1][1] = 0;
}

// Numero 8 - Guardar los movimientos que esten entrando
void saveMovements() {
  savingPositions = true;
  defaultPositionServo();
}

// Numero 9 - Cancelar el guardar movimientos
void cancelAutomatization() {
  savingPositions = false;
  indexPosition = 0;

  for (int index = 0; index < indexPosition; index++) {
    listPositions[index][0] = -1;
    listPositions[index][1] = 0;
  }
}

// Numero 10 - Iniciar la ejecucion de la automatizacion
void initAutomaization() {
  // Setiar posicinoes iniciales
  defaultPositionServo();

  // Comenzando a ejecutar la automaizacion
  for (int index = 0; index < sizeof(positionsInitial) / sizeof(positionsInitial[0]); index++) {
    if (listPositions[index][0] == -1) continue;
    if (savingPositions == false) break;

    setServo(listPositions[index][0], listPositions[index][1]);
    delay(300);
  }
}


// Numero 11 - Mover hacia adelante
void moveForward() {
  digitalWrite(dir1Motor1, 1);
  digitalWrite(dir2Motor1, 0);

  digitalWrite(dir1Motor2, 1);
  digitalWrite(dir2Motor2, 0);

  analogWrite(speedMotor1, 255);
  analogWrite(speedMotor2, 255);
}

// Numero 12 - Mover hacia atras
void moveBack() {
  digitalWrite(dir1Motor1, 0);
  digitalWrite(dir2Motor1, 1);

  digitalWrite(dir1Motor2, 0);
  digitalWrite(dir2Motor2, 1);

  analogWrite(speedMotor1, 255);
  analogWrite(speedMotor2, 255);
}

// Numero 13 - Mover hacia la derecha
void moveRight() {
  digitalWrite(dir1Motor1, 1);
  digitalWrite(dir2Motor1, 0);

  digitalWrite(dir1Motor2, 1);
  digitalWrite(dir2Motor2, 0);

  analogWrite(speedMotor1, 60);
  analogWrite(speedMotor2, 255);

  delay(1500);

  analogWrite(speedMotor1, 0);
  analogWrite(speedMotor2, 0);
}

// Numero 12 - Mover hacia la izquierda
void moveLeft() {
  digitalWrite(dir1Motor1, 1);
  digitalWrite(dir2Motor1, 0);

  digitalWrite(dir1Motor2, 1);
  digitalWrite(dir2Motor2, 0);

  analogWrite(speedMotor1, 255);
  analogWrite(speedMotor2, 60);

  delay(1500);

  analogWrite(speedMotor1, 0);
  analogWrite(speedMotor2, 0);
}






// Poniendo los servos en sus valores iniciales
void defaultPositionServo() {
  for (int index = 0; index < sizeof(positionsInitial) / sizeof(positionsInitial[0]); index++) {
    setServo(index, positionsInitial[index]);
    delay(300);
  }
}

// Moviendo posiciones de los servo
void setServo(uint8_t n_servo, int angulo) {
  int duty = map(angulo, 0, 180, pos0, pos180);
  listServos.setPWM(n_servo, 0, duty);
}
