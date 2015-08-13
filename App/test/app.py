from pubnub import Pubnub

pubnub = Pubnub(publish_key="pub-c-a1f796fb-1508-4c7e-9a28-9645035eee90", subscribe_key="sub-c-d4dd77a4-1e13-11e5-9dcf-0619f8945a4f")

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
