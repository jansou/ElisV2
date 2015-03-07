#include <Siv3D.hpp>
#include "../Common.hpp"
# include "Elis.hpp"

void Main()
{
	Window::Resize(1280, 720);
	//Graphics::SetBackground(Palette::Black);

	//
	//	全アプリ共有フォント
	//
	//FontAsset::Register(L"Font.Common.Light26", 26, Typeface::Regular, { L"Common" });
	//FontAsset::Register(L"Font.Common.Light18", 18, Typeface::Regular, { L"Common" });

	//
	//	初期化するアプリ
	//
	std::unique_ptr<Game> game(new Elis);

	if (!game->init()) return;

	while (System::Update())
	{
		if (Input::KeyAlt.pressed && Input::KeyEnter.clicked)
		{
			//Window::SetFullscreen(!Window::IsFullSceen(), Point(1280, 720));
		}

		if (auto next = game->next())
		{
			//Graphics::SetBackground(Palette::Black);
			//
			//	とりあえず終了（本来とは違う動作）
			//
			System::Exit();
		}

		game->update();
	}
}
