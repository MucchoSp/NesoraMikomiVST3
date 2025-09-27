// VST3 SDKのインクルードファイル
#include "public.sdk/source/vst/vsteditcontroller.h"


// 自作VST用のインクルードファイル
#include "myvst3define.h"
#include "pluginterfaces/vst/ivstmidicontrollers.h" // MIDIのPitchBendやControlChangeを扱うため


// VST3作成に必要な名前空間を使用
namespace Steinberg {
	namespace Vst {


		// ===================================================================================
		// VSTのパラメーターを操作するためのControllerクラス
		// ===================================================================================
		class MyVSTController : public EditController
							  , public IMidiMapping // MIDIのPitchBendやControlChangeを扱うために必要
		{
		public:
			// クラスを初期化する関数(必須)
			tresult PLUGIN_API initialize(FUnknown* context);

			// 自作VST Controllerクラスのインスタンスを作成するための関数(必須)
			static FUnknown* createInstance(void*) { return (IEditController*)new MyVSTController(); }

			// MIDIのPitchBendやControlChangeとパラメーターを関連付ける関数
			virtual tresult PLUGIN_API getMidiControllerAssignment(int32 busIndex, int16 channel, CtrlNumber midiControllerNumber, ParamID& id);

			// VST基本インターフェイス(IMidiMapping 等)をオーバーライドした場合に必要な宣言(詳細不明)
			// DEF_INTERFACEにインターフェイスを継承した分だけ加えていく。
			OBJ_METHODS(MyVSTController, EditController)
				DEFINE_INTERFACES
				DEF_INTERFACE(IMidiMapping)
				END_DEFINE_INTERFACES(EditController)
				REFCOUNT_METHODS(EditController)
		};


	}
} // namespace SteinbergとVstの終わり