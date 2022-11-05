#include "close-gaps.hpp"

cv::Mat closeGaps(std::string path, cv::Mat img, bool saving) {
    cv::Mat image;
    image = read(path, img);
    // convert image to binary
    cv::Mat thresh;
    cv::cvtColor(image, image, cv::COLOR_RGB2GRAY);
    cv::threshold(image, thresh, 127, 255, cv::THRESH_BINARY);

    cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(1, 3));
    cv::Mat src = image;
    cv::Mat closed;
    for (int i = 0; i < 1; i++) {
        cv::morphologyEx(src, closed, cv::MORPH_CLOSE, element);
        src = closed;
    }

    cv::Mat element2 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 1));
    src = image;
    for (int i = 0; i < 1; i++) {
        cv::morphologyEx(src, closed, cv::MORPH_CLOSE, element2);
        src = closed;
    }

    if (saving) {
        save(src, path, "-closed");
    }

    return src;
}


// #include <opencv2/opencv.hpp> 

// using namespace cv;
// using namespace std;


// vector<vector<Point> > contours;
// vector<Vec4i> hierarchy;

// vector<Point2d> z;
// vector<Point2d> Z;


// int main(int argc, char **argv)
// {
// Mat mTest,mThresh,mConnected;
// int nbIter;
// int borderType=cv::BORDER_REPLICATE;
// Scalar borderValue=cv::morphologyDefaultBorderValue();
// Point ancrage=cv::Point(-1,-1);
// cv::Mat element = cv::getStructuringElement( MORPH_CROSS,cv::Size( 3, 3 ),cv::Point( 1, 1 ) );


// Mat  m=imread("../images/test-close/gaps.png");
// cvtColor(m, m, cv::COLOR_RGB2GRAY);
// nbIter=1;
// Mat m1,m2,m3,m4,m5;
// threshold(m,mThresh,150,255,THRESH_BINARY);
// mThresh.convertTo(mThresh, CV_8UC1);
// erode(mThresh, m1, element,ancrage,nbIter,borderType,borderValue);
// findContours(m1,contours,hierarchy, RETR_LIST, CHAIN_APPROX_NONE);
// int sizeMax=0,idx=0;
// for (int i = 1; i < contours.size(); i++)
// {
//     if (contours[i].size()>sizeMax)
//     {
//         idx=i;
//         sizeMax=contours[i].size();
//     }
// }
// Mat mc=Mat::zeros(m.size(),CV_8UC1);
// Mat md=Mat::zeros(m.size(),CV_8UC1);

// Moments mu = moments( contours[idx], false );

// for (int i=0;i<contours[idx].size();i++)
//     z.push_back(contours[idx][i]);
// dft(z,Z,DFT_COMPLEX_OUTPUT|DFT_SCALE);

// vector<Point> ctrAppro;
// vector<Point2d> derivAppro;
// complex<double> I (0,1);
// double TwoPI = 2*acos(-1.0);
// int fn=300;
// for (int i = 0; i < contours[idx].size(); i++)
// {
//     complex<double> zz=complex<double>(Z[0].x,Z[0].y);
//     complex<double> zzd=complex<double>(0,0);
//     for (int j = 1; j < fn; j++)
//     {
//         double s = double(i) ;
//         complex<double> cplusn=complex<double>(Z[j].x,Z[j].y);
//         complex<double> cminusn=complex<double>(Z[Z.size()-j].x,Z[Z.size()-j].y);
//         zz += cplusn * exp(I*complex<double>(TwoPI*s*j/Z.size()))+cminusn * exp(I*complex<double>(TwoPI*s*(Z.size()-j)/Z.size()));
//         zzd += cplusn * I*complex<double>(TwoPI*j/Z.size()) *exp(I*complex<double>(TwoPI*s*j/Z.size()))+cminusn*I*complex<double>(-TwoPI*j/Z.size()) * exp(I*complex<double>(TwoPI*s*(Z.size()-j)/Z.size()));
//     }
//     ctrAppro.push_back(Point((int)(zz.real()),(int)(zz.imag())));
//     derivAppro.push_back(Point2d(zzd.real(),zzd.imag()));

// }
// mc.convertTo(mc, CV_8UC1);
// drawContours(mc,contours,idx, Scalar(128),FILLED);
// contours.push_back(ctrAppro);
// bool firstHole=false;
// Point pp0,pp1,pp2,pp3;
// for (int i = 0; i < contours[idx].size(); i += 1)
// {
//     Point p1,p2;
//     p1 = Point(ctrAppro[i].x+5*derivAppro[i].y/ norm(derivAppro[i]),  ctrAppro[i].y-5*derivAppro[i].x/ norm(derivAppro[i])) ;
//     p2 = Point(ctrAppro[i].x+80*derivAppro[i].y/ norm(derivAppro[i]),  ctrAppro[i].y-80*derivAppro[i].x/ norm(derivAppro[i])) ;
//     line(mc, p1, p2, Scalar(255),2);
//     cv::LineIterator it(mThresh, p1, p2, 4);
//     cv::LineIterator it2 = it;
//     vector<uchar> buf(it.count);
//     bool hole=true;
//     for(int nbPt = 0; nbPt < it.count; nbPt++, ++it)
//     {
//         if (mThresh.at<uchar>(it.pos()) == 0)
//         {
//             hole=false;
//             break;
//         }
//     }
//     if (hole == true && firstHole==false)
//     {
//         p1 = Point(ctrAppro[i].x-4*derivAppro[i].y/ norm(derivAppro[i]),  ctrAppro[i].y+4*derivAppro[i].x/ norm(derivAppro[i])) ;
//         p2 = Point(ctrAppro[i].x+20*derivAppro[i].y/ norm(derivAppro[i]),  ctrAppro[i].y-20*derivAppro[i].x/ norm(derivAppro[i])) ;
//         line(mThresh, p1, p2, Scalar(0),1);
//         firstHole=true;
//         i+=4;
//         pp0=p1;
//         pp1=p2;
//     }
//     else if (hole==false && firstHole==true)
//     {
//         p1 = Point(ctrAppro[i].x-4*derivAppro[i].y/ norm(derivAppro[i]),  ctrAppro[i].y+4*derivAppro[i].x/ norm(derivAppro[i])) ;

//         p2 = Point(ctrAppro[i].x+20*derivAppro[i].y/ norm(derivAppro[i]),  ctrAppro[i].y-20*derivAppro[i].x/ norm(derivAppro[i])) ;
//         line(mThresh, p1, p2, Scalar(0),1);
//         firstHole=false;
//         i+=4;
//         pp2=p2;
//         pp3=p1;
//         line(mThresh, pp0, pp3, Scalar(255),5);
//     }


// }
// mc.convertTo(mc, CV_8UC1);
// drawContours(mc,contours,contours.size()-1, Scalar(255));

// imwrite("../images/test-close/mdes.png",mc);
// imwrite("../images/test-close/mthresh.png",mThresh);
// return 0;
// };



// #include "close-gaps.hpp"

// std::vector<std::vector<cv::Point>> contours;
// std::vector<cv::Vec4i> hierarchy;

// std::vector<cv::Point2d> z;
// std::vector<cv::Point2d> Z;


// cv::Mat closeGaps(std::string path, cv::Mat img, bool saving) {
//     cv::Mat image;
//     image = read(path, img);
//     // convert image to binary
//     cv::Mat thresh;
//     cv::cvtColor(image, image, cv::COLOR_RGB2GRAY);
//     cv::threshold(image, thresh, 127, 255, cv::THRESH_BINARY);

//     // erode image
//     int nbIter;
//     int borderType = cv::BORDER_REPLICATE;
//     cv::Scalar borderValue = cv::morphologyDefaultBorderValue();
//     cv::Point ancrage = cv::Point(-1, -1);
//     cv::Mat element = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(3, 3), cv::Point(1, 1));

//     cv::Mat eroded;
//     cv::erode(thresh, eroded, element, ancrage, nbIter, borderType, borderValue);

//     // find contours
//     cv::findContours(eroded, contours, hierarchy, cv::RETR_LIST , cv::CHAIN_APPROX_NONE);
    
//     // find largest contour
//     int sizeMax = 0;
//     int idx = 0;
//     for (int i = 1; i < contours.size(); i++) {
//         if (contours[i].size() > sizeMax) {
//             idx = i;
//             sizeMax = contours[i].size();
//         }
//     }

//     cv::Mat mc = cv::Mat::zeros(image.size(), CV_8UC1);
//     cv::Mat md = cv::Mat::zeros(image.size(), CV_8UC1);
    
//     cv::Moments mu = cv::moments(contours[idx], false);
//     for (int i = 0; i < contours[idx].size(); i++) {
//         z.push_back(contours[idx][i]);
//     }

//     cv::dft(z, Z, CV_HAL_DFT_COMPLEX_OUTPUT);

//     std::vector<cv::Point> ctrAppro; 
//     std::vector<cv::Point> derivAppro;
//     std::complex<double> I (0, 1);
//     double TwoPI = 2*std::acos(-1.0);
//     int fn = 300;
//     for (int i = 0; i < contours[idx].size(); i++){
//         std::complex<double> zz = std::complex<double>(Z[0].x, Z[0].y);
//         std::complex<double> zzd = std::complex<double>(0, 0);
//         for (int j = 1; j < fn; j++) {
//             double s = double(i);
//             std::complex<double> cplusn = std::complex<double>(Z[j].x,Z[j].y);
//             std::complex<double> cminusn = std::complex<double>(Z[Z.size()-j].x,Z[Z.size()-j].y);
//             zz += cplusn * exp(I*std::complex<double>(TwoPI*s*j/Z.size()))+cminusn * exp(I*std::complex<double>(TwoPI*s*(Z.size()-j)/Z.size()));
//             zzd += cplusn * I*std::complex<double>(TwoPI*j/Z.size()) *exp(I*std::complex<double>(TwoPI*s*j/Z.size()))+cminusn*I*std::complex<double>(-TwoPI*j/Z.size()) * exp(I*std::complex<double>(TwoPI*s*(Z.size()-j)/Z.size()));
//         }
//         ctrAppro.push_back(cv::Point((int)(zz.real()),(int)(zz.imag())));
//         derivAppro.push_back(cv::Point2d(zzd.real(),zzd.imag()));
//     }

//     cv::drawContours(mc,contours,idx, cv::Scalar(128), cv::FILLED);
//     contours.push_back(ctrAppro);
//     bool firstHole=false;
//     cv::Point pp0,pp1,pp2,pp3;

//     for (int i = 0; i < contours[idx].size(); i += 1) {
//         cv::Point p1,p2;
//         p1 = cv::Point(ctrAppro[i].x+5*derivAppro[i].y/ cv::norm(derivAppro[i]),  ctrAppro[i].y-5*derivAppro[i].x/ cv::norm(derivAppro[i])) ;
//         p2 = cv::Point(ctrAppro[i].x+80*derivAppro[i].y/ cv::norm(derivAppro[i]),  ctrAppro[i].y-80*derivAppro[i].x/ cv::norm(derivAppro[i])) ;
//         cv::line(mc, p1, p2, cv::Scalar(255),2);
//         cv::LineIterator it(thresh, p1, p2, 4);
//         cv::LineIterator it2 = it;
//         std::vector<uchar> buf(it.count);
//         bool hole=true; 

//         for(int nbPt = 0; nbPt < it.count; nbPt++, ++it) {
//             if (thresh.at<uchar>(it.pos()) == 0) {
//                 hole=false;
//                 break;
//             }
//         }

//         if (hole == true && firstHole==false) {
//             p1 = cv::Point(ctrAppro[i].x-4*derivAppro[i].y/ cv::norm(derivAppro[i]),  ctrAppro[i].y+4*derivAppro[i].x/ cv::norm(derivAppro[i])) ;
//             p2 = cv::Point(ctrAppro[i].x+20*derivAppro[i].y/ cv::norm(derivAppro[i]),  ctrAppro[i].y-20*derivAppro[i].x/ cv::norm(derivAppro[i])) ;
//             cv::line(thresh, p1, p2, cv::Scalar(0),1);
//             firstHole=true;
//             i+=4;
//             pp0=p1;
//             pp1=p2;
//         }  else if (hole==false && firstHole==true) {
//             p1 = cv::Point(ctrAppro[i].x-4*derivAppro[i].y/ cv::norm(derivAppro[i]),  ctrAppro[i].y+4*derivAppro[i].x/ cv::norm(derivAppro[i])) ;

//             p2 = cv::Point(ctrAppro[i].x+20*derivAppro[i].y/ cv::norm(derivAppro[i]),  ctrAppro[i].y-20*derivAppro[i].x/ cv::norm(derivAppro[i])) ;
//             cv::line(thresh, p1, p2, cv::Scalar(0),1);
//             firstHole=false;
//             i+=4;
//             pp2=p2;
//             pp3=p1;
//             cv::line(thresh, pp0, pp3, cv::Scalar(255),5);
//         }
//     }   

//     cv::drawContours(mc,contours,contours.size()-1, cv::Scalar(255));

//     if (saving) {
//         save(thresh, path, "-closed");
//     }

//     return thresh;
// }

int main(int argc, char** argv) {
    if (argc < 2) {
         std::cerr << "Must pass in image to run DoG on." << std::endl;
    } else {
        for (int i = 1; i < argc; i++) {
            cv::Mat image;
            closeGaps(argv[i], image, true);
        }
    }
}