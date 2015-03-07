# include "ELUI.h"

ELUI::ELUI()
	:m_paused(false)
	,m_choicedMagic(ElisMagicType::Normal)
	,m_keyRight(Input::MouseR)
	,m_keyLeft(Input::MouseL)
	,m_onPut(false)
	,m_keyShift(Input::KeyShift)
	,m_keySpace(Input::KeySpace)
	,m_menustate(ELMenuState::Base)
{
	/*
	TextureAsset::Register(L"ElisUIbase",L"data/Elis/UI/UIbase.png");
	TextureAsset::Register(L"ElisUIgem",L"data/Elis/UI/UIgem.png");

	TextureAsset::Register(L"ElBossUIbase",L"data/Elis/UI/bossUIbase.png");
	TextureAsset::Register(L"ElBossUIgem",L"data/Elis/UI/bossUIgem.png");


	////////////////////////////menu用
	TextureAsset::Register(L"texELmenu_base",L"data/Elis/UI/menu_base.png");
	TextureAsset::Register(L"texELmenu_pausemenu",L"data/Elis/UI/pausemenu.png");
	TextureAsset::Register(L"texELmenu_gameover",L"data/Elis/UI/gameover.png");
	TextureAsset::Register(L"texELmenu_askBackStage",L"data/Elis/UI/askBackStage.png");

	TextureAsset::Register(L"texELmenu_retry",L"data/Elis/UI/retry.png");
	TextureAsset::Register(L"texELmenu_backTitle",L"data/Elis/UI/backTitle.png");
	TextureAsset::Register(L"texELmenu_yes",L"data/Elis/UI/yes.png");
	TextureAsset::Register(L"texELmenu_no",L"data/Elis/UI/no.png");

	TextureAsset::Register(L"texElpausemenu1",L"data/Elis/UI/pausemenu1.png");
	TextureAsset::Register(L"texElbackStage",L"data/Elis/UI/backStage.png");

	////////////////////////////////////////////

	TextureAsset::Register(L"texELmagicBase",L"data/Elis/UI/magicBase.png");
	TextureAsset::Register(L"texELmagicIcon",L"data/Elis/UI/magicIcon.png");

	SoundAsset::Register(L"elisMagic_change",L"data/Elis/SE/System/magic_change.mp3");
	*/

	TextureAsset::Register(L"texElisUIbase2", L"data/Elis/UI/UIBase2.png");
	TextureAsset::Register(L"texElisUIgem2", L"data/Elis/UI/mini.png");
}

namespace
{
	const Point kdrawPos(10,30);

	const Point kbossDrawPos(960,30);

	const useMP kUseMP;

	const int kMPStone = kUseMP.MPStone;//25;
	const int kMPIce = kUseMP.MPIce;//15;
	const int kMPNormal = kUseMP.MPNormal;//10;
	const int kMPFire = kUseMP.MPFire;//25;
	const int kMPWind = kUseMP.MPWind;//15;

	const Rect kPauseRect(480,270,320,179);
	const Rect kMenuTitleRect(27,22,265,26);
	const Rect kPauseRect1(27,72,265,24);
	const Rect kPauseRect2(27,118,265,24);

	const Point kMagicBasePos(1280/2,720/2);

	const int kMagicR = 32;

	const Rect kMagicRect(64,64);

	const int kSelectMagicSize = 5;

	const SelectMagicIcon kMagicIcons[kSelectMagicSize] =
	{
		SelectMagicIcon(Circle(Point(1,-129)+kMagicBasePos,kMagicR),ElisMagicType::Normal),//通常
		SelectMagicIcon(Circle(Point(122,-39)+kMagicBasePos,kMagicR),ElisMagicType::Fire),//火
		SelectMagicIcon(Circle(Point(76,105)+kMagicBasePos,kMagicR),ElisMagicType::Ice),//氷
		SelectMagicIcon(Circle(Point(-75,105)+kMagicBasePos,kMagicR),ElisMagicType::Stone),//石
		SelectMagicIcon(Circle(Point(-122,-39)+kMagicBasePos,kMagicR),ElisMagicType::Wind)//風
	};
};

ELGameState ELUI::pauseUpdate(const Point &cursorpos)
{
	if(Rect(kPauseRect.pos + kPauseRect1.pos,kPauseRect1.size).intersects(cursorpos))
	{
		if(!m_onPut)
		{
			if(SoundAsset(L"sndELsystemButton").isPlaying) SoundAsset(L"sndELsystemButton").stop();

			SoundAsset(L"sndELsystemButton").play();

			m_onPut = true;
		}

		if(m_keyLeft.clicked)
		{
			if(SoundAsset(L"sndElisPushstart").isPlaying) SoundAsset(L"sndElisPushstart").stop();

			SoundAsset(L"sndElisPushstart").play();

			m_paused = false;

			return ELGameState::Play;
		}
	}
	else if(Rect(kPauseRect.pos + kPauseRect2.pos,kPauseRect2.size).intersects(cursorpos))
	{
		if(!m_onPut)
		{
			if(SoundAsset(L"sndELsystemButton").isPlaying) SoundAsset(L"sndELsystemButton").stop();

			SoundAsset(L"sndELsystemButton").play();

			m_onPut = true;
		}

		if(m_keyLeft.clicked)
		{
			if(SoundAsset(L"sndElisPushstart").isPlaying) SoundAsset(L"sndElisPushstart").stop();

			SoundAsset(L"sndElisPushstart").play();

			m_paused = false;

		return ELGameState::StageSelect;
		}
		
	}
	else if(m_keySpace.clicked)
	{
		if(SoundAsset(L"sndElisPushstart").isPlaying) SoundAsset(L"sndElisPushstart").stop();

		SoundAsset(L"sndElisPushstart").play();

		m_paused = false;

		return ELGameState::Play;
	}
	else
	{
		m_onPut = false;
	}

	return ELGameState::Play;
}

ELPlayState ELUI::update(const ElisMagicType &magicType)
{
	if(m_keySpace.clicked)
	{
		m_paused = true;
	}
	else if(/*m_keyShift.pressed && */m_keyRight.clicked)
	{
		if(SoundAsset(L"elisMagic_change").isPlaying)	SoundAsset(L"elisMagic_change").stop();

		SoundAsset(L"elisMagic_change").play();

		switch (magicType)
		{
		case ElisMagicType::Normal:
			m_choicedMagic = ElisMagicType::Ice;
			break;
		case ElisMagicType::Ice:
			m_choicedMagic = ElisMagicType::Stone;
			break;
		case ElisMagicType::Stone:
			m_choicedMagic = ElisMagicType::Wind;
			break;
		case ElisMagicType::Wind:
			m_choicedMagic = ElisMagicType::Fire;
			break;
		case ElisMagicType::Fire:
			m_choicedMagic = ElisMagicType::Normal;
			break;
		default:
			break;
		}
	}
	/*
	else if(m_keyRight.clicked)
	{	
		m_choicedMagic = magicType;
		return ELPlayState::EditMagic;
	}
	else if(Input::Key1.clicked)
	{
		m_choicedMagic = ElisMagicType::Normal;
		if(SoundAsset(L"elisMagic_change").isPlaying)	SoundAsset(L"elisMagic_change").stop();

		SoundAsset(L"elisMagic_change").play();
	}
	else if(Input::Key2.clicked)
	{
		if(SoundAsset(L"elisMagic_change").isPlaying)	SoundAsset(L"elisMagic_change").stop();

		SoundAsset(L"elisMagic_change").play();

		m_choicedMagic = ElisMagicType::Ice;
	}
	else if(Input::Key3.clicked)
	{
		if(SoundAsset(L"elisMagic_change").isPlaying)	SoundAsset(L"elisMagic_change").stop();

		SoundAsset(L"elisMagic_change").play();

		m_choicedMagic = ElisMagicType::Stone;
	}
	else if(Input::Key4.clicked)
	{
		if(SoundAsset(L"elisMagic_change").isPlaying)	SoundAsset(L"elisMagic_change").stop();

		SoundAsset(L"elisMagic_change").play();

		m_choicedMagic = ElisMagicType::Wind;
	}
	else if(Input::Key5.clicked)
	{
		if(SoundAsset(L"elisMagic_change").isPlaying)	SoundAsset(L"elisMagic_change").stop();

		SoundAsset(L"elisMagic_change").play();

		m_choicedMagic = ElisMagicType::Fire;
	}
	*/
	return ELPlayState::Playing;
}

ELPlayState ELUI::editMagicUpdate(const Point &cursorpos)
{
	if(m_keyRight.clicked)
	{
		m_onPut = false;
		return ELPlayState::Playing;
	}
	
	bool flag = false;

	for(int i=0;i<kSelectMagicSize;++i)
	{
		
		if(cursorpos.intersects(kMagicIcons[i].cir))
		{
			flag = true;

			if(!m_onPut)
			{
				if(SoundAsset(L"sndELsystemButton").isPlaying) SoundAsset(L"sndELsystemButton").stop();

				SoundAsset(L"sndELsystemButton").play();

				m_onPut = true;
			}

			if(m_keyLeft.clicked)
			{
				if(SoundAsset(L"elisMagic_change").isPlaying)	SoundAsset(L"elisMagic_change").stop();

				SoundAsset(L"elisMagic_change").play();

				m_choicedMagic = kMagicIcons[i].magicType;
			}
		}
	}

	if(!flag)
		m_onPut = false;

	return ELPlayState::EditMagic;
}

bool ELUI::updateGameover(const Point &cursorpos)
{
	switch (m_menustate)
	{
	case ELMenuState::Base:
		if(Rect(kPauseRect.pos + kPauseRect1.pos,kPauseRect1.size).intersects(cursorpos))
		{//リトライ
			if(!m_onPut)
			{
				if(SoundAsset(L"sndELsystemButton").isPlaying) SoundAsset(L"sndELsystemButton").stop();

				SoundAsset(L"sndELsystemButton").play();

				m_onPut = true;
			}

			if(m_keyLeft.clicked)
			{
				if(SoundAsset(L"sndElisPushstart").isPlaying) SoundAsset(L"sndElisPushstart").stop();

				SoundAsset(L"sndElisPushstart").play();

				m_menustate = ELMenuState::Retry;
				m_onPut = false;
				return true;
			}
		}
		else if(Rect(kPauseRect.pos + kPauseRect2.pos,kPauseRect2.size).intersects(cursorpos))
		{//ステージへ戻る
			if(!m_onPut)
			{
				if(SoundAsset(L"sndELsystemButton").isPlaying) SoundAsset(L"sndELsystemButton").stop();

				SoundAsset(L"sndELsystemButton").play();

				m_onPut = true;
			}

			if(m_keyLeft.clicked)
			{
				if(SoundAsset(L"sndElisPushstart").isPlaying) SoundAsset(L"sndElisPushstart").stop();

				SoundAsset(L"sndElisPushstart").play();

				m_onPut = false;
				m_menustate = ELMenuState::Ask;
				return false;
			}
		}
		else{m_onPut = false;}
		break;
	case ELMenuState::Ask:
		if(Rect(kPauseRect.pos + kPauseRect1.pos,kPauseRect1.size).intersects(cursorpos))
		{//はい
			if(!m_onPut)
			{
				if(SoundAsset(L"sndELsystemButton").isPlaying) SoundAsset(L"sndELsystemButton").stop();

				SoundAsset(L"sndELsystemButton").play();

				m_onPut = true;
			}

			if(m_keyLeft.clicked)
			{
				if(SoundAsset(L"sndElisPushstart").isPlaying) SoundAsset(L"sndElisPushstart").stop();

				SoundAsset(L"sndElisPushstart").play();

				m_menustate = ELMenuState::Accept;
				m_onPut = false;
				return true;
			}
		}
		else if(Rect(kPauseRect.pos + kPauseRect2.pos,kPauseRect2.size).intersects(cursorpos))
		{//いいえ
			
			if(!m_onPut)
			{
				if(SoundAsset(L"sndELsystemButton").isPlaying) SoundAsset(L"sndELsystemButton").stop();

				SoundAsset(L"sndELsystemButton").play();

				m_onPut = true;
			}

			if(m_keyLeft.clicked)
			{
				if(SoundAsset(L"sndElisPushstart").isPlaying) SoundAsset(L"sndElisPushstart").stop();

				SoundAsset(L"sndElisPushstart").play();

				m_onPut = false;

				m_menustate = ELMenuState::Base;
				return false;
			}
		}
		else{m_onPut = false;}
		break;
	case ELMenuState::null:
		break;
	default:
		break;
	}
	

	return false;
}

void ELUI::draw(const ElisMagicType &magicType, int life, int MP) const
{
	int gemI=-1;
	//bool useable;

	TextureAsset(L"texElisUIbase2").draw(kdrawPos);

	switch (magicType)
	{
	case ElisMagicType::Normal:
		gemI = 0;
		//useable = MP > kMPNormal;
		break;
	case ElisMagicType::Fire:
		gemI = 4;
		//useable = MP > kMPFire;
		break;
	case ElisMagicType::Ice:
		gemI = 1;
		//useable = MP > kMPIce;
		break;
	case ElisMagicType::Stone:
		gemI = 2;
		//useable = MP > kMPStone;
		break;
	case ElisMagicType::Wind:
		gemI = 3;
		//useable = MP > kMPWind;
		break;
	}

	TextureAsset(L"ElisUIgem")(0, 66 * gemI, 66, 66).drawAt(kdrawPos + Point(43, 40));

	//小さいとこ
	for (int i = gemI; i < 5 + gemI; ++i)
	{
		if (i != gemI)
		{
			int n = i;
			if (n >= 5)
			{
				n -= 5;
			}
			TextureAsset(L"texElisUIgem2")(0, 27 * n, 28, 27).draw(kdrawPos + Point(58 + 29 * (i - gemI), 44));
		}
	}

	//TextureAsset(L"ElisUIbase").draw(kdrawPos);

	Rect(kdrawPos+Point(85,11),int(210*double(life/20.0)),27).draw(Color(201,27,68));

	Rect(kdrawPos + Point(85, 44), int(210 * double(MP / 100.0)), 27).draw(Color(97, 143, 123));

	/*
	if(useable)
		Rect(kdrawPos+Point(85,44),int(210*double(MP/100.0)),27).draw(Color(97,143,123));
	else
		Rect(kdrawPos+Point(85,44),int(210*double(MP/100.0)),27).draw(Color(116,116,116));
	*/


}

void ELUI::bossDraw(const ElisMagicType &magicType, int life, int maxlife) const
{
	int gemI;

	switch (magicType)
	{
	case ElisMagicType::Normal:
		gemI = 0;
		break;
	case ElisMagicType::Fire:
		gemI = 1;
		break;
	case ElisMagicType::Ice:
		gemI = 2;
		break;
	case ElisMagicType::Stone:
		gemI = 3;
		break;
	case ElisMagicType::Wind:
		gemI = 4;
		break;
	default:
		gemI = -1;
		break;
	}

	TextureAsset(L"ElBossUIbase").draw(kbossDrawPos);

	if(gemI != -1)
		TextureAsset(L"ElBossUIgem")(0,66*gemI,66,66).drawAt(kbossDrawPos+Point(268,40));

	Rect(kbossDrawPos+Point(14,44),int(211*double(life*1.0/maxlife*1.0)),27).draw(Color(120,169,155));

}

void ELUI::pauseDraw(const Point &cursorpos)
{
	Rect(0,0,1280,720).draw(Color(0,0,0,100));

	TextureAsset(L"texELmenu_base").draw(kPauseRect.pos);
	TextureAsset(L"texELmenu_pausemenu").draw(kPauseRect.pos+kMenuTitleRect.pos);
	TextureAsset(L"texElpausemenu1").draw(kPauseRect.pos+kPauseRect1.pos);
	TextureAsset(L"texElbackStage").draw(kPauseRect.pos+kPauseRect2.pos);

	if(Rect(kPauseRect.pos + kPauseRect1.pos,kPauseRect1.size).intersects(cursorpos))
	{
		Rect(kPauseRect.pos + kPauseRect1.pos,kPauseRect1.size).draw(Color(255,255,255,155));
	}
	else if(Rect(kPauseRect.pos + kPauseRect2.pos,kPauseRect2.size).intersects(cursorpos))
	{
		Rect(kPauseRect.pos + kPauseRect2.pos,kPauseRect2.size).draw(Color(255,255,255,155));
	}
}

void ELUI::editMagicDraw(const Point &cursorpos)
{
	Rect(0,0,1280,720).draw(Color(0,0,0,200));
	TextureAsset(L"texELmagicBase").drawAt(kMagicBasePos);

	Circle(kMagicBasePos,kMagicR).draw(Color(0,0,0,155));

	for(size_t i=0;i<kSelectMagicSize;++i)
	{
		if(m_choicedMagic == kMagicIcons[i].magicType)
		{
			const Circle cir = kMagicIcons[i].cir;
			Circle(cir.center,cir.r-5).draw(Color(0,0,0,155));
		}

		TextureAsset(L"texELmagicIcon")
			(kMagicRect.w*cursorpos.intersects(kMagicIcons[i].cir),i*kMagicRect.h,kMagicRect.w,kMagicRect.h)
			.drawAt(kMagicIcons[i].cir.center);

		if(m_choicedMagic == kMagicIcons[i].magicType)
			TextureAsset(L"texELmagicIcon")(0,i*kMagicRect.h,kMagicRect.w,kMagicRect.h)
			.drawAt(kMagicBasePos);
	}
}

void ELUI::gameoverDraw(const Point &cursorpos)
{
	Rect(0,0,1280,720).draw(Color(0,0,0,100));

	TextureAsset(L"texELmenu_base").draw(kPauseRect.pos);

	switch (m_menustate)
	{
	case ELMenuState::Base:
		TextureAsset(L"texELmenu_gameover").draw(kPauseRect.pos+kMenuTitleRect.pos);
		TextureAsset(L"texELmenu_retry").draw(kPauseRect.pos+kPauseRect1.pos);
		TextureAsset(L"texElbackStage").draw(kPauseRect.pos+kPauseRect2.pos);
		break;
	case ELMenuState::Ask:
		TextureAsset(L"texELmenu_askBackStage").draw(kPauseRect.pos+kMenuTitleRect.pos);
		TextureAsset(L"texELmenu_yes").draw(kPauseRect.pos+kPauseRect1.pos);
		TextureAsset(L"texELmenu_no").draw(kPauseRect.pos+kPauseRect2.pos);
		break;
	case ELMenuState::Retry:
		Rect(1280,720).draw(Palette::Black);
		break;
	case ELMenuState::Accept:
		Rect(1280,720).draw(Palette::Black);
		break;
	case ELMenuState::null:
		break;
	default:
		break;
	}

	if(Rect(kPauseRect.pos + kPauseRect1.pos,kPauseRect1.size).intersects(cursorpos))
	{
		Rect(kPauseRect.pos + kPauseRect1.pos,kPauseRect1.size).draw(Color(255,255,255,155));
	}
	else if(Rect(kPauseRect.pos + kPauseRect2.pos,kPauseRect2.size).intersects(cursorpos))
	{
		Rect(kPauseRect.pos + kPauseRect2.pos,kPauseRect2.size).draw(Color(255,255,255,155));
	}
}