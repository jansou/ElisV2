# pragma once
# include <Siv3D.hpp>
# include "ELDoor.h"
# include "ELCommon.h"

struct ELMapStartPos
{
	ELMapStartPos(const String& _stageName,const Point& _startPos)
		:stageName(_stageName),startPos(_startPos)
	{

	}

	ELMapStartPos()
	{
		stageName = L"null";
		startPos = Point(0,0);
	}

	String stageName;
	Point startPos;
};


class ELMap
{
private:
	Texture m_texMap;//îwåi
	Image m_imgTerrain;//ínå`ÉfÅ[É^
	int m_windowW;
	int m_windowH;
	Sound m_sndBGM;//ébíËìIÇ»BGM
	String m_bgmName;//ã»ñº
	ELMapStartPos m_startpos;
	String m_stageName;
	std::vector<ELDoor> m_doors;
	int m_doorIndex;
	
	bool m_haveLogo;
	String m_logoName;
	int m_logoframe;
	bool m_bgmPlayed;
public:
	ELMap();
	bool init();
	bool loadStage(const String& name,const String& stageName);
	void draw(const Point& cameraPos);
	bool update(const Rect& elisRect, const ElisState& elisState);
	void BGMupdate(const ElisState& elisState);
	bool isFloor(int x,int y)const;
	Point getMapSize()const;
	ELFloorType getFloorType(int x,int y)const;

	Point getStartPos() const;

	String getDestName()const;

	Point getDestPos() const;

	void clearDoors();

	String getStartName() const;

	void stopStageBGM();

	void playStageMusic(const String &bgmName);

	bool getHaveLogo() const {return m_haveLogo;}

	void drawLogo();

	void setLogoname(const String& logoname){
		m_logoName = logoname;
		m_haveLogo=true;
		m_logoframe = 1;
	}

	bool haveLogoname(const String &logoname){return m_logoName == logoname;}
};