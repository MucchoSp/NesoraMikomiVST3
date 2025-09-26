// VST3 SDKのインクルードファイル
#include "pluginterfaces\base\funknown.h"

// VST3作成に必要なの名前空間を使用
namespace Steinberg {
	namespace Vst {

		// FUIDの生成
		static const FUID ProcessorUID(0x5521CC35, 0x51EB4F19, 0xAF3F9BF6, 0xCE40F585);
		static const FUID ControllerUID(0x81D69634, 0xB3584DB8, 0xBF074635, 0x4F5410DB);

	}
} // namespace SteinbergとVstの終わり

// 続きはこちら https://www.utsbox.com/?p=694