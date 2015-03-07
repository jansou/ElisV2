# include "ELTalk.h"

ELTalk::ELTalk()
{
	m_column = 0;
	m_talkFont = Font(20, Typeface::Default);
	TextureAsset::Register(L"icon_test", L"data/Elis/Talk/Icon/test.png");
}

namespace
{

};

//void ELTalk::setShowTalk(const String& text, const String& iconName)
void ELTalk::setNormalTalk(const String& filename)
{
	//テキストの読み込み
	//m_talkPhrase.text = text;
	CSVReader csv(L"data/Elis/Talk/" + filename + L".csv");
	//アイコンの設定
	//m_talkPhrase.iconName = iconName;
	
	if (csv.isEmpty)
	{
		return;
	}

	int y=0;
	for (;;++y)
	{
		int x = 0;

		if (csv.get<String>(y, 0).lower() == L"end")
		{
			break;
		}

		TalkPhrase item;
		String command = csv.get<String>(y, x).lower();

		if (command == L"lefttalk")
		{
			item.direction = TalkDirection::LeftTalking;
		}
		else if (command == L"righttalk")
		{
			item.direction = TalkDirection::RightTalking;
		}
		else
		{
			item.direction = TalkDirection::Null;
		}

		++x;
		item.leftIconName = csv.get<String>(y, x).lower();
		++x;
		item.rightIconName = csv.get<String>(y, x).lower();
		++x;
		item.text1 = csv.get<String>(y, x).lower();
		++x;
		item.text2 = csv.get<String>(y, x).lower();
		++x;
		item.text3 = csv.get<String>(y, x).lower();
		
		m_talkPhrases.emplace_back(item);
	}

	m_isSetTalk = true;
	m_isTalkAct = true;
	m_column = 0;
}

void ELTalk::normalUpdate()
{
	if (Input::KeyEnter.clicked)
	{
		//m_isSetTalk = false;
		//m_isTalkAct = false;
		
		if (m_column+1 >=  m_talkPhrases.size())
		{
			m_isTalkAct = false;
		}
		else
		{
			++m_column;
		}
	}
}

void ELTalk::draw()
{
	Rect(128, 50, 1024, 180).draw(Color(255, 255, 255));

	m_talkFont(m_talkPhrases[m_column].text1).draw(370, 80, Palette::Black);
	m_talkFont(m_talkPhrases[m_column].text2).draw(370, 120, Palette::Black);
	m_talkFont(m_talkPhrases[m_column].text3).draw(370, 160, Palette::Black);

	switch (m_talkPhrases[m_column].direction)
	{
	case TalkDirection::LeftTalking:
	{
		TextureAsset(L"icon_" + m_talkPhrases[m_column].leftIconName).draw(110, 100/*140*/);
		TextureAsset(L"icon_" + m_talkPhrases[m_column].rightIconName).mirror().draw(1280 - 110 - 256, 100/*140*/,Alpha(100));

		break;
	}
	case TalkDirection::RightTalking:
	{
		TextureAsset(L"icon_" + m_talkPhrases[m_column].leftIconName).draw(110, 100/*140*/, Alpha(100));
		TextureAsset(L"icon_" + m_talkPhrases[m_column].rightIconName).mirror().draw(1280 - 110 - 256, 100/*140*/);

		break;
	}
	default:
		break;
	}
}
