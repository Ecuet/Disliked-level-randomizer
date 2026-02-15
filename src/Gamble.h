#pragma once
#include <Geode/Geode.hpp>

#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/modify/EndLevelLayer.hpp>
#include <Geode/modify/PlayerObject.hpp>

#include "GambleMenu.h"
#include "LoadLevelPopup.hpp"

#include <vector>
#include <random>
#include <chrono>
#include <thread>
#include <functional>

inline std::vector<int> OriginalLevels = {
   121377, 173373, 218652, 906967, 158119, 238113, 1957420, 74458, 86293, 1576350, 504100, 203156, 1670841,
   760070, 151076, 1225197, 296176, 227173, 111010, 1200831, 468396, 1123217, 1971304, 277503, 107417388,
   111386, 488134, 666203, 1060305, 279361, 99361, 1116402, 249090, 60661, 215633, 1291078, 263287, 159931,
   191751, 159892, 120185, 72540, 1224259, 126788818, 1412727, 1174702, 123247, 2278753, 481354, 60771,
   110133, 1803901, 1280432, 369214, 152389, 2473054, 59297, 1938283, 2080527, 400123, 620464, 184440, 169862,
   153697, 541675, 160230, 1193581, 134458, 1946486, 72437, 922621, 220369, 999024, 1285881, 601365, 218497,
   277142, 53884, 608361, 179906, 82348741, 1655600, 448229, 83434, 960890, 104304230, 216526, 114127, 72131,
   511179, 68184, 293446, 1908735, 340647
};

template <typename T>
T getRandomElement(std::vector<T> const& vec) {
	static std::mt19937 generator(std::chrono::system_clock::now().time_since_epoch().count());
	std::uniform_int_distribution<size_t> distribution(0, vec.size() - 1);
	return vec[distribution(generator)];
}


class Gamble {
private:
	Gamble() {};

public:
	Gamble(Gamble const&) = delete;
	Gamble operator=(Gamble const&) = delete;

	static Gamble& get() {
		static Gamble instance;
		return instance;
	}



	void GambleIt();
	void OpenPopup(bool returnBack);
	void NewBest(int best, bool returnBack);

	void onButtonPress(bool btn2, bool returnback);

	void reset();
	void PromptReset();

	void Skip();

	std::vector<int> Levels = OriginalLevels;

	int CurrentBest = 0;
	int CurrentLevel = 0;
	int tempBest = 0;
	bool RoulleteStarted = false;
	bool InProgress = false;
	bool loadNextAfterQuit = false;
	bool disablePlay = false;
	int skipsLeft = 3;
};