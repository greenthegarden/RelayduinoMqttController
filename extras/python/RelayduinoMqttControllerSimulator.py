#!/usr/bin/env python

#---------------------------------------------------------------------------------------
# Load configuration values
#
#---------------------------------------------------------------------------------------

# see https://wiki.python.org/moin/ConfigParserShootout
from configobj import ConfigObj
config = ConfigObj('RelayduinoMqttController.cfg')

print("{0}".format("Relayduino MQTT Controller"))


#---------------------------------------------------------------------------------------
# Modules and methods to support MQTT
#
#---------------------------------------------------------------------------------------

# requires paho-mqtt package to support MQTT
# see https://pypi.python.org/pypi/paho-mqtt/1.1
# for more information about package
# see http://www.hivemq.com/blog/mqtt-client-library-paho-python

import paho.mqtt.client as mqtt

# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc) :
	print("Connected with result code "+str(rc))
	# Subscribing in on_connect() means that if the connection is lost
	# the subscriptions will be renewed when reconnecting.
	print("Subscribing to topics ...")
	for topic in config['mqtt_topics']['TOPICS'] :
#	topic = config['mqtt_topics']['TOPIC']
		client.subscribe(topic)
		print("{0}".format(topic))

def on_subscribe(client, userdata, mid, granted_qos) :
    print("Subscribed: "+str(mid)+" "+str(granted_qos))

def on_message(client, userdata, message) :
    print("message received:\n{0}: {1}".format(message.topic, message.payload))

def on_publish(client, userdata, mid) :
    print("mid: {0}".format(str(mid)))

def on_disconnect(client, userdata, rc) :
	print("Disconnect returned:")
	print("client: {0}".format(str(client)))
	print("userdata: {0}".format(str(userdata)))
	print("result: {0}".format(str(rc)))

def on_log(client, userdata, level, buf) :
    print("{0}".format(buf))

client               = mqtt.Client()
client.on_connect    = on_connect
client.on_subscribe  = on_subscribe
client.on_message    = on_message
client.on_publish    = on_publish
client.on_disconnect = on_disconnect
# Uncomment to enable debug messages
#client.on_log       = on_log

try :
	mqtt_username = config['mqtt_configuration']['MQTT_BROKER_USERNAME']
	try :
		mqtt_password = config['mqtt_configuration']['MQTT_BROKER_PASSWORD']
		client.username_pw_set(mqtt_username, mqtt_password);
	except NameError:
		print("{0}",format("No mqtt password set"))
		client.username_pw_set(mqtt_username);
except NameError:
	print("{0}",format("No mqtt username set"))

client.connect(
               config['mqtt_configuration']['MQTT_BROKER_IP'],
               int(config['mqtt_configuration']['MQTT_BROKER_PORT']),
               int(config['mqtt_configuration']['MQTT_BROKER_PORT_TIMEOUT']),
               )

client.loop_start()


def tidyupAndExit() :
	running = False       #Stop thread1
	# Disconnect mqtt client			mqttc.loop_stop()
	client.disconnect()
	print("Bye")
	exit(0)

def processInput() :
	relay, duration = raw_input("Enter relay and duration (minutes) in form <relay duration>: ").split()
	try :
		int(relay)
	except ValueError :
		print("Please enter an integer for relay")
		return
	try :
		int(duration)
	except ValueError :
		print("Please enter an integer for duration")
		return
	if ( int(relay) in [1,2,3,4,5,6,7,8] ) :
		payload = relay + "," + duration
		client.publish("relayduino/control/relay", payload)
	else :
		print("Ensure relay number is correct!!")
		break

# Loop continuously
while True :
	try :
		processInput()
	except KeyboardInterrupt :      #Triggered by pressing Ctrl+C
		tidyupAndExit()
