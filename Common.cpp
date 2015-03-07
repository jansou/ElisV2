#include "Common.hpp"

//static const Rect rectButtonGame(402,230,220,40);
//static const Rect rectButtonMenu(402,300,220,40);
//static const Rect rectButtonExit(402,370,220,40);

PauseHelper::PauseHelper()
	:	m_count(0),
		m_requestCapture(false),
		m_pausing(false),
		m_gui(L"CommonMenu")
{
	System::SetExitEvent(WindowEvent::CloseButton);
	m_gui.init();

	GUI::Skin skin;
	m_gui.add(Widget::Button(L"�Q�[���ɖ߂�",384));
	m_gui.add(Widget::NewLine());
	m_gui.add(Widget::Button(L"���j���[�ɖ߂�",384));
	m_gui.add(Widget::NewLine());
	m_gui.add(Widget::Button(L"�I������",384));
	m_gui.add(Widget::NewLine());

	 const Vec2 screenCenter(640,360);
	 const Vec2 guiSize = m_gui.rect.size;
	 const Vec2 guiPos = screenCenter - guiSize/2;

	 m_gui.setPos(guiPos.x,guiPos.y);
	 m_gui.setVisible(false);

//	const Vec2 pos = Vec2(640,360)-m_gui.rect.size*0.5;
//	m_gui.setPos(pos.x,pos.y);
//	m_gui.setVisible(false);
}

bool PauseHelper::onPause() const
{
	return m_pausing;
}

void PauseHelper::update( optional<Application>& next, const Color& menuColor )
{
	if(!m_pausing)
	{
		return;
	}

	if(m_requestCapture)
	{
		m_textureBack.fill(Graphics::ReceiveScreenCapture());

		m_requestCapture = false;
	}

	++m_count;

	m_textureBack.resize(1280,720).draw();

	if(m_gui.button(L"���j���[�ɖ߂�").pushed)
	{
		/*m_gui.setVisible(false);
		m_pausing = false;*/
		next = Application::Title;
	}

	if(m_gui.button(L"�I������").pushed)
	{
		System::Exit();
	}

	/*
	m_textureBack.resize(1280,720).draw();

	Rect(0,0,1280,720).draw(Color(0,0,0,140));

	Rect(332,200,360,240).draw(menuColor);

	const Font& font = Resource::Font(L"Font.Common.Light18");

	rectButtonGame.drawFrame(1,1);
	rectButtonMenu.drawFrame(1,1);
	rectButtonExit.drawFrame(1,1);

	if(rectButtonGame.mouseOver)
		rectButtonGame.draw(Alpha(80));

	if(rectButtonMenu.mouseOver)
		rectButtonMenu.draw(Alpha(80));

	if(rectButtonExit.mouseOver)
		rectButtonExit.draw(Alpha(80));

	font.drawCenter(L"�Q�[���ɖ߂�",232);

	font.drawCenter(L"���j���[�ɖ߂�",302);

	font.drawCenter(L"�I������",372);

	if(rectButtonMenu.leftClicked)
	{
		next = Application::Title;
	}

	if(rectButtonExit.leftClicked)
	{
		System::Exit();
	}
	*/
}

bool PauseHelper::pauseStarted()
{
	if(m_pausing)
	{
		return false;
	}

	if(System::GetPreviousEvent() & WindowEvent::EscapeKey)
	{
		//m_gui.setActive(true);
		m_gui.setVisible(true);

		m_pausing = true;

		Graphics::RequestScreenCapture();

		m_requestCapture = true;

		m_count = 0;
	}

	return m_pausing;
}

bool PauseHelper::returnedToGame()
{
	if(!m_pausing)
	{
		return false;
	}

	//
	//	10 �t���[���ڈȍ~�� Escape �L�[��������邩�A
	//	[�Q�[���ɖ߂�] �{�^���������ꂽ��|�[�Y�𒆒f���Atrue ��Ԃ��B
	//
	if((m_count>10 && Input::KeyEscape.clicked) || m_gui.button(L"�Q�[���ɖ߂�").pushed)
	{
		//m_gui.setActive(false);
		m_gui.setVisible(false);
		m_pausing = false;
		return true;
	}

	return false;

	/*
	//
	//	10 �t���[���ڈȍ~�� Escape �L�[��������邩�A
	//	[�Q�[���ɖ߂�] �{�^���������ꂽ��|�[�Y�𒆒f���Atrue ��Ԃ��B
	//
	if((m_count>10 && Input::KeyEscape.clicked) || rectButtonGame.leftClicked)
	{
		//m_gui.setActive(false);
		m_gui.setVisible(false);
		m_pausing = false;
		return true;
	}

	return false;
	*/
}


