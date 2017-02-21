#pragma once

#include <tuiframework/server/MSPConfig.h>
#include <tuiframework/server/MSPType.h>
#include <TUIPlugins/tuitypes/common/CommonTypeReg.h>
#include <tuiframework/core/IMSP.h>
#include <tuiframework/core/EventDelegate.h>

namespace tuiframework {

	class OPCConnectorMSP : public IMSP
	{
	public:
		/**
		* \brief Constructor.
		*
		* \author Sebastian Keppler
		* \date 04.02.2016
		*
		* \param config The MSP-configuration.
		*/

		OPCConnectorMSP(const MSPConfig & config);

		/**
		* \brief Destructor.
		*
		* \author Sebastian Keppler
		* \date 04.02.2016
		*/

		virtual ~OPCConnectorMSP();

		/**
		* \brief Creates a function.
		*
		* \author Sebastian Keppler
		* \date 04.02.2016
		*
		* \param [in,out] arg If non-null, the argument.
		*
		* \return null if it fails, else the new function.
		*/

		static IMSP * createFunction(void * arg);

		/**
		* \brief Gets MSP-Type name.
		*
		* \author Sebastian Keppler
		* \date 04.02.2016
		*
		* \return The MSP-type name.
		*/

		static const std::string & getMSPTypeName();

		/**
		* \brief Gets type name.
		*
		* \author Sebastian Keppler
		* \date 04.02.2016
		*
		* \return The type name.
		*/

		virtual const std::string & getTypeName() const;

		/**
		* \brief Gets event sink.
		*
		* \author Sebastian Keppler
		* \date 04.02.2016
		*
		* \param name The name.
		*
		* \return null if it fails, else the event sink.
		*/

		virtual IEventSink * getEventSink(const std::string & name);

		/**
		* \brief Registers the event sink.
		*
		* \author Sebastian Keppler
		* \date 04.02.2016
		*
		* \param name				 The name.
		* \param [in,out] eventSink If non-null, the event sink.
		*/

		virtual void registerEventSink(const std::string & name, IEventSink * eventSink);

		/**
		* \brief Gets MSP-type.
		*
		* \author Sebastian Keppler
		* \date 04.02.2016
		*
		* \return The MSP-type.
		*/

		virtual const MSPType & getMSPType() const;

		/**
		* \brief Handles the WinMod-Event described by e.
		*
		* \author Sebastian Keppler
		* \date 04.02.2016
		*
		* \param [in,out] e If non-null, the WinModEvent to process.
		*/

		void handleWinModEvent(WinModEvent * e);

	protected:
		/** \brief The configuration. */
		MSPConfig config;
		/** \brief The type. */
		MSPType type;

		/** \brief The WinMod event delegate. */
		EventDelegateNC<WinModEvent, OPCConnectorMSP> OPCEventDelegate;
		/** \brief The name to identifier map. */
		std::map<std::string, int> nameToIdMap;

		/** \brief Array of EventSinks with the Size of 4, one for Float,Byte,Word and DWord each */
		IEventSink * out[4];
	};

}