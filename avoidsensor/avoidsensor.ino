/*---------------------KRENOVATOR---------------------
  LIVE MOVEMENT MONITORING

  The sensor use is avoidance sensor; as for this
  project, the sensor is used to detect any object
  infront of it. The code is to read the sensor
  behavior.

  Get the code at github
  https://github.com/MZulsyahmi/live-movement-monitoring-using-MQTT.git
  
  by M.Zulsyahmi @krenovator
  August 2019
 */

//initialize for pin used
int avoidpin = D4;  //KY-032 avoidance sensor

void setup() {
  //declaration for pin
  pinMode(avoidpin, INPUT);

  Serial.begin(9600);
}

void loop() {
  int Sensor = digitalRead(avoidpin);

  //condition used: if...else condition
  //HIGH = 1 or ON      LOW = 0 or OFF
  if(Sensor == LOW){
    Serial.println("OBJECT");
  }
  else{
    Serial.println("No Object Detected");
  }
  delay(1000);
}
