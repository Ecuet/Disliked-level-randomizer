#include "Gamble.h"



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
        Gamble::get().OpenPopup(false);
        Gamble::get().RoulleteStarted = true;

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
        Gamble& gamble = Gamble::get();

        LevelInfoLayer::onEnterTransitionDidFinish();

        if (gamble.loadNextAfterQuit && gamble.RoulleteStarted) {
            gamble.loadNextAfterQuit = false;
            gamble.GambleIt();

        }
        int id = this->m_level->m_levelID;
        if (gamble.CurrentLevel == id) gamble.disablePlay = false;
    }

    void onPlay(CCObject * sender) {
        Gamble& gamble = Gamble::get();
        if (!gamble.disablePlay || !gamble.RoulleteStarted) {
            LevelInfoLayer::onPlay(sender);
        }
    }

};


// class $modify(EndLevelLayer) {
// public:
//    void onMenu(CCObject * sender) {
//        Gamble& gamble = Gamble::get();
//        if (gamble.RoulleteStarted) {

//            gamble.OpenPopup(false);
//        }
//        else {
//            EndLevelLayer::onMenu(sender);
//        }


//    }
// };

class $modify(PlayLayer) {
public:
    
    void levelComplete() {
        Gamble& gamble = Gamble::get();
        PlayLayer* pl = PlayLayer::get();

        int id = pl->m_level->m_levelID;

        if (gamble.RoulleteStarted && id == gamble.CurrentLevel) gamble.NewBest(100, false);

        PlayLayer::levelComplete();
    }

};

class $modify(PlayerObject) {
public:
    struct Fields
    {
        int lastWarningShew = 0;
    };
    void playerDestroyed(bool p) {
        Gamble& gamble = Gamble::get();
        if (gamble.RoulleteStarted) {
            PlayLayer* pl = PlayLayer::get();

            int CurrentPerc = pl->getCurrentPercentInt();
            int id = pl->m_level->m_levelID;
            if (CurrentPerc > gamble.CurrentBest && id == gamble.CurrentLevel) {
                pl->pauseGame(true);

                gamble.tempBest = CurrentPerc;


                Gamble::get().NewBest(gamble.tempBest, true);
            }
            else if(id == gamble.CurrentLevel && m_fields->lastWarningShew != id){
                int attempts = pl->m_attempts;
                if(attempts > 50 && gamble.skipsLeft > 0){
                    Notification::create("stuck? you can use skip btw",NotificationIcon::Info,3.f)->show();
                    m_fields->lastWarningShew = id;
                }
            }
        }

        PlayerObject::playerDestroyed(p);
    }
};
