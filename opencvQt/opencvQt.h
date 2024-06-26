#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/qfiledialog.h>
#include <QtWidgets/qinputdialog.h>
#include <QtWidgets/qmessagebox.h>
#include "ui_opencvQt.h"

#include <opencv2/opencv.hpp>

class opencvQt : public QMainWindow
{
    Q_OBJECT

public:
    opencvQt(QWidget *parent = nullptr);
    ~opencvQt();
    static void showHist(cv::Mat& img, cv::Mat& dst)
    {
        using namespace cv;
        using namespace std;
        //1、创建3个矩阵来处理每个通道输入图像通道。
        //我们用向量类型变量来存储每个通道，并用split函数将输入图像划分成3个通道。
        vector<Mat>bgr;
        split(img, bgr);

        //2、定义直方图的区间数
        int numbers = 256;

        //3、定义变量范围并创建3个矩阵来存储每个直方图
        float range[] = { 0,256 };
        const float* histRange = { range };
        Mat b_hist, g_hist, r_hist;

        //4、使用calcHist函数计算直方图
        int numbins = 256;
        calcHist(&bgr[0], 1, 0, Mat(), b_hist, 1, &numbins, &histRange);
        calcHist(&bgr[1], 1, 0, Mat(), g_hist, 1, &numbins, &histRange);
        calcHist(&bgr[2], 1, 0, Mat(), r_hist, 1, &numbins, &histRange);

        //5、创建一个512*300像素大小的彩色图像，用于绘制显示
        int width = 512;
        int height = 300;
        Mat histImage(height, width, CV_8UC3, Scalar(20, 20, 20));

        //6、将最小值与最大值标准化直方图矩阵
        normalize(b_hist, b_hist, 0, height, NORM_MINMAX);
        normalize(g_hist, g_hist, 0, height, NORM_MINMAX);
        normalize(r_hist, r_hist, 0, height, NORM_MINMAX);

        //7、使用彩色通道绘制直方图
        int binStep = cvRound((float)width / (float)numbins);  //通过将宽度除以区间数来计算binStep变量

        for (int i = 1; i < numbins; i++)
        {
            line(histImage,
                Point(binStep * (i - 1), height - cvRound(b_hist.at<float>(i - 1))),
                Point(binStep * (i), height - cvRound(b_hist.at<float>(i))),
                Scalar(255, 0, 0)
            );
            line(histImage,
                Point(binStep * (i - 1), height - cvRound(g_hist.at<float>(i - 1))),
                Point(binStep * (i), height - cvRound(g_hist.at<float>(i))),
                Scalar(0, 255, 0)
            );
            line(histImage,
                Point(binStep * (i - 1), height - cvRound(r_hist.at<float>(i - 1))),
                Point(binStep * (i), height - cvRound(r_hist.at<float>(i))),
                Scalar(0, 0, 255)
            );
        }
        dst = histImage;
        return;
    }

private slots:
    void openimg();
    void saveimg();
    void blur();
    void midblur();
    void gray();
    void about();
    void video_gray();
    void showsource();
    void showdst();
    void video_blur();
    void video_detect();
    void edgedetect();
    void bin();
    void video_bin();
    void video_midblur();
    void caclHist();
    void equlized();
    void lapulasi();
    
private:
    QImage tmpimg;
    QImage sourceimg;
    QString oriingpath;
    Ui::opencvQtClass ui;
    void showall(std::string name,QImage src);
    cv::Mat img2mat(QImage image) {
        cv::Mat Img;
        //qDebug() << image.format();
        switch (image.format())
        {
        case QImage::Format_ARGB32:
        case QImage::Format_RGB32:
        case QImage::Format_ARGB32_Premultiplied:
            image = image.convertToFormat(QImage::Format_RGB888);
            Img = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
            
            break;
        case QImage::Format_RGB888:
        {
            Img = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
            
            //QImage Format_RGB888是按照R,G,B排布，  Mat按照B,G,R排布， 因而，需要进行互换；
            break;
        }
        case QImage::Format_Indexed8:
            Img = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
            break;
        }
        return Img;
    }
    QImage mat2img(cv::Mat mat) {
        switch (mat.type())
        {
            // 8位4通道
        case CV_8UC4:
        {
            QImage image(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB32);
            return image;
        }

        // 8位3通道
        case CV_8UC3:
        {
            QImage image(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
            return image.rgbSwapped();
        }

        // 8位单通道
        case CV_8UC1:
        {
            static QVector<QRgb>  sColorTable;
            // only create our color table once
            if (sColorTable.isEmpty())
            {
                for (int i = 0; i < 256; ++i)
                    sColorTable.push_back(qRgb(i, i, i));
            }
            QImage image(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Indexed8);
            image.setColorTable(sColorTable);
            return image;
        }

        default:
            qDebug("Image format is not supported: depth=%d and %d channels\n", mat.depth(), mat.channels());
            break;
        }
        return QImage();
    }
    static void onChange(int pos, void* userInput) {
        *(int*)userInput = pos;
    }
    static void onMouse(int event, int x, int y, int, void* userInput) {
        if (event == cv::EVENT_LBUTTONDBLCLK) {
            *(bool*)userInput = !*(bool*)userInput;
        }
    }
    cv::Mat showHistoCallback(cv::Mat img)
    {
        using namespace cv;
        // Separate image in BRG
        std::vector<Mat> bgr;
        split(img, bgr);

        // Create the histogram for 256 bins
        // The number of possibles values
        int numbins = 256;

        /// Set the ranges ( for B,G,R) )
        float range[] = { 0, 256 };
        const float* histRange = { range };

        Mat b_hist, g_hist, r_hist;

        calcHist(&bgr[0], 1, 0, Mat(), b_hist, 1, &numbins, &histRange);
        calcHist(&bgr[1], 1, 0, Mat(), g_hist, 1, &numbins, &histRange);
        calcHist(&bgr[2], 1, 0, Mat(), r_hist, 1, &numbins, &histRange);

        // Draw the histogram
        // We go to draw lines for each channel
        int width = 512;
        int height = 300;
        // Create image with gray base
        Mat histImage(height, width, CV_8UC3, Scalar(20, 20, 20));

        // Normalize the histograms to height of image
        normalize(b_hist, b_hist, 0, height, NORM_MINMAX);
        normalize(g_hist, g_hist, 0, height, NORM_MINMAX);
        normalize(r_hist, r_hist, 0, height, NORM_MINMAX);

        int binStep = cvRound((float)width / (float)numbins);
        for (int i = 1; i < numbins; i++)
        {
            line(histImage,
                Point(binStep * (i - 1), height - cvRound(b_hist.at<float>(i - 1))),
                Point(binStep * (i), height - cvRound(b_hist.at<float>(i))),
                Scalar(255, 0, 0)
            );
            line(histImage,
                Point(binStep * (i - 1), height - cvRound(g_hist.at<float>(i - 1))),
                Point(binStep * (i), height - cvRound(g_hist.at<float>(i))),
                Scalar(0, 255, 0)
            );
            line(histImage,
                Point(binStep * (i - 1), height - cvRound(r_hist.at<float>(i - 1))),
                Point(binStep * (i), height - cvRound(r_hist.at<float>(i))),
                Scalar(0, 0, 255)
            );
        }

        imshow("Histogram", histImage);

    }

    cv::Mat equalizeCallback(cv::Mat img)
    {   
        using namespace cv;
        Mat result;
        // Convert BGR image to YCbCr
        Mat ycrcb;
        cvtColor(img, ycrcb, COLOR_BGR2YCrCb);

        // Split image into channels
        std::vector<Mat> channels;
        split(ycrcb, channels);

        // Equalize the Y channel only
        equalizeHist(channels[0], channels[0]);

        // Merge the result channels
        merge(channels, ycrcb);

        // Convert color ycrcb to BGR
        cvtColor(ycrcb, result, COLOR_YCrCb2BGR);

        // Show image
        imshow("Equalized", result);
    }
   
};
