#include <Arduino.h>
#include <ESP8266WiFi.h>

WiFiClient espClient;

int inputPin = D8;
const char* ssid = "XXXX";
const char* password = "XXXX";

const char* doorbell_server = "10.10.10.10";
int doorbell_port = 5050;

unsigned long ontime = 0;

void setup_wifi()
{
  delay(10);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(inputPin, INPUT);

  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  digitalWrite(LED_BUILTIN, HIGH); //turn off LED

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup(void)
{
  Serial.begin(115200);
  setup_wifi();
}

// Function that is looped forever
void loop(void)
{
  if(digitalRead(inputPin) == HIGH)
  {
    if (ontime == 0)
    {
      ontime = millis();
    }
    else if (millis() - ontime > 10)
    {
      Serial.println("DingDong");
      digitalWrite(LED_BUILTIN, LOW);

      if (espClient.connect(doorbell_server, doorbell_port))
      {
        espClient.println("GET /ring HTTP/1.1");
        espClient.println("Host: " + String(doorbell_server));
        espClient.println("Connection: close");
        espClient.println();
      }

      delay(1000);
      digitalWrite(LED_BUILTIN, HIGH);
      ontime = 0;
    }
  }
  else
  {
    ontime = 0;
  }
}

