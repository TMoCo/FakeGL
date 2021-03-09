//////////////////////////////////////////////////////////////////////
//
//  University of Leeds
//  COMP 5812M Foundations of Modelling & Rendering
//  User Interface for Coursework
//
//  September, 2020
//
//  ------------------------
//  FakeGL.h
//  ------------------------
//  
//  A unit for implementing OpenGL workalike calls
//  
///////////////////////////////////////////////////

// include guard
#ifndef FAKEGL_H
#define FAKEGL_H

#include "Cartesian3.h"
#include "Homogeneous4.h"
#include "Matrix4.h"
#include "RGBAImage.h"
#include <vector>
#include <deque>

// we will store all of the FakeGL context in a class object
// this is similar to the real OpenGL which handles multiple windows
// by having separate variables for each stored in separate graphics
// contexts.   
//
// We have simplified the calls, so the code is not *identical* to
// the OpenGL calls, but it's pretty close

// class constants
// constants for Begin()
const unsigned int FAKEGL_POINTS = 0;
const unsigned int FAKEGL_LINES = 1;
const unsigned int FAKEGL_TRIANGLES = 2;
// bitflag constants for Clear()
const unsigned int FAKEGL_COLOR_BUFFER_BIT = 1;
const unsigned int FAKEGL_DEPTH_BUFFER_BIT = 2;
// constants for Enable()/Disable()
const unsigned int FAKEGL_LIGHTING = 1;
const unsigned int FAKEGL_TEXTURE_2D = 2;
const unsigned int FAKEGL_DEPTH_TEST = 3;
const unsigned int FAKEGL_PHONG_SHADING = 4;
// constants for Light() - actually bit flags
const unsigned int FAKEGL_POSITION = 1;
const unsigned int FAKEGL_AMBIENT = 2;
const unsigned int FAKEGL_DIFFUSE = 4;
const unsigned int FAKEGL_AMBIENT_AND_DIFFUSE = 6;
const unsigned int FAKEGL_SPECULAR = 8;
// additional constants for Material()
const unsigned int FAKEGL_EMISSION = 16;
const unsigned int FAKEGL_SHININESS = 32;
// constants for matrix operations
const unsigned int FAKEGL_MODELVIEW = 1;
const unsigned int FAKEGL_PROJECTION = 2;
// constants for texture operations
const unsigned int FAKEGL_MODULATE = 1;
const unsigned int FAKEGL_REPLACE = 2;
// constant for converting degrees to radians
const float PI = 3.1415927410125732421875;

// class with vertex attributes
class vertexWithAttributes
    { // class vertexWithAttributes
    public:
	// Position in OCS
    Homogeneous4 position;

	// Colour
    RGBAValue colour;

    // Normal 
    Homogeneous4 normal;

    // material properties
    float* ambient;
    float* diffuse;
    float* specular;
    float* emissive;
    float exponent;

    // Texture coords
    float u;
    float v;
    }; // class vertexWithAttributes

// class for a vertex after transformation to screen space
class screenVertexWithAttributes
    { // class screenVertexWithAttributes
    public:
	// Position in DCS
    Cartesian3 position;

	// Colour
    RGBAValue colour;

    // Normal 
    Cartesian3 normal;

    // material properties
    float* ambient;
    float* diffuse;
    float* specular;
    float* emissive;
    float exponent;

    // Texture coords
    float u;
    float v;
    }; // class screenVertexWithAttributes

// class for a fragment with attributes
class fragmentWithAttributes
    { // class fragmentWithAttributes
    public:
    // the row & column address in the framebuffer
    int row, col;

    // the RGBA colour of the fragment
    RGBAValue colour;

    // the depth value (z) of the fragment, used for depth test
    float depth;

    }; // class fragmentWithAttributes

// the class storing the FakeGL context
class FakeGL
    { // class FakeGL
    // for the sake of simplicity & clarity, we make everything public
    public:
    //-----------------------------
    // MATRIX STATE
    //-----------------------------

    // state for current matrix
    unsigned int matrixMode;
    // matrix deques behave like stacks, access top of stack with back(), modify stack with push_back() and pop_back()
    // the model view matrix for model and world coordinates. Has its own stack, current matrix is the one at the top of the stack
    std::deque<Matrix4> modelViewStack;

    // The projection matrix. Has its own stack where the current matrix is the one at the top of the stack
    std::deque<Matrix4> projectionStack;
    
    // store the variables for affine transformation from NDC to SC
    float xPixelOrigin;
    float yPixelOrigin;
    
    // the maximum between viewport height and width
    float viewPortSize;

    //-----------------------------
    // ATTRIBUTE STATE
    //-----------------------------

    // attributes that are enabled/disabled, act as booleans that are turned on (1) or off (0)
    // lighting state
    unsigned int lighting;

    // texture state
    unsigned int texture;

    // depth test state
    unsigned int depthTest;

    // phong shading state
    unsigned int phongShading;

    //-----------------------------
    // OUTPUT FROM INPUT STAGE
    // INPUT TO TRANSFORM STAGE
    //-----------------------------

    // we want a queue of vertices with attributes for passing to the rasteriser
    std::deque<vertexWithAttributes> vertexQueue;

    //-----------------------------
    // TRANSFORM/LIGHTING STATE
    //-----------------------------

    // light's position as homogeneous coordinate, w = 0 is directional light
    Homogeneous4 lightPosition;

    // the light ambient component
    float ambientLight[4];

    // the diffuse component 
    float diffuseLight[4];

    // the light specular component
    float specularLight[4];

    // the light specular exponent
    float exponent;
    
    // the colour set by GlColor
    RGBAValue attributeColour;

    // the material ambient component
    float ambientMat[4];

    // the material diffuse component 
    float diffuseMat[4];
    
    // the material specular component 
    float specularMat[4];
    
    // the material emissive component 
    float emissiveMat[4];

    // the current normal
    Cartesian3 attributeNormal;

    //-----------------------------
    // OUTPUT FROM TRANSFORM STAGE
    // INPUT TO RASTER STAGE
    //-----------------------------
    std::deque<screenVertexWithAttributes> rasterQueue;

    //-----------------------------
    // RASTERISE STATE
    //-----------------------------

    // state variable for primitive type
    unsigned int primitive;

    // stores desired point size
    float pointSize;

    // stores desired line width 
    float lineWidth;

    //-----------------------------
    // OUTPUT FROM RASTER STAGE
    // INPUT TO FRAGMENT STAGE
    //-----------------------------
    std::deque<fragmentWithAttributes> fragmentQueue;

    //-----------------------------
    // TEXTURE STATE
    //-----------------------------

    // texture mode
    unsigned int texMode;

    // current texture coordinates
    float attributeU;
    float attributeV;

    // texture
    RGBAImage textureData;

    //-----------------------------
    // FRAMEBUFFER STATE
    // OUTPUT FROM FRAGMENT STAGE
    //-----------------------------

    // state for clear
    unsigned int clear;

    // the clear colour initialised at 0 
    RGBAValue clearColour;

    // depth buffer clear value initialised RGB at 0 and alpha at 1
    RGBAValue depthColour;
    
	// the frame buffer itself
    RGBAImage frameBuffer;

    // current depth range
    float nearVal;
    float farVal;
     
    // rather than define an extra class, we will cheat and use 
    // a second RGBAImage in which the alpha stores the depth buffer
    RGBAImage depthBuffer;
    
    //-------------------------------------------------//
    //                                                 //
    // CONSTRUCTOR / DESTRUCTOR                        //
    //                                                 //
    //-------------------------------------------------//
    
    // constructor
    FakeGL();
    
    // destructor
    ~FakeGL();
    
    //-------------------------------------------------//
    //                                                 //
    // GEOMETRIC PRIMITIVE ROUTINES                    //
    //                                                 //
    //-------------------------------------------------//
    
    // starts a sequence of geometric primitives
    void Begin(unsigned int PrimitiveType);
    
    // ends a sequence of geometric primitives
    void End();

    // sets the size of a point for drawing
    void PointSize(float size);

    // sets the width of a line for drawing purposes
    void LineWidth(float width);
    
    //-------------------------------------------------//
    //                                                 //
    // MATRIX MANIPULATION ROUTINES                    //
    //                                                 //
    //-------------------------------------------------//

    // set the matrix mode (i.e. which one we change)   
    void MatrixMode(unsigned int whichMatrix);

    // pushes a matrix on the stack
    void PushMatrix();

    // pops a matrix off the stack
    void PopMatrix();

    // load the identity matrix
    void LoadIdentity();
    
    // multiply by a known matrix in column-major format
    void MultMatrixf(const float *columnMajorCoordinates);

    // sets a perspective projection matrix
    void Frustum(float left, float right, float bottom, float top, float zNear, float zFar);

    // sets an orthographic projection matrix
    void Ortho(float left, float right, float bottom, float top, float zNear, float zFar);

    // rotate the matrix
    void Rotatef(float angle, float axisX, float axisY, float axisZ);

    // scale the matrix
    void Scalef(float xScale, float yScale, float zScale);
    
    // translate the matrix
    void Translatef(float xTranslate, float yTranslate, float zTranslate);
    
    // sets the viewport
    void Viewport(int x, int y, int width, int height);

    //-------------------------------------------------//
    //                                                 //
    // VERTEX ATTRIBUTE ROUTINES                       //
    //                                                 //
    //-------------------------------------------------//

    // sets colour with floating point
    void Color3f(float red, float green, float blue);
    
    // sets material properties
    void Materialf(unsigned int parameterName, const float parameterValue);
    void Materialfv(unsigned int parameterName, const float *parameterValues);

    // sets the normal vector
    void Normal3f(float x, float y, float z);

    // sets the texture coordinates
    void TexCoord2f(float u, float v);

    // sets the vertex & launches it down the pipeline
    void Vertex3f(float x, float y, float z);

    //-------------------------------------------------//
    //                                                 //
    // STATE VARIABLE ROUTINES                         //
    //                                                 //
    //-------------------------------------------------//

    // disables a specific flag in the library
    void Disable(unsigned int property);
    
    // enables a specific flag in the library
    void Enable(unsigned int property);
    
    //-------------------------------------------------//
    //                                                 //
    // LIGHTING STATE ROUTINES                         //
    //                                                 //
    //-------------------------------------------------//

    // sets properties for the one and only light
    void Light(int parameterName, const float *parameterValues);

    //-------------------------------------------------//
    //                                                 //
    // TEXTURE PROCESSING ROUTINES                     //
    //                                                 //
    // Note that we only allow one texture             //
    // so glGenTexture & glBindTexture aren't needed   //
    //                                                 //
    //-------------------------------------------------//

    // sets whether textures replace or modulate
    void TexEnvMode(unsigned int textureMode);

    // sets the texture image that corresponds to a given ID
    void TexImage2D(const RGBAImage &textureImage);

    //-------------------------------------------------//
    //                                                 //
    // FRAME BUFFER ROUTINES                           //
    //                                                 //
    //-------------------------------------------------//

    // clears the frame buffer
    void Clear(unsigned int mask);
    
    // sets the clear colour for the frame buffer
    void ClearColor(float red, float green, float blue, float alpha);
    
    //-------------------------------------------------//
    //                                                 //
    // ROUTINE TO FLUSH THE PIPELINE                   //
    //                                                 //
    //-------------------------------------------------//
    
    // flushes the pipeline
    void Flush();

    //-------------------------------------------------//
    //                                                 //
    // MAJOR PROCESSING ROUTINES                       //
    //                                                 //
    //-------------------------------------------------//

    // transform one vertex & shift to the transformed queue
    void TransformVertex();

    // rasterise a single primitive if there are enough vertices on the queue
    bool RasterisePrimitive();

    // rasterises a single point
    void RasterisePoint(screenVertexWithAttributes &vertex0);

    // rasterises a single line segment
    void RasteriseLineSegment(screenVertexWithAttributes &vertex0, screenVertexWithAttributes &vertex1);
    
    // rasterises a single triangle
    void RasteriseTriangle(screenVertexWithAttributes &vertex0, screenVertexWithAttributes &vertex1, screenVertexWithAttributes &vertex2);
    
    // process a single fragment
    void ProcessFragment();
    
    }; // class FakeGL

// standard routine for dumping the entire FakeGL context (except for texture / image)
std::ostream &operator << (std::ostream &outStream, FakeGL &fakeGL); 

// subroutines for other classes
std::ostream &operator << (std::ostream &outStream, vertexWithAttributes &vertex); 
std::ostream &operator << (std::ostream &outStream, screenVertexWithAttributes &vertex); 
std::ostream &operator << (std::ostream &outStream, fragmentWithAttributes &fragment); 

// include guard        
#endif