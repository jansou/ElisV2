# pragma once
# include <Siv3D.hpp>

const int ELWindowWidth = 1280;
const int ELWindowHeight = 720;

enum class ELMenuState
{//menuの選択肢に使う
	Base,
	Ask,
	Retry,
	Accept,
	null,
};


enum class ELTitleMode//タイトルの画面遷移状態
{
	Title,
	Config,
	null,
};

enum class ELCameraEffect
{
	LittleShake,//画面を揺らす
	null,
};

enum class ELCameraGraphicEffect//色的な効果
{
	WhiteBurn,//爆発
	null,
};

enum class ELEnemyState
{
	Ready,
	Staying,
	Moving,
	Jumping,
	Attacking,
	Damaged,
	Dead,
	Standby,
	null,
};


enum class ELFloorType
{
	NormalFloor,
	PassFloor,
	DeathFloor,
	Null,
};

struct useMP
{
	useMP()
	{
		MPStone = 25;
		MPIce = 15;
		MPNormal = 10;
		MPFire = 25;
		MPWind = 20;
	}

	 int MPStone;
	 int MPIce;
	 int MPNormal;
	 int MPFire;
	 int MPWind;
};

enum class ELGameState
{
	Title,
	Play,
	StageSelect,

};

enum class ELPlayState
{
	Playing,
	GameOver,
	Event,
	EditMagic,
	Clear,
};

//技名
enum class ELShootType
{
	ElisNormalShot1,
	ElisNormalShot2,
	ElisNormalShot3,
	ElisIce,
	ElisFire,
	ElisWind,
	Arrow,
	CurveAxe,
	SwordRain,
	Firebreath,
	Rockfall,
	UpperTentacle,
	LongIce,
	null,
};

enum class ElisMagicType
{
	Normal,
	Fire,
	Wind,
	Ice,
	Stone,
	null,
};

enum class ELShooter
{
	Player,
	Enemy,
};

enum class ELItemType
{
	LifeUpS,
	LifeUpM,
	LifeUpL,
	MagicUpS,
	MagicUpM,
	MagicUpL,
};

enum class ELObjectType
{
	block,
	magicBlock,
	movePlate,
};

enum class ElisState
{
	Staying,
	Running,
	Jumping,
	Falling,
	//Charge,
	//Attack,
	Damaged,
	Fire,
	Dead,
};

enum class ElisAttackState
{
	Stay,
	Charge,
	Attack,
	Fire,
};