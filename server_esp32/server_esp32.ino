#include <WiFi.h>
#include <WebServer.h>

// Definir las credenciales de Wi-Fi
const char* ssid = "PRULAND";
const char* password = "Huminta24";

// Inicializar el servidor en el puerto 80
WebServer server(80);

// Pin donde está conectado el LED
const int ledPin = 2;
bool ledEncendido = false; // Estado inicial del LED

// Agregar soporte CORS
void handleCors() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "GET, PUT, OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
  server.send(204);
}

// Función para manejar la solicitud PUT
void handlePut() {
  if (server.hasArg("plain")) {
    String body = server.arg("plain");
    if (body == "on") {
      digitalWrite(ledPin, HIGH);
      ledEncendido = true;
      server.sendHeader("Access-Control-Allow-Origin", "*");  // Agregar el header CORS
      server.send(200, "text/plain", "LED encendido");
    } else if (body == "off") {
      digitalWrite(ledPin, LOW);
      ledEncendido = false;
      server.sendHeader("Access-Control-Allow-Origin", "*");  // Agregar el header CORS
      server.send(200, "text/plain", "LED apagado");
    } else {
      server.sendHeader("Access-Control-Allow-Origin", "*");  // Agregar el header CORS
      server.send(400, "text/plain", "Solicitud inválida");
    }
  } else {
    server.sendHeader("Access-Control-Allow-Origin", "*");  // Agregar el header CORS
    server.send(400, "text/plain", "No se recibió cuerpo en la solicitud");
  }
}

// Función para manejar la solicitud GET
void handleGet() {
  server.sendHeader("Access-Control-Allow-Origin", "*");  // Agregar el header CORS
  if (ledEncendido) {
    server.send(200, "text/plain", "on");
  } else {
    server.send(200, "text/plain", "off");
  }
}

void setup() {
  pinMode(ledPin, OUTPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a WiFi...");
  }
  Serial.println("Conectado a WiFi");
  Serial.println(WiFi.localIP());

  // Definir la ruta para manejar PUT, GET y OPTIONS
  server.on("/led", HTTP_PUT, handlePut);
  server.on("/led", HTTP_GET, handleGet);

  // Manejar preflight requests (OPTIONS) para CORS
  server.on("/led", HTTP_OPTIONS, handleCors);

  server.begin();
  Serial.println("Servidor iniciado");
}

void loop() {
  server.handleClient();
}
