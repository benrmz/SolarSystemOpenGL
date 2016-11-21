//
//  Shape.cpp
//  final_v0
//
//  Created by Benji Ramirez on 11/5/16.
//  Copyright © 2016 Benji Ramirez. All rights reserved.
//

#include "Shape.hpp"
#include "Angel.h"
#include "mat.h"
#include "vec.h"
#include <GLUT/glut.h>


void Shape::Initialize(mat4 rotate, vec3 translate, vec3 scaling, int numVertices,size_t offset, size_t normals_offset, size_t tex_coords_offset){
    
    this->rotation = rotate;
    this->translation = translate;
    this->scale = scaling;
    this->shape_offset = offset;
    this->normal_offset = normals_offset;
    this->tex_offset = tex_coords_offset;
    setTransformRTS();
    this->transformation = getTransform();
    this->numVertices = numVertices;
}

void Shape::setTransformRTS(){
    
    this->transformation = this->rotation * Translate(this->translation) * Scale(this->scale);
}

void Shape::setTransformTRS(){
    this->transformation = Translate(this->translation) * this->rotation * Scale(this->scale);
}

void Shape::setTransform(mat4 rotate, vec3 translate, vec3 scale){
    this->rotation = rotate;
    this->translation = translate;
    this->scale = scale;
    setTransformRTS();
}

void Shape::setScale(vec3 scaling){
    scale = scaling;
    this->setTransformRTS();
}

void Shape::setRotation(mat4 rotate){
    this->rotation = rotate;
    this->setTransformRTS();
}

void Shape::setTranslation(vec3 translate){
    this->translation = translate;
    this->setTransformRTS();
}

mat4 Shape::getTransform(){
    return this->transformation;
}

mat4 Shape::getRotation(){
    return this->rotation;
}

vec3 Shape::getTranslation(){
    return this->translation;
}

vec3 Shape::getScale(){
    return this->scale;
}

size_t Shape::getOffset(){
    return this->shape_offset;
}

size_t Shape::getNormalOffset(){
    return this->normal_offset;
}

size_t Shape::getTextureOffset(){
    return this->tex_offset;
}

int Shape::getNumVertices(){
    return this->numVertices;
}
