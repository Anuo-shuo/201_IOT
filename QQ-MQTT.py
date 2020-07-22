import websocket,threading,time,json,paho.mqtt.client as mqtt,datetime

admin_user=('1812417248')

str_1="{\"action\": \"send_private_msg\",\"params\": {\"user_id\": 1812417248,\"message\": \"我上线啦！\"}}"

def on_connect(client, userdata, flags, rc):
    if str(rc) == "0" :
        print("连接成功")
    else:
        print("连接失败")

def on_message(client, userdata, msg):
    str_test=msg.topic + " " + str(msg.payload)
    if msg.topic == 'DOOR':
        send_to_uesr('1812417248',"门的当前状态为:"+str(msg.payload)[2:-1]+"更新时间为:"+datetime.datetime.now().strftime('%m月%d日,%R.'))
    if msg.topic == 'WENDU' :
        global wendu
        wendu=str(msg.payload)
        print("接收到的温度为："+wendu[2:-1],"更新时间："+datetime.datetime.now().strftime('%m月%d日,%R.'))
        
def ws_on_message(ws, message):
    text = json.loads(message)
    if 'message' in text :
        if str(text['user_id']) in admin_user: 
            if text['message'] in ('开灯','kd','KD','西小渣，开灯','/忙到飞起') :
                client.publish('LIGHT',payload='1',qos=0)
                send_to_uesr(str(text['user_id']),"好的，灯开了！")
                print("开灯","时间："+datetime.datetime.now().strftime('%m月%d日,%R.'))
            if text['message'] in ('关灯','gd','GD','西小渣，关灯','/搬砖中','[CQ:bface,id=47BE043DA1362E0D5747FFE2F0CB3B82,p=202939]') :
                client.publish('LIGHT',payload='0',qos=0)
                send_to_uesr(str(text['user_id']),"好的，灯关了！")
                print("关灯","时间："+datetime.datetime.now().strftime('%m月%d日,%R.'))
            if text['message'] in ('温度','wendu','wd') :
                send_to_uesr(str(text['user_id']),"温度为:"+wendu[2:-1]+"摄氏度")
                print("当前温度为："+wendu[2:-1])
            if text['message'] in ('闪起来','flash','high','西小渣，闪起来','嗨起来','蹦迪','西小渣，嗨起来') :
                client.publish('LIGHT',payload='3',qos=0)
                send_to_uesr(str(text['user_id']),"嗨起来！")
                print("闪起来","时间："+datetime.datetime.now().strftime('%m月%d日,%R.'))
            if text['message'] in ('别闪了','sflash','quiet','西小渣，别闪了','停') :
                client.publish('LIGHT',payload='4',qos=0)
                send_to_uesr(str(text['user_id']),"好吧，不玩了~")
                print("不闪了","时间："+datetime.datetime.now().strftime('%m月%d日,%R.'))
         #  if text['message'][0]=='m': 
         #       client.publish('esp8266',payload=text['message'][1:],qos=0)
#                send_to_uesr(str(text['user_id']),"你发送的数据为主题"+'esp8266'+"内容:"+text['message'][1:])  


def ws_on_error(ws, error):
    print(ws)
    print(error)

def ws_on_close(ws):
    print(ws)
    print("### closed ###")

def ws_on_open(ws):
    my_ws.send(str_1,1)


def send_to_uesr(user,message):

    my_ws.send("{\"action\": \"send_private_msg\",\"params\": {\"user_id\": "+user+",\"message\": \""+message+"\"}}")        
 

websocket.enableTrace(False)

my_ws = websocket.WebSocketApp("ws://127.0.0.1:6700")
client = mqtt.Client()


def ws_init():
    my_ws.on_message=ws_on_message
    my_ws.on_error=ws_on_error
    my_ws.on_close=ws_on_close
    my_ws.on_open=ws_on_open
    my_ws.run_forever()
  
def mqtt_init():
    time.sleep(0.2)
    client.on_connect = on_connect
    client.on_message = on_message
    client.connect('123.56.16.193', 1883, 600) 
    client.subscribe('WENDU', qos=0)
    client.subscribe('DOOR', qos=0)
    client.loop_forever() 


def main() :
    t1= threading.Thread(target=ws_init) 
    t2= threading.Thread(target=mqtt_init)
    t1.start()
    t2.start()
if __name__ == '__main__':
    main()



