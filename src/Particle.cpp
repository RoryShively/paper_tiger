//
// Created by Rory Shively on 2/24/18.
//

#include "Particle.hpp"
#include "VectorFieldInput.hpp"
#include <vector>


ci::vec2 getDiffVec( ci::vec2 destVec, ci::vec2 originVec )
{
    auto dVec = destVec - originVec;
    if ( isnan(dVec.x) || isnan(dVec.y) ) return ci::vec2( 0, 0 );
    return dVec;
}

float getVecMag( ci::vec2 diffVec )
{
    return pow( ( pow((diffVec.x ), 2 ) + pow( ( diffVec.y ), 2) ), 0.5 );
}

ci::vec2 getUnitVec( ci::vec2 vec )
{
    auto uVec = vec / getVecMag(vec);
    if ( isnan(uVec.x) || isnan(uVec.y) ) return ci::vec2( 0, 0 );
    return uVec;
}

ci::vec2 getUnitVec( ci::vec2 vec, float mag )
{
    auto uVec = vec / mag;
    if ( isnan( uVec.x ) || isnan( uVec.y ) ) return ci::vec2( 0, 0 );
    return uVec;
}

ci::vec2 scaleVec( ci::vec2 vec, float mag )
{
    auto sVec = vec * mag;
    if ( isnan( sVec.x ) || isnan( sVec.y ) ) return ci::vec2( 0, 0 );
    return sVec;
}


Particle::Particle( ci::vec2 loc, const ci::Channel32f &channel )
{
    mLoc = loc;
    mGravityCenter = loc;
    mMomentum = ci::vec2( 0, 0 );
    mRadius = channel.getValue( loc ) * 4.0f;

    float gray = channel.getValue( loc );
    mColor = ci::Color( 0, gray / 1.6, gray );
}

void Particle::update( const ci::Channel32f &channel, const ci::vec2 &mouseLoc, ci::ImageSourceRef &input, VectorFieldInput &vInput, bool mExpel )
{
//    auto expelVec = getDiffVec( mLoc,  mouseLoc );
//    float expelMagtitude = getVecMag( expelVec );
//    ci::vec2 expelUnitVec = getUnitVec( expelVec, expelMagtitude );
//    float eForce = 30000 / ( pow( expelMagtitude, 2 ) + 600 );
//    ci::vec2 eVec = scaleVec( expelUnitVec, eForce );


//    int locX = ( ( (int) mLoc.x % 10 ) * 10 );
//    int locY = ( ( (int) mLoc.y % 10 ) * 10 );
//    std::cout << "x: " << locX << "y: " << locY;

    auto gravVec = getDiffVec( mGravityCenter, mLoc );
    float gravMagtitude = getVecMag( gravVec );
    ci::vec2 gravUnitVec = getUnitVec( gravVec, gravMagtitude );
    float gForce = log( gravMagtitude ) * 2;
    ci::vec2 gVec = scaleVec( gravUnitVec, gForce );

    if ( mExpel )
    {
        int locX = static_cast<int>(mLoc.x);
        int locY = static_cast<int>(mLoc.y);
        auto eVec = vInput.getVec( locX, locY );
        mMomentum = eVec + gVec;
    } else {
        mMomentum = gVec;
    }


    mLoc += mMomentum;
}

void Particle::draw()
{
    ci::gl::color( mColor );
    if ( mRadius > 1.0f )
    {
        ci::gl::drawSolidCircle( mLoc, mRadius );
    }
}

