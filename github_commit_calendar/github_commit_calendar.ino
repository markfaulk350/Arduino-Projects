/*

 Board: Arduino Uno WiFi Rev2
 LED Strip: HKBAYI 4M WS2812B 5050 SMD Built-in WS2811 IC Black Board LED Strip Lights
 https://www.amazon.com/gp/product/B00JYPJAL2/ref=ppx_yo_dt_b_asin_title_o04_s00?ie=UTF8&psc=1

 ---------- Github Commits Calendar ----------

 Connects to a local wifi network and makes an HTTP request to a node.js API that web scrapes a Github profile page for commit statistics.
 Parses response body from response headers and loops through commit statistics to light up an individually addressable WS2812 led strip.
 Each led on the strip will represent a calendar day. If the GitHub user makes a commit on that calendar day, the led will light up.
 The more a user commits on that day, the brighter the led will become.

 ---------- API ENPOINTS ----------
 These API endpoints will work for any GitHub user. You just need to specify the GitHub username  /github-user-stats/insert-github-username-here

 Query Params:
 format= json or string
 weeks= can be any number of weeks between 1 and 52

 // String Response - For this arduino project
 https://github-contribution-api.herokuapp.com/github-user-stats/markfaulk350?format=string&weeks=52

 // JSON Response - For any web client
 https://github-contribution-api.herokuapp.com/github-user-stats/markfaulk350?format=json

 ---------- API ENPOINTS ----------

 The HTTP response body used for this project is in text format:
 |start|0,5,3,7,3,10,0,0,6,22

 Each int respresents the number of commits per day. The first int next to the "|start|" flag is today. It's ordered from newest to oldest.

 A "," delimiter is used to seperate the days.

 We use a |start| flag to determine where the response body starts.
 We use the total raw response length to determine where the response body ends.

 Uses the Adafruit_NeoPixel library to controll the WS2812 led strip using a single data pin on the arduino uno

 */

#include <SPI.h>
#include <WiFiNINA.h> // Needed for wifi functionality on Arduino Uno WiFi Rev2 Board
#include <Adafruit_NeoPixel.h>

#include "secrets.h" // Contains wifi network name and password

#define DATA_PIN 7
#define NUM_LEDS 21

char ssid[] = SECRET_SSID; // Wifi Network
char pass[] = SECRET_PASS; // Wifi Password

String responseString; // Where the raw http response is written to
bool hasStartedReading = false; // Used to tell if we have started reading the HTTP response from the API

int status = WL_IDLE_STATUS;

WiFiClient client; // Initialize the Wifi client library

char server[] = "github-contribution-api.herokuapp.com"; // or IPAddress server(64,131,82,241);

unsigned long lastConnectionTime = 0;              // last time you connected to the server, in milliseconds
const unsigned long postingInterval = 20L * 1000L; // delay between updates, in milliseconds

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, DATA_PIN, NEO_GRB + NEO_KHZ800);

void setup()
{
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.print("-- setup ------------------------------\n");

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE)
  {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true)
      ;
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION)
  {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED)
  {
    Serial.print("Attempting to connect to Wifi Network: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  // you're connected now, so print out the status:
  printWifiStatus();

  strip.begin();

  Serial.print("-/ setup ------------------------------\n\n");
}

void loop()
{
  while (client.available())
  {
    char c = client.read();
    responseString.concat(c); // Concatenates each response character to the raw responseString
    hasStartedReading = true;
  }

  // Request has finished and we now have raw response in responseString
  while (!client.available() && hasStartedReading == true)
  {
    // Serial.print(responseString);

    int responseLength = responseString.length();
    int start = responseString.indexOf('|start|');
    String responseBody = responseString.substring(start + 7, responseLength); // Seperate response body from response headers

    // NEED TO COUNT NUMBER OF COMMAS IN STRING TO KNOW HOW MANY DAYS WORTH OF DATA THE API SENT
    int commaCount = 0; // The amount of data will be (commaCount + 1) !!!!

    for (int i = 0; i < responseBody.length(); i++)
    {
      if (responseBody.charAt(i) == ',')
      {
        commaCount = commaCount + 1;
      }
    }

    int numOfDaysRecieved = commaCount + 1;


    // Make a copy of the response body string and make it an array of charecters.
    // Then use strtok to split the days up using the "," delimeter ex) from ["0", ",", "1", "2", ",", "5"] to [0, 12, 5]
    // Then we can easily loop through the array and light up leds
    char copy[responseBody.length() + 1];
    responseBody.toCharArray(copy, responseBody.length() + 1);

    char *strings[responseBody.length() + 1]; // Will hold the array of days when split
    int i = 0;
    char *ptr = NULL;

    ptr = strtok(copy, ",");

    while (ptr != NULL)
    {
      strings[i] = ptr;
      i++;
      ptr = strtok(NULL, ",");
    }

    strip.clear();

    for (i = 0; i < NUM_LEDS; i++)
    {
      // strip.setPixelColor(i, strip.Color(0, 200, 0));
      // strip.show();

      if(atoi(strings[i]) == 0) {
        strip.setPixelColor(i, strip.Color(0, 0, 0));
        strip.show();
      } else if (atoi(strings[i]) > 0){
        strip.setPixelColor(i, strip.Color(0, 200, 0));
        strip.show();
      }
      // Serial.println(strings[i]);
    }

    // We now need to extract the response body so that we can populate an array with values
    // We know the length of the entire response, we know that the response starts and ends with \n

    // Reset variables for next loop
    hasStartedReading = false;
    responseString = "";
  }

  // if 20 seconds have passed since your last connection, connect again.
  if (millis() - lastConnectionTime > postingInterval)
  {
    Serial.write("\n\n\n");
    httpRequest();
  }
}

void httpRequest()
{
  Serial.print("-- httpRequest ------------------------------\n");

  client.stop();

  // if there's a successful connection:
  if (client.connect(server, 80))
  {
    Serial.println("connecting...");
    // client.println("GET / HTTP/1.1");
    // client.println("Host: example.org");
    client.println("GET /github-user-stats/markfaulk350?format=string&weeks=3 HTTP/1.1");
    client.println("Host: github-contribution-api.herokuapp.com");
    client.println("User-Agent: ArduinoWiFi/1.1");
    client.println("Connection: close");
    client.println();

    // note the time that the connection was made:
    lastConnectionTime = millis();
  }
  else
  {
    // if you couldn't make a connection:
    Serial.println("connection failed");
  }
  Serial.print("-/ httpRequest ------------------------------\n");
}

void printWifiStatus()
{
  Serial.print("-- printWifiStatus ------------------------------\n");

  // print the SSID of the network you're attached to:
  Serial.print("Wifi Network: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("Arduino's IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");

  Serial.print("-/ printWifiStatus ------------------------------\n");
}