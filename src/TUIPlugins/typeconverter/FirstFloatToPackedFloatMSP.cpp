#include <TUIPlugins/typeconverter/FirstFloatToPackedFloatMSP.h>
#include <tuiframework/core/Exception.h>
#define USE_TFDEBUG
#include <tuiframework/logging/Logger.h>

static const char * inTag = "in";
static const char * outTagPacked = "outPacked";

namespace tuiframework {

	IMSP * FirstFloatToPackedFloatMSP::createFunction(void * arg)
	{
		MSPConfig * config = static_cast<MSPConfig *>(arg);
		return new FirstFloatToPackedFloatMSP(*config);
	}

	static std::string typeName = "FirstFloatToPackedFloatMSP";


	std::string FirstFloatToPackedFloatMSP::getMSPTypeName()
	{
		return typeName;
	}

	FirstFloatToPackedFloatMSP::FirstFloatToPackedFloatMSP(const MSPConfig & config) :
		config(config) {



		this->out = 0;

		this->nameToIdMap[outTagPacked] = 0;

		try {
			this->id = config.getParameterGroup().getInt("Convert|id");
			this->size = config.getParameterGroup().getInt("Convert|size");

			TFDEBUG("ID = " << this->id);
			TFDEBUG("size = " << this->size);

		}
		catch (Exception & e) {
			TFERROR(e.getFormattedString());
		}

		this->eventDelegate.setReceiver(this, &FirstFloatToPackedFloatMSP::handleEvent);

		std::vector<std::pair<int, float>> floatVector;
		for (int i = 0; i < this->size; i++)
		{
			floatVector.push_back(pair<int, float >(i, INT32_MIN));
		}
		packedFloat.setItems(floatVector);

	}

	FirstFloatToPackedFloatMSP::~FirstFloatToPackedFloatMSP()
	{
	}

	const std::string & FirstFloatToPackedFloatMSP::getTypeName() const
	{
		return this->type.getTypeName();
	}

	IEventSink * FirstFloatToPackedFloatMSP::getEventSink(const std::string & name)
	{
		if (name.compare(inTag) == 0) {
			return &eventDelegate;
		}
		return 0;
	}

	void FirstFloatToPackedFloatMSP::registerEventSink(const std::string & name, IEventSink * eventSink)
	{
		map<string, int>::iterator iter = this->nameToIdMap.find(name);
		if (iter == this->nameToIdMap.end()) {
			return;
		}
		this->out = eventSink;
	}

	const MSPType & FirstFloatToPackedFloatMSP::getMSPType() const
	{
		return this->type;
	}
	void FirstFloatToPackedFloatMSP::handleEvent(AnalogChangedEvent * e)
	{

		if (this->out)
		{

			//pair<int, float> value_pair(0, e->getPayload().getItems.at(0).second);
			//cout << "Content event getPayload at id : " << this->id << "Content : " << e->getPayload().getItems().at(this->id).second << endl;
			//if (this->id == 1)
			//{
			//	packedFloat.getItems().at(0).second = e->getPayload().getItems().at(this->nameToIdMap[inTag]).second;
			//}
			//else
			//{
			//	packedFloat.getItems().at(0).second = e->getPayload().getItems().at(this->nameToIdMap[inTagPacked]).second;
			//}

			packedFloat.getItems().at(this->id).second = e->getPayload();
			cout << "PackedFloat : " << this->packedFloat << endl;
			PackedAnalogEvent* event = new PackedAnalogEvent(-1, -1, packedFloat);
			this->out->push(event);

		}
	}

}
