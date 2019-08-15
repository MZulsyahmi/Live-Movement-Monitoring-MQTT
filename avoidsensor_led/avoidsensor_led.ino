/*---------------------KRENOVATOR---------------------
  LIVE MOVEMENT MONITORING

  The sensor use is avoidance sensor; as for this
  project, the sensor is used to detect any object
  infront of it. The code is to read the sensor
  behavior. The LED is used as a trigger component
  for the presence or absence of any object.

  Get the code at github
  https://github.com/MZulsyahmi/live-movement-monitoring-using-MQTT.git
  
  by M.Zulsyahmi @krenovator
  August 2019
 */

//initialize for pin used
int avoidpin = D4;  //KY-032 avoidance sensor

int ledR = D8;      //red LED
int ledG = D7;      //green LED

void setup() {
  //declaration for the pin
  pinMode(avoidpin, INPUT);

  pinMode(ledR,OUTPUT);
  pinMode(ledG,OUTPUT);
  
  Serial.begin(9600);
}

void loop() {
  int Sensor = digitalRead(avoidpin);

  //condition used: if...else condition
  //HIGH = 1 or ON      LOW = 0 or OFF
  if(Sensor == LOW){
    Serial.println("OBJECT");
    digitalWrite(ledR, HIGH);
    digitalWrite(ledG, LOW);
  }
  else{
    Serial.println("No Object Detected");
    digitalWrite(ledR, LOW);
    digitalWrite(ledG, HIGH);
  }
  delay(1000);
}
