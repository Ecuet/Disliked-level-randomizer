#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/EndLevelLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/cocos/layers_scenes_transitions_nodes/CCLayer.h>
#include <Geode/binding/PlayerObject.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/cocos/sprite_nodes/CCSprite.h>
#include "LoadLevelPopup.hpp"
#include <vector>
#include <random>
#include <chrono>
#include <thread>

using namespace geode::prelude;

std::vector<int> OriginalLevels = {
    887,       286279,    226632,  292418,   270128,  1851973,   790262,  252772,    808068,
    2429387,   212938,    1138878, 129053,   120491,  242188,    496763,  1879951,   615520,
    193345,    180857,    100584,  1165107,  1250634, 109139,    219938,  111010,    593413,
    109990,    215832,    1808513, 116631,   1431395, 87582,     1669203, 92287510,  846870,
    199557,    2318902,   412557,  214942,   1215696, 15217666,  827470,  158646,    1957420,
    1576350,   74458,     86293,   504100,   203156,  1670841,   1225197, 760070,    151076,
    296176,    227173,    1971304, 468396,   1200831, 1123217,   277503,  99361,     111386,
    107417388, 666203,    488134,  279361,   249090,  1060305,   215633, 1291078,
    1116402,   263287,    159892,   159931,  191751,    60661,   120185,    72540,
    1224259,   123247,    1412727,  1174702, 152389,    2278753, 481354,    110133,
    60771,     1280432,   1803901, 369214,   2473054, 1938283,   59297,   620464,    2080527,
    160230,    169862,    400123,  184440,   153697, 541675,     1193581, 134458,    340647,
    1946486,   72437,     220369,  922621,   999024,  1285881,   601365,  218497,    179906,
    277142,    608361,    53884,   82348741, 1655600, 448229,    83434,   104304230, 960890,
    216526,    114127,    72131,   511179,   68184,   293446,    1908735
};

std::vector<int> Levels = OriginalLevels;

int CurrentBest = 0;
int CurrentLevel = 0;
int tempBest = 0;
bool RoulleteStarted = false;
bool InProgress = false;
bool loadNextAfterQuit = false;
bool disablePlay = false;

//PlayLayer* pl;

template <typename T>
T getRandomElement(std::vector<T> const& vec) {
    static std::mt19937 generator(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<size_t> distribution(0, vec.size() - 1);
    return vec[distribution(generator)];
}

void reset() {
    CurrentBest = 0;
    CurrentLevel = 0;
    tempBest = 0;
    RoulleteStarted = false;
    InProgress = false;
    loadNextAfterQuit = false;
    disablePlay = false;
    Levels = OriginalLevels;

}

void Gamble() {

    int randomLevel = 0;
    if (InProgress) {
        randomLevel = CurrentLevel;
    }
    else {
       
        randomLevel = getRandomElement(Levels);

        auto it = std::find(Levels.begin(), Levels.end(), randomLevel);
        if (it != Levels.end()) Levels.erase(it); 

        CurrentLevel = randomLevel;
        InProgress = true;
    }


   
    LoadLevelPopup::create(randomLevel)->show();
    
}

void OpenPopup(bool returnBack) {
    
    geode::createQuickPopup(
        "ultra mega disliked level roulette 3k",
        "wanna keep gambling? now your progress " + numToString(CurrentBest) + "%",
        "hell nah",
        "i feel lucky",
        [returnBack](auto, bool btn2) {
            if (btn2) {
                if (returnBack) {
                    if (auto pl = PlayLayer::get()) {
                        disablePlay = true;
                        pl->onQuit();
                    }
                }
                else {
                    Gamble();
                }
                
            }
            else {
                RoulleteStarted = false;
            }
        }
    ); 
}

void NewBest(int best,bool returnBack) {
    InProgress = false;
    CurrentBest = best;

    if (best == 100) {
        FLAlertLayer::create(
            "Congratulations!", "You really completed this horrible challenge (why?)", "yeah i did"
        )->show();
        reset();
    }
    else {
        if (returnBack)
            loadNextAfterQuit = true;
        OpenPopup(returnBack);
        
    }
}

class $modify(MyMenuLayer, MenuLayer) {
public:
    void changemoreGameButton() {
        auto menu = this->getChildByID("more-games-menu");
        if (menu) {
            auto morebutton = menu->getChildByID("more-games-button");
            if (morebutton) {
                CCSprite* sprite = CCSprite::create("gamble.png"_spr);

                static_cast<CCMenuItemSpriteExtra*>(morebutton)->setNormalImage(sprite);

                morebutton->setVisible(true);
            }
        }
        else {
            log::info("menu wasnt found");
        }
    }

    void onMoreGames(CCObject * sender) {
        OpenPopup(false);
        RoulleteStarted = true;

    }

    bool init() {
        if (!MenuLayer::init()) return false;
        changemoreGameButton();
 
        

        return true;
    }
};

class $modify(LevelInfoLayer) {
public:
    void onEnterTransitionDidFinish() {
  

        LevelInfoLayer::onEnterTransitionDidFinish();
      //  CCDirector::get()->pushScene(MenuLayer::scene(false));
         if (loadNextAfterQuit && RoulleteStarted) {
            loadNextAfterQuit = false;   
            Gamble();                 
            
         }
         int id = this->m_level->m_levelID;
         if (CurrentLevel == id) disablePlay = false;
    }

    void onPlay(CCObject* sender) {
        if (!disablePlay || !RoulleteStarted) {
            LevelInfoLayer::onPlay(sender);
        }
    }

};


class $modify(EndLevelLayer) {     
public:  
    void onMenu(CCObject* sender) {
        if (RoulleteStarted) {
            OpenPopup(false);
        }
        else {
            EndLevelLayer::onMenu(sender);
        }
        
        
    }
};

class $modify(PlayLayer) {
public:
    void levelComplete() {
        if (RoulleteStarted) NewBest(100,false);

        PlayLayer::levelComplete();
    }

};

class $modify(PlayerObject) {
public:
    void playerDestroyed(bool p) {
        if (RoulleteStarted) {
            PlayLayer* pl = PlayLayer::get();
            int CurrentPerc = pl->getCurrentPercentInt();
            if (CurrentPerc > CurrentBest) {
                pl->pauseGame(true);

                tempBest = CurrentPerc;

                NewBest(tempBest, true);
            }
        }   

        PlayerObject::playerDestroyed(p);
    }
};
