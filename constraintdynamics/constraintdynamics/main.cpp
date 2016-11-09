//
//  main.cpp
//  constraintdynamics
//
//  Created by Raymond Kim on 11/1/16.
//  Copyright © 2016 raymnd. All rights reserved.
//

#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/ext.hpp"

// AntTweakBar
#include <AntTweakBar.h>


#include "Particle.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

void computeBeadPosition();
void drawBead( GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLint numberOfSides );
void drawRing( GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLint numberOfSides );
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
static void cursorPositionCallback(GLFWwindow* window, double xPos, double yPos);
void cursorEnterCallback(GLFWwindow* window, int entered);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mode);
void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);

Particle bead = Particle();
float initial = 0.0;

// window state booleans
bool PAUSE = true;
bool DRAG = false;
bool GRAV = false;
bool MASS = false;
bool VEL = false;
bool FRIC = false;
double xCur = 0, yCur = 0;
double xprev = 0, yprev = 0;

double dt = .01;
GLfloat beadscale = 120;
glm::vec2 offset = glm::vec2(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
float gravity = -9.8;
float mass = 1;
float friction = 1.0f;

int main( void )
{
    GLFWwindow *window;
    
    if ( !glfwInit( ) )
    {
        return -1;
    }
    
    window = glfwCreateWindow( SCREEN_WIDTH, SCREEN_HEIGHT, "A Bead on a Wire", NULL, NULL );
    
    // AntTweakBar Init
//    TwInit(TW_OPENGL, NULL);
//    TwWindowSize(150, 200);
//    TwBar *myBar;
//    myBar = TwNewBar("NameOfMyTweakBar");
    
    //keyboard callbacks
    glfwSetKeyCallback(window, keyCallback);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);
    glfwSetCursorPosCallback(window, cursorPositionCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    
    //mouse callbacks
    glfwSetCursorEnterCallback(window, cursorEnterCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, 1);
    
    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
    
    if ( !window )
    {
        glfwTerminate( );
        return -1;
    }
    
    // Make the window's context current
    glfwMakeContextCurrent( window );
    
    glViewport( 0.0f, 0.0f, screenWidth, screenHeight );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );
    glOrtho( 0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, 0, 1 );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( );
    
    double past = 0.0;
    bead.setPosition(glm::vec2(1, 0));
    bead.setPosition(M_PI/6);
    
    // Loop until the user closes the window
    while ( !glfwWindowShouldClose( window ) )
    {
        
        double current = glfwGetTime();
        double difference = current - past;
        
        //glfwGetCursorPos(window, &xCur, &yCur);
        
        if (difference > dt) {
            glClearColor(0.96f, 0.94f, 0.87f, 1.0f);
            glClear( GL_COLOR_BUFFER_BIT );
            
            
            
            
            // render OpenGL here
            drawRing( offset.x, offset.y, 0, beadscale, 36 );
            
            glm::vec2 beadposition = bead.position();
            beadposition = beadposition * beadscale;
            beadposition = beadposition + offset;
        
            drawBead( beadposition.x, beadposition.y, 0, bead.size(), 36 );
            
            if (!PAUSE) {
                computeBeadPosition();
            } else if (PAUSE) {
                if (DRAG) {
                    float newtheta;
                    if ((xCur - SCREEN_WIDTH/2) < 0) {
                        newtheta = M_PI + atan( ( -yCur + SCREEN_HEIGHT/2)/(xCur - SCREEN_WIDTH/2));
                    } else
                        newtheta = atan( ( -yCur + SCREEN_HEIGHT/2)/(xCur - SCREEN_WIDTH/2));
//                    std::cout << yCur + SCREEN_HEIGHT/2<<" : "<< xCur - SCREEN_WIDTH/2<< std::endl;
                    bead.setPosition(newtheta);
                    bead.setVelocity(initial);
                }
                if (VEL) {
                    float length = glm::length(glm::vec2(xprev, yprev) - glm::vec2(xCur, yCur));
                    drawRing(beadposition.x, beadposition.y, 0, length, 36);
                }
            }
            
            //drawRing(xCur, SCREEN_HEIGHT - yCur, 0, 20, 35);
            
//            TwDraw();
            
            // Swap front and back buffers
            glfwSwapBuffers( window );
            
            // Poll for and process events
            glfwPollEvents( );
        
            past = current;

        }
    }
    
    glfwTerminate( );
    
    return 0;
}

void computeBeadPosition() {
    
    glm::vec2 grav = glm::vec2(0, gravity);
    glm::vec2 forces = grav * friction;
    
    glm::vec2 tangent = glm::vec2(-bead.position().y, bead.position().x);
    
    float thetadot = (glm::dot(tangent, forces)/glm::dot(tangent, tangent))/bead.mass();
    //thetadot *= 0.7;
    
    float angular = bead.velocity();
    angular += thetadot * dt;
    
    bead.setPosition(bead.theta() + dt * angular);
    bead.setVelocity(bead.velocity() + dt * thetadot);
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mode) {
    
    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    
    if (key == GLFW_KEY_SPACE) {
        switch (action) {
            case GLFW_PRESS:{
                PAUSE = !PAUSE;
                 break;
            }
            default:
                break;
        }
        
    }
    if (key == GLFW_KEY_F) {
        switch (action) {
            case GLFW_REPEAT:
                FRIC = true;
                break;
                
            case GLFW_RELEASE:
                FRIC = false;
                std::cout << "coeffienct of damping is: " << friction << std::endl;
                
            default:
                FRIC = false;
                break;
        }
    }
    if (key == GLFW_KEY_G) {
        switch (action) {
            case GLFW_REPEAT:
                GRAV = true;
                break;
                
            case GLFW_RELEASE:
                GRAV = false;
                std::cout << "gravity is: " << gravity << std::endl;
                
            default:
                GRAV = false;
                break;
        }
    }
    
    if (key == GLFW_KEY_M) {
        switch (action) {
            case GLFW_REPEAT:
                MASS = true;
                break;
            
            case GLFW_RELEASE:
                MASS = false;
                std::cout << "mass is: " << bead.mass() << std::endl;

                
            default:
                MASS = false;
                break;
        }
        
    }
    
    if (key == GLFW_KEY_O && action == GLFW_PRESS) {
        std::cout << "******** STATE **********\n  gravity: " << gravity << std::endl;
        std::cout << "  Bead Position: " << glm::to_string(bead.position()) << std::endl;
        std::cout << "  Bead Angular Velocity: " << bead.velocity() << std::endl;
        std::cout << "  Bead Mass " << bead.mass() << std::endl;
    }
    
}

static void cursorPositionCallback(GLFWwindow* window, double xPos, double yPos) {
    //std::cout<< "(" << xPos << "," << yPos << ")" << std::endl;
    xCur = xPos;
    yCur = yPos;
}

void cursorEnterCallback(GLFWwindow* window, int entered){
//    if (entered) {
//        std::cout << "in window" << std::endl;
//    } else
//        std::cout << "out window" << std::endl;

}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mode){
    
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        switch (action) {
            case GLFW_PRESS:{
                if (PAUSE) {
                    if (glm::length((bead.position()*beadscale + offset)-(glm::vec2(xCur, SCREEN_HEIGHT - yCur))) < bead.size()) {
                        DRAG = true;
                    }
                }
                
                
                break;
            }
            case GLFW_RELEASE: {
                if (DRAG) {
                    DRAG = false;
                }
                break;
            }
            default:
                break;
        }
        
    }
    
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        switch (action) {
            case GLFW_PRESS:{
//                std::cout << "right button clicked" << std::endl;
                if (PAUSE) {
                    if (glm::length((bead.position()*beadscale + offset)-(glm::vec2(xCur, SCREEN_HEIGHT - yCur))) < bead.size()) {
                        VEL = true;
                        xprev = xCur;
                        yprev = yCur;
                    }
                    
                }
                break;
            }
                
            case GLFW_RELEASE: {
//                std::cout << "right button released" << std::endl;
                VEL = false;
                float length = glm::length(glm::vec2(xprev, yprev) - glm::vec2(xCur, yCur))/30;
                
                if (bead.position().x > 0) {
                    if (yCur - yprev > 0) {
                        length *= -1;
                    }
                }else {
                    if (yCur - yprev < 0) {
                        length *= -1;
                    }
                }
                bead.setVelocity(length);
                std::cout << bead.velocity() << std::endl;
                break;
            }
            default:
                break;
        }
        
    }
}

void scrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
    //std::cout << xOffset << " : " << yOffset << std::endl;
    if (GRAV) {
        gravity -= yOffset/2;
        std::cout << "gravity: " << gravity << std::endl;
        
    }
    if (MASS) {
        mass -= yOffset/2;
        bead.setMass(mass);
        std::cout << "bead mass: " <<bead.mass() << std::endl;
    }
    if (FRIC) {
        friction -= yOffset/10;
        std::cout << "friction: " <<friction << std::endl;
    }
}


void drawBead( GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLint numberOfSides )
{
    int numberOfVertices = numberOfSides + 2;
    
    GLfloat twicePi = 2.0f * M_PI;
    
    GLfloat circleVerticesX[numberOfVertices];
    GLfloat circleVerticesY[numberOfVertices];
    GLfloat circleVerticesZ[numberOfVertices];
    
    circleVerticesX[0] = x;
    circleVerticesY[0] = y;
    circleVerticesZ[0] = z;
    
    for ( int i = 1; i < numberOfVertices; i++ )
    {
        circleVerticesX[i] = x + ( radius * cos( i *  twicePi / numberOfSides ) );
        circleVerticesY[i] = y + ( radius * sin( i * twicePi / numberOfSides ) );
        circleVerticesZ[i] = z;
    }
    
    GLfloat allCircleVertices[( numberOfVertices ) * 3];
    
    for ( int i = 0; i < numberOfVertices; i++ )
    {
        allCircleVertices[i * 3] = circleVerticesX[i];
        allCircleVertices[( i * 3 ) + 1] = circleVerticesY[i];
        allCircleVertices[( i * 3 ) + 2] = circleVerticesZ[i];
    }
    //glColor4f(0.0f,0.0f,0.0f,1.0f);
    glColor4f(0.4f, 0.8f, 1.0f, 0.1f);
    glEnableClientState( GL_VERTEX_ARRAY );
    glVertexPointer( 3, GL_FLOAT, 0, allCircleVertices );
    glDrawArrays( GL_TRIANGLE_FAN, 0, numberOfVertices);
    glDisableClientState( GL_VERTEX_ARRAY );
}

void drawRing( GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLint numberOfSides )
{
    GLint numberOfVertices = numberOfSides + 1;
    
    GLfloat doublePi = 2.0f * M_PI;
    
    GLfloat circleVerticesX[numberOfVertices];
    GLfloat circleVerticesY[numberOfVertices];
    GLfloat circleVerticesZ[numberOfVertices];
    
    //circleVerticesX[0] = x;
    //circleVerticesY[0] = y;
    //circleVerticesZ[0] = z;
    
    for ( int i = 0; i < numberOfVertices; i++ )
    {
        circleVerticesX[i] = x + ( radius * cos( i * doublePi / numberOfSides ) );
        circleVerticesY[i] = y + ( radius * sin( i * doublePi / numberOfSides ) );
        circleVerticesZ[i] = z;
    }
    
    GLfloat allCircleVertices[numberOfVertices * 3];
    
    for ( int i = 0; i < numberOfVertices; i++ )
    {
        allCircleVertices[i * 3] = circleVerticesX[i];
        allCircleVertices[( i * 3 ) + 1] = circleVerticesY[i];
        allCircleVertices[( i * 3 ) + 2] = circleVerticesZ[i];
    }
    glColor4f(0.0f,0.0f,0.0f,1.0f);
    glEnableClientState( GL_VERTEX_ARRAY );
    glVertexPointer( 3, GL_FLOAT, 0, allCircleVertices );
    glDrawArrays( GL_LINE_STRIP, 0, numberOfVertices );
    glDisableClientState( GL_VERTEX_ARRAY );
}




