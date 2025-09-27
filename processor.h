// VST3 SDKのインクルードファイル
#include "public.sdk/source/vst/vstaudioeffect.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include "pluginterfaces/vst/ivstevents.h" // イベントバス利用時に必要

#include <vector>

// VST3作成に必要なの名前空間を使用
namespace Steinberg {
	namespace Vst {

		// ===================================================================================
		// 音声信号を処理するProcessorクラス
		// ===================================================================================
		class MyVSTProcessor : public AudioEffect
		{
		protected:
			ParamValue volume;
			std::vector<float> pitchList; // 押されたキーの音程(周波数)を保存する可変長配列

			ParamValue theta; // オシレータとして使用するSIN関数の角度θ
		public:
			// コンストラクタ
			MyVSTProcessor();

			// クラスを初期化する関数(必須)
			tresult PLUGIN_API initialize(FUnknown* context);

			// バス構成を設定する関数。
			tresult PLUGIN_API setBusArrangements(SpeakerArrangement* inputs, int32 numIns, SpeakerArrangement* outputs, int32 numOuts);

			// 音声信号を処理する関数(必須)
			tresult PLUGIN_API process(ProcessData& data);

			// 自作関数
			// MIDIノートオンイベント、MIDIノートオフイベントを受け取った場合に処理する関数
			virtual void onNoteOn(int channel, int note, float velocity);
			virtual void onNoteOff(int channel, int note, float velocity);

			// 自作VST Processorクラスのインスタンスを作成するための関数(必須)
			static FUnknown* createInstance(void*) { return (IAudioProcessor*)new MyVSTProcessor(); }
		};


	}
} // namespace SteinbergとVstの終わり