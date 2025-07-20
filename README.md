# Proyecto: Monitoreo y Automatización con ESP32, DHT22 y ThingSpeak

Este proyecto permite medir temperatura y humedad desde un ESP32 con sensor DHT22, enviar los datos a ThingSpeak y, en su versión avanzada, controlar luces y ventiladores mediante relés.

## 🔧 Componentes

- ESP32
- Sensor DHT22
- Módulo de 3 relés
- Conexión Wi-Fi
- Cuenta en ThingSpeak

## 📁 Versiones del código

### 🟢 `basico/`
- Lee temperatura y humedad
- Envia datos a ThingSpeak cada 20 segundos

### 🔵 `avanzado/`
- Lee sensor + sincroniza hora por NTP
- Maneja 3 relés:
  - Luz de cultivo (encendida entre 17:00 y 13:00)
  - 2 ventiladores activados si temperatura > 29.6 °C o humedad > 70%
- Envía a ThingSpeak:
  - Campo 1: estado ("Hora obtenida" o "Fallo seguro")
  - Campo 2: temperatura
  - Campo 3: humedad

## 🧠 Lógica de fallo seguro

Si el ESP32 no logra obtener la hora por NTP:
- Enciende automáticamente la luz de cultivo
- Envía un mensaje de “Fallo seguro” a ThingSpeak

## 📊 Visualización

Los datos se visualizan en un canal de ThingSpeak (configurable).

## 🧑‍💻 Autor

Desarrollado por Mauro Montenegro  
[LinkedIn](https://www.linkedin.com/in/mauro-montenegro-data/)
