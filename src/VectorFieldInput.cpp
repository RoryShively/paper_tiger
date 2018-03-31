//
// Created by Rory Shively on 2/26/18.
//

#include "opencv2/opencv.hpp"

#include "../include/VectorFieldInput.hpp"


VectorFieldInput::VectorFieldInput( cv::Mat img )
{
    // Convert image to grayscale. Effectively a scalar field
    cv::cvtColor( img, mScalarField, cv::COLOR_RGB2GRAY );
//    img.convertTo(mScalarField, CV_8U, 255.0);

    // Convert to binary image
    cv::threshold( mScalarField, mScalarField, 100, 255, 0 );

    // Get contours and hierarchy
    findContours(
            mBinaryField,
            mContours,
            mHierarchy,
            CV_RETR_TREE,
            CV_CHAIN_APPROX_SIMPLE,
            cv::Point( 0, 0 )
    );

    int imgWidth = mScalarField.size().width;
    int imgHeight = mScalarField.size().height;

    cv::imshow( "depth", mScalarField );

    cv::waitKey(1);

//    cv::imshow( "binary", mScalarField );
//
//    cv::waitKey(1);

//    cv::Mat drawing = cv::Mat::zeros( mScalarField.size(), CV_8UC3 );
//    cv::RNG rng(12345);
//    for( int i = 0; i< mContours.size(); i++ )
//    {
//        cv::Scalar color = cv::Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
//        drawContours( drawing, mContours, i, color, 2, 8, mHierarchy, 0, cv::Point() );
//    }
//
//    /// Show in a window
//    cv::namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
//    cv::imshow( "Contours", drawing );
//
//    cv::waitKey(1);

    std::vector< cv::Point > contour;
    if ( !mContours.empty() )
        contour = mContours[0];

    for( int x = 0; x < imgWidth; x += 1 )
    {
        std::vector < std::vector < float > > w;
        mVectorField.push_back( w );

        for( int y = 0; y < imgHeight; y += 1 )
        {

            std::vector <float> v;
            mVectorField[x].push_back( v );
            auto particlePoint = cv::Point( x, y );

            double mag = 0;
            if ( !mContours.empty() )
                mag = pointPolygonTest( contour, cv::Point( particlePoint.x, particlePoint.y ), true );

            if( mag > 10 )
            {
                cv::Point closest = contour[0];
                double dist = pow( ( particlePoint.x - contour[0].x ), 2 ) + pow( ( particlePoint.y - contour[0].y ), 2 );
                for(int i = 1; i< contour.size(); i++)
                {
                    double tempDist = pow( ( particlePoint.x - contour[i].x ), 2 ) + pow( ( particlePoint.y - contour[i].y ), 2 );
                    if( tempDist < dist )
                    {
                        dist = tempDist;
                        closest = contour[i];
                    }
                }

                // Unit vec
                auto diffVec = getDiffVec( closest,  particlePoint );
                double vecMag = getVecMag( diffVec );
                auto unitVec = getUnitVec( diffVec, vecMag );

                mVectorField[x][y].push_back( unitVec.x * static_cast<float>(mag) / 8 );
                mVectorField[x][y].push_back( unitVec.y * static_cast<float>(mag) / 8 );

            }
            else
            {
                mVectorField[x][y].push_back( 0 );
                mVectorField[x][y].push_back( 0 );
            }

        }
    }
}

cv::Point VectorFieldInput::getDiffVec( cv::Point destVec, cv::Point originVec )
{
    auto dVec = destVec - originVec;
    if ( isnan(dVec.x) || isnan(dVec.y) ) return cv::Point( 0, 0 );
    return dVec;
}

double VectorFieldInput::getVecMag( cv::Point diffVec )
{
    return pow( ( pow((diffVec.x ), 2 ) + pow( ( diffVec.y ), 2) ), 0.5 );
}

cv::Point VectorFieldInput::getUnitVec( cv::Point vec )
{
    auto uVec = vec / getVecMag(vec);
    if ( isnan(uVec.x) || isnan(uVec.y) ) return cv::Point( 0, 0 );
    return uVec;
}

cv::Point2f VectorFieldInput::getUnitVec( cv::Point vec, double mag )
{
//    auto uVec = vec / mag;
//    if ( isnan( uVec.x ) || isnan( uVec.y ) ) return cv::Point( 0, 0 );
//    return uVec;
    return cv::Point2f( ( vec.x / (float) mag ), ( vec.y / (float) mag ) );
}

cv::Point2f VectorFieldInput::getVecPoint( cv::Point2f origin, cv::Point2f uvec)
{
    return cv::Point2f( ( origin.x + ( uvec.x * 10 ) ), ( origin.y + ( uvec.y * 10 ) ) );
}

cv::Point VectorFieldInput::scaleVec( cv::Point vec, double mag )
{
    auto sVec = vec * mag;
    if ( isnan( sVec.x ) || isnan( sVec.y ) ) return cv::Point( 0, 0 );
    return sVec;
}
