# include "ELEnemyInfo.h"

ELEnemyInfo::ELEnemyInfo():m_cameraEffect(ELCameraEffect::null)
{
	SoundAsset::Register(L"sndELEnemyDamaged",L"data/Elis/SE/Enemy/enemyDamaged.mp3");
	SoundAsset::Register(L"sndELEnemyDefeat",L"data/Elis/SE/Enemy/enemyDefeat.mp3");

}

namespace
{
	const int kXoffset = 200;
	const int kYoffset = 200;

	const Point kSsize = Point(64,64);
	const Point kMsize = Point(128,128);
	const Point kLsize = Point(256,256);
};

ELEnemyName ELEnemyInfo::loadName(const String& Name) const
{
	//LOGFMT_ERR(Name);

	if(Name==L"slime")
	{
		return ELEnemyName::slime;
	}
	else if(Name==L"slimer")
	{
		return ELEnemyName::slimeR;
	}
	else if(Name==L"slimeb")
	{
		return ELEnemyName::slimeB;
	}
	else if(Name==L"slimew")
	{
		return ELEnemyName::slimeW;
	}
	else if(Name==L"slimeg")
	{
		return ELEnemyName::slimeG;
	}
	else if(Name==L"slimed")
	{
		return ELEnemyName::slimeD;
	}
	else if(Name==L"redcap")
	{
		return ELEnemyName::redcap;
	}
	else if(Name==L"bluecap")
	{
		return ELEnemyName::bluecap;
	}
	else if(Name==L"sharkman")
	{
		return ELEnemyName::sharkman;
	}
	else if(Name==L"darkbrave")
	{
		return ELEnemyName::darkbrave;
	}
	else if(Name==L"airsnake")
	{
		return ELEnemyName::airsnake;
	}
	else if(Name==L"boneman")
	{
		return ELEnemyName::boneman;
	}
	else if(Name==L"raven")
	{
		return ELEnemyName::raven;
	}
	else if(Name==L"ravenacross")
	{
		return ELEnemyName::ravenacross;
	}
	else if (Name == L"blockmaker")
	{
		return ELEnemyName::blockmaker;
	}
	else if (Name == L"tentacle")
	{
		return ELEnemyName::tantacle;
	}
	////////////boss
	else if(Name==L"bigslime")
	{
		return ELEnemyName::bigslime;
	}
	else if(Name==L"seaserpent")
	{
		return ELEnemyName::seaserpent;
	}
	////////////////
	else {
		return ELEnemyName::null;
	}
}

ELEnemySize ELEnemyInfo::loadSize(const String& enemysize)const
{
	//LOGFMT_ERR(enemysize);

	if (enemysize == L"s"){
		return ELEnemySize::S;
	}
	else if (enemysize == L"m"){
		return ELEnemySize::M;
	}
	else if (enemysize == L"l"){
		return ELEnemySize::L;
	}

	//LOGFMT_ERR("サイズ！");
	return ELEnemySize::null;
}

bool ELEnemyInfo::loadEnemyData()
{
	const CSVReader csv(L"data/Elis/Enemy/EnemyData.csv");

	if(!csv)
	{
		return false;
	}

	for(int i=0;;++i)
	{
		String Name = csv.get<String>(i, 0).lower();

		if(Name.isEmpty)//名前が空なら名簿読み込み終了
		{
			break;
		}

		//LOGFMT_ERR(loadName(Name));

		m_enemyData.emplace_back(loadName(Name),//名前
			loadSize(csv.get<String>(i,1).lower()),//サイズ
			Rect(//あたり判定
			csv.get<int>(i,2)//x
			,csv.get<int>(i,3)//y
			,csv.get<int>(i,4)//w
			,csv.get<int>(i,5)//h
			),
			csv.get<int>(i,6)//体力
			);
	}

	return true;
}


bool ELEnemyInfo::setEnemysPlace(const String& mapName, const String& stageName)
{
	m_isBoss = false;
	m_bossType = ElisMagicType::null;
	m_bossMaxlife = 0;
	m_bossLife = 0;

	const CSVReader csv(L"data/Elis/Enemy/setEnemy/" + mapName + L"/"+stageName+L"_setEnemys.csv");

	if(!csv)
	{
		return false;
	}

	for(int i=0;;++i)
	{
		String SName = csv.get<String>(i, 0).lower();

		if(SName.isEmpty)
		{
			break;
		}

		ELEnemyName EName = loadName(SName);

		//LOG_ERROR(SName);

		m_enemyPlaces.emplace_back(
			EName,
			Point(csv.get<int>(i,1),csv.get<int>(i,2))
			);
	}

	//LOG_ERROR(m_enemyPlaces.size());

	return true;
}

ELEnemyData ELEnemyInfo::getEnemyData(const ELEnemyName& Name)const
{
	for(size_t i=0;i<m_enemyData.size(); ++i)
	{
		if(m_enemyData[i].Name == Name)
		{	
			return m_enemyData[i];
		}
	}
	
	return ELEnemyData(ELEnemyName::null,ELEnemySize::null,Rect(0,0,0,0),0);
}

void ELEnemyInfo::setEnemy(const ELEnemyName& Name, const Point& pos)
{
	ELEnemyData Data = getEnemyData(Name);

	switch(Name)
	{
	case ELEnemyName::slime:
		{
			m_enemies.emplace_back
				(std::make_shared<ELSlime>(Data.Name,Data.Size,Data.hitRect,Data.HP,pos));
			break;
		}
	case ELEnemyName::slimeR:
		{
			m_enemies.emplace_back
				(std::make_shared<ELSlimeR>(Data.Name,Data.Size,Data.hitRect,Data.HP,pos));
			break;
		}
	case ELEnemyName::slimeB:
		{
			m_enemies.emplace_back
				(std::make_shared<ELSlimeB>(Data.Name,Data.Size,Data.hitRect,Data.HP,pos));
			break;
		}
	case ELEnemyName::slimeG:
		{
			m_enemies.emplace_back
				(std::make_shared<ELSlimeG>(Data.Name,Data.Size,Data.hitRect,Data.HP,pos));
			break;
		}
	case ELEnemyName::slimeW:
		{
			m_enemies.emplace_back
				(std::make_shared<ELSlimeW>(Data.Name,Data.Size,Data.hitRect,Data.HP,pos));
			break;
		}
	case ELEnemyName::slimeD:
		{
			m_enemies.emplace_back
				(std::make_shared<ELSlimeD>(Data.Name,Data.Size,Data.hitRect,Data.HP,pos));
			break;
		}
	case ELEnemyName::sharkman:
		{
			m_enemies.emplace_back
				(std::make_shared<ELSharkman>(Data.Name,Data.Size,Data.hitRect,Data.HP,pos));
			break;
		}
	case ELEnemyName::redcap:
		{
			m_enemies.emplace_back
				(std::make_shared<ELRedcap>(Data.Name,Data.Size,Data.hitRect,Data.HP,pos));
			break;
		}
	case ELEnemyName::bluecap:
		{
			m_enemies.emplace_back
				(std::make_shared<ELBluecap>(Data.Name,Data.Size,Data.hitRect,Data.HP,pos));
			break;
		}
	case ELEnemyName::airsnake:
		{
			m_enemies.emplace_back
				(std::make_shared<ELAirsnake>(Data.Name,Data.Size,Data.hitRect,Data.HP,pos));
			break;
		}
	case ELEnemyName::boneman:
		{
			m_enemies.emplace_back
				(std::make_shared<ELBoneman>(Data.Name,Data.Size,Data.hitRect,Data.HP,pos));
			break;
		}
	case ELEnemyName::raven:
		{
			m_enemies.emplace_back
				(std::make_shared<ELRaven>(Data.Name,Data.Size,Data.hitRect,Data.HP,pos));
			break;
		}
	case ELEnemyName::ravenacross:
		{
			m_enemies.emplace_back
				(std::make_shared<ELRavenAcross>(Data.Name,Data.Size,Data.hitRect,Data.HP,pos));
			break;
		}
	case ELEnemyName::blockmaker:
		{
			m_enemies.emplace_back
				(std::make_shared<ELBlockMaker>(Data.Name, Data.Size, Data.hitRect, Data.HP, pos));
			break;
		}
	case ELEnemyName::tantacle:
		{
			m_enemies.emplace_back
				(std::make_shared<ELTentacle>(Data.Name, Data.Size, Data.hitRect, Data.HP, pos));
			break;
		}
		////////boss
	case ELEnemyName::darkbrave:
		{
			m_isBoss = true;
			m_bossType = ElisMagicType::Normal;
			m_bossMaxlife = Data.HP;

			m_enemies.emplace_back
				(std::make_shared<ELDarkbrave>(Data.Name, Data.Size, Data.hitRect, Data.HP, pos));
			break;
		}
	case ELEnemyName::bigslime:
		{
			m_isBoss = true;
			m_bossType = ElisMagicType::null;
			m_bossMaxlife = Data.HP;
			m_enemies.emplace_back
				(std::make_shared<ELBigslime>(Data.Name,Data.Size,Data.hitRect,Data.HP,pos));
			break;
		}
	case ELEnemyName::seaserpent:
		{
			m_isBoss = true;
			m_bossType = ElisMagicType::Ice;
			m_bossMaxlife = Data.HP;
			m_enemies.emplace_back
				(std::make_shared<ELSeaserpent>(Data.Name,Data.Size,Data.hitRect,Data.HP,pos));
			//LOG_ERROR(L"seaser!!!");
			break;
		}
		//////////

	}

	//LOG_ERROR(L"seaser!!!");
}

void ELEnemyInfo::update(const ELMap& map, const Point& playerpos, ELObjectInfo& object, const Point& camerapos, ELItem &item, ELAttack &attack)
{	
	//敵を消す
	for(size_t i = 0;i<m_enemies.size();++i)
	{
		if(m_enemies[i]->isErased() || (!incamera(m_enemies[i]->getPos(),camerapos) 
			&& !isBoss(m_enemyPlaces[i].Name)))
		{
			eraseEnemy(i);
		}
	}

	for(size_t i=0; i<m_enemyPlaces.size(); ++i)
	{
		/*
		if(m_isBoss)//ボスならエフェクト発動
		{
			for(const auto &enemy : m_enemies)
			{
				if(isBoss(enemy->getName()))
				{
					m_bossLife = enemy->getLife();
					//if(enemy->getName() == ELEnemyName::bigslime)
						m_cameraEffect = enemy->getCameraEffect();
				}
			}
		}
		*/
		if((incamera(m_enemyPlaces[i].Pos,camerapos) 
			|| isBoss(m_enemyPlaces[i].Name))
			&& !m_enemyPlaces[i].created)
		{
			setEnemy(m_enemyPlaces[i].Name,m_enemyPlaces[i].Pos);
			m_enemyPlaces[i].created = true;
		}

		bool creatflag = true;

		if(m_enemyPlaces[i].created 
			&& !incamera(m_enemyPlaces[i].Pos,camerapos)
			&& !isBoss(m_enemyPlaces[i].Name))
		{
			for(const auto& enemy : m_enemies)
			{
				if(m_enemyPlaces[i].Pos == enemy->getStartPos())
					creatflag = false;
			}

			if(creatflag)
				m_enemyPlaces[i].created=false;
		}
	}

	for(auto& enemy : m_enemies)
	{
		enemy->update(map, playerpos, object, item, attack, camerapos);

		if (m_isBoss)//ボスならエフェクト発動
		{
			//for (const auto &enemy : m_enemies)
			{
				if (isBoss(enemy->getName()))
				{
					m_bossLife = enemy->getLife();
					//if(enemy->getName() == ELEnemyName::bigslime)
					m_cameraEffect = enemy->getCameraEffect();
				}
			}
		}
	}
}

void ELEnemyInfo::eraseEnemy(int num)
{
	m_enemies.erase(m_enemies.begin()+num);
}

bool ELEnemyInfo::incamera(const Point& pos, const Point& camerapos)const
{
	return pos.x > camerapos.x - kXoffset
			&& pos.x < camerapos.x + 1280 +kXoffset
			&& pos.y > camerapos.y - kYoffset
			&& pos.y <camerapos.y + 720 + kYoffset;
}

void ELEnemyInfo::draw(const Point& camerapos)const
{
	for(const auto& enemy : m_enemies)
	{
		enemy->draw
			(
			camerapos, 
			modifyDrawPos(enemy->getEnemySize())
			);
	}
}

Point ELEnemyInfo::modifyDrawPos(const ELEnemySize& enemySize)const
{
	switch (enemySize)
	{
	case ELEnemySize::S:
		return Point(kSsize.x/2,kSsize.y);
		break;
	case ELEnemySize::M:
		return Point(kMsize.x/2,kMsize.y);
		break;
	case ELEnemySize::L:
		return Point(kLsize.x/2,kLsize.y);
		break;
	default:
		return Point(0,0);
		break;
	}	
}

int ELEnemyInfo::getDamage(const Rect& body) const//プレイヤーが敵にぶつかる
{
	for(const auto& enemy : m_enemies)
	{
		if(enemy->isHit(body) && !enemy->isDamaged())
		{
			return enemy->getDamage();
		}
	}

	return 0;
}

void ELEnemyInfo::checkHit(const ELAttack& attack)//敵が攻撃に当たる
{
	for(const auto& enemy : m_enemies)
	{
		if (!enemy->isDamaged())
		{
			if (enemy->getName() == ELEnemyName::seaserpent)
			{
				enemy->giveDamage(attack.getDamage(enemy->getHitRect(), ELShooter::Enemy));//damageくらう

				for (size_t i = 0; i < enemy->getOptionEnemysSize();++i)
				{
					enemy->giveOptionEnemyDamage(attack.getDamage(enemy->getOptionEnemyHitRect(i), ELShooter::Enemy),i);//damageくらう
				}
			}
			else
				enemy->giveDamage(attack.getDamage(enemy->getHitRect(), ELShooter::Enemy));//damageくらう
		}
	}
}

void ELEnemyInfo::clearEnemies()
{
	m_enemies.clear();
}

void ELEnemyInfo::clearEnemyPlaces()
{
	m_enemyPlaces.clear();
}

//////////イベント用

bool ELEnemyInfo::ready()
{
	for(auto &enemy : m_enemies)
	{
		return enemy->ready();
	}

	return false;
}

void ELEnemyInfo::addFramecount()
{
	for(auto &enemy : m_enemies)
	{
		return enemy->addFramecount();
	}
}

Point ELEnemyInfo::getBurstPos() const
{
	for(auto &enemy : m_enemies)
	{
		return enemy->getBurstPos();
	}

	return Point(0,0);
}

bool ELEnemyInfo::enemyIsDead() const
{
	bool flag = false;
	for(auto &enemy : m_enemies)
	{
		if (enemy->getEnemyState() != ELEnemyState::Dead)
			return false;

	}

	return true;
}