// 自作VST用のインクルードファイル
#include "myvst3define.h"
#include "myvst3fuid.h"
#include "controller.h"


// VST3作成に必要な名前空間を使用
namespace Steinberg {
	namespace Vst {


		// クラスを初期化する関数(必須)
		tresult PLUGIN_API MyVSTController::initialize(FUnknown* context)
		{
			// まず継承元クラスの初期化を実施
			tresult result = EditController::initialize(context);
			if (result == kResultTrue)
			{
				// パラメーターを追加
				parameters.addParameter(STR16("param1"), STR16("..."), 0, 1, ParameterInfo::kCanAutomate, PARAM1_TAG);

				// 以下固有の初期化を実施。

				// 今回は何もしない
			}

			// 初期化が成功すればkResultTrueを返す。
			result = kResultTrue;
			return result;
		}


	}
} // namespace SteinbergとVstの終わり