#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqtt_server = "broker.hivemq.com";

WiFiClient espClient;
PubSubClient client(espClient);

int passes = 0;
int gols = 0;

void setup_wifi() {
  Serial.print("Conectando-se ao WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Conectado!");
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Tentando conectar ao MQTT...");
    if (client.connect("WokwiJogadora1")) {
      Serial.println("Conectado!");
    } else {
      Serial.print("Falhou, rc=");
      Serial.print(client.state());
      Serial.println(" tentando novamente em 5 segundos");
      delay(5000);
    }
  }
}

void envia_dados() {
  float velocidade = random(40, 250) / 10.0;    // 4.0 a 25.0 km/h
  int bpm = random(80, 180);
  float temp = random(360, 385) / 10.0;

  passes++;
  if (random(0, 10) == 0) gols++;

  String payload = "{\"velocidade\":" + String(velocidade, 1) +
                   ",\"passes\":" + String(passes) +
                   ",\"gols\":" + String(gols) +
                   ",\"bpm\":" + String(bpm) +
                   ",\"temp\":" + String(temp, 1) + "}";

  Serial.println("Enviando dados: " + payload);
  client.publish("womanunited/jogadora1", payload.c_str());
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  randomSeed(analogRead(0));
}

void loop() {
  if (!client.connected()) reconnect();
  client.loop();
  envia_dados();
  delay(5000);
}