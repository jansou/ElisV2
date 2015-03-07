
# pragma once
# include <Siv3D.hpp>
# include "ELEnemy\ELEnemy.h"
# include "ELMap.h"
# include "ELAttack.h"
# include "ELObjectInfo.h"
# include "ELItem.h"
# include "ELCommon.h"

# include "ELEnemy\ELSharkman.h"
# include "ELEnemy\ELSlime.h"
# include "ELEnemy\ELSlimeR.h"
# include "ELEnemy\ELSlimeG.h"
# include "ELEnemy\ELSlimeB.h"
# include "ELEnemy\ELSlimeD.h"
# include "ELEnemy\ELSlimeW.h"
# include "ELEnemy\ELRedcap.h"
# include "ELEnemy\ELBluecap.h"
# include "ELEnemy\ELDarkbrave.h"
# include "ELEnemy\ELAirsnake.h"
# include "ELEnemy\ELBoneman.h"
# include "ELEnemy\ELRaven.h"
# include "ELEnemy\ELRavenAcross.h"
# include "ELEnemy\ELBlockMaker.h"
# include "ELEnemy\ELTentacle.h"

//Boss
# include "ELEnemy\ELBigslime.h"
# include "ELEnemy\ELSeaserpent.h"
//

enum class ELEnemyName
{
	slime,
	slimeR,
	slimeG,
	slimeW,
	slimeB,
	slimeD,
	redcap,
	bluecap,
	sharkman,
	darkbrave,
	airsnake,
	boneman,
	raven,
	ravenacross,
	blockmaker,
	tantacle,
	////boss
	bigslime,
	seaserpent,
	//


	null,
};

enum class ELEnemySize
{
	S,
	M,
	L,
	null,
};

struct ELEnemyData
{
	ELEnemyData(ELEnemyName _Name,ELEnemySize _Size,Rect _hitRect,int _HP)
		:Name(_Name), Size(_Size), hitRect(_hitRect), HP(_HP)
	{
	}

	ELEnemyName Name;
	Rect hitRect;
	ELEnemySize Size;
	int HP;
};

enum ELEnemyFace
{
	EnemyLeft,
	EnemyRight,
};

struct ELEnemyPlace
{
	ELEnemyPlace(ELEnemyName _Name, Point _Pos)
		:Name(_Name), Pos(_Pos)
	{
		created = false;
	}

	ELEnemyName Name;
	Point Pos;//pos�͑�����������Ɏ��
	bool created;
};



class ELEnemyInfo
{
private:

	std::vector<ELEnemyPlace> m_enemyPlaces;//�G�̔z�u�̏��Ȃ�

	std::vector<ELEnemyData> m_enemyData;//�G�̏�����ʂ�܂Ƃ߂�

	std::vector<std::shared_ptr<ELEnemy>> m_enemies;//�G���˒����ɓ������炱���ɂԂ�����

	void eraseEnemy(int num);

	bool isBoss(const ELEnemyName& name)
	{
		return (name == ELEnemyName::darkbrave) 
			|| (name == ELEnemyName::bigslime)
			|| (name == ELEnemyName::seaserpent);
	}

	ELCameraEffect m_cameraEffect;

public:
	ELEnemyInfo();

	void update(const ELMap& map, const Point& playerpos, ELObjectInfo& object, const Point& camerapos, ELItem &item, ELAttack &attack);

	void draw(const Point& camerapos)const;

	bool loadEnemyData();//���ׂĂ̓G�̊�{����\�ߕۑ����Ă���

	bool setEnemysPlace(const String& mapName, const String& stageName);//�G�̔z�u��������̏����Z�b�g����

	void setEnemy(const ELEnemyName& Name, const Point& pos);//�G��ǉ�����

	ELEnemyData getEnemyData(const ELEnemyName& Name)const;//�f�[�^�̎擾

	ELEnemyName loadName(const String& Name)const;//file����ǂ񂾖��O��ϊ�����

	bool incamera(const Point& pos, const Point& camerapos)const;

	ELEnemySize loadSize(const String& enemysize)const;

	Point modifyDrawPos(const ELEnemySize& enemySize)const;

	int getDamage(const Rect& body) const;

	void checkHit(const ELAttack& attack);

	void clearEnemies();

	void clearEnemyPlaces();

	bool m_isBoss;
	ElisMagicType m_bossType;
	int m_bossLife;
	int m_bossMaxlife;

	bool getIsBoss() const{return m_isBoss;}
	ElisMagicType getbossType() const {return m_bossType;}
	int getBossLife() const {return m_bossLife;}
	int getBossMaxLife() const {return m_bossMaxlife;}

	ELCameraEffect getCameraEffect() const
	{
		return m_cameraEffect;
	}

	////////////�C�x���g�p
	bool ready();

	void addFramecount();

	bool enemyEmpty()
	{
		return m_enemies.size()==0;
	}

	Point getBurstPos() const;//��������ʒu�̒������擾

	bool enemyIsDead() const;
};