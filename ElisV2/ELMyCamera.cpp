# include "ELMyCamera.h"

ELMyCamera::ELMyCamera()
	:m_framecount(0),
	m_graphicEffect(ELCameraGraphicEffect::null),
	m_burstPos(Point(0,0)),
	m_flashed(false)
{
	
}

namespace
{
	const int kSideSpace = 0;

	const int kUpSpace = 0;

	const int kDownSpace = 0;
};

Point ELMyCamera::getCameraPos()
{
	return m_camerapos;
}

void ELMyCamera::update(const Point& playerPos,const Point& mapSize)
{

	Point pos = -Point((1280-128)/2,720/5*3) + playerPos;/*プレイヤー位置からカメラを引いたもの*/

	//以下カメラの位置調整
	if(pos.x<0)
	{
		pos.x = 0;
	}

	if(pos.x+1280 > mapSize.x)
	{
		pos.x = mapSize.x - 1280;
	}

	if(pos.y<0)
	{
		pos.y = 0;
	}

	if(pos.y+720>mapSize.y)
	{
		pos.y = mapSize.y-720;
	}

	m_camerapos = pos;
}

void ELMyCamera::cameraEffect(const ELCameraEffect &cameraEffect)
{
	switch (cameraEffect)
	{
	case ELCameraEffect::LittleShake:
		{
			if(!SoundAsset(L"ELSElittleShake").isPlaying)
				SoundAsset(L"ELSElittleShake").play();

			const int wavePower = 6;
		
			m_camerapos += Point(Random(-wavePower,wavePower),Random(-wavePower,wavePower));
			break;
		}
	default:
		break;
	}
}

bool ELMyCamera::setBurst(const Point &burstpos)
{
	if(m_framecount > 300)
	{
		m_graphicEffect = ELCameraGraphicEffect::null;
		m_framecount = 0;
		m_flashed = false;
		return false;
	}

	m_burstPos = burstpos;
	m_graphicEffect = ELCameraGraphicEffect::WhiteBurn;

	//++m_framecount;

	

	return true;
}

void ELMyCamera::cameraGraphicEffect()
{
	switch (m_graphicEffect)
	{
	case ELCameraGraphicEffect::WhiteBurn:
		{
		const Vec2 pos = m_burstPos - m_camerapos;
		const int Interval = 10;

		if(m_framecount == 0 && !m_flashed)
		{
			SoundAsset(L"ELSEburst").play();
		}

		if(!m_flashed&&((m_framecount>10 && m_framecount <20)
			|| (m_framecount>40&&m_framecount<50)))
		{
			Rect(0,0,1280,720).draw(Color(255,255,255));
		}

		if(m_framecount >60 && !m_flashed)
		{
			m_flashed = true;
			m_framecount = 0;
		}


		if(m_flashed)
		{
			if(m_framecount > Interval*7)
				Triangle(pos,Vec2(0,200),Vec2(0,150)).draw(Palette::White);
			if(m_framecount > Interval*6)
				Triangle(pos,Vec2(0,720),Vec2(0,670)).draw(Palette::White);
			if(m_framecount > Interval*5)
				Triangle(pos,Vec2(1280,720),Vec2(1230,720)).draw(Palette::White);
			if(m_framecount > Interval*4)
				Triangle(pos,Vec2(1280,200),Vec2(1280,250)).draw(Palette::White);
			if(m_framecount > Interval*2)
				Triangle(pos,Vec2(1000,0),Vec2(1050,0)).draw(Palette::White);
			if(m_framecount > Interval*3)
				Triangle(pos,Vec2(350,0),Vec2(400,0)).draw(Palette::White);

			Rect(0,0,1280,720).draw(Color(255,255,255,Min(255,m_framecount*2)));
		}

			break;
		}
	case ELCameraGraphicEffect::null:
		break;
	default:
		break;
	}

	++m_framecount;
}