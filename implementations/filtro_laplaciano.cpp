#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

int main(){
    cv::Mat imagemOriginal;
    imagemOriginal = cv::imread("teste1.jpg", CV_LOAD_IMAGE_COLOR);

    if (!imagemOriginal.data){
        std::cout << "falha ao carregar a imagem" << std::endl;
        return -1;
    }

    // aplicando filtro gaussiano para reduzir o ruido da imagem
    cv::GaussianBlur(imagemOriginal, imagemOriginal, cv::Size(5, 5), 0, 0);
    // transformando a cor da imagem para grayscale
    cv::cvtColor(imagemOriginal,imagemOriginal, CV_RGB2GRAY);

    // aplicando filtro passa-alto
    cv::Mat imagemDestino;
    cv::Laplacian(imagemOriginal, imagemDestino, CV_16S);

    // convertendo imagem para 8bits a fim de ficar visivel
    cv::convertScaleAbs(imagemDestino, imagemDestino);

    //  demonstração das imagens no terminal
    cv::namedWindow("demo1", CV_WINDOW_AUTOSIZE);
    cv::imshow("demo1", imagemOriginal);
    cv::namedWindow("demo2", CV_WINDOW_AUTOSIZE);
    cv::imshow("demo2", imagemDestino);

    cv::waitKey(0);
    return 0;
}
