# pragma once
# include <Siv3D.hpp>
# include "ELCommon.h"

class ELStageSelect
{
private:
	String m_mapName;
	bool m_clearedCave;
	bool m_clearedPort;
	bool m_clearedTemple;
	bool m_clearedFortress;
	bool m_clearedPalace;
	bool m_clearedTower;

	bool m_onPut;

	Key m_keyLeft;

public:
	ELStageSelect();
	bool init();
	bool update(const Point cursorpos);
	void draw(const Point &cursorpos);
	String getMapName() const { return m_mapName; }
	bool loadSave();
	void releaseData();

	void showCaveAnime(const Point &cursorpos);//caveのアニメ
	void showPortAnime(const Point &cursorpos);//portのアニメ
	void showTempleAnime(const Point &cursorpos);//templeのアニメ
	void showFortressAnime(const Point &cursorpos);//fortressのアニメ
	void showPalaceAnime(const Point &cursorpos);//palaceのアニメ
	void showTowerAnime(const Point &cursorpos);//towerのアニメ

	void drawSign(const String& assetName, const Point& pos, int frameCount, int maxFrame);
	void drawIcon(const String& assetName, const RectF& drawRect, const Vec2& drawPos);
	void showGeneralAnime(bool intersect, bool cleared, int& frameCount, Circle c, String iconName, String signName, RectF drawRect, Vec2 drawPos, Point pos);

};