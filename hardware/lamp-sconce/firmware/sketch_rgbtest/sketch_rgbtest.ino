/***
  *  sketch_ha_sconce
  *  Daniel P Williams <dwilliams@port8080.net>
  *  2012-02-19
  *  Fireware for the HA-STS Sconce Lamp.  This code is based on the example code from:
  *    Arduino site (http://arduino.cc)
  *    Webduino library (http://code.google.com/p/webduino/)
  */
#include <SPI.h>
#include <Ethernet.h>
#include <WebServer.h>

// LED output defines and state variables
#define RED_PIN 3
#define GRN_PIN 5
#define BLU_PIN 6

int red_val = 0;
int grn_val = 0;
int blu_val = 0;

// Ethernet setup
// MAC (ethernet hardware) address:
byte mac[] = { 0xFE, 0xED, 0xDE, 0xAD, 0xBE, 0xEF };

// Web server setup
// Define base page path:
#define PREFIX ""
// Set server port to port 80
WebServer webserver(PREFIX, 80);

// Functions for replies from the web server
// Setup the default reply:
void reply(WebServer &server, WebServer::ConnectionType type, char *, bool) {
  // process the post
  if(type == WebServer::POST) {
    bool repeat;
    char name[16];
    char value[16];
    do {
      repeat = server.readPOSTparam(name, 16, value, 16);
      if(strcmp(name, "red") == 0) {
        red_val = strtoul(value, NULL, 10);
      }
      if(strcmp(name, "grn") == 0) {
        grn_val = strtoul(value, NULL, 10);
      }
      if(strcmp(name, "blu") == 0) {
        blu_val = strtoul(value, NULL, 10);
      }
    } while(repeat);
    //server.httpSuccess();
    server.httpSeeOther(PREFIX);  //reload page
    return;
  }
  
  server.httpSuccess();
  
  if(type == WebServer::GET) {
    // if not a header request, show page
//    P(reply1) = "<h1>Sconce Lamp</h1><hr /><p>Current color value (R, G, B): ";
//    P(reply2) = "</p>";
    P(reply1) = "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">\n<html xmlns=\"http://www.w3.org/1999/xhtml\">\n<head><title>HA-STS Sconce ";
    P(reply2) = "</title>\n<meta http-equiv=\"Content-type\" content=\"text/html;charset=UTF-8\" /></head>\n<body><h1>HA-STS Sconce Lamp</h1><hr /><form action=\"/\"><ul><li>MAC Address: ";
    P(reply3) = "</li><li>IP Address: ";
    P(reply4) = "</li><li>Red Level: <input type=\"text\" name=\"red\" value=\"";
    P(reply5) = "\" /></li><li>Green Level: <input type=\"text\" name=\"grn\" value=\"";
    P(reply6) = "\" /></li><li>Blue Level: <input type=\"text\" name=\"blu\" value=\"";
    P(reply7) = "\" /></li></ul><div><input type=\"submit\" value=\"Change Color\" /></div></form><div style=\"background-color:rgb(";
    P(reply8) = ");width:100px;height:100px;\">&nbsp;</div></body></html>";
    server.printP(reply1);
    //print MAC address
    server.print(mac[0], HEX);
    server.print(":");
    server.print(mac[1], HEX);
    server.print(":");
    server.print(mac[2], HEX);
    server.print(":");
    server.print(mac[3], HEX);
    server.print(":");
    server.print(mac[4], HEX);
    server.print(":");
    server.print(mac[5], HEX);
    server.printP(reply2);
    //print MAC address
    server.print(mac[0], HEX);
    server.print(":");
    server.print(mac[1], HEX);
    server.print(":");
    server.print(mac[2], HEX);
    server.print(":");
    server.print(mac[3], HEX);
    server.print(":");
    server.print(mac[4], HEX);
    server.print(":");
    server.print(mac[5], HEX);
    server.printP(reply3);
    //print IP address
    server.print(Ethernet.localIP());
    server.printP(reply4);
    server.print(red_val);
    server.printP(reply5);
    server.print(grn_val);
    server.printP(reply6);
    server.print(blu_val);
    server.printP(reply7);
    server.print(red_val);
    server.print(",");
    server.print(grn_val);
    server.print(",");
    server.print(blu_val);
    server.printP(reply8);
  }
}

void setup() {
  // set pins 3,5,6 to output for leds
  pinMode(RED_PIN, OUTPUT);
  pinMode(GRN_PIN, OUTPUT);
  pinMode(BLU_PIN, OUTPUT);
  Ethernet.begin(mac);
  
  webserver.setDefaultCommand(&reply);
  webserver.addCommand("index.html", &reply);
  webserver.begin();
}

void loop() {
  webserver.processConnection();
  
  // set brightness to value 0-255
  analogWrite(RED_PIN, red_val);
  analogWrite(GRN_PIN, grn_val);
  analogWrite(BLU_PIN, blu_val);

}
