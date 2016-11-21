//
//  Shape.hpp
//  final_v0
//
//  Created by Benji Ramirez on 11/5/16.
//  Copyright © 2016 Benji Ramirez. All rights reserved.
//

#ifndef Shape_hpp
#define Shape_hpp

#include <stdio.h>
#include "mat.h"
#include "Angel.h"
#include "vec.h"

class Shape{

public:
    void Initialize(mat4 , vec3 , vec3 , int , size_t , size_t , size_t );
    //Overloading the setTransform Method for when we ant to set all 3 at once
    void setTransformRTS();
    void setTransformTRS();
    void setTransform(mat4, vec3, vec3);
    mat4 getRotation();
    vec3 getTranslation();
    vec3 getScale();
    mat4 getTransform();
    void setScale(vec3);
    void setTranslation(vec3);
    void setRotation(mat4);
    size_t getOffset();
    size_t getNormalOffset();
    size_t getTextureOffset();
    int getNumVertices();
    
private:
    mat4 rotation;
    vec3 translation;
    vec3 scale;
    mat4 transformation;
    size_t shape_offset;
    size_t normal_offset;
    size_t tex_offset;
    int numVertices;
    
    
};


#endif /* Shape_hpp */
