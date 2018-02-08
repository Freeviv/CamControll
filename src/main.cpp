#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include "FS.h" // SPIFFS

#include "crc.h"


#define FOCUS D5
#define TRIGGER D6

const char *ssid = "ESP8266_AP";
const char *passwd = "12345678";

ESP8266WebServer server(80);

struct timelapse_config_t
{
    uint32_t num_img;
    uint32_t processed;
    uint32_t ttp; // time to picture
    crc checksum;
} __attribute__((packed));


timelapse_config_t tlc;
bool config_existing;
bool ts_started = false;
unsigned long last_img;

void calculate_config_crc()
{
    tlc.checksum = crcSlow((uint8_t*)&tlc,sizeof(timelapse_config_t) - sizeof(crc));
}

bool validate_config_crc()
{
    return tlc.checksum == crcSlow((uint8_t*)&tlc,sizeof(timelapse_config_t) - sizeof(crc));
}
void trigger_photo()
{
    digitalWrite(FOCUS,HIGH);
    delay(200);
    digitalWrite(TRIGGER,HIGH);
    delay(200);
    digitalWrite(FOCUS,LOW);
    digitalWrite(TRIGGER,LOW);
}

void store_config()
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
    // fps=60&length=300&speedup=30&ttype=finished
    if(server.arg("fps")== ""     ||
       server.arg("length")== ""  ||
       server.arg("speedup")== "" ||
       server.arg("ttype") == "")
    {
        server.sendHeader("Location", String("/"), true);
        server.send ( 302, "text/plain", "");
        return;
    }
    int fps = atoi(server.arg("fps").c_str());
    int length = atoi(server.arg("length").c_str());
    if(server.arg("ttype").compareTo("finished") == 0)
    {
        Serial.println("Type: finished");
        tlc.num_img = fps * length;
        tlc.ttp = length * fps;
    }
    else
    {
        Serial.println("Type: raw");
        tlc.ttp = fps;
        tlc.num_img = length;
    }
    Serial.print("Config:\nNumber of images: ");
    Serial.println(tlc.num_img);
    Serial.print("Take every ");
    Serial.print(tlc.ttp);
    Serial.println(" seconds an image");
    tlc.processed = 0;
    calculate_config_crc();
    store_config();
    server.sendHeader("Location", String("/started.html"), true);
    server.send(303);
    ts_started = true;
}

void handleNotFound()
{
    server.sendHeader("Location",String("/fnf.html"),true);
    server.send(302);
}

void setup() {
    Serial.begin(115200);
    Serial.println();
    Serial.println("Startup");

    Serial.print("Setting up pin mode");
    pinMode(FOCUS,OUTPUT);
    Serial.print(".");
    pinMode(TRIGGER,OUTPUT);
    Serial.print(".");
    delay(200);
    digitalWrite(FOCUS,LOW);
    Serial.print(".");
    digitalWrite(TRIGGER,HIGH);
    Serial.println(" finished!");

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

    if(!config_existing)
    {
        Serial.println("Mounting filesystem");
        SPIFFS.begin();
        Serial.print("Required files are ");
        if(checkFilesSPIFFS())
        {
            Serial.println("available.");
        }
        else
        {
            Serial.println("not available.");
            ESP.deepSleep(UINT32_MAX);
        }


        setupWifi();

        Serial.println("Started server setup...");
        // register all possible requests
        server.serveStatic("/",SPIFFS,"/index.html");
        server.serveStatic("/style.css",SPIFFS,"/style.css");
        server.serveStatic("/fnf.html",SPIFFS,"/fnf.html");
        server.serveStatic("/started.html",SPIFFS,"/started.html");
        server.serveStatic("/script_collection.js",SPIFFS,"/script_collection.js");
        server.on("/index.html",handleIndex);
        server.onNotFound(handleNotFound);
        server.begin();
        Serial.println("Finished server setup...");
    }
    else
    {
        Serial.println("Timelapse mode...");
    }
}

void loop() {
    if(!config_existing)
    {
        server.handleClient();
    }
    if(ts_started && last_img == 0 || millis() - last_img >= tlc.ttp * 1000 * 1000)
    {
        last_img = millis();
        Serial.printf("Taking photo...");
        unsigned long t = millis();
        trigger_photo();
        tlc.processed++;
        calculate_config_crc();
        store_config();
    }
}
