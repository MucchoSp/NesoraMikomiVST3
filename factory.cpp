// VST3 SDKのインクルードファイル
#include "public.sdk/source/main/pluginfactory.h"

// 自作VSTのヘッダファイルをインクルード
#include "myvst3fuid.h"
#include "processor.h"
#include "controller.h"

// 製作者(製作会社)の名前。終端文字「\0」含めて64文字まで。
#define MYVST_VENDOR   "Nesora"

// 製作者(製作会社)のWebサイトのURL。終端文字「\0」含めて256文字まで。
#define MYVST_URL      "https://misskey.io/@MucchoSP"

// 製作者(製作会社)の連絡先メールアドレス。終端文字「\0」含めて128文字まで。
#define MYVST_EMAIL    "puttyosp@gmail.com"

// 自作するVSTの名前。終端文字「\0」含めて64文字まで。
#define MYVST_VSTNAME  "Nesora Mikomi"

// 自作するVSTのバージョン。終端文字「\0」含めて64文字まで。
#define MYVST_VERSION  "1.0.0" 

// 自作するVSTのカテゴリ。終端文字「\0」含めて64文字まで。
#define MYVST_SUBCATEGORIES Vst::PlugType::kFx


// ===================================================================================
// DLLファイルの初期化、終了処理関数
// ===================================================================================
// 基本的に何もする必要はない。(VST SDK 3.7.1より前のバージョンの場合はコメントを外すこと)
//bool InitModule() { return true; }
//bool DeinitModule() { return true; }


// ===================================================================================
// 自作VSTプラグインの生成
// ===================================================================================
BEGIN_FACTORY_DEF(MYVST_VENDOR, MYVST_URL, MYVST_EMAIL)

	// MyVSTProcessorクラスの作成を行う
	DEF_CLASS2(INLINE_UID_FROM_FUID(Steinberg::Vst::ProcessorUID),
		PClassInfo::kManyInstances,
		kVstAudioEffectClass,
		MYVST_VSTNAME,
		Vst::kDistributable,
		MYVST_SUBCATEGORIES,
		MYVST_VERSION,
		kVstVersionString,
		Steinberg::Vst::MyVSTProcessor::createInstance)

	// MyVSTControllerクラスの作成を行う
	DEF_CLASS2(INLINE_UID_FROM_FUID(Steinberg::Vst::ControllerUID),
		PClassInfo::kManyInstances,
		kVstComponentControllerClass,
		MYVST_VSTNAME " Controller",	// 自作VSTの名前に"Controller"を付与したもの。
		0,						    // 使わないので必ず0にする
		"",						    // 使わないので必ず""にする
		MYVST_VERSION,
		kVstVersionString,
		Steinberg::Vst::MyVSTController::createInstance)

END_FACTORY