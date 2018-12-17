#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>

using namespace std;

cv::Mat imagem, imagemRuido, imagemFinal;

int main(){
    // Carregamento da imagem
    imagem = cv::imread("teste1.jpg", CV_LOAD_IMAGE_COLOR);
    // Checagem de erro na leitura
    if (!imagem.data){
       cout << "Erro leitura" << endl;
       return -1;
    }
    
    // Conversão BGR to YUV
    cv::cvtColor(imagem, imagem, CV_BGR2YUV);

    // Redução de ruido
    cv::GaussianBlur(imagem, imagemRuido, cv::Size(3,3), 0,0);

    // Segmentação da cor Azul no formato YUV
    cv::inRange(imagemRuido, cv::Scalar(0,20,0), cv::Scalar(170,255,100), imagemFinal);   

    // Exibição
    cv::namedWindow("Segmentação", CV_WINDOW_AUTOSIZE);
    cv::imshow("Segmentação", imagemFinal);
    cv::imshow("Original", imagem);
    cv::waitKey(0);
    
    return 0;
}