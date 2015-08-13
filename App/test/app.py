from pubnub import Pubnub

pubnub = Pubnub(publish_key="demo", subscribe_key="demo")

def callback(message, channel):
    print(message)
  
  
def error(message):
    print("ERROR : " + str(message))
  
  
def connect(message):
    print("CONNECTED")
    print pubnub.publish(channel='RadioStudio', message={"Requester":"APP","Device ID":0,"Request Type":4,"Request Value":0})
  
def reconnect(message):
    print("RECONNECTED")
  
  
def disconnect(message):
    print("DISCONNECTED")

pubnub.subscribe(channels='RadioStudios', callback=callback, error=callback,
                 connect=connect, reconnect=reconnect, disconnect=disconnect)
