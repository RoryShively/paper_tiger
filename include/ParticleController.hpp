//
// Created by Rory Shively on 2/24/18.
//

#pragma once

#include "Particle.hpp"
#include "VectorFieldInput.hpp"
#include <list>


class ParticleController
{
protected:
    std::list<Particle> mParticles;
    ci::Channel32f          mChannel;

public:
    ParticleController() {};
    ParticleController( ci::ImageSourceRef &img );

    void update( const ci::Channel32f &channel, VectorFieldInput &vInput, bool mExpel );
    void draw();
    void addParticles( int amt );
    void addParticle( int xi, int yi, const ci::Channel32f &channel );
    void removeParticles( int amt );
};
