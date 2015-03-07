# pragma once
# include <Siv3D.hpp>
# include "ELCommon.h"
//# include "../Common.hpp"

enum TalkDirection
{
	LeftTalking,
	RightTalking,
	Null,
};


struct TalkPhrase
{
	TalkPhrase()
	{}

	TalkPhrase(
		const TalkDirection& _talkDirection,
		const String& _leftIconName,
		const String& _rightIconName,
		const String& _text1,
		const String& _text2, 
		const String& _text3
		)
		:direction(_talkDirection),
		leftIconName(_leftIconName),
		rightIconName(_rightIconName),
		text1(_text1), 
		text2(_text2),
		text3(_text3)
	{

	}

	TalkDirection direction;

	String leftIconName;
	String rightIconName;

	String text1;
	String text2;
	String text3;
};

class ELTalk
{
private:
	bool m_isSetTalk;
	bool m_isTalkAct;//処理中か。falseなら終了

	std::vector< TalkPhrase > m_talkPhrases;

	Font m_talkFont;

	int m_column;
public:
	ELTalk();

	//void setShowTalk(const String &text, const String &iconName);
	void setNormalTalk(const String& filename);//指定した会話csvを読み込む

	void normalUpdate();

	bool isSetTalk()
	{
		return m_isSetTalk;
	}

	void setIsSetTalk(bool b)
	{
		m_isSetTalk = b;
	}

	bool getTalkActing()
	{
		return m_isTalkAct;
	}
	
	void draw();

	void reset()
	{
		m_isSetTalk = false;
		m_talkPhrases.clear();
	}
};