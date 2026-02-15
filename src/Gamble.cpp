#include "Gamble.h"


void Gamble::GambleIt() {
    int randomLevel = 0;
    if (InProgress) {
        randomLevel = CurrentLevel;
    }
    else {

        randomLevel = getRandomElement(Levels);

        auto it = std::find(Levels.begin(), Levels.end(), randomLevel);
        if (it != Levels.end()) Levels.erase(it);

        if (Levels.empty()) {
            Levels = OriginalLevels;
            log::info("refilled levels cuz skill issue");
        }

        CurrentLevel = randomLevel;
        InProgress = true;
    }
    
    LoadLevelPopup::create(randomLevel)->show();
}

void Gamble::OpenPopup(bool returnBack) {
    GambleMenu::create(CurrentBest,returnBack,skipsLeft)->show();
}

void Gamble::NewBest(int best, bool returnBack) {
    InProgress = false;
    CurrentBest = best;

    if (best == 100) {
        FLAlertLayer::create(
            "Congratulations!", "YOU DID THIS! (why?) CONGRATULATIONS! I ALWAYS BELIEVED IN YOU! WAS IT WORTH IT? now play omori", "thanks, thanks"
        )->show();
        reset();
    }
    else {
        if (returnBack)
            loadNextAfterQuit = true;
        OpenPopup(returnBack);

    }
}

void Gamble::onButtonPress(bool btn2, bool returnBack) {
    if (btn2) {
        if (auto pl = PlayLayer::get()) {
            pl->onQuit();
        }
        else{
            GambleIt();
        }
        

    }
    else {
        RoulleteStarted = false;
    }
}

void Gamble::Skip() {
    if (skipsLeft > 0) {
        --skipsLeft;
        InProgress = false;

        if (auto pl = PlayLayer::get()) {
                if(CurrentBest >= 99) {
                    FLAlertLayer::create("nuh uh","you cant skip 99%",":sob:");
                    return;
                }
                ++CurrentBest;
                loadNextAfterQuit = true;
                pl->onQuit();
        }
        else{
             GambleIt();
        }

       
    }
}

void Gamble::PromptReset(){
    geode::createQuickPopup(
        "are you sure?",
        "this will reset your progress, so you ll have to start from scratch",
        "actually, nvm",
        "yeah",
        [](auto, bool btn2) {
            if(btn2){
                Gamble::get().reset();
              
                Notification::create("Done! now show em up",geode::NotificationIcon::Success,3.f)->show();
            }
        }
    );
}

void Gamble::reset() {
    CurrentBest = 0;
    CurrentLevel = 0;
    tempBest = 0;
    RoulleteStarted = false;
    InProgress = false;
    loadNextAfterQuit = false;
    disablePlay = false;
    Levels = OriginalLevels;
    skipsLeft = 3;

}

