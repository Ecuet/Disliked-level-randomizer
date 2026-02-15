#pragma once

#include "Gamble.h"
#include <functional>

using namespace geode::prelude;

class GambleMenu : public Popup {
public:
	virtual bool init(float progress, bool returnBack, int skipsleft);
	static GambleMenu* create(float progress, bool returnBack, int skipsleft);

protected:
	void gamble(CCObject* sender);
	void Close(CCObject* sender);
	void skip(CCObject* sender);
	void Reset(CCObject* sender);

	bool returnBack = false;
};