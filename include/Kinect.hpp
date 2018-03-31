//
// Created by Rory Shively on 3/4/18.
//

#pragma once

#include <libfreenect2/libfreenect2.hpp>
#include <libfreenect2/frame_listener_impl.h>
#include <libfreenect2/registration.h>
#include <libfreenect2/packet_pipeline.h>
#include <libfreenect2/logger.h>
#include <opencv2/opencv.hpp>



class Kinect
{
protected:
    bool                            shutdown = false;
    bool                            paused = false;
    bool                            enableRGB = false;
    bool                            enableIR = false;
    bool                            enableDepth = true;

    cv::Mat*                         inputMatix;

//    libfreenect2::Freenect2Device   *devtopause;
    libfreenect2::Freenect2Device   *device;
    libfreenect2::PacketPipeline    *pipeline;
    std::string                     serial;

//    libfreenect2::SyncMultiFrameListener    listener;
//    void(*listener)(unsigned int);
    unsigned int                types = 0;
    libfreenect2::FrameMap      frames;

    void        discover();
    void        getPipeline();
    void        configure();
    void        sigintHandler(int s);
    void        sigusr1Handler(int s);

public:
    Kinect( cv::Mat* inputMatRef );

    void        getFrames();
    void        start();
    void        pause();
    void        stop();

    std::string getSerial() { return serial; }
};
