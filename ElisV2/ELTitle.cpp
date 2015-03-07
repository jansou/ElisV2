# include "ELTitle.h"

namespace
{
	Rect kButtonRect(0,0,178,40);

	const Point kConfigPos(355,29);

	const Rect kConfRightRect(Point(323,105)+kConfigPos,138,46);
	const Rect kConfLeftRect(Point(110,105)+kConfigPos,138,46);

	const Rect kConfSelectRect(Point(450,594)+kConfigPos,76,37);
	//const Rect kConfBackRect(Point(328,594)+kConfigPos,76,37);


};

ELTitle::ELTitle():
	m_MouseLeft(Input::MouseL)
	,m_onPut(false)
	,m_titleMode(ELTitleMode::Title)
	,m_isRightHand(true)
	,m_pressed(false)
{

}

void ELTitle::init()
{
	TextureAsset::Register(L"texElisTitle",L"data/Elis/Title/title.png");
	SoundAsset::Register(L"sndElisTitle",L"data/Elis/BGM/title.mp3");
	SoundAsset::Register(L"sndElisPushstart",L"data/Elis/SE/System/pushstart.mp3");

	TextureAsset::Register(L"texElisNewgame",L"data/Elis/Title/newgame.png");
	TextureAsset::Register(L"texElisContinue",L"data/Elis/Title/continue.png");
	TextureAsset::Register(L"texElisConfig",L"data/Elis/Title/config.png");
	SoundAsset::Register(L"sndELsystemButton",L"data/Elis/SE/System/systemButton.mp3");
	//////////////keyconfig用
	TextureAsset::Register(L"texELconfigbase",L"data/Elis/Title/config/configbase.png");
	TextureAsset::Register(L"texELconfigkey",L"data/Elis/Title/config/key.png");
	TextureAsset::Register(L"texELconfig_select",L"data/Elis/Title/config/button_select.png");
	TextureAsset::Register(L"texELconfig_right",L"data/Elis/Title/config/button_right.png");
	TextureAsset::Register(L"texELconfig_left",L"data/Elis/Title/config/button_left.png");
	TextureAsset::Register(L"texELconfig_select",L"data/Elis/Title/config/button_select.png");
	TextureAsset::Register(L"texELconfig_back",L"data/Elis/Title/config/button_back.png");

}

bool ELTitle::update(const Point& cursorpos)
{
	if(!SoundAsset(L"sndElisTitle").isPlaying)	SoundAsset(L"sndElisTitle").play();

	switch (m_titleMode)
	{
	case ELTitleMode::Title:
		{
			return updateTitle(cursorpos);
			break;
		}
	case ELTitleMode::Config:
		{
			return updateConfig(cursorpos);
			break;
		}
	case ELTitleMode::null:
		break;
	default:
		break;
	}

	return false;
}

bool ELTitle::updateConfig(const Point &cursorpos)
{
	if(m_pressed)
	{
		m_pressed = false;
		m_titleMode = ELTitleMode::Title;
		return false;
	}

	if(cursorpos.intersects(kConfSelectRect))
	{///////決定押したら
		if(!m_onPut)
		{
			if(SoundAsset(L"sndELsystemButton").isPlaying) SoundAsset(L"sndELsystemButton").stop();

			SoundAsset(L"sndELsystemButton").play();
			m_onPut = true;
		}

		if(m_MouseLeft.clicked)
		{
			m_onPut = false;
			if(SoundAsset(L"sndElisPushstart").isPlaying) SoundAsset(L"sndElisPushstart").stop();

			SoundAsset(L"sndElisPushstart").play();
			
			m_pressed = true;

			return true;
		}
	}
	else if(cursorpos.intersects(kConfLeftRect))
	{////////左利き
		if(!m_onPut)
		{
			if(SoundAsset(L"sndELsystemButton").isPlaying) SoundAsset(L"sndELsystemButton").stop();

			SoundAsset(L"sndELsystemButton").play();
			m_onPut = true;
		}

		if(m_MouseLeft.clicked)
		{
			m_onPut = false;
			if(SoundAsset(L"sndElisPushstart").isPlaying) SoundAsset(L"sndElisPushstart").stop();

			SoundAsset(L"sndElisPushstart").play();

			m_isRightHand = false;
		}
	}
	else if(cursorpos.intersects(kConfRightRect))
	{///////////////////////右利き
		if(!m_onPut)
		{
			if(SoundAsset(L"sndELsystemButton").isPlaying) SoundAsset(L"sndELsystemButton").stop();

			SoundAsset(L"sndELsystemButton").play();
			m_onPut = true;
		}

		if(m_MouseLeft.clicked)
		{
			m_onPut = false;
			if(SoundAsset(L"sndElisPushstart").isPlaying) SoundAsset(L"sndElisPushstart").stop();

			SoundAsset(L"sndElisPushstart").play();

			m_isRightHand = true;
		}
	}
	else
	{
		m_onPut = false;
	}

	return false;
}

bool ELTitle::updateTitle(const Point &cursorpos)
{
	if(cursorpos.intersects(Rect(kButtonRect.pos+Point(640-89,410-20),kButtonRect.size)))
	{//newgame
		if(!m_onPut)
		{
			if(SoundAsset(L"sndELsystemButton").isPlaying) SoundAsset(L"sndELsystemButton").stop();

			SoundAsset(L"sndELsystemButton").play();
			m_onPut = true;
		}

		if(m_MouseLeft.clicked)
		{
			if(SoundAsset(L"sndELsystemButton").isPlaying) SoundAsset(L"sndELsystemButton").stop();

			SoundAsset(L"sndELsystemButton").play();
			
			m_onPut = false;

			return true;
		}
	}
	else if(cursorpos.intersects(Rect(kButtonRect.pos+Point(640-89,480-20),kButtonRect.size)))
	{//continue
		if(!m_onPut)
		{
			if(SoundAsset(L"sndELsystemButton").isPlaying) SoundAsset(L"sndELsystemButton").stop();

			SoundAsset(L"sndELsystemButton").play();
			m_onPut = true;
		}

		if(m_MouseLeft.clicked)
		{
			if(SoundAsset(L"sndElisPushstart").isPlaying) SoundAsset(L"sndElisPushstart").stop();

			SoundAsset(L"sndElisPushstart").play();
			m_nextState = ELGameState::StageSelect;
			m_onPut = false;
			return true;
		}
	}
	else if(cursorpos.intersects(Rect(kButtonRect.pos+Point(640-89,550-20),kButtonRect.size)))
	{//config
		if(!m_onPut)
		{
			
			if(SoundAsset(L"sndELsystemButton").isPlaying) SoundAsset(L"sndELsystemButton").stop();

			SoundAsset(L"sndELsystemButton").play();
			m_onPut = true;
		}

		if(m_MouseLeft.clicked)
		{
			if(SoundAsset(L"sndElisPushstart").isPlaying) SoundAsset(L"sndElisPushstart").stop();

			SoundAsset(L"sndElisPushstart").play();

			m_titleMode = ELTitleMode::Config;

			m_onPut = false;
		}
	}
	else
	{
		m_onPut = false;
	}

	return false;
}

void ELTitle::draw(const Point& cursorpos)
{
	TextureAsset(L"texElisTitle").draw();

	switch(m_titleMode)
	{
	case ELTitleMode::Title:
		{
			TextureAsset(L"texElisNewgame")(0,40*cursorpos.intersects(Rect(kButtonRect.pos+Point(640-89,410-20),kButtonRect.size)),178,40).drawAt(640,410);//中心座標410,480,550
			TextureAsset(L"texElisContinue")(0,40*cursorpos.intersects(Rect(kButtonRect.pos+Point(640-89,480-20),kButtonRect.size)),178,40).drawAt(640,480);
			TextureAsset(L"texElisConfig")(0,40*cursorpos.intersects(Rect(kButtonRect.pos+Point(640-89,550-20),kButtonRect.size)),178,40).drawAt(640,550);
			break;
		}
	case ELTitleMode::Config:
		{
			/*
			configbase  :355,29,570,662
			button_left :110,105,138,46
			button_right:323,105,138,46
			key         :0,170,571,299
			button_enter:450,594,76,37

			決定  :63,594,206,37
			戻る	    :328,594,76,37
			*/

			TextureAsset(L"texELconfigbase").draw(kConfigPos);


			if(m_isRightHand)
				kConfRightRect.draw(Color(255,255,255,155));
			else
				kConfLeftRect.draw(Color(255,255,255,155));

			TextureAsset(L"texELconfigkey")
				(0,299*m_isRightHand
				,571,299).draw(Point(0,170)+kConfigPos);
			
			TextureAsset(L"texELconfig_right")
				(0,46*cursorpos.intersects(kConfRightRect)
				,138,46).draw(kConfRightRect.pos);
			TextureAsset(L"texELconfig_left")
				(0,46*cursorpos.intersects(kConfLeftRect),138,46).draw(kConfLeftRect.pos);

			TextureAsset(L"texELconfig_select")
				(0,37*cursorpos.intersects(kConfSelectRect)
				,kConfSelectRect.size.x
				,kConfSelectRect.size.y).draw(kConfSelectRect.pos);
			/*
			TextureAsset(L"texELconfig_back")
				(0,37*cursorpos.intersects(kConfBackRect)
				,kConfBackRect.size.x
				,kConfBackRect.size.y).draw(kConfBackRect.pos);
			*/
			break;
		}
	default:
		break;
	}


	
}

void ELTitle::rereaseTex()
{
	
	SoundAsset(L"sndElisTitle").stop();
	TextureAsset::Release(L"texElisTitle");
	SoundAsset::Release(L"sndElisTitle");
	
}