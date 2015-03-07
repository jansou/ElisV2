# include "ELStageSelect.h"

namespace
{
	const int kR = 130;

	const Circle kCirCave(46 + 291.0 / 2, 91 + 226 / 2, kR);

	const Circle kCirPort(438 + 352 / 2, 137 + 135.0 / 2, kR);

	const Circle kCirTemple(186 + 292 / 2, 329 + 187.0 / 2, kR);

	const Circle kCirFortress(486 + 299.0 / 2, 496 + 207.0 / 2, kR);

	const Circle kCirPalace(844 + 344 / 2, 402 + 182 / 2, kR);

	const Circle kCirTower(828 + 423.0 / 2, 26 + 293.0 / 2, kR);
};

ELStageSelect::ELStageSelect() :
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
	const String signPath = L"data/Elis/StageSelect/sign/";
	TextureAsset::Register(L"texELcave_sign", signPath + L"cave_sign.png");
	TextureAsset::Register(L"texELport_sign", signPath + L"port_sign.png");
	TextureAsset::Register(L"texELtemple_sign", signPath + L"temple_sign.png");
	TextureAsset::Register(L"texELfortress_sign", signPath + L"fortress_sign.png");
	TextureAsset::Register(L"texELpalace_sign", signPath + L"palace_sign.png");
	TextureAsset::Register(L"texELtower_sign", signPath + L"tower_sign.png");
	return true;
}

bool ELStageSelect::loadSave()
{
	const CSVReader csv(L"data/Elis/Save/save.csv");

	if (!csv)
	{
		return false;
	}

	for (int i = 0;; ++i)
	{
		const String Name = csv.get<String>(i, 0).lower();

		if (Name.isEmpty)
		{
			break;
		}

		const bool cleared = csv.get<String>(i, 1).lower() == L"true";

		if (Name == L"cave")
		{
			m_clearedCave = cleared;
		}
		else if (Name == L"port")
		{
			m_clearedPort = cleared;
		}
		else if (Name == L"temple")
		{
			m_clearedTemple = cleared;
		}
		else if (Name == L"fortress")
		{
			m_clearedFortress = cleared;
		}
		else if (Name == L"palace")
		{
			m_clearedPalace = cleared;
		}
		else if (Name == L"tower")
		{
			m_clearedTower = cleared;
		}
	}

	return true;
}

bool ELStageSelect::update(const Point cursorpos)
{
	if (!SoundAsset(L"sndELstageselect").isPlaying){
		SoundAsset(L"sndELstageselect").play();
	}

	const auto f = [&](String mapName){
		if (!m_onPut)
		{
			if (SoundAsset(L"sndELsystemButton").isPlaying){
				SoundAsset(L"sndELsystemButton").stop();
			}
			SoundAsset(L"sndELsystemButton").play();

			m_onPut = true;
		}

		if (m_keyLeft.clicked)
		{
			m_mapName = mapName;
			SoundAsset(L"sndELstage_choice").play();
			m_onPut = false;
			return true;
		}
		return false;
	};
	if (kCirCave.intersects(cursorpos)){
		if (f(L"cave")){
			return true;
		}
	}
	else if (kCirPort.intersects(cursorpos) && m_clearedCave){
		if (f(L"port")){
			return true;
		}
	}
	else if (kCirTemple.intersects(cursorpos) && m_clearedPort){
		if (f(L"temple")){
			return true;
		}
	}
	else if (kCirFortress.intersects(cursorpos) && m_clearedTemple){
		if (f(L"fortress")){
			return true;
		}
	}
	else if (kCirPalace.intersects(cursorpos) && m_clearedFortress){
		if (f(L"palace")){
			return true;
		}
	}
	else if (kCirTower.intersects(cursorpos) && m_clearedPalace){
		if (f(L"tower")){
			return true;
		}
	}
	else{
		m_onPut = false;
	}
	return false;
}

void ELStageSelect::draw(const Point &cursorpos)
{
	TextureAsset(L"texELworldmap").draw();

	const TextureAsset& t = TextureAsset(L"texELmapbit");

	///////////////////////////////////////////////
	//toPort

	t.drawAt(Point(342 + 25, 284 - 14));
	t.drawAt(Point(342 + 53, 284 - 22));
	t.drawAt(Point(342 + 81, 284 - 30));

	//toFortress
	t.drawAt(Point(595 - 17, 270 + 29));
	t.drawAt(Point(595 - 34, 270 + 58));
	t.drawAt(Point(595 - 51, 270 + 87));
	t.drawAt(Point(595 - 68, 270 + 116));
	t.drawAt(Point(595 - 85, 270 + 145));
	t.drawAt(Point(595 - 102, 270 + 174));

	//toTemple
	t.drawAt(Point(315 + 24, 518 + 22));
	t.drawAt(Point(315 + 24 * 2, 518 + 22 * 2));
	t.drawAt(Point(315 + 24 * 3, 518 + 22 * 3));
	t.drawAt(Point(315 + 24 * 4, 518 + 22 * 4));
	t.drawAt(Point(315 + 24 * 5, 518 + 22 * 5));
	t.drawAt(Point(315 + 24 * 6, 518 + 22 * 6));

	//toPalace
	t.drawAt(Point(785 + 32, 669 - 14));
	t.drawAt(Point(785 + 32 * 2, 669 - 14 * 2));
	t.drawAt(Point(785 + 32 * 3, 669 - 14 * 3));
	t.drawAt(Point(785 + 32 * 4, 669 - 14 * 4));
	t.drawAt(Point(785 + 32 * 5, 669 - 14 * 5));

	//toTower
	t.drawAt(Point(1016 + 2 * 3 + 5, 506 - 30 * 3 - 20));
	t.drawAt(Point(1016 + 2 * 4 + 5, 506 - 30 * 4 - 20));
	t.drawAt(Point(1016 + 2 * 5 + 5, 506 - 30 * 5 - 20));
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

void ELStageSelect::drawSign(const String& assetName, const Point& pos, int frameCount, int maxFrame)
{
	TextureAsset(assetName)(0, 126, 132, 126).draw
		(pos.x + ((frameCount == maxFrame) ? 0 : ((frameCount % 4 >= 2) ? frameCount / 2 : -frameCount / 2))
		, pos.y);
}
void ELStageSelect::drawIcon(const String& assetName, const RectF& drawRect, const Vec2& drawPos){
	TextureAsset(assetName)(drawRect).draw(drawPos);
}

void ELStageSelect::showGeneralAnime(bool intersect, bool cleared, int& frameCount, Circle c, String iconName, String signName, RectF drawRect, Vec2 drawPos, Point pos){
	if (intersect)
	{
		const int animeframe = 4;
		const int maxframe = animeframe * 2;

		if (intersect && cleared){
			c.draw(Color(255, 255, 255, 200));
		}

		frameCount = Min(++frameCount, maxframe);

		if (cleared){
			drawIcon(iconName, drawRect, drawPos);
		}

		drawSign(signName, pos, frameCount, maxframe);
	}
	else
	{
		if (cleared){
			drawIcon(iconName, drawRect, drawPos);
		}

		frameCount = 0;
	}
}

void ELStageSelect::showCaveAnime(const Point &cursorpos)
{
	static int frameCount = 0;

	const bool intersect = kCirCave.intersects(cursorpos);
	const Circle c = kCirCave;

	const RectF drawRect(0, 226 * intersect, 291, 226);
	const Vec2 drawPos(46, 91);

	const String iconName = L"texELcaveIcon";
	const String signName = L"texELcave_sign";
	const Point pos(317, 208);

	if (intersect)
	{
		const int animeframe = 4;
		const int maxframe = animeframe * 2;

		c.draw(Color(255, 255, 255, 200));

		frameCount = Min(++frameCount, maxframe);

		drawIcon(iconName, drawRect, drawPos);

		drawSign(signName, pos, frameCount, maxframe);
	}
	else
	{
		drawIcon(iconName, drawRect, drawPos);

		frameCount = 0;
	}

}

void ELStageSelect::showPortAnime(const Point &cursorpos)
{
	static int frameCount = 0;

	const bool intersect = kCirPort.intersects(cursorpos);
	const Circle c = kCirPort;
	const RectF drawRect(0, 135 * intersect, 352, 135);
	const Vec2 drawPos(438, 137);

	const String iconName = L"texELportIcon";
	const String signName = L"texELport_sign";
	const Point pos(744, 206);
	const bool cleared = m_clearedCave;

	showGeneralAnime(intersect, cleared, frameCount, c, iconName, signName, drawRect, drawPos, pos);
}

void ELStageSelect::showTempleAnime(const Point &cursorpos)
{
	static int frameCount = 0;

	const bool intersect = kCirTemple.intersects(cursorpos);
	const Circle c = kCirTemple;
	const RectF drawRect(0, 187 * intersect, 292, 187);
	const Vec2 drawPos(186, 329);

	const String iconName = L"texELtempleIcon";
	const String signName = L"texELtemple_sign";
	const Point pos(444, 397);
	const bool cleared = m_clearedPort;

	showGeneralAnime(intersect, cleared, frameCount, c, iconName, signName, drawRect, drawPos, pos);
}

void ELStageSelect::showFortressAnime(const Point &cursorpos)
{
	static int frameCount = 0;

	const bool intersect = kCirFortress.intersects(cursorpos);
	const Circle c = kCirFortress;
	const RectF drawRect(0, 207 * intersect, 299, 207);
	const Vec2 drawPos(486, 496);

	const String iconName = L"texELfortressIcon";
	const String signName = L"texELfortress_sign";
	const Point pos(773, 576);

	if (intersect)
	{
		const int animeframe = 4;
		const int maxframe = animeframe * 2;

		frameCount = Min(++frameCount, maxframe);

		if (kCirFortress.intersects(cursorpos) && m_clearedPort){
			kCirFortress.draw(Color(255, 255, 255, 200));
		}

		if (m_clearedTemple){
			drawIcon(iconName, drawRect, drawPos);
		}

		drawSign(signName, pos, frameCount, maxframe);
	}
	else
	{
		if (m_clearedTemple){
			drawIcon(iconName, drawRect, drawPos);
		}
		frameCount = 0;
	}
}

void ELStageSelect::showPalaceAnime(const Point &cursorpos)
{
	static int frameCount = 0;

	const bool intersect = kCirPalace.intersects(cursorpos);
	const Circle c = kCirPalace;
	const RectF drawRect(0, 182 * intersect, 344, 182);
	const Vec2 drawPos(844, 402);

	const String iconName = L"texELpalaceIcon";
	const String signName = L"texELpalace_sign";
	const Point pos(1155, 468);
	const bool cleared = m_clearedFortress;

	showGeneralAnime(intersect, cleared, frameCount, c, iconName, signName, drawRect, drawPos, pos);
}

void ELStageSelect::showTowerAnime(const Point &cursorpos)
{
	static int frameCount = 0;

	const bool intersect = kCirTower.intersects(cursorpos);
	const Circle c = kCirTower;
	const RectF drawRect(0, 293 * intersect, 423, 293);
	const Vec2 drawPos(828, 26);

	const String iconName = L"texELtowerIcon";
	const String signName = L"texELtower_sign";
	const Point pos(1140, 198);
	const bool cleared = m_clearedPalace;

	showGeneralAnime(intersect, cleared, frameCount, c, iconName, signName, drawRect, drawPos, pos);
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