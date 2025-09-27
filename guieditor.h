#ifndef __GUIEDITOR_H__
#define __GUIEDITOR_H__

// VST3 SDKのインクルードファイル
#include "public.sdk/source/vst/vstguieditor.h"
#include "pluginterfaces/vst/ivstplugview.h"

// 自作VST用のインクルードファイル
#include "myvst3define.h"

// VST3作成に必要なの名前空間を使用
namespace Steinberg {
	namespace Vst {

		using namespace VSTGUI;

		// ============================================================================================
		// VSTのGUIクラス
		// ============================================================================================
		class MyVSTGUIEditor : public VSTGUIEditor, public IControlListener
		{
		public:
			// コンストラクタ(必須)
			MyVSTGUIEditor(void* controller);

			// GUIウィンドウを開いたときに呼び出される関数(必須)
			virtual bool PLUGIN_API open(void* parent, const PlatformType& platformType = PlatformType::kDefaultNative);

			// GUIウィンドウを閉じたときに呼び出される関数(必須)
			virtual void PLUGIN_API close();

			// GUIウィンドウのコントローラを操作したときに呼び出される関数(必須)
			void valueChanged(CControl* pControl);

			// 自作関数
			// ノブコントロールを作成する関数
			void createKnob(ParamID tag, int x, int y);
			void createSlider(ParamID tag, int x, int y);
			void createCombobox(ParamID tag, int x, int y);

			// VSTGUIEditorクラスの各種設定を自作GUIクラス置き換えるマクロ(必須)
			DELEGATE_REFCOUNT(VSTGUIEditor)
		};

	}
} // namespace SteinbergとVstの終わり

#endif