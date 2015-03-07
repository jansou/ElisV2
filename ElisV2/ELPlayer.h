# pragma once
# include <Siv3D.hpp>
# include <vector>
# include "ELMap.h"
# include "ELAttack.h"
# include "ELEnemyInfo.h"
# include "ELMyCursor.h"
# include "ELObjectInfo.h"
# include "ELCommon.h"

enum ElisFace
{
	Left,
	Right,
};

class ELPlayer
{
private:
	Rect m_elisRect;
	Rect m_elisHitRect;
	//略さない
	//Texture m_texElisNormal;//通常状態
	//Texture m_texElisRun;//歩いているときは通常と交互に表示
	//Texture m_texElisJump;//飛んでいるとき

	ElisState m_elisState;
	ElisState m_oldState;
	ElisFace m_elisFace;

	ElisAttackState m_elisAttackState;
	int m_chargeFrameCount;

	int m_attackInterval;

	Vec2 m_startPos;
	Vec2 m_endPos;

	int m_speed;
	int m_riseOffset;

	int m_gravity;

	ELMap m_map;

	int m_frameCount;

	int m_jumpH;
	bool m_jumped;

	bool m_invincibled;
	int m_invincibleTime;

	//Sound m_sndElisJump;//とんだ時
	//Sound m_sndElisLand;//着地した時
	//Sound m_sndElisCharge;//溜め中
	//Sound m_sndElisDead;//死亡中
	//Sound m_sndElisDamaged;//喰らい音
	//Sound m_sndElisIce;//氷魔法

	Key m_keyRight;
	Key m_keyLeft;
	Key m_keyUp;
	Key m_keyDown;
	Key m_mouseLeft;

	static bool intersectBody(const Point& foot,const Point& head, const ELMap& map, const ELFloorType& floorType);
	
	Point footCollisionPos()const
	{
		return m_elisRect.center + Point(0,m_elisRect.h/2);
	}

	Point headCollisionPos()const
	{
		return Point(m_elisRect.center.x,m_elisRect.y+50);
	}

	void MoveXAxis(const ELMap& map, ELObjectInfo& object, const Point &camerapos);
	void MoveYAxis(ELMap& map, ELObjectInfo& object, const Point &camerapos);

	void JumpProcess(const ELMap& map, ELObjectInfo& object, const Point &camerapos);
	void PassFloorProcess(const ELMap& map);
	void FallProcess(const ELMap& map, ELObjectInfo& object, const Point &camerapos);
	void IdentifyState(ELMap& map, ELObjectInfo& object, const Point &camerapos);
	void ChargeProcess(ELAttack& attack, const ELMyCursor& cursor, const Point& camerapos, ELObjectInfo &object);
	void DamageProcess(const ELMap& map, ELObjectInfo& object, const Point &camerapos);
	void AttackProcess(const ELMap& map, ELObjectInfo& object, const Point &camerapos);
	void DeadProcess(ELMap &map);
	void FireProcess(const ELMap& map, ELObjectInfo& object, const Point &camerapos, ELAttack &attackconst ,const ELMyCursor &cursor);

	int m_life;
	int m_maxLife;
	
	int m_MP;
	//int m_maxMP;
	ElisMagicType m_elisMagicType;

	void resetCount();

	//int m_MPframeCount;

public:
	ELPlayer();
	bool init();
	void draw(const Point& cameraPos)const;
	ELPlayState update(ELMap& map , ELAttack& attack, const ELMyCursor& cursor,
		const ELEnemyInfo& enemy, ELObjectInfo& object, const Point& camerapos);
	
	Point getPlayerPos()const//map上のプレイヤーの位置
	{return m_elisRect.pos;}

	Point getPlayerFootCollisionPos() const;
	void checkHit(const ELEnemyInfo& enemyInfo, const ELAttack& attack);
	bool isDead() const;
	Rect getRect() const;
	
	ElisState getState() const;

	void setPos(const Point& pos); 

	void restartProcess(const ELMap& map, const ELEnemyInfo& enemy);

	int getLife() const;
	ElisMagicType getMagicType() const {return m_elisMagicType;}
	
	void itemEffect(const ELItemType &itemType);

	void setMagic(const ElisMagicType &magicType);

	void objectUpdate(const Point &camerapos, ELObjectInfo &object);

	//int getUseMP(const ElisMagicType &elisMagic);
	
	int ELPlayer::getMP() const
	{
		return m_MP;
	}
	
	////////////////イベント用コマンド
	bool walkto(int x, int speed, ELMap &map,ELObjectInfo &object, const Point &camerapos);

	bool identyfybase(ELMap &map,ELObjectInfo &object, const Point &camerapos);

	void addFramecount();

	/////////////////コンフィグ用
	void changeKey(bool isRightHand);
};