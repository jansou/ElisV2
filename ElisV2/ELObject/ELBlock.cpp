# include "ELBlock.h"

ELBlock::ELBlock(const Point& setPos):
	m_setPos(setPos),
	m_broke(false),
	m_erased(false),
	m_frameCount(0)
{
	m_hitRect = Rect(m_setPos.x-32,m_setPos.y-64,64,64);

	//TextureAsset::Register(L"texELblock",L"data/Elis/Object/block/block.png");
	//TextureAsset::Register(L"texELblock_erase",L"data/Elis/Object/block/block_erase.png");
	//SoundAsset::Register(L"sndELBlockBreak",L"data/Elis/SE/Object/blockBreak.mp3");
}

namespace
{
	const bool kTouchable = true;
	const int kAnimeTiming = 8;
};

void ELBlock::update()
{
	if(m_broke)
	{
		m_frameCount += 1;

		if(m_frameCount == kAnimeTiming*2)
		{
			m_erased = true;
		}
	}
}

void ELBlock::draw(const Point& camerapos)const
{
	if(m_broke)
	{
		TextureAsset(L"texELblock_erase")(0,128*(m_frameCount/kAnimeTiming),128,128).drawAt(m_hitRect.center-camerapos);

		return;
	}


	TextureAsset(L"texELblock").draw(m_hitRect.pos-camerapos);
	//m_hitRect.drawFrame(10,10,Palette::Aliceblue);
}

bool ELBlock::intersectBody(const Rect& body)
{
	return false;
}

Point ELBlock::getPos() const
{
	return m_setPos;
}

Rect ELBlock::getRect() const
{
	return m_hitRect;
}

bool ELBlock::isTouchable(const Point& head,const Point& foot) 
{
	if(m_broke) return false;

	//return Line(foot,head).intersects(m_hitRect);
	int n =std::abs(foot.y-head.y);

	for(int i=0;i<=n;++i)
	{
		if(Point(foot.x,foot.y - i).intersects(m_hitRect))
		{
			return true;	
		}
	}

	return false;
}

bool ELBlock::attackUpdate(const ELShootType& shootType)//Á–Å‚·‚é‚Æ‚«‚Ítrue
{
	if(shootType == ELShootType::ElisIce
		|| shootType == ELShootType::LongIce)
	{
		if(SoundAsset(L"sndELBlockBreak").isPlaying) SoundAsset(L"sndELBlockBreak").stop();

		SoundAsset(L"sndELBlockBreak").play();

		m_broke = true;

		return false;
	}

	return true;
}

bool ELBlock::getErased() const
{
	return m_erased;
}