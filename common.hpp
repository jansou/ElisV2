# pragma once
# include <Siv3D.hpp>

enum class Application
{
	Title,
	Elis,
	GreedShooter,
	OekakiMagic,
	Streaks,
	RitoruToransu,
	Hamburger,
	UpUp,
};

	//
	//	ゲーム共通インターフェース
	//
	class Game
	{
	public:

		virtual ~Game(){}

		virtual bool init() = 0;

		virtual bool update() = 0;

		virtual Optional<Application> next() = 0;
	};

	//
	//	ポーズ画面作成クラス
	//
	//		[Esc] キーでポーズメニューが開くようになる。
	//
	class PauseHelper
	{
	public:

		PauseHelper(
			);

		//
		//	ポーズ中なら true を返す。
		//
		//	ポーズ中はカーソル以外何も描画する必要はありません。ゲームの更新も不要です。
		//
		bool
		onPause(
			) const;

		//
		//	毎フレーム呼ぶ。
		//
		//	ポーズ中ならポーズメニューを表示し、ポーズメニューの操作を受け付ける。
		//
		//	menuColor はポーズメニューの色
		//
		//		[ゲームに戻る] が選択された場合、直後の pauseEnded() が true を返す。
		//
		//		[メニューに戻る] が選択された場合、next に Application::Title が格納される。
		//
		//		[終了する] が選択された場合、内部で System::Exit() が呼ばれる。
		//
		void
		update(
			Optional<Application>& next,
			const Color& menuColor = Palette::Seagreen
			);

		//
		//	毎フレーム PauseHelper::update() のあとに呼ぶ。
		//
		//	ポーズ状態に入ると true を返す。
		//
		bool
		pauseStarted(
			);

		//
		//	毎フレーム PauseHelper::update() のあとに呼ぶ。
		//
		//	ポーズからゲームに復帰すると true を返す。
		//
		bool
		returnedToGame(
			);

	private:

		int m_count;

		DynamicTexture m_textureBack;

		bool m_requestCapture;

		bool m_pausing;

		//GUIGroup m_gui;
	};
