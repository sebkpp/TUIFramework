#include "ByteToBytePackage.h"


#include "../../tuiframework/core/Exception.h"
#define USE_TFDEBUG
#include "../../tuiframework/logging/Logger.h"

static const char * in0Tag = "in0";
static const char * in1Tag = "in1";
/*static const char * in2Tag = "in2";
static const char * in3Tag = "in3";
static const char * in4Tag = "in4";
static const char * in5Tag = "in5";
static const char * in6Tag = "in6";
static const char * in7Tag = "in7";
static const char * in8Tag = "in8";
static const char * in9Tag = "in9";*/
static const char * outTagPacked = "outPacked";

namespace tuiframework {
	IMSP * ByteToBytePackage::createFunction(void * arg)
	{
		MSPConfig * config = static_cast<MSPConfig *>(arg);
		return new ByteToBytePackage(*config);
	}

	static std::string typeName = "ByteToBytePackage";


	std::string ByteToBytePackage::getMSPTypeName()
	{
		return typeName;
	}

	ByteToBytePackage::ByteToBytePackage(const MSPConfig & config) :
		config(config) {

		this->eventDelegate.setReceiver(this, &ByteToBytePackage::handleEvent);

		this->out = 0;
		this->nameToIdMap[outTagPacked] = 0;

		try {
			this->id = config.getParameterGroup().getInt("Convert|id");
			this->byteSize = config.getParameterGroup().getInt("Convert|byteSize");
			TFDEBUG("ID = " << this->id);
			TFDEBUG("ByteSize = " << this->byteSize);
		}
		catch (Exception & e) {
			TFERROR(e.getFormattedString());
		}

		std::vector<std::pair<int, int>> byteVector;
		for (int i = 0; i < byteSize; ++i)
		{
			byteVector.push_back(pair<int, int >(i, INT32_MIN));
		}
		packedByte.setItems(byteVector);
	}

	ByteToBytePackage::~ByteToBytePackage()
	{
	}

	const std::string & ByteToBytePackage::getTypeName() const
	{
		return this->type.getTypeName();
	}

	IEventSink * ByteToBytePackage::getEventSink(const std::string & name)
	{
		if (name.compare(in0Tag) == 0) {
			return &eventDelegate;
		}
		if (name.compare(in1Tag) == 0) {
			return &eventDelegate;
		}
		/*if (name.compare(in2Tag) == 0) {
			return &eventDelegate;
		}
		if (name.compare(in3Tag) == 0) {
			return &eventDelegate;
		}
		if (name.compare(in4Tag) == 0) {
			return &eventDelegate;
		}
		if (name.compare(in5Tag) == 0) {
			return &eventDelegate;
		}
		if (name.compare(in6Tag) == 0) {
			return &eventDelegate;
		}
		if (name.compare(in7Tag) == 0) {
			return &eventDelegate;
		}
		if (name.compare(in8Tag) == 0) {
			return &eventDelegate;
		}
		if (name.compare(in9Tag) == 0) {
			return &eventDelegate;
		}*/
		return 0;
	}

	void ByteToBytePackage::registerEventSink(const std::string & name, IEventSink * eventSink)
	{
		map<string, int>::iterator iter = this->nameToIdMap.find(name);
		if (iter == this->nameToIdMap.end()) {
			return;
		}
		this->out = eventSink;
	}

	const MSPType & ByteToBytePackage::getMSPType() const
	{
		return this->type;
	}
	void ByteToBytePackage::handleEvent(DigitalChangedEvent * e)
	{
		if (this->out)
		{
			int entityID = e->getAddress().getEntityID();
			int portID = e->getAddress().getPortNr();

			if (this->index <= this->byteSize - 1)
				initEvent(entityID, portID);

			packedByte.getItems().at(this->eventOrder[std::to_string(entityID) + std::to_string(portID)]).second = e->getPayload();
			PackedIntegerEvent* event = new PackedIntegerEvent(-1, -1, packedByte);
			//for(int i=0;i<wd.getFloats().size();i++)
			//cout <<"MSPOut: Floats: "  << event2->getPayload().at(i)<< endl;

			this->out->push(event);
			cout << "Packed Integer event" <<packedByte<< endl;
		}
	}

	void ByteToBytePackage::initEvent(const int & entityID, const int & portID)
	{
		this->eventOrder[std::to_string(entityID) + std::to_string(portID)] = this->index;
		++index;
	}
}