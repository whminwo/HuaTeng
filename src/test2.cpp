#include <stdio.h>
#include <stdlib.h>

#include "CameraApi.h"
#include "CameraDefine.h"
#include "CameraStatus.h"

#include <opencv2/opencv.hpp>

unsigned char * g_pRgbBuffer; // RGB 버퍼

int main() {
    int iCameraCounts = 1; // 카메라 개수 초기화
    int iStatus = -1; // 상태 변수 초기화
    tSdkCameraDevInfo tCameraEnumList; // 카메라 열거 목록
    int hCamera; // 카메라 핸들
    tSdkCameraCapbility tCapability; // 카메라 능력
    tSdkFrameHead sFrameInfo; // 프레임 정보
    BYTE* pbyBuffer; // 이미지 버퍼 포인터
    int iDisplayFrames = 10000; // 디스플레이할 프레임 수
    int channel = 3; // 이미지 채널 수 (기본값: 컬러 이미지)

    // 카메라 SDK 초기화
    CameraSdkInit(1);
    // 연결된 카메라 장치 수 열거
    iStatus = CameraEnumerateDevice(&tCameraEnumList, &iCameraCounts);
    printf("state = %d\n", iStatus); // 상태 출력

    printf("count = %d\n", iCameraCounts); // 카메라 개수 출력

    // 연결된 카메라가 없을 경우
    if (iCameraCounts == 0) {
        return -1; // 프로그램 종료
    }

    // 카메라 초기화
    iStatus = CameraInit(&tCameraEnumList, -1, -1, &hCamera);
    printf("state = %d\n", iStatus); // 상태 출력
    if (iStatus != CAMERA_STATUS_SUCCESS) {
        return -1; // 프로그램 종료
    }
    tSdkImageResolution Resolution = { 0 }; 

    // 카메라 능력 획득
    CameraGetCapability(hCamera, &tCapability);

    // 자동 노출 모드 비활성화
    CameraSetAeState(hCamera, false);

    // RGB 버퍼 메모리 할당
    g_pRgbBuffer = (unsigned char*)malloc(tCapability.sResolutionRange.iHeightMax * tCapability.sResolutionRange.iWidthMax * 3);

    // 카메라 실행
    CameraPlay(hCamera);

    CameraSetRotate(hCamera,2);
    CameraSetMirror(hCamera,0,1);
    CameraSetMirror(hCamera,1,0);

    if(tCapability.sIspCapacity.bMonoSensor){
        channel=1;
        CameraSetIspOutFormat(hCamera,CAMERA_MEDIA_TYPE_MONO8);
    }else{
        channel=3;
        CameraSetIspOutFormat(hCamera,CAMERA_MEDIA_TYPE_BGR8);
    }


    while(iDisplayFrames--)
    {
        if(CameraGetImageBuffer(hCamera,&sFrameInfo,&pbyBuffer,1000) == CAMERA_STATUS_SUCCESS)
		{
		    CameraImageProcess(hCamera, pbyBuffer, g_pRgbBuffer,&sFrameInfo);

            cv::Size imageSize(sFrameInfo.iWidth, sFrameInfo.iHeight);

            // OpenCV Mat 객체로 변환
            cv::Mat matImage(imageSize, sFrameInfo.uiMediaType == CAMERA_MEDIA_TYPE_MONO8 ? CV_8UC1 : CV_8UC3, g_pRgbBuffer);

			imshow("cam2", matImage);

            int key = cv::waitKey(5);

            CameraReleaseImageBuffer(hCamera,pbyBuffer);
            if (key == 27) {
            break;
            }
		}
    }

    
    // 카메라 종료
    CameraUnInit(hCamera);
    // RGB 버퍼 메모리 해제
    free(g_pRgbBuffer);
    return 0; // 프로그램 종료
}