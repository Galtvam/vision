#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

int main(){
    cv::Mat imagemOriginal;
    imagemOriginal = cv::imread("example.jpg", CV_LOAD_IMAGE_COLOR);

    if (!imagemOriginal.data){
        std::cout << "Falha no load da imagem" << std::endl;
        return -1;
    }

    cv::Mat imagemDestino;
    cv::GaussianBlur(imagemOriginal, imagemDestino, cv::Size(5, 5), 0, 0);
    // args:  (imagemOriginal, imagemDestino, tamanhoTerminal "padrão cv::Size(5, 5)", desvioPadraoX, desvioPadraoY)

    cv::namedWindow("demo1", CV_WINDOW_AUTOSIZE);
    cv::imshow("demo1", imagemOriginal);

    cv::namedWindow("demo2", CV_WINDOW_AUTOSIZE);
    cv::imshow("demo2", imagemDestino);

    cv::waitKey(0);
    return 0;
}
