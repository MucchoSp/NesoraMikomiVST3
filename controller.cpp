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
				parameters.addParameter(STR16("param1"), STR16("..."), 0, 1, ParameterInfo::kCanAutomate, PARAM_DEPTH_TAG);
				
				// 範囲パラメーターを作成
				RangeParameter* param1 = new RangeParameter(STR16("Speed"), PARAM_SPEED_TAG, STR16("Hz"), 0.5f, 30.0f, 5.0f);
				param1->setPrecision(2); // 小数第何位まで表示するか
				// 範囲パラメーターをコントローラーに追加
				parameters.addParameter(param1);

				// 自作パラメーターを作成
				MyParameter* param3 = new MyParameter(STR16("Freq"), PARAM_FILTERFREQ_TAG, STR16("Hz"), 50.0f, 22000.0f, 22000.0f);
				param3->setPrecision(2); // 小数第何位まで表示するか
				parameters.addParameter(param3);

				// 範囲パラメーターを作成
				RangeParameter* param4 = new RangeParameter(STR16("Q"), PARAM_FILTERQ_TAG, STR16(""), 0.5f, 12.0f, 0.5f);
				parameters.addParameter(param4);

				// 文字列リストパラメーターを作成、追加
				StringListParameter* param2 = new StringListParameter(STR16("Type"), PARAM_TYPE_TAG);
				param2->appendString(STR16("Volume"));  // リスト項目を追加
				param2->appendString(STR16("Tremolo"));
				param2->appendString(STR16("Panning"));
				param2->appendString(STR16("Lowpass Filter"));
				// 文字列リストパラメーターをコントローラーに追加
				parameters.addParameter(param2);

				// 以下固有の初期化を実施。

				// 今回は何もしない
			}

			// 初期化が成功すればkResultTrueを返す。
			result = kResultTrue;
			return result;
		}

		tresult PLUGIN_API MyVSTController::setComponentState(IBStream* state)
		{
			// 現在のProcessorクラスの状態を読込
			// マルチプラットフォーム対応にする場合はエンディアンに注意
			tresult res;

			// 保存されているデータを読み込む
			// 保存されているデータが複数ある場合はstate->readを繰り返す
			ParamValue depth;
			res = state->read(&depth, sizeof(ParamValue));
			if (res != kResultOk)
			{
				// 読込に失敗した場合はkResultFalseを返す。
				return kResultFalse;
			}
			// 読み込まれたデータをパラメーターに反映する
			// 反映するデータは0.0～1.0の範囲にしておく
			depth = plainParamToNormalized(PARAM_DEPTH_TAG, depth);
			setParamNormalized(PARAM_DEPTH_TAG, depth);

			// 関数の処理に問題がなければkResultOkを返す
			return kResultOk;
		}


	}
} // namespace SteinbergとVstの終わり