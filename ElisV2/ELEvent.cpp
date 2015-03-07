# include "ELEvent.h"

ELEvent::ELEvent()
	:m_actedAutoEvent(false)
	,m_actedTalkEvent(false)
	,m_acting(false)
	,m_eventIndex(-1)
	,m_column(-1)
{

}

bool ELEvent::loadEvent(const String& mapName,const String& stageName)
{
	m_mapName = mapName;

	const CSVReader csv(L"data/Elis/Event/" +mapName +L"/"+ stageName +L".csv");
	
	if(!csv) return false;

	for(int i=0;;++i)
	{
		String SName = csv.get<String>(i,0).lower();

		if(SName.isEmpty)
		{
			break;
		}

		if(SName==L"autoevent")//�����𖞂����Ǝ����Ŕ�������
		{
			Rect r(csv.get<int>(i,2),csv.get<int>(i,3),csv.get<int>(i,4),csv.get<int>(i,5));
			const String term = csv.get<String>(i,6).lower();

			setAutoEvent(csv.get<String>(i,1).lower(),r,csv.get<String>(i,6).lower());//�t�@�C�����̎w��Ɣ�������
		}

		if(SName == L"talkevent")//�Ώۂ̑O�Œ��ׂ�s�������Ɣ�������
		{
			//door�Ɗ�{�I�Ɉꏏ���ƍl������B�t�@�C�����A�ꏊ�A�g�p����摜�ō\�������
		}
	}
			
	return true;
}

void ELEvent::setTalkEvent(const String& filename, const Point &setpos)
{
	m_setTalkEvents.emplace_back(filename,setpos);
}

void ELEvent::setAutoEvent(const String& filename, const Rect &actrect, const String &term)
{
	m_setAutoEvents.emplace_back(filename,actrect,term);
}

ELPlayState ELEvent::update(ELPlayer &player, ELEnemyInfo &enemy, ELMyCamera &mycamera, ELMap &map,ELObjectInfo &object, ELTalk &talk)
{

	if(m_actedAutoEvent)
	{
		return actEvent(m_setAutoEvents[m_eventIndex].filename, player, object, mycamera, map,enemy,talk);
	}
	else if(m_actedTalkEvent)
	{
		return actEvent(m_setTalkEvents[m_eventIndex].filename, player, object, mycamera, map, enemy,talk);
	}

	if(!m_setAutoEvents.empty())
	{
		for(size_t i= 0; i< m_setAutoEvents.size(); ++i)
		{
			if(player.getRect().intersects(m_setAutoEvents[i].actRect))
			{
				int flag= false;

				if(m_setAutoEvents[i].term.isEmpty)	flag = true;
				else if(m_setAutoEvents[i].term == L"enemyempty")
				{
					flag = enemy.enemyEmpty();
				}
				else if(m_setAutoEvents[i].term == L"enemydead")
				{
					flag = enemy.enemyIsDead();
				}

				if(flag)
				{
					m_actedAutoEvent = true;
					m_eventIndex = i;
					m_setAutoEvents[i].readed = true;
					m_csv = CSVReader(L"data/Elis/Event/" + m_mapName +L"/"+ m_setAutoEvents[i].filename +L".csv");

					return ELPlayState::Event;
				}
				break;
			}
		}
	}

	return ELPlayState::Playing;
}

ELPlayState ELEvent::actEvent(
	const String &filename, ELPlayer &player, 
	ELObjectInfo &object, ELMyCamera &mycamera, 
	ELMap &map, ELEnemyInfo &enemy, ELTalk &talk)
{
	if(!m_acting)
		++m_column;

	const String target = m_csv.get<String>(m_column,0).lower();

	if(target.isEmpty)
	{
		if(m_actedAutoEvent)	m_setAutoEvents.erase(m_setAutoEvents.begin()+ m_eventIndex);

		m_actedAutoEvent = false;
		m_actedTalkEvent = false;
		m_column = -1;
		return ELPlayState::Playing;
	}

	if(target == L"player")//player�Ɋ�����R�}���h
	{
		const String Scommand = m_csv.get<String>(m_column,1).lower();

		if(Scommand == L"walkto")//��������
		{	
			m_acting = player.walkto(m_csv.get<int>(m_column,2),
				m_csv.get<int>(m_column,3),map,object,mycamera.getCameraPos());
		}
		else if(Scommand == L"walktowithcamera")//�J�������ꏏ��
		{
			m_acting = player.walkto(m_csv.get<int>(m_column,2),
				m_csv.get<int>(m_column,3),map,object,mycamera.getCameraPos());
			
			mycamera.update(player.getPlayerPos(),map.getMapSize());
		}
		else if(Scommand == L"identyfybase")//�p����Ԃ���{�ɖ߂�
		{
			m_acting = player.identyfybase(map,object,mycamera.getCameraPos());
			
			mycamera.update(player.getPlayerPos(),map.getMapSize());
		}

	}
	else if(target == L"camera")
	{
		const String Scommand = m_csv.get<String>(m_column,1).lower();

		
	}
	else if(target == L"enemy")
	{
		const String Scommand = m_csv.get<String>(m_column,1).lower();

		if(Scommand == L"ready")
		{
			m_acting = enemy.ready();
		}
	}
	else if(target == L"system")//�G�t�F�N�g�Ȃǂ�S������
	{
		const String Scommand = m_csv.get<String>(m_column,1).lower();

		if(Scommand == L"stageclear")
		{
			m_actedAutoEvent = false;
			m_actedTalkEvent = false;
			m_column = -1;
			return ELPlayState::Clear;
		}
		else if(Scommand == L"stopbgm")
		{
			map.stopStageBGM();
			m_acting = false;
		}
		else if(Scommand == L"playbgm")
		{
			const String bgmName = m_csv.get<String>(m_column,2).lower();
			map.playStageMusic(bgmName);
			m_acting = false;
		}
		else if(Scommand == L"showlogo")
		{
			if(!map.getHaveLogo())
			{
				const String logoname = m_csv.get<String>(m_column,2).lower();

				if(!logoname.isEmpty && !map.haveLogoname(logoname))
				{
					map.setLogoname(logoname);
				}
			}

			m_acting = map.getHaveLogo();
			
			mycamera.update(player.getPlayerPos(),map.getMapSize());

		}
		else if(Scommand == L"burst")
		{

			m_acting = mycamera.setBurst(enemy.getBurstPos());

			if(!m_acting)
			{
				enemy.clearEnemies();
				
			}
		}
	}
	else if (target == L"talk")//��b����
	{
		const String Scommand = m_csv.get<String>(m_column, 1).lower();

		if (Scommand == L"normal")//���ʂɉ�b
		{
			//�e�L�X�g�f�[�^�Ɗ�摜�̃t�@�C����
			//m_acting = player.walkto(m_csv.get<int>(m_column, 2),
			//m_csv.get<int>(m_column, 3), map, object, mycamera.getCameraPos());
			if (!talk.isSetTalk())
			{
				/*
				String term1 = m_csv.get<String>(m_column, 2).lower();
				String leftIconName = m_csv.get<String>(m_column, 3).lower();

				talk.setShowTalk
					(
					term1
					, leftIconName
					);
					*/
				String talkfilename = m_csv.get<String>(m_column, 2).lower();
				talk.setNormalTalk(m_mapName + L"/" + talkfilename);
			}

			m_acting = talk.getTalkActing();

			if (!m_acting)
			{
				talk.reset();
				//talk.setIsSetTalk(false);
			}
		}

	}

	
	player.addFramecount();
	enemy.addFramecount();

	return ELPlayState::Event;
}

void ELEvent::clearEvents()
{
	m_setAutoEvents.clear();
	m_setTalkEvents.clear();
}

void ELEvent::draw(Point camerapos)
{
	for(size_t i= 0; i< m_setAutoEvents.size(); ++i)
	{
		Rect(m_setAutoEvents[i].actRect.pos-camerapos,
			m_setAutoEvents[i].actRect.size).draw(Palette::Red);
	}
}