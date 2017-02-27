#pragma once

#include <tuiframework/server/MSPConfig.h>
#include <tuiframework/server/MSPType.h>
#include <TUIPlugins/tuitypes/common/CommonTypeReg.h>
#include <tuiframework/core/IMSP.h>
#include <tuiframework/core/EventDelegate.h>
#include <TUIFramework\xml\XMLNodeFilter.h>

#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLString.hpp>

#include <xercesc/framework/StdOutFormatTarget.hpp>
#include <xercesc/framework/MemBufFormatTarget.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>

using namespace XERCES_CPP_NAMESPACE;

namespace tuiframework 
{

	class Mapper : public IMSP
	{
	public:

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

		Mapper(const MSPConfig & config);
		~Mapper();

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

		void handleEvent(WinModEvent * e);

		/**
		* \brief Handles the WinMod-Event described by e.
		*
		* \author Sebastian Keppler
		* \date 04.02.2016
		*
		* \param [in,out] e If non-null, the WinModEvent to process.
		*/

		void handleAnalogEvent(PackedAnalogEvent * e);

		/**
		* \brief Handles the WinMod-Event described by e.
		*
		* \author Sebastian Keppler
		* \date 04.02.2016
		*
		* \param [in,out] e If non-null, the WinModEvent to process.
		*/

		void handleIntegerEvent(PackedIntegerEvent * e);

		/*
		* \brief
		*
		* \author Sebastian Keppler
		* \date 12.09.2016
		* \param 
		*/
		void mapping();

	protected:
		/** \brief The configuration. */
		MSPConfig config;
		/** \brief The type. */
		MSPType type;

		/** \brief The WinMod event delegate. */
		EventDelegateNC<WinModEvent, Mapper> mapper;
		EventDelegateNC<PackedAnalogEvent, Mapper> packedAnalogHandler;
		EventDelegateNC<PackedIntegerEvent, Mapper> packedIntegerHandler;


		/** \brief The name to identifier map. */
		std::map<std::string, int> nameToIdMap;

		IEventSink * sceneOut;
		//IEventSink * sceneOutPacked;
	private:
		vector<SceneObject> sceneGraphObjects;
		vector<int> sceneObjectChildCount;
		WinModData tmpData;
		string name;
	};

}