//
// Created by Rory Shively on 3/4/18.
// https://openkinect.github.io/libfreenect2/index.html
// http://answers.opencv.org/question/76468/opencvkinect-onekinect-for-windows-v2linuxlibfreenect2/
//

#include "Kinect.hpp"

#include <iostream>
#include <libfreenect2/libfreenect2.hpp>
#include <libfreenect2/frame_listener_impl.h>
#include <libfreenect2/registration.h>
#include <libfreenect2/packet_pipeline.h>
#include <libfreenect2/logger.h>
#include <opencv2/opencv.hpp>



Kinect::Kinect( cv::Mat* inputMatRef )
{
    std::cout << "type at init Kinect: "  << inputMatRef << std::endl;
    inputMatix = inputMatRef;
    std::cout << "type at init Kinect: "  << inputMatix << std::endl;
    discover();
    getPipeline();
    configure();
    stop();
}

void Kinect::discover()
{
    libfreenect2::Freenect2 freenect2;
    device = nullptr;
    pipeline = nullptr;

    if( freenect2.enumerateDevices() == 0 )
    {
        std::cout << "no device connected!" << std::endl;
        return;
    }
    if ( serial.empty() )
    {
        serial = freenect2.getDefaultDeviceSerialNumber();
    }
}

void Kinect::getPipeline()
{
    pipeline = new libfreenect2::CpuPacketPipeline();
}

void Kinect::configure()
{
    libfreenect2::Freenect2 freenect2;
    device = freenect2.openDevice( serial, pipeline );

    if ( enableRGB )
        types |= libfreenect2::Frame::Color;
    if ( enableIR )
        types |= libfreenect2::Frame::Ir;
    if ( enableDepth )
        types |= libfreenect2::Frame::Depth;

    libfreenect2::SyncMultiFrameListener listener( (u_int) types );
    device->setColorFrameListener(&listener);
    device->setIrAndDepthFrameListener(&listener);

//    start();
//    getFrames();
//}
//
//void Kinect::start()
//{
    if ( enableRGB && enableDepth )
    {
        if ( !device->start() )
            return;
    }
    else
    {
        if ( !device->startStreams( enableRGB, enableDepth ) )
            return;
    }
    std::cout << "device serial: " << device->getSerialNumber() << std::endl;
    std::cout << "device firmware: " << device->getFirmwareVersion() << std::endl;
//}

//void Kinect::getFrames()
//{
//    libfreenect2::SyncMultiFrameListener listener(types);

    libfreenect2::Registration* registration = new libfreenect2::Registration(device->getIrCameraParams(), device->getColorCameraParams());
    libfreenect2::Frame undistorted(512, 424, 4), registered(512, 424, 4), depth2rgb(1920, 1080 + 2, 4);

    cv::Mat rgbmat, depthmat, depthmatUndistorted, irmat, rgbd, rgbd2;

    while(!shutdown)
    {
        if (!listener.waitForNewFrame(frames, 10*1000)) // 10 seconds
        {
            std::cout << "timeout!" << std::endl;
            return;
        }
//        libfreenect2::Frame *rgb = frames[libfreenect2::Frame::Color];
//        libfreenect2::Frame *ir = frames[libfreenect2::Frame::Ir];
        libfreenect2::Frame *depth = frames[libfreenect2::Frame::Depth];

//        std::cout << "depth height: " << depth->height << "\n" << "depth width: " << depth->width << std::endl;

//        std::cout << "type at init Kinect: "  << inputMatRef << std::endl;
//
        cv::Mat( (int)depth->height, (int)depth->width, CV_32FC1, depth->data ).copyTo( *inputMatix );

//        std::cout << "[kinect func] [matrix &]: " << inputMatRef << std::endl;
        std::cout << "[kinect func] [inputMatrix.size]: " << inputMatix << std::endl;
//        (*inputMatRef) = depthmat;
//        cv::imshow( "depth", depthmat / 4500.0f );

//        std::cout << depthmat.size << std::endl;

//        registration->apply(rgb, depth, &undistorted, &registered);

//        int key = cv::waitKey(1);
//        shutdown = shutdown || (key > 0 && ((key & 0xFF) == 27)); // shutdown on escape


        listener.release(frames);
    }
}

void Kinect::stop()
{
    device->stop();
    device->close();
}