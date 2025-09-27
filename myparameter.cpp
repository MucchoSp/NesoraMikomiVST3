#include "myparameter.h"

// VST3作成に必要なの名前空間を使用
namespace Steinberg {
	namespace Vst {

		// コンストラクタ(継承元メンバー変数などの初期化を行うためほぼ必須)
		MyParameter::MyParameter(const TChar* title, ParamID tag, const TChar* units, ParamValue minPlain,
			ParamValue maxPlain, ParamValue defaultValuePlain, int32 flags, UnitID unitID)
			: Parameter(title, tag, units, 0.0f, 0, flags, unitID) // 一部の引数は継承元クラスにそのまま渡し、初期化する
		{
			// 最小値、最大値を設定
			min = minPlain;
			max = maxPlain;

			// デフォルト値を設定
			// valueNormalizedは継承元(Parameter)クラスで定義された正規化された現在の値
			// toNormalized()関数を使って引数のデフォルト値(表示値)を正規化する
			valueNormalized = toNormalized(defaultValuePlain);
		}

		// 正規化された値(0.0f～1.0fの値)を表示値の文字列にする関数
		void MyParameter::toString(ParamValue valueNormalized, String128 string) const
		{
			// 正規化された値を文字列にするにはUString128クラスを使う
			// UString128クラスは内部に文字列用バッファ(長さ128)を持ったクラス
			// 詳細は割愛するが、文字列をいろいろ変換するための関数が用意されている
			UString128 wrapper;

			// printFloat()関数はfloatの値を文字列にし、wrapperの内部文字列用バッファに保存する
			// 正規化された値なので、toPlain()関数を使い表示値に変換してから保存する。
			// precisionは少数第何位まで表示するかを示す変数(継承元クラスで定義済み)
			wrapper.printFloat(toPlain(valueNormalized), precision);

			// 内部文字列用バッファからstringにコピーする
			wrapper.copyTo(string, 128);
		}

		// 表示されている文字列から正規化された値(0.0f～1.0fの値)を取得する関数
		bool MyParameter::fromString(const TChar* string, ParamValue& valueNormalized) const
		{
			// 表示値の文字列から正規化値を取得にするにはUString128クラスを使う
			// まず、wrapperの内部文字列用バッファにstringを設定する
			UString wrapper((TChar*)string, strlen16(string));

			//  表示値の文字列から表示値を取得する
			ParamValue plainvalue;
			if (wrapper.scanFloat(plainvalue))
			{
				// 表示値取得に成功したら、toNormalized()関数で正規化し、valueNormalizedに代入
				valueNormalized = toNormalized(valueNormalized);

				// 変換に成功したらtrueを返す
				return true;
			}

			// 変換に失敗したらfalseを返す
			return false;
		}

		// 正規化された値(0.0f～1.0fの値)から表示されている値にする関数
		ParamValue MyParameter::toPlain(ParamValue valueNormalized) const
		{
			// 正規化値を表示値に変換して返す
			// toNormalized()関数とtoPlain()関数で可逆であることが必要
			// 指数関数としてはとりあえずvalueNormalizedを3乗する関数にしておく(適当)
			// あわせて、最小値～最大値の範囲に収めるよう計算する。
			return ((max - min) * pow(valueNormalized, 3.0f)) + min;
		}

		// 表示されている値から正規化された値(0.0f～1.0fの値)にする関数
		ParamValue MyParameter::toNormalized(ParamValue plainValue) const
		{
			// 表示値を正規化値に変換して返す
			// toNormalized()関数とtoPlain()関数で可逆であることが必要
			// toPlain()関数では3乗しているので、plainValueの3乗根を計算する
			return pow((plainValue - min) / (max - min), 1.0f / 3.0f);
		}

	}
} // namespace SteinbergとVstの終わり