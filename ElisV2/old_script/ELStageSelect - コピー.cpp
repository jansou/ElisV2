# include "ELStageSelect.h"

namespace
{
	const int kR =130;

	const Circle kCirCave(46+291.0/2,91+226/2,kR);

	const Circle kCirPort(438+352/2,137+135.0/2,kR);

	const Circle kCirTemple(186+292/2,329+187.0/2,kR);

	const Circle kCirFortress(486+299.0/2,496+207.0/2,kR);

	const Circle kCirPalace(844+344/2,402+182/2,kR);

	const Circle kCirTower(828+423.0/2,26+293.0/2,kR);
};

ELStageSelect::ELStageSelect():
	m_clearedCave(false),
	m_clearedPort(false),
	m_clearedTemple(false),
	m_clearedFortress(false),
	m_clearedPalace(false),
	m_clearedTower(false),
	m_onPut(false),
	m_keyLeft(Input::MouseL)
{

}


bool ELStageSelect::init()
{
	/*
	TextureAsset::Register(L"texELcaveIcon",L"data/Elis/StageSelect/cave.png");
	TextureAsset::Register(L"texELfortressIcon",L"data/Elis/StageSelect/fortress.png");
	TextureAsset::Register(L"texELpalaceIcon",L"data/Elis/StageSelect/palace.png");
	TextureAsset::Register(L"texELportIcon",L"data/Elis/StageSelect/port.png");
	TextureAsset::Register(L"texELtempleIcon",L"data/Elis/StageSelect/temple.png");
	TextureAsset::Register(L"texELtowerIcon",L"data/Elis/StageSelect/tower.png");

	TextureAsset::Register(L"texELworldmap",L"data/Elis/StageSelect/worldmap.png");

	TextureAsset::Register(L"texELmapbit",L"data/Elis/StageSelect/mapbit.png");
	
	SoundAsset::Register(L"sndELstageselect",L"data/Elis/BGM/stageselect.mp3");
	SoundAsset::Register(L"sndELstage_choice",L"data/Elis/SE/System/stage_choice.mp3");
	*/
	
	TextureAsset::Register(L"texELcave_sign",L"data/Elis/StageSelect/sign/cave_sign.png");
	TextureAsset::Register(L"texELport_sign",L"data/Elis/StageSelect/sign/port_sign.png");
	TextureAsset::Register(L"texELtemple_sign",L"data/Elis/StageSelect/sign/temple_sign.png");
	TextureAsset::Register(L"texELfortress_sign",L"data/Elis/StageSelect/sign/fortress_sign.png");
	TextureAsset::Register(L"texELpalace_sign",L"data/Elis/StageSelect/sign/palace_sign.png");
	TextureAsset::Register(L"texELtower_sign",L"data/Elis/StageSelect/sign/tower_sign.png");

	return true;
}

bool ELStageSelect::loadSave()
{
	const CSVReader csv(L"data/Elis/Save/save.csv");

	if(!csv)
	{
		return false;
	}

	String Name;

	for(int i=0;;++i)
	{
		Name = csv.get<String>(i,0).lower();

		if(Name.isEmpty)
		{
			break;
		}

		//LOGFMT_ERR(loadName(Name));
		if(Name == L"cave")
		{
			m_clearedCave = csv.get<String>(i,1).lower() == L"true";
		}
		else if(Name == L"port")
		{
			m_clearedPort = csv.get<String>(i,1).lower() == L"true";
		}
		else if(Name == L"temple")
		{
			m_clearedTemple = csv.get<String>(i,1).lower() == L"true";
		}
		else if(Name == L"fortress")
		{
			m_clearedFortress = csv.get<String>(i,1).lower() == L"true";
		}
		else if(Name == L"palace")
		{
			m_clearedPalace = csv.get<String>(i,1).lower() == L"true";
		}
		else if(Name == L"tower")
		{
			m_clearedTower = csv.get<String>(i,1).lower() == L"true";
		}
	}

	return true;
}

bool ELStageSelect::update(const Point cursorpos)
{
	if(!SoundAsset(L"sndELstageselect").isPlaying)
		SoundAsset(L"sndELstageselect").play();

	if(kCirCave.intersects(cursorpos))
	{
		if(!m_onPut)
		{
			if(SoundAsset(L"sndELsystemButton").isPlaying) SoundAsset(L"sndELsystemButton").stop();

			SoundAsset(L"sndELsystemButton").play();

			m_onPut = true;
		}

		if(m_keyLeft.clicked)
		{
			m_mapName = L"cave";
			SoundAsset(L"sndELstage_choice").play();
			m_onPut = false;
			return true;
		}
	}
	else if(kCirPort.intersects(cursorpos) && m_clearedCave)
	{
		if(!m_onPut)
		{
			if(SoundAsset(L"sndELsystemButton").isPlaying) SoundAsset(L"sndELsystemButton").stop();

			SoundAsset(L"sndELsystemButton").play();

			m_onPut = true;
		}

		if(m_keyLeft.clicked)
		{
			m_mapName = L"port";
			SoundAsset(L"sndELstage_choice").play();
			m_onPut = false;
			return true;
		}
	}
	else if(kCirTemple.intersects(cursorpos) && m_clearedPort)
	{
		if(!m_onPut)
		{
			if(SoundAsset(L"sndELsystemButton").isPlaying) SoundAsset(L"sndELsystemButton").stop();

			SoundAsset(L"sndELsystemButton").play();

			m_onPut = true;
		}

		if(m_keyLeft.clicked)
		{
			m_mapName = L"temple";
			SoundAsset(L"sndELstage_choice").play();
			m_onPut = false;
			return true;
		}
	}
	else if(kCirFortress.intersects(cursorpos) && m_clearedTemple)
	{
		if(!m_onPut)
		{
			if(SoundAsset(L"sndELsystemButton").isPlaying) SoundAsset(L"sndELsystemButton").stop();

			SoundAsset(L"sndELsystemButton").play();

			m_onPut = true;
		}

		if(m_keyLeft.clicked)
		{
			m_mapName = L"fortress";
			SoundAsset(L"sndELstage_choice").play();
			m_onPut = false;
			return true;
		}
	}
	else if(kCirPalace.intersects(cursorpos) && m_clearedFortress)
	{
		if(!m_onPut)
		{
			if(SoundAsset(L"sndELsystemButton").isPlaying) SoundAsset(L"sndELsystemButton").stop();

			SoundAsset(L"sndELsystemButton").play();

			m_onPut = true;
		}

		if(m_keyLeft.clicked)
		{
			m_mapName = L"palace";
			SoundAsset(L"sndELstage_choice").play();
			m_onPut = false;
			return true;
		}
	}
	else if(kCirTower.intersects(cursorpos) && m_clearedPalace)
	{
		if(!m_onPut)
		{
			if(SoundAsset(L"sndELsystemButton").isPlaying) SoundAsset(L"sndELsystemButton").stop();

			SoundAsset(L"sndELsystemButton").play();
			
			m_onPut = true;
		}

		if(m_keyLeft.clicked)
		{
			m_mapName = L"tower";
			SoundAsset(L"sndELstage_choice").play();
			m_onPut = false;
			return true;
		}
	}
	else
	{
		m_onPut = false;
	}

	//return ELGameState::StageSelect;
	return false;
}

void ELStageSelect::draw(const Point &cursorpos)
{
	TextureAsset(L"texELworldmap").draw();

		///////////////////////////////////////////////
	//toPort
	TextureAsset(L"texELmapbit").drawAt(Point(342+25,284-14));
	TextureAsset(L"texELmapbit").drawAt(Point(342+53,284-22));
	TextureAsset(L"texELmapbit").drawAt(Point(342+81,284-30));

	//toFortress
	TextureAsset(L"texELmapbit").drawAt(Point(595-17,270+29));
	TextureAsset(L"texELmapbit").drawAt(Point(595-34,270+58));
	TextureAsset(L"texELmapbit").drawAt(Point(595-51,270+87));
	TextureAsset(L"texELmapbit").drawAt(Point(595-68,270+116));
	TextureAsset(L"texELmapbit").drawAt(Point(595-85,270+145));
	TextureAsset(L"texELmapbit").drawAt(Point(595-102,270+174));
	
	//toTemple
	TextureAsset(L"texELmapbit").drawAt(Point(315+24,518+22));
	TextureAsset(L"texELmapbit").drawAt(Point(315+24*2,518+22*2));
	TextureAsset(L"texELmapbit").drawAt(Point(315+24*3,518+22*3));
	TextureAsset(L"texELmapbit").drawAt(Point(315+24*4,518+22*4));
	TextureAsset(L"texELmapbit").drawAt(Point(315+24*5,518+22*5));
	TextureAsset(L"texELmapbit").drawAt(Point(315+24*6,518+22*6));

	//toPalace
	TextureAsset(L"texELmapbit").drawAt(Point(785+32,669-14));
	TextureAsset(L"texELmapbit").drawAt(Point(785+32*2,669-14*2));
	TextureAsset(L"texELmapbit").drawAt(Point(785+32*3,669-14*3));
	TextureAsset(L"texELmapbit").drawAt(Point(785+32*4,669-14*4));
	TextureAsset(L"texELmapbit").drawAt(Point(785+32*5,669-14*5));

	//toTower
	TextureAsset(L"texELmapbit").drawAt(Point(1016+2*3+5,506-30*3-20));
	TextureAsset(L"texELmapbit").drawAt(Point(1016+2*4+5,506-30*4-20));
	TextureAsset(L"texELmapbit").drawAt(Point(1016+2*5+5,506-30*5-20));
	//////////////////////////////////////////////////////////////////////////
		
	////////stageアイコンの表示////////////////
	//cave
	showCaveAnime(cursorpos);
	//port
	showPortAnime(cursorpos);
	//temple
	showTempleAnime(cursorpos);
	//fortress
	showFortressAnime(cursorpos);
	//palace
	showPalaceAnime(cursorpos);
	//tower
	showTowerAnime(cursorpos);
	///////////////////////////////////////////////
	
}

void ELStageSelect::showCaveAnime(const Point &cursorpos)
{
	
	static int frameCount = 0;

	if(kCirCave.intersects(cursorpos))
	{
		const int animeframe = 4;
		const int maxframe = animeframe*2;
		const int a=200;
		kCirCave.draw(Color(255,255,255,a));
		
		frameCount = Min(++frameCount, maxframe);

		/*
		TextureAsset(L"texELcave_sign")
			(0
			,126*(frameCount/animeframe)
			,132
			,126)
			.draw(333+132,184);
			
		
		TextureAsset(L"texELcave_sign")
			(0
			,126*2
			,132
			,126)
			.draw(333+264,184+(maxframe*4-frameCount*4));
		*/
		TextureAsset(L"texELcaveIcon")(0,226*kCirCave.intersects(cursorpos),291,226).draw(46,91);

		const Point pos(317,208);
		TextureAsset(L"texELcave_sign")
			(0
			,126
			,132
			,126)
			.draw
			(pos.x+((frameCount == maxframe) ? 0 : ((frameCount % 4 >= 2) ? frameCount/2 : -frameCount/2))
			,pos.y);
			
	}
	else
	{
		TextureAsset(L"texELcaveIcon")(0,226*kCirCave.intersects(cursorpos),291,226).draw(46,91);

		frameCount = 0;
	}

}

void ELStageSelect::showPortAnime(const Point &cursorpos)
{	
	static int frameCount = 0;

	if(kCirPort.intersects(cursorpos))
	{
		const int animeframe = 4;
		const int maxframe = animeframe*2;
		const int a=200;
		if(kCirPort.intersects(cursorpos) && m_clearedCave)
			kCirPort.draw(Color(255,255,255,a));
		
		frameCount = Min(++frameCount, maxframe);

		if(m_clearedCave)
			TextureAsset(L"texELportIcon")(0,135*kCirPort.intersects(cursorpos),352,135).draw(438,137);

		const Point pos(744,206);

		TextureAsset(L"texELport_sign")
			(0
			,126
			,132
			,126)
			.draw
			(pos.x+((frameCount == maxframe) ? 0 : ((frameCount % 4 >= 2) ? frameCount/2 : -frameCount/2))
			,pos.y);
			
	}
	else
	{
		if(m_clearedCave)
			TextureAsset(L"texELportIcon")(0,135*kCirPort.intersects(cursorpos),352,135).draw(438,137);

		frameCount = 0;
	}
}

void ELStageSelect::showTempleAnime(const Point &cursorpos)
{	
	static int frameCount = 0;

	if(kCirTemple.intersects(cursorpos))
	{
		const int animeframe = 4;
		const int maxframe = animeframe*2;
		const int a=200;

		if(kCirTemple.intersects(cursorpos) && m_clearedPort)
			kCirTemple.draw(Color(255,255,255,a));
		
		frameCount = Min(++frameCount, maxframe);

		if(m_clearedPort)
			TextureAsset(L"texELtempleIcon")(0,187*kCirTemple.intersects(cursorpos),292,187).draw(186,329);

		const Point pos(444,397);

		TextureAsset(L"texELtemple_sign")
			(0
			,126
			,132
			,126)
			.draw
			(pos.x+((frameCount == maxframe) ? 0 : ((frameCount % 4 >= 2) ? frameCount/2 : -frameCount/2))
			,pos.y);
			
	}
	else
	{
		if(m_clearedPort)
			TextureAsset(L"texELtempleIcon")(0,187*kCirTemple.intersects(cursorpos),292,187).draw(186,329);

		frameCount = 0;
	}
}

void ELStageSelect::showFortressAnime(const Point &cursorpos)
{	
	static int frameCount = 0;

	if(kCirFortress.intersects(cursorpos))
	{
		const int animeframe = 4;
		const int maxframe = animeframe*2;
		const int a=200;

		frameCount = Min(++frameCount, maxframe);

		if(kCirFortress.intersects(cursorpos) && m_clearedPort)
			kCirFortress.draw(Color(255,255,255,a));

		if(m_clearedTemple)
			TextureAsset(L"texELfortressIcon")(0,207*kCirFortress.intersects(cursorpos),299,207).draw(486,496);

		const Point pos(773,576);

		TextureAsset(L"texELfortress_sign")
			(0
			,126
			,132
			,126)
			.draw
			(pos.x+((frameCount == maxframe) ? 0 : ((frameCount % 4 >= 2) ? frameCount/2 : -frameCount/2))
			,pos.y);
			
	}
	else
	{
		if(m_clearedTemple)
			TextureAsset(L"texELfortressIcon")(0,207*kCirFortress.intersects(cursorpos),299,207).draw(486,496);

		frameCount = 0;
	}
}

void ELStageSelect::showPalaceAnime(const Point &cursorpos)
{	
	static int frameCount = 0;

	if(kCirPalace.intersects(cursorpos))
	{
		const int animeframe = 4;
		const int maxframe = animeframe*2;
		const int a=200;

		if(kCirPalace.intersects(cursorpos) && m_clearedFortress)
			kCirPalace.draw(Color(255,255,255,a));

		if(m_clearedFortress)
			TextureAsset(L"texELpalaceIcon")(0,182*kCirPalace.intersects(cursorpos),344,182).draw(844,402);

		frameCount = Min(++frameCount, maxframe);
		const Point pos(1155,468);

		TextureAsset(L"texELpalace_sign")
			(0
			,126
			,132
			,126)
			.draw
			(pos.x+((frameCount == maxframe) ? 0 : ((frameCount % 4 >= 2) ? frameCount/2 : -frameCount/2))
			,pos.y);
			
	}
	else
	{
		if(m_clearedFortress)
			TextureAsset(L"texELpalaceIcon")(0,182*kCirPalace.intersects(cursorpos),344,182).draw(844,402);

		frameCount = 0;
	}
}

void ELStageSelect::showTowerAnime(const Point &cursorpos)
{	
	static int frameCount = 0;

	if(kCirTower.intersects(cursorpos))
	{
		const int animeframe = 4;
		const int maxframe = animeframe*2;
		const int a=200;

		if(kCirTower.intersects(cursorpos) && m_clearedPalace)
			kCirTower.draw(Color(255,255,255,a));

		if(m_clearedPalace)
			TextureAsset(L"texELtowerIcon")(0,293*kCirTower.intersects(cursorpos),423,293).draw(828,26);
	
		frameCount = Min(++frameCount, maxframe);
		const Point pos(1140,198);

		TextureAsset(L"texELtower_sign")
			(0
			,126
			,132
			,126)
			.draw
			(pos.x+((frameCount == maxframe) ? 0 : ((frameCount % 4 >= 2) ? frameCount/2 : -frameCount/2))
			,pos.y);
			
	}
	else
	{
		if(m_clearedPalace)
			TextureAsset(L"texELtowerIcon")(0,293*kCirTower.intersects(cursorpos),423,293).draw(828,26);
	
		frameCount = 0;
	}
}

void ELStageSelect::releaseData()
{
	TextureAsset::Release(L"texELworldmap");

	TextureAsset::Release(L"texELcaveIcon");

	TextureAsset::Release(L"texELportIcon");

	TextureAsset::Release(L"texELtempleIcon");

	TextureAsset::Release(L"texELfortressIcon");

	TextureAsset::Release(L"texELpalaceIcon");

	TextureAsset::Release(L"texELtowerIcon");

	SoundAsset(L"sndELstageselect").stop();

	SoundAsset::Release(L"sndELstageselect");
}