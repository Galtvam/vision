#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>

using namespace std;

cv::VideoCapture videoLoad;
cv::Mat frame;

cv::Mat imagemRuido, imagemFinal, imagemTeste;

int main(){
    videoLoad.open("Troca_de_goleiro.mp4");
    //if(!videoLoad.isOpened()){
    //    cout << "Erro ao carregar o vídeo" << endl;
    //    return -1;
   // }

    cv::namedWindow("tela", CV_WINDOW_AUTOSIZE);
    for( ; ;){
        videoLoad >> frame;
        if (frame.empty()){
            break;
        }

        // redução de ruido
        cv::GaussianBlur(frame, imagemRuido, cv::Size(5,5), 0, 0);
        //cv::blur(frame, imagemRuido, cv::Size(3,3));
        
        cv::Canny(imagemRuido, imagemTeste, 80, 200, 3);
        cv::inRange(imagemRuido, cv::Scalar(160,80,0), cv::Scalar(255,255,95), imagemFinal);

        cv::imshow("tela", frame);
        cv::imshow("tela2", imagemTeste);
        cv::imshow("janela", imagemFinal);
        cv::waitKey(20);
    }
    cv::waitKey(0);

    return 0;
}
