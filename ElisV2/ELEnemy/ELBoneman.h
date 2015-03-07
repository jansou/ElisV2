
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

class ELBoneman : public ELEnemy
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

	bool m_damaged;

	Rect m_hitRect;
	Rect m_hitArea;
	bool m_erased;

	void gravity(const ELMap& map, ELObjectInfo& object, const Point &camerapos);
	void MoveXAxis(const ELMap& map, int speed, const Point &camerapos, ELObjectInfo& object);
	void MoveYAxis(const ELMap& map, int speed, const Point &camerapos, ELObjectInfo& object);
	void IdentifyState(const ELMap& map, ELObjectInfo& object);
	static bool intersectBody(const Point& foot,const Point& head, const ELMap& map);
	
public:
	ELBoneman(const ELEnemyName& Name,
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

	int getLife() const {return m_HP;}

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