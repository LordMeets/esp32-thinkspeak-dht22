#include <WiFi.h>            // Librería para manejar Wi-Fi
#include <DHT.h>             // Librería para el sensor DHT
#include <ThingSpeak.h>      // Librería para ThingSpeak

// Configuración del sensor DHT
#define DHTPIN 4             // Pin D4 del ESP32 para conectar el sensor DHT22
#define DHTTYPE DHT22        // Modelo del sensor
DHT dht(DHTPIN, DHTTYPE, 22); 

// Configuración Wi-Fi
const char* ssid = "Mauro";          // Reemplaza con tu SSID de Wi-Fi
const char* password = "Mortykhala";  // Reemplaza con tu contraseña de Wi-Fi

// Configuración ThingSpeak
unsigned long myChannelNumber = 2680221;     // Reemplázalo con tu número de canal ThingSpeak
const char* myWriteAPIKey = "EVNK4TV6YFM2CYWH";  // Tu clave API de escritura ThingSpeak

WiFiClient client;  // Cliente WiFi para conectar a ThingSpeak

void setup() {
  Serial.begin(115200);   // Inicia la comunicación serial
  dht.begin();            // Inicia el sensor DHT22
  
  // Conectar a Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Conectando a Wi-Fi");
  
  // Espera hasta que se conecte
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  
  Serial.println();
  Serial.println("Conectado a la red Wi-Fi.");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());

  // Iniciar la conexión a ThingSpeak
  ThingSpeak.begin(client);
}

void loop() {
  // Leer la temperatura y humedad del DHT22
  float h = dht.readHumidity(); 
  float t = dht.readTemperature(); 

  // Imprimir en el monitor serial
  Serial.println("Humedad: "); 
  Serial.println(h);
  Serial.println("Temperatura: ");
  Serial.println(t);

  // Enviar los datos a ThingSpeak
  ThingSpeak.setField(1, t);  // Campo 1 = Temperatura
  ThingSpeak.setField(2, h);  // Campo 2 = Humedad
  
  // Enviar los datos al canal
  int responseCode = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  
  if (responseCode == 200) {
    Serial.println("Datos enviados correctamente a ThingSpeak");
  } else {
    Serial.print("Error al enviar los datos a ThingSpeak: ");
    Serial.println(responseCode);
  }

  delay(20000);  // Espera 20 segundos entre actualizaciones (limitación de ThingSpeak)
}
