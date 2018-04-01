#include <thread>

#include "opencv2/opencv.hpp"

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Log.h"

#include "../include/ParticleController.hpp"
#include "../include/Kinect.hpp"
#include "VectorFieldInput.hpp"


class PaperTigerApp : public ci::app::App {
private:
    bool                        mExpel;
    ci::ImageSourceRef          mInput;
    ci::Channel32f              mImgChannel;
    ParticleController          mParticleController;
    ci::vec2                    mMouseLoc;
    VectorFieldInput            mVectorField;

    // Kinect related stuff
    std::thread                 kinectThread;
    cv::Mat                     inputMatrix;

    // Temporary
    cv::Mat                     input;

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


void setupKinect( cv::Mat* inputMatRef )
{
    std::cout << "type at setup kinect: " << inputMatRef << std::endl;
    auto kinect = Kinect( inputMatRef );
//    std::cout << kinect.getSerial() << std::endl;
//    kinect.start();
}

void PaperTigerApp::setup()
{
    // Pipe image data to inputMatrix through Kinect
//    kinectThread = std::thread( setupKinect, &inputMatrix );

    // Load background image
    ci::ImageSourceRef img = ci::loadImage( ci::app::loadAsset( "tiger.jpg" ) );

    // Load particles
    mParticleController = ParticleController( img );

    // OpenCV Test
    // ###########
//    sleep( 5 ); // Sleep 2 seconds
//    mVectorField = VectorFieldInput( inputMatrix );
    input = cv::imread( "/Users/Rory/CLionProjects/paper_tiger/assets/blob2.jpg", -1 );
//    mVectorField = VectorFieldInput( input );
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
    mVectorField = VectorFieldInput( input );
    mParticleController.update( mImgChannel, mVectorField, mExpel );

//    std::cout << "[update func] [matrix &]: " << &inputMatrix << std::endl;
//    std::cout << "[update func] [inputMatrix.size]: " << inputMatrix.size << std::endl;
//
//    cv::imshow( "depth", inputMatrix / 4500.0f );
//
//    cv::waitKey(1);
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
