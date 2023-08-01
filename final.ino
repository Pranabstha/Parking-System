#include <Servo.h>

#include <SPI.h>
#include <RFID.h>

#define SS_PIN 10
#define RST_PIN 9
RFID rfid(SS_PIN, RST_PIN);
Servo Serv;
//Initializing pin for the ultrasonic and rfid
String rfidCard;
int pinServo = 3;
int const trigPin = 7;
int const echoPin = 2;
int const trigPin2 = 4;
int const echoPin2 = 5;

int distance = 0;
int duration;
int duration1;
int distance1;
bool itIsOpen = false;
bool itIsOpenExit = false;

void setup() {
  Serv.write(0);
  Serv.attach(pinServo);
  Serial.begin(9600);
  Serial.println("Starting the RFID Reader...");
  SPI.begin();
  rfid.init();
  //initializing output and inout pins
  pinMode(8, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
}

void loop() {
  if (rfid.isCard()) {
    if (rfid.readCardSerial()) {
      rfidCard = String(rfid.serNum[0]) + " " + String(rfid.serNum[1]) + " " + String(rfid.serNum[2]) + " " + String(rfid.serNum[3]);//reading the RFID cards serial number
    }
    rfid.halt();

    if (rfidCard == "194 58 245 27") {// checking the rfid number for the access
      Serial.print("Access granted!!!!");
      itIsOpen = true;
      while (itIsOpen == true) {
        Serv.write(90);
        //int threshold = 10;

        // Output pulse with 1ms width on trigPin
        digitalWrite(trigPin, HIGH);
        delay(10);
        digitalWrite(trigPin, LOW);


        duration = pulseIn(echoPin, HIGH);
        // Distance is half the duration devided by 29.1 (from datasheet)
        distance = (duration / 2) / 29.1;
        //Serial.println(distance);

        if (distance < 10 & distance > 0) {// checking the distace to close the gate
          Serv.write(0);
          delay(3000);
          itIsOpen = false;
        }
      }


    } else {
      Serial.print("access denied!!!!!");
      Serv.write(0);
      delay(3000);
    }
  }
  digitalWrite(trigPin, HIGH);
  delay(10);
  digitalWrite(trigPin, LOW);
  // Measure the pulse input in echo pin
  // duration = pulseIn(echoPin, HIGH);
  // distance = (duration / 2) / 29.1;


  int duration2 = pulseIn(echoPin, HIGH);
  // Distance is half the duration devided by 29.1 (from datasheet)
  int distance2 = (duration2 / 2) / 29.1;
  itIsOpenExit = false;
  Serial.println(distance2);// checking the distace to open the gate
  if (distance2< 10 & distance2 > 0) {
    itIsOpenExit = true;
    Serv.write(90);
    while (itIsOpenExit == true) {
      // Output pulse with 1ms width on trigPin
      digitalWrite(trigPin2, HIGH);
      delay(10);
      digitalWrite(trigPin2, LOW);
      // Measure the pulse input in echo pin
      duration1 = pulseIn(echoPin2, HIGH);
      // Distance is half the duration devided by 29.1 (from datasheet)
      distance1 = (duration1 / 2) / 29.1;
      if (distance1< 10 & distance1 > 0) {// checking the distace to close the gate
        Serv.write(0);

        itIsOpenExit = false;
      }
    }
  }

}