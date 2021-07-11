#define BLYNK_PRINT Serial // Serial Port Opening
#define TINY_GSM_MODEM_SIM800 // GSM Module 
#include <TinyGPS++.h> // This library is for GPS module
// Serial Communication Libraries
#include <SoftwareSerial.h> 
#include <AltSoftSerial.h>
//Client APIs
#include <TinyGsmClient.h>
#include <BlynkSimpleTinyGSM.h>

char auth[] = "XXXXXXXXXXXXXXXXXXXXXXXXXXXX"; // Code of Server Address
char apn[]  = "airtelgprs.com"; // Application Private Network
char user[] = "";
char pass[] = "";

TinyGPSPlus gps;
AltSoftSerial Location;
SoftwareSerial Sim(3,2); //Rx,Tx
WidgetMap myMap(V3); // Virtual Pin of Server

int RED = 4;
int YELLOW = 5;
int GREEN = 6;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Started");
  Sim.begin(9600);
  pinMode(RED,OUTPUT);
  pinMode(YELLOW,OUTPUT);
  pinMode(GREEN,OUTPUT);
  Red();

  delay(3000);
  Serial.println("Sending MSG");
  delay(10000);

  SendMessage();
  Location.begin(9600);
  TinyGsm modem(Sim);
  delay(10000);

  Yellow();

  Serial.println("Intitializing Modem..");
  modem.restart();
  Serial.println("Restarted");

  Blynk.begin(auth, modem, apn, user, pass);
}

void loop() {
  // put your main code here, to run repeatedly:
  Blynk.run();

}

BLYNK_READ(V2)
{
  getGPS();
}

void getGPS()
{
  Serial.println("Listening to Location");
  delay(1000);
  Green();
  while (Location.available() > 0)
  {
    if (gps.encode(Location.read()))
    {
      if(gps.location.isValid())
      {
        Serial.println("Valid Location");
        sendData();
      }
      else
      {
        Serial.println("INVALID LOCATION");
      }
    }
  }
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  } 
}

void sendData()
{
  myMap.location(0, gps.location.lat(), gps.location.lng(), "User");
  Blynk.virtualWrite(V1, gps.location.lat());
  Blynk.virtualWrite(V2, gps.location.lng());
  Green();
  
}
void SendMessage()
{
  Serial.println("Sending 1");
  Sim.begin(9600);
  delay(1000);
  Sim.println("AT"); //Attention
  delay(500);
  Sim.println("AT+CMGF=1"); // GSM module turned into Text Mode
  delay(500);
  Sim.println("AT+CMGS=\"+XXXXXXXXXX\"\r");// SMS sending AT command for Gsm Module , XXXX= Phone number of person
  delay(500);
  Sim.print("Emergency, Check Blynk");
  delay(500);
  Sim.write(26);
  delay(5000);

  Serial.println("Sending 2");

  Sim.println("AT+CMGF=1"); // GSM module turned into Text Mode
  delay(500);
  Sim.println("AT+CMGS=\"+XXXXXXXXX\"\r");//XXXX= Phone number of person
  delay(500);
  Sim.print("Emergency, Check Blynk 2");
  delay(500);
  Sim.write(26);
  delay(5000);

  Serial.println("Sending 3");

  Sim.println("AT+CMGF=1"); // GSM module turned into Text Mode
  delay(500);
  Sim.println("AT+CMGS=\"+XXXXXXXX\"\r"); //XXXX= Phone number of person
  delay(500);
  Sim.print("Emergency, Check Blynk 3");
  delay(500);
  Sim.write(26);
  delay(5000);
}

void Red()
{
  digitalWrite(RED,HIGH);
  digitalWrite(YELLOW,LOW);
  digitalWrite(GREEN,LOW);
}

void Yellow()
{
  digitalWrite(RED,LOW);
  digitalWrite(YELLOW,HIGH);
  digitalWrite(GREEN,LOW);
}

void Green(){
  digitalWrite(RED,LOW);
  digitalWrite(YELLOW,LOW);
  digitalWrite(GREEN,HIGH);
}
