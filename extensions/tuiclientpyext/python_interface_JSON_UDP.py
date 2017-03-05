#! /usr/bin/env python3.5
import sys
import tuiclient
import json
from time import sleep
import _thread
import socket
import select

TUIdict = dict() #dictionary where we store all the values, and the meta datas
JSONdict = dict() #dictionary used for the TCP connection, filled only with the values

def systemMsgSink(msg):
	if msg == 1:
		print('CONNECTION ESTABLISHED')

		tuiclient.setEventCallback(dstmethod) #we call only one time this function, all the connections between the TUI objects and the server are done, based on the XML file, in the C++ code

		print("EVENT CALLBACK SUCEED")

#This function is called automatically each time a value changes.
#As this funcsttion also handles the initialization of the dictionnary, it is called for each TUI objects before any connection to make sure they are all stored into the dictionary (and so into the JSON file)
def dstmethod(name, portname, value, description, constraintMin, constraintMax, trafoType):
	global JSONdict

	initDict(name, portname, description, constraintMin, constraintMax, trafoType)

	#print("TUI_Instance: " + name + " ; port: " + portname + " ; value: " + value)
	JSONdict[name][portname]['Value'] = float(value) #we update the value of the corresponding port in the dictionary we use for the TCP connection
	
	PortName = portname + "Out"

	tuiclient.sendEvent(name, PortName, value)

#initialize the dictionary if it is not already done
def initDict(name, portname, description, constraintMin, constraintMax, trafoType):
	global TUIdict
	Port = dict()
	values = dict()

	if (name in TUIdict):
		Port = TUIdict[name]
		if (portname in TUIdict[name]):
			return

	description = description.split('_')
	description = name + '_' + description[1] + '_' + description[2]

	values['Constraint_Max'] = constraintMax
	values['Constraint_Min'] = constraintMin
	values['Transformation_Type'] = trafoType
	values['Description'] = description
	values['Value'] = 0.0

	Port[portname] = values

	TUIdict[name] = Port

	#the JSON file is created and filled only in the beginning of the programm, it contains all the informations.
	try:
		with open('TUIdict.json', 'w') as outfile:
			json.dump(TUIdict, outfile, ensure_ascii=False, indent=4, separators=(',', ': '))
		print("TUI_Instance: " + name + ", port: " + portname + " added to JSON")
	except:
		print("JSON file occupied")

	initJSON()
	return

#initialize the dictionary useful for the TCP connection
def initJSON():
	global TUIdict
	global JSONdict

	Port = dict()
	values = dict()

	for name in TUIdict.keys():
		for portName in TUIdict[name].keys():
			values['Value'] = TUIdict[name][portName]['Value']
			Port[portName] = values
			values = dict()

		JSONdict[name] = Port
		Port = dict()

#TCP server
def recv(tmp):
	global JSONdict

	UDP_IP = "127.0.0.1"
	UDP_PORT = 5005

	sock = socket.socket(socket.AF_INET, # Internet
						 socket.SOCK_DGRAM) # UDP

	while True:
		jsonDict = json.dumps(JSONdict)
		sock.sendto(jsonDict.encode('utf-8'), (UDP_IP, UDP_PORT))


tuiclient.init()
print('INITIALISATION FINISHED')

input()
th1 = _thread.start_new_thread(recv,("tmp",))
tuiclient.connectServer(8998, 8999, '127.0.0.1:7999', 1, systemMsgSink)
