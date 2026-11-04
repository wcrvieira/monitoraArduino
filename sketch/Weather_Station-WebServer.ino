// Estação meteorológica com registro de temperatura, umidade do ar e 
// velocidade do vento (imitação de um anemômetro)
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include "index.h"  //Web page header file
WebServer server(80);
 
#include <DHT.h>
#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// Carrega a biblioteca NewPing do sensor ultrassônico
#include <NewPing.h>
#define trigger 19
#define echo 18
#define max_distancia 300
// Inicializa o sensor ultrassônico
NewPing sonar(trigger, echo, max_distancia);

//Enter your SSID and PASSWORD
const char* ssid = "Rede";
const char* password = "Senha";
 
void handleRoot() {
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}
 
void handleADC() {
    // send XML file containing input states
    XML_response();
}
 
void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("Inicializando Sketch...");
 
/*
//ESP32 As access point
  WiFi.mode(WIFI_AP); //Access Point mode
  WiFi.softAP(ssid, password);
*/
//ESP32 connects to your wifi 
  WiFi.mode(WIFI_STA); //Connectto your wifi
  WiFi.begin(ssid, password);
 
  Serial.println("Connecting to ");
  Serial.print(ssid);
 
  //Wait for WiFi to connect
  while(WiFi.waitForConnectResult() != WL_CONNECTED){      
      Serial.print(".");
    }
    
  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP

  server.on("/", handleRoot);      //This is display page
  server.on("/readADC", handleADC);//To get update of ADC Value only
 
  server.begin();                  //Start server
  Serial.println("HTTP server started");
  dht.begin();
}
 
void loop()
{   
  server.handleClient();
  delay(1);
}
// send the XML file containing analog value
void XML_response()
{
  // read without samples.
  byte temperature = dht.readTemperature();
  byte humidity = dht.readHumidity();
  
  // Lendo as informações do sensor ultrassônico
  unsigned int distancia = sonar.convert_cm(sonar.ping_median(5));
  
String content = "<?xml version = \"1.0\" ?>";
    content += "<inputs>";
    content += "<analog>";
    content += (int)temperature;
    content += "</analog>";
    content += "<analog>";
    content += (int)humidity;
    content += "</analog>";
    content += "<analog>";
    content += (int)distancia;
    content += "</analog>";
    content += "</inputs>"; 
     
    server.sendHeader("Cache-Control", "no-cache");
    server.send(200, "text/xml", content); //Send web page
}
