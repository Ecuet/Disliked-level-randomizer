#include "GambleMenu.h"
#include <vector>
#include <algorithm>

using namespace geode::prelude;

std::vector<std::string> funnyThings = {
	"wanna keep gambling?","do you believe in Lady Luck?", "Hey, thats you again,", "im praying for you get the worst level ever dw,", "alr, try harder,", "thats me, omori!","nah, you'd lose,",
	"believe in yourself, who else will!","im worrying for you, please, give up,","ill be the judge,",":thumbsup:","i wont succumb,","idle death gamble:","merry christmas","i am the honored one",
	"you re cooked","wanna place a bet?","all on green","buy high sell low"
};



GambleMenu* GambleMenu::create(float progress, bool rb, int skipsleft) {
	auto menu = new GambleMenu;
	if (menu->init(progress, rb,skipsleft)) {
		menu->autorelease();
		return menu;
	}
	delete menu;
	return nullptr;
}

bool GambleMenu::init(float progress, bool rb,int skipsleft) {
	if (!Popup::init(350.f, 150.f)) return false;

	setTitle("mega super duper disliked level roulette 3k");
	setID("gamblepop"_spr);



	float x = m_mainLayer->getContentWidth() / 2;
	float y = (m_mainLayer->getContentHeight() / 2);

	std::string phrase = getRandomElement(funnyThings);
	float Textscale = std::clamp((m_mainLayer->getContentWidth() / phrase.length()) / (x / 7),0.2f,0.7f);

	auto phraseText = cocos2d::CCLabelBMFont::create(fmt::format("{}",phrase).c_str(), "bigFont.fnt");
	phraseText->setScale(Textscale);
	phraseText->setPosition({x,y + 25.f });

	m_mainLayer->addChild(phraseText);

	auto progressText = cocos2d::CCLabelBMFont::create(fmt::format("Your progress: {}%",progress).c_str(), "bigFont.fnt");
	progressText->setScale(.7f);
	progressText->setPosition({ x,y });

	m_mainLayer->addChild(progressText);


	auto menu = CCMenu::create();

	auto Skip = ButtonSprite::create(fmt::format("Skips left: {}", skipsleft).c_str());
	Skip->setScale(.5f);

	auto Reset = ButtonSprite::create("Reset");
	Reset->setScale(.5f);

	auto closebtn = CCMenuItemSpriteExtra::create(
		ButtonSprite::create("hell nah"),
		this,
		menu_selector(GambleMenu::Close)
	);

	auto gamblebtn = CCMenuItemSpriteExtra::create(
		ButtonSprite::create("i feel lucky"),
		this,
		menu_selector(GambleMenu::gamble)
	);
	auto skipbtn = CCMenuItemSpriteExtra::create(
		Skip,
		this,
		menu_selector(GambleMenu::skip)
	);
	auto resetbtn = CCMenuItemSpriteExtra::create(
		Reset,
		this,
		menu_selector(GambleMenu::Reset)
	);
	gamblebtn->setPosition({ 75.f ,0.f });
	closebtn->setPosition({ -90.f ,0.f});
	skipbtn->setPosition({ -30.f ,25.f });
	resetbtn->setPosition({ 50.f ,25.f });

	menu->setPosition({ x,25.f });
	menu->addChild(closebtn);
	menu->addChild(gamblebtn);
	menu->addChild(skipbtn);
	menu->addChild(resetbtn);

	m_mainLayer->addChild(menu);

	returnBack = rb;

	return true;
}

void GambleMenu::Close(CCObject* sender) {
	Popup::onClose(sender);

	Gamble::get().onButtonPress(false, returnBack);
	returnBack = false;

}

void GambleMenu::gamble(CCObject* sender) {
	Popup::onClose(sender);

	Gamble::get().onButtonPress(true, returnBack);
	returnBack = false;
}

void GambleMenu::skip(CCObject* sender) {
	Gamble::get().Skip();
}

void GambleMenu::Reset(CCObject* sender) {
	Popup::onClose(sender);
	Gamble::get().PromptReset();
}