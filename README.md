# ESP32 + DHT22 + ThingSpeak

Este proyecto muestra cómo enviar datos de temperatura y humedad desde un ESP32 utilizando el sensor DHT22 hacia una cuenta de [ThingSpeak](https://thingspeak.com/), para visualización en la nube.

## 🔧 Componentes

- ESP32
- Sensor DHT22
- Conexión Wi-Fi
- Cuenta en ThingSpeak

## ⚙️ Funcionamiento

1. El ESP32 se conecta a la red Wi-Fi configurada.
2. Cada 20 segundos:
   - Lee la temperatura y humedad desde el sensor DHT22.
   - Envía los datos a ThingSpeak (campos 1 y 2).

## 📦 Código

El archivo `sensor_thingspeak.ino` contiene:

- Configuración de Wi-Fi
- Lectura del sensor
- Envío de datos a ThingSpeak
- Salida por monitor serial para depuración

## 📊 Ejemplo de canal ThingSpeak

> 🔗 [Link al canal público (si querés compartirlo)](https://thingspeak.com/channels/XXXXXX)

## 🧑‍💻 Autor

Desarrollado por Mauro Montenegro  
[LinkedIn](https://www.linkedin.com/in/mauro-montenegro-data/)
