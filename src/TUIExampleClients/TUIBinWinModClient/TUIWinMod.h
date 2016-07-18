
#include <tuiframework/client/client.h>
#include <TUIPlugins/tuitypes/common/CommonTypeReg.h>


class TUIWinMod
{
public:
	TUIWinMod(void);
	virtual ~TUIWinMod(void);

	//// Connecting Signals and Slots
	void connect();
    void disconnect();
	void AnalogSignalChanged(const AnalogVectorChangedEvent * e);
	//void MatrixSignalChanged(const PackedMatrix4Event * e);
	//void Matrix1SignalChanged(const Matrix4Event * e);

	void DigitalSignalChanged(const IntegerVectorChangedEvent * e);
	void WordSignalChanged(const WordChangedEvent *e);
	void dWordSignalChanged(const DWordChangedEvent *e);

private:
	VectorList<int> byteValues;
	VectorList<float> floatValues;
	VectorList<WORD> wordValues;
	VectorList<DWORD> dWordValues;

	tuiframework::IEventChannel * dOutChannel;
	tuiframework::IEventChannel * aOutChannel;
	tuiframework::IEventChannel * wOutChannel;
	tuiframework::IEventChannel * dwOutChannel;
};

