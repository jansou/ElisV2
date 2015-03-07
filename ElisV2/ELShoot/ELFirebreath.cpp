# include "ELFirebreath.h"


ELFirebreath::ELFirebreath(
	const ELShootType& shootType,
		const ELShooter& shooter,
		const Vec2& beginpos,
		const Vec2& endpos):
	m_shootType(shootType),
	m_shooter(shooter),
	m_beginpos(beginpos),
	m_endpos(endpos),
	m_pos(beginpos),
	m_damege(3),//É_ÉÅÅ[ÉWó 
	m_collided(false),//è’ìÀîªíË
	m_erased(false),
	m_frameCount(0)
{
	
	TextureAsset::Register(L"texELfirebreath",
		L"data/Elis/Effect/Enemy/Firebreath.png");
	TextureAsset::Register(L"texELfirebreath_hit",
		L"data/Elis/Effect/Enemy/Firebreath_hit.png");
}

namespace
{
	//const double kSpeed = 0.01;
	const int kSpeed = 5;

	inline int width(const String &name)
	{
		return TextureAsset(name).width;
	}

	inline int height(const String &name)
	{
		return TextureAsset(name).height;
	}

	//9,27,22,12
	const int kx = 9;
	const int ky = 27;
	const int kw = 22;
	const int kh = 12;

	const int kAnimeTiming = 8;
	const int kEraseTime = kAnimeTiming*4;

	
};

void ELFirebreath::update(const ELMap& map)
{
	if(m_collided)
	{
		if(m_frameCount<kEraseTime-1)
		{
			m_frameCount += 1;
		}
		else
		{
			m_erased = true;
		}
		return;
	}

	//if(!m_collided)
	//{
		m_pos.x += kSpeed * face();
	//}
	m_hitRect = Rect(int(m_pos.x - 128/2 + kx), int(m_pos.y - 128/2 + ky), kw,kh);

	m_frameCount +=1;

	return;
}

void ELFirebreath::draw(const Point& camerapos)const
{
	//LOGFMT_ERR("shoot!!!!!");

	//Rect(m_hitRect.pos.x-camerapos.x,m_hitRect.pos.y-camerapos.y,m_hitRect.size.x,m_hitRect.size.y).draw(Palette::Aliceblue);
	
	int x = ((face()==1) ? 64 : 0);

	if(m_collided)
	{
		TextureAsset(L"texELfirebreath_hit")(0,(m_frameCount/kAnimeTiming)%(height(L"texELfirebreath_hit")/128)*128,128,128)
			.drawAt(m_hitRect.center-camerapos);
		//Circle(m_pos.x-camerapos.x,m_pos.y-camerapos.y,10).draw(Palette::Black);
		return;
	}

	TextureAsset(L"texELfirebreath")(x,(m_frameCount/kAnimeTiming)%(height(L"texELfirebreath")/64)*64,64,64)
			.drawAt(m_hitRect.center-camerapos);


	//Circle(m_pos.x-camerapos.x,m_pos.y-camerapos.y,10).draw(Palette::Black);
	//Circle(m_endpos.x-camerapos.x,m_endpos.y-camerapos.y,10).draw(Palette::Black);

	//Resource::Font(L"font").draw(Format()+direction(),0,40,Palette::Blue);

	//m_hitRect.drawFrame(1,1,Palette::Blue);
}

Point ELFirebreath::getPos() const
{
	return Point(int(m_pos.x),int(m_pos.y));
}

bool ELFirebreath::isHit(const Rect& body)
{
	if(!m_collided)
	{
		if(body.intersects( m_hitRect))
		{
			m_collided = true;
			m_frameCount = 0;
			return true;
		}
	}

	return false;
}

ELShooter ELFirebreath::getShooter() const
{
	return m_shooter;
}

int ELFirebreath::getDamage() const
{
	return m_damege;
}

bool ELFirebreath::isErased() const
{
	return m_erased;
}

ELShootType ELFirebreath::getShootType() const
{
	return m_shootType;
}

void ELFirebreath::giveErase()
{
	m_collided = true;
}