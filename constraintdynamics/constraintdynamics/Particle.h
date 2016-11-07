//
//  Particle.h
//  constraintdynamics
//
//  Created by Raymond Kim on 11/6/16.
//  Copyright © 2016 raymnd. All rights reserved.
//

#ifndef Particle_h
#define Particle_h

#include <math.h>
#include <glm/glm.hpp>

using namespace glm;

class Particle {
    
public:
    
    Particle() {
        this->mposition = vec2(0.0f, 1.0f);
        this->mtheta = M_PI/2;
        this->msize = 10;
        this->mmass = 1; //kilogram
        this->mangvel = 0.0f;
    }
    
    vec2 position() {
        return mposition;
    }
    float velocity() {
        return mangvel;
    }
    float size() {
        return msize;
    }
    float theta() {
        return mtheta;
    }
    float mass() {
        return mmass;
    }
    void setPosition(vec2 p) {
        this->mposition = p;
        this->mtheta = acosf(p.x);
    }
    void setPosition(float t) {
        this->mposition = vec2(cosf(t), sinf(t));
        this->mtheta = t;
    }
    void setVelocity(float v) {
        this->mangvel = v;
    }
    void setMass(float m) {
        this->mmass = m;
    }
    
    
private:
    
    vec2 mposition;
    float mangvel;
    float mtheta;
    float msize;
    float mmass;
    
    
};


#endif /* Particle_h */
