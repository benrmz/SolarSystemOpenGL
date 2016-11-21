#if 1
// spherical camera + persp + lighting + texture
//
// Display a color cube
//
// Colors are assigned to each vertex and then the rasterizer interpolates
//   those colors across the triangles.  We us an orthographic projection
//   as the default projetion.

#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <vector>
#include <cstring>
using namespace std;

#include <GLUT/glut.h>

#include "Angel.h"
#include "simplemodels.h"
#include "Shape.hpp"
#include "Planet.hpp"
#include "Planet_data.hpp"

GLuint loadBMP_custom(unsigned char** data, int* w, int* h, const char * imagepath);

// Array of rotation angles (in degrees) for each coordinate axis
enum { Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3 };
int      Axis = Zaxis;
GLfloat  Theta1[NumAxes] = { 80.0, 0.0, 0.0 };
GLfloat  Theta2[NumAxes] = { 0.0, 30.0, 0.0 };
GLfloat  Theta3[NumAxes] = { 0.0, 0.0, 0.0 };
GLfloat  Theta4[NumAxes] = { 0.0, 0.0, 0.0 };

//---- for use when doing operations on the planets

const int NUM_TEXTURES = 11;

enum WORLDS{
    SUN     = 0,
    MERCURY = 1,
    VENUS   = 2,
    EARTH   = 3,
    MARS    = 4,
    JUPITER = 5,
    SATURN  = 6,
    URANUS  = 7,
    NEPTUNE = 8,
    PLUTO   = 9,
    UNIVERSE= 10
};



int WIDTHS[NUM_TEXTURES];
int HEIGHTS[NUM_TEXTURES];
unsigned char* PICS[NUM_TEXTURES];

size_t CUBE_OFFSET;
size_t COLOR_OFFSET;
size_t CUBE_NORMALS_OFFSET;
size_t CUBE_TEXCOORDS_OFFSET;
size_t CYLINDER_OFFSET;
size_t CYLINDER_NORMALS_OFFSET;
size_t CYLINDER_TEXCOORDS_OFFSET;
size_t SPHERE_OFFSET;
size_t SPHERE_NORMALS_OFFSET;
size_t SPHERE_TEXCOORDS_OFFSET;


//----------------------------------------------------------------------------

GLuint program;

GLuint vPosition;
GLuint vNormal;
GLuint vColor;
GLuint vTexCoord;

GLuint textures[NUM_TEXTURES];

//-----------------SHAPE OBJECTS ------------------------------
//  to be created in the init function
Shape Cube;
Shape Cylinder;
Shape Sphere;

//---------------------- FRACTAL BOUNDARIES -------------------------

float BRANCHES = 5.0;
float SMALLEST_SCALE = 0.2;
float LARGEST_SCALE = 4.0;

//--------------------------------------------------------------

void
init()
{
    // Load shaders and use the resulting shader program
    program = InitShader( "vshader21.glsl", "fshader21.glsl" );
    glUseProgram( program );
    
    //---------------------------------------------------------------------
    colorcube();
    colortube();
    colorbube();
    //---------------------------------------------------------------------

    //---- Initialize texture objects
    glGenTextures( NUM_TEXTURES, textures );
    
    glActiveTexture( GL_TEXTURE0 );
    
    
    for (int i = 0; i < NUM_TEXTURES; i++){
        
        loadBMP_custom(&PICS[i], &WIDTHS[i], &HEIGHTS[i], TEX_IMAGES[i]);
        glBindTexture( GL_TEXTURE_2D, textures[i] );
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, WIDTHS[i], HEIGHTS[i], 0, GL_BGR, GL_UNSIGNED_BYTE, PICS[i] );
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );

    }
    
    //----set offset variables
    
    CUBE_OFFSET = 0;
    COLOR_OFFSET = sizeof(points_cube);
    CUBE_NORMALS_OFFSET = COLOR_OFFSET + sizeof(colors);
    CUBE_TEXCOORDS_OFFSET = CUBE_NORMALS_OFFSET + sizeof(normals_cube);
    CYLINDER_OFFSET = CUBE_TEXCOORDS_OFFSET + sizeof(tex_coords_cube);
    CYLINDER_NORMALS_OFFSET = CYLINDER_OFFSET + sizeof(points_cylinder);//changed
    CYLINDER_TEXCOORDS_OFFSET = CYLINDER_NORMALS_OFFSET + sizeof(normals_cylinder); //added
    SPHERE_OFFSET = CYLINDER_TEXCOORDS_OFFSET + sizeof(tex_coords_cylinder); //changed
    SPHERE_NORMALS_OFFSET = SPHERE_OFFSET + sizeof(points_sphere);
    SPHERE_TEXCOORDS_OFFSET = SPHERE_NORMALS_OFFSET + sizeof(normals_sphere);

    // Create a vertex array object
    GLuint vao;
    glGenVertexArrays( 1, &vao );  // removed 'APPLE' suffix for 3.2
    glBindVertexArray( vao );
    
    // Create and initialize a buffer object
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(points_cube) + sizeof(normals_cube) + sizeof(tex_coords_cube) + sizeof(colors) + sizeof(points_cylinder) + sizeof(normals_cylinder) + sizeof(tex_coords_cylinder) + sizeof(points_sphere) + sizeof(normals_sphere) + sizeof(tex_coords_sphere), NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, CUBE_OFFSET, sizeof(points_cube), points_cube );
    glBufferSubData( GL_ARRAY_BUFFER, COLOR_OFFSET, sizeof(colors), colors );
    glBufferSubData( GL_ARRAY_BUFFER, CUBE_NORMALS_OFFSET, sizeof(normals_cube), normals_cube );
    glBufferSubData( GL_ARRAY_BUFFER, CUBE_TEXCOORDS_OFFSET, sizeof(tex_coords_cube), tex_coords_cube );
    glBufferSubData( GL_ARRAY_BUFFER, CYLINDER_OFFSET, sizeof(points_cylinder), points_cylinder );
    glBufferSubData( GL_ARRAY_BUFFER, CYLINDER_NORMALS_OFFSET, sizeof(normals_cylinder), normals_cylinder );
    glBufferSubData( GL_ARRAY_BUFFER, CYLINDER_TEXCOORDS_OFFSET, sizeof(tex_coords_cylinder), tex_coords_cylinder );
    glBufferSubData( GL_ARRAY_BUFFER, SPHERE_OFFSET, sizeof(points_sphere), points_sphere );
    glBufferSubData( GL_ARRAY_BUFFER, SPHERE_NORMALS_OFFSET, sizeof(normals_sphere), normals_sphere );
    glBufferSubData( GL_ARRAY_BUFFER, SPHERE_TEXCOORDS_OFFSET, sizeof(tex_coords_sphere), tex_coords_sphere );
    
    
    //---------------------------------------------------------------------
    
    // set up vertex arrays
    vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_OFFSET) );
    
    vNormal = glGetAttribLocation( program, "vNormal" );
    glEnableVertexAttribArray( vNormal );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_NORMALS_OFFSET) );
    
    vColor = glGetAttribLocation( program, "vColor" );
    glEnableVertexAttribArray( vColor );
    glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(COLOR_OFFSET) );
    
    vTexCoord = glGetAttribLocation( program, "vTexCoord" );
    glEnableVertexAttribArray( vTexCoord );
    glVertexAttribPointer( vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_TEXCOORDS_OFFSET) );
    
    // Set the value of the fragment shader texture sampler variable (myTextureSampler) to GL_TEXTURE0
    glUniform1i( glGetUniformLocation(program, "myTextureSampler"), 0 );

    //---------------------------------------------------------------------
    
    glEnable( GL_DEPTH_TEST );
    glClearColor( 0.0, 0.0, 0.0, 1.0 );
    
    //------------ Initializing the shape objects -------------------
    
    Cube.Initialize(mat4(), vec3(), vec3(), NumVerticesCube, CUBE_OFFSET, CUBE_NORMALS_OFFSET, CUBE_TEXCOORDS_OFFSET);
    Cylinder.Initialize(mat4(), vec3(), vec3(), NumVerticesCylinder, CYLINDER_OFFSET, CYLINDER_NORMALS_OFFSET, CYLINDER_TEXCOORDS_OFFSET);
    Sphere.Initialize(mat4(), vec3(), vec3(), NumVerticesSphere, SPHERE_OFFSET, SPHERE_NORMALS_OFFSET, SPHERE_TEXCOORDS_OFFSET);

    
}

//-------------------- Initializing Textures -----------------------------------

void initTextures(int NumTextures){
    
    for(int i = 0; i < NumTextures; i++){
    }

}


//----------------------------------------------------------------------------

void
SetMaterial( vec4 ka, vec4 kd, vec4 ks, float s )
{
    glUniform4fv( glGetUniformLocation(program, "ka"), 1, ka );
    glUniform4fv( glGetUniformLocation(program, "kd"), 1, kd );
    glUniform4fv( glGetUniformLocation(program, "ks"), 1, ks );
    glUniform1f( glGetUniformLocation(program, "Shininess"), s );
}

//----------------------------------------------------------------------------

void
SetLight( vec4 lpos, vec4 la, vec4 ld, vec4 ls )
{
    glUniform4fv( glGetUniformLocation(program, "LightPosition"), 1, lpos);
    glUniform4fv( glGetUniformLocation(program, "AmbientLight"), 1, la);
    glUniform4fv( glGetUniformLocation(program, "DiffuseLight"), 1, ld);
    glUniform4fv( glGetUniformLocation(program, "SpecularLight"), 1, ls);
}

//----------------------------------------------------------------------------
 //  MY own functions for displaying the objects

void displayShape(Shape shape){

    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, shape.getTransform() );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(shape.getOffset()) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(shape.getNormalOffset()) );
    glVertexAttribPointer( vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(shape.getTextureOffset()) );
    glDrawArrays( GL_TRIANGLES, 0, shape.getNumVertices() );

}

//  Creating a rough fractal generation functions as main module;


//  General fractal generation in the XZ plane
void generateLateral(Shape shape,GLfloat radius, mat4 rotate, vec3 translate, vec3 scale){
    //creating the sphere by inputs passed
    
    shape.setTransform(rotate, translate, scale);
    displayShape(shape);
    if (scale.x > SMALLEST_SCALE && scale.x < LARGEST_SCALE){
        for (int i = 0.0; i < BRANCHES; i++){
            GLfloat theta = i/BRANCHES * 2*M_PI; // The angle of the child shape in relation to the parent
            //printf("%f\n%f", theta,translate.x + (radius*cos(theta)));
            //creating the child shape
            GLfloat newX = translate.x + radius*cos(theta);
            GLfloat newZ = translate.z + radius*sin(theta);
            //Recursively calling the generate function to create fractal
            generateLateral(shape,radius/2.5, rotate, vec3( newX, translate.y, newZ), scale/1.9);
            
        }
    }
}

//================= Creating a fractal generation in  Y-axis only

void generateVertical(Shape shape,GLfloat radius, mat4 rotate, vec3 translate, vec3 scale){
    
    //Creating the cylindrical structure
    shape.setTransform(rotate, translate, scale);
    displayShape(shape);
    generateLateral(shape, radius, rotate, translate, scale);
    if (scale.x > SMALLEST_SCALE && scale.x < LARGEST_SCALE){
        GLfloat distance = translate.y + radius;
        //generate the tower above
        GLfloat newY = distance;
        generateVertical(shape, radius/2, rotate, vec3(translate.x, newY, translate.z), scale/2);
        //Generate tower below
        generateVertical(shape, radius/2, rotate, vec3(translate.x, -newY, translate.z), scale/2);
    }
}

// Generation function for the space colony--------------

void generateColony(Shape baseShape, Shape towerShape, GLfloat baseRadius, GLfloat towerRadius, mat4 rotate, vec3 translate, vec3 scale){
    baseShape.setTransform(rotate, translate, scale);
    displayShape(baseShape);
    towerShape.setTransform(rotate, translate, scale);
    displayShape(towerShape);
    GLfloat theta, i, newX, newZ;
    if (scale.x >SMALLEST_SCALE && scale.x < LARGEST_SCALE){
        generateVertical(towerShape, towerRadius, rotate, translate, scale);
        for (i = 0.0; i < BRANCHES; i++){
            theta = i/BRANCHES * 2*M_PI;
            newX = translate.x + baseRadius*cos(theta);
            newZ = translate.z + baseRadius*sin(theta);
            generateColony(baseShape, towerShape, baseRadius, towerRadius/2, rotate/2, vec3(newX, translate.y, newZ), scale/2);
            
        }
    }
}



//--------------------------------------------------------------------------------



mat4 proj_matrix;

mat4 view_matrix;

float r = 5.0;

void
display( void )
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    //---- lights
    
    SetLight( vec4( 2.0, 2.0, 2.0, 1.0 ), vec4(0.4, 0.4, 0.4, 1.0), vec4(0.7, 0.7, 0.7, 1.0), vec4(0.7, 0.7, 0.7, 1.0) );
    
    //---- camera intrinsic parameters
    
    float left = -1.0;
    float right = 1.0;
    float bottom = -1.0;
    float top = 1.0;
    float zNear = 0.5;
    float zFar = 500.0;
    
    //proj_matrix = Scale(2.0/(right-left), 2.0/(top-bottom), 2.0/(zNear-zFar));
    
    proj_matrix = Frustum( left, right, bottom, top, zNear, zFar );
    
    glUniformMatrix4fv( glGetUniformLocation( program, "projection" ), 1, GL_TRUE, proj_matrix );
    
    //---- camera extrinsic parameters
    
    float tr_y = Theta3[Yaxis]* M_PI/180.0;
    float tr_z = Theta3[Zaxis]* M_PI/180.0;
    float eye_z = r * (cos(tr_z)) * cos(tr_y);
    float eye_x = r * (cos(tr_z)) * sin(tr_y);
    float eye_y = r * sin(tr_z);
    
    vec4 up = vec4(0.0, cos(tr_z), 0.0, 0.0);
    cout << up << ' ' << normalize(up) << endl;
    
    view_matrix = LookAt( vec4(eye_x, eye_y, eye_z, 1.0), vec4(0.0, 0.0, 0.0, 1.0), vec4(0.0, cos(tr_z), 0.0, 0.0));
    
    glUniformMatrix4fv( glGetUniformLocation( program, "view" ), 1, GL_TRUE, view_matrix );
    
    //---- action
    
    SetLight( vec4( 0.0, 0.0, 0.0, 1.0 ), vec4(0.7, 0.7, 0.7, 1.0), vec4(0.9, 0.9, 0.9, 1.0), vec4(0.5, 0.5, 0.5, 1.0) );
    
    //---- spherical sun
    glBindTexture( GL_TEXTURE_2D, textures[2] );
    glUniform1i( glGetUniformLocation(program, "texture_on"), true );
    
    SetMaterial( vec4(0.9, 0.9, 0.5, 1.0), vec4(0.9, 0.9, 0.5, 1.0), vec4(0.9, 0.9, 0.0, 1.0), 0.5);
    glUniform1i( glGetUniformLocation(program, "light_out"), true );
    
    // Some fractals:
    // colony generator:
    //generateColony(Sphere, Sphere, 4.0, 1.3, RotateY(Theta4[Xaxis]), vec3(0.0,0.0,0.0), vec3(1.0,1.0,1.0));

    //The Universe background
    glBindTexture( GL_TEXTURE_2D, textures[UNIVERSE] );
    glUniform1i( glGetUniformLocation(program, "texture_on"), true );
    Sphere.setTransform(RotateY(0), vec3(0.0,0.0,1.0), vec3(10.0,10.0,10.0)*GLOBAL_SCALE);
    //displayShape(Sphere);
    
    //the Sun:
    
    //color the sun
    glBindTexture( GL_TEXTURE_2D, textures[SUN] );
    glUniform1i( glGetUniformLocation(program, "texture_on"), true );
    Sphere.setTransform(RotateY(Theta4[Xaxis]), vec3(0.0,0.0,0.0), vec3(1.0,1.0,1.0));

    displayShape(Sphere);
    
    for (int i =0; i< NUM_TEXTURES-1; i++){ //ONLY LOOPING THROUGH THE PLANETS
        glBindTexture(GL_TEXTURE_2D, textures[i+1]);
        Sphere.setTransform(RotateY(Theta4[Xaxis]), vec3(0.8 + DISTANCE_AU[i],0,0)*GLOBAL_SCALE, vec3(DIAMETERS[i+1]/(SUN_DIAMETER/4)) *GLOBAL_SCALE );
        displayShape(Sphere);

    }

    glutSwapBuffers();
}



//----------------------------------------------------------------------------

void
keyboard( unsigned char key, int x, int y )
{
    switch( key ) {
        case 033:    // Escape key
        case 'q': case 'Q':
            exit( EXIT_SUCCESS );
            break;
            
        case 'y':    //---- theta
            Axis = Yaxis;
            Theta3[Axis] += 5.0;
            Theta3[Axis] = fmod(Theta3[Axis], 360.0);
            glutPostRedisplay();
            break;
        case 'z':    //---- phi
            Axis = Zaxis;
            Theta3[Axis] += 5.0;
            Theta3[Axis] = fmod(Theta3[Axis], 360.0);
            glutPostRedisplay();
            break;
            
        case 'r':    //---- increase radius
            r += 0.5;
            glutPostRedisplay();
            break;
        case 'R':    //---- decrease radius
            r -= 0.5;
            glutPostRedisplay();
            break;
            
        case 'u':
            //view_matrix = default_view_matrix;
            Theta3[Xaxis] = 0.0;
            Theta3[Yaxis] = 0.0;
            Theta3[Zaxis] = 0.0;
            glutPostRedisplay();
            break;
    }
}




//----------------------------------------------------------------------------

void
mouse( int button, int state, int x, int y )
{
    if ( state == GLUT_DOWN ) {
        switch( button ) {
            case GLUT_LEFT_BUTTON:    Axis = Xaxis;  break;
            case GLUT_MIDDLE_BUTTON:  Axis = Yaxis;  break;
            case GLUT_RIGHT_BUTTON:   Axis = Zaxis;  break;
        }
    }
}

//----------------------------------------------------------------------------

void
idle( void )
{
    //Theta1[Axis] = fmod(Theta1[Axis]+1, 360.0);
    //Theta2[Axis] = fmod(Theta2[Axis]+2, 360.0);
    
    //Theta4[Xaxis] = fmod(Theta4[Xaxis]+0.5, 360.0);
    
    
    glutPostRedisplay();
}

//----------------------------------------------------------------------------

void
reshape( int w, int h )
{
    glViewport(0,0,w,h);
}

//----------------------------------------------------------------------------

int
main( int argc, char **argv )
{
    glutInit( &argc, argv );
    glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( 1000, 1000 );
    glutCreateWindow( "Scene" );
    
    init();
    
    glutDisplayFunc( display );
    glutReshapeFunc( reshape );
    glutKeyboardFunc( keyboard );
    glutMouseFunc( mouse );
    glutIdleFunc( idle );
    
    glutMainLoop();
    return 0;
}

#endif
















