# pragma once
# include <Siv3D.hpp>
# include "../ELMap.h"
# include "ELEnemy.h"
# include "../ELEnemyInfo.h"
# include "../ELObjectInfo.h"
# include "../ELItem.h"
# include "../ELCommon.h"
# include "../ELAttack.h"

enum class ELEnemyName;

enum class ELEnemySize;

enum ELEnemyFace;

enum class ELEnemyState;





class ELSeaserpentTentacle
{
private:
	int m_frameCount;
	int m_oldFrameCount;
	int m_jumpH;
	int m_speed;
	ELEnemyState m_oldState;

	ELEnemyName m_name;
	ELEnemySize m_size;
	int m_HP;
	Point m_pos;
	Point m_startPos;
	ELEnemyState m_state;
	ELEnemyFace m_face;

	Rect m_hitRect;
	Rect m_hitArea;
	bool m_erased;

	void MoveXAxis(const ELMap& map, int speed, const Point &camerapos);
	void jump(const ELMap& map, int jumpH, const Point &camerapos);
	static bool intersectBody(const Point& foot, const Point& head, const ELMap& map);

	Vec2 direction(const Vec2& beginpos, const Vec2& endpos)const
	{
		return Vec2(endpos - beginpos).normalize();
	}

	Vec2 m_playerpos;
	int m_MaxHp;

	

public:
	ELSeaserpentTentacle();

	void setPos(const Point& pos);
	void setState(const ELEnemyState& state)
	{
		m_state = state;
		m_frameCount = 0;
	}
	/*
	ELSeaserpentTentacle(const ELEnemyName& Name,
		const ELEnemySize& Size,
		const Rect& hitRect,
		int HP,
		const Point& Pos);
		*/
	void update(const ELMap& map, const Point& playerpos, ELObjectInfo& object, ELItem &item, ELAttack &attack, const Point &camerapos);

	void draw(const Point& camerapos, const Point& modifyDrawPos)const;

	ELEnemySize getEnemySize() const;

	Point getPos() const;

	bool isHit(const Rect &body) const;

	int getDamage() const;

	Rect getHitRect() const;

	void giveDamage(int damage);

	Point getStartPos() const;

	bool isErased() const;

	bool isDamaged() const;

	int getLife() const { return m_HP; }

	ELEnemyName getName() const
	{
		return m_name;
	}

	void addFramecount()
	{
		++m_frameCount;
	}

	ELEnemyState getEnemyState() const
	{
		return m_state;
	}
};


enum class ELSeaserpentAttack
{
	UpperTentacle,
	IceShoot,
};

class ELSeaserpent : public ELEnemy
{
private:
	int m_frameCount;
	int m_oldFrameCount;
	int m_jumpH;
	int m_speed;
	ELEnemyState m_oldState;

	ELEnemyName m_name;
	ELEnemySize m_size;
	int m_HP;
	Point m_pos;
	Point m_startPos;
	Rect m_drawRect;
	ELEnemyState m_state;
	ELEnemyFace m_face;
	Rect m_hitRect;
	Rect m_hitArea;
	bool m_erased;
	Rect m_coreRect;
	bool m_invisibled;
	int m_damageCount;

	ELSeaserpentAttack m_attackType;
	//â∫Ç©ÇÁÇÃìÀÇ´è„Ç∞çUåÇ
	void Attack_UpperTentacle(ELAttack& attack, const Point& playerpos);

	void drawTail_stay() const;
	void drawBody_stay() const;
	void drawHead_stay() const;
	/*
	//////////////êGéË//////////////////////////////
	//êGéËÇÃç¿ïW
	Point m_tentacleFrontNodes[10];//ëO
	Point m_tentacleBackNodes[10];//å„
	//êGéËÇÃçXêV
	void updateFrontTentacle();
	void updateBackTentacle();
	//êGéËÇÃï`âÊ
	void drawFrontTentacle(const Point& camerapos)const;//ëO
	void drawBackTentacle(const Point& camerapos)const;//å„
	////////////////////////////////////////////////
	*/
	ELCameraEffect m_cameraEffect;

	void attackProcess(const Point& playerpos);
	void MoveXAxis(const ELMap& map, int speed, ELObjectInfo& object, const Point &camerapos);
	void IdentifyState(const ELMap& map, ELObjectInfo& object, const Point &camerapos);

	static bool intersectBody(const Point& foot, const Point& head, const ELMap& map);

	std::vector< ELSeaserpentTentacle > m_tentacles;
	//void tentacle1update(const ELMap& map, const Point& playerpos, ELObjectInfo& object, ELItem &item, ELAttack &attack, const Point &camerapos);

	int m_maxHP;

public:
	ELSeaserpent(const ELEnemyName& Name,
		const ELEnemySize& Size,
		const Rect &hitRect,
		int HP,
		const Point& Pos);

	void update(const ELMap& map, const Point& playerpos, ELObjectInfo& object, ELItem &item, ELAttack &attack, const Point &camerapos);

	void draw(const Point& camerapos, const Point& modifyDrawPos)const;

	ELEnemySize getEnemySize() const;

	Rect getDrawRect() const;

	Point getPos() const;

	bool isHit(const Rect &body) const;

	int getDamage() const;

	Rect getHitRect() const;

	void giveDamage(int damage);

	Point getStartPos() const;

	bool isErased() const;

	bool isDamaged() const;

	int getLife() const { return m_HP; }

	ELEnemyName getName() const
	{
		return m_name;
	}
	///////////////////////////////
	void addFramecount()
	{
		++m_frameCount;
	}

	ELCameraEffect getCameraEffect() const
	{
		return m_cameraEffect;
	}

	bool ready();

	Point getBurstPos() const { return m_hitRect.center; }

	ELEnemyState getEnemyState() const
	{
		return m_state;
	}

};
