#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <time.h>

using namespace std;

typedef struct RGB{
    uchar blue;
    uchar green;
    uchar red;
} RGB;

cv::Mat imagem, imagemRuido, imagemFinal;

int main(){
    clock_t tempo;
	tempo = clock();

    imagem = cv::imread("teste1.jpg", CV_LOAD_IMAGE_COLOR);
    
    if (!imagem.data){
       cout << "Erro leitura" << endl;
       return -1;
    }
    
    // redução de ruido
    cv::GaussianBlur(imagem, imagemRuido, cv::Size(5,5), 0,0);
    //cv::blur(imagem, imagemRuido, cv::Size(3,3));

    cv::inRange(imagemRuido, cv::Scalar(200,90,0), cv::Scalar(255,255,100), imagemFinal);

    //cv::namedWindow("teste", CV_WINDOW_AUTOSIZE);
    //cv::imshow("teste", imagemFinal);
    cv::imwrite("save.jpg", imagemFinal);
    //cv::imshow("janela", imagem);
    //cv::waitKey(0);

    printf("Tempo:%f",(clock() - tempo) / (double)CLOCKS_PER_SEC);
    
    return 0;
}