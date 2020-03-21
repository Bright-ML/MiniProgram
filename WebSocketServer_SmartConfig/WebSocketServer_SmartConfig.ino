//#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
#include <ESP8266mDNS.h>
//#include <Hash.h>

WebSocketsServer webSocket = WebSocketsServer(80);
char service[] = "websocket";
char proto[] = "tcp";
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED: //如果连接失败
      Serial.printf("[%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED: //如果连接成功
      {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        webSocket.sendTXT(num, "连接成功");  //发送：返回信息到客户端
      }
      break;
    case WStype_TEXT: //如果收到字符串
      Serial.printf("[%u] get Text: %s\n", num, payload);
      //webSocket.sendTXT(num, payload);//发送信息到客户端
      //webSocket.broadcastTXT(payload); // 发送数据到所有连接的客户端
      break;
    case WStype_BIN: //收到二进制
      Serial.printf("[%u] get binary length: %u\n", num, length);
      hexdump(payload, length);
      webSocket.sendBIN(num, payload, length); //发送信息到客户端
      break;
  }
}

void setup() {
  Serial.begin(115200);  
  smartConfig();//这里调用一键配网函数,替换上面的普通配网
  
  Serial.setDebugOutput(true);
  Serial.println();

  for (uint8_t t = 4; t > 0; t--) { //等待t秒之后
    Serial.printf("[SETUP] BOOT WAIT %d...\n", t);
    Serial.flush(); //等待串口数据发送结束
    delay(1000);
  }

  if (MDNS.begin("esp8266")) {   // void setup下编写
    Serial.println("MDNS responder started");
  }
  MDNS.addService(service,proto,80);
  
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void loop() {
  MDNS.update();   // void loop下编写
  webSocket.loop();
  while (Serial.available() > 0)
  {
    String data;
    data = Serial.readString();
    delay(5);
    webSocket.broadcastTXT(data);// 发送数据到所有连接的客户端
    Serial.println(data);
  }
}

//==========将下列代码添加到需要一键配网项目代码的最后==========
void smartConfig()
{
  WiFi.begin();
  for (int i = 0; i < 5; i++)
  {
    if (WiFi.status() == WL_CONNECTED)
    {
      Serial.println("AutoConfig Success");
      Serial.printf("SSID:%s\r\n", WiFi.SSID().c_str());
      Serial.printf("PSW:%s\r\n", WiFi.psk().c_str());
      //WiFi.printDiag(Serial); //打印关键的Wi-Fi诊断信息，信息比较多
      break;
    }
    else
    {
      Serial.print("AutoConfig Waiting......");
      Serial.println(WiFi.status());
      delay(1000);
    }
  }
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("AutoConfig Faild!" );
    WiFi.mode(WIFI_STA);
    Serial.println("\r\nWait for Smartconfig");
    WiFi.beginSmartConfig();
    while (1)
    {
      Serial.print(".");
      if (WiFi.smartConfigDone())
      {
        Serial.println("SmartConfig Success");
        Serial.printf("SSID:%s\r\n", WiFi.SSID().c_str());
        Serial.printf("PSW:%s\r\n", WiFi.psk().c_str());
        WiFi.setAutoConnect(true);  // 设置自动连接
        break;
      }
      delay(1000);
    }
  }
}
