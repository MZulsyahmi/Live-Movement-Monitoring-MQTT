/*---------------------KRENOVATOR---------------------
  LIVE MOVEMENT ALERT

  The sensor use is avoidance sensor; as for this
  project, the sensor is used to detect any object
  infront of it. The code is to read the sensor
  behavior. The LED is used as a trigger component
  for the presence or absence of any object and buzzer
  will produce sound when a object is near. OLED
  display is to monitor the result on display live.

  Get the code at github
  https://github.com/MZulsyahmi/live-movement-monitoring-using-MQTT.git
  
  by M.Zulsyahmi @krenovator
  August 2019
 */
 
/*the library for OLED display*/
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET LED_BUILTIN
Adafruit_SSD1306 display(OLED_RESET);

//initialize for pin used
int avoidpin = D4;  //KY-032 avoidance sensor

int ledR = D8;      //red LED
int ledG = D7;      //green LED

int buzzer = D3;    //buzzer
int i;              //the initialize for the function to be read in Adafruit

#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#if (SSD1306_LCDHEIGHT != 64)   //to declare the pixel of OLED
#error("Height incorrect, please fix Adafruit_SSD1306.h!");   //if got an error, the codes need to be changed
#endif

/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "YourNetworkName"
#define WLAN_PASS       "NetworkPassword"

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "YourUsername"
#define AIO_KEY         "YourAIOkey"

/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// or... use WiFiFlientSecure for SSL
//WiFiClientSecure client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/

// Setup a feed called 'photocell' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Publish Movement = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/YourFeedsName");

// Setup a feed called 'onoff' for subscribing to changes.
//Adafruit_MQTT_Subscribe onoffbutton = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/onoff");

/*************************** Sketch Code ************************************/

// Bug workaround for Arduino 1.6.6, it seems to need a function declaration
// for some reason (only affects ESP8266, likely an arduino-builder bug).
void MQTT_connect();

void setup() {
  display.clearDisplay();
  
  Serial.begin(115200);
  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  
  //declaration for the pin
  pinMode(avoidpin, INPUT);

  pinMode(ledR,OUTPUT);
  pinMode(ledG,OUTPUT);
  pinMode(buzzer,OUTPUT);

  // Clear the buffer/display
  display.clearDisplay();

  Serial.println(F("Adafruit MQTT demo"));

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());

  // Setup MQTT subscription for onoff feed.
  //mqtt.subscribe(&onoffbutton);
}

uint32_t x=0;

void loop() {
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();

  // this is our 'wait for incoming subscription packets' busy subloop
  // try to spend your time here

  // Now we can publish stuff!
  Serial.print(F("\nSending photocell val "));
  Serial.print(x);
  Serial.print("...");
  
/************************ The controlled coding ********************************/

  int Sensor = digitalRead(avoidpin);

  //condition used: if...else condition
  //HIGH = 1 or ON      LOW = 0 or OFF
  if(Sensor == LOW){
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.setTextSize(2);
    display.print("OBJECT");
    i  = 1;                       //call 1 if there is movement
    
    digitalWrite(ledR, HIGH);
    digitalWrite(ledG, LOW);
    digitalWrite(buzzer,HIGH);
  }
  else{
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.setTextSize(3);
    display.print("No Object");
    i  = 0;                       //call 0 if there is no movement
    
    digitalWrite(ledR, LOW);
    digitalWrite(ledG, HIGH);
    digitalWrite(buzzer,LOW);
  }

/************************ Condition to publish ********************************/
  
  if (!Movement.publish(i)) {
    Serial.println(F("Failed"));
  }
  else {
    Serial.println(F("OK!"));
  }

  delay(2000);
  display.clearDisplay();

  // ping the server to keep the mqtt connection alive
  // NOT required if you are publishing once every KEEPALIVE seconds
  /*
  if(! mqtt.ping()) {
    mqtt.disconnect();
  }
  */
}

void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}
