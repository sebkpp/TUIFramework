#pragma once
#include <tuiframework/server/MSPConfig.h>
#include <tuiframework/server/MSPType.h>
#include <TUIPlugins/tuitypes/common/CommonTypeReg.h>
#include <tuiframework/core/IMSP.h>
#include <tuiframework/core/EventDelegate.h>

namespace tuiframework {
	class FirstFloatToPackedFloatMSP :
		public IMSP
	{
	public:

		FirstFloatToPackedFloatMSP(const MSPConfig & config);
		~FirstFloatToPackedFloatMSP();
		static IMSP * createFunction(void * arg);
		static std::string getMSPTypeName();



		virtual const std::string & getTypeName() const;
		virtual IEventSink * getEventSink(const std::string & name);
		virtual void registerEventSink(const std::string & name, IEventSink * eventSink);
		virtual const MSPType & getMSPType() const;

		void handleEvent(AnalogChangedEvent * e);
	protected:
		MSPConfig config;
		MSPType type;


		EventDelegateNC < AnalogChangedEvent, FirstFloatToPackedFloatMSP > eventDelegate;


		PackedType<float> packedFloat;
		IEventSink * out;
		std::map<std::string, int> nameToIdMap;

		int id;
		int size;

		 

	};
}

