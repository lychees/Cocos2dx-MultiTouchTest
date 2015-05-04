//
//  Template.cpp
//  Cocos_Robot
//
//  Created by MinakoKojima on 15/4/21.
//
//

#include "Template.h"


int stoi(string s){
    int x = 0;
    bool flag = 0;
    
    if (s[0] == '-'){
        flag = 1;
    }
    
    FOR(i, (flag ? 1 : 0), SZ(s)){
        x *= 10;
        x += s[i] - '0';
    }
    if (flag) x = -x;
    return x;
}

string itos(int x){
    
    bool flag = 0;
    
    if (x < 0){
        flag = 1;
        x = -x;
    }
    
    string s;
    while (x){
        s += '0' + (x % 10);
        x /= 10;
    }
    if (s.empty()) s = "0";
    else reverse(s.begin(), s.end());
    
    if (flag) s = '-' + s;
    return s;
}