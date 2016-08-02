#include "TUIClientWrapper.h"

extern "C"
{
	static TUICsharp* CSharpInstance;

	bool connectUnityWithTUIServer(int recievePort, int senderPort, const char* serverIPPort, void* tuiInit) {
		TUIInit init = *(TUIInit*)tuiInit;
		initTypeRegistration(getEventFactory());
		CommonTypeReg::registerTypes(&getEventFactory(), &getEventChannelFactory());

		getchar();

		connectWithTUIServer(recievePort, senderPort, std::string(serverIPPort), &init, true);
		return true;
	}

	void disconnectUnityWithTUIServer() {
		disconnectFromTUIServer();
	}

	void* createTUICsharpInstance() {
		CSharpInstance = new TUICsharp();
		return CSharpInstance;
	}

	void* createTUIInitInstance() {
		return new TUIInit();
	}

	void setTUIInitParameter(void* tuiInitInstance, void* tuiCSharpInstance) {
		TUIInit* tuiInit = (TUIInit*)tuiInitInstance;
		tuiInit->setTUICsharp((TUICsharp*)tuiCSharpInstance);
	}

	void connectingParameters(void* instance, int TUIType, const char* objectName, const char* channelName, integerCallback call) {
		TUICsharp* tuiUnity = (TUICsharp*)instance;
		tuiUnity->connecting(TUIType, std::string(objectName), std::string(channelName), call);
	}

	void connectingParametersfloat(void* instance, int TUIType, const char* objectName, const char* channelName, floatCallback call) {
		TUICsharp* tuiUnity = (TUICsharp*)instance;
		tuiUnity->connecting(TUIType, std::string(objectName), std::string(channelName), call);
	}

	void connectingParametersbool(void* instance, int TUIType, const char* objectName, const char* channelName, boolCallback call) {
		TUICsharp* tuiUnity = (TUICsharp*)instance;
		tuiUnity->connecting(TUIType, std::string(objectName), std::string(channelName), call);
	}

	void connectingParametersmouse(void* instance, int TUIType, const char* objectName, const char* channelName, mouseCallback call) {
		TUICsharp* tuiUnity = (TUICsharp*)instance;
		tuiUnity->connecting(TUIType, std::string(objectName), std::string(channelName), call);
	}

	void* connectingParametersMatrix4(void* instance, int TUIType, const char* objectName, const char* channelName) {
		TUICsharp* tuiUnity = (TUICsharp*)instance;
		Matrix4<double>* newData = new Matrix4<double>();
		matrix4Callback call = [newData](Matrix4<double> val) -> void {
			*newData = val;
		};
		tuiUnity->connecting(TUIType, std::string(objectName), std::string(channelName), call);
		return newData;
	}

	float getMatrix4Data(void* matrix, int row, int col) {
		Matrix4<double>* data = (Matrix4<double>*)matrix;
		return *(*data)[row * 4 + col];
	}


	int getParameterCount(void* instance) {
		TUICsharp* tuiUnity = (TUICsharp*)instance;
		return tuiUnity->getParameterCount();
	}

	int getDebugMessagesCount(void* instance) {
		TUICsharp* tuiUnity = (TUICsharp*)instance;
		return tuiUnity->debugMessages.size();
	}

	int getDebugMessage(void* instance) {
		TUICsharp* tuiUnity = (TUICsharp*)instance;
		int tmp = tuiUnity->debugMessages.front();
		tuiUnity->debugMessages.pop();
		return tmp;
	}

}


