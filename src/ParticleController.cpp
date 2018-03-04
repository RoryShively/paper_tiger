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

void ParticleController::update( const ci::Channel32f &channel, const ci::vec2 &mouseLoc, ci::ImageSourceRef &input, VectorFieldInput &vInput, bool mExpel )
{
    for( std::list<Particle>::iterator p = mParticles.begin(); p != mParticles.end(); ++p )
    {
        p->update( channel, mouseLoc, input, vInput, mExpel );
    }
}

void ParticleController::draw()
{
    for( std::list<Particle>::iterator p = mParticles.begin(); p != mParticles.end(); ++p )
    {
        p->draw();
    }
}

void ParticleController::addParticle( int xi, int yi, const ci::Channel32f &channel ) {
    float x = ( xi + 0.5f ) * 10.0f;
    float y = ( yi + 0.5f ) * 10.0f;
    mParticles.push_back( Particle( ci::vec2( x, y ), channel ) );
}
