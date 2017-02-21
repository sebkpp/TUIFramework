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


#include <TUIFramework/client/TUIClientApp.h>

#include <TUIFramework/client/TUIObjectStub.h>

#include <TUIFramework/core/IEvent.h>
#include <TUIFramework/core/IDevice.h>
#include <TUIFramework/core/HostAddress.h>
#include <TUIFramework/core/EventMsgRouter.h>
#include <TUIFramework/core/EventRouter.h>

#define USE_TFDEBUG
#include <TUIFramework/logging/Logger.h>
#include <TUIFramework/core/TypeRegistration.h>

#include <TUIPlugins/tuitypes/common/CommonTypeReg.h>

#include <sstream>
#include <string>
#include <cstring> // GCC 4.3 related build problem

#include <iostream>

using namespace std;

namespace tuiframework {

TUIClientApp::TUIClientApp()  :
    udpSenderSocket(outHostMsgQueue),
    udpReceiverSocket(inSerializedDataQueue),
    multicastListener(inSerializedDataQueue),
    outHostMsgDispatcher(outHostMsgQueue),
    eventSerializer(outEventQueue, outHostMsgDispatcher),
    eventDeserializer(inSerializedDataQueue, inEventQueue, eventFactory),
    stubContainer(*this, this->eventChannelFactory),
    systemNotificationSink(0),
    connectedWithServer(false),
    created(false),
    multicastListenerActivated(false),
    myReceivePortNr(0) {

	TFINFO("Booting TUI Client")
}


TUIClientApp::~TUIClientApp() {
}



void TUIClientApp::exit() {
}


void TUIClientApp::create() {
    //@@TODO make it thread safe
    if ( ! this->created) {
        this->udpReceiverSocket.create();
        this->eventDeserializer.create();
        this->eventSerializer.create();
        this->udpSenderSocket.create();
        this->created = true;
    }
}


void TUIClientApp::cancel() {
    //@@TODO make it thread safe
    this->udpReceiverSocket.cancel();
    this->eventDeserializer.cancel();
    this->eventSerializer.cancel();
    this->udpSenderSocket.cancel();
    
#ifndef _WIN32
    if (this->multicastListenerActivated) {
        this->multicastListener.cancel();
    }
#endif

    this->created = false;
    this->outputLoopRunning = false;
}


void TUIClientApp::handleAttachedObjectsMsg(AttachedObjectsMsg * event) {
    TFDEBUG("TUIClientApp::handleAttachedObjectsMsg " << event);
    if ( ! this->connectedWithServer) {
		cout << "AttachedObjectMSG: " << event->getPayload().getTUIObjectTypeVector().at(0) << endl;
        this->stubContainer.createStubs(event->getPayload().getTUIObjectInstanceVector(), event->getPayload().getTUIObjectTypeVector());
        this->attachedObjects = event->getPayload();

		/*
		for (int i = 0; i < this->attachedObjects.getTUIObjectInstanceVector().size(); ++i)
		{
			cout << "TUIObject Instance Vector ####" << this->attachedObjects.getTUIObjectInstanceVector().at(i).getName();
			for (std::map<string, int>::const_iterator it = this->attachedObjects.getTUIObjectInstanceVector().at(i).getNameChannelNrMap().cbegin(); it != this->attachedObjects.getTUIObjectInstanceVector().at(i).getNameChannelNrMap().cend(); ++it)
			{
				cout << it->first << " => " << it->second << endl;
			}

		}
		*/
        this->connectedWithServer = true;
        if (this->systemNotificationSink) {
            this->systemNotificationSink->push(new SystemMsg(CONNECTION_ESTABLISHED));
        }
    }
}


void TUIClientApp::handleMulticastGroupInvitationMsg(MulticastGroupInvitationMsg * event) {
    TFDEBUG("TUIClientApp::handleMulticastGroupInvitationMsg " << event);
    const HostAddress & address = event->getPayload();
    this->multicastListener.setMCGroupIPAddress(address.getIPAddress());
    this->multicastListener.setPort(address.getPortNr());
    this->multicastListener.create();
    this->multicastListenerActivated = true;
    SystemCmdMsg * msg = new SystemCmdMsg();
    msg->setPayload(SystemCmd(SystemCmd::removeConnection, static_cast<unsigned short>(this->myReceivePortNr)));
    this->outEventQueue.push(msg);
}


void TUIClientApp::push(IEvent * event) {
    this->outEventQueue.push(event);
}



TUIObjectStubContainer & TUIClientApp::getTUIObjectStubContainer() {
    return this->stubContainer;
}


void TUIClientApp::processEvents() {
    while (this->inEventQueue.size()) {
        IEvent * event = this->inEventQueue.pop();
        if (event) {
            if (event->getEventTypeID() == HostEvent::EventTypeID()) {
                HostEvent * ipEventMsg = static_cast<HostEvent *>(event);
                IEvent * event2 = ipEventMsg->getPayload();

				/*
				if(event2->getEventTypeID() == ByteChangedEvent::EventTypeID())
				{
					ByteChangedEvent* byteEvent = (ByteChangedEvent*)event2;
					unsigned char byteTest = byteEvent->getPayload();
					cout <<"Hex" << hex << (int)byteTest << endl;
					
					for(int i = 0; i < 8; i++){
						cout << ((byteTest >> i) & 1);
					}
					cout << endl;
				}
				*/
				
                if (event2->getEventTypeID() >= EPEventMsgTypeIDOffset) {
                    this->stubContainer.handleEvent(static_cast<IEventMsg<EPAddress> *>(event2));
                } else if (event2->getEventTypeID() == AttachedObjectsMsg::EventTypeID()) {
                    this->handleAttachedObjectsMsg(static_cast<AttachedObjectsMsg *>(event2));
                } else if (event2->getEventTypeID() == MulticastGroupInvitationMsg::EventTypeID()) {
                    this->handleMulticastGroupInvitationMsg(static_cast<MulticastGroupInvitationMsg *>(event2));
                }
            }
            delete event;
        }
    }
}


IEventFactory & TUIClientApp::getEventFactory() {
    return this->eventFactory;
}


IEventChannelFactory & TUIClientApp::getEventChannelFactory() {
    return this->eventChannelFactory;
}


void TUIClientApp::executeOutputLoop() {
    this->outputLoopRunning = true;
    while (outputLoopRunning) {
        this->inEventQueue.waitForData();
        this->processEvents();
    }
}


bool TUIClientApp::connectWithTUIServer(
        int mySendPortNr,
        int myReceivePortNr,
        const std::string & serverAddress,
        IEventSink * systemNotificationSink,
        bool ownership) {

    this->myReceivePortNr = myReceivePortNr;
    this->systemNotificationSink = systemNotificationSink;


    this->outHostMsgDispatcher.addDstAddress(HostAddress(serverAddress));
    this->udpSenderSocket.setMyPort(mySendPortNr);
    this->udpReceiverSocket.setMyPort(myReceivePortNr);

    this->create();

    SystemCmdMsg * msg = new SystemCmdMsg();
    msg->setPayload(SystemCmd(SystemCmd::requestConnection, static_cast<unsigned short>(this->myReceivePortNr)));
    this->outEventQueue.push(msg);

    if (ownership) {
        this->executeOutputLoop();
    }

    return true;
}


bool TUIClientApp::disconnectFromTUIServer() {
    this->cancel();

    return true;
}


const AttachedObjects & TUIClientApp::getAttachedObjects() const {
    return this->attachedObjects;
}

}

