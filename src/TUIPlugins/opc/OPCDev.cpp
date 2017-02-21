#include <TUIPlugins/opc/OPCDev.h>

#include <tuiframework/core/ITUIServer.h>
#include <tuiframework/core/IEvent.h>
#include <tuiframework/server/DeviceConfig.h>
#include <tuiframework/core/Exception.h>

#include <TUIPlugins/tuitypes/common/CommonTypeReg.h>

#include <iostream>
#include <sstream>
#include <array>

using namespace tuiframework;
using namespace std;

#define MESSAGEPUMPUNTIL(x)	while(!x){{MSG msg;while(PeekMessage(&msg,NULL,NULL,NULL,PM_REMOVE)){TranslateMessage(&msg);DispatchMessage(&msg);}Sleep(1);}}

/**
* Handle asynch data coming from changes in the OPC group
*/
class CMyCallback :public IAsynchDataCallback {
public:
	void OnDataChange(COPCGroup & group, CAtlMap<COPCItem *, OPCItemData *> & changes, IEventSink * eventSink, DeviceDescriptor deviceDescriptor, int entityID, CString blockWriteVariable) //
	{
		//printf("Group %s, item changes\n", group.getName());
		POSITION pos = changes.GetStartPosition();
		while (pos != NULL) {
			
			COPCItem * item = changes.GetNextKey(pos);
			printf("-----> %s \n", item->getName());
			CAtlArray<CPropertyDescription> propDesc;
			item->getSupportedProperties(propDesc);

			CAutoPtrArray<SPropertyValue> propVals;
			item->getProperties(propDesc, propVals);
			short dataType = propVals[0]->value.iVal;
			printf("Datatype: %d",dataType);
			
			// Create a new AnalogEvent
			// Set Address of the WinMod Event
			
			//printf("%d\n", dataType);
			for (int i = 0; i < propDesc.GetCount(); i++) 
			{
				if (propVals[i] == NULL) 
				{
					printf("Failed to get property %u\n", propDesc[i].id);
				}
				else 
				{
					//printf("Property %u=", propDesc[i].id);
					//printf("%d\n", dataType);
						// Set the WinMod-Data Object as Payload for the Event
						switch (propDesc[i].type)
						{
						case VT_R8:
							//printf("%f\n", propVals[i]->value.dblVal); 
							break;
						case VT_R4:
							//printf("%f\n", propVals[i]->value.fltVal); 
							break;
						case VT_I4:
							//printf("%d\n", propVals[i]->value.iVal);
							break;
						case VT_I2:
							//printf("%d\n", propVals[i]->value.iVal); 
							break;
						case VT_I1:
							//printf("%d\n", propVals[i]->value.bVal);
							break;
						case VT_BOOL:
							//printf("%d\n", propVals[i]->value.boolVal); 
							break;
						case VT_DATE:
							//printf("%d\n", propVals[i]->value.date); 
							break;
						 case VT_VARIANT:
							if (dataType == VT_R4)
							{
							
								printf("with Value:  %f\n", propVals[i]->value.fltVal);
							
								// Create a new AnalogEvent
								AnalogChangedOPCEvent *event = new AnalogChangedOPCEvent();
								// Set Address of the WinMod Event
								event->setAddress(EPAddress(entityID, deviceDescriptor.getNameChannelNrMap()["aOPC"]));
							
								OPCType<float> opcType;
								opcType.setData(std::pair<std::string, float>(std::string(CT2CA(item->getName())), propVals[i]->value.fltVal));

								event->setPayload(opcType);
								// Pushes the Event in the EventSink
								eventSink->push(event);
						
							}

							if (dataType == VT_BOOL)
							{
								printf("with Value:  %d\n", propVals[i]->value.boolVal);
								DigitalChangedOPCEvent *event = new DigitalChangedOPCEvent();
								// Set Address of the WinMod Event
								event->setAddress(EPAddress(entityID, deviceDescriptor.getNameChannelNrMap()["dOPC"]));
								OPCType<bool> opcType;
								opcType.setData(std::pair<std::string, bool>(std::string(CT2CA(item->getName())), propVals[i]->value.boolVal));
								// Set the WinMod-Data Object as Payload for the Event
								event->setPayload(opcType);
								// Pushes the Event in the EventSink
								eventSink->push(event);
								// Delete to prevent Memory-Leak
								//delete event;
							
							}

							if (dataType == VT_I4)
							{
								printf("with Value:  %d\n", propVals[i]->value.intVal);
								IntegerChangedOPCEvent *event = new IntegerChangedOPCEvent();
								// Set Address of the WinMod Event
								event->setAddress(EPAddress(entityID, deviceDescriptor.getNameChannelNrMap()["iOPC"]));
								OPCType<int> opcType;
								opcType.setData(std::pair<std::string, int>(std::string(CT2CA(item->getName())), propVals[i]->value.intVal));
								// Set the WinMod-Data Object as Payload for the Event
								event->setPayload(opcType);
								// Pushes the Event in the EventSink
								eventSink->push(event);
								//printf("%d\n", propVals[i]->value.iVal);
							}
							break;
						default:
							//printf("No suitable Data-Type\n"); 
							break;
						}
						
				}				
			}
		}
	}
};


/**
*	Handle completion of asynch operation
*/
class CTransComplete :public ITransactionComplete {
	CString completionMessage;
public:
	CTransComplete() {
		completionMessage = "Asynch operation is completed";
	};

	void complete(CTransaction &transaction) {
		printf("%s\n", completionMessage);
	}

	void setCompletionMessage(const CString & message) {
		completionMessage = message;
	}
};


namespace tuidevices
{
	IDevice * __cdecl OPCDev::createFunction(void * arg)
	{
		DeviceConfig * deviceConfig = static_cast<DeviceConfig *>(arg);
		return new OPCDev(*deviceConfig);
	}

	std::string OPCDev::getDeviceName() 
	{
		return "OPCDev";
	}
}

namespace tuidevices {





	tuidevices::OPCDev::OPCDev(const tuiframework::DeviceConfig & deviceConfig)
	{
		this->deviceDescriptor.setEntityID(deviceConfig.getEntityID());
		this->entityID = this->deviceDescriptor.getEntityID();

		map<string, Port> portMap;

		// Port Mapping of In-/Output Ports
		portMap["dOPC"] = Port("dOPC", "DigitalOPCChannel", Port::Sink);
		portMap["aOPC"] = Port("aOPC", "AnalogOPCChannel", Port::Sink);
		portMap["iOPC"] = Port("iOPC", "IntegerOPCChannel", Port::Sink);
		portMap["dOPCIn"] = Port("dOPCIn", "DigitalOPCChannel", Port::Source);
		portMap["aOPCIn"] = Port("aOPCIn", "AnalogOPCChannel", Port::Source);
		portMap["iOPCIn"] = Port("iOPCIn", "IntegerOPCChannel", Port::Sink);

		portMap["mIn"] = Port("mIn", "MouseChannel", Port::Source);

		DeviceType deviceType;
		deviceType.setPortMap(portMap);
		deviceType.setDeviceTypeName(getDeviceName());
		deviceType.setDescription("OPC - Interface");
		this->deviceDescriptor.setDeviceType(deviceType);

		std::map<std::string, int> nameChannelNrMap;
		int nr = 1;
		map<string, Port>::iterator i = portMap.begin();
		map<string, Port>::iterator e = portMap.end();

		while (i != e) {
			nameChannelNrMap[(*i).second.getName()] = nr;
			++nr;
			++i;
		}

		// Try to read XML-Parameters
		try {
			this->hostName = deviceConfig.getParameterGroup().getString("Client|Hostname");
			this->opcServerName = deviceConfig.getParameterGroup().getInt("Client|OPCServer");
			this->blockWriteVariable = CString(deviceConfig.getParameterGroup().getString("Client|BlockWriteVariable").c_str());
		}
		catch (Exception & e) {
			//TFERROR(e.getFormattedString());
		}


		this->deviceDescriptor.setNameChannelNrMap(nameChannelNrMap);

	}

	OPCDev::~OPCDev()
	{
	}

	tuiframework::IEventSink * tuidevices::OPCDev::getEventSink() const
	{
		return this->eventSink;
	}

	int tuidevices::OPCDev::getEntityID() const
	{
		return this->entityID;
	}

	void tuidevices::OPCDev::deviceConnect(tuiframework::ITUIServer & tuiServer)
	{

		// Making known this OPC-Device-Type to the TUIServer 
		tuiServer.tuiServerRegisterDevice(this->entityID, *this);
		tuiServer.tuiServerRegisterEventSink(this->entityID, *this);
		this->tuiServer = &tuiServer;


	}

	void tuidevices::OPCDev::deviceDisconnect()
	{
		// Disconnects this OPC-Device-Type from the TUIServer
		if (tuiServer) {
			this->tuiServer->tuiServerDeregisterDevice(this->entityID);
			this->tuiServer->tuiServerDeregisterEventSink(this->entityID);
			this->tuiServer = 0;
		}
	}

	bool tuidevices::OPCDev::deviceExecute()
	{
		
		{
			// Starts the Output-Thread
			int rc = pthread_create(&this->outputLoopThread, NULL, OPCDev::outputLoopThread_run, this);
			if (rc)
			{
				printf("ERROR; return code from pthread_create() is %d\n", rc);
			}

		}
		
		
		{
			// Starts the Input-Thread
			int rc = pthread_create(&this->inputLoopThread, NULL, OPCDev::inputLoopThread_run, this);
			if (rc) {
				printf("ERROR; return code from pthread_create() is %d\n", rc);
			}

		}
		
		return true;
	}

	void tuidevices::OPCDev::deviceStop()
	{		
		// Stops Input-Thread
		pthread_cancel(this->inputLoopThread);
		pthread_join(this->inputLoopThread, 0);

		// Stops Output-Thread
		pthread_cancel(this->outputLoopThread);
		pthread_join(this->outputLoopThread, 0);

		printf("Closing...\n");
	}

	void tuidevices::OPCDev::deviceSetEventSink(tuiframework::IEventSink * eventSink)
	{
		this->eventSink = eventSink;
	}

	const tuiframework::DeviceDescriptor & tuidevices::OPCDev::getDeviceDescriptor() const
	{
		return this->deviceDescriptor;
	}

	void tuidevices::OPCDev::push(tuiframework::IEvent * event)
	{
		// Pushes an Event to the output Queue
		this->outputEventQueue.push(event);
	}

	void * tuidevices::OPCDev::inputLoopThread_run(void * arg)
	{
		printf("OPCDev - input loop thread started\n");
		OPCDev * opcDevice = static_cast<OPCDev *>(arg);
		opcDevice->executeInputLoop();
		return 0;
	}

	void tuidevices::OPCDev::executeInputLoop()
	{
		// OPC
		inputLoopRunning = true;

		if(COPCClient::isOpen())
		{
			COPCClient::stop();
		}
		COPCClient::init();
		host = COPCClient::makeHost(CString(this->hostName.c_str()));


		CString serverName = "Matrikon.OPC.Genie.1";//"CAEN.HVOPCServer.1";
		opcServer = host->connectDAServer(serverName);

		// make group
		unsigned long refreshRate;
		group = opcServer->makeGroup("Group1", true, 1000, refreshRate, 0.0);
		//COPCItem * readWritableItem = group->addItem(CString("Robot1.Position"), true);

		// SYNCH READ of item
		//OPCItemData data;
		//->readSync(data,OPC_DS_DEVICE);
		//printf("Synch read quality %d value %d\n", data.wQuality, data.vDataValue.iVal);
		//VARIANT var;
		//var.vt = VT_R4;
		//var.fltVal = 32.0f;

		//complete.setCompletionMessage("*******Asynch write completion handler has been invoked");
		//CTransaction *t = readWritableItem->writeAsynch(var, &complete);
		//readWritableItem->writeSync(var);

		//this->inputLoopRunning = true;
		
		CAtlArray<CString> itemNames;
		CAtlArray<COPCItem *>itemsCreated;
		CAtlArray<HRESULT> errors;

		// browse server
		CAtlArray<CString> opcItemNames;
		opcServer->getItemNames(opcItemNames);
		printf("Got %d names\n", opcItemNames.GetCount());
		for (int i = 0; i < opcItemNames.GetCount(); i++)
		{
			itemNames.Add(opcItemNames[i]);
			printf("%s\n",opcItemNames[i]);
		}

		if (group->addItems(itemNames, itemsCreated,errors,true) != 0)
		{
			printf("Item create failed\n");
		}

		// SYNCH read on Group
		//COPCItem_DataMap opcData;
		//group->readSync(itemsCreated, opcData, OPC_DS_DEVICE);

		// Enable asynch - must be done before any asynch call will work
		CMyCallback usrCallBack;
		group->enableAsynch(usrCallBack,this->eventSink,this->deviceDescriptor,this->entityID,this->blockWriteVariable);

		CTransComplete complete;
		//complete.setCompletionMessage("*******Asynch read completion handler has been invoked (OPC item)");
		CTransaction* t = nullptr;///= readWritableItem->readAsynch(&complete);
		//MESSAGEPUMPUNTIL(t->isCompeleted())
	
		//const OPCItemData * asychData = t->getItemValue(readWritableItem); // not owned
		//if (!FAILED(asychData->error)){
		//	printf("Asynch read quality %d value %d\n", asychData->wQuality, asychData->vDataValue.iVal);
		//}
		//delete t;

		// Aysnch read opc items from a group
		complete.setCompletionMessage("*******Asynch read completion handler has been invoked (OPC GROUP)");
		t = group->readAsync(itemsCreated, &complete);
		//MESSAGEPUMPUNTIL(t->isCompeleted())
		MESSAGEPUMPUNTIL(false)
		//while (this->inputLoopRunning)
		//{
		//}
		
	}

	void OPCDev::eventCreator(bool sent)
	{

	}

	void * tuidevices::OPCDev::outputLoopThread_run(void * arg)
	{
		printf("OPCDevDA - output loop thread started\n");
		OPCDev * opcDevice = static_cast<OPCDev *>(arg);
		opcDevice->executeOutputLoop();

		return 0;
	}

	void tuidevices::OPCDev::executeOutputLoop()
	{

		this->outputLoopRunning = true;



		while(outputLoopRunning)
		{
			// Get incoming Event from EventQueue
			IEvent * e = this->outputEventQueue.pop();
			
			// If Event is valid
			if (e) 
			{
				if(COPCClient::isOpen())
				{
					COPCClient::stop();
				}
				COPCClient::init();
				
				host = COPCClient::makeHost(CString(this->hostName.c_str()));


				CString serverName = "Matrikon.OPC.Genie.1";//"CAEN.HVOPCServer.1";
				opcServer = host->connectDAServer(serverName);

				// make group
				unsigned long refreshRate = 500;
				group = opcServer->makeGroup("Group1", true, 1000, refreshRate, 0.0);

				
				COPCItem *writeableItem = nullptr;
				
				if(e->getEventTypeID() == AnalogChangedOPCEvent::EventTypeID())
				{
					AnalogChangedOPCEvent *opcEvent = (AnalogChangedOPCEvent*) e;
					OPCType<float> item = opcEvent->getPayload();
					CString itemName(item.getData().first.c_str());
					
						//cout << item.getData().first << " " << item.getData().second << endl;
						//cout << group->getName();
					writeableItem = group->addItem(itemName, true);
						//CTransComplete complete;
					VARIANT var;
					var.vt = VT_R4;
					var.fltVal = item.getData().second +1;
						//complete.setCompletionMessage("*******Asynch write completion handler has been invoked");
						//cout << "Bin hier gelandet1" << endl;
						//CTransaction *t = writeableItem->writeAsynch(var,&complete);
						//cout << "Bin hier gelandet2" << endl;
						//writeableItem->writeSync(var);
						//readWritableItem->writeAsynch(var, &complete);
						//MESSAGEPUMPUNTIL(t->isCompeleted())
						//cout << "Bin hier gelandet3" << endl;
					writeableItem->writeSync(var);

				}

				if(e->getEventTypeID() == DigitalChangedOPCEvent::EventTypeID())
				{
					DigitalChangedOPCEvent *opcEvent = (DigitalChangedOPCEvent*) e;
					OPCType<bool> item = opcEvent->getPayload();
					CString itemName(item.getData().first.c_str());
					
					//cout << item.getData().first << " " << item.getData().second << endl;
					//cout << group->getName();
					writeableItem = group->addItem(itemName, true);
						//CTransComplete complete;
					VARIANT var;
					var.vt = VT_BOOL;
					var.boolVal = item.getData().second;

						//complete.setCompletionMessage("*******Asynch write completion handler has been invoked");
						//cout << "Bin hier gelandet1" << endl;
						//CTransaction *t = writeableItem->writeAsynch(var,&complete);
						//cout << "Bin hier gelandet2" << endl;
						
						//t = readWritableItem->writeAsynch(var, &complete);
						//MESSAGEPUMPUNTIL(t->isCompeleted())
						//cout << "Bin hier gelandet3" << endl;
						writeableItem->writeSync(var);
				}

				if(e->getEventTypeID() == IntegerChangedOPCEvent::EventTypeID())
				{
					IntegerChangedOPCEvent *opcEvent = (IntegerChangedOPCEvent*) e;
					OPCType<int> item = opcEvent->getPayload();
					CString itemName(item.getData().first.c_str());
					
					//cout << item.getData().first << " " << item.getData().second << endl;
					//cout << group->getName();
					writeableItem = group->addItem(itemName, true);
						
					//CTransComplete complete;
					VARIANT var;
					var.vt = VT_I4;
					var.intVal = item.getData().second +1;

					//complete.setCompletionMessage("*******Asynch write completion handler has been invoked");
					//cout << "Bin hier gelandet1" << endl;
					//CTransaction *t = writeableItem->writeAsynch(var,&complete);
					//cout << "Bin hier gelandet2" << endl;
					//writeableItem->writeSync(var);
					//MESSAGEPUMPUNTIL(t->isCompeleted())
					//cout << "Bin hier gelandet3" << endl;
					writeableItem->writeSync(var);
				}
				
				if(e->getEventTypeID() == MouseEvent::EventTypeID())
				{
					//cout << "Test" << endl;
					MouseEvent *opcEvent = (MouseEvent*) e;
					MouseData item = opcEvent->getPayload();
					VARIANT var;
					
					COPCItem * writeableItemX = group->addItem(CString("mouse.deltaX"),true);
					var.vt = VT_I4;
					var.intVal = item.getDeltaX();
					writeableItemX->writeSync(var);

					COPCItem * writeableItemY = group->addItem(CString("mouse.deltaY"),true);
					var.vt = VT_I4;
					var.intVal = item.getDeltaY();
					writeableItemY->writeSync(var);
					

					writeableItemX->~COPCItem();
					writeableItemY->~COPCItem();
					opcEvent->~EPEventMsg();
				}
				
					//MESSAGEPUMPUNTIL(false);
					//cout << "test" << endl;
				//writeableItem->~COPCItem();
			}	
			//MESSAGEPUMPUNTIL(false);

			//e->~IEvent();
		}
		//Sleep(100);
	}
}

