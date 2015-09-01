// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//
#pragma once

#include <stdio.h>
#include <tchar.h>

#include <opencv2\opencv.hpp> //cv::VideoCapture

const int NUM_FRAMES = 15;
const int TIME_INTERVAL = 1000 / NUM_FRAMES;  // 计时器时间间隔


const int FPS = 15;//   视频存储——帧率
const int CODEC = CV_FOURCC('D', 'I', 'V', 'X');



// TODO: reference additional headers your program requires here
