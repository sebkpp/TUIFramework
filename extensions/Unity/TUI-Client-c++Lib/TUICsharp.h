#pragma once

#include <tuiframework/client/client.h>
#include <TUIPlugins/tuitypes/common/CommonTypeReg.h>

using namespace tuiframework;

/**
* Integer Callback
* @param value Ganzzahl-Werte die an die TUI �berwegeben werden sollen.
*/
typedef void(*integerCallback)(int value); // Callback for Integer Values

/**
* Float Callback
* @param value Gleitkomma-Werte die an die TUI �berwegeben werden sollen.
*/
typedef void(*floatCallback)(float value); // Callback for float Values

/**
* Boolean Callback
* @param value Boolean-Werte die an die TUI �berwegeben werden sollen.
*/
typedef void(*boolCallback)(bool value); // Callback for boolean Values

typedef void(*doubleCallback)(double value); // Callback for double Values

/**
* ### Experimentel ###
* Mouse Callback
* @param value Mouse-Werte die an die TUI �berwegeben werden sollen.
*/
typedef void(*mouseCallback)(MouseData value); // Experimental Callback for MouseData

// Experimental
typedef void(*matrix4Callback)(Matrix4Data value); // Experimental Callback for MouseData


/**
* Struktur zum Zusammenfassen der Parameter zum Verbinden mit dem TUI-Channel
*/
struct listValues 
{
	int tuiType;
	std::string objectName;
	std::string channelName;
	integerCallback intCall;
	floatCallback floatCall;
	boolCallback boolCall;
	mouseCallback mouseCall;
	matrix4Callback matrix4Call;
};

/**
* Diese Klasse f�hrt den Programmcode in TUI auf C++ Seite aus der von C# aus ausgel�st wurde.
*/
class TUICsharp
{
public:
	TUICsharp();
	~TUICsharp();

	/**
	* F�gt die Parameter zu einem Struct zusammen und speichert diese in einer Liste.
	* @param TUIType ID des TUITypes
	* @param TUIObjectname TUI-Object-Type Name
	* @param channelName Name des TUI-Channels
	* @param integerCallBack Callback f�r Integer-Werte.
	*/
	void connecting(int TUIType, std::string TUIObjectName, std::string channelName, integerCallback call);

	/**
	* F�gt die Parameter zu einem Struct zusammen und speichert diese in einer Liste.
	* @param TUIType ID des TUITypes
	* @param TUIObjectname TUI-Object-Type Name
	* @param channelName Name des TUI-Channels
	* @param integerCallBack Callback f�r Float-Werte.
	*/
	void connecting(int TUIType, std::string TUIObjectName, std::string channelName, floatCallback call);

	/**
	* F�gt die Parameter zu einem Struct zusammen und speichert diese in einer Liste.
	* @param TUIType ID des TUITypes
	* @param TUIObjectname TUI-Object-Type Name
	* @param channelName Name des TUI-Channels
	* @param integerCallBack Callback f�r Boolean-Werte.
	*/
	void connecting(int TUIType, std::string TUIObjectName, std::string channelName, boolCallback call);

	/**
	* ### Experimentel ###
	* F�gt die Parameter zu einem Struct zusammen und speichert diese in einer Liste.
	* @param TUIType ID des TUITypes
	* @param TUIObjectname TUI-Object-Type Name
	* @param channelName Name des TUI-Channels
	* @param integerCallBack Callback f�r MouseData.
	*/
	void connecting(int TUIType, std::string TUIObjectName, std::string channelName, mouseCallback call);

	// Experimental
	void connecting(int TUIType, std::string TUIObjectName, std::string channelName, matrix4Callback call);



	/*
	* Verbindet die in der Liste enthaltenten Parameter mit dem TUI-Server
	*/
	void connect();

	/*
	* Beendet die Verbindung.
	*/
	void disconnect();



	/**
	* Funktion die ausgef�hrt wird sobald ein Event ausgel�st wird.
	* @param e integer Event
	*/
	void SignalChanged(const IntegerChangedEvent * e);

	/**
	* Funktion die ausgef�hrt wird sobald ein Event ausgel�st wird.
	* @param e Boolean Event
	*/
	void SignalChanged(const DigitalChangedEvent * e);

	/**
	* Funktion die ausgef�hrt wird sobald ein Event ausgel�st wird.
	* @param e Float Event
	*/
	void SignalChanged(const AnalogChangedEvent * e);

	/**
	* ### Experimentel ###
	* Funktion die ausgef�hrt wird sobald ein Event ausgel�st wird.
	* @param e Mouse Event
	*/
	void SignalChanged(const MouseEvent * e);

	// Experimental
	void SignalChanged(const Matrix4ChangedEvent * e);


private:

	/**
	* Struktur die alle Parameter f�r das verbinden des Channels mit dem TUI-Server enth�lt.
	*/
	listValues listVal;

	/**
	* Vector der alle strukturen mit den Parametern enth�lt.
	*/
	std::vector<listValues> list;

};

