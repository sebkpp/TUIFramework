/*
    Copyright (C) 2010, 2011, 2012 The Fraunhofer Institute for Production Systems and
    Design Technology IPK. All rights reserved.

    This file is part of the TUIFramework library.
    It includes a software framework which contains common code
    providing generic functionality for developing applications
    with a tangible user interface (TUI).
    
    The TUIFramework library is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    The TUIFramework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with the TUIFramework.  If not, see <http://www.gnu.org/licenses/>.
*/



#include <TUIPlugins/dummy/DummyDevMatrix4.h>

#include <tuiframework/core/ITUIServer.h>
#include <tuiframework/core/IEvent.h>
#include <tuiframework/server/DeviceConfig.h>
#include <TUIPlugins/tuitypes/common/CommonTypeReg.h>

#include <cstdlib> // GCC 4.3 related build problem

#include <ctime>

#include <iostream>
#include <cstdio>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#endif

using namespace tuiframework;
using namespace std;

//IRe...
#include <chrono>
#include <XInput.h>
typedef chrono::high_resolution_clock Time;
typedef chrono::duration<float> fsec;
auto timeZero = Time::now();
#define MAX_CONTROLLERS 4
#define DEFAULT_PORT "27015"
#define DEAD_ZONE 300.0
#pragma comment(lib,"xinput9_1_0.lib")	// Need to link with Win7 xinput lib
#pragma comment (lib, "Ws2_32.lib")		// Need to link with Ws2_32.lib

int timeStep = 0;

struct CONTROLLER_STATE
{
	XINPUT_STATE lastState;
	XINPUT_STATE state;
	DWORD dwResult;
	//bool bLockVibration;
	XINPUT_VIBRATION vibration;
};

struct CONTROLLER_STATE g_Controllers[MAX_CONTROLLERS];
WCHAR g_szMessage[4][1024] = { 0 };
HWND g_hWnd;

HRESULT UpdateControllerState();

HRESULT UpdateControllerState()
{
	for (DWORD i = 0; i < MAX_CONTROLLERS; i++)
	{
		g_Controllers[i].lastState = g_Controllers[i].state;
		g_Controllers[i].dwResult = XInputGetState(i, &g_Controllers[i].state);
	}
	return S_OK;
}

//... IRe



namespace tuidevices {


IDevice * DummyDevMatrix4::createFunction(void * arg) {
    DeviceConfig * deviceConfig = static_cast<DeviceConfig *>(arg);
    return new DummyDevMatrix4(*deviceConfig);
}


std::string DummyDevMatrix4::getDeviceName() {
    return "DummyDevMatrix4";
}



DummyDevMatrix4::DummyDevMatrix4(const DeviceConfig & deviceConfig) {

    this->deviceDescriptor.setEntityID(deviceConfig.getEntityID());
    this->entityID = this->deviceDescriptor.getEntityID();

    map<string, Port> portMap;

    portMap["1"] = Port("1", "Matrix4Channel", Port::Source, "6DOF");
    portMap["2"] = Port("2", "Matrix4Channel", Port::Source, "6DOF");
    portMap["3"] = Port("3", "Matrix4Channel", Port::Source, "6DOF");
    portMap["4"] = Port("4", "Matrix4Channel", Port::Source, "6DOF");
    portMap["5"] = Port("5", "Matrix4Channel", Port::Source, "6DOF");
    portMap["6"] = Port("6", "Matrix4Channel", Port::Source, "6DOF");
    portMap["7"] = Port("7", "Matrix4Channel", Port::Source, "6DOF");
    portMap["8"] = Port("8", "Matrix4Channel", Port::Source, "6DOF");
	portMap["VREDOutput"] = Port("VREDOutput", "Matrix4VRED", Port::Source, "6DOF");

    DeviceType deviceType;
    deviceType.setPortMap(portMap);
    deviceType.setDeviceTypeName("DummyDevMatrix4");
    deviceType.setDescription("");

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
    this->deviceDescriptor.setNameChannelNrMap(nameChannelNrMap);

	//IRe ....Initialize Gamepad Controller
	DWORD dwResult;
	for (DWORD i = 0; i< 1; i++) //xInput takes up to 4 controllers => MS:  i = 0; i< XUSER_MAX_COUNT; i++
	{
		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(XINPUT_STATE));

		// Simply get the state of the controller from XInput.
		dwResult = XInputGetState(i, &state);

		if (dwResult == ERROR_SUCCESS)
		{

			printf("Gamepad Controller is connected.\n");
			printf("%s\n",i);
			printf("Simulation is paused, waiting for VRED to connect on port %s.\n", DEFAULT_PORT);
			fgetc(stdin);
		}
		else
		{
			printf("Error! No controller connected!\n");
			printf("Connect a controller, then press the Return key to continue.");
			fgetc(stdin);
		}
	}

	//...IRe

}


DummyDevMatrix4::~DummyDevMatrix4() {
}




bool DummyDevMatrix4::deviceExecute() {
    {
        int rc = pthread_create(&this->outputLoopThread, NULL, DummyDevMatrix4::outputLoopThread_run, this);
        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
        }
    }

    {
        int rc = pthread_create(&this->inputLoopThread, NULL, DummyDevMatrix4::inputLoopThread_run, this);
        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
        }
    }
    return true;
}


void DummyDevMatrix4::deviceStop() {
    pthread_cancel(this->inputLoopThread);
    pthread_join(this->inputLoopThread, 0);
    pthread_cancel(this->outputLoopThread);
    pthread_join(this->outputLoopThread, 0);
}


void DummyDevMatrix4::push(tuiframework::IEvent * event) {
    this->outputEventQueue.push(event);
}


void DummyDevMatrix4::deviceConnect(tuiframework::ITUIServer & tuiServer) {
    tuiServer.tuiServerRegisterDevice(this->entityID, *this);
    tuiServer.tuiServerRegisterEventSink(this->entityID, *this);
    this->tuiServer = &tuiServer;
}


void DummyDevMatrix4::deviceDisconnect() {
    if (tuiServer) {
        this->tuiServer->tuiServerDeregisterDevice(this->entityID);
        this->tuiServer->tuiServerDeregisterEventSink(this->entityID);
        this->tuiServer = 0;
    }
}


void DummyDevMatrix4::deviceSetEventSink(tuiframework::IEventSink * eventSink) {
    this->eventSink = eventSink;
}


const DeviceDescriptor & DummyDevMatrix4::getDeviceDescriptor() const {
    return this->deviceDescriptor;
}


void * DummyDevMatrix4::inputLoopThread_run(void * arg) {
    printf("DummyDevMatrix4 - input loop thread started\n");
    DummyDevMatrix4 * dummyDevice = static_cast<DummyDevMatrix4 *>(arg);
    dummyDevice->executeInputLoop();
    return 0;
}


void DummyDevMatrix4::executeInputLoop() {
    this->inputLoopRunning = true;
    while (this->inputLoopRunning) {
#ifndef _WIN32
    fd_set rfds;
    struct timeval tv;
    int retval;

    FD_ZERO(&rfds);
    FD_SET(0, &rfds);

    tv.tv_sec = 0;
    tv.tv_usec = 100000;
    retval = select(1, &rfds, 0, 0, &tv);
#endif
#ifdef _WIN32
        Sleep(40); //IRe
#endif
        if (this->eventSink) {
            {
				int a = 0;
                float c = 0;

				//IRe
				auto timeCurrent = Time::now();
				fsec fs = timeCurrent - timeZero; // Rehfeld: Time in seconds
				//ms d = std::chrono::duration_cast<ms>(fs); // Rehfeld: Time in milliseconds
				//std::cout << fs.count() << "s\n";
				//std::cout << d.count() << "ms\n";
				double runningTime = fs.count();
				//IRe

				UpdateControllerState();
				XINPUT_STATE state = g_Controllers[0].state;
				int BTN = state.Gamepad.wButtons;			// controller buttons 				
				float LT = state.Gamepad.bLeftTrigger;		//trigger values range from 0 to 255
				float RT = state.Gamepad.bRightTrigger;		//trigger values range from 0 to 255
				float RX = state.Gamepad.sThumbRX;			// thumb stick right X values range from -32767 to +32767
				float RY = state.Gamepad.sThumbRY;			// thumb stick right Y values range from -32767 to +32767
				//determine how far the controller is pushed
				float magnitude = sqrt(RX*RX + RY*RY);
				//determine the direction the controller is pushed
				float normalizedLX = RX / magnitude;
				float normalizedLY = RY / magnitude;
				float normalizedMagnitude = 0;
				//check if the controller is outside a circular dead zone
				if (magnitude > DEAD_ZONE)
				{
					//clip the magnitude at its expected maximum value
					if (magnitude > 32767) magnitude = 32767;
					//adjust magnitude relative to the end of the dead zone
					magnitude -= DEAD_ZONE;
					//optionally normalize the magnitude with respect to its expected range
					//giving a magnitude value of 0.0 to 1.0
					normalizedMagnitude = magnitude / (32767 - DEAD_ZONE);
				}
				else //if the controller is in the deadzone zero out the magnitude
				{
					magnitude = 0.0;
					normalizedMagnitude = 0.0;
				}
				//printf("Left-Thumb X: %f \n", RX);
				//printf("Left-Thumb Y: %f \n", RY);
				//printf("Left-Thumb Magnitude: %f \n", magnitude);
				//printf("Left-Thumb Normalized Magnitude: %f \n", normalizedMagnitude);
				//printf("Button pressed: %d \n", BTN);

				//...IRe


                map<std::string, int>::iterator i = this->deviceDescriptor.getNameChannelNrMap().begin();
                map<std::string, int>::iterator e = this->deviceDescriptor.getNameChannelNrMap().end();
                while (i != e && a < 1) {
                    Matrix4Event * event = new Matrix4Event();
					//Matrix4VREDEvent *vredEvent = new Matrix4VREDEvent();
					//event->setAddress(EPAddress(this->entityID, this->deviceDescriptor.getNameChannelNrMap()["VREDOutput"]));
					event->setAddress(EPAddress(this->entityID, (*i).second));
                    				
					//IRe......
					/*--------------------------------------------------------------
					Get input from controller
					=> using only controller 0 of 0-3 possible controllers
					--------------------------------------------------------------*/

					
					
					
					//std::vector < Matrix4<double> > vredVector;
					
					Matrix4<double> mat;
                    
					if (timeStep < 100){
						mat[0][0] = 65.6 / 100.0 * timeStep ;
						mat[0][1] = 104.7 / 100.0 * timeStep;
						mat[0][2] = 181.8 / 100.0 * timeStep;
						mat[0][3] = -78.2 / 100.0 * timeStep;

						mat[1][0] = -25.1 / 100.0 * timeStep;
						mat[1][1] = 1.0;
						mat[1][2] = 0.0;
						mat[1][3] = 0.0;

						mat[2][0] = BTN;
						mat[2][1] = RX;
						mat[2][2] = RY;
						mat[2][3] = normalizedMagnitude;

						mat[3][0] = 0;
						mat[3][1] = timeStep;
						mat[3][2] = runningTime; //IRe
						mat[3][3] = 0;
					}

					else if (timeStep < 300){
						mat[0][0] = 65.6 + (90.0 - 65.6) / 200.0 * (timeStep - 100);
						mat[0][1] = 104.7 - (104.7 + 100.4) / 200.0 * (timeStep - 100);
						mat[0][2] = 181.8 - 181.8 / 200.0 *(timeStep - 100);
						mat[0][3] = -78.2 - (83.1 - 78.2) / 200.0 *(timeStep - 100);

						mat[1][0] = -25.1 + 25.1 / 200.0 * (timeStep - 100);
						mat[1][1] = 0.0;
						mat[1][2] = 1.0;
						mat[1][3] = 0.0;

						mat[2][0] = BTN;
						mat[2][1] = RX;
						mat[2][2] = RY;
						mat[2][3] = normalizedMagnitude;

						mat[3][0] = 0;
						mat[3][1] = timeStep;
						mat[3][2] = runningTime; //IRe
						mat[3][3] = 0;
					}

					else {
						mat[0][0] = 90.0 - 90.0 / 100.0 *  (timeStep - 300);
						mat[0][1] = -100.4 + 100.4 / 100.0 * (timeStep - 300);
						mat[0][2] = 0.0 + 0.0 / 100.0 * (timeStep - 300);
						mat[0][3] = -83.1 + 83.1 / 100.0 * (timeStep - 300);

						mat[1][0] = 0.0 + 0.0 / 100.0 * (timeStep - 300);
						mat[1][1] = 0.0;
						mat[1][2] = 0.0;
						mat[1][3] = 1.0;

						mat[2][0] = BTN;
						mat[2][1] = RX;
						mat[2][2] = RY;
						mat[2][3] = normalizedMagnitude;

						mat[3][0] = 0;
						mat[3][1] = timeStep;
						mat[3][2] = runningTime; //IRe
						mat[3][3] = 0;
					}
					
					// Add Matrix here to Vector
					//vredEvent->setPayload(vectorMatrix)
					//vredVector.push_back(mat);
					event->setPayload(mat);
					eventSink->push(event);
                    c += 1.0f;
                    ++a;
                    ++i;

					timeStep += 1;
					if (timeStep > 400){ timeStep = 0; }
                }
            }
        }
    }
}


void DummyDevMatrix4::executeOutputLoop() {
    this->outputLoopRunning = true;
    while (outputLoopRunning) {
        this->outputEventQueue.waitForData();
        IEvent * event = this->outputEventQueue.pop();
        if (event) {
            cout << "DummyDevMatrix4: " << event->getEventTypeID() << " " << event << endl;
            //delete event;
        }
    }
}


void * DummyDevMatrix4::outputLoopThread_run(void * arg) {
    printf("DummyDevMatrix4 - output loop thread started\n");
    DummyDevMatrix4 * dummyDevice = static_cast<DummyDevMatrix4 *>(arg);
    dummyDevice->executeOutputLoop();
    return 0;
}

}


