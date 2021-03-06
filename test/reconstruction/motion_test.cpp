#include "io.h"
#include "ocv.h"
#include "eigen.h"
#include "timer.h"
#include "reconstruction/eucm_motion_stereo.h"
#include "reconstruction/scale_parameters.h"


//TODO use SGM stereo as prior to the motion stereo to improve the accuacy

int main(int argc, char** argv)
{	
    ifstream paramFile(argv[1]);
    if (not paramFile.is_open())
    {
        cout << argv[1] << " : ERROR, file is not found" << endl;
        return 0;
    }
    
    array<double, 6> params1;
    array<double, 6> params2;
    
    cout << "First EU Camera model parameters :" << endl;
    for (auto & p: params1) 
    {
        paramFile >> p;
        cout << setw(10) << p;
    }
    cout << endl;
    paramFile.ignore();
    
    cout << "Second EU Camera model parameters :" << endl;
    for (auto & p: params2) 
    {
        paramFile >> p;
        cout << setw(10) << p;
    }
    cout << endl;
    paramFile.ignore();
    
    array<double, 6> cameraPose;
    cout << "Camera pose wrt the robot :" << endl;
    for (auto & e: cameraPose) 
    {
        paramFile >> e;
        cout << setw(10) << e;
    }
    cout << endl;
    paramFile.ignore();
    Transformation<double> TbaseCamera(cameraPose.data());
    
    array<double, 6> robotPose1, robotPose2;
    cout << "First robot's pose :" << endl;
    for (auto & e: robotPose1) 
    {
        paramFile >> e;
        cout << setw(10) << e;
    }
    cout << endl;
    cout << "Second robot's pose :" << endl;
    for (auto & e: robotPose2) 
    {
        paramFile >> e;
        cout << setw(10) << e;
    }
    cout << endl;
    paramFile.ignore();
    Transformation<double> T01(robotPose1.data()), T02(robotPose2.data());
    
    Transformation<double> TleftRight = T01.compose(TbaseCamera).inverseCompose(T02.compose(TbaseCamera));
    
    MotionStereoParameters stereoParams;
    stereoParams.verbosity = 0;
    int foo;
    paramFile >> foo;
    paramFile >> foo;
    paramFile >> stereoParams.dispMax;
    paramFile >> stereoParams.scale;
    paramFile.ignore();
    string fileName1, fileName2;
    
    
    
    while(getline(paramFile, fileName1))
    {
        getline(paramFile, fileName2);
        
        Mat8u img1 = imread(fileName1, 0);
        Mat8u img2 = imread(fileName2, 0);
    //    img1 /= 0.96;
    //    
    //    Laplacian(img1, img1lap, CV_16S, 3);
    //    Laplacian(img2, img2lap, CV_16S, 3);
    //    
    //    GaussianBlur(img1, img1, Size(3, 3), 0, 0);
    //    GaussianBlur(img2, img2, Size(3, 3), 0, 0);
    //    
    //    img1lap = img1lap + 128;
    //    img2lap = img2lap + 128;
    //    img1lap.copyTo(img1);
    //    img2lap.copyTo(img2);
    ////    
        
        stereoParams.u0 = 0;
        stereoParams.v0 = 0;
        stereoParams.uMax = img1.cols;
        stereoParams.vMax = img1.rows;
        stereoParams.setEqualMargin();
//        stereoParams.setYMargin(330);
        
        Timer timer;
        EnhancedCamera camera1(params1.data()), camera2(params2.data());
        MotionStereo stereo(&camera1, &camera2, stereoParams);
        stereo.setBaseImage(img1);
        cout << "    initialization time : " << timer.elapsed() << endl;
        
        
    //    
    //    for (auto & x : {Point(320, 300), Point(500, 300), Point(750, 300), Point(350, 500), Point(600, 450)})
    //    {
    //        out1(x) = 0;
    //        out1(x.y + 1, x.x) = 0;
    //        out1(x.y, x.x + 1) = 255;
    //        out1(x.y + 1, x.x + 1) = 255;
    //        stereo.traceEpipolarLine(x, out2);
    //    }
        
        
        
        

    //    scaleParams.u0 = 700;
    //    scaleParams.v0 = 200;
    //    scaleParams.uMax = img1.cols;
    //    scaleParams.vMax = img1.rows;
    //    scaleParams.setYMargin(700);
    //    scaleParams.setXMargin(400);
        
        Mat32f res;
        timer.reset();
        DepthMap depth = stereo.compute(TleftRight, img2);
        cout << timer.elapsed() << endl;
        timer.reset();

//        depth.filterNoise();
        cout << timer.elapsed() << endl;
        
        depth.toInverseMat(res);    
//        medianBlur(res, res, 5);
        imshow("out1", img1);
        imshow("out2", img2);
        imshow("res", res / 10);
                
        waitKey(); 
    }
    return 0;
}



