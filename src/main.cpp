#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include "FS.h" // SPIFFS

#include "crc.h"

const char *ssid = "ESP8266_AP";
const char *passwd = "12345678";

ESP8266WebServer server(80);

enum FREQ_MOD_OP
{
    MULTIPLY = 0,
    DIVIDE
};

struct timelapse_config_t
{
    uint32 num_img;
    double freq;
    uint32 operant;
    FREQ_MOD_OP op : 2;
    bool startServer;
    bool deepSleep;
    crc checksum;
} __attribute__((packed));



timelapse_config_t tlc;
bool config_existing;



void calculate_config_crc()
{
    tlc.checksum = crcSlow((uint8_t*)&tlc,sizeof(timelapse_config_t) - sizeof(crc));
}

bool validate_config_crc()
{
    return tlc.checksum == crcSlow((uint8_t*)&tlc,sizeof(timelapse_config_t) - sizeof(crc));
}

void start_timelapse()
{
    ESP.rtcUserMemoryWrite(0,(uint32_t*)&tlc,sizeof(timelapse_config_t));
}

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

void handleIndex()
{
    // n_img=2000&freq=1&freq_mod=%2F100
    if(server.arg("n_img")== ""      ||
       server.arg("freq")== ""       ||
       server.arg("freq_mod")== ""   ||
       server.arg("deepSleep") == "" ||
       server.arg("webserver") == "")
    {
        server.sendHeader("Location", String("/"), true);
        server.send ( 302, "text/plain", "");
    }
    int n_img = atoi(server.arg("n_img").c_str());
    double frequency = atof(server.arg("freq").c_str());
    int operant = atoi(server.arg("freq_mod").substring(1).c_str());
}

void handleNotFound()
{

}

void setup() {
    Serial.begin(115200);
    Serial.println();

    // try reading existing configuration
    ESP.rtcUserMemoryRead(0,(uint32_t*)&tlc,sizeof(timelapse_config_t));
    config_existing = validate_config_crc();
    if(config_existing)
    {
        Serial.println("Found existing configuration. Reusing it...");
    } else 
    {
        Serial.println("No configuration found!");
    }

    if(!config_existing || tlc.startServer)
    {
        Serial.println("Mounting filesystem");
        SPIFFS.begin();
        Serial.print("Required files are ");
        Serial.println(checkFilesSPIFFS() ? "available." : "not available!");

        setupWifi();

        Serial.println("Started server setup...");
        // register all possible requests
        server.serveStatic("/",SPIFFS,"/index.html");
        server.serveStatic("/style.css",SPIFFS,"/style.css");
        server.on("/index.html",handleIndex);
        server.onNotFound(handleNotFound);
        server.begin();
        Serial.println("Finished server setup...");
    }
}

void loop() {
    if(!config_existing || tlc.startServer)
    {
        server.handleClient();
    }
}
