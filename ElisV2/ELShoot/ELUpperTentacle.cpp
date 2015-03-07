# include "ELUpperTentacle.h"


ELUpperTentacle::ELUpperTentacle(
	const ELShootType& shootType,
		const ELShooter& shooter,
		const Vec2& beginpos,
		const Vec2& endpos):
	m_shootType(shootType),
	m_shooter(shooter),
	m_beginpos(beginpos),
	m_endpos(endpos),
	m_pos(beginpos),
	m_damege(5),//É_ÉÅÅ[ÉWó 
	m_collided(false),//è’ìÀîªíË
	m_erased(false),
	m_frameCount(0),
	m_preActed(false),
	m_rising(true),
	m_staying(false)
{
	//TextureAsset::Register(L"ELtexTentacle_Fin",
	//	L"data/Elis/Effect/Seaserpent/fin.png");

	TextureAsset::Register(L"ELtexTentacle_Ice",
		L"data/Elis/Effect/Seaserpent/ice.png");

	for(int i = 0;i<10;++i)
	{
		m_nodes[i] = beginpos;
	}

	m_preTargetPos = beginpos - Vec2(0,40);
	m_targetPos = beginpos - Vec2(0,500);
}

namespace
{
	inline int width(const String &name)
	{
		return TextureAsset(name).width;
	}

	inline int height(const String &name)
	{
		return TextureAsset(name).height;
	}

	const int kx = 47;
	const int ky = 9;
	const int kw = 34;
	const int kh = 111;

	const int kAnimeTiming = 8;
	const int kEraseTime = 40;

	const unsigned int size = 10;
	const double kPreSpeed = 4.0;
	const double kSpeed = 20.0;
	const double kFallSpeed = 8.0;

	const int kStayTime = 20;
};

void ELUpperTentacle::update(const ELMap& map)
{
	if(m_staying)
	{
		if(++m_frameCount > kStayTime)
		{
			m_frameCount = 0;
			m_staying = false;
		}
	}
	else if(!m_preActed)
	{
		if(m_rising && m_pos.y <= m_preTargetPos.y)
		{
			m_rising = false;
		}

		for(int i=0;i<size;++i)
		{
			m_nodes[i] = m_beginpos - (Abs(m_beginpos - m_pos)/size * i);
		}

		m_pos -= Vec2(0,((m_rising) ? 1 : -1) * kPreSpeed);

		if(!m_rising && m_beginpos.y <= m_pos.y)
		{
			m_pos = m_beginpos;
			m_preActed = true;
			m_staying = true;
			m_rising = true;
		}
	}
	else
	{
		m_pos -= Vec2(0,(m_rising) ? kSpeed : -kFallSpeed);

		for(int i=0;i<size;++i)
		{
			m_nodes[i] = m_beginpos - (Abs(m_beginpos - m_pos)/size * i);
		}

		if(!m_rising && m_beginpos.y <= m_pos.y)
		{
			m_pos = m_beginpos;
			m_erased = true;
		}

		if(!m_rising && m_pos.y == m_beginpos.y)
		{
			m_rising = true;
		}
		else if(m_rising && m_pos.y <= m_targetPos.y)
		{
			m_rising = false;
			m_staying = true;
		}
	}

	m_hitRect = Rect(int(m_pos.x - 128 / 2 + kx), int(m_pos.y - 128 / 2 + ky), kw, kh);

	return;
}

void ELUpperTentacle::draw(const Point& camerapos)const
{
	const int r = 10;

	for(int i = 0;i<size-1;++i)
	{
		Circle cir(m_nodes[i],r);
		cir.draw(Color(67,92,117));
		cir.drawFrame(0.5,0.5,Palette::Black);
	}

	

	const double radian = Radians(90.0);
	//TextureAsset(L"ELtexTentacle_Fin").rotate(-radian).drawAt(m_nodes[size-2]);
	TextureAsset(L"ELtexTentacle_Ice").rotate(-radian).drawAt(m_pos);
	
	//LOGFMT_ERR("shoot!!!!!");

	//Rect(m_hitRect.pos.x-camerapos.x,m_hitRect.pos.y-camerapos.y,m_hitRect.size.x,m_hitRect.size.y).draw(Palette::Aliceblue);
	

	//Circle(m_pos.x-camerapos.x,m_pos.y-camerapos.y,10).draw(Palette::Black);
	//Circle(m_endpos.x-camerapos.x,m_endpos.y-camerapos.y,10).draw(Palette::Black);

	//Resource::Font(L"font").draw(Format()+direction(),0,40,Palette::Blue);

	//m_hitRect.drawFrame(10,0,Palette::Black);
	/*
	m_hitRect.rotatedAt(
		m_hitRect.center.x,
		m_hitRect.center.y,
		Atan2(direction().y, direction().x)
		).draw();
		*/
}

Point ELUpperTentacle::getPos() const
{
	return Point(int(m_pos.x),int(m_pos.y));
}

bool ELUpperTentacle::isHit(const Rect& body)
{
	if(!m_collided)
	{
		/*
		if(body.intersects( 
			m_hitRect.rotatedAt(
				m_hitRect.center.x,
				m_hitRect.center.y,
				Atan2(direction().y,direction().x)
				)))
				*/
		if (body.intersects(m_hitRect))
		{
			m_collided = true;
			m_frameCount = 0;
			return true;
		}
	}

	return false;
}

ELShooter ELUpperTentacle::getShooter() const
{
	return m_shooter;
}

int ELUpperTentacle::getDamage() const
{
	return m_damege;
}

bool ELUpperTentacle::isErased() const
{
	return m_erased;
}

ELShootType ELUpperTentacle::getShootType() const
{
	return m_shootType;
}

void ELUpperTentacle::giveErase()
{
	m_collided = true;
}