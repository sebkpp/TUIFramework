#! /usr/bin/env python3.5
import sys
import tuiclient
import json
from time import sleep

TUIdict = dict() #dictionary where we store all the values, and the meta datas

def systemMsgSink(msg):
    if msg == 1:
        print('CONNECTION ESTABLISHED')

        tuiclient.setEventCallback(dstmethod) #we call only one time this function, all the connections between the TUI objects and the server are done, based on the XML file, in the C++ code
        
        print("EVENT CALLBACK SUCEED")

#This function is called automatically each time a value changes.
#As this function also handles the initialization of the dictionnary, it is called for each TUI objects before any connection to make sure they are all stored into the dictionary (and so into the JSON file)
def dstmethod(name, portname, value, description, constraintMin, constraintMax, trafoType):
	global TUIdict

	initDict(name, portname, description, constraintMin, constraintMax, trafoType)

	print("TUI_Instance: " + name + " ; port: " + portname + " ; value: " + value)

	TUIdict[name][portname]['Value'] = float(value) #we update the value of the corresponding port

	#we create/update the JSON file with the dictionary
	try:
		with open('TUIdict.json', 'w') as outfile:
			json.dump(TUIdict, outfile, ensure_ascii=False, indent=4, separators=(',', ': '))
		sleep(0.01) #frees the access to permit the clients to read the JSON file
	except:
		print("JSON file occupied")
		return
	return

#initialize the dictionary if it is not already done
def initDict(name, portname, description, constraintMin, constraintMax, trafoType):
	global TUIdict
	Port = dict()
	values = dict()

	if (name in TUIdict):
		Port = TUIdict[name]
		if (portname in TUIdict[name]):
			return

	values['Constraint_Max'] = constraintMax
	values['Constraint_Min'] = constraintMin
	values['Transformation_Type'] = trafoType
	values['Description'] = description
	values['Value'] = 0.0

	Port[portname] = values

	TUIdict[name] = Port

	print("TUI_Instance: " + name + ", port: " + portname + " added to JSON")

tuiclient.init()
print('INITIALISATION FINISHED')

input()
tuiclient.connectServer(8998, 8999, '127.0.0.1:7999', 1, systemMsgSink)