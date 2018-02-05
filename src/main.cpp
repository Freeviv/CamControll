#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include "FS.h" // SPIFFS

const char *ssid = "ESP8266_AP";
const char *passwd = "12345678";

ESP8266WebServer server(80);

void setupWifi()
{
    WiFi.mode(WIFI_AP);
    IPAddress addr = IPAddress(192,168,1,1);
    IPAddress sub = IPAddress(255,255,255,0);
    WiFi.softAPConfig(addr,addr,sub);
    Serial.println(WiFi.softAP(ssid,passwd) ? "Ready" : "Failded!");
    IPAddress myIP = WiFi.softAPIP();
	Serial.print("AP IP address: ");
	Serial.println(myIP);
}

bool checkFilesSPIFFS()
{
    return 
        SPIFFS.exists("/index.html");
}

void handleRoot()
{
    Serial.println("Started handle root...");
    File root = SPIFFS.open("/index.html","r");
    size_t size = root.size();
    char *r = new char[size + 1];
    r[size] = '\0';
    root.readBytes(r,size);
    root.close();
    server.send(200,"text/html",String(r));
    delete[] r;
    Serial.println("Finished handle root...");
}

void handleNotFound()
{

}

void setup() {
    Serial.begin(115200);
    Serial.println();
    Serial.println("Mounting filesystem");
    SPIFFS.begin();
    Serial.print("Required files are ");
    Serial.println(checkFilesSPIFFS() ? "available." : "not available!");

    setupWifi();

    Serial.println("Started server setup...");
    server.on("/",handleRoot);
    server.onNotFound(handleNotFound);
    server.begin();
    Serial.println("Finished server setup...");
}

void loop() {
    // put your main code here, to run repeatedly:
    server.handleClient();
}
