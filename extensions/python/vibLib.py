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
		#setRenderQuality(VR_QUALITY_RAYTRACING) 
		#enableHeadlight(SWITCH_OFF)
		#enableAntialiasing(SWITCH_ON)
		#load(hvpData.pathOprDir  +  "operator.vpb") 
		load(hvpData.pathPntDir  + pntMtx[pntGrp][0] + "_"  + pntMtx[pntGrp][1][pntID] + ".vpb") 
		#load(hvpData.pathEnvDir  + envMtx[envGrp][0] + "_"  + envMtx[envGrp][1][envID] + ".vpb") 
		#load(hvpData.pathVehDir  + vehMtx[vehGrp][0] + "_"  + vehMtx[vehGrp][1][vehID] + ".vpb")   
		#hvpData.env = envMtx[envGrp][0] + "_"  + envMtx[envGrp][1][envID]
		#envPtr = findMaterial(hvpData.env)
		#applyEnvToAllMaterials(envPtr) 
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
	
	
def operate():
	if hvpData.drivers[5] == 1.0:
		hvpData.objPtr[0].setActive(True)
	else:
		hvpData.objPtr[0].setActive(False)
		
	if hvpData.drivers[6] == 1.0:
		hvpData.objPtr[1].setActive(True)
	else:
		hvpData.objPtr[1].setActive(False)
		
	if hvpData.drivers[7] == 1.0:
		hvpData.objPtr[2].setActive(True)
	else:
		hvpData.objPtr[2].setActive(False)
		
	setTransformNodeRotation(hvpData.drvPtr[0], hvpData.drivers[0], 0.0, 0.0)
	setTransformNodeRotation(hvpData.drvPtr[1], hvpData.drivers[1], 0.0, 0.0)
	setTransformNodeRotation(hvpData.drvPtr[2], hvpData.drivers[2], 0.0, 0.0)
	setTransformNodeRotation(hvpData.drvPtr[3], hvpData.drivers[3], 0.0, 0.0)
	setTransformNodeRotation(hvpData.drvPtr[4], hvpData.drivers[4], 0.0, 0.0)
	setTransformNodeRotation(hvpData.drvPtr[5], hvpData.drivers[5], 0.0, 0.0) #test
	setTransformNodeRotation(hvpData.drvPtr[6], hvpData.drivers[6], 0.0, 0.0) #test
	setTransformNodeTranslation(hvpData.drvPtr[7], 0.0, hvpData.drivers[7] +1300, 0.0, False) #test

	transf_1 = getTransformNodeTranslation(hvpData.oprPtr[0],True) #vec3f
	rx1 = transf_1.x()
	ry1 = transf_1.y()
	
	#setTransformNodeTranslation(hvpData.oprPtr[0], rx1 + hvpData.drivers[12]/10.0, ry1 + hvpData.drivers[13]/10.0, 0.0, True)
	
	
def driverSetup():
	hvpData.drvPtr = []
	hvpData.objPtr = []
	hvpData.oprPtr = []
	hvpData.drivers = [0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0]
	
	hvpData.drvPtr.append(findNode("link1"))
	hvpData.drvPtr.append(findNode("link2"))
	hvpData.drvPtr.append(findNode("link3"))
	hvpData.drvPtr.append(findNode("link6"))
	hvpData.drvPtr.append(findNode("flansch"))
	hvpData.drvPtr.append(findNode("link4"))	# test
	hvpData.drvPtr.append(findNode("link5"))	# test
	hvpData.drvPtr.append(findNode("PLH9002000513-A-Konzept 2 Assembly (150%) (View)"))	# test
	
	hvpData.objPtr.append(findNode("Front_1"))
	hvpData.objPtr.append(findNode("Front_2"))
	hvpData.objPtr.append(findNode("Front_3"))
	
	hvpData.oprPtr.append(findNode("Operator"))
	
	hvpData.objPtr[0].setActive(True)
	hvpData.objPtr[1].setActive(False)
	hvpData.objPtr[2].setActive(True)
	
	setTransformNodeRotation(hvpData.drvPtr[0], 0.0, 0.0, 0.0)
	setTransformNodeRotation(hvpData.drvPtr[1], 0.0, 0.0, 0.0)
	setTransformNodeRotation(hvpData.drvPtr[2], 0.0, 0.0, 0.0)
	setTransformNodeRotation(hvpData.drvPtr[3], 0.0, 0.0, 0.0)
	setTransformNodeRotation(hvpData.drvPtr[4], 0.0, 0.0, 0.0)
	setTransformNodeRotation(hvpData.drvPtr[5], 0.0, 0.0, 0.0) #test
	setTransformNodeRotation(hvpData.drvPtr[6], 0.0, 0.0, 0.0) #test
	setTransformNodeTranslation(hvpData.drvPtr[7], 0.0, 0.0, 0.0,False) #test
	
	hvpData.timer = vrTimer(hvpData.timeStep)
	hvpData.timer.connect(operate)
	hvpData.timer.setActive(False)
	
	
def startDriver():
	global lock
	if lock:
		driverSetup()
		startReceiver()
		hvpData.timer.setActive(True)
		lock = False
	else:
		print "Socket already created!"
	
def stopDriver():
	global lock
	#sock.setsockopt(socket.SOL_IP, socket.IP_DROP_MEMBERSHIP, socket.inet_aton('127.0.0.1') + socket.inet_aton('0.0.0.0'))
	try:
		lock = True
		driverSetup()
		print "Closing Socket..."
		sock.shutdown(socket.SHUT_RDWR) # shuts down the socket on both side
		sock.close()
		print "Closing Socket Succeeded..."
		hvpData.timer.setActive(False)
	except IOError:
		print "IOError: Socket was already closed previously!"
	except ValueError:
		print "ValueError: Socket was already closed previously!"
	except TypeError:
		print "TypeError: Socket was already closed previously!"
	
def recv(tmp):
	print "Creating Socket..."
	UDP_IP = "127.0.0.1"
	#UDP_PORT = 7999 # gets the state of button changed => freezes server output
	#UDP_PORT = 7998 # gets nothing
	#UDP_PORT = 8998 # gets nothing
	UDP_PORT = 8999 # grabs the client signal - all => freezes client output
	global sock  # sets the socket as a global variable so it can be reused in other functions
	sock = socket.socket(socket.AF_INET, # Internet
						 socket.SOCK_DGRAM) # UDP
	sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1) # socket flag preventing the socket to be left in a TIME_WAIT state (we can reuse the socket)
	sock.bind((UDP_IP, UDP_PORT))
	print "Binding Socket Succeeded..."
	while True:

		data, addr = sock.recvfrom(1024) # buffer size is 1024 bytes
		if data !="":
			list = data.split(' ')
			#print "5: %s  -   23: %s - 41: %s - 95: %s" % (list[5],list[23],list[41],list[95]) 
			#if len(list) == 51:
			str = list[136] # cannot convert last value of buffer to float?? 17 * matNumber
				
			hvpData.drivers[0] = float(list[5]) # link1 list[3]
			hvpData.drivers[1] = float(list[23]) # link2 list[4]
			hvpData.drivers[2] = float(list[41]) # link3
			hvpData.drivers[3] = float(list[59]) # link6
			hvpData.drivers[4] = float(list[77]) # flansch
			hvpData.drivers[5] = float(list[95]) # test link4
				
			hvpData.drivers[6] = float(list[113]) # visibility 0
			#hvpData.drivers[6] = float(list[9]) # visibility 1
			hvpData.drivers[7] = float(list[131]) # visibility 2
				
			hvpData.drivers[8] = float(list[11]) # button number
			hvpData.drivers[9] = float(list[12]) # thumb right X
			hvpData.drivers[10] = float(list[13]) # thumb right Y
			hvpData.drivers[11] = float(list[14]) # thumb right normalized magnitude
				
			hvpData.drivers[12] = float(list[15]) # blank
			hvpData.drivers[13] = float(list[16]) #time step 0 - 400
			hvpData.drivers[14] = float(list[17]) #clock time 
			
				

def startReceiver():
	print "Connecting to TUI Framework..."
	th1 = start_new_thread(recv,("tmp",))
	
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

#keyA = vrKey(Key_A)
#keyA.connect(startReceiver)	

	
# def SetZoom(value):
	# setCameraZoom(value)
	# vrLogInfo("Zooming by factor: " + str(value)) 
	# hvpLog("Info","a","Zooming by factor: " + str(value) + "</br>") 
	
	
# def hvpSetExposure(value):
	# envMat = findMaterial(hvpData.env)
	# e = envMat.fields().getReal32("exposure")
	# envMat.fields().setReal32("exposure", value)
	# vrLogInfo("Set exposure to: " + str(value)) 
	# hvpLog("Info","a","Set exposure to: " + str(value) + "</br>") 
	
	
# def hvpDrive():
	# if hvpData.drive == True:
		# vrLogWarning("Executing drive animation. Hit p-key to pause, c-key to contiune animation...")
		# hvpLog('info','a','Executing drive animation. Hit p-key to pause, c-key to contiune animation...</br>')
		# playCAnimation("drive",0,490)
	# else:
		# vrLogWarning( "This scene is not suitable for driving animations.Please load a suitable scene!")
		# hvpLog('note','a','This scene is not suitable for driving animations.</br>Please load a suitable scene!</br>')

# def hvpPause():
	# pauseCAnimation("drive",True)
	# vrLogWarning("Paused drive animation...")
	# hvpLog('info','a','Pause drive animation...</br>')
	
# def hvpCont():
	# pauseCAnimation("drive",False)
	# vrLogWarning("Executing drive animation...")
	# hvpLog('info','a','Continue drive animation...</br>')

	

