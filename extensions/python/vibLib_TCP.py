import socket
import json, os.path, time

#################################################################################################################################
####################################################### GLOBAL VARIABLES ########################################################
#################################################################################################################################

TUIdict = dict()				#TUIFramework signal is a dict
ConfigDict = dict()				#configuration dictionary, loaded from a JSON file
nodesList = dict()				#principal data structure
receive = False
path = "C:/Users/micka/Documents/Fraunhofer_2016_2017/TUI_real_merge_18_01_17/extensions/tuiclientpyext/TUIdict.json"
connection = True

#################################################################################################################################
####################################################### NODES LIST SETUP ########################################################
#################################################################################################################################

##### Call the recursive function for each child node of each instance #####
def recurseFindNodes( instances ):
    global nodesList

    for instance in instances.keys():
    	if len(instances[instance]) == 0:
    		print("Instance: " + instance + " is not found!")
    		del(nodesList[instance])
    		print(nodesList)
    		continue
        children = instances[instance][0].getNChildren()
        if children > 0:
        	for child in xrange(0, children):
        		researchNode(str(instance), instances[instance][0].getChild(child), nodesList[instance].keys())

##### Recursive function which looks if the child in parameter is a moving node and then call itself for all the children of the child #####
def researchNode( instanceName, instanceChild, portName ):
    global nodesList
    
    for port in portName:
        if instanceChild.getName() == nodesList[instanceName][port]['Description']:
            key = 'Pointer'
            if nodesList[instanceName][port].has_key(key) is False:
                setRotationAndPosition(instanceName, port, instanceChild)

    children = instanceChild.getNChildren()

    if children > 0:
        for child in xrange(0, children):
            researchNode(instanceName, instanceChild.getChild(child), nodesList[instanceName].keys())

##### Set the node pointer, its original position, its rotation position and its receive value in the dictionary nodesList #####
def setRotationAndPosition( instanceName, port, nodePtr ):
	global nodesList
	nodesList[instanceName][port]['Pointer'] = nodePtr
	nodesList[instanceName][port]['Original_Position'] = calculateLocalPosition(nodePtr)
	nodesList[instanceName][port]['Rotation_Position'] = nodePtr.getRotation()
	nodesList[instanceName][port]['Receive_Value'] = 0
	nodesList[instanceName][port]['Sign'] = True 		#signal sign, useful for the translation equations

##### Find all the interesting nodes in the scene and store them in a way that we can distinguish them #####
def findInterestingNodes():
	global nodesList
	global ConfigDict

	instances = dict()
	nodesList = ConfigDict

	for instance in nodesList.keys():
		instances[instance] = findNodes(str(instance))

	recurseFindNodes(instances)

#################################################################################################################################
#######################################################  RECEPTION SETUP  #######################################################
#################################################################################################################################

##### Receive the informations from the Python Interface with a TCP connection #####
def recv(tmp):
	global TUIdict
	global receive
	global connection

	hote = "localhost"
	port = 12800

	connexion_avec_serveur = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	connexion_avec_serveur.connect((hote, port))
	print("Connexion établie avec le serveur sur le port {}".format(port))

	connection = True
	while connection:
	    msg_a_envoyer = "Size"
	    # Peut planter si vous tapez des caractères spéciaux
	    msg_a_envoyer = msg_a_envoyer.encode()
	    # On envoie le message
	    connexion_avec_serveur.send(msg_a_envoyer)
	    msg_recu = connexion_avec_serveur.recv(1024)
	    #print(msg_recu.decode()) # Là encore, peut planter s'il y a des accents
	    byteSize = int(msg_recu.decode())

	    msg_a_envoyer = "Receive"
	    # Peut planter si vous tapez des caractères spéciaux
	    msg_a_envoyer = msg_a_envoyer.encode()
	    # On envoie le message
	    connexion_avec_serveur.send(msg_a_envoyer)

	    msg_recu = connexion_avec_serveur.recv(byteSize)
	    #print(msg_recu.decode()) # Là encore, peut planter s'il y a des accents
	    TUIdict = json.loads(msg_recu.decode())

	    if receive is False:
	    	findInterestingNodes()
	    	receive = True
	connexion_avec_serveur.close()
	print("Socket closed")

#################################################################################################################################
#######################################################   LOADING JSON    #######################################################
#################################################################################################################################

##### Load the JSON file which contains the information sent by the python interface #####
def loadJSON():
	global ConfigDict
	global path
	
	try:
		with open(path) as json_file:
			ConfigDict = json.load(json_file)
	except:
		print("JSON file occupied")
		return
	return

#################################################################################################################################
####################################################### TRANSLATION SETUP #######################################################
#################################################################################################################################

##### Calculate the local position of the node pointed by nodePtr #####
def calculateLocalPosition( nodePtr ):
	position = nodePtr.getLocalTranslation()
	matrix = nodePtr.getWorldTransform()

	xl = matrix[0] * position[0] + matrix[1] * position[1] + matrix[2] * position[2]
	yl = matrix[4] * position[0] + matrix[5] * position[1] + matrix[6] * position[2]
	zl = matrix[8] * position[0] + matrix[9] * position[1] + matrix[10] * position[2]

	return [xl, yl, zl]																															

#################################################################################################################################
######################################################      OPERATE      ########################################################
#################################################################################################################################

##### Update the signal and call the good sub-operate functions #####
def operate():
	global TUIdict
	global nodesList
	global receive

	if receive is False:
		return

	#print(TUIdict)
	for instance in nodesList.keys():
		for port in nodesList[instance].keys():
			nodePtr = nodesList[instance][port]['Pointer']
			
			if nodesList[instance][port]['Value'] != TUIdict[instance][port]['Value']:
				nodesList[instance][port]['Value'] = TUIdict[instance][port]['Value']

				if nodesList[instance][port]['Transformation_Type'] == 'rot' :
					rotationOperate (nodePtr, instance, port)

				else:
					translationOperate(nodePtr, instance, port)
			pass

##### Operate rotation on the node pointed by nodePtr using the value stored in the dictionary #####
def rotationOperate (nodePtr, instance, port):
	global nodesList

	rotate_x = nodesList[instance][port]['Rotation_Position'][0]
	rotate_y = nodesList[instance][port]['Rotation_Position'][1]
	rotate_z = nodesList[instance][port]['Rotation_Position'][2]

	if rotate_x == 0 and rotate_y == 0 and rotate_z == 0:
		pass
	else:
		setTransformNodeRotationOrientation(nodePtr, rotate_x, rotate_y, rotate_z)
	nodePtr.setRotation(rotate_x, rotate_y, rotate_z + nodesList[instance][port]['Value'])

##### Operate translation on the node pointed by nodePtr using the value stored in the dictionary #####
def translationOperate ( nodePtr, instance, port ):
	global nodesList

	translate_x = nodesList[instance][port]['Original_Position'][0]
	translate_y = nodesList[instance][port]['Original_Position'][1]
	translate_z = nodesList[instance][port]['Original_Position'][2]

	position = nodePtr.getLocalTranslation()
	matrix = nodePtr.getWorldTransform()
	local_position = calculateLocalPosition(nodePtr)

	if nodesList[instance][port]['Sign'] is True:
		d = nodesList[instance][port]['Value'] - abs(translate_y - local_position[1])
		if nodesList[instance][port]['Value'] < 0:
			nodesList[instance][port]['Sign'] = False
	else:
		d = nodesList[instance][port]['Value'] + abs(translate_y - local_position[1])
		if nodesList[instance][port]['Value'] >= 0:
			nodesList[instance][port]['Sign'] = True

	nodePtr.setLocalTranslation(matrix[1] * d + position[0], matrix[5] * d + position[1], matrix[9] * d + position[2])

#################################################################################################################################
######################################################    SETUP SCENE    ########################################################
#################################################################################################################################

def vibGetVeh():
	hvpData.listVeh = []
	PNG = []
	OSB = []
	for file in os.listdir(hvpData.pathVehDir):
		if file.endswith(".vpb"):
			OSB.append(file.split('.')[0])
	for file in os.listdir(hvpData.pathVehPrv):
		if file.endswith(".png"):
			PNG.append(file.split('.')[0])
	if len(OSB) == len(PNG):
		for i in range(len(OSB)):
			if OSB[i] == PNG[i]:
				pass 
			else:
				vrLogError("Preview not found for: " + str(OSB[i]) + ". Please contact nVIZ!")
				return False
	else:
		vrLogError("Number of vehicle previews not equal to number of VRED files. Please contact nVIZ!")
		return False
	for i in OSB: 
		hvpData.listVeh.append(i)
	return True

def vibGetEnv():
	hvpData.listEnv = []
	PNG = []
	OSB = []
	for file in os.listdir(hvpData.pathEnvDir):
		if file.endswith(".vpb"):
			OSB.append(file.split('.')[0])
	for file in os.listdir(hvpData.pathEnvPrv):
		if file.endswith(".png"):
			PNG.append(file.split('.')[0])
	if len(OSB) == len(PNG):
		for i in range(len(OSB)):
			if OSB[i] == PNG[i]:
				pass 
			else:
				vrLogError("Preview not found for: " + str(OSB[i]) + ". Please contact nVIZ!")
				return False
	else:
		vrLogError("Number of environment previews not equal to number of VRED files. Please contact nVIZ!")
		return False
	for i in OSB: 
		hvpData.listEnv.append(i)
	return True

def vibGetPnt():
	hvpData.listPnt = []
	PNG = []
	OSB = []
	for file in os.listdir(hvpData.pathPntDir):
		if file.endswith(".vpb"):
			OSB.append(file.split('.')[0])
	for file in os.listdir(hvpData.pathPntPrv):
		if file.endswith(".png"):
			PNG.append(file.split('.')[0])
	if len(OSB) == len(PNG):
		for i in range(len(OSB)):
			if OSB[i] == PNG[i]:
				pass 
			else:
				vrLogError("Preview not found for: " + str(OSB[i]) + ". Please contact nVIZ!")
				return False
	else:
		vrLogError("Number of plant previews not equal to number of VRED files. Please contact nVIZ!")
		return False
	for i in OSB: 
		hvpData.listPnt.append(i)
	return True

def vibLoadScene(vehGrp, envGrp, pntGrp, vehID, envID, pntID):
	vibLog("note", "w", "Loading scene...please wait.:</br>")
	vehCodes = []
	global lock
	lock = True
	for file in os.listdir(hvpData.pathVehDir):
		if file.endswith(".vpb"):
			code = (file.split('.')[0])
			vehCodes.append(code.split('_'))
	if vehGrp == 1:
		vrLogWarning("Warning: Invalid vehicle selection! Please select a valid vehicle.")
		vibLog("note", "w", "Invalid vehicle selection! Please select a valid vehicle.</br>")
		return
	envCodes = []
	for file in os.listdir(hvpData.pathEnvDir):
		if file.endswith(".vpb"):
			code = (file.split('.')[0])
			envCodes.append(code.split('_'))
	pntCodes = []
	for file in os.listdir(hvpData.pathPntDir):
		if file.endswith(".vpb"):
			code = (file.split('.')[0])
			pntCodes.append(code.split('_'))
	vehMtx = selMtx(vehCodes)
	envMtx = selMtx(envCodes)
	pntMtx = selMtx(pntCodes)
	vrLogInfo("+-------------------------------------------------------------------------------------------------------+")
	vrLogInfo("Loading scene:")
	vrLogWarning("  - Product: " + vehMtx[vehGrp][0] + "_"  + vehMtx[vehGrp][1][vehID])
	vrLogWarning("  - Environment: " + envMtx[envGrp][0] + "_"  + envMtx[envGrp][1][envID])
	vrLogWarning("  - Plant: " + pntMtx[pntGrp][0] + "_"  + pntMtx[pntGrp][1][pntID])
	vrLogInfo("+-------------------------------------------------------------------------------------------------------+")
	vibLog("note", "w", "Loaded scene:</br>")	
	vibLog("info", "a", "  - Product: " + vehMtx[vehGrp][0] + "_"  + vehMtx[vehGrp][1][vehID] + "</br>")
	vibLog("info", "a", "  - Environment: " + envMtx[envGrp][0] + "_"  + envMtx[envGrp][1][envID] + "</br>")
	vibLog("info", "a", "  - Plant: " + pntMtx[pntGrp][0] + "_"  + pntMtx[pntGrp][1][pntID] + "</br>")
	hvpData.drive = False
	try:

		load(hvpData.pathPntDir  + pntMtx[pntGrp][0] + "_"  + pntMtx[pntGrp][1][pntID] + ".vpb") 
		vibSelectView(0, 0, 0)
		driverSetup()

	except:
		vrLogError("Unexpected error loading VRED scene. Please contact nVIZ!")
		return 
	return
	
def vibLog(color,writeMode,string):
	logFile = open(hvpData.pathLog+"log.html", writeMode) #writeMode: w - new file , a - append to file
	if color == "note":
		logFile.write( "<font color='#6bc3c9'>" + string +  "</font>"  + "\n")
	elif color == "error":
		logFile.write( "<font color='red'>" + string +  "</font>"  + "\n")
	else:
		logFile.write(string + "\n")
	logFile.close()	
	
def selMtx(codes):
	list = []
	selMtx = []
	for i in range(len(codes)+1):
		if i == 0:
			list.append(codes[i][1])
		elif i == len(codes):
			grp = codes[i-1][0]
			selMtx.append([grp,list])
		else:
			_grp = codes[i-1][0]
			grp = codes[i][0]
			env = codes[i][1]
			if _grp == grp:
				list.append(env)
			else:
				selMtx.append([_grp,list])
				list = []
				list.append(env)
	return selMtx
	
def vibSelectView(vehGrp, vehID, vp): 
	if vp == 0:
		setFromAtUp(-1, 424.4, -1867.0, 1624.1, -1189.2, -355.4, 1605.5, -0.006, 0.005, 0.99) 
	elif vp == 1:
		setFromAtUp(-1, -1237.1, -670.2, 1470.3, -1342.5, 366.5, 1465.3, -0.0005, 0.005, 0.999)
	else:
		setFromAtUp(-1,-689.6, 1405.7, 1489.1, -659.8, 260.9, 1491.4, -0.0052, 0.002, 0.99) 
	setFar(100000000.0) 
	setNear(100.0)	
	setFov(45.0)
	setCameraZUp(true)		
	if vehGrp == 0:
		if vehID == 1:
			setFov(45.0)
	vrLogInfo("Set view: " + str(vp+1)) 
	vibLog("Info","a","Set view to: " + str(vp+1) + "</br>") 

#################################################################################################################################
#####################################################    SCENE STARTER    #######################################################
#################################################################################################################################

def driverSetup():
	hvpData.drvPtr = []
	hvpData.objPtr = []
	hvpData.oprPtr = []
	hvpData.drivers = [0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0]
	hvpData.timer = vrTimer(hvpData.timeStep)
	hvpData.timer.connect(operate)
	hvpData.timer.setActive(False)
	
def startDriver():
	global lock
	if lock:
		driverSetup()
		print("Connecting to TUI Framework...")
		loadJSON()
		print("Configuration loaded from JSON...")
		th1 = start_new_thread(recv,("tmp",))
		hvpData.timer.setActive(True)
		lock = False
		print("Start SUCCEEDED!")
	else:
		print("Error during start!")
	
def stopDriver():
	global lock
	global connection

	try:
		lock = True
		driverSetup()
		hvpData.timer.setActive(False)
		connection = False
		print("Stopping program success!")
	except IOError:
		print("Error during close!")
	
def setOculus():
	setDisplayMode(VR_DISPLAY_OCULUS_RIFT)

def setMonitor():
	setDisplayMode(VR_DISPLAY_STANDARD)


keyP = vrKey(Key_P)
keyP.connect(stopDriver)
keyC = vrKey(Key_C)
keyC.connect(startDriver)

keyO = vrKey(Key_O)
keyO.connect(setOculus)

keyM = vrKey(Key_M)
keyM.connect(setMonitor)