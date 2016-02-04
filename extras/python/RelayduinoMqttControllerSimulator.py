#!/usr/bin/env python

#---------------------------------------------------------------------------------------
# Load configuration values
#
#---------------------------------------------------------------------------------------

# see https://wiki.python.org/moin/ConfigParserShootout
from configobj import ConfigObj
config = ConfigObj('RelayduinoMqttController.cfg')

print("{0}".format("Relayduino MQTT Controller Simulator"))


#---------------------------------------------------------------------------------------
# Modules and methods to support MQTT
#
#---------------------------------------------------------------------------------------

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
#    print(time.clock())
	print(message.topic+" "+str(message.payload))
	if (message.topic == "relayduino/control/master") :
		if (message.payload == "0") :
			switchRelayOff()	# switch all relays off
	if (message.topic == "relayduino/control/relay_1") :
		if (message.payload == "1") :
			switchRelayOn()
		elif (message.payload == "0") :
			switchRelayOff()
	else :
		print("unexpected message received!!")
#    print("message received: topic is {0} with payload {1}".format(message.topic, message.payload))

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


relayOn = False

def switchRelay() :
	global relayOn
	if (relayOn) :
		relayOn = False
		client.publish("relayduino/status/relay_1", "0")
		print("{0}".format("Switched relay off"))
	else :
		relayOn = True
		client.publish("relayduino/status/relay_1", "1")
		print("{0}".format("Switched relay on"))

def switchRelayOn() :
	global relayOn
	if (relayOn) :
		return
	else :
		switchRelay()

def switchRelayOff() :
	global relayOn
	if (not relayOn) :
		return
	else :
		switchRelay()

command_types_valid_inputs = ['d']
#led_control_valid_inputs   = ['0', '1']

print("Enter 'd' to set duration: ")

# Loop continuously
while True :
	try :
		command_type = raw_input()
		if command_type in command_types_valid_inputs :
#			if command_type is 'd' :
#				mqttc.publish("relayduino/request/uptime");
#			elif command_type is 'm' :
#				mqttc.publish("relayduino/request/memory");
			if command_type is 'd' :
				duration = raw_input("Enter duration in minutes: ")
				try :
					int(duration)
				except ValueError :
					print "Ensure an integer is entered"
				else :
					client.publish("relayduino/control/duration_1", int(duration));
			else :
				print("Should not be here!!")
		else :
			print("Invalid command type entered")
	except KeyboardInterrupt :      #Triggered by pressing Ctrl+C
		tidyupAndExit()
