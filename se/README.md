
# ESP8266 Phishing

This project demonstrates a basic implementation of the ESP8266 as a phishing device. It creates an open WiFi access point that redirects users to a login page, logs any login attempts, and provides an admin panel to review these attempts.

> **Note:** This project is created solely for educational purposes. Using this code in real networks or without permission is illegal and could violate privacy laws. Ensure compliance with all applicable regulations before testing or deploying.


## Dependencies

1. **ESP8266WiFi** — Manages the WiFi connection of the ESP8266 (included in the ESP8266 core libraries).
2. **ESP8266WebServer** — Sets up a web server on the ESP8266 to handle HTTP requests.
3. **DNSServer** — Redirects all DNS requests to the ESP8266, which allows users to be automatically redirected to the login page upon connecting.

