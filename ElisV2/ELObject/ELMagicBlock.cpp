# include "ELMagicBlock.h"

ELMagicBlock::ELMagicBlock(const Point& setPos):
	m_setPos(setPos),
	m_broke(false),
	m_erased(false),
	m_frameCount(0),
	m_Animed(false)
{
	m_hitRect = Rect(m_setPos.x-32,m_setPos.y-64,64,64);
	/*
	TextureAsset::Register(L"texELMagicBlock",L"data/Elis/Object/magicBlock/magicBlock.png");

	TextureAsset::Register(L"texELMagicBlock_Anime",L"data/Elis/Object/magicBlock/magicBlock_Anime.png");
	*/
}

namespace
{
	const bool kTouchable = true;
	const int kAnimeTiming = 8;
	const int kEraseTime = 200;
};

void ELMagicBlock::update()
{
	if(!m_Animed && kAnimeTiming*3 > m_frameCount)
	{
		m_frameCount += 1;
		return;
	}

	if(!m_Animed)
	{
		m_Animed = true;
		m_frameCount = 0;
	}

	if(m_broke)
	{
		if(m_frameCount > kAnimeTiming*2-2)	m_erased = true;
	}
	else if(m_frameCount > kEraseTime)
		m_erased = true;

	m_frameCount += 1;
}

void ELMagicBlock::draw(const Point& camerapos)const
{
	if(!m_Animed)
	{
		TextureAsset(L"texELMagicBlock_Anime")(0,64*(m_frameCount/kAnimeTiming),64,64).drawAt(m_hitRect.center-camerapos);
		return;
	}

	if(m_broke)
	{
		TextureAsset(L"texELblock_erase")(0,128*(m_frameCount/kAnimeTiming),128,128).drawAt(m_hitRect.center-camerapos);

		return;
	}

	int a = 255;

	if(m_frameCount > kEraseTime/2)
		a -= 100;

	TextureAsset(L"texELMagicBlock").draw(m_hitRect.pos-camerapos, Alpha(a));
	//m_hitRect.drawFrame(10,10,Palette::Aliceblue);
}

bool ELMagicBlock::intersectBody(const Rect& body)
{
	return false;
}

Point ELMagicBlock::getPos() const
{
	return m_setPos;
}

Rect ELMagicBlock::getRect() const
{
	return m_hitRect;
}

bool ELMagicBlock::isTouchable(const Point& head,const Point& foot)
{
	if(m_broke) return false;

	//return Line(foot,head).intersects(m_hitRect);
	int n =std::abs(foot.y-head.y);

	for(int i=0;i<=n;++i)
	{
		if(Point(foot.x,foot.y - i).intersects(m_hitRect))
		{
			if(!m_broke && !m_Animed)
			{
				//LOGFMT_ERR("aaaaaaaaaaaaaaaaaaaaaaaa");

				m_broke = true;
				m_frameCount = 1;
				return false;
			}

			return true;	
		}
	}

	return false;
}

bool ELMagicBlock::attackUpdate(const ELShootType& shootType)//Á–Å‚·‚é‚Æ‚«‚Ítrue
{
	if(shootType == ELShootType::ElisIce)
	{
		if(!SoundAsset(L"sndELBlockBreak").isPlaying) SoundAsset(L"sndELBlockBreak").stop();

		SoundAsset(L"sndELBlockBreak").play();

		m_broke = true;

		m_frameCount = 1;

		return false;
	}

	return true;
}

bool ELMagicBlock::getErased() const
{
	return m_erased;
}