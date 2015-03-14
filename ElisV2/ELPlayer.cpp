# include "ELPlayer.h"

//余計な変数を作らない

ELPlayer::ELPlayer()
	:m_elisRect(100, 100, 128, 128)//100,100は暫定初期位置
	, m_elisState(ElisState::Staying)
	, m_elisAttackState(ElisAttackState::Stay)
	, m_oldState(m_elisState)
	, m_elisFace(Right)
	, m_speed(0)
	, m_riseOffset(3)
	, m_gravity(5)
	, m_jumpH(0)
	, m_frameCount(0)
	, m_chargeFrameCount(0)
	, m_jumped(false)
	, m_keyRight(Input::KeyD)
	, m_keyLeft(Input::KeyA)
	, m_keyUp(Input::KeyW)
	, m_keyDown(Input::KeyS)
	, m_mouseLeft(Input::MouseL)
	, m_elisHitRect(44, 54, 40, 74)//当たり判定の箇所。画像的な座標を示す
	, m_life(20)//とりあえずHP
	, m_maxLife(20)
	, m_invincibled(false)
	, m_invincibleTime(0)
	, m_elisMagicType(ElisMagicType::Normal)
	//,m_maxMP(100)
	, m_MP(0)
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
	const int kDamagedTime = 20;
	const int kInvincibleTime = 60;
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
	const String dataPath = L"data/Elis/";
	const String playerPath = dataPath + L"Player/";

	LOG(L"data:", dataPath, L"\nplayer", playerPath);

	TextureAsset::Register(L"texElis_readyCharging", playerPath + L"elis_readyCharging.png");
	TextureAsset::Register(L"texELStick_readyCharging", playerPath + L"stick_readyCharging.png");
	TextureAsset::Register(L"texElis_charge", playerPath + L"elis_charge.png");
	TextureAsset::Register(L"texELStick_charge", playerPath + L"stick_charge.png");
	TextureAsset::Register(L"texElis_swing", playerPath + L"elis_swing.png");
	TextureAsset::Register(L"texELStick_swing", playerPath + L"stick_swing.png");
	TextureAsset::Register(L"texElis_damaged", playerPath + L"elis_damaged.png");
	TextureAsset::Register(L"texElis_dead", playerPath + L"elis_dead.png");
	TextureAsset::Register(L"ELcharge1", playerPath + L"charge1.png");
	TextureAsset::Register(L"ELcharge2", playerPath + L"charge2.png");
	TextureAsset::Register(L"ELcharge3", playerPath + L"charge3.png");
	TextureAsset::Register(L"ELBcharge1", playerPath + L"Bcharge1.png");
	TextureAsset::Register(L"ELBcharge2", playerPath + L"Bcharge2.png");
	TextureAsset::Register(L"ELBcharge3", playerPath + L"Bcharge3.png");
	TextureAsset::Register(L"ELScharge1", playerPath + L"Scharge1.png");
	TextureAsset::Register(L"ELScharge2", playerPath + L"Scharge2.png");
	TextureAsset::Register(L"ELScharge3", playerPath + L"Scharge3.png");
	TextureAsset::Register(L"ELRcharge1", playerPath + L"Rcharge1.png");
	TextureAsset::Register(L"ELRcharge2", playerPath + L"Rcharge2.png");
	TextureAsset::Register(L"ELRcharge3", playerPath + L"Rcharge3.png");
	TextureAsset::Register(L"ELGcharge1", playerPath + L"Gcharge1.png");
	TextureAsset::Register(L"ELGcharge2", playerPath + L"Gcharge2.png");
	TextureAsset::Register(L"ELGcharge3", playerPath + L"Gcharge3.png");

	const String elisSEPath = dataPath + L"SE/Elis/";

	SoundAsset::Register(L"sndElisJump", elisSEPath + L"ElisJump.mp3");//跳躍音
	SoundAsset::Register(L"sndElisLand", elisSEPath + L"ElisLand.mp3");//着地音
	SoundAsset::Register(L"sndElisCharge", elisSEPath + L"ElisCharge.mp3");//溜め中
	SoundAsset::Register(L"sndElisDead", elisSEPath + L"ElisDead.mp3");//死亡中
	SoundAsset::Register(L"sndElisDamaged", elisSEPath + L"ElisDamaged.mp3");//くらった
	SoundAsset::Register(L"sndElisIce", elisSEPath + L"ElisIce.mp3");//氷魔法
	SoundAsset::Register(L"sndELGetItem", dataPath + L"SE/System/getItem.mp3");
	SoundAsset::Register(L"sndELMagicShoot", elisSEPath + L"ElisMagicShoot.mp3");
	SoundAsset::Register(L"sndElisStone", elisSEPath + L"ElisStone.mp3");
	SoundAsset::Register(L"sndElisFire", elisSEPath + L"ElisFire.mp3");
	SoundAsset::Register(L"sndElisWind", elisSEPath + L"ElisWind.mp3");

	TextureAsset::Register(L"texElisNormal", playerPath + L"elis_stay.png");
	TextureAsset::Register(L"texElisRun", playerPath + L"elis_move.png");
	TextureAsset::Register(L"texElisJump", playerPath + L"elis_jump.png");
	TextureAsset::Register(L"texElisFire", playerPath + L"elis_fire.png");

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

bool ELPlayer::intersectBody(const Point& foot, const Point& head, const ELMap& map, const ELFloorType& floorType)//通常床と身体の接触判定
{
	for (int i = 1; i<foot.y - head.y; ++i)
	{
		if (map.isFloor(foot.x, foot.y - i) && map.getFloorType(foot.x, foot.y - i) == floorType)
		{
			return true;
		}
	}
	return false;
}

//基本的に1画面に収まらない程長い関数は作らない
ELPlayState ELPlayer::update(ELMap& map, ELAttack& attack, const ELMyCursor& cursor,
	const ELEnemyInfo& enemy, ELObjectInfo& object, const Point& camerapos)
{
	if (m_life == 0)
	{
		if (m_elisState != ElisState::Dead)
			DeadProcess(map);

		m_elisState = ElisState::Dead;
	}

	m_oldState = m_elisState;

	if (m_elisState == ElisState::Dead)
	{
		if (!SoundAsset(L"sndElisDead").isPlaying){
			SoundAsset(L"sndElisDead").play();
		}

		m_frameCount += 1;

		if (m_frameCount > kDeadTime){
			return ELPlayState::GameOver;
		}

		return ELPlayState::Playing;
	}
	else if (m_elisState == ElisState::Damaged)
	{
		if (m_frameCount < kDamagedTime)
		{
			DamageProcess(map, object, camerapos);
			m_frameCount += 1;
			return ELPlayState::Playing;
		}
	}
	else if (m_elisState == ElisState::Fire)
	{
		FireProcess(map, object, camerapos, attack, cursor);
	}

	//チャージ処理////////////////////
	if (m_elisAttackState == ElisAttackState::Attack)
	{
		if (m_frameCount < kAttackingTime)
		{
			AttackProcess(map, object, camerapos);
			m_frameCount += 1;
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
	}
	////////////////////////////

	if (m_elisState != ElisState::Fire)
	{
		MoveXAxis(map, object, camerapos);
		MoveYAxis(map, object, camerapos);
	}

	if (m_invincibled)
	{
		m_invincibleTime += 1;

		if (m_invincibleTime == kInvincibleTime)
		{
			m_invincibled = false;
			m_invincibleTime = 0;
		}
	}

	if (m_oldState != m_elisState)
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
	m_elisRect.setCenter(map.getStartPos()+Point(0,-64));
	m_frameCount = 0;
	m_chargeFrameCount = 0;
	m_life = m_maxLife;
	m_speed = 0;
	m_elisState = ElisState::Staying;
	m_elisMagicType = ElisMagicType::Normal;
	m_invincibled = false;
}

void ELPlayer::AttackProcess(const ELMap& map, ELObjectInfo& object, const Point &camerapos)
{
}

void ELPlayer::FireProcess(const ELMap& map, ELObjectInfo& object, const Point &camerapos, ELAttack &attack, const ELMyCursor &cursor)
{
	Vec2 dir = Vec2(m_endPos - m_startPos).normalize();

	Point foot = footCollisionPos();
	Point head = headCollisionPos();


	for (int i = 1; i <= kFireSpeed; ++i)
	{
		const int fx = int(foot.x + dir.x * i);
		const int fy = int(foot.y + dir.y * i);
		const int hx = int(head.x + dir.x * i);
		const int hy = int(head.y + dir.y * i);

		if (intersectBody(Point(fx, fy), Point(hx, hy), map, ELFloorType::NormalFloor)
			|| object.isTouchable(Point(hx, hy), Point(fx, fy), camerapos))
		{
			m_elisRect.moveBy(Point(int(dir.x*(i - 1)), int(dir.y*(i - 1))));
			break;
		}
		else if (i == kFireSpeed)
		{
			m_elisRect.moveBy(Point(int(dir.x*kFireSpeed), int(dir.y*kFireSpeed)));
			break;
		}
	}

	if (m_frameCount % 4 == 0)
	{
		const Point startpos(m_elisRect.center.x, footCollisionPos().y - m_elisHitRect.size.y / 2);
		const Point endpos = startpos + Point(int(dir.x * 10), int(dir.y * 10));
		attack.makeShoot(ELShooter::Player, ELShootType::ElisFire, startpos, endpos);
	}

	if (m_frameCount >60)
	{
		m_elisState = ElisState::Jumping;
		m_elisAttackState = ElisAttackState::Stay;
	}
}

void ELPlayer::DamageProcess(const ELMap& map, ELObjectInfo& object, const Point &camerapos)
{
	const Point foot = footCollisionPos();
	const Point head = headCollisionPos();
	const int dir = (m_elisFace == ElisFace::Left ? 1 : -1);

	for (int i = 0; i<2; ++i)
	{
		for (int j = 0; j <= m_riseOffset; ++j)
		{
			const Point fPos(foot.x + dir, foot.y - j - 1);
			const Point hPos(head.x + dir, head.y - j - 1);
			if (!(map.isFloor(fPos.x, fPos.y)) && !intersectBody(fPos, hPos, map, ELFloorType::NormalFloor))
			{
				if (!object.isTouchable(hPos, fPos, camerapos))
				{
					m_elisRect.moveBy(Point(dir, -j));
					break;
				}
			}
		}
	}

	FallProcess(map, object, camerapos);
}

void ELPlayer::ChargeProcess(ELAttack& attack, const ELMyCursor& cursor, const Point& camerapos, ELObjectInfo &object)
{
	m_elisFace = (m_elisRect.center.x > cursor.getCursorPos().x + camerapos.x ? ElisFace::Left : ElisFace::Right);

	if (!m_mouseLeft.pressed)
	{
		if (SoundAsset(L"sndElisCharge").isPlaying){
			SoundAsset(L"sndElisCharge").stop();
		}

		const Point startpos(m_elisRect.center.x, footCollisionPos().y - m_elisHitRect.size.y / 2);
		const Point endpos = cursor.getCursorPos() + camerapos;

		switch (m_elisMagicType)
		{
		case ElisMagicType::Normal:
		{
			SoundAsset(L"sndELMagicShoot").play();

			ELShootType type;
			if (m_chargeFrameCount < kChargeTime)
			{
				type = ELShootType::ElisNormalShot1;
			}
			else if (m_chargeFrameCount < kChargeTime * 2)
			{
				type = ELShootType::ElisNormalShot2;
			}
			else
			{
				type = ELShootType::ElisNormalShot3;
			}

			attack.makeShoot(ELShooter::Player, type, startpos, endpos);

			break;
		}
		case ElisMagicType::Ice:
		{
			if (SoundAsset(L"sndElisIce").isPlaying){
				SoundAsset(L"sndElisIce").stop();
			}
			SoundAsset(L"sndElisIce").play();

			attack.makeShoot(ELShooter::Player, ELShootType::ElisIce, startpos, endpos);

			break;
		}
		case ElisMagicType::Wind:
		{
			if (SoundAsset(L"sndElisWind").isPlaying){
				SoundAsset(L"sndElisWind").stop();
			}
			SoundAsset(L"sndElisWind").play();

			attack.makeShoot(ELShooter::Player, ELShootType::ElisWind, startpos, endpos);

			break;
		}
		case ElisMagicType::Stone:
		{
			if (SoundAsset(L"sndElisStone").isPlaying){
				SoundAsset(L"sndElisStone").stop();
			}
			SoundAsset(L"sndElisStone").play();

			object.addObject(ELObjectType::magicBlock, cursor.getCursorPos() + Point(0, 32) + camerapos);
			break;
		}
		case ElisMagicType::Fire:
		{
			if (SoundAsset(L"sndElisFire").isPlaying){
				SoundAsset(L"sndElisFire").stop();
			}

			SoundAsset(L"sndElisFire").play();

			m_startPos = Point(m_elisRect.center.x, footCollisionPos().y - m_elisHitRect.size.y / 2);
			m_endPos = cursor.getCursorPos() + camerapos;
			m_elisState = ElisState::Fire;
			m_frameCount = 0;
			m_chargeFrameCount = 0;
			m_elisAttackState = ElisAttackState::Fire;

			return;
			break;
		}
		default:
			return;
		}
		m_elisAttackState = ElisAttackState::Attack;
		m_frameCount = 0;
		m_chargeFrameCount = 0;
		return;
	}

	if (m_mouseLeft.pressed)
	{
		if (!SoundAsset(L"sndElisCharge").isPlaying){
			SoundAsset(L"sndElisCharge").play();//チャージ音
		}

		return;
	}
}

void ELPlayer::MoveXAxis(const ELMap& map, ELObjectInfo& object, const Point &camerapos)
{
	bool inputed = false;

	if (!(m_keyRight.pressed && m_keyLeft.pressed))//同時押し回避
	{
		if (m_keyRight.pressed)///////////////右に進むキー
		{
			m_elisFace = Right;
			inputed = true;
			m_speed = Min(m_speed + 1, kMaxSpeed);
		}
		if (m_keyLeft.pressed)////////////////左に進むキー
		{
			m_elisFace = Left;
			inputed = true;
			m_speed = Max(m_speed - 1, -kMaxSpeed);
		}
	}

	if (!inputed)//なんも押さないと…
	{
		m_speed = Clamp(m_speed - 1, kMinSpeed, m_speed + 1);
	}

	const int dir = (0 <= m_speed ? 1 : -1);

	for (int i = 0; i<std::abs(m_speed); ++i)
	{
		for (int j = 0; j <= m_riseOffset; ++j)
		{
			const Point foot = footCollisionPos();
			const Point head = headCollisionPos();

			const Point footPos(foot.x + dir, foot.y - j - 1);
			const Point headPos(head.x + dir, head.y - j - 1);

			if (!map.isFloor(footPos.x, footPos.y) && !intersectBody(footPos, headPos, map, ELFloorType::NormalFloor))
			{
				if (!object.isTouchable(headPos + Point(0, 1), footPos + Point(0, 1), camerapos))
				{
					m_elisRect.moveBy(Point(dir, -j));
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

	FallProcess(map, object, camerapos);

	IdentifyState(map, object, camerapos);
}

void ELPlayer::JumpProcess(const ELMap& map, ELObjectInfo& object, const Point &camerapos)
{
	const Point head = headCollisionPos();
	const Point foot = footCollisionPos();

	if (m_keyUp.clicked && m_elisState != ElisState::Jumping)//ジャンプした状態にする
	{
		if (SoundAsset(L"sndElisJump").isPlaying){
			SoundAsset(L"sndElisJump").stop();
		}
		SoundAsset(L"sndElisJump").play();

		m_jumpH = kMaxJumpH;
		m_jumped = true;
	}

	if (m_keyUp.pressed && m_jumped)//ジャンプする
	{
		if (m_jumpH>0 && m_jumped)
		{
			for (int i = 1; i <= m_jumpH; ++i)
			{
				const Point headPos(head.x, head.y - 1);
				const Point footPos(foot.x, foot.y - 1);
				if (map.isFloor(head.x, head.y - i) || object.isTouchable(headPos, footPos, camerapos))
				{
					if (map.getFloorType(headPos.x, headPos.y) == ELFloorType::NormalFloor
						|| object.isTouchable(headPos, footPos, camerapos))//床なら
					{
						m_jumpH = i - 1;
						break;
					}
				}
			}
			m_elisRect.y -= m_jumpH;
			m_jumpH -= kDrop;
		}
	}
	else if (!m_keyUp.pressed && m_jumped)//
	{
		m_jumpH = 0;
	}
}

void ELPlayer::PassFloorProcess(const ELMap& map)
{
	const Point head = headCollisionPos();
	const Point foot = footCollisionPos();

	if (m_keyDown.clicked
		&& map.isFloor(foot.x, foot.y)
		&& map.getFloorType(foot.x, foot.y) == ELFloorType::PassFloor)//
	{
		m_elisRect.y += 1;
	}
}

void ELPlayer::FallProcess(const ELMap& map, ELObjectInfo& object, const Point &camerapos)
{
	//落下に関する部分
	if (m_jumped && m_jumpH - m_gravity >0)
	{
		m_elisRect.y += m_gravity;
		return;
	}

	const Point head = headCollisionPos();
	const Point foot = footCollisionPos();
	for (int i = 0; i <= m_gravity; ++i)
	{
		if (//object.isTouchable(Point(head.x, head.y + i), Point(foot.x, foot.y + i), camerapos)
			object.isTouchable(Point(foot.x, foot.y + i), Point(foot.x, foot.y + i), camerapos)
			|| map.isFloor(foot.x, foot.y + i))
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

	if (intersectBody(Point(head.x, foot.y + 1), Point(head.x, head.y), map, ELFloorType::DeathFloor))
	{
		DeadProcess(map);
		return;
	}

	if (m_elisState != ElisState::Fire
		&& m_elisAttackState != ElisAttackState::Attack
		&& m_mouseLeft.clicked)
	{
		m_elisAttackState = ElisAttackState::Charge;
	}

	//接地しているかの判定
	if (!(object.isTouchable(foot, foot, camerapos)) 
		&& !(map.isFloor(foot.x, foot.y))
		|| m_jumpH - m_gravity >0)//
	{
		m_elisState = ElisState::Jumping;
		m_gravity = Min(m_gravity + kDrop, kMaxGravity);
		return;
	}//ジャンプ(下降)中であることの判定
	else
	{
		m_jumped = false;
		m_gravity = kGravity0;
		m_jumpH = 0;
	}



	if (m_speed != 0)
	{
		if (m_elisState == ElisState::Jumping){
			SoundAsset(L"sndElisLand").play();//////着地音
		}
		m_elisState = ElisState::Running;
		return;
	}

	if (m_elisState == ElisState::Jumping){
		SoundAsset(L"sndElisLand").play();//////着地音
	}
	m_elisState = ElisState::Staying;
	return;
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
	const double x = 128 * m_elisFace;
	const double y = 128;
	const double w = m_elisRect.w;
	const double  h = m_elisRect.h;
	const int a = 255 - 100 * m_invincibled;
	const Rect stickRect(m_elisRect.pos.x - 64, m_elisRect.pos.y - 128, 256, 256);
	const Point stickPos = stickRect.pos - cameraPos;
	const Rect drawStickRect(m_elisFace*stickRect.size.x, 0, stickRect.size);

	//攻撃モーション
	if (m_elisState != ElisState::Fire && m_elisAttackState == ElisAttackState::Attack)
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
			/*
		case ElisState::Falling:
			TextureAsset(L"texElisJump")(x, y, w, h).draw(pos, Alpha(a));
			break;
			*/
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

	const auto drawTex = [&](const String& assetName){
		TextureAsset(assetName)
			(0,
			256 * ((m_chargeFrameCount / kAnimeTiming) % (height(assetName) / drawStickRect.size.y) - 1),
			256,
			256
			).draw(stickPos, Alpha(a));
	};

	const auto drawMagic = [&](ElisMagicType type, String assetName){
		if (m_elisMagicType == type)
		{
			if (m_chargeFrameCount > kChargeTime * 2)
			{
				drawTex(assetName + L"3");

				return;
			}

			drawTex(assetName + L"1");

			if (m_chargeFrameCount > kChargeTime)
			{
				drawTex(assetName + L"2");
			}
		}
	};

	if (m_elisAttackState == ElisAttackState::Charge)
	{
		drawMagic(ElisMagicType::Normal, L"ELcharge");
		drawMagic(ElisMagicType::Stone, L"ELScharge");
		drawMagic(ElisMagicType::Ice, L"ELBcharge");
		drawMagic(ElisMagicType::Fire, L"ELRcharge");
		drawMagic(ElisMagicType::Wind, L"ELGcharge");
	}
}

void ELPlayer::objectUpdate(const Point &camerapos, ELObjectInfo &object)
{
	const Point head = headCollisionPos();
	const Point foot = footCollisionPos();
	m_elisRect.pos += object.objectEffect(head, foot, camerapos);
}

Point ELPlayer::getPlayerFootCollisionPos() const
{
	return footCollisionPos();
}

void ELPlayer::checkHit(const ELEnemyInfo& enemyInfo, const ELAttack& attack)
{
	if (!m_invincibled && m_elisState != ElisState::Dead)
	{
		int oldlife = m_life;

		Rect body(m_elisRect.pos + m_elisHitRect.pos, m_elisHitRect.size);

		m_life -= attack.getDamage(body, ELShooter::Player);//damageくらう

		if (m_life == oldlife)
		{
			m_life -= enemyInfo.getDamage(body);//damageくらう
		}

		if (m_life != oldlife)
		{
			if (!SoundAsset(L"sndElisDamaged").isPlaying){
				SoundAsset(L"sndElisDamaged").play();
			}

			m_elisState = ElisState::Damaged;
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
	return Rect(m_elisRect.pos + m_elisHitRect.pos, m_elisHitRect.size);
}

ElisState ELPlayer::getState() const
{
	return m_elisState;
}

void ELPlayer::setPos(const Point& pos)
{
	m_elisRect.setCenter(pos.x, pos.y - 64);
}

int ELPlayer::getLife() const
{
	return m_life;
}

void ELPlayer::itemEffect(const ELItemType &itemType)
{
	if (SoundAsset(L"sndELGetItem").isPlaying){
		SoundAsset(L"sndELGetItem").stop();
	}

	const auto playSound = [](){
		SoundAsset(L"sndELGetItem").play();
	};

	switch (itemType)
	{
	case ELItemType::LifeUpS:
		playSound();
		m_life = Min(m_life + 4, m_maxLife);
		break;
	case ELItemType::LifeUpM:
		playSound();
		m_life = Min(m_life + 8, m_maxLife);
		break;
	case ELItemType::LifeUpL:
		playSound();
		m_life = m_maxLife;
		break;
	case ELItemType::MagicUpS:
		playSound();
		m_MP += 1;
		break;
	case ELItemType::MagicUpM:
		playSound();
		m_MP += 5;
		break;
	case ELItemType::MagicUpL:
		playSound();
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

bool ELPlayer::walkto(int x, int speed, ELMap &map, ELObjectInfo &object, const Point &camerapos)
{
	if (m_elisState != ElisState::Running)
	{
		m_frameCount = 0;
		m_elisState = ElisState::Running;
		m_jumped = false;
		m_jumpH = 0;
	}

	if (m_elisRect.center.x > x)
	{
		m_elisFace = ElisFace::Left;

		m_elisRect.setCenter(Point(Max(m_elisRect.center.x - speed, x), m_elisRect.center.y));
	}
	else
	{
		m_elisFace = ElisFace::Right;

		m_elisRect.setCenter(Point(Min(m_elisRect.center.x + speed, x), m_elisRect.center.y));
	}


	FallProcess(map, object, camerapos);
	IdentifyState(map, object, camerapos);

	if (m_elisRect.center.x == x)
	{
		m_elisState = ElisState::Staying;
		m_speed = 0;
		m_frameCount = 0;
		return false;
	}

	m_elisState = ElisState::Running;

	return true;
}

bool ELPlayer::identyfybase(ELMap &map, ELObjectInfo &object, const Point &camerapos)
{

	if (m_elisState == ElisState::Fire)
	{
		m_elisState = ElisState::Jumping;
	}

	else if (m_elisState == ElisState::Jumping)
	{
		m_jumpH = 0;
		m_speed = 0;
	}
	else
	{
		m_invincibled = false;
		m_speed = 0;
		m_elisState = ElisState::Staying;
		m_elisAttackState = ElisAttackState::Stay;
	}

	FallProcess(map, object, camerapos);
	IdentifyState(map, object, camerapos);

	return m_elisState != ElisState::Staying;
}

void ELPlayer::addFramecount()
{
	m_frameCount += 1;
}

//////////////////////////////コンフィグ用
void ELPlayer::changeKey(bool isRightHand)
{
	if (isRightHand)
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