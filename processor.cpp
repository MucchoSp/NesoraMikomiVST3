// 自作VST用のインクルードファイル
#include "myvst3fuid.h"
#include "myvst3define.h"
#include "processor.h"


// VST3作成に必要なの名前空間を使用
namespace Steinberg {
	namespace Vst {

		// =================================================================================
		// コンストラクタ
		// =================================================================================
		MyVSTProcessor::MyVSTProcessor()
		{
			// コントローラーのFUIDを設定する
			setControllerClass(ControllerUID);
		}

		// ===================================================================================
		// クラスを初期化する関数
		// ===================================================================================
		tresult PLUGIN_API MyVSTProcessor::initialize(FUnknown* context)
		{
			// まず継承元クラスの初期化を実施
			tresult result = AudioEffect::initialize(context);
			if (result == kResultTrue)
			{
				// 入力と出力を設定
				addAudioOutput(STR16("AudioOutput"), SpeakerArr::kStereo);

				// 入力のEventBusを追加する
				addEventInput(STR16("Event Input"), 1);

				// 以下固有の初期化を実施。
				volume = 0.0f;
				theta = 0.0f;
				pitchList.clear();
			}


			// 初期化が成功すればkResultTrueを返す。
			return result;
		}

		tresult PLUGIN_API MyVSTProcessor::setBusArrangements(SpeakerArrangement* inputs, int32 numIns, SpeakerArrangement* outputs, int32 numOuts)
		{
			// inputとoutputのバスが1つずつで、かつinputとoutputの構成がステレオの場合
			if (numIns == 1 && numOuts == 1 && inputs[0] == SpeakerArr::kStereo && outputs[0] == SpeakerArr::kStereo)
			{
				return AudioEffect::setBusArrangements(inputs, numIns, outputs, numOuts);
			}

			// 対応していないバス構成の場合、kResultFalseを返す。
			return kResultFalse;
		}

		// ===================================================================================
		// 音声信号を処理する関数
		// ===================================================================================
		tresult PLUGIN_API MyVSTProcessor::process(ProcessData& data)
		{
			// パラメーター変更の処理
			// 与えられたパラメーターがあるとき、dataのinputParameterChangesに
			// IParameterChangesクラスへのポインタのアドレスが入る
			if (data.inputParameterChanges != NULL)
			{
				// 与えられたパラメーターの数を取得
				int32 paramChangeCount = data.inputParameterChanges->getParameterCount();

				// 与えられたパラメーター分、処理を繰り返す。
				for (int32 i = 0; i < paramChangeCount; i++)
				{
					// パラメーター変更のキューを取得
					// (処理するサンプル内に複数のパラメーター変更情報がある可能性があるため、
					// キューという形になっている。)
					IParamValueQueue* queue = data.inputParameterChanges->getParameterData(i);
					if (queue != NULL)
					{
						// どのパラメーターが変更されたか知るため、パラメーターtagを取得
						int32 tag = queue->getParameterId();

						// 変更された回数を取得
						int32 valueChangeCount = queue->getPointCount();
						ParamValue value;
						int32 sampleOffset;

						// 最後に変更された値を取得
						if (queue->getPoint(valueChangeCount - 1, sampleOffset, value) == kResultTrue)
						{
							// tagに応じた処理を実施
							switch (tag)
							{
								// 今回はパラメータがないので何もしない
							}
						}
					}
				}
			}

			// イベントの処理
			// 与えられたイベントがあるときdataのinputEventsに
			// IEventListクラスへのポインタのアドレスが入る
			IEventList* eventList = data.inputEvents;
			if (eventList != NULL)
			{
				// イベントの数を取得する。
				int32 numEvent = eventList->getEventCount();
				for (int32 i = 0; i < numEvent; i++)
				{
					// i番目のイベントデータを取得する
					Event event;
					if (eventList->getEvent(i, event) == kResultOk)
					{
						int16 channel;
						int16 noteNo;
						float velocity;

						// イベントデータのタイプごとに振り分け
						switch (event.type)
						{
						case Event::kNoteOnEvent: // ノートオンイベントの場合
							channel = event.noteOn.channel;
							noteNo = event.noteOn.pitch;
							velocity = event.noteOn.velocity;

							onNoteOn(channel, noteNo, velocity);

							break;

						case Event::kNoteOffEvent: // ノートオフイベントの場合
							channel = event.noteOff.channel;
							noteNo = event.noteOff.pitch;
							velocity = event.noteOff.velocity;

							onNoteOff(channel, noteNo, velocity);
							break;
						}
					}
				}
			}

			// 入力・出力バッファのポインタをわかりやすい変数に格納
			// inputs[]、outputs[]はAudioBusの数だけある(addAudioInput()、addAudioOutput()で追加した分だけ)
			// 今回はAudioBusは1つだけなので 0 のみとなる
			// channelBuffers32は32bit浮動小数点型のバッファで音声信号のチャンネル数分ある
			// モノラル(kMono)なら 0 のみで、ステレオ(kStereo)なら 0(Left) と 1(Right) となる
			Sample32* outL = data.outputs[0].channelBuffers32[0];
			Sample32* outR = data.outputs[0].channelBuffers32[1];

			// numSamplesで示されるサンプル分、音声を処理する
			for (int32 i = 0; i < data.numSamples; i++)
			{
				// オシレーターの音程を取得する。
				float pitch = 0.0f;

				// 音程リストのサイズが0でない場合
				if (pitchList.size() != 0)
				{
					// 最後に押されたMIDIノートの音程を取得する
					pitch = pitchList[pitchList.size() - 1];
				}

				// 角度θに音程に応じた角速度を加える
				// (ここではサンプルレート44100Hz固定で計算しているので必要に応じて処理する)
				theta += (2.0f * 3.14159265f * pitch) / 44100.0f;

				// ボリュームとオシレータ(sin()関数)の値(振幅)を計算して出力する
				// (突然大きな音がでるとまずいのでとりあえず0.3fを掛けている)
				outL[i] = 0.3f * volume * sin(theta);
				outR[i] = 0.3f * volume * sin(theta);
			}

			// 問題なければkResultTrueを返す(おそらく必ずkResultTrueを返す)
			return kResultTrue;
		}

		void MyVSTProcessor::onNoteOn(int channel, int note, float velocity)
		{
			// MIDIノートオンイベントの処理を行う

			// 簡単なサンプルなので、channelとvelocityは無視する

			// 押されたノートから、音程を計算
			// ノートNo.69が440Hzになる。これを基準に計算する。
			// 計算式の詳細説明については割愛
			float pitch = (440.0f * powf(2.0f, (float)(note - (69)) / 12.0));

			// pitchListの最後に追加する
			pitchList.push_back(pitch);

			// ボリュームが0.0fだと音が出ないのでを1.0fにしておく
			volume = 1.0f;

		}

		void MyVSTProcessor::onNoteOff(int channel, int note, float velocity)
		{
			// MIDIノートオフイベントの処理を行う

			// 簡単なサンプルなので、channelとvelocityは無視する

			// 押されたノートから、音程を計算
			float pitch = (440.0f * powf(2.0f, (float)(note - (69)) / 12.0));

			// pitchListを最初から検索し、pitchに合致するものを削除する
			for (int i = 0; i < (int)pitchList.size(); i++)
			{
				if (pitchList[i] == pitch)
				{
					// pitchと合致するものがあった場合、
					// 該当するデータを取り除いて検索を終了する
					pitchList.erase(pitchList.begin() + i);
					break;
				}
			}

			// pitchListのサイズが0の場合、つまり押されたノートがない場合、
			// ボリュームを0.0fにして音を消す
			if (pitchList.size() == 0)
			{
				volume = 0.0f;
			}

		}

	}
} // namespace SteinbergとVstの終わり