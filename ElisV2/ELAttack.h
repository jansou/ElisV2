# pragma once
# include <Siv3D.hpp>
# include "ELMap.h"
# include "ELShoot\ELShoot.h"
# include "ELShoot\ELElisShoot1.h"
# include "ELShoot\ELElisShoot2.h"
# include "ELShoot\ELElisShoot3.h"
# include "ELShoot\ELElisIce.h"
# include "ELShoot\ELArrow.h"
# include "ELShoot\ELCurveAxe.h"
# include "ELShoot\ELSwordrain.h"
# include "ELShoot\ELElisFire.h"
# include "ELShoot\ELFirebreath.h"
# include "ELShoot\ELElisWind.h"
# include "ELShoot\ELRockfall.h"
# include "ELShoot\ELUpperTentacle.h"
# include "ELShoot\ELLongIce.h"

# include "ELCommon.h"




class ELAttack
{
private:
	std::vector<std::shared_ptr<ELShoot>> m_shoots;
	bool incamera(const Point& pos, const Point& camerapos)const;

public:
	ELAttack();
	void update(const ELMap& map, const Point& camerapos);

	void draw(const Point& camerapos)const;

	/*
	makeShoot
(
自分か敵か、
玉の種類、
始点、
終点)
の要素からなる関数。
応じた弾を作ってくれる。
	*/
	void makeShoot(
		const ELShooter& shooter,
		const ELShootType& shootType,
		const Point& beginpos,
		const Point& endpos
		);

	int getDamage(const Rect &body, const ELShooter &shooter) const;

	void clearShoots() {m_shoots.clear();}

	int objectHit(const Rect &body);

	void erase(int index);

	void giveErase(int index);

	ELShootType getShootType(int index) const;
};