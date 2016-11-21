//
//  Planet_data.hpp
//  FINAL_PROJECT
//
//  Created by Ramirez, Benjamin on 11/21/16.
//  Copyright © 2016 Ramirez, Benjamin. All rights reserved.
//

#ifndef Planet_data_h
#define Planet_data_h


char*  TEX_IMAGES[] = {
    "images/sunmap.bmp",
    "images/mercurymap.bmp",
    "images/venusmap.bmp",
    "images/earthmap.bmp",
    "images/marsmap.bmp",
    "images/jupitermap.bmp",
    "images/saturnmap.bmp",
    "images/uranusmap.bmp",
    "images/neptunemap.bmp",
    "images/plutomap.bmp",
    "images/universemap.bmp"
};

float DISTANCE_AU[] ={
    0,
    0.39,   //mercury
    0.723,  //venus
    1,      //earth
    1.524,  //mars
    5.203,  //jupiter
    9.539,  //saturn
    19.18,  //Uranus
    30.06,  //Neptune
    39.53   //Pluto
};

float SUN_DIAMETER = 864; //the diameter by which we scale the others
float GLOBAL_SCALE = 6.0;
float DIAMETERS[] ={
    SUN_DIAMETER,    //sun
    3.0,    //mercury
    7.5,    //venus
    7.9,    //earth
    4.2,    //mars
    88.7,   //jupiter
    74.60,  //saturn
    32.6,   //Uranus
    30.2,   //neptune
    1.4     //pluto
    
};

float ORBIT_SPEED[]={//relative to earth year
    0,  //sun
    0.24,    //mercury
    0.616,   //venus
    1,      //earth
    1.88,   //mars
    11.862, //jupiter
    29.456, //saturn
    84.01,  //uranus
    164.81, //neptune
    247.7   //pluto
};


#endif /* Planet_data_h */
