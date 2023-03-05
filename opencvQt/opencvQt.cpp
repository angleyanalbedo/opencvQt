#include "opencvQt.h"
#define tmppath "tmp.jpg"
#define oripath "ori.jpg"
opencvQt::opencvQt(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    
}

opencvQt::~opencvQt()
{}
void opencvQt::openimg() {
    QString filename = ui.lineEdit->text();
    
    if (filename.isEmpty()) {
        filename = QFileDialog().getOpenFileName(this, QString().fromStdWString(L"打开文件的"), 
            "C:\\Users\\33298\\OneDrive\\Pictures");
        
        if (filename.isEmpty()) {
            QMessageBox().warning(this, QString().fromStdWString(L"错误"), 
                QString().fromStdWString(L"打开文件失败"));
            return;
        }
        ui.lineEdit->setText(filename);
    }
    sourceimg = QImage(filename);
    tmpimg = QImage(filename);
    tmpimg.save(tmppath);
    sourceimg.save(oripath);
    ui.label->setPixmap(QPixmap(filename));
}
void opencvQt::saveimg() {
    if (tmpimg.isNull()) {
        QMessageBox().warning(this, QString().fromStdWString(L"图片缓存是空的"),
            QString().fromStdWString(L"请重试"));
        return;
    }
    QString filename = ui.lineEdit_2->text();

    if (filename.isEmpty()) {
        filename = QFileDialog().getSaveFileName(this, QString().fromStdWString(L"保存文件的"),
            "C:\\Users\\33298\\OneDrive\\Pictures");
        
        if (filename.isEmpty()) {
            QMessageBox().warning(this, QString().fromStdWString(L"错误"),
                QString().fromStdWString(L"保存文件失败"));
            return;
        }
        ui.lineEdit_2->setText(filename);
    }
    tmpimg.save(filename);
}
void opencvQt::blur() {
    //cv::Mat img = img2mat(tmpimg);
    cv::Mat img = cv::imread(tmppath);
    cv::Mat tmp = img;
    cv::cvtColor(img,tmp, cv::ColorConversionCodes::COLOR_BGR2RGB);
    cv::blur(tmp, img, cv::Size(3, 3));
    cv::imwrite(tmppath,tmp);
    tmpimg.load(tmppath);
    //tmpimg = mat2img(img);
    ui.label_2->setPixmap(QPixmap().fromImage(tmpimg));
}
void opencvQt::video_blur() {
    using namespace cv;
    bool flag = true;
    VideoCapture capture(0);
    Mat frame;
    namedWindow("camerablur", WINDOW_AUTOSIZE);
    setMouseCallback("camerablur", onMouse, &flag);
    while (capture.read(frame)) {
        if (flag) {
            cv::cvtColor(frame, frame, cv::ColorConversionCodes::COLOR_BGR2RGB);
            cv::blur(frame, frame, cv::Size(3, 3));
        }
        imshow("camerablur", frame);

        char c = waitKey(50);
        if (c == 27) {
            break;
        }
    }
    capture.release();

    waitKey(0);
}
void opencvQt::video_detect() {
    using namespace cv;
    bool flag = true;
    VideoCapture capture(0);
    Mat frame;
    namedWindow("cameraedgetect", WINDOW_AUTOSIZE);
    imshow("cameraedgetect", frame);
    while (capture.read(frame)) {
        if (flag) {
            cv::cvtColor(frame, frame, cv::ColorConversionCodes::COLOR_BGR2RGB);
            cv::Canny(frame, frame, 100, 300);
        }
        imshow("cameraedgetect", frame);

        char c = waitKey(50);
        if (c == 27) {
            break;
        }
    }
    capture.release();

    waitKey(0);
}
void opencvQt::video_gray() {
    using namespace cv;
    bool flag = true;
    VideoCapture capture(0);
    Mat frame;
    namedWindow("cameragray", WINDOW_AUTOSIZE);
    setMouseCallback("cameragray", onMouse, &flag);
    while (capture.read(frame)) {
        if (flag) {
            cv::cvtColor(frame, frame, cv::ColorConversionCodes::COLOR_BGR2GRAY);
        }
        imshow("cameragray", frame);
        char c = waitKey(50);
        if (c == 27) {
            break;
        }
    }
    capture.release();
    
    waitKey(0);
}
void opencvQt::gray() {
    
    //cv::Mat img = img2mat(tmpimg);
    cv::Mat img = cv::imread(tmppath);
    cv::cvtColor(img, img, cv::ColorConversionCodes::COLOR_BGR2GRAY);
    //tmpimg = mat2img(img);
    cv::imwrite(tmppath, img);
    tmpimg.load(tmppath);
    ui.label_2->setPixmap(QPixmap().fromImage(tmpimg));
}
void opencvQt::about(){
    QMessageBox().about(this, QString().fromStdWString(L"信息"), 
        QString().fromStdWString(L"学好信息"));
}
void opencvQt::edgedetect(){
    //cv::Mat img = img2mat(tmpimg);
    cv::Mat img = cv::imread(tmppath);
    cv::cvtColor(img, img, cv::ColorConversionCodes::COLOR_BGR2GRAY);
    cv::Canny(img, img, 100, 300);
    //tmpimg = mat2img(img);
    cv::imwrite(tmppath, img);
    tmpimg.load(tmppath);
    ui.label_2->setPixmap(QPixmap().fromImage(tmpimg));
}
void opencvQt::midblur(){
    cv::Mat img = cv::imread(tmppath);
    //cv::Mat img = img2mat(tmpimg);
    cv::cvtColor(img, img, cv::ColorConversionCodes::COLOR_BGR2GRAY);
    cv::medianBlur(img, img,3);
    cv::imwrite(tmppath, img);
    tmpimg.load(tmppath);
    //tmpimg = mat2img(img);
    ui.label_2->setPixmap(QPixmap().fromImage(tmpimg));
}
void opencvQt::showall(std::string name,QImage src) {
    cv::Mat tmp;
    //tmp = img2mat(src);
    tmp = cv::imread(tmppath);
    cv::imshow(name, tmp);
}
void opencvQt::showdst() {
    showall("dst", tmpimg);
}
void opencvQt::showsource() {
    showall("source", sourceimg);
}
void opencvQt::bin() {
    int num = 0;
    num = QInputDialog().getInt(this, QString().fromStdWString(L"阈值的输入"),
        QString().fromStdWString(L"输入数字"));
    if (num != 0) {
        cv::Mat img = img2mat(tmpimg);
        cv::cvtColor(img, img, cv::COLOR_BGR2GRAY);
        cv::Mat  gray_binary;

        //灰度图像二值化
        //THRESH_BINARY_INV--灰度大于阈值125则设为0, 其他值为最大
        cv::threshold(img, gray_binary, num, 255, cv::THRESH_BINARY_INV);
        tmpimg = mat2img(gray_binary);
        ui.label_2->setPixmap(QPixmap().fromImage(tmpimg));
    }
    else
    {
        QMessageBox().warning(this, QString().fromStdWString(L"错误"),
            QString().fromStdWString(L"输入的数字小于0"));
    }
}
void opencvQt::video_bin() {
    using namespace cv;
    bool flag = true;
    int num = 0;
    num = QInputDialog().getInt(this, QString().fromStdWString(L"阈值的输入"),
        QString().fromStdWString(L"输入数字"));
    if (num == 0)
        return;
    VideoCapture capture(0);
    Mat frame;
    namedWindow("camerabin", WINDOW_AUTOSIZE);
    createTrackbar("bintracker", "camerabin", &num, 255, onChange, &num);
    setMouseCallback("camerabin", onMouse,&flag);
    while (capture.read(frame)) {
        if (flag) {
            cv::cvtColor(frame, frame, cv::ColorConversionCodes::COLOR_BGR2RGB);
            cv::threshold(frame, frame, num, 255, cv::THRESH_BINARY_INV);
        }
        imshow("camerabin", frame);
        char c = waitKey(50);
        if (c == 27) {
            break;
        }
    }
    capture.release();

    waitKey(0);
}
void opencvQt::video_midblur() {
    using namespace cv;
    bool flag = true;
    VideoCapture capture(0);
    Mat frame;
    namedWindow("cameramidblur", WINDOW_AUTOSIZE);
    setMouseCallback("cameramidblur", onMouse, &flag);
    while (capture.read(frame)) {
        if (flag) {
            cv::cvtColor(frame, frame, cv::ColorConversionCodes::COLOR_BGR2RGB);
            cv::medianBlur(frame, frame, 3);
        }
        imshow("cameramidblur", frame);

        char c = waitKey(50);
        if (c == 27) {
            break;
        }
    }
    capture.release();

    waitKey(0);
}
void opencvQt::equlized() {

}
void opencvQt::caclHist() {

}