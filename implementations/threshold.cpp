#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

cv::Mat imagem, imagemGray, imagemDestino;
int thresholdValue = 0;

void applyThreshold(int, void*){
    cv::threshold( imagemGray, imagemDestino, thresholdValue, 255, cv::THRESH_BINARY);

    cv::imshow("tela", imagemDestino);
}

int main(){
    // carregamento da imagem
    imagem = cv::imread("teste1.jpg", CV_LOAD_IMAGE_COLOR);
    // bloco de validação do load
    if (!imagem.data){
        std::cout << "Falha ao carregar a imagem" << std::endl;
        return -1;
    }

    //cv::cvtColor(imagem, imagemGray, CV_BGR2GRAY);
    imagem.copyTo(imagemGray);
    cv::GaussianBlur(imagemGray, imagemGray, cv::Size(5,5),0,0);

    cv::imshow("janela", imagem);

    cv::namedWindow("tela", CV_WINDOW_AUTOSIZE);

    cv::createTrackbar("Trackbar name", "tela", &thresholdValue, 255, applyThreshold);

    applyThreshold(0, 0);

    while(true){
        int c;
        c = cv::waitKey(20);
        if ((char)c == 27){
            break;
        }
    }

    return 0;
}