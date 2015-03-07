#include <Siv3D.hpp>
#include "../Common.hpp"
# include "Elis.hpp"

void Main()
{
	Window::Resize(1280, 720);
	//Graphics::SetBackground(Palette::Black);

	//
	//	�S�A�v�����L�t�H���g
	//
	//FontAsset::Register(L"Font.Common.Light26", 26, Typeface::Regular, { L"Common" });
	//FontAsset::Register(L"Font.Common.Light18", 18, Typeface::Regular, { L"Common" });

	//
	//	����������A�v��
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
			//	�Ƃ肠�����I���i�{���Ƃ͈Ⴄ����j
			//
			System::Exit();
		}

		game->update();
	}
}
