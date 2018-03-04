#include "opencv2/opencv.hpp"

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Log.h"

#include "../include/ParticleController.hpp"
#include "VectorFieldInput.hpp"


class PaperTigerApp : public ci::app::App {
private:
    bool                        mExpel;
    ci::ImageSourceRef          mInput;
    ci::Channel32f              mImgChannel;
    ParticleController          mParticleController;
    ci::vec2                    mMouseLoc;
    VectorFieldInput            mVectorField;

public:
    void setup() override;
    void mouseDown( ci::app::MouseEvent event ) override;
    void mouseUp( ci::app::MouseEvent event ) override;
    void mouseMove( ci::app::MouseEvent event ) override;
    void mouseDrag( ci::app::MouseEvent event ) override;
    void update() override;
    void draw() override;

    void enableFileLogging()
        { ci::log::makeLogger<ci::log::LoggerFile>( "/tmp/logging/cinder.log", true ); }
};

void PaperTigerApp::setup()
{
    // Load background image
    ci::ImageSourceRef img = ci::loadImage( ci::app::loadAsset( "tiger.jpg" ) );
    ci::ImageSourceRef mInput = ci::loadImage( ci::app::loadAsset( "blob2.jpg" ) );
//    mImgChannel = Channel32f( img );

    // Load particles
    mParticleController = ParticleController( img );

    // OpenCV Test
    // ###########
    auto input = cv::imread( "/Users/rory.shively/CLionProjects/paper_tiger/assets/blob2.jpg", -1 );
    mVectorField = VectorFieldInput( input );
}

void PaperTigerApp::mouseDown( ci::app::MouseEvent event )
{
    mExpel = true;
}

void PaperTigerApp::mouseUp( ci::app::MouseEvent event )
{
    mExpel = false;
}

void PaperTigerApp::mouseMove( ci::app::MouseEvent event ) {
    mMouseLoc = event.getPos();
}

void PaperTigerApp::mouseDrag( ci::app::MouseEvent event ) {
    mouseMove( event );
}

void PaperTigerApp::update()
{
    mParticleController.update( mImgChannel, mMouseLoc, mInput, mVectorField, mExpel );
}

void PaperTigerApp::draw()
{
    ci::gl::clear();
    mParticleController.draw();
}

CINDER_APP( PaperTigerApp, ci::app::RendererGl, [](ci::app::App::Settings *settings) {
    settings->setWindowSize( 1000, 600 );
    settings->setFrameRate( 120 );
})
