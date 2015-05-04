//
//  Audio.h/Users/xiaodao/Documents/CocosProjects/Cocos_Snake/Classes/Audio.h
//  CocosProject
//
//  Created by MinakoKojima on 15/4/20.
//
//

#ifndef __CocosProject__Audio__
#define __CocosProject__Audio__

#pragma once

#include "AudioEngine.h"
USING_NS_CC;
using namespace experimental;

#include <iostream>
#include <string>

using namespace std;

namespace Audio{

    extern bool BGM_ON, ME_ON, SE_ON;
    extern double BGM_Volume, ME_Volume, SE_Volume; // 玩家设置的音量，Volume set by the Player
    extern double BGM_Volume2, ME_Volume2, SE_Volume2; //游戏设置的音量, Volume set by the Game
    
    inline void playSE(const string s){
        if (SE_ON){ // 如果音效开启
            AudioEngine::play2d("audio/SE/" + s, 0, SE_Volume * SE_Volume2);
        }
    }
    inline void playBGM(const string s){
        if (BGM_ON){
            AudioEngine::play2d("audio/BGM/" + s, 1, BGM_Volume * BGM_Volume2);
        }
    }
    
    inline void playME(const string s){
        BGM_Volume = 0;
        if (ME_ON == 1){
            AudioEngine::play2d("audio/ME/" + s, 0, ME_Volume * ME_Volume2);
        }
    }
    
    
    inline void playSEs(const vector<string> s){
        int i = rand() % s.size();
        playSE(s[i]);
    }
    
    /*void setBGMVolume(){
        AudioEngine::setVolume(Global::BGMid, Config->BGM_Volume * Config->BGM_Volume2);
    }*/
    //void playME(const string s);
    //void playSEs(const vector<string> s);
}

#endif /* defined(__CocosProject__Audio__) */
