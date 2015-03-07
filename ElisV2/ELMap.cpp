#include "ELMap.h"

ELMap::ELMap() :m_windowW(1280), m_windowH(720), m_bgmPlayed(false)
{
	SoundAsset::Register(L"ELSEdoor",L"data/Elis/SE/System/door.mp3");
	SoundAsset::Register(L"ELBGMtower",L"data/Elis/BGM/tower.mp3");
	SoundAsset::Register(L"ELBGMpalace",L"data/Elis/BGM/palace.mp3");
	SoundAsset::Register(L"ELBGMfortress",L"data/Elis/BGM/fortress.mp3");
	SoundAsset::Register(L"ELBGMtemple",L"data/Elis/BGM/temple.mp3");
	SoundAsset::Register(L"ELBGMfreezedport",L"data/Elis/BGM/freezedport.mp3");
	SoundAsset::Register(L"ELBGMlastboss1",L"data/Elis/BGM/lastboss1.mp3");
	SoundAsset::Register(L"ELBGMtutorial",L"data/Elis/BGM/tutorial.mp3");
	SoundAsset::Register(L"ELBGMbossbattle",L"data/Elis/BGM/bossbattle.mp3");


	TextureAsset::Register(L"ELlogotower",L"data/Elis/Map/logo/tower.png");
	TextureAsset::Register(L"ELlogocave",L"data/Elis/Map/logo/cave.png");
	TextureAsset::Register(L"ELlogotemple",L"data/Elis/Map/logo/temple.png");
	TextureAsset::Register(L"ELlogopalace",L"data/Elis/Map/logo/palace.png");
	TextureAsset::Register(L"ELlogoport",L"data/Elis/Map/logo/port.png");
	TextureAsset::Register(L"ELlogofortress",L"data/Elis/Map/logo/fortress.png");

	TextureAsset::Register(L"ELlogoclear",L"data/Elis/Map/logo/clear.png");
	TextureAsset::Register(L"ELlogobossbattle",L"data/Elis/Map/logoBossbattle.png");

	SoundAsset::Register(L"ELSEstage_start",L"data/Elis/SE/System/stage_start.mp3");
	
	SoundAsset::Register(L"ELSEbossbattle_start",L"data/Elis/SE/System/bossbattle_start.mp3");

}

bool ELMap::loadStage(const String& name, const String& stageName)
{
	m_haveLogo = false;
	m_logoName = L"null";

	m_stageName = name;

	m_imgTerrain = Image(L"data/Elis/Map/" + name + L"/" + stageName + L"_terrain.png");
	
	
	m_texMap = Texture(L"data/Elis/Map/"+ name + L"/" + stageName + L"_map.png");

	
	const CSVReader csv(L"data/Elis/Map/" +name +L"/"+ stageName +L".csv");


	if(!csv) return false;

	if(m_bgmName != csv.get<String>(0,0).lower())//曲名を取得
	{
		if(SoundAsset(L"ELBGM"+ m_bgmName).isPlaying)
			SoundAsset(L"ELBGM"+ m_bgmName).stop();

		m_bgmPlayed = false;
		m_bgmName = csv.get<String>(0,0).lower();
		//SoundAsset(L"ELBGM"+ m_bgmName).setLoop(true);
	}

	for(int i=1;;++i)
	{
		String SName = csv.get<String>(i,0).lower();

		if(SName.isEmpty)
		{
			break;
		}
		else if(SName==L"setstartpos")
		{
			m_startpos = ELMapStartPos(csv.get<String>(i,1).lower(), Point(csv.get<int>(i,2),csv.get<int>(i,3)));
		}
		else if(SName==L"door")
		{
			const Point p = Point(csv.get<int>(i,1),csv.get<int>(i,2));
			const String str = csv.get<String>(i,3).lower();
			const Point p2 = Point(csv.get<int>(i,4),csv.get<int>(i,5));
			const String str2 = csv.get<String>(i,6).lower();

			m_doors.emplace_back(p,str,p2,str2);
		}
		else if(SName==L"showlogo")
		{
			const String str = csv.get<String>(i,1).lower();
			
			if(!str.isEmpty)
			{
				m_haveLogo = true;
				m_logoName = str;
				m_logoframe = 1;
			}
		}
	}

	if(!m_imgTerrain)
		LOG_ERROR(L"trrainのデータが読み込めていない");

	if(!m_texMap)
		LOG_ERROR(L"trrainのデータが読み込めていない");

	return !!m_imgTerrain && !!m_texMap;
}

void ELMap::BGMupdate(const ElisState& elisState)
{
	if(elisState != ElisState::Dead)
	{
		if(m_bgmName == L"bossbattle" && m_bgmPlayed && !SoundAsset(L"ELBGM"+ m_bgmName).isPlaying)
		{
			SoundAsset(L"ELBGM"+ m_bgmName).setPosSec(23.132);
			SoundAsset(L"ELBGM"+ m_bgmName).play();
		}
		else if(!SoundAsset(L"ELBGM"+ m_bgmName).isPlaying)
		{
				SoundAsset(L"ELBGM"+ m_bgmName).play();
				m_bgmPlayed = true;
		}
	}
}

bool ELMap::update(const Rect& elisRect, const ElisState& elisState)
{
	

	for(size_t i = 0;i<m_doors.size() ;++i)
	{
		if(m_doors[i].update(elisRect, elisState))
		{
			SoundAsset(L"ELSEdoor").play();

			m_doorIndex = i;

			return true;
		}
	}

	return false;

	//Window::SetTitle(Format()+m_sndBGM.posSec+L" / "+m_sndBGM.lengthSec);
}

void ELMap::draw(const Point& cameraPos)
{
	Point pos = cameraPos;
	m_texMap(pos.x,pos.y,m_windowW,m_windowH).draw();

	for(size_t i = 0;i<m_doors.size() ;++i)
	{
		m_doors[i].draw(cameraPos);
	}

	//Window::SetTitle(Format() + SoundAsset(L"ELBGM"+ m_bgmName).lengthSec +L"/"+ SoundAsset(L"ELBGM"+ m_bgmName).posSec);

	//Resource::Font(L"Font.Common.Light18").drawCenter(m_bgmName,200,Palette::Blue);

	//Circle(m_startpos.startPos, 30).draw(Palette::Red);
}


bool ELMap::isFloor(int x,int y)const
{
#ifndef _DEBUG
	if(x<0 || m_imgTerrain.width<=x || y<0 || m_imgTerrain.height<=y)
	{
		return false;
	}
#endif
	return m_imgTerrain[y][x]!=Palette::White;
}

Point ELMap::getMapSize()const
{
	return Point(m_imgTerrain.width,m_imgTerrain.height);
}

ELFloorType ELMap::getFloorType(int x,int y)const
{
	Color col(m_imgTerrain[y][x].r
		, m_imgTerrain[y][x].g
		, m_imgTerrain[y][x].b
		);

	if (col == Palette::Black)
	{
		return ELFloorType::NormalFloor;
	}
	else if (col == Palette::Blue)
	{
		return ELFloorType::PassFloor;
	}
	else if (col == Palette::Red)
	{
		return ELFloorType::DeathFloor;
	}
	else
	{
		return ELFloorType::Null;
	}
	/*
	switch(m_imgTerrain[y][x].c)
	{
	case Palette::Black:	
		return ELFloorType::NormalFloor;
		break;
	case Palette::Blue:
		return ELFloorType::PassFloor;
		break;
	case Palette::Red:
		return ELFloorType::DeathFloor;
		break;
	default:	
		return ELFloorType::Null;
	}
	*/
}

Point ELMap::getStartPos() const
{
	return m_startpos.startPos;
}

String ELMap::getStartName() const
{
	return m_startpos.stageName;
}

String ELMap::getDestName() const
{
	return m_doors[m_doorIndex].getStageName();
}

Point ELMap::getDestPos() const
{
	return m_doors[m_doorIndex].getDestunationPoint();
}

void ELMap::clearDoors()
{
	m_doors.clear();
}
/////////////////////////////////////////////////////イベント用
void ELMap::stopStageBGM()
{
	if(SoundAsset(L"ELBGM"+ m_bgmName).isPlaying)
		SoundAsset(L"ELBGM"+ m_bgmName).stop();

	m_bgmPlayed = false;
}

void ELMap::playStageMusic(const String &bgmName)
{
	if(SoundAsset(L"ELBGM"+ m_bgmName).isPlaying)
		SoundAsset(L"ELBGM"+ m_bgmName).stop();

	m_bgmName = bgmName;

	/*
	SoundAsset(L"ELBGM"+ m_bgmName).setLoop(true);
	
	SoundAsset(L"ELBGM"+ m_bgmName).play();
	m_bgmPlayed = true;
	*/
}

void ELMap::drawLogo()
{	
	if(m_logoName == L"logotower")
	{
		if(m_logoframe == 1)
			SoundAsset(L"ELSEstage_start").play();

		Rect rect(360,320,560,70);
		rect.draw(Color(255,255,255,100));

		TextureAsset(L"EL"+m_logoName)(0,0,rect.size.x*Min(m_logoframe,30)/30,rect.size.y)
		.draw(rect.pos);
	}
	else if(m_logoName == L"logocave")
	{
		if(m_logoframe == 1)
			SoundAsset(L"ELSEstage_start").play();

		Rect rect(360,320,560,70);
		rect.draw(Color(255,255,255,100));

		TextureAsset(L"EL"+m_logoName)(0,0,rect.size.x*Min(m_logoframe,30)/30,rect.size.y)
		.draw(rect.pos);
	}
	else if(m_logoName == L"logobossbattle")
	{
		if(m_logoframe == 1)
			SoundAsset(L"ELSEbossbattle_start").play();
		TextureAsset(L"EL"+m_logoName)(
		0,
		143*(m_logoframe / 8%(TextureAsset(L"EL"+m_logoName).height/143)),
		1082,
		143).draw(0,200);
	}
	else if(m_logoName == L"logoclear")
	{
		if(m_logoframe == 1)
			SoundAsset(L"ELSEstage_start").play();

		Rect rect(360,320,560,70);
		rect.draw(Color(255,255,255,100));

		TextureAsset(L"EL"+m_logoName)(0,0,rect.size.x*Min(m_logoframe,30)/30,rect.size.y)
		.draw(rect.pos);
	}

	if(m_logoframe == 64-1)
	{
		m_haveLogo = false;
		//m_logoName = L"null";
	}

	++m_logoframe;
}

//////////////////////////////////////////////////////////////