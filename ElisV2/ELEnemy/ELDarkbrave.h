
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

enum class ELDarkbraveState
{
	Ready,
	NormalStaying,
	BattleStaying,
	Floating,
	Moving,
	Jumping,
	Attacking,
	Damaged,
	SwordRain,
	Dash,
	Warp,
	Strike,
	Dead,
};

enum class AttackMode
{
	SwordRain,
	Strike,
	null,
};

class ELDarkbrave : public ELEnemy
{
private:
	int m_frameCount;
	int m_oldFrameCount;
	int m_jumpH;
	int m_speed;
	ELDarkbraveState m_oldState;

	ELEnemyName m_name;
	ELEnemySize m_size;
	int m_HP;
	Point m_pos;
	Point m_startPos;
	ELDarkbraveState m_state;
	AttackMode m_attackMode;

	ELEnemyFace m_face;

	Rect m_hitRect;
	Rect m_hitArea;
	bool m_erased;
	bool m_damaged;

	void gravity(const ELMap& map, ELObjectInfo& object, const Point &camerapos);
	void MoveXAxis(const ELMap& map, int speed, const Point &camerapos, ELObjectInfo& object);
	void MoveYAxis(const ELMap& map, int speed, const Point &camerapos, ELObjectInfo& object);

	void jump(const ELMap& map, int jumpH, const Point &camerapos);
	void IdentifyState(const ELMap& map, ELObjectInfo& object);
	static bool intersectBody(const Point& foot,const Point& head, const ELMap& map);

	void FloatingProcess(const ELMap& map, const Point &camerapos, ELObjectInfo& object, const Point &playerpos);
	void SwordRainProcess(const ELMap& map, const Point &camerapos, ELObjectInfo& object, ELAttack &attack);
	void StrikeProcess(const ELMap& map, const Point &camerapos, ELObjectInfo& object, ELAttack &attack, const Point &playerpos);
	
	void Warp(const Point& tarpos);
	bool m_warped;

	int m_damageFrame;
	int m_choiceCount;
	int m_oldWarpto;

public:
	ELDarkbrave(const ELEnemyName& Name,
		const ELEnemySize& Size,
		const Rect& hitRect,
		int HP,
		const Point& Pos);

	void update(const ELMap& map, const Point& playerpos, ELObjectInfo& object, ELItem &item, ELAttack &attack, const Point &camerapos);

	void draw(const Point& camerapos,const Point& modifyDrawPos)const;

	ELEnemySize getEnemySize() const; 

	Point getPos() const; 

	bool isHit(const Rect &body) const;

	int getDamage() const;

	Rect getHitRect() const;

	void giveDamage(int damage);

	Point getStartPos() const;

	bool isErased() const;

	bool isDamaged() const;

	ELEnemyName getName() const
	{
		return m_name;
	}

	int getLife() const {return m_HP;}

	//////////
	bool ready();

	void addFramecount()
	{++m_frameCount;}

	ELEnemyState getEnemyState() const
	{
		if(m_state == ELDarkbraveState::Dead)
			return ELEnemyState::Dead;

		return ELEnemyState::null;
	}

	Point getBurstPos() const {return m_pos-Point(0,59);}
};