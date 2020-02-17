// client.read() can only read one char at a time. We need to take the char and store it in either a string or char array.
// We need to determine when we have read the entire response from the server.
// Then once we know we have the entire response. Seperate the response headers from response data.

/*
  Repeating Wifi Web Client

 This sketch connects to a web server and makes a request
 using a WiFi equipped Arduino Uno Rev2.

 */

#include <SPI.h>
#include <WiFiNINA.h>

#include "secrets.h"

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;
bool hasStartedReading = false;

String responseString;

int status = WL_IDLE_STATUS;

// Initialize the Wifi client library
WiFiClient client;

char server[] = "github-contribution-api.herokuapp.com";
//IPAddress server(64,131,82,241);

unsigned long lastConnectionTime = 0;              // last time you connected to the server, in milliseconds
const unsigned long postingInterval = 20L * 1000L; // delay between updates, in milliseconds
// const unsigned long postingInterval = 10L * 1000L; // delay between updates, in milliseconds

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

  Serial.print("-/ setup ------------------------------\n\n");
}

void loop()
{

  // THE IDEA IS TO MAKE AN API CALL FOR NEW GITHUB DATA EVERY 5 MIN.
  // THE API CALL WILL TRIGGER A NODE.JS WEBSCRAPER THAT WILL GO THROUGH GITHUB AND GRAB COMMIT HISTORY
  // THE API CAN EITHER RETURN JSON OR A STRING DEPENDING UPON QUERY PARAM "format" = json or string
  // WE CAN SELECT THE NUMER OF WEEKS OF DATA WE WANT USING THE QUERY PARAM "weeks" = 52
  // WE WANT TO GET THE STRING FROM THE API PARSE IT AND SAVE THE DATA TO A LIST
  // WE THEN WANT TO ITERATE THROUGH THE LIST AND LIGHT UP LEDS
  // COLORS WILL VARY DEPENDING UPON THE NUMBER OF GIT COMMITS PER DAY

  // if there's incoming data from the net connection.
  // send it out the serial port.  This is for debugging
  // purposes only:

  while (client.available())
  {
    char c = client.read();
    responseString.concat(c);
    // Serial.write(c);
    // Serial.print(responseString.length());

    hasStartedReading = true;
  }

  // Request has finished and we now have response in responseString
  while (!client.available() && hasStartedReading == true)
  {
    // Serial.print(responseString);

    int responseLength = responseString.length();
    int start = responseString.indexOf('|start|');
    String responseBody = responseString.substring(start + 7, responseLength);

    // NEED TO COUNT NUMBER OF COMMAS IN STRING TO KNOW HOW MANY WE CAN PRINT / LEDS TO LIGHT
    int commaCount = 0; // The amount of data will be (commaCount + 1) !!!!

    for (int i = 0; i < responseBody.length(); i++)
    {
      if (responseBody.charAt(i) == ',')
      {
        commaCount = commaCount + 1;
        // Serial.println("Comma has been found!");
      }
    }

    // Serial.println('commaCount:');
    // Serial.println(commaCount);

    // Serial.print(responseBody);

    // We need to know how many slots we need for the data
    // We can calculate this by counting the number of commas in the response body + 1 because the last number does not have a comma

    char copy[responseBody.length() + 1];
    responseBody.toCharArray(copy, responseBody.length() + 1);

    char *strings[responseBody.length() + 1];
    int i = 0;
    char *ptr = NULL;

    ptr = strtok(copy, ",");

    while (ptr != NULL)
    {
      strings[i] = ptr;
      i++;
      ptr = strtok(NULL, ",");
    }

    // strings[i] = strtok(copy, ",");

    // while (strings[i] != NULL)
    // {
    //   strings[i++] = strtok(NULL, ",");
    // }

    for (i = 0; i < commaCount + 1; i++)
    {
      Serial.println(strings[i]);
    }

    // Serial.print(copy);

    // We now need to extract the response body so that we can populate an array with values
    // We know the length of the entire response, we know that the response starts and ends with \n

    hasStartedReading = false;
    responseString = "";
  }

  // if ten seconds have passed since your last connection,
  // then connect again and send data:
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
    client.println("GET /github-user-stats/fabpot?format=string&weeks=2 HTTP/1.1");
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
