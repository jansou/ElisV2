# include "Elis.hpp"

Elis::Elis():
	m_playState(ELPlayState::Playing),
	m_gameState(ELGameState::Title),
	m_doorOpened(false)
{

}

void Elis::blackFade()
{
	const unsigned alpha = static_cast<unsigned>(255.0*m_fadeOutTimer.elapsed()/fadeOutTime());
	Rect(0,0,1280,720).draw(Color(0,0,0,alpha));
}

void Elis::whiteFade()
{
	const unsigned alpha = static_cast<unsigned>(255.0*m_fadeOutTimer.elapsed()/fadeOutTime());
	Rect(0,0,1280,720).draw(Color(255,255,255,alpha));
}

void Elis::playingDraw()
{
	map.draw(mycamera.getCameraPos());

	objectInfo.draw(mycamera.getCameraPos());

	enemyInfo.draw(mycamera.getCameraPos());

	item.draw(mycamera.getCameraPos());

	player.draw(mycamera.getCameraPos());

	attack.draw(mycamera.getCameraPos());
}

bool Elis::loadData(const String& mapName,const String& stageName)
{
	enemyInfo.clearEnemies();

	enemyInfo.clearEnemyPlaces();

	if(!enemyInfo.setEnemysPlace(mapName, stageName))
	{
		LOG_ERROR(L"敵のデータがまずい");
		return false;
	}

	map.clearDoors();

	if(!map.loadStage(mapName, stageName))
	{
		LOG_ERROR(stageName + L"関連のデータに問題あり");
		return false;
	}

	objectInfo.clearObjects();

	if(!objectInfo.loadObjectInfo(mapName, stageName))
	{
		LOG_ERROR(stageName + L"関連のobjectデータに問題あり");
		return false;
	}

	item.clearItem();
	item.loadItem(mapName, stageName);

	elEvent.clearEvents();
	if(!elEvent.loadEvent(mapName,stageName))
	{}

	attack.clearShoots();

	return true;
}

bool Elis::init()
{
	Window::SetTitle(L" Elis");

	if(!player.init())
	{
		LOG_ERROR(L"player関連のデータに問題あり");
		return false;
	}

	if(!cursor.init())
	{
		LOG_ERROR(L"cursor関連のデータに問題あり");
		return false;
	}

	if(!enemyInfo.loadEnemyData())
	{
		LOG_ERROR(L"敵のデータがまずい");
		return false;
	}
	
	Cursor::SetStyle(CursorStyle::None);

	title.init();

	if(!stageSelect.init())
	{
		LOG_ERROR(L"stageselectのデータがまずい");
		return false;
	}

	loadResources();

	return true;
}

void Elis::loadResources()
{
	const int size = 10+17+2;

	ResourceInfo resources[size]=
	{
		/////////////////stageselect用
		ResourceInfo(L"texELcaveIcon",L"data/Elis/StageSelect/cave.png",L"texture"),
		ResourceInfo(L"texELfortressIcon",L"data/Elis/StageSelect/fortress.png",L"texture"),
		ResourceInfo(L"texELpalaceIcon",L"data/Elis/StageSelect/palace.png",L"texture"),
		ResourceInfo(L"texELportIcon",L"data/Elis/StageSelect/port.png",L"texture"),
		ResourceInfo(L"texELtempleIcon",L"data/Elis/StageSelect/temple.png",L"texture"),
		ResourceInfo(L"texELtowerIcon",L"data/Elis/StageSelect/tower.png",L"texture"),
		ResourceInfo(L"texELworldmap",L"data/Elis/StageSelect/worldmap.png",L"texture"),
		ResourceInfo(L"texELmapbit",L"data/Elis/StageSelect/mapbit.png",L"texture"),

		ResourceInfo(L"sndELstageselect",L"data/Elis/BGM/stageselect.mp3",L"sound"),
		ResourceInfo(L"sndELstage_choice",L"data/Elis/SE/System/stage_choice.mp3",L"sound")
		/////////////////////////////////////////////////////////////////

		//////////////////////UI用
		,
		/////////////体力ゲージ用
		ResourceInfo(L"ElisUIbase",L"data/Elis/UI/UIbase.png",L"texture"),
		ResourceInfo(L"ElisUIgem",L"data/Elis/UI/UIgem.png",L"texture"),
		ResourceInfo(L"ElBossUIgem",L"data/Elis/UI/bossUIgem.png",L"texture"),
		ResourceInfo(L"ElBossUIbase",L"data/Elis/UI/bossUIbase.png",L"texture"),
		
		////////////menu関係用
		ResourceInfo(L"texELmenu_base",L"data/Elis/UI/menu_base.png",L"texture"),
		ResourceInfo(L"texELmenu_pausemenu",L"data/Elis/UI/pausemenu.png",L"texture"),
		ResourceInfo(L"texELmenu_gameover",L"data/Elis/UI/gameover.png",L"texture"),
		ResourceInfo(L"texELmenu_askBackStage",L"data/Elis/UI/askBackStage.png",L"texture"),
		ResourceInfo(L"texELmenu_retry",L"data/Elis/UI/retry.png",L"texture"),
		ResourceInfo(L"texELmenu_backTitle",L"data/Elis/UI/backTitle.png",L"texture"),
		ResourceInfo(L"texELmenu_yes",L"data/Elis/UI/yes.png",L"texture"),
		ResourceInfo(L"texELmenu_no",L"data/Elis/UI/no.png",L"texture"),
		ResourceInfo(L"texElpausemenu1",L"data/Elis/UI/pausemenu1.png",L"texture"),
		ResourceInfo(L"texElbackStage",L"data/Elis/UI/backStage.png",L"texture"),
		/////////////魔法選択用
		ResourceInfo(L"texELmagicBase",L"data/Elis/UI/magicBase.png",L"texture"),
		ResourceInfo(L"texELmagicIcon",L"data/Elis/UI/magicIcon.png",L"texture"),
		ResourceInfo(L"elisMagic_change",L"data/Elis/SE/System/magic_change.mp3",L"sound")
		//////////////////////////////////////////////////////////////////

		////////////////////////////////////////カメラ効果用
		,
		ResourceInfo(L"ELSElittleShake",L"data/Elis/SE/System/littleShake.mp3",L"sound"),
		ResourceInfo(L"ELSEburst",L"data/Elis/SE/System/burst.mp3",L"sound")
		///////////////////////////////////////
	};

	for(int i=0;i<size;++i)
	{
		if(resources[i].resourceType==L"texture")
		{
			TextureAsset::Register(resources[i].tag,resources[i].path);
		}
		else if(resources[i].resourceType==L"sound")
		{
			SoundAsset::Register(resources[i].tag,resources[i].path);
		}
	}
}

bool Elis::update()
{
	//if(!m_pause.onPause())
	{
		cursor.update();

		switch (m_gameState)
		{
		case ELGameState::Title:
			{
				if(!m_fadeOutTimer.isActive && title.update(cursor.getCursorPos()))
				{
					if(title.getTitleMode() == ELTitleMode::Config)
					{
						player.changeKey(title.getIsRightHand());
						UI.changeKey(title.getIsRightHand());
					}
					else if(title.getTitleMode() == ELTitleMode::Title)
					{
						m_fadeOutTimer.restart();
					}
				}
				else if(fadeOutTime()<m_fadeOutTimer.elapsed())
				{
					m_fadeOutTimer.reset();
					title.rereaseTex();
					if(!stageSelect.loadSave())
					{
						LOG_ERROR(L"saveがまずい");
						return false;
					}
					m_gameState = ELGameState::StageSelect;

					break;
				}
				title.draw(cursor.getCursorPos());

				cursor.draw();

				blackFade();
			
				break;
			}
		case ELGameState::StageSelect:
			{
				if(!m_fadeOutTimer.isActive && stageSelect.update(cursor.getCursorPos()))
				{
					m_fadeOutTimer.restart();
				}
				else if(fadeOutTime()<m_fadeOutTimer.elapsed())
				{
					m_fadeOutTimer.reset();

					m_mapName = stageSelect.getMapName();
					m_stageName = L"stage00";
					m_playState = ELPlayState::Playing;

					if(!loadData(m_mapName, m_stageName))
					{
						return false;
					}

					player.restartProcess(map,enemyInfo);

					mycamera.update(player.getPlayerPos(),map.getMapSize());
						
					stageSelect.releaseData();

					m_gameState = ELGameState::Play;
					UI.resetChoicedMagic();
					TextureAsset::ReleaseAll();
					SoundAsset::ReleaseAll();

					break;
				}

				stageSelect.draw(cursor.getCursorPos());

				cursor.draw();

				blackFade();
				
				break;
			}
		case ELGameState::Play:
			switch(m_playState)
			{
				case ELPlayState::Playing:
				{
					map.BGMupdate(player.getState());

					if(UI.getPaused())
					{
						m_gameState = UI.pauseUpdate(cursor.getCursorPos());

						if(m_gameState != ELGameState::Play)
						{
							TextureAsset::ReleaseAll();
							SoundAsset::ReleaseAll();
							break;
						}
					}
					else
					{
						if (m_doorOpened)
						{
							if (fade.CircleFadeOut())
							{
								fade.resetCount();
								m_doorOpened = false;
								player.setPos(map.getDestPos());

								//Rect(1280, 720).draw(Palette::Black);

								if (!loadData(m_mapName, map.getDestName()))	return false;

								TextureAsset::ReleaseAll();
							}
						}
						else if(map.update(player.getRect(), player.getState()))//ドア開けた
						{
							m_doorOpened = true;
							/*
							player.setPos(map.getDestPos());

							Rect(1280,720).draw(Palette::Black);

							if(!loadData(m_mapName,map.getDestName()))	return false;

							TextureAsset::ReleaseAll();
							//Resource::ReleaseAllSounds();
							*/
						}
						else
						{
							m_playState = player.update(map, attack, cursor, enemyInfo, objectInfo,mycamera.getCameraPos());

							enemyInfo.update(map, player.getPlayerFootCollisionPos(), objectInfo, mycamera.getCameraPos(), item, attack);

							attack.update(map, mycamera.getCameraPos());

							if(item.update(player.getRect(), map, objectInfo,mycamera.getCameraPos(),cursor.getCursorRect()))
							{
								player.itemEffect(item.getItemType());
								item.erase(item.getIndex());
							}

							objectInfo.attackUpdate(attack, mycamera.getCameraPos());

							player.objectUpdate(mycamera.getCameraPos(), objectInfo);

							objectInfo.update(mycamera.getCameraPos());

							//player.setMagic(UI.update(player.getMgicType()));

							///////////////////////
							player.checkHit(enemyInfo, attack);

							enemyInfo.checkHit(attack);

							if(m_playState != ELPlayState::GameOver)
							{
								m_playState = elEvent.update(player,enemyInfo,mycamera,map,objectInfo,talk);

								if(m_playState == ELPlayState::Event)
									attack.clearShoots();
							
								if(m_playState != ELPlayState::Event)
								{
									m_playState = UI.update(player.getMagicType());
									player.setMagic(UI.getChoicedMagic());
								}
							}
							///////////////////////
							mycamera.update(player.getPlayerPos(),map.getMapSize());

							if(enemyInfo.getIsBoss())
								mycamera.cameraEffect(enemyInfo.getCameraEffect());

						}
						//elEvent.draw(mycamera.getCameraPos());
					}

					playingDraw();
				
					UI.draw(player.getMagicType(),player.getLife(),player.getMP());

					if(enemyInfo.getIsBoss())
						UI.bossDraw(enemyInfo.getbossType(), enemyInfo.getBossLife(), enemyInfo.getBossMaxLife());

					if(map.getHaveLogo())
						map.drawLogo();

					if(UI.getPaused())	UI.pauseDraw(cursor.getCursorPos());

					cursor.draw();

					if(m_playState == ELPlayState::GameOver)
						UI.resetMenu();

					if (m_doorOpened)
					{
						fade.CircleFadeOutDraw(player.getRect().center-mycamera.getCameraPos());
					}

					break;
				}
				case ELPlayState::EditMagic:
				{
					map.BGMupdate(player.getState());

					m_playState = UI.editMagicUpdate(cursor.getCursorPos());
					
					if(m_playState == ELPlayState::Playing)
					{
						player.setMagic(UI.getChoicedMagic());
					}

					playingDraw();

					UI.draw(player.getMagicType(),player.getLife(),player.getMP());

					if(enemyInfo.getIsBoss())
						UI.bossDraw(enemyInfo.getbossType(), enemyInfo.getBossLife(), enemyInfo.getBossMaxLife());

					UI.editMagicDraw(cursor.getCursorPos());

					cursor.draw();

					break;
				}
				case ELPlayState::GameOver:
				{
					map.BGMupdate(player.getState());

					if(UI.updateGameover(cursor.getCursorPos()))
					{
						if(UI.getMenuState() == ELMenuState::Retry)
						{
							if(!loadData(m_mapName,map.getStartName()))
							{
								return false;
							}

							player.restartProcess(map,enemyInfo);
							UI.resetChoicedMagic();
							m_playState = ELPlayState::Playing;
						}
						else if(UI.getMenuState() == ELMenuState::Accept)
						{
							m_gameState = ELGameState::StageSelect;
						}
					}
					

					playingDraw();
				
					UI.gameoverDraw(cursor.getCursorPos());

					cursor.draw();

					break;
				}
				case ELPlayState::Event:
				{
					//map.BGMupdate(player.getState());

					m_playState = elEvent.update(player,enemyInfo,mycamera,map,objectInfo,talk);

					playingDraw();

					if(map.getHaveLogo())
							map.drawLogo();
			
					if(mycamera.getGraphicEffect() != ELCameraGraphicEffect::null)
					{
						mycamera.cameraGraphicEffect();
					}


					if (talk.getTalkActing())
					{
						talk.normalUpdate();
						talk.draw();
					}

					//elEvent.draw(mycamera.getCameraPos());

					break;
				}
				case ELPlayState::Clear:
				{
					playingDraw();

					if(fade.FadeOut())
					{
						fade.resetCount();
						m_gameState = ELGameState::StageSelect;
					}

					
					break;
				}
			}

			break;
		}
	}
 
	//m_pause.update(m_next);	// タイトルに戻る場合 m_next に Title が代入される
	/*
	if(m_pause.pauseStarted())
	{
		//
		//	ポーズが開始された瞬間 true
		Cursor::SetStyle(CursorStyle::Default);
	}
	else
	{
		
	}
 */
	/*
	if(m_pause.returnedToGame())
	{
		Cursor::SetStyle(CursorStyle::None);
		//
		//	ポーズが終了された瞬間 true
		//
	}
 */
	return true;
}
/*
optional<Application::Type> App::next()
{
	return m_next;
}
*/
