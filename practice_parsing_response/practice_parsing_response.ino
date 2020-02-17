#include <WString.h>
#include <Ethernet.h>

String readString = String(100); //string for fetching data from address

///////////////////////
String teststring = String(100);
String finalstring = String(100);
String flag = String(2);
int ind1 = 0;
int ind2 = 0;
int pos = 0;
//////////////////////

void setup()
{

    //start Ethernet
    Ethernet.begin(mac, ip, gateway, subnet);
    server.begin();

    //enable serial data print
    Serial.begin(9600);
}

void loop()
{
    // Create a client connection
    Client client = server.available();
    if (client)
    {
        while (client.connected())
        {
            if (client.available())
            {
                char c = client.read();

                //read char by char HTTP request
                if (readString.length() < 100)
                {

                    //store characters to string
                    readString.append(c);
                }

                //if HTTP request has ended
                if (c == '\n')
                {

                    ///////////////
                    //Serial.println(readString);
                    //readString looks like "GET /?-0p1555-1p500t1000 HTTP/1.1"

                    if (readString.contains("-"))
                    { //test for servo control sring
                        readString.replace('-', '#');
                        pos = readString.length(); //capture string length
                        //find start of servo command string (#)
                        ind1 = readString.indexOf('#');
                        //capture front part of command string
                        teststring = readString.substring(ind1, pos);
                        //locate the end of the command string
                        ind2 = teststring.indexOf(' ');
                        //capturing the servo command string from readString
                        finalstring = readString.substring(ind1, ind2 + ind1);
                        //print "finalstring" to com port;
                        Serial.println(finalstring); //print string with CR
                    }
                    ////////////////////////
                    //GET /?Slidervalue0=1800&Submit=Sub+0 HTTP/1.1
                    if (readString.contains("Slidervalue"))
                    {
                        ind1 = readString.indexOf('u');
                        ind2 = readString.indexOf('&');
                        finalstring = readString.substring(ind1 + 1, ind2);
                        finalstring.replace('e', '#');
                        finalstring.replace('=', 'p');
                        Serial.println(finalstring);
                    }
                    ///////////////////

                    //now output HTML data header
                    client.println("HTTP/1.1 204 Zoomkat");
                    client.println();
                    client.println();
                    delay(1);
                    //stopping client
                    client.stop();

                    /////////////////////
                    //clearing string for next read
                    readString = "";
                    teststring = "";
                    finalstring = "";
                }
            }
        }
    }
}