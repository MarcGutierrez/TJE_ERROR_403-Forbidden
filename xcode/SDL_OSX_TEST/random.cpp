//
//  random.cpp
//  TJE_XCODE
//
//  Created by manu on 27/6/23.
//  Copyright © 2023 manu. All rights reserved.
//

#include <stdio.h>
#include "random.h"

int get_random_sign()
{
    static std::default_random_engine e(time(0));
    static std::uniform_real_distribution<float> dis(0, 1); // range [0, 1)
    return (dis(e) > .5f) ? -1 : 1;
}

float get_random_dist()
{
    static std::default_random_engine e(time(0));
    static std::uniform_real_distribution<float> dis(750, 1000); // range [400, 800)
    static std::normal_distribution<float> norm(300, 75);
    return dis(e) + get_random_sign()*norm(e);
}

int get_random_enemy_num(int diff)
{
    static std::default_random_engine e(time(0));
    static std::uniform_int_distribution<int> dis(10, 20); // range [1, 20]
    return dis(e) + diff; // in essence a gaussian area surrounding the spawn square to distribute enemies evenly
}

float get_random_spd()
{
    static std::default_random_engine e(time(0));
    static std::uniform_real_distribution<float> dis(950.f, 1700.f); // range [min, max)
    return dis(e);
}

float get_random_cdShot()
{
    static std::default_random_engine e(time(0));
    static std::uniform_real_distribution<float> dis(0.5f, 3.f); // range [min, max)
    return dis(e);
}

float get_random_disp()
{
    static std::default_random_engine e(time(0));
    static std::uniform_real_distribution<float> dis(.1f, .3f); // range [min, max)
    return dis(e);
}

float get_random_dispBoss()
{
    static std::default_random_engine e(time(0));
    static std::uniform_real_distribution<float> dis(.15f, .25f); // range [min, max)
    return dis(e);
}

int get_random_bulletsBoss(int diff)
{
    static std::default_random_engine e(time(0));
    static std::uniform_int_distribution<int> dis(3, 5); // range [3, 5]
    return dis(e) + diff * 2;
}

int get_random_hpBoss(int diff)
{
    static std::default_random_engine e(time(0));
    static std::uniform_int_distribution<int> dis(35, 60); // range [35, 60]
    return dis(e) + diff;
}

int get_random_powerUP()
{
    static std::default_random_engine e(time(0));
    static std::uniform_int_distribution<int> dis(1, 3); // range [1, 3]
    return dis(e);
}

float get_random_dir()
{
    static std::default_random_engine e(time(0));
    static std::uniform_real_distribution<float> dis(-0.5f, 0.5f); // range [-0.5, 0.5]
    return dis(e);
}
