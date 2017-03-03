#pragma once
#include <Windows.h>
#include <chrono>

#include "../../tuiframework/core/EventQueue.h"
#include "../../tuiframework/core/IDevice.h"
#include "../../tuiframework/core/IEventSink.h"
#include "../../tuiframework/server/DeviceDescriptor.h"

#include "../../TUIPlugins/tuitypes/common/CommonTypeReg.h"

#include <iostream>
#include <cstdio>

#include "OPCClient.h"
#include "OPCHost.h"
#include "OPCServer.h"
#include "OPCProperties.h"
#include "OPCGroup.h"
#include "OPCItem.h"
#include "OPCItemData.h"
#include "Transaction.h"

class CMyCallback;

namespace tuiframework {
	class ITUIServer;
}

namespace tuidevices {

	class OPCDev : public tuiframework::IDevice, public tuiframework::IEventSink
	{
	public:
		OPCDev(const tuiframework::DeviceConfig & deviceConfig);
		virtual ~OPCDev();
		static IDevice * __cdecl createFunction(void * arg);
		static std::string __cdecl getDeviceName();
		tuiframework::IEventSink * getEventSink() const;
		int getEntityID() const;
		virtual void deviceConnect(tuiframework::ITUIServer & tuiServer);
		virtual void deviceDisconnect();
		virtual bool deviceExecute();
		virtual void deviceStop();
		virtual void deviceSetEventSink(tuiframework::IEventSink * eventSink);
		virtual const tuiframework::DeviceDescriptor & getDeviceDescriptor() const;
		virtual void push(tuiframework::IEvent * event);
	protected:
		static void * inputLoopThread_run(void * arg);
		void executeInputLoop();
		static void * outputLoopThread_run(void * arg);
		void executeOutputLoop();
		volatile bool outputLoopRunning;
		pthread_t outputLoopThread;
		volatile bool inputLoopRunning;
		pthread_t inputLoopThread;
		tuiframework::EventQueue outputEventQueue;
	private:
		/** \brief Information describing the device. */
		tuiframework::DeviceDescriptor deviceDescriptor;

		void eventCreator(bool sent);

		/** \brief Identifier for the entity. */
		int entityID;

		/** \brief The tui server. */
		tuiframework::ITUIServer * tuiServer;

		/** \brief The event sink. */
		tuiframework::IEventSink * eventSink;

		std::string hostName;
		std::string opcServerName;
		CString blockWriteVariable;

		COPCHost *host;
		COPCServer *opcServer;
		COPCGroup *group;
		COPCItem * readWritableItem;
		CAtlArray<COPCItem *>itemsCreated;
	};

}