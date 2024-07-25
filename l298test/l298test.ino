#include <WiFi.h>
#include <WebServer.h>

// AP credentials
const char* ap_ssid = "INADAZE BBBB Turret";

// Motor B (adjusted for your setup)
int motor2Pin1 = 25;
int motor2Pin2 = 26;
int enable2Pin = 27;

// PWM properties
int dutyCycle = 255; // Max duty cycle for full power

// Web server running on port 80
WebServer server(80);

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Initialize the motor pins as outputs
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  pinMode(enable2Pin, OUTPUT);

  // Setup Access Point
  WiFi.softAP(ap_ssid);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // Define web server routes
  server.on("/", handleRoot);
  server.on("/arm", handleArm);
  server.on("/stop", handleStop);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  // Handle client requests
  server.handleClient();
}

void handleRoot() {
  String html = "<!DOCTYPE html><html lang='en'><head>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<style>";
  html += "body { font-family: Arial, sans-serif; display: flex; justify-content: center; align-items: center; height: 100vh; background-color: #e0e0e0; margin: 0; }";
  html += ".container { text-align: center; }";
  html += "h1 { font-size: 2rem; color: #333; margin-bottom: 20px; }";
  html += ".status { font-size: 1.5rem; color: #28a745; margin-bottom: 20px; }";
  html += "button { width: 100%; max-width: 300px; height: 60px; font-size: 1.5rem; border: none; border-radius: 30px; background: #e0e0e0; box-shadow: 20px 20px 60px #bebebe, -20px -20px 60px #ffffff; cursor: pointer; transition: all 0.3s ease; margin-bottom: 10px; }";
  html += "button:active { box-shadow: inset 20px 20px 60px #bebebe, inset -20px -20px 60px #ffffff; }";
  html += ".button-group { display: flex; flex-direction: column; align-items: center; }";
  html += ".button-row { display: flex; justify-content: center; gap: 10px; }";
  html += "</style>";
  html += "</head><body>";
  html += "<div class='container'>";
  html += "<h1>INADAZE BBBB Turret</h1>";
  html += "<div class='status' id='status'>System Active</div>";
  html += "<div class='button-group'>";
  html += "<button onclick='armSystem()'>ARM SYSTEM</button>";
  html += "<button onclick='stopSystem()'>STOP!</button>";
  html += "<div class='button-row'>";
  html += "<button disabled>UP</button>";
  html += "</div>";
  html += "<div class='button-row'>";
  html += "<button disabled>LEFT</button>";
  html += "<button disabled>RIGHT</button>";
  html += "</div>";
  html += "<div class='button-row'>";
  html += "<button disabled>DOWN</button>";
  html += "</div>";
  html += "</div>";
  html += "</div>";
  html += "<script>";
  html += "function armSystem() {";
  html += "  fetch('/arm').then(response => {";
  html += "    if (response.ok) {";
  html += "      document.getElementById('status').textContent = 'Turret Armed';";
  html += "      alert('System Armed');";
  html += "    } else {";
  html += "      alert('Failed to arm system');";
  html += "    }";
  html += "  });";
  html += "}";
  html += "function stopSystem() {";
  html += "  fetch('/stop').then(response => {";
  html += "    if (response.ok) {";
  html += "      document.getElementById('status').textContent = 'System Active';";
  html += "      alert('System Stopped');";
  html += "    } else {";
  html += "      alert('Failed to stop system');";
  html += "    }";
  html += "  });";
  html += "}";
  html += "</script>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void handleArm() {
  Serial.println("System Armed");
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
  analogWrite(enable2Pin, dutyCycle);
  server.send(200, "text/plain", "OK");
}

void handleStop() {
  Serial.println("System Stopped");
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);
  analogWrite(enable2Pin, 0);
  server.send(200, "text/plain", "OK");
}
