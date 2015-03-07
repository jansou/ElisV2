#pragma once
# include <Siv3D.hpp>
# include "ELMap.h"
# include "ELPlayer.h"
# include "ELMyCamera.h"
# include "ELMyCursor.h"
# include "ELEnemyInfo.h"
# include "ELAttack.h"
# include "ELObjectInfo.h"
# include "ELUI.h"
# include "ELTitle.h"
# include "ELCommon.h"
# include "ELItem.h"
# include "ELEvent.h"
# include "ELStageSelect.h"
# include "ELFade.h"
# include "ELTalk.h"

//# include "../Common.hpp"

struct ResourceInfo
{
	ResourceInfo(const String &_tag, const String &_path, const String &_resourceType)
	{
		tag = _tag;
		path = _path;
		resourceType = _resourceType;
	}

	String tag;
	String path;
	String resourceType;
};

class Elis : public Game
{
private:
	static unsigned fadeOutTime()
	{
		return 1000;
	}

	//std::vector<ResourceInfo> m_resources;

	//PauseHelper m_pause;
	Optional<Application> m_next;

	ELMap map;
	ELObjectInfo objectInfo;
	ELPlayer player;
	ELMyCamera mycamera;
	ELMyCursor cursor;
	ELEnemyInfo enemyInfo;
	ELAttack attack;
	ELItem item;
	ELUI UI;
	ELTitle title;
	ELEvent elEvent;
	ELStageSelect stageSelect;
	ELFade fade;
	ELTalk talk;

	ELPlayState m_playState;
	Point camerapos;
	ELGameState m_gameState;
	String m_mapName;
	String m_stageName;
	TimerMillisec m_fadeOutTimer;

	bool loadData(const String& mapName,const String& stageName); 

	void playingDraw();

	void blackFade();
	void whiteFade();

	void loadResources();

	bool m_doorOpened;
public:
	Elis();

	bool init();

	bool update();

	Optional<Application> next(){return m_next;}
};