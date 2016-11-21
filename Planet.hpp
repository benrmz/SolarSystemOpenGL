//
//  Planet.hpp
//  FINAL_PROJECT
//
//  Created by Ramirez, Benjamin on 11/21/16.
//  Copyright © 2016 Ramirez, Benjamin. All rights reserved.
//

#ifndef Planet_hpp
#define Planet_hpp

#include <stdio.h>
#include "mat.h"
#include "Angel.h"
#include "vec.h"

class Planet{

    void Initialize(char*, float, float, float);
    void setTexture(char*);
    void setOrbitSpeed(float);
    void setDistance(float);
    void setDiameter(float);
    
    char* getTexture();
    float getOrbitSpeed();
    float getDistance();
    float getDiameter();
    
    char* texture;
    float distance;
    float diameter;
    float orbitSpeed;

};


#endif /* Planet_hpp */

