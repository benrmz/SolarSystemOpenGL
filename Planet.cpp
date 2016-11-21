//
//  Planet.cpp
//  FINAL_PROJECT
//
//  Created by Ramirez, Benjamin on 11/21/16.
//  Copyright © 2016 Ramirez, Benjamin. All rights reserved.
//

#include "Planet.hpp"

void Planet::Initialize(char* texture_image, float sun_distance, float size, float orbit_speed){

    this->texture = texture_image;
    this->distance = sun_distance;
    this->diameter = size;
    this->orbitSpeed = orbit_speed;

}

void Planet::setTexture(char* texture_image){
    this->texture = texture_image;
}

void Planet::setOrbitSpeed(float newSpeed){
    this->orbitSpeed = newSpeed;
}

void Planet::setDistance(float newDistance){
    this->distance = newDistance;
}

void Planet::setDiameter(float newDiameter){
    this->diameter = newDiameter;
}
