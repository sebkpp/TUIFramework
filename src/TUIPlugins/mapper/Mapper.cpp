#include <TUIPlugins\mapper\Mapper.h>

static const char * inWindModTag = "WinModIn";
static const char * outTag = "objectOut";

namespace tuiframework
{
	IMSP * tuiframework::Mapper::createFunction(void * arg)
	{
		MSPConfig * config = static_cast<MSPConfig *>(arg);
		return new Mapper(*config);
	}

	static std::string typeName = "Mapper";

	const std::string & tuiframework::Mapper::getMSPTypeName()
	{
		return typeName;
	}

	Mapper::Mapper(const MSPConfig & config) : config(config) , sceneOut(0)
	{
		this->mapper.setReceiver(this, &Mapper::handleEvent);
		this->packedAnalogHandler.setReceiver(this, &Mapper::handleAnalogEvent);
		this->packedIntegerHandler.setReceiver(this, &Mapper::handleIntegerEvent);

		this->name = config.getParameterGroup().getString("Client|Partname");
		mapping();

		//cout << "Childsize: " << sceneGraphObjects.size() << endl;


		for (int i = 0; i < sceneGraphObjects.size(); i++)
		{
			sceneObjectChildCount.push_back(sceneGraphObjects.at(i).getParameterListSize());
		}

	}


	Mapper::~Mapper()
	{
	}

	const std::string & tuiframework::Mapper::getTypeName() const
	{
		return getMSPTypeName();
	}

	IEventSink * tuiframework::Mapper::getEventSink(const std::string & name)
	{
		if (name.compare(inWindModTag) == 0) {
			return &this->mapper;
		}
	}

	void tuiframework::Mapper::registerEventSink(const std::string & name, IEventSink * eventSink)
	{
		if (name.compare(outTag) == 0)
		{
			this->sceneOut = eventSink;
		}
	}

	const MSPType & tuiframework::Mapper::getMSPType() const
	{
		return this->type;
	}

	void tuiframework::Mapper::handleEvent(WinModEvent * e)
	{

		WinModData winModData = e->getPayload();
		int listSize = sceneObjectChildCount.at(0);

		// i counter for all avialable float values
		// j counter for sceneObjects
		// k counter for float values inside a sceneObject
		for (int i = 0, j = 0, k = 0; i < winModData.getFloats().size(); i++)
		{
			string value = to_string(winModData.getFloats().at(i));
			string prevValue = "";

			if (tmpData.getFloats().size() != 0)
			{
				prevValue = to_string(tmpData.getFloats().at(i));
			}

			bool valueChanged = false;

			if (k >= listSize)
			{
				k = 0;
				j++;
				listSize = sceneObjectChildCount.at(j);
			}
			
			if (value != prevValue)
			{
				this->sceneGraphObjects.at(j).changeValue(k, value);
				valueChanged = true;
			}
			k++;

			
			if (this->sceneOut && k >= listSize && valueChanged)
			{
				this->sceneOut->push(new SceneObjectEvent(-1, -1, this->sceneGraphObjects.at(j)));
				//cout << this->sceneGraphObjects.at(j);
			}
		}

		tmpData = winModData;
	}

	void Mapper::handleAnalogEvent(PackedAnalogEvent * e)
	{
		vector<pair<int,float> > v = e->getPayload().getItems();
		int listSize = sceneObjectChildCount.at(0);
		int size = v.size();
		// i counter for all avialable float values
		// j counter for sceneObjects
		// k counter for float values inside a sceneObject
		for (int i = 0, j = 0, k = 0; i < size; i++)
		{
			string value = to_string(v.at(i).second);
			string prevValue = "";

			/*
			if (tmpData.getFloats().size() != 0)
			{
				prevValue = to_string(tmpData.getFloats().at(i));
			}
			*/
			//bool valueChanged = false;

			if (k >= listSize)
			{
				k = 0;
				j++;
				listSize = sceneObjectChildCount.at(j);
			}


			//if (value != prevValue)
			//{
				this->sceneGraphObjects.at(j).changeValue(k, value);
				bool valueChanged = true;
			//}
			k++;


			if (this->sceneOut && k >= listSize && valueChanged)
			{
				this->sceneOut->push(new SceneObjectEvent(-1, -1, this->sceneGraphObjects.at(j)));
				//cout << this->sceneGraphObjects.at(j);
			}
		}
	}

	void Mapper::handleIntegerEvent(PackedIntegerEvent * e)
	{
		vector<pair<int, int> > v = e->getPayload().getItems();
		int listSize = sceneObjectChildCount.at(0);
		int size = v.size();
		// i counter for all avialable float values
		// j counter for sceneObjects
		// k counter for float values inside a sceneObject
		for (int i = 0, j = 0, k = 0; i < size; i++)
		{
			string value = to_string(v.at(i).second);
			string prevValue = "";

			/*
			if (tmpData.getFloats().size() != 0)
			{
			prevValue = to_string(tmpData.getFloats().at(i));
			}
			*/
			//bool valueChanged = false;

			if (k >= listSize)
			{
				k = 0;
				j++;
				listSize = sceneObjectChildCount.at(j);
			}


			//if (value != prevValue)
			//{
			this->sceneGraphObjects.at(j).changeValue(k, value);
			bool valueChanged = true;
			//}
			k++;


			if (this->sceneOut && k >= listSize && valueChanged)
			{
				this->sceneOut->push(new SceneObjectEvent(-1, -1, this->sceneGraphObjects.at(j)));
				//cout << this->sceneGraphObjects.at(j);
			}
		}
	}

	void Mapper::mapping()
	{
		
		try {
			cout << "Initalizing..." << endl;
			XMLPlatformUtils::Initialize();
			cout << "Initalized" << endl;
		}  catch (const XMLException& toCatch) 
		{

			
			char * message = XMLString::transcode(toCatch.getMessage());
			cout << "Error during initialization! :\n" << message << "\n";
			XMLString::release(&message);
			
			return;
		}
		
		

		XercesDOMParser* parser = new XercesDOMParser();
		parser->setValidationScheme(XercesDOMParser::Val_Auto);
		parser->setDoNamespaces(true);    // optional
		parser->setDoSchema(true);
		parser->setIncludeIgnorableWhitespace(false);
		
		char* xmlFile = "C:/Users/keppseba/Documents/TUI2/extensions/PDM-XML_LBR.xml";
		//char* xmlFile = "C:/Users/SebKpp/Documents/Git/TUIFramework/extensions/test.xml";

		try 
		{
			parser->parse(xmlFile);
			DOMDocument *doc = parser->getDocument();
			XMLNodeFilter * xmlNodeFilter = new XMLNodeFilter();
			DOMTreeWalker * domTreeWalker = doc->createTreeWalker(doc->getDocumentElement(), DOMNodeFilter::SHOW_ALL, xmlNodeFilter, true);
			
			DOMNode* node = domTreeWalker->getCurrentNode();
			string parents = "";

			string tmp = "";
			string delimiter = "_";
			vector<string> list;

			while (node) // Traverse all Nodes
			{

				DOMElement * ele = static_cast<DOMElement*>(node);

				if (XMLString::compareString(XMLString::transcode(node->getParentNode()->getNodeName()), "SceneGraph") == 0)
				{
					tmp = XMLString::transcode(ele->getAttribute(XMLString::transcode("name")));
				}

				string nodeName = XMLString::transcode(ele->getNodeName()); // Debug, Current NodeName
				if (XMLString::compareString( XMLString::transcode(node->getNodeName()), "SceneObject") == 0)
				{
					parents = "";
					DOMNode *parNode = node->getParentNode(); // Current Parent Node
					while(XMLString::compareString(XMLString::transcode(parNode->getNodeName()), "SceneGraph") != 0 )
					{	
						// Reverse Traverse, Check if current SceneObject is on top of the Hierachy
						DOMElement* parEle = static_cast<DOMElement*>(parNode);
						
						// Construct a string from all parent nodes 
						string parName = XMLString::transcode(parEle->getAttribute(XMLString::transcode("name")));
						parents.insert(0, parName);
						parents.insert(parName.length(), delimiter); // Insert Delimiter into String
						
						//parents = XMLString::transcode(parEle->getAttribute(XMLString::transcode("name"))) + parents + "_";
						//cout << XMLString::transcode(parEle->getAttribute(XMLString::transcode("name"))) << endl;
						

						parNode = parNode->getParentNode();

					}
				}
				else if (XMLString::compareString(XMLString::transcode(node->getNodeName()), "parameter") == 0)
				{
					DOMElement* parEle = static_cast<DOMElement*>(node->getParentNode());
					string ss = parents;

					// Add Parent name to String
					ss.append(XMLString::transcode(parEle->getAttribute(XMLString::transcode("name"))));
					ss.append(delimiter); // Add Delimiter
					//Add Degree of Freedom
					ss.append(XMLString::transcode(ele->getAttribute(XMLString::transcode("name"))));
					ss.append(delimiter); // Add Delminter
					ss.append(XMLString::transcode(node->getTextContent())); // Add value to string

					//float value = stof(XMLString::transcode(node->getTextContent()));
					
					list.push_back(ss);
					//cout << ss << XMLString::transcode(ele->getAttribute(XMLString::transcode("name"))) << "_" << XMLString::transcode(node->getTextContent()) << endl;
				}
 				node = domTreeWalker->nextNode();
				
				if (node == NULL || (XMLString::compareString(XMLString::transcode(node->getParentNode()->getNodeName()), "SceneGraph") == 0 && !list.empty()) )
				{
					parents = ""; // Reset Parents
					//string currentNodeName = XMLString::transcode(ele->getAttribute(XMLString::transcode("name")));
					this->sceneGraphObjects.push_back(SceneObject(tmp, list));
					list.clear();
				}
			}
		}
		catch (const XMLException& toCatch) {
			char* message = XMLString::transcode(toCatch.getMessage());
			cout << "Exception message is: \n"
				<< message << "\n";
			XMLString::release(&message);
			return ;
		}
		catch (const DOMException& toCatch) {
			char* message = XMLString::transcode(toCatch.msg);
			cout << "Exception message is: \n"
				<< message << "\n";
			XMLString::release(&message);
			return ;
		}
		catch (...) {
			cout << "Unexpected Exception \n";
			return ;
		}
		
		XMLPlatformUtils::Terminate();
	}
}