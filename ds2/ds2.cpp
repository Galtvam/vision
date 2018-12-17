#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>

using namespace std;

// carregamento do vídeo
cv::VideoCapture videoLoad;
cv::Mat originalCapture, frame;

// Mascaras de efeito
cv::Mat imagemVerde, imagemRosa;

// Cores segmentadas na imagem
cv::Scalar verdeMin = cv::Scalar(0,150,0) , verdeMax = cv::Scalar(140,255,100);
cv::Scalar rosaMin = cv::Scalar(160,140,160) , rosaMax = cv::Scalar(255,160,255);

// Distancia euclidiana de colapso
const int distMax = 15;

// Funções
void gerarTracking(cv::Mat imagemFinal);
float distanciaEuclidiana(int x1, int y1, int x2, int y2);
bool checarCor(cv::Mat imagem, int x, int y, cv::Scalar corMin, cv::Scalar corMax);
vector<cv::Point2f> convergirCentros(cv::Mat imagem, vector<cv::Point2f> centros);


int main(){
    videoLoad.open("Troca_de_goleiro.mp4");
    if(!videoLoad.isOpened()){
        // Checagem de erros ao carregar vídeo
        cout << "Erro ao carregar o vídeo" << endl;
        return -1;
    }

    // Janela de Tracking
    cv::namedWindow("Tracking", CV_WINDOW_AUTOSIZE);
    // Obtenção dos frames
    for( ; ;){

        // Mascaras de filtragem
        cv::Mat imagemInteresse, imagemFinal;

        // Obtenção do frame
        videoLoad >> originalCapture;
        if (originalCapture.empty()){
            break;
        }

        //  Conversão do frame obtido em BGR para YUV
        cv::cvtColor(originalCapture, frame, CV_BGR2YUV);
        //  Redução de ruido
        cv::GaussianBlur(frame, frame, cv::Size(3,3), 0, 0);
        
        //  Mascara verde
        cv::inRange(frame, verdeMin, verdeMax, imagemVerde);
        //  Mascara rosa
        cv::inRange(frame, rosaMin, rosaMax, imagemRosa);

        // Junção das mascaras
        cv::bitwise_or(imagemVerde, imagemRosa, imagemInteresse);
        // Segmentação do frame pela região de interesse
        cv::bitwise_and(frame, frame, imagemFinal, imagemInteresse);

        gerarTracking(imagemFinal);
    }
    cv::waitKey(0);

    return 0;
}


void gerarTracking(cv::Mat imagem){
        //  Mascara canny
        cv::Mat imagemCanny;
        // Arrays adicionais
        vector<vector<cv::Point> > contornos;
        vector<cv::Vec4i> hierarquia;

        // Obter bordas
        cv::Canny(imagem, imagemCanny, 10, 20, 3);
        // Obter contornos
        cv::findContours( imagemCanny, contornos, hierarquia, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

        //  Gerar momentos
        vector<cv::Moments> mu(contornos.size());
        for(int i = 0; i < contornos.size(); i++ ){
            mu[i] = cv::moments(contornos[i], false); 
        }

        // Obter centros de massa
        vector<cv::Point2f> mc(contornos.size());
        for(int i = 0; i < contornos.size(); i++ ){
            mc[i] = cv::Point2f(mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00);
        }

        // Novo vetor que comportará os centros de massa colididos
        vector<cv::Point2f> centrosColididos(contornos.size());
        centrosColididos = convergirCentros(frame, mc);

        // Desenhar os centros de massa
        cv::Mat drawing = cv::Mat::zeros( imagemCanny.size(), CV_8UC3 );
        for( int i = 0; i< contornos.size(); i++ ){
            if(centrosColididos[i].x != 0 && centrosColididos[i].y != 0){
                cv::circle(originalCapture, centrosColididos[i], 7, cv::Scalar(255,0,0), -1, 8, 0);
                }
            }

        // Mostra o tracking
        cv::imshow("Tracking", originalCapture);
        cv::waitKey(20);
}


vector<cv::Point2f> convergirCentros(cv::Mat imagem ,vector<cv::Point2f> centros){
    // Cordenadas dos dois centros de massa
    int cordenadaX, cordenadaY, cordenadaX2, cordenadaY2;
    // Variaveis de auxilio
    float dist; bool valid;
    // Vetor que será retornado
    vector<cv::Point2f> colisionVector(centros.size());

    for(int i=0; i<centros.size() - 1; i++){
        cordenadaX = centros[i].x;
        cordenadaY = centros[i].y;
        if(cordenadaX >= 0 && cordenadaY >= 0){  // Cheque de corversão float to int foi válida
            for(int k=i; k<centros.size(); k++){
                cordenadaX2 = centros[k].x;
                cordenadaY2 = centros[k].y;
                if(cordenadaX2 >= 0 && cordenadaY2 >= 0){  // Cheque de corversão float to int foi válida
                    dist = distanciaEuclidiana(cordenadaX, cordenadaY, cordenadaX2, cordenadaY2);
                    // Checagem da cor dos dois centros " i " e " k " se correspondem as cores desejadas
                    valid = (
                        ( checarCor(imagem, cordenadaX, cordenadaY, verdeMin, verdeMax) &&
                        checarCor(imagem, cordenadaX2, cordenadaY2, rosaMin, rosaMax) ) 
                        || 
                        ( checarCor(imagem, cordenadaX2, cordenadaY2, verdeMin, verdeMax) &&
                        checarCor(imagem, cordenadaX, cordenadaY, rosaMin, rosaMax) )
                    );
                    if(dist <= distMax && valid){  // caso a distancia esteja dentro do limiar e as cores correspondam é feito o colapso
                        colisionVector[i] = cv::Point2f(0,0);
                        // Nova cordenada corresponderá a média da localização dos centros de massa
                        colisionVector[k] = cv::Point2f(int((cordenadaX+cordenadaX2)/2), int((cordenadaY+cordenadaY2)/2));
                        break;
                    }
                }
            }
        }
    }
    return colisionVector;
}


float distanciaEuclidiana(int x1, int y1, int x2, int y2){
    float distancia;
    // ( (x1-x2)^2 + (y1-y2)^2 ) ^ 0.5
    distancia = pow( 
        ( pow( (x1 - x2), 2) + pow( (y1-y2), 2 ) )
         , 0.5);
    return distancia;
}


bool checarCor(cv::Mat imagem, int x, int y, cv::Scalar corMin, cv::Scalar corMax){
    // Mandando o ponteiro ser direcionado para coluna y da linha x da imagem
    uchar *pixel = imagem.ptr<uchar>(y, x);
    // 0 = B 
    // 1 = G 
    // 2 = R
    if(pixel[0] >= corMin[0] && pixel[1] >= corMin[1] && pixel[2] >= corMin[2]){
        if(pixel[0] <= corMax[0] && pixel[1] <= corMax[1] && pixel[2] <= corMax[2]){
            return true;
        }else{
            return false;
        }
    }else{
        return false;
    }
}