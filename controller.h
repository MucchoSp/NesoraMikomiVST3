// VST3 SDKのインクルードファイル
#include "public.sdk/source/vst/vsteditcontroller.h"


// 自作VST用のインクルードファイル
#include "myvst3define.h"


// VST3作成に必要な名前空間を使用
namespace Steinberg {
	namespace Vst {


		// ===================================================================================
		// VSTのパラメーターを操作するためのControllerクラス
		// ===================================================================================
		class MyVSTController : public EditController
		{
		public:
			// クラスを初期化する関数(必須)
			tresult PLUGIN_API initialize(FUnknown* context);

			// 自作VST Controllerクラスのインスタンスを作成するための関数(必須)
			static FUnknown* createInstance(void*) { return (IEditController*)new MyVSTController(); }

		};


	}
} // namespace SteinbergとVstの終わり