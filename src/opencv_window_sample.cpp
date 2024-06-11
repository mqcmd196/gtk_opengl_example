#include <opencv2/opencv.hpp>
#include <opencv2/core/opengl.hpp>
#include <opencv2/highgui.hpp>
#include <GL/gl.h>

void on_opengl(void* param){
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-0.5f, -0.5f);
    glVertex2f( 0.5f, -0.5f);
    glVertex2f( 0.5f,  0.5f);
    glVertex2f(-0.5f,  0.5f);
    glEnd();
}


int main(){
    int width = 640, height = 480;
    cv::Mat image = cv::Mat(height, width, CV_8UC3, cv::Scalar(255, 0, 0));

    cv::namedWindow("OpenGL", cv::WINDOW_OPENGL);
    cv::resizeWindow("OpenGL", width, height);

    cv::setOpenGlContext("OpenGL");
    cv::setOpenGlDrawCallback("OpenGL", on_opengl);

    while(true){
        cv::imshow("OpenGL", image);
        if(cv::waitKey(30) >= 0) break;
    }

    cv::setOpenGlDrawCallback("OpenGL", 0);
    cv::destroyAllWindows();

    return 0;
}
