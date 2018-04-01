//
// Created by Rory Shively on 2/24/18.
//

#pragma once

#include <math.h>
#include "cinder/Rand.h"
#include "cinder/gl/gl.h"
#include "VectorFieldInput.hpp"


class Particle
{
protected:
    ci::vec2        mGravityCenter;
    ci::vec2        mLoc;
    ci::Color       mColor;
    float           mRadius;
    ci::vec2        mMomentum;

public:
    Particle( ci::vec2 loc, const ci::Channel32f &channel );
    void update( const ci::Channel32f &channel, VectorFieldInput &vInput, bool mExpel );
    void draw();
};
