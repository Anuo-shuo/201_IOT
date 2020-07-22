#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#define KEY 0
#include <Blinker.h>

#define BLINKER_PRINT Serial
#define BLINKER_WIFI

/*！！！注意：代码中print/println均是为了串口调试，可删！！！*/
char auth[] = "2d020238571d";
/*char ssid[] = "Anuo";
char pswd[] = "123456789";*/
const char* ssid = "Anuo";
const char* password = "123456789";
const char* mqtt_server = "123.56.16.193";

BlinkerButton Button1("led");
BlinkerButton Button2("flash");

int counter = 0;

void button1_callback(const String & state) {
    BLINKER_LOG("get button state: ", state);
    digitalWrite(BUILTIN_LED, !digitalRead(BUILTIN_LED));
}
void dataRead(const String & data)
{
    BLINKER_LOG("Blinker readString: ", data);
    counter++;
    Number1.print(counter);
}


WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
//int value = 0;
int flag = 0;
int key_flag = 1;
//int brightness = 0;
//int fadeAmount = 15;

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

/*！！！注意：以上两个函数基本无需修改！！！*/
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
/*！！！注意：以上基本无需修改！！！*/


  flag=(char)payload[0];
  
 /* if ((char)payload[0] == '1') {
    flag = 1;
  }
  else if ((char)payload[0] == '0') {
    flag = 0;
  }
  else if ((char)payload[0] == '3') {
    flag = 3;
  }
  else if ((char)payload[0] == '4') {
    flag = 4;
  }*/
}


void setup() {
  pinMode(BUILTIN_LED, OUTPUT);    
  pinMode(KEY, INPUT);
  digitalWrite(BUILTIN_LED, HIGH);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  
  BLINKER_DEBUG.stream(BLINKER_PRINT);
  Blinker.begin(auth);//Blinker.begin(auth,ssid,pswd);
  Blinker.attachData(dataRead);
  Button1.attach(button1_callback);

  reconnect();
  while (!client.subscribe("LIGHT", 0)) {
    Serial.println("订阅失败;尝试重新订阅！");
    client.subscribe("LIGHT", 0);
    delay(300);
  }
  Serial.println("订阅成功~");
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.setCallback(callback);
  client.loop();

   Blinker.run(); 


  switch (flag) {
    case '1': digitalWrite(BUILTIN_LED, LOW); break;  /*注：LOW为关闭*/
    case '0': digitalWrite(BUILTIN_LED, HIGH); break;
    case '3': {
        digitalWrite(BUILTIN_LED, !digitalRead(BUILTIN_LED));
        delay(500);
        break;
      }
    case '4': digitalWrite(BUILTIN_LED, HIGH); break;
  }
  if (digitalRead(KEY) != key_flag)
  {
    Serial.println("开关状态改变");
    Serial.print("Publish message: door:");
    Serial.println(digitalRead(KEY));
    snprintf (msg, 10, "%d", digitalRead(KEY));
    client.publish("DOOR", msg);    /*注：发布格式为 client.publish("主题", msg);*/
  }
  key_flag = digitalRead(KEY);

  /*  long now = millis();
    if (now - lastMsg > 2000) {
      lastMsg = now;
      ++value;
      snprintf (msg, 50, "%d", value);
      Serial.print("Publish message: ");+
      Serial.println(msg);
      client.publish("WENDU", msg);
    }*/
}
