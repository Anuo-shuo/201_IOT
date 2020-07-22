# 一、项目简介
该项目基于物联网技术，用来改造201宿舍.  
该项目中，能够使用QQ或者微信小程序控制下位机ESP8266.  
后续会逐步引入更多的设备.
# 二、项目构成
## 上位机
### 1.Python程序（QQ-MQTT.PY)
<img src="https://github.com/Anuo-shuo/201_IOT/blob/master/qq.jpg" width = "64" height = "124" alt="QQ界面" align=center />  
![Python运行结果](https://github.com/Anuo-shuo/201_IOT/blob/master/py-run.png)
### 2.微信小程序
![小程序码](https://github.com/Anuo-shuo/201_IOT/blob/master/weapp-id.png)  
![小程序界面](https://github.com/Anuo-shuo/201_IOT/blob/master/weapp.jpg)
## 下位机
基于Arduino的ESP8266(ESP8266-MQTT.INO)
## MQTT服务器
ws://fallingyou.love:8083  
wss://fallingyou.love:8084
# 三、项目联调
经测试，整个系统目前已能稳定运行.  
![小程序与服务器联调](https://github.com/Anuo-shuo/201_IOT/blob/master/liantiao-weapp.png)  
![小程序与下位机联调1](https://github.com/Anuo-shuo/201_IOT/blob/master/liantiao2.jpg)![小程序与下位机联调2](https://github.com/Anuo-shuo/201_IOT/blob/master/liantiao3.jpg)  
![QQ与下位机联调](https://github.com/Anuo-shuo/201_IOT/blob/master/liantiao-qq.jpg)  
