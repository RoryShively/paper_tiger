//
// Created by Rory Shively on 2/26/18.
//

#pragma once

#include "cinder/Rand.h"
#include "cinder/gl/gl.h"
#include "opencv2/opencv.hpp"
#include <vector>


class VectorFieldInput
{
protected:
    cv::Mat                             mScalarField;
    cv::Mat                             mBinaryField;
    std::vector<std::vector<cv::Point>> mContours;
    std::vector<cv::Vec4i>              mHierarchy;
//    float                               mVectorField;
    std::vector<std::vector<std::vector<float>>>         mVectorField;

    cv::Point   getDiffVec( cv::Point destVec, cv::Point originVec );
    double      getVecMag( cv::Point diffVec );
    cv::Point   getUnitVec( cv::Point vec );
    cv::Point2f getUnitVec( cv::Point vec, double mag );
    cv::Point2f getVecPoint( cv::Point2f origin, cv::Point2f uvec);
    cv::Point   scaleVec( cv::Point vec, double mag );

public:
    VectorFieldInput() {};
    VectorFieldInput( cv::Mat img );
//    float* getField() { return *mVectorField; }
    std::vector<std::vector<std::vector<float>>>& getField() { return mVectorField; }
    ci::vec2 getVec(int x, int y) {
        int xSize = mVectorField.size();
        int ySize = mVectorField[0].size();

        float vecX, vecY;
        if ( x < xSize && y < ySize )
        {
            vecX = mVectorField[x][y][0];
            vecY = mVectorField[x][y][1];
        }
        else
            vecX = 0.0f; vecY = 0.0f;

//        std::cout << x << " " << y << "HAS: " << vecX << " " << vecY << std::endl;
        return ci::vec2( vecX * 2, vecY * 2 );
    }


};

