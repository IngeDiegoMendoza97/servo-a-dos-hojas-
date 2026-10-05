#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define SS_PIN 10
#define RST_PIN 9

MFRC522 rfid(SS_PIN, RST_PIN);

Servo servo1;
Servo servo2;

// UID de tu tarjeta (cámbialo después)
byte tarjetaValida[4] = {0xDE, 0xAD, 0xBE, 0xEF};

bool puertaAbierta = false;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();

  servo1.attach(3);
  servo2.attach(5);

  cerrarPuerta();
}

void loop() {

  if (!rfid.PICC_IsNewCardPresent()) return;
  if (!rfid.PICC_ReadCardSerial()) return;

  if (compararUID(rfid.uid.uidByte)) {
    abrirPuerta();
    delay(5000);
    cerrarPuerta();
  }

  rfid.PICC_HaltA();
}

// -------- FUNCIONES --------

bool compararUID(byte *uid) {
  for (byte i = 0; i < 4; i++) {
    if (uid[i] != tarjetaValida[i]) return false;
  }
  return true;
}

void abrirPuerta() {
  servo1.write(90);
  servo2.write(90);
  puertaAbierta = true;
}

void cerrarPuerta() {
  servo1.write(0);
  servo2.write(0);
  puertaAbierta = false;
}