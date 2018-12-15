#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

cv::Mat imagem, imagemDestino, imagemGray, detected_edges;

int edgeThresh = 1;
int lowThreshold;
int const max_lowThreshold = 100;
int ratio = 3;
int kernel_size = 3;

void CannyThreshold(int, void*){

    cv::GaussianBlur(imagemGray, detected_edges, cv::Size(5,5),0,0);
    //cv::blur(imagemGray, detected_edges, cv::Size(5,5));

    cv::Canny(detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size);

    imagemDestino = cv::Scalar::all(0);

    imagem.copyTo(imagemDestino, detected_edges);
    cv::imshow("janela", imagemDestino);
}


int main(){
    // carregamento da imagem
    imagem = cv::imread("teste1.jpg", CV_LOAD_IMAGE_COLOR);
    // bloco de validação do load
    if (!imagem.data){
        std::cout << "Falha ao carregar a imagem" << std::endl;
        return -1;
    }

    imagemDestino.create(imagem.size(), imagem.type());

    cv::cvtColor(imagem, imagemGray, CV_BGR2GRAY);

    cv::namedWindow("janela", CV_WINDOW_AUTOSIZE);

    cv::createTrackbar("trackbar name", "janela", &lowThreshold, max_lowThreshold, CannyThreshold);

    CannyThreshold(0,0);

    while(true){
        int c;
        c = cv::waitKey(20);
        if ((char)c == 27){
            break;
        }
    }

    return 0;
}
