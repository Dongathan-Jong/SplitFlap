#include <Wire.h>
#include <WiFi.h>
#include <WebServer.h>

int payloadIndex;
int numUnits = 1;
int currentUnit;
static byte value = 1;
String characters = "abcdefghijklmnopqrstuvwxyz$&#0123456789:.-?!";
String payload = "";

const char* ssid = ;
const char* password = ;

const char htmlPage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>Hi!</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    * { box-sizing: border-box; }
    body {
      margin: 0;
      min-height: 100vh;
      display: flex;
      align-items: center;
      justify-content: center;
      background: linear-gradient(135deg, #1f2937, #111827);
      font-family: 'Segoe UI', Roboto, Helvetica, Arial, sans-serif;
    }
    .card {
      background: #ffffff;
      padding: 40px 36px;
      border-radius: 16px;
      box-shadow: 0 10px 30px rgba(0,0,0,0.3);
      width: 90%;
      max-width: 380px;
      text-align: center;
    }
    h2 {
      margin-top: 0;
      margin-bottom: 24px;
      color: #111827;
      font-weight: 600;
    }
    input[type="text"] {
      width: 100%;
      padding: 12px 14px;
      font-size: 16px;
      border: 1px solid #d1d5db;
      border-radius: 8px;
      margin-bottom: 16px;
      outline: none;
      transition: border-color 0.2s;
    }
    input[type="text"]:focus {
      border-color: #2563eb;
    }
    input[type="submit"] {
      width: 100%;
      padding: 12px 14px;
      font-size: 16px;
      font-weight: 600;
      color: white;
      background: #2563eb;
      border: none;
      border-radius: 8px;
      cursor: pointer;
      transition: background 0.2s;
    }
    input[type="submit"]:hover {
      background: #1d4ed8;
    }
  </style>
</head>
<body>
  <div class="card">
    <h2>Special Characters: "$&#:.-?!"</h2>
    <form action="/submit" method="GET">
      <input type="text" name="message" placeholder="Type something...">
      <input type="submit" value="Send">
    </form>
  </div>
</body>
</html>
)rawliteral";

WebServer server(80);

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to wifi");
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Connected");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/submit", handleSubmit);
  server.begin();
  Wire.begin();
  currentUnit = numUnits + 7;
  delay(2000);
   

}

void loop() 
{
  server.handleClient();
}

void sendByte()
{
  Wire.beginTransmission(currentUnit);
  Wire.write(payloadIndex);
  Wire.endTransmission();
}

void handleRoot()
{
  server.send(200, "text/html", htmlPage);
}

void handleSubmit()
{
  if(server.hasArg("message"))
  {
    payload = server.arg("message");
    Serial.print("Payload:");
    Serial.println(payload);
  }
  server.sendHeader("Location", "/");
  server.send(303);

  if(payload == "" || payload == " ")
  {
    payloadIndex = 0;
    sendByte();
  }
  else 
  {
    for(int i = 0; i < characters.length(); i++)
    {
      if(payload.charAt(0) == characters.charAt(i))
      {
        payloadIndex = i + 1;
        Serial.println(payloadIndex);
        sendByte();
        break;
      }
    }
  }


}
