//
// Created by Rory Shively on 2/24/18.
//

#include <stdio.h>
#include "ParticleController.hpp"
#include "VectorFieldInput.hpp"


ParticleController::ParticleController( ci::ImageSourceRef &img )
{
    auto channel = ci::Channel32f( img );

    for( int y=0; y<( img->getWidth() / 10 ); y++ )
    {
        for( int x=0; x<( img->getHeight() / 10 ); x++ )
        {
            addParticle( y, x, channel );
        }
    }
}

void ParticleController::update( const ci::Channel32f &channel, VectorFieldInput &vInput, bool mExpel )
{
    for (auto &mParticle : mParticles) {
        mParticle.update( channel, vInput, mExpel );
    }
}

void ParticleController::draw()
{
//    for( auto p = mParticles.begin(); p != mParticles.end(); ++p )
    for (auto &mParticle : mParticles) {
        mParticle.draw();
    }
}

void ParticleController::addParticle( int xi, int yi, const ci::Channel32f &channel ) {
    float x = ( xi + 0.5f ) * 10.0f;
    float y = ( yi + 0.5f ) * 10.0f;
    mParticles.emplace_back( Particle( ci::vec2( x, y ), channel ) );
}
