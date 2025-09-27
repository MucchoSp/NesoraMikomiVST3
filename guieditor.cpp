// 自作VST用のインクルードファイル
#include "guieditor.h"


// VST3作成に必要なの名前空間を使用
namespace Steinberg {
	namespace Vst {

		// ============================================================================================
		// コンストラクタ
		// ============================================================================================
		MyVSTGUIEditor::MyVSTGUIEditor(void* controller)
			: VSTGUIEditor(controller)
		{
			// コンストラクタでウィンドウサイズを設定する
			// 設定しなければ、ウィンドウが開かない
			ViewRect viewRect(0, 0, 200, 200);
			setRect(viewRect);
		}

		// ============================================================================================
		// GUIウィンドウを開いたとき、閉じたときに呼び出される関数
		// ============================================================================================
		bool PLUGIN_API MyVSTGUIEditor::open(void* parent, const PlatformType& platformType)
		{
			// GUIウィンドウが開かれたときに、UIを作成する

			// まずはフレーム(配置領域)がすでに作成・設定されているか確認。
			// すでに作成・設定されている場合(frameがNULLでない場合)は終了
			// frameは継承元クラスで定義されている。
			if (frame) { return false; }

			// 作成するフレームのサイズを設定
			CRect size(0, 0, 200, 200);

			// フレームを作成。作成に失敗したら(NULLなら)終了。
			// 引数には、フレームサイズ、自作GUIクラスのポインタを指定する
			frame = new CFrame(size, this);
			if (frame == NULL) { return false; }

			// 作成したフレームに背景画像を設定
			CBitmap* cbmp = new CBitmap("background.png"); // リソースから背景画像を読み込む
			frame->setBackground(cbmp); // フレームに背景画像を設定
			cbmp->forget();	// フレームに設定後は背景画像はforgetで解放しておく

			// 作成したフレームを開く
			frame->open(parent);


			//　---------------------------------------------
			// ここから各コントローラの作成
			createCombobox(PARAM_TYPE_TAG, 10, 10);
			createKnob(PARAM_SPEED_TAG, 20, 35);
			createSlider(PARAM_DEPTH_TAG, 20, 90);

			// GUIウィンドウのオープンに成功した場合はtrueを返す
			return true;
		}


		// GUIウィンドウを閉じたときに呼び出される関数
		void PLUGIN_API MyVSTGUIEditor::close()
		{
			// GUIウィンドウが閉じたときに、UIを削除する

			// フレームを解放
			// 背景画像や追加したつまみ(ノブ）やスライダーなどもあわせて解放される
			// (個別で解放する必要はない)
			if (frame)
			{
				frame->forget();
				frame = 0;
			}
		}

		// ============================================================================================
		// GUIウィンドウのコントローラを操作したときに呼び出される関数
		// ============================================================================================
		void MyVSTGUIEditor::valueChanged(CControl* pControl)
		{
			// どのパラメーターが操作されたかを取得する。
			int32 index = pControl->getTag();

			// パラメーターの値を取得する。
			float value = pControl->getValueNormalized();

			// 取得した値をパラメーターに反映させる
			controller->setParamNormalized(index, value);

			// 音声処理クラスに反映した値を通知する
			controller->performEdit(index, value);
		}


		// ============================================================================================
		// 各コントローラを作成する自作関数
		// ============================================================================================
		void MyVSTGUIEditor::createKnob(ParamID tag, int x, int y)
		{
			// ノブコントロールを作成する

			// まずは画像ファイルを読み込む
			CBitmap* backbmp = new CBitmap("knob.png");
			CBitmap* handlebmp = new CBitmap("knob_handle.png");

			// ノブコントロールのサイズを画像ファイルから取得する
			CRect size(0, 0, backbmp->getWidth(), backbmp->getHeight());

			// ノブコントロールの位置を指定
			// 位置を設定(frameの左上が0,0となる)
			size.offset(x, y);

			// ノブコントロールの作成
			CKnob* control = new CKnob(size, this, tag, backbmp, handlebmp);

			// パラメーターの現在の値を取得し、コントローラに反映
			// controllerは継承元クラスで定義されており、
			// GUIEditorクラスを呼び出したコントローラ操作クラスを示す。
			ParamValue value = controller->getParamNormalized(tag);
			control->setValueNormalized(value);

			// ノブコントロールをフレームに登録
			frame->addView(control);

			// 読み込んだ画像は忘れず解放
			backbmp->forget();
			handlebmp->forget();

		}

		void MyVSTGUIEditor::createSlider(ParamID tag, int x, int y)
		{
			// スライダーの作成
			// まずは画像ファイルを読み込む
			CBitmap* backbmp = new CBitmap("slider.png");
			CBitmap* handlebmp = new CBitmap("slider_handle.png");

			// 画像ファイルからサイズを取得する
			CRect  size;
			size(0, 0, backbmp->getWidth(), backbmp->getHeight());
			size.offset(x, y);   // 位置を設定(tergetframeの左上が0,0となる)

			// スライダーの作成
			// スライダーハンドルはスライダー背景画像の上から下まで動かないよう、
			// 若干のマージンを持たせる。このマージンは画像によって変わる
			int bmpmargin = 1;
			CVerticalSlider* control = new CVerticalSlider(size, this, tag,
				y + bmpmargin,
				y + backbmp->getHeight() - (handlebmp->getHeight() + bmpmargin),
				handlebmp, backbmp);

			// パラメーターの現在の値を取得し、コントローラに反映
			ParamValue value = controller->getParamNormalized(tag);
			control->setValueNormalized(value);

			// ノブをフレームに登録
			frame->addView(control);

			// 読み込んだ画像は忘れず解放
			backbmp->forget();
			handlebmp->forget();

		}

		void MyVSTGUIEditor::createCombobox(ParamID tag, int x, int y)
		{
			// コンボボックスのサイズを設定(サイズは適当)
			CRect  size;
			size(0, 0, 60, 16);
			size.offset(x, y);

			// コンボボックス作成
			COptionMenu* control = new COptionMenu(size, this, tag);

			// コンボボックスに選択肢を追加する
			control->addEntry("Volume");
			control->addEntry("Tremolo");
			control->addEntry("Panning");


			// パラメーターの現在の値を取得し、コンボボックス反映
			ParamValue value = controller->getParamNormalized(tag);
			control->setValueNormalized(value);

			// 小型フォントを指定
			control->setFont(kNormalFontSmaller);

			// 文字色・背景色を設定
			control->setFontColor(kWhiteCColor);
			control->setBackColor(kBlackCColor);
			control->setFrameColor(kRedCColor);

			// コンボボックスをフレームに登録
			frame->addView(control);

		}


	}
} // namespace SteinbergとVstの終わり