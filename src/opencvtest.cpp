#include <opencv2/opencv.hpp>
#include <iostream>
int main() {
    // 이미지 파일 경로
    std::string imagePath = "example.jpg";
    
    // 이미지 로드
    cv::Mat image = cv::imread(imagePath);
// 이미지가 제대로 로드되었는지 확인
if (!image.data) {
    std::cout << "이미지를 읽을 수 없습니다." << std::endl;
    return -1;
}
// 윈도우 생성 및 이미지 표시
cv::namedWindow("Image", cv::WINDOW_NORMAL); // 윈도우 생성
cv::imshow("Image", image); // 이미지 표시
// 사용자가 키를 입력할 때까지 대기
cv::waitKey(0);
// 윈도우 종료
cv::destroyAllWindows();
return 0;
}