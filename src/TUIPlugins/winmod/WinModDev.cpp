
#include "../../TUIPlugins/winmod/WinModDev.h"

#include "../../tuiframework/core/ITUIServer.h"
#include "../../tuiframework/core/IEvent.h"
#include "../../tuiframework/server/DeviceConfig.h"
#include "../../tuiframework/core/Exception.h"

#include "../../TUIPlugins/tuitypes/common/CommonTypeReg.h"

#include <iostream>
#include <sstream>
#include <array>


using namespace tuiframework;
using namespace std;

namespace tuidevices
{
	IDevice * __cdecl WinModDev::createFunction(void * arg)
	{
		DeviceConfig * deviceConfig = static_cast<DeviceConfig *>(arg);
		return new WinModDev(*deviceConfig);
	}

	std::string WinModDev::getDeviceName() {
		return "WinModDev";
	}

}

namespace tuidevices {

	WinModDev::WinModDev(const DeviceConfig & deviceConfig)
	{
		this->deviceDescriptor.setEntityID(deviceConfig.getEntityID());
		this->entityID = this->deviceDescriptor.getEntityID();

		map<string, Port> portMap;

		// Port Mapping of In-/Output Ports
		portMap["fO"] = Port("fO", "PackedAnalog", Port::Sink);

		portMap["fIn"] = Port("fIn", "PackedAnalog", Port::Source);

		portMap["byteOut"] = Port("byteOut", "PackedInteger", Port::Sink);
		portMap["byteIn"] = Port("byteIn", "PackedByte", Port::Source);

		DeviceType deviceType;
		deviceType.setPortMap(portMap);
		deviceType.setDeviceTypeName(getDeviceName());
		deviceType.setDescription("WinMod - Interface");
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
			this->ipcName = deviceConfig.getParameterGroup().getString("Client|IPCName");
			this->readOffset = deviceConfig.getParameterGroup().getInt("Client|readOff");
			this->readSize = deviceConfig.getParameterGroup().getInt("Client|readSize");
			this->writeOffset = deviceConfig.getParameterGroup().getInt("Client|writeOff");
			this->writeSize = deviceConfig.getParameterGroup().getInt("Client|writeSize");
			this->floatSize = deviceConfig.getParameterGroup().getInt("Client|sliderCount");
			this->byteSize = deviceConfig.getParameterGroup().getInt("Client|buttonCount");
			this->floatOff = deviceConfig.getParameterGroup().getInt("Client|sliderOff");
			this->byteOff = deviceConfig.getParameterGroup().getInt("Client|buttonOff");
			this->wordOff = deviceConfig.getParameterGroup().getInt("Client|wordOff");
			this->wordSize = deviceConfig.getParameterGroup().getInt("Client|wordSize");
			this->dWordOff = deviceConfig.getParameterGroup().getInt("Client|dWordOff");
			this->dWordSize = deviceConfig.getParameterGroup().getInt("Client|dWordSize");
			//TFDEBUG("PortNr = " << this->portNr);
		}
		catch (Exception & e) {
			//TFERROR(e.getFormattedString());
		}

		//this->receiver.setMyPort(this->portNr);

		this->deviceDescriptor.setNameChannelNrMap(nameChannelNrMap);

	}


	WinModDev::~WinModDev(void)
	{
	}



	tuiframework::IEventSink * WinModDev::getEventSink() const {
		return this->eventSink;
	}


	int WinModDev::getEntityID() const {
		return this->entityID;
	}


	bool WinModDev::deviceExecute() {

		{
			// Starts the Output-Thread
			int rc = pthread_create(&this->outputLoopThread, NULL, WinModDev::outputLoopThread_run, this);
			if (rc)
			{
				printf("ERROR; return code from pthread_create() is %d\n", rc);
			}

		}

		{
			// Starts the Input-Thread
			int rc = pthread_create(&this->inputLoopThread, NULL, WinModDev::inputLoopThread_run, this);
			if (rc) {
				printf("ERROR; return code from pthread_create() is %d\n", rc);
			}

		}

		return true;
	}


	void WinModDev::deviceStop() {
		// Stops Input-Thread
		pthread_cancel(this->inputLoopThread);
		pthread_join(this->inputLoopThread, 0);

		// Stops Output-Thread
		pthread_cancel(this->outputLoopThread);
		pthread_join(this->outputLoopThread, 0);

		printf("Closing...\n");
	}


	void WinModDev::push(IEvent * event) {
		// Pushes an Event to the output Queue
		this->outputEventQueue.push(event);
	}

	void WinModDev::deviceConnect(tuiframework::ITUIServer & tuiServer) {
		// Making known this WinMod-Device-Type to the TUIServer 
		tuiServer.tuiServerRegisterDevice(this->entityID, *this);
		tuiServer.tuiServerRegisterEventSink(this->entityID, *this);
		this->tuiServer = &tuiServer;
	}


	void WinModDev::deviceDisconnect() {
		// Disconnects this WinMod-Device-Type from the TUIServer
		if (tuiServer) {
			this->tuiServer->tuiServerDeregisterDevice(this->entityID);
			this->tuiServer->tuiServerDeregisterEventSink(this->entityID);
			this->tuiServer = 0;
		}
	}


	void WinModDev::deviceSetEventSink(tuiframework::IEventSink * eventSink) {
		this->eventSink = eventSink;
	}


	const DeviceDescriptor & WinModDev::getDeviceDescriptor() const {
		return this->deviceDescriptor;
	}

	void * WinModDev::inputLoopThread_run(void * arg) {
		printf("WinModDev - input loop thread started\n");
		WinModDev * WinModDevice = static_cast<WinModDev *>(arg);
		WinModDevice->executeInputLoop();
		return 0;
	}


	void WinModDev::executeInputLoop() {

		this->inputLoopRunning = true;

		// Connect to the WinMod Shared Process Memory
		HRESULT hr = WMY200CreateIPCRange(ipcStringToLPCTSRT(ipcName), readOffset, readSize, writeOffset, writeSize, &m_hRange);
		_ASSERT(SUCCEEDED(hr) || m_hRange);

		// Check if successfull connected
		if (SUCCEEDED(hr) && m_hRange)
		{
			float *fltCurrentData = (float*)calloc(floatSize, sizeof(float));
			unsigned char *iCurrentData = (unsigned char*)calloc(byteSize, sizeof(unsigned char));
			//unsigned short *wordCurrentData = (unsigned short*) calloc(wordSize,sizeof(unsigned short));
			//unsigned long *dWordCurrentData = (unsigned long*) calloc(dWordSize,sizeof(unsigned long));

			//WinModData wd;

			while (this->inputLoopRunning)
			{
				// Debug for Time-Measures
				//chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now();

				// Syncronization between Threads
#ifndef _WIN32
				fd_set rfds;
				struct timeval tv;
				int retval;

				FD_ZERO(&rfds);
				FD_SET(0, &rfds);

				tv.tv_sec = 0;
				tv.tv_usec = 300000;
				retval = select(1, &rfds, 0, 0, &tv);
#endif

#ifdef _WIN32
				Sleep(100);
#endif

				// If floatOff Parameter from the XML-Configuration is not disabled with -1
				if (floatOff != -1)
				{
					HRESULT hr = WMY200ReadBlockFloat(m_hRange, fltCurrentData, this->floatOff / 4, this->floatSize);
					// Read the Float-Block from Memory
					if (hr == WM_Y200_S_OK) // Float-Daten aus Prozessdatenabbild holen
					{
						// If Output-Port is ready
						if (this->eventSink)
						{

							PackedAnalogEvent *event = new PackedAnalogEvent();
							PackedType <float> pf;
							vector<pair<int, float>> vf;
							for (int i = 0; i < floatSize; i++)
							{
								pair<int, float> value_pair(i, fltCurrentData[i]);
								vf.push_back(value_pair);
							}
							pf.setItems(vf);
							// Encapsulate the Float-Values in the WinMod-Data Object
							//wd.setFloats(fltCurrentData,floatSize);
							event->setAddress(EPAddress(this->entityID, this->deviceDescriptor.getNameChannelNrMap()["fO"]));

							event->setPayload(pf);
							// Pushes the Event in the EventSink
							eventSink->push(event);
						}
					}

					// If Reading from Float-Memory wasnt successfull print Error Messages
					if (hr == WM_Y200_E_INVALID_READ_PARAM)
					{
						cout << "Y200 - Invalid Read-Parameter" << endl;
					}
					if (hr == WM_Y200_E_NO_SHARED_MEM)
					{
						cout << "Y200 - No Shared Memory" << endl;
					}
					if (hr == WM_Y200_E_INVALID_HANDLE)
					{
						cout << "Y200 - Invalid Handle" << endl;
					}
				}

				// If Byte-Offset(Buttons) is not disabled in XML-Configuration
				if (byteOff != -1)
				{
					// Read from Byte-Block
					if (WMY200ReadBlock8(m_hRange, iCurrentData, this->byteOff, byteSize) == WM_Y200_S_OK) // Integer-Daten aus Prozessdatenabbild holen
					{
						// Check if Output-Port is ready
						if (this->eventSink)
						{
							// ToDo Reactivate Byte-Values
							for (int i = 0; i < byteSize; i++) {
								this->packedInteger.init(byteSize);
								this->packedInteger.getItems().at(i).second = (int)iCurrentData[i];
							}

							PackedIntegerEvent *event2 = new PackedIntegerEvent();
							event2->setAddress(EPAddress(this->entityID, this->deviceDescriptor.getNameChannelNrMap()["byteOut"]));
							event2->setPayload(packedInteger);
							eventSink->push(event2);
						}

					}
				}

				/*	// If WordOffset is not disabled with -1 in XML-Configuration
					if(wordOff != -1)
					{
						// Read from Word-Block
						if(WMY200ReadBlock16(m_hRange,wordCurrentData,this->wordOff/2,this->wordSize)==WM_Y200_S_OK) // Word-Daten aus Prozessdatenabbild holen
						{
							// If Output-Port is ready
							if (this->eventSink)
							{
								// Encapsulate Word-Values in the WinMod-Data Object
								wd.setWords(wordCurrentData,wordSize);
							}
						}
					}

					// If DWordOffset is not disabled with -1 in XML-Configuration
					if(dWordOff != -1)
					{
						// Read from DWord-Block
						if(WMY200ReadBlock32(m_hRange,dWordCurrentData,this->dWordOff/4,this->dWordSize)==WM_Y200_S_OK) // Float-Daten aus Prozessdatenabbild holen
						{
							// If Output-Port is ready
							if (this->eventSink)
							{
								// Encapsulate DWord-Values in the WinMod-Data Object
								wd.setDWords(dWordCurrentData,dWordSize);
							}
						}
					}*/


					// Create a new WinModEvent
					//WinModEvent *event = new WinModEvent();
					// Set Address of the WinMod Event
					//event->setAddress(EPAddress(this->entityID, this->deviceDescriptor.getNameChannelNrMap()["wO"]));
					// Set the WinMod-Data Object as Payload for the Event
					//event->setPayload(wd);
					// Pushes the Event in the EventSink
					//eventSink->push(event);

					// Delete to avoid Memory-Leak
				//delete event;
			}
		}

		// Destroy WinMod IPC Handler
		if (SUCCEEDED(WMY200DestroyIPCRange(m_hRange)));
		m_hRange = NULL;
	}

	void WinModDev::executeOutputLoop() {

		this->outputLoopRunning = true;


		while (outputLoopRunning)
		{
			// Get incoming Event from EventQueue
			IEvent * event = this->outputEventQueue.pop();

			// If Event is valid
			if (event)
			{
				//HRESULT hr=WMY200CreateIPCRange(ipcStringToLPCTSRT(ipcName),readOffset,readSize,writeOffset,writeSize,&m_hRange);
				//_ASSERT(SUCCEEDED(hr) || m_hRange);

				// If WinMod-IPC-Handler is valid
				if (m_hRange)
				{
					// Check if event is eventType for Float-Values 	
					if (event->getEventTypeID() == PackedAnalogEvent::EventTypeID())
					{
						// Casting the IEvent to PackedAnalogChangedEvent
						PackedAnalogEvent* analogEvent = (PackedAnalogEvent*)event;
						// Get the floatValues
						PackedType<float> pf = analogEvent->getPayload();
						// Size of floatvalues
						int size = pf.getItems().size();
						// Get the Pointer to the floatValues
						//const float *value = pf.getValues().data();

						vector<float> floatVector;
						//cout << "Size: " << size << endl;
						for (int i = 0; i < size; i++)
						{
							floatVector.push_back(pf.getItems().at(i).second);
							//cout << "Value: " << floatVector.at(i) << endl;
						}


						//cout << value[0] << " " << value[1] << endl;
						// Write FloatValues to WinMod shared-Memory process
						if (WMY200WriteBlockFloat(m_hRange, floatVector.data(), floatOff, size) == WM_Y200_S_OK) {}

						//delete analogEvent;
						//delete value;
					}

					else if (event->getEventTypeID() == PackedIntegerEvent::EventTypeID())
					{
						PackedIntegerEvent* byteEvent = (PackedIntegerEvent*)event;

						//cout << byteEvent->getPayload() << endl;
						PackedType<int> b = byteEvent->getPayload();
						std::vector<int> intArray;
						int size = b.getItems().size();


						for (int i = 0; i < size; i++) {
							intArray.push_back(b.getItems().at(i).second);
						}


						vector<BYTE*> byteArray;

						for (int i = 0; i < size; i++) {
							byteArray.push_back((BYTE*)(intArray.data() + i));
						}


						for (int i = 0; i < size; i++) {
							// Write Bytes to WinMod shared-Memory process
							if (WMY200WriteBlock8(m_hRange, *(byteArray.data() + i), byteOff + i, size) == WM_Y200_S_OK) {}
						}

					}
				}
			}
		}
	}


	void * WinModDev::outputLoopThread_run(void * arg) {

		printf("WinModDevDA - output loop thread started\n");
		WinModDev * winModDevice = static_cast<WinModDev *>(arg);
		winModDevice->executeOutputLoop();

		return 0;
	}


	// Not used
	LPCTSTR WinModDev::ipcStringToLPCTSRT(string ipcString) {
		if (ipcString == "NULL")
			return NULL;
		else
			return (LPCTSTR)ipcString.c_str();
	}

}

