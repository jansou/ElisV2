# include "ELPlayer.h"

//余計な変数を作らない

ELPlayer::ELPlayer()
	:m_elisRect(100,100,128,128)//100,100は暫定初期位置
	,m_elisState(ElisState::Staying)
	, m_elisAttackState(ElisAttackState::Stay)
	,m_oldState(m_elisState)
	,m_elisFace(Right)
	,m_speed(0)
	,m_riseOffset(3)
	,m_gravity(5)
	,m_jumpH(0)
	,m_frameCount(0)
	, m_chargeFrameCount(0)
	,m_jumped(false)
	,m_keyRight(Input::KeyD)
	,m_keyLeft(Input::KeyA)
	,m_keyUp(Input::KeyW)
	,m_keyDown(Input::KeyS)
	,m_mouseLeft(Input::MouseL)
	,m_elisHitRect(44,54,40,74)//当たり判定の箇所。画像的な座標を示す
	,m_life(20)//とりあえずHP
	,m_maxLife(20)
	,m_invincibled(false)
	,m_invincibleTime(0)
	,m_elisMagicType(ElisMagicType::Normal)
	//,m_maxMP(100)
	,m_MP(0)
	//,m_MPframeCount(0)
{
	//コンストラクタの関数内での初期化はただの代入
	//初期化は初期化リストで
}

namespace
{
	const int kMaxSpeed = 5;
	const int kMinSpeed = 0;
	const int kGravity0 = 5;
	const int kMaxGravity = 10;
	const int kMaxJumpH = 27;//ジャンプ初速度
	const int kDrop = 1;//ジャンプ速度の減少度兼重力加速度
	const int kDamagedTime=20;
	const int kInvincibleTime=60;
	const int kDeadTime = 64;
	const int kMPperSec = 48;

	int height(const String name)
	{
		return	TextureAsset(name).height;
	}

	const int kAttackingTime = 31;
	const int kAnimeTiming = 8;

	const int kChargeTime = 60;

	const useMP kUseMP;

	const int kMPStone = kUseMP.MPStone;//25;
	const int kMPIce = kUseMP.MPIce;//15;
	const int kMPNormal = kUseMP.MPNormal;//10;
	const int kMPFire = kUseMP.MPFire;//25;
	const int kMPWind = kUseMP.MPWind;//15;

	const int kMPGainTime = 30;
	const int kMPGain = 1;

	const double kFireSpeed = 15.0;
};

bool ELPlayer::init()
{
	/*
	struct ReSourceInfo
	{
		char* name;
		char* file;
	};

	ReSourceInfo ReSourceInfoTbl[] = {
		{"texElis_readyCharging","data/Elis/Player/elis_readyCharging.png"},
		{"texElis_readyCharging","data/Elis/Player/elis_readyCharging.png"},
		{"texElis_readyCharging","data/Elis/Player/elis_readyCharging.png"},
		{"texElis_readyCharging","data/Elis/Player/elis_readyCharging.png"},
	};

	for(int resourceIndex = 0; i < sizeof() / sizeof(); resourceIndex++)
	{
		TextureAsset::Register(ReSourceInfoTbl[resourceIndex].name,ReSourceInfoTbl[resourceIndex].file);
	}
	*/

	TextureAsset::Register(L"texElis_readyCharging",L"data/Elis/Player/elis_readyCharging.png");
	TextureAsset::Register(L"texELStick_readyCharging",L"data/Elis/Player/stick_readyCharging.png");
	TextureAsset::Register(L"texElis_charge",L"data/Elis/Player/elis_charge.png");
	TextureAsset::Register(L"texELStick_charge",L"data/Elis/Player/stick_charge.png");
	TextureAsset::Register(L"texElis_swing",L"data/Elis/Player/elis_swing.png");
	TextureAsset::Register(L"texELStick_swing",L"data/Elis/Player/stick_swing.png");
	TextureAsset::Register(L"texElis_damaged",L"data/Elis/Player/elis_damaged.png");
	TextureAsset::Register(L"texElis_dead",L"data/Elis/Player/elis_dead.png");
	TextureAsset::Register(L"ELcharge1",L"data/Elis/Player/charge1.png");
	TextureAsset::Register(L"ELcharge2",L"data/Elis/Player/charge2.png");
	TextureAsset::Register(L"ELcharge3",L"data/Elis/Player/charge3.png");
	TextureAsset::Register(L"ELBcharge1",L"data/Elis/Player/Bcharge1.png");
	TextureAsset::Register(L"ELBcharge2",L"data/Elis/Player/Bcharge2.png");
	TextureAsset::Register(L"ELBcharge3",L"data/Elis/Player/Bcharge3.png");
	TextureAsset::Register(L"ELScharge1",L"data/Elis/Player/Scharge1.png");
	TextureAsset::Register(L"ELScharge2",L"data/Elis/Player/Scharge2.png");
	TextureAsset::Register(L"ELScharge3",L"data/Elis/Player/Scharge3.png");
	TextureAsset::Register(L"ELRcharge1",L"data/Elis/Player/Rcharge1.png");
	TextureAsset::Register(L"ELRcharge2",L"data/Elis/Player/Rcharge2.png");
	TextureAsset::Register(L"ELRcharge3",L"data/Elis/Player/Rcharge3.png");
	TextureAsset::Register(L"ELGcharge1",L"data/Elis/Player/Gcharge1.png");
	TextureAsset::Register(L"ELGcharge2",L"data/Elis/Player/Gcharge2.png");
	TextureAsset::Register(L"ELGcharge3",L"data/Elis/Player/Gcharge3.png");

	SoundAsset::Register(L"sndElisJump",L"data/Elis/SE/Elis/ElisJump.mp3");//跳躍音
	SoundAsset::Register(L"sndElisLand",L"data/Elis/SE/Elis/ElisLand.mp3");//着地音
	SoundAsset::Register(L"sndElisCharge",L"data/Elis/SE/Elis/ElisCharge.mp3");//溜め中
	SoundAsset::Register(L"sndElisDead",L"data/Elis/SE/Elis/ElisDead.mp3");//死亡中
	SoundAsset::Register(L"sndElisDamaged",L"data/Elis/SE/Elis/ElisDamaged.mp3");//くらった
	SoundAsset::Register(L"sndElisIce",L"data/Elis/SE/Elis/ElisIce.mp3");//氷魔法
	SoundAsset::Register(L"sndELGetItem",L"data/Elis/SE/System/getItem.mp3");
	SoundAsset::Register(L"sndELMagicShoot",L"data/Elis/SE/Elis/ElisMagicShoot.mp3");
	SoundAsset::Register(L"sndElisStone",L"data/Elis/SE/Elis/ElisStone.mp3");
	SoundAsset::Register(L"sndElisFire",L"data/Elis/SE/Elis/ElisFire.mp3");
	SoundAsset::Register(L"sndElisWind",L"data/Elis/SE/Elis/ElisWind.mp3");

	TextureAsset::Register(L"texElisNormal",L"data/Elis/Player/elis_stay.png");
	TextureAsset::Register(L"texElisRun",L"data/Elis/Player/elis_move.png");
	TextureAsset::Register(L"texElisJump",L"data/Elis/Player/elis_jump.png");
	TextureAsset::Register(L"texElisFire",L"data/Elis/Player/elis_fire.png");

	return TextureAsset(L"texElisNormal") 
		&& TextureAsset(L"texElisRun") 
		&& TextureAsset(L"texElisJump")
		&& SoundAsset(L"sndElisJump")
		&& SoundAsset(L"sndElisLand")
		&& SoundAsset(L"sndElisCharge")
		&& SoundAsset(L"sndElisDead")
		&& SoundAsset(L"sndElisDamaged")
		&& SoundAsset(L"sndElisIce")
		&& SoundAsset(L"sndELGetItem")
		&& SoundAsset(L"sndELMagicShoot")
		&& TextureAsset(L"texElis_readyCharging") 
		&& TextureAsset(L"texELStick_readyCharging")
		&& TextureAsset(L"texElis_charge") 
		&& TextureAsset(L"texELStick_charge")
		&& TextureAsset(L"texElis_swing") 
		&& TextureAsset(L"texELStick_swing")
		&& TextureAsset(L"texElis_damaged") 
		&& TextureAsset(L"texElis_dead")
		&& TextureAsset(L"ELcharge1") 
		&& TextureAsset(L"ELcharge2")
		&& TextureAsset(L"ELcharge3");
}

bool ELPlayer::intersectBody(const Point& foot,const Point& head, const ELMap& map, const ELFloorType& floorType)//通常床と身体の接触判定
{

	for(int i=1;i<foot.y-head.y;++i)
	{
		if(map.isFloor(foot.x,foot.y-i) && map.getFloorType(foot.x,foot.y-i)==floorType)
		{
			return true;
		}
	}

	return false;
}

//基本的に1画面に収まらない程長い関数は作らない
ELPlayState ELPlayer::update(ELMap& map, ELAttack& attack, const ELMyCursor& cursor, 
					  const ELEnemyInfo& enemy, ELObjectInfo& object, const Point& camerapos )
{
	if(m_life <= 0)
	{
		if(m_elisState != ElisState::Dead)
			DeadProcess(map);

		m_elisState = ElisState::Dead;
		
	}
	/*
	if(m_MP != m_maxMP)
	{
		++m_MPframeCount;
		if(m_MPframeCount == kMPGainTime)
		{
			if(m_elisState == ElisState::Charge)
				m_MP = Min(m_MP + kMPGain*2, m_maxMP);
			else
				m_MP = Min(m_MP + kMPGain, m_maxMP);

			m_MPframeCount = 0;
		}
	}
	*/
	m_oldState = m_elisState;

	if(m_elisState == ElisState::Dead)
	{
		if(!SoundAsset(L"sndElisDead").isPlaying)	SoundAsset(L"sndElisDead").play();

		m_frameCount += 1;

		if(m_frameCount > kDeadTime)	return ELPlayState::GameOver;

		return ELPlayState::Playing;
	}
	else if(m_elisState == ElisState::Damaged)
	{
		if(m_frameCount < kDamagedTime)
		{
			DamageProcess(map, object, camerapos);
			m_frameCount += 1;
			return ELPlayState::Playing;
		}
	}
	else if(m_elisState == ElisState::Fire)
	{
		FireProcess(map, object, camerapos, attack, cursor);
		//++m_frameCount;
	}
	/*
	else if(m_elisState == ElisState::Attack)
	{
		if(m_frameCount < kAttackingTime)
		{
			AttackProcess(map, object, camerapos);
			m_frameCount += 1;
			return ELPlayState::Playing;
		}
	}
	else if(m_elisState==ElisState::Charge)
	{
		ChargeProcess(attack, cursor, camerapos, object);

		if(m_elisState == ElisState::Attack)	return ELPlayState::Playing;
	}
	*/

	//チャージ処理////////////////////
	if (m_elisState!=ElisState::Dead)
	{
		if (m_elisAttackState == ElisAttackState::Attack)
		{
			if (m_frameCount < kAttackingTime)
			{
				AttackProcess(map, object, camerapos);
				m_frameCount += 1;
				//return ELPlayState::Playing;
			}
			else
			{
				m_elisAttackState = ElisAttackState::Stay;
			}
		}
		else if (m_elisAttackState == ElisAttackState::Charge)
		{
			ChargeProcess(attack, cursor, camerapos, object);
			
			++m_chargeFrameCount;
			//if (m_elisState == ElisAttackState::Attack)	return ELPlayState::Playing;
		}
	}
	////////////////////////////

	if(m_elisState != ElisState::Fire)
	{
		MoveXAxis(map,object, camerapos);
		MoveYAxis(map,object,camerapos);
	}

	if(m_invincibled)
	{
		m_invincibleTime += 1;

		if(m_invincibleTime == kInvincibleTime)
		{
			m_invincibled = false;
			m_invincibleTime = 0;
		}
	}

	if(m_oldState != m_elisState)
	{
		resetCount();
	}

	m_frameCount += 1;

	return ELPlayState::Playing;
}


//ゲームオーバー後にすべてを初期化してリスタートするプロセス（保留にする）
void ELPlayer::restartProcess(const ELMap& map, const ELEnemyInfo& enemy)
{
	//プレイヤーの初期化
	//m_elisRect.pos = map.getStartPos();
	m_elisRect.pos = map.getStartPos()+Point(-64,-128);
	m_frameCount = 0;
	m_chargeFrameCount = 0;
	m_life = m_maxLife;
	//m_MP = m_maxMP;
	m_speed = 0;
	m_elisState = ElisState::Staying;
	m_elisAttackState = ElisAttackState::Stay;
	m_elisMagicType = ElisMagicType::Normal;
	m_invincibled = false;
}

void ELPlayer::AttackProcess(const ELMap& map, ELObjectInfo& object, const Point &camerapos)
{
	//FallProcess(map, object, camerapos);
}

void ELPlayer::FireProcess(const ELMap& map, ELObjectInfo& object, const Point &camerapos, ELAttack &attack, const ELMyCursor &cursor)
{
	Vec2 dir = Vec2(m_endPos - m_startPos).normalize();

	Point foot = footCollisionPos();
	Point head = headCollisionPos();


	for(int i=1;i<=kFireSpeed;++i)
	{

		if(intersectBody(Point(int(foot.x+dir.x*i),int(foot.y+dir.y*i)),
			Point(int(head.x+dir.x*i),int(head.y+dir.y*i)),map,ELFloorType::NormalFloor)
			||object.isTouchable(Point(int(head.x+dir.x*i),int(head.y+dir.y*i)),Point(int(foot.x+dir.x*i),int(foot.y+dir.y*i)),camerapos))
		{			
			m_elisRect.moveBy(Point(int(dir.x*(i-1)),int(dir.y*(i-1))));
			//m_elisRect.setCenter(Point(int(m_elisRect.center.x + dir.x*(i-1)),int(m_elisRect.center.y + dir.y*(i-1))));

			break;		
		}
		else if(i==kFireSpeed)
		{
			//m_elisRect.setCenter(Point(int(m_elisRect.center.x + dir.x*kFireSpeed),int(m_elisRect.center.y + dir.y*kFireSpeed)));

			m_elisRect.moveBy(Point(int(dir.x*kFireSpeed),int(dir.y*kFireSpeed)));
			break;
		}
	}

	if(m_frameCount % 4 == 0)
	{
		const Point startpos(m_elisRect.center.x,footCollisionPos().y-m_elisHitRect.size.y/2);
		const Point endpos = startpos+Point(int(dir.x*10),int(dir.y*10));
		attack.makeShoot(ELShooter::Player, ELShootType::ElisFire,startpos,endpos);
	}

	if(m_frameCount >60)
	{
		m_elisState = ElisState::Jumping;
		m_elisAttackState = ElisAttackState::Stay;
	}
}

void ELPlayer::DamageProcess(const ELMap& map , ELObjectInfo& object, const Point &camerapos)
{
	const int dir = (m_elisFace==ElisFace::Left ? 1 : -1);

	for(int i=0;i<2;++i)
	{
		for(int j=0;j<=m_riseOffset;++j)
		{
			const Point foot = footCollisionPos();
			const Point head = headCollisionPos();
			if(!(map.isFloor(foot.x+dir,foot.y-j-1)) 
				&& !intersectBody(
				Point(foot.x+dir,foot.y-j-1),
				Point(head.x+dir,head.y-j-1),
				map,ELFloorType::NormalFloor))
			{
				if(!object.isTouchable(
				Point(head.x+dir,head.y-j-1),
				Point(foot.x+dir,foot.y-j-1),camerapos))
				{
				m_elisRect.moveBy(Point(dir,-j));
				break;
				}
			}
		}	
	}
		
	FallProcess(map, object, camerapos);
}

void ELPlayer::ChargeProcess(ELAttack& attack, const ELMyCursor& cursor, const Point& camerapos, ELObjectInfo &object)
{
	m_elisFace = (m_elisRect.center.x > cursor.getCursorPos().x+camerapos.x ? ElisFace::Left : ElisFace::Right);

	if(!m_mouseLeft.pressed)
	{
		if(SoundAsset(L"sndElisCharge").isPlaying)	SoundAsset(L"sndElisCharge").stop();

		const Point startpos(m_elisRect.center.x,footCollisionPos().y-m_elisHitRect.size.y/2);
		const Point endpos = cursor.getCursorPos()+camerapos;

		switch (m_elisMagicType)
		{
			case ElisMagicType::Normal:
			{
				SoundAsset(L"sndELMagicShoot").play();

				//if(m_MP >= kMPNormal)
				{
					if (/*m_frameCount*/m_chargeFrameCount < kChargeTime)
					{
						attack.makeShoot(ELShooter::Player,	ELShootType::ElisNormalShot1,startpos,endpos);
					}
					else if (/*m_frameCount*/m_chargeFrameCount < kChargeTime * 2)
					{
						attack.makeShoot(ELShooter::Player, ELShootType::ElisNormalShot2,startpos,endpos);
					}
					else
					{
						attack.makeShoot(ELShooter::Player, ELShootType::ElisNormalShot3,startpos,endpos);
					}

					//m_MP -= kMPNormal;
				}

				break;
			}
			case ElisMagicType::Ice:
				{
					//if(m_MP >= kMPIce)
					{
						if(SoundAsset(L"sndElisIce").isPlaying)	SoundAsset(L"sndElisIce").stop();

						SoundAsset(L"sndElisIce").play();

						attack.makeShoot(ELShooter::Player, ELShootType::ElisIce,startpos,endpos);

						//m_MP -= kMPIce;
					}
					break;
				}
			case ElisMagicType::Wind:
				{
					//if(m_MP >= kMPWind)
					{
						if(SoundAsset(L"sndElisWind").isPlaying)	SoundAsset(L"sndElisWind").stop();

						SoundAsset(L"sndElisWind").play();

						attack.makeShoot(ELShooter::Player, ELShootType::ElisWind,startpos,endpos);

						//m_MP -= kMPWind;
					}

					break;
				}
			case ElisMagicType::Stone:
				{
						//if(m_MP >= kMPStone)
						{
						if(SoundAsset(L"sndElisStone").isPlaying)	SoundAsset(L"sndElisStone").stop();

						SoundAsset(L"sndElisStone").play();

						object.addObject(ELObjectType::magicBlock, cursor.getCursorPos()+Point(0,32)+camerapos);

						//m_MP -= kMPStone;
					}
					break;
				}
			case ElisMagicType::Fire:
				{
					//if(m_MP >= kMPFire)
					{
						if(SoundAsset(L"sndElisFire").isPlaying)	SoundAsset(L"sndElisFire").stop();

						SoundAsset(L"sndElisFire").play();

						//attack.makeShoot(ELShooter::Player, ELShootType::ElisFire,startpos,endpos);

						m_startPos = Point(m_elisRect.center.x,footCollisionPos().y-m_elisHitRect.size.y/2);
						m_endPos = cursor.getCursorPos()+camerapos;
						m_elisState = ElisState::Fire;
						m_frameCount = 0;
						m_chargeFrameCount = 0;
						m_elisAttackState = ElisAttackState::Fire;

						//m_MP -= kMPFire;

						return;
					}
					break;
				}
			default:
				return;
		}

		//m_elisState = ElisState::Attack;
		m_elisAttackState = ElisAttackState::Attack;
		m_frameCount = 0;
		m_chargeFrameCount = 0;
		return;
	}

	if(m_mouseLeft.pressed)
	{
		if(!SoundAsset(L"sndElisCharge").isPlaying)	SoundAsset(L"sndElisCharge").play();//チャージ音
		
		return;
	}	
}

void ELPlayer::MoveXAxis(const ELMap& map, ELObjectInfo& object, const Point &camerapos)
{
	bool inputed=false;
	
	if(!(m_keyRight.pressed && m_keyLeft.pressed))//同時押し回避
	{
		if(m_keyRight.pressed)///////////////右に進むキー
		{
			m_elisFace = Right;
			inputed = true;
			m_speed = Min(m_speed+1,kMaxSpeed);
		}
		if(m_keyLeft.pressed)////////////////左に進むキー
		{
			m_elisFace = Left;
			inputed = true;
			m_speed = Max(m_speed-1,-kMaxSpeed);
		}
	}

	if(!inputed)//なんも押さないと…
	{
		m_speed = Clamp(m_speed-1,kMinSpeed,m_speed+1);
	}
	
	const int dir = (0<=m_speed ? 1 : -1);	

	for(int i=0;i<std::abs(m_speed);++i)
	{
		for(int j=0;j<=m_riseOffset;++j)
		{
			const Point foot = footCollisionPos();
			const Point head = headCollisionPos();

			if(!map.isFloor(foot.x+dir,foot.y-j-1)
				&& !intersectBody(Point(foot.x+dir,foot.y-j-1),Point(head.x+dir,head.y-j-1),map,ELFloorType::NormalFloor)
				)
			{
				if(!object.isTouchable(Point(head.x+dir,head.y-j),Point(foot.x+dir,foot.y-j),camerapos))
				{
				m_elisRect.moveBy(Point(dir,-j));
				break;
				}
			}
		}	
	}
}

void ELPlayer::MoveYAxis(ELMap& map, ELObjectInfo& object, const Point &camerapos)
{
	JumpProcess(map, object, camerapos);

	PassFloorProcess(map);

	FallProcess(map, object,camerapos);

	IdentifyState(map, object, camerapos);

}

void ELPlayer::JumpProcess(const ELMap& map, ELObjectInfo& object, const Point &camerapos)
{
	const Point head = headCollisionPos();
	const Point foot = footCollisionPos();

	if(m_keyUp.clicked && m_elisState != ElisState::Jumping)//ジャンプした状態にする
	{
		//if(!m_jumped)	
		if(SoundAsset(L"sndElisJump").isPlaying)
			SoundAsset(L"sndElisJump").stop();

		SoundAsset(L"sndElisJump").play();

		m_jumpH = kMaxJumpH;
		m_jumped = true;
	}

	if(m_keyUp.pressed && m_jumped)//ジャンプする
	{
		if(m_jumpH>0 && m_jumped)
		{
			for(int i=1;i<=m_jumpH;++i)
			{
				if(map.isFloor(head.x,head.y-i) 
					|| object.isTouchable(Point(head.x,head.y-i),Point(foot.x,foot.y-i),camerapos))
				{
					if(map.getFloorType(head.x,head.y-i) == ELFloorType::NormalFloor
						|| object.isTouchable(Point(head.x,head.y-i),Point(foot.x,foot.y-i),camerapos))//床なら
					{
						m_jumpH = i-1;
						break;
					}
				}
			}
			m_elisRect.y -= m_jumpH;

			//LOGFMT_DBG(m_jumpH);

			m_jumpH -= kDrop;
		}
	}
	else if(!m_keyUp.pressed && m_jumped)//
	{
		m_jumpH = 0;
	}
}

void ELPlayer::PassFloorProcess(const ELMap& map)
{
	const Point head = headCollisionPos();
	const Point foot = footCollisionPos();

	if(m_keyDown.clicked 
		&& map.isFloor(foot.x,foot.y)
		&& map.getFloorType(foot.x,foot.y) == ELFloorType::PassFloor)//
	{
		m_elisRect.y += 1;
	}
}

void ELPlayer::FallProcess(const ELMap& map, ELObjectInfo& object, const Point &camerapos)
{
	const Point head = headCollisionPos();
	const Point foot = footCollisionPos();

	//落下に関する部分
	if(m_jumped && m_jumpH-m_gravity >0)
	{
		m_elisRect.y += m_gravity;
		return;
	}

	for(int i=0;i<=m_gravity;++i)
	{
		if(object.isTouchable(Point(head.x,head.y+i),Point(foot.x,foot.y+i),camerapos)
			|| map.isFloor(foot.x,foot.y+i) )
		{
			m_elisRect.y += i;
			return;
		}
	}

	m_elisRect.y += m_gravity;
}

void ELPlayer::IdentifyState(ELMap& map, ELObjectInfo& object, const Point &camerapos)
{
	const Point head = headCollisionPos();
	const Point foot = footCollisionPos();

	if(intersectBody(Point(footCollisionPos().x,footCollisionPos().y+1),
				Point(headCollisionPos().x,headCollisionPos().y),
				map,ELFloorType::DeathFloor))
	{
		DeadProcess(map);
		return;
	}

	if (m_elisState != ElisState::Fire
		&& m_elisAttackState!=ElisAttackState::Attack
		&& m_mouseLeft.clicked)
	{
		m_elisAttackState = ElisAttackState::Charge;
		//return;
	}

	//状態の判定
	/*
	if((m_elisState == ElisState::Staying || m_elisState == ElisState::Charge) 
		&& m_mouseLeft.clicked)
	{
		m_elisState = ElisState::Charge;
		return;
	}
	
	if(m_oldState == ElisState::Charge && m_elisState == ElisState::Attack && m_elisState == ElisState::Fire)
	{
		return;
	}
	*/
	if(!(object.isTouchable(head,foot, camerapos) || (map.isFloor(foot.x,foot.y)))
		|| m_jumpH-m_gravity >0)//
	{
		m_elisState = ElisState::Jumping;
		m_gravity = Min(m_gravity + kDrop, kMaxGravity);
		return;
	}

	m_jumped = false;
	m_gravity = kGravity0;
	m_jumpH = 0;

	if(m_speed!=0)
	{
		if(m_elisState == ElisState::Jumping)	SoundAsset(L"sndElisLand").play();//////着地音
		
		m_elisState = ElisState::Running;
		return;
	}

	//if(m_elisState!=ElisState::Charge)
	{
		if(m_elisState == ElisState::Jumping)	SoundAsset(L"sndElisLand").play();//////着地音

		m_elisState = ElisState::Staying;
		return;
	}
}

void ELPlayer::DeadProcess(ELMap &map)
{
	map.stopStageBGM();
	m_elisState = ElisState::Dead;
	m_elisAttackState = ElisAttackState::Stay;
	m_jumped = false;
	m_gravity = kGravity0;
	m_jumpH = 0;
	m_frameCount = 0;
	m_chargeFrameCount = 0;
}

void ELPlayer::resetCount()
{
	m_frameCount = 0;
}

void ELPlayer::draw(const Point& cameraPos)const
{
	const Point pos = m_elisRect.pos - cameraPos;
	const double x=128*m_elisFace,y=128,w=m_elisRect.w,h=m_elisRect.h;
	const int a = 255 -100*m_invincibled;
	const Rect stickRect(m_elisRect.pos.x-64, m_elisRect.pos.y -128, 256, 256);
	const Point stickPos = stickRect.pos - cameraPos;
	const Rect drawStickRect(m_elisFace*stickRect.size.x,0,stickRect.size);

	//攻撃モーション
	if(m_elisState != ElisState::Fire && m_elisState != ElisState::Dead
		&& m_elisAttackState == ElisAttackState::Attack)
	{
		TextureAsset(L"texElis_swing")
			(x, y*((m_frameCount / kAnimeTiming) % (height(L"texElis_swing") / int(h))), w, h).
			draw(pos, Alpha(a));

		TextureAsset(L"texELStick_swing")
			(drawStickRect.pos.x,
			256 * ((m_frameCount) / kAnimeTiming),
			drawStickRect.size.x,
			drawStickRect.size.y).
			draw(stickPos, Alpha(a));
	}
	else
	{
		switch (m_elisState)
		{
		case ElisState::Staying:
			TextureAsset(L"texElisNormal")(x, y*((m_frameCount / kAnimeTiming) % int(TextureAsset(L"texElisNormal").height / y)), w, h).draw(pos, Alpha(a));
			break;
		case ElisState::Running:
			TextureAsset(L"texElisRun")(x, y*((m_frameCount / kAnimeTiming) % int(TextureAsset(L"texElisNormal").height / y)), w, h).draw(pos, Alpha(a));
			break;
		case ElisState::Jumping:
			TextureAsset(L"texElisJump")(x, y, w, h).draw(pos, Alpha(a));
			break;
		case ElisState::Falling:
			TextureAsset(L"texElisJump")(x, y, w, h).draw(pos, Alpha(a));
			break;/*
		case ElisState::Charge:
		if(m_frameCount < 2)
		{
		TextureAsset(L"texElis_readyCharging")(x,y,w,h).draw(pos,Alpha(a));
		TextureAsset(L"texELStick_readyCharging")(drawStickRect).draw(stickPos,Alpha(a));
		}
		else
		{
		TextureAsset(L"texElis_charge")
		(x,y*((m_frameCount/kAnimeTiming)%(height(L"texElis_charge")/(int)y)),w,h).
		draw(pos,Alpha(a));

		//assert(kAnimeTiming!=0);
		//assert(height(L"texELStick_charge")!=0);
		//assert(drawStickRect.pos.y!=0);

		TextureAsset(L"texELStick_charge")
		(drawStickRect.pos.x,
		drawStickRect.pos.y*(((m_frameCount/kAnimeTiming)%(height(L"texELStick_charge"))/drawStickRect.size.y)),
		drawStickRect.size.x,
		drawStickRect.size.y).
		draw(stickPos,Alpha(a));

		if(m_elisMagicType == ElisMagicType::Normal)
		{
		if(m_frameCount>kChargeTime*2)
		{
		TextureAsset(L"ELcharge3")
		(0,
		256*((m_frameCount/kAnimeTiming)%(height(L"ELcharge3")/drawStickRect.size.y)-1),
		256,
		256
		).draw(stickPos,Alpha(a));

		return;
		}

		TextureAsset(L"ELcharge1")
		(0,
		256*((m_frameCount/kAnimeTiming)%(height(L"ELcharge1")/drawStickRect.size.y)-1),
		256,
		256
		).draw(stickPos,Alpha(a));

		if(m_frameCount>kChargeTime)
		{
		TextureAsset(L"ELcharge2")
		(0,
		256*((m_frameCount/kAnimeTiming)%(height(L"ELcharge2")/drawStickRect.size.y)-1),
		256,
		256
		).draw(stickPos,Alpha(a));
		}
		}
		if(m_elisMagicType == ElisMagicType::Stone)
		{
		if(m_frameCount>kChargeTime*2)
		{
		TextureAsset(L"ELScharge3")
		(0,
		256*((m_frameCount/kAnimeTiming)%(height(L"ELScharge3")/drawStickRect.size.y)-1),
		256,
		256
		).draw(stickPos,Alpha(a));

		return;
		}

		TextureAsset(L"ELScharge1")
		(0,
		256*((m_frameCount/kAnimeTiming)%(height(L"ELScharge1")/drawStickRect.size.y)-1),
		256,
		256
		).draw(stickPos,Alpha(a));

		if(m_frameCount>kChargeTime)
		{
		TextureAsset(L"ELScharge2")
		(0,
		256*((m_frameCount/kAnimeTiming)%(height(L"ELScharge2")/drawStickRect.size.y)-1),
		256,
		256
		).draw(stickPos,Alpha(a));
		}

		}
		else if(m_elisMagicType == ElisMagicType::Ice)
		{
		if(m_frameCount>kChargeTime*2)
		{
		TextureAsset(L"ELBcharge3")
		(0,
		256*((m_frameCount/kAnimeTiming)%(height(L"ELBcharge3")/drawStickRect.size.y)-1),
		256,
		256
		).draw(stickPos,Alpha(a));

		return;
		}

		TextureAsset(L"ELBcharge1")
		(0,
		256*((m_frameCount/kAnimeTiming)%(height(L"ELBcharge1")/drawStickRect.size.y)-1),
		256,
		256
		).draw(stickPos,Alpha(a));

		if(m_frameCount>kChargeTime)
		{
		TextureAsset(L"ELBcharge2")
		(0,
		256*((m_frameCount/kAnimeTiming)%(height(L"ELBcharge2")/drawStickRect.size.y)-1),
		256,
		256
		).draw(stickPos,Alpha(a));
		}
		}
		else if(m_elisMagicType == ElisMagicType::Fire)
		{
		if(m_frameCount>kChargeTime*2)
		{
		TextureAsset(L"ELRcharge3")
		(0,
		256*((m_frameCount/kAnimeTiming)%(height(L"ELRcharge3")/drawStickRect.size.y)-1),
		256,
		256
		).draw(stickPos,Alpha(a));

		return;
		}

		TextureAsset(L"ELRcharge1")
		(0,
		256*((m_frameCount/kAnimeTiming)%(height(L"ELRcharge1")/drawStickRect.size.y)-1),
		256,
		256
		).draw(stickPos,Alpha(a));

		if(m_frameCount>kChargeTime)
		{
		TextureAsset(L"ELRcharge2")
		(0,
		256*((m_frameCount/kAnimeTiming)%(height(L"ELRcharge2")/drawStickRect.size.y)-1),
		256,
		256
		).draw(stickPos,Alpha(a));
		}
		}
		else if(m_elisMagicType == ElisMagicType::Wind)
		{
		if(m_frameCount>kChargeTime*2)
		{
		TextureAsset(L"ELGcharge3")
		(0,
		256*((m_frameCount/kAnimeTiming)%(height(L"ELGcharge3")/drawStickRect.size.y)-1),
		256,
		256
		).draw(stickPos,Alpha(a));

		return;
		}

		TextureAsset(L"ELGcharge1")
		(0,
		256*((m_frameCount/kAnimeTiming)%(height(L"ELGcharge1")/drawStickRect.size.y)-1),
		256,
		256
		).draw(stickPos,Alpha(a));

		if(m_frameCount>kChargeTime)
		{
		TextureAsset(L"ELGcharge2")
		(0,
		256*((m_frameCount/kAnimeTiming)%(height(L"ELGcharge2")/drawStickRect.size.y)-1),
		256,
		256
		).draw(stickPos,Alpha(a));
		}
		}
		}
		break;
		case ElisState::Attack:
		{
		TextureAsset(L"texElis_swing")
		(x,y*((m_frameCount/kAnimeTiming)%(height(L"texElis_swing")/int(h))),w,h).
		draw(pos,Alpha(a));

		TextureAsset(L"texELStick_swing")
		(drawStickRect.pos.x,
		256*((m_frameCount)/kAnimeTiming),
		drawStickRect.size.x,
		drawStickRect.size.y).
		draw(stickPos,Alpha(a));
		break;
		}*/
		case ElisState::Fire:
		{
			TextureAsset(L"texElisFire")
				(x, 0, w, h).draw(pos);

			break;
		}
		case ElisState::Damaged:
			TextureAsset(L"texElis_damaged")(x, y, w, h).draw(pos, Alpha(a));
			break;
		case ElisState::Dead:
			if (m_frameCount < kDeadTime)
			{
				TextureAsset(L"texElis_dead")
					(x, y*((m_frameCount / kAnimeTiming) % (height(L"texElis_dead") / int(h))), w, h).draw(pos);
			}
			break;
		default:
			break;
		}
	}
	//Resource::Font(L"Common").draw(Format()+L"体力："+m_life + L"MP" + m_MP + L"フレーム："+m_frameCount,80,0,Palette::Black);

	//Rect(m_elisRect.pos + m_elisHitRect.pos-cameraPos,m_elisHitRect.size).draw(Color(100,100,100,100));

	//Rect(m_elisRect.pos+m_elisHitRect.pos,m_elisHitRect.size).drawFrame(10,10,Palette::Black);

	//Circle(Point(m_elisRect.center.x,footCollisionPos().y)-cameraPos,1).drawFrame(1,1,Palette::Orange);

	if (m_elisAttackState == ElisAttackState::Charge)
	{
		{
			/*
			TextureAsset(L"texElis_charge")
				(x, y*((m_chargeFrameCount / kAnimeTiming) % (height(L"texElis_charge") / (int)y)), w, h).
				draw(pos, Alpha(a));
				*/
			//assert(kAnimeTiming!=0);
			//assert(height(L"texELStick_charge")!=0);
			//assert(drawStickRect.pos.y!=0);

			if (m_elisAttackState == ElisAttackState::Charge)
			{
				if (m_elisMagicType == ElisMagicType::Normal)
				{
					if (m_chargeFrameCount > kChargeTime * 2)
					{
						TextureAsset(L"ELcharge3")
							(0,
							256 * ((m_chargeFrameCount / kAnimeTiming) % (height(L"ELcharge3") / drawStickRect.size.y) - 1),
							256,
							256
							).draw(stickPos, Alpha(a));

						return;
					}

					TextureAsset(L"ELcharge1")
						(0,
						256 * ((m_chargeFrameCount / kAnimeTiming) % (height(L"ELcharge1") / drawStickRect.size.y) - 1),
						256,
						256
						).draw(stickPos, Alpha(a));

					if (m_chargeFrameCount > kChargeTime)
					{
						TextureAsset(L"ELcharge2")
							(0,
							256 * ((m_chargeFrameCount / kAnimeTiming) % (height(L"ELcharge2") / drawStickRect.size.y) - 1),
							256,
							256
							).draw(stickPos, Alpha(a));
					}
				}
				if (m_elisMagicType == ElisMagicType::Stone)
				{
					if (m_chargeFrameCount > kChargeTime * 2)
					{
						TextureAsset(L"ELScharge3")
							(0,
							256 * ((m_chargeFrameCount / kAnimeTiming) % (height(L"ELScharge3") / drawStickRect.size.y) - 1),
							256,
							256
							).draw(stickPos, Alpha(a));

						return;
					}

					TextureAsset(L"ELScharge1")
						(0,
						256 * ((m_chargeFrameCount / kAnimeTiming) % (height(L"ELScharge1") / drawStickRect.size.y) - 1),
						256,
						256
						).draw(stickPos, Alpha(a));

					if (m_chargeFrameCount > kChargeTime)
					{
						TextureAsset(L"ELScharge2")
							(0,
							256 * ((m_chargeFrameCount / kAnimeTiming) % (height(L"ELScharge2") / drawStickRect.size.y) - 1),
							256,
							256
							).draw(stickPos, Alpha(a));
					}

				}
				else if (m_elisMagicType == ElisMagicType::Ice)
				{
					if (m_chargeFrameCount > kChargeTime * 2)
					{
						TextureAsset(L"ELBcharge3")
							(0,
							256 * ((m_chargeFrameCount / kAnimeTiming) % (height(L"ELBcharge3") / drawStickRect.size.y) - 1),
							256,
							256
							).draw(stickPos, Alpha(a));

						return;
					}

					TextureAsset(L"ELBcharge1")
						(0,
						256 * ((m_chargeFrameCount / kAnimeTiming) % (height(L"ELBcharge1") / drawStickRect.size.y) - 1),
						256,
						256
						).draw(stickPos, Alpha(a));

					if (m_chargeFrameCount > kChargeTime)
					{
						TextureAsset(L"ELBcharge2")
							(0,
							256 * ((m_chargeFrameCount / kAnimeTiming) % (height(L"ELBcharge2") / drawStickRect.size.y) - 1),
							256,
							256
							).draw(stickPos, Alpha(a));
					}
				}
				else if (m_elisMagicType == ElisMagicType::Fire)
				{
					if (m_chargeFrameCount > kChargeTime * 2)
					{
						TextureAsset(L"ELRcharge3")
							(0,
							256 * ((m_chargeFrameCount / kAnimeTiming) % (height(L"ELRcharge3") / drawStickRect.size.y) - 1),
							256,
							256
							).draw(stickPos, Alpha(a));

						return;
					}

					TextureAsset(L"ELRcharge1")
						(0,
						256 * ((m_chargeFrameCount / kAnimeTiming) % (height(L"ELRcharge1") / drawStickRect.size.y) - 1),
						256,
						256
						).draw(stickPos, Alpha(a));

					if (m_chargeFrameCount > kChargeTime)
					{
						TextureAsset(L"ELRcharge2")
							(0,
							256 * ((m_chargeFrameCount / kAnimeTiming) % (height(L"ELRcharge2") / drawStickRect.size.y) - 1),
							256,
							256
							).draw(stickPos, Alpha(a));
					}
				}
				else if (m_elisMagicType == ElisMagicType::Wind)
				{
					if (m_chargeFrameCount > kChargeTime * 2)
					{
						TextureAsset(L"ELGcharge3")
							(0,
							256 * ((m_chargeFrameCount / kAnimeTiming) % (height(L"ELGcharge3") / drawStickRect.size.y) - 1),
							256,
							256
							).draw(stickPos, Alpha(a));

						return;
					}

					TextureAsset(L"ELGcharge1")
						(0,
						256 * ((m_chargeFrameCount / kAnimeTiming) % (height(L"ELGcharge1") / drawStickRect.size.y) - 1),
						256,
						256
						).draw(stickPos, Alpha(a));

					if (m_chargeFrameCount > kChargeTime)
					{
						TextureAsset(L"ELGcharge2")
							(0,
							256 * ((m_chargeFrameCount / kAnimeTiming) % (height(L"ELGcharge2") / drawStickRect.size.y) - 1),
							256,
							256
							).draw(stickPos, Alpha(a));
					}
				}
			}

		}
	}
}

void ELPlayer::objectUpdate(const Point &camerapos, ELObjectInfo &object)
{
	const Point head = headCollisionPos();
	const Point foot = footCollisionPos();
	m_elisRect.pos += object.objectEffect(head,foot,camerapos);
}

Point ELPlayer::getPlayerFootCollisionPos() const
{
	return footCollisionPos();
}

void ELPlayer::checkHit(const ELEnemyInfo& enemyInfo, const ELAttack& attack)
{
	if(!m_invincibled && m_elisState != ElisState::Dead/* && m_elisState != ElisState::Fire*/)
	{
		int oldlife = m_life;

		Rect body(m_elisRect.pos+m_elisHitRect.pos,m_elisHitRect.size);

		m_life -= attack.getDamage(body,ELShooter::Player);//damageくらう

		if(m_life == oldlife)
		{
			m_life -= enemyInfo.getDamage(body);//damageくらう
		}

		if(m_life != oldlife)
		{
			if(!SoundAsset(L"sndElisDamaged").isPlaying)	SoundAsset(L"sndElisDamaged").play();

			m_elisState = ElisState::Damaged;
			//m_elisAttackState = ElisAttackState::Stay;
			m_frameCount = 0;
			m_invincibled = true;
			m_jumpH = 0;
			m_speed = 0;
		}
	}
	m_life = Max(m_life, 0);

	return;
}

bool ELPlayer::isDead() const
{
	return m_elisState == ElisState::Dead;
}

Rect ELPlayer::getRect() const
{
	return Rect(m_elisRect.pos + m_elisHitRect.pos,m_elisHitRect.size);
}

ElisState ELPlayer::getState() const
{
	return m_elisState;
}

void ELPlayer::setPos(const Point& pos)
{
	m_elisRect.setCenter(pos.x,pos.y - 64);
}

int ELPlayer::getLife() const
{
	return m_life;
}

void ELPlayer::itemEffect(const ELItemType &itemType)
{
	if(SoundAsset(L"sndELGetItem").isPlaying)
		SoundAsset(L"sndELGetItem").stop();

	switch (itemType)
	{
	case ELItemType::LifeUpS:
		SoundAsset(L"sndELGetItem").play();
		m_life = Min(m_life+4,m_maxLife);
		break;
	case ELItemType::LifeUpM:
		SoundAsset(L"sndELGetItem").play();
		m_life = Min(m_life+8,m_maxLife);
		break;
	case ELItemType::LifeUpL:
		SoundAsset(L"sndELGetItem").play();
		m_life = m_maxLife;
		break;
	case ELItemType::MagicUpS:
		SoundAsset(L"sndELGetItem").play();
		//m_MP = Min(m_MP+30,m_maxMP);
		m_MP += 1;
		break;
	case ELItemType::MagicUpM:
		SoundAsset(L"sndELGetItem").play();
		//m_MP = Min(m_MP+60,m_maxMP);
		m_MP += 5;
		break;
	case ELItemType::MagicUpL:
		SoundAsset(L"sndELGetItem").play();
		//m_MP = m_maxMP;
		m_MP += 10;
		break;
	default:
		break;
	}
}

void ELPlayer::setMagic(const ElisMagicType &magicType)
{
	m_elisMagicType = magicType;
}

///////イベント用コマンド

bool ELPlayer::walkto(int x, int speed, ELMap &map,ELObjectInfo &object, const Point &camerapos)
{
	if(m_elisState != ElisState::Running)
	{
		m_frameCount = 0;
		m_elisState = ElisState::Running;
		m_jumped = false;
		m_jumpH = 0;
	}

	if(m_elisRect.center.x > x)
	{
		m_elisFace = ElisFace::Left;

		m_elisRect.setCenter(Point(Max(m_elisRect.center.x-speed,x),m_elisRect.center.y));
	}
	else
	{
		m_elisFace = ElisFace::Right;

		m_elisRect.setCenter(Point(Min(m_elisRect.center.x+speed,x),m_elisRect.center.y));
	}

	
	FallProcess(map, object, camerapos);
	IdentifyState(map,object,camerapos);

	if(m_elisRect.center.x == x)
	{
		m_elisState = ElisState::Staying;
		m_speed = 0;
		m_frameCount = 0;
		return false;
	}

	m_elisState = ElisState::Running;

	return true;
}

bool ELPlayer::identyfybase(ELMap &map,ELObjectInfo &object, const Point &camerapos)
{
	
	if(m_elisState == ElisState::Fire)
	{
		m_elisState = ElisState::Jumping;
	}

	else if(m_elisState == ElisState::Jumping)
	{
		m_jumpH=0;
		m_speed=0;
	}
	else
	{
		m_invincibled = false;
		m_speed=0;
		m_elisState = ElisState::Staying;
	}

	FallProcess(map, object, camerapos);
	IdentifyState(map,object,camerapos);

	return m_elisState != ElisState::Staying;
}

void ELPlayer::addFramecount()
{
	m_frameCount += 1;
}

//////////////////////////////コンフィグ用
void ELPlayer::changeKey(bool isRightHand)
{
	if(isRightHand)
	{
		m_keyRight = Input::KeyD;
		m_keyLeft = Input::KeyA;
		m_keyUp = Input::KeyW;
		m_keyDown = Input::KeyS;
		m_mouseLeft = Input::MouseR;
	}
	else
	{
		m_keyRight = Input::KeyRight;
		m_keyLeft = Input::KeyLeft;
		m_keyUp = Input::KeyUp;
		m_keyDown = Input::KeyDown;
		m_mouseLeft = Input::MouseL;
	}
}