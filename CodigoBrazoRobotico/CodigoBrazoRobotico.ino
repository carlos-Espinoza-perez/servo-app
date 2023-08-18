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

boolean savingPositions = false;
int listPositions[2][10] = {}; // { n_servo, ang } 
int indexPosition = 0;





void setup() {
  bt.begin(38400);

  listServos.begin();
  listServos.setPWMFreq(50);
}

void loop() {
  //char value = bt.readString()[0];
  String value = bt.readStringUntil('\n');
  value.trim();

  Serial.println(value);

  if (value != "\0") {
    //calc_move(value);
  }
}






// Calcular que servo mover
void calc_move(String message) {
  uint8_t n_servo;
  int ang;
  sscanf(message.c_str(), "%d-%d", &n_servo, &ang);

  // Validando que son los 6 servos los que se estan moviendo
  if (n_servo <= 6) {
    move_servo(n_servo, ang);
  }
}

void move_servo(uint8_t n_servo, int ang) { 
  setServo(n_servo, ang);

  if (savingPositions) {
    listPositions[indexPosition][0] = n_servo;
    listPositions[indexPosition][1] = ang;
    indexPosition += 1;
  }
}







// Moviendo posiciones de los servo
void setServo(uint8_t n_servo, int angulo) {
  int duty = map(angulo, 0, 180, pos0, pos180);
  listServos.setPWM(n_servo, 0, duty);
}

// Obteniendo la ultima posicion de movimiento de los servo
int getServo(uint8_t n_servo) {
  return listServos.getPWM(n_servo);
}
