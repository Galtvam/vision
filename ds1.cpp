#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace std;

typedef struct RGB{
    uchar blue;
    uchar green;
    uchar red;
} RGB;

cv::Mat imagem, imagemRuido, imagemVerde;

int main(){
    imagem = cv::imread("teste1.jpg", CV_LOAD_IMAGE_COLOR);
    
    if (!imagem.data){
        cout << "Erro leitura" << endl;
        return -1;
    }
    
    // redução de ruido
    cv::GaussianBlur(imagem, imagemRuido, cv::Size(5,5), 0,0);

    RGB *pixelColor;
    for (int i=0; i<imagem.rows; i++){
        for (int j=0; j<imagem.cols; j++){
            pixelColor = &(imagemRuido.ptr<RGB>(i)[j]);
            if(pixelColor->blue < 200 || pixelColor->red > 120 || pixelColor->green > 150){
                pixelColor->green = 0;
                pixelColor->red = 0;
                pixelColor->blue = 0;
            }else{
                pixelColor->green = 255;
                pixelColor->red = 255;
                pixelColor->blue = 255;
            }
        }
    }

    cv::namedWindow("teste", CV_WINDOW_AUTOSIZE);
    cv::imshow("teste", imagemRuido);
    //cv::imwrite("save.jpg", imagemRuido);
    cv::imshow("janela", imagem);
    cv::waitKey(0);

    return 0;
}