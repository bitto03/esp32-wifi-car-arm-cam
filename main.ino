#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>

// Motor pins
const int ENA = 4;    // D10
const int IN1 = 5;    // D9
const int IN2 = 18;   // D8
const int IN3 = 19;   // D7
const int IN4 = 23;   // D6
const int ENB = 27;   // D5

const int motorSpeed = 200;

// Servo pins
const int servoPins[] = {13, 12, 14, 16, 26, 25}; // Servo 4 is pin 26
Servo servos[6];

// Create Web Server on port 80
WebServer server(80);

// HTML + CSS + JS (AJAX)
const char* htmlPage = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>ESP32 Robot Control</title>
  <style>
    body { text-align: center; font-family: Arial; }
    button, input[type=range] { margin: 10px; padding: 10px 30px; font-size: 18px; }
    .servo { margin-bottom: 20px; }
  </style>
  <script>
    function sendCmd(cmd) {
      fetch("/" + cmd);
    }

    function setServo(id, val) {
      fetch(`/servo?num=${id}&angle=${val}`);
      document.getElementById("val" + id).innerText = val;
    }
  </script>
</head>
<body>
  <h2>ESP32 Robot & Servo Control</h2>
  <button onclick="sendCmd('forward')">FORWARD</button><br>
  <button onclick="sendCmd('left')">LEFT</button>
  <button onclick="sendCmd('right')">RIGHT</button><br>
  <button onclick="sendCmd('backward')">BACKWARD</button><br>
  <button onclick="sendCmd('stop')" style="background:red; color:white;">STOP</button><br><br>

  <h3>Servo Control</h3>
  %SERVO_CONTROLS%
</body>
</html>
)rawliteral";

// Dynamically create servo sliders
String getServoControls() {
  String html = "";
  for (int i = 0; i < 6; i++) {
    int minAngle = (i == 4) ? 0 : 0;
    int maxAngle = (i == 4) ? 20 : 180;
    int defaultAngle = (i == 4) ? 0 : 90;

    html += "<div class='servo'>Servo " + String(i) +
            ": <span id='val" + String(i) + "'>" + String(defaultAngle) + "</span><br>" +
            "<input type='range' min='" + String(minAngle) + "' max='" + String(maxAngle) +
            "' value='" + String(defaultAngle) +
            "' oninput='setServo(" + String(i) + ", this.value)'></div>";
  }
  return html;
}

// Motor Functions
void moveForward() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(ENA, motorSpeed); analogWrite(ENB, motorSpeed);
  server.send(200, "text/plain", "Forward");
}

void moveBackward() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  analogWrite(ENA, motorSpeed); analogWrite(ENB, motorSpeed);
  server.send(200, "text/plain", "Backward");
}

void turnLeft() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(ENA, motorSpeed); analogWrite(ENB, motorSpeed);
  server.send(200, "text/plain", "Left");
}

void turnRight() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  analogWrite(ENA, motorSpeed); analogWrite(ENB, motorSpeed);
  server.send(200, "text/plain", "Right");
}

void stopMotors() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
  analogWrite(ENA, 0); analogWrite(ENB, 0);
  server.send(200, "text/plain", "Stopped");
}

// Handle root with dynamic servo controls
void handleRoot() {
  String page = htmlPage;
  page.replace("%SERVO_CONTROLS%", getServoControls());
  server.send(200, "text/html", page);
}

// Handle servo control via AJAX
void handleServo() {
  if (server.hasArg("num") && server.hasArg("angle")) {
    int num = server.arg("num").toInt();
    int angle = server.arg("angle").toInt();
    if (num >= 0 && num < 6) {
      if (num == 4) {
        angle = constrain(angle, 0, 20); // Limit servo 4 to 0–20 degrees
      } else {
        angle = constrain(angle, 0, 180);
      }
      servos[num].write(angle);
      server.send(200, "text/plain", "Servo updated");
    } else {
      server.send(400, "text/plain", "Invalid servo");
    }
  } else {
    server.send(400, "text/plain", "Missing parameters");
  }
}

void setup() {
  Serial.begin(115200);

  // Motor pin setup
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT); pinMode(ENB, OUTPUT);

  // Servo setup
  for (int i = 0; i < 6; i++) {
    servos[i].attach(servoPins[i]);
    if (i == 4)
      servos[i].write(0); // Servo 4 starts at 0°
    else
      servos[i].write(90); // Others center at 90°
  }

  // WiFi Access Point
  WiFi.softAP("ROBOT_AP", "12345678");
  Serial.println("Access Point: Robot-AP");
  Serial.println("Visit: http://192.168.4.1");

  // Route setup
  server.on("/", handleRoot);
  server.on("/forward", moveBackward);
  server.on("/backward", moveForward); 
  server.on("/left", turnRight);
  server.on("/right", turnLeft);
  server.on("/stop", stopMotors);
  server.on("/servo", handleServo);
  server.begin();
}

void loop() {
  server.handleClient();
}
