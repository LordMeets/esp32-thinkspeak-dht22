#include <WiFi.h>
#include <DHT.h>
#include <ThingSpeak.h>
#include <time.h>

// Configuración del sensor DHT
#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE, 22);

// Configuración Wi-Fi
const char* ssid = "Mauro";
const char* password = "Mortykhala";

// Pines GPIO del ESP32 conectados a los relés
int releK4 = 22;  // Luz LED de cultivo
int releK3 = 23;  // Ventilador 1
int releK2 = 21;  // Ventilador 2

// Umbrales para temperatura y humedad
float umbralTemp = 29.6;
float umbralHum = 70.0;

// Configuración para controlar el tiempo (para la luz LED)
const long gmtOffset_sec = -10800;    // GMT-3 para Argentina
const int daylightOffset_sec = 3600;  // Ajuste horario de verano

// Hora para encender y apagar la luz de cultivo
int horaEncender = 4;   // Encender a las 4:00 AM
int horaApagar = 0;     // Apagar a las 0:00 AM (medianoche)

// Variables para control de tiempo y estado
bool horaObtenida = false;
bool falloSeguro = true;
bool mensajeEnviado = false;

// ThingSpeak
unsigned long myChannelNumber = 2680221;
const char * myWriteAPIKey = "EVNK4TV6YFM2CYWH";
WiFiClient client;

// Variables para ventiladores
unsigned long tiempoVentiladoresEncendidos = 0;
unsigned long duracionEncendidoVentiladores = 25 * 60 * 1000;  // 25 minutos

void setup() {
  Serial.begin(115200);
  dht.begin();

  pinMode(releK4, OUTPUT);
  pinMode(releK3, OUTPUT);
  pinMode(releK2, OUTPUT);

  digitalWrite(releK4, HIGH);
  digitalWrite(releK3, HIGH);
  digitalWrite(releK2, HIGH);

  ThingSpeak.begin(client);

  conectarWiFi();
  obtenerHora();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    conectarWiFi();
  }

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  String estado;

  if (!isnan(h) && !isnan(t)) {
    controlarVentiladores(t, h);
    estado = horaObtenida ? "Hora obtenida" : "Fallo seguro";

    // Enviar datos a ThingSpeak
    ThingSpeak.setField(1, t);
    ThingSpeak.setField(2, h);
    ThingSpeak.setField(3, estado);

    int httpCode = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    if (httpCode == 200) {
      Serial.println("Datos enviados a ThingSpeak correctamente.");
    } else {
      Serial.println("Error al enviar datos a ThingSpeak.");
    }
  } else {
    Serial.println("Lectura inválida del sensor DHT.");
  }

  controlarLuzCultivo();

  delay(20000);  // Espera de 20 segundos
}

void conectarWiFi() {
  if (WiFi.status() == WL_CONNECTED) return;

  WiFi.begin(ssid, password);
  Serial.print("Conectando a Wi-Fi");

  int tiempoEspera = 0;
  while (WiFi.status() != WL_CONNECTED && tiempoEspera < 5) {
    delay(1000);
    Serial.print(".");
    tiempoEspera++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConectado a la red Wi-Fi.");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
    obtenerHora();
  } else {
    Serial.println("\nNo se pudo conectar a Wi-Fi.");
  }
}

void obtenerHora() {
  configTime(gmtOffset_sec, daylightOffset_sec, "pool.ntp.org", "time.nist.gov");

  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("No se pudo obtener la hora.");
    horaObtenida = false;
    falloSeguro = true;
    return;
  }

  Serial.println("Hora obtenida correctamente.");
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");

  horaObtenida = true;
  falloSeguro = false;
}

void controlarVentiladores(float temp, float hum) {
  if (temp > umbralTemp || hum > umbralHum) {
    digitalWrite(releK2, LOW);
    digitalWrite(releK3, LOW);
    Serial.println("Ventiladores encendidos.");
    tiempoVentiladoresEncendidos = millis();
  }

  else if (millis() - tiempoVentiladoresEncendidos > duracionEncendidoVentiladores) {
    digitalWrite(releK2, HIGH);
    digitalWrite(releK3, HIGH);
    Serial.println("Ventiladores apagados tras 25 min.");
  }
}

void controlarLuzCultivo() {
  struct tm timeinfo;

  if (!horaObtenida) {
    Serial.println("Fallo seguro: luz encendida.");
    digitalWrite(releK4, LOW);
    return;
  }

  if (getLocalTime(&timeinfo)) {
    int currentHour = timeinfo.tm_hour;
    if (currentHour >= horaEncender || currentHour < horaApagar) {
      digitalWrite(releK4, LOW);
      Serial.println("Luz LED encendida según horario.");
    } else {
      digitalWrite(releK4, HIGH);
      Serial.println("Luz LED apagada según horario.");
    }
  } else {
    Serial.println("Error accediendo a hora local.");
  }
}
