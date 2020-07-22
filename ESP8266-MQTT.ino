#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#define KEY 0

/*！！！注意：代码中print/println均是为了串口调试，可删！！！*/
/*char ssid[] = "Anuo";
char pswd[] = "123456789";*/
const char* ssid = "Anuo";
const char* password = "123456789";
const char* mqtt_server = "123.56.16.193";


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

}


void setup() {
  pinMode(BUILTIN_LED, OUTPUT);    
  pinMode(KEY, INPUT);
  digitalWrite(BUILTIN_LED, HIGH);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  
  client.setCallback(callback);
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
 /* delay(500);
  value++;
  if (value==5) {
    value/=5;
    snprintf (msg, 50, "{\"Temp\":31,\"Hum\":56,\"Light\":520}");
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("LIGHT", msg);
  }*/
}
