from pubnub import Pubnub
import json
record = {}
pubnub = Pubnub(publish_key="demo", subscribe_key="demo")

def checking (requester,reqtype,deviceid,requestval):
	print (requester,reqtype,deviceid,requestval)
	val = 1
	
	if (requester == "DEVICE"):
		if(reqtype == 1):
			if(requestval == 0):
				record[deviceid] = 0
				print record
			if(requestval == 1):
				record[deviceid]=1
				print record
	
	if (requester == "APP"):
		if (reqtype == 4):
			pubnub.publish(channel='RadioStudios', message=record)
	
def callback(message, channel):
    requester = message["Requester"]
    deviceid = message["Device ID"]
    reqtype = message["Request Type"]
    requestval = message["Request Value"]
    checking(requester,reqtype,deviceid,requestval)
         
def error(message):
    print("ERROR : " + str(message))
   
def reconnect(message):
    print("RECONNECTED")
  
def disconnect(message):
    print("DISCONNECTED")

pubnub.subscribe(channels='RadioStudio', callback=callback, error=callback,
                 reconnect=reconnect, disconnect=disconnect)
