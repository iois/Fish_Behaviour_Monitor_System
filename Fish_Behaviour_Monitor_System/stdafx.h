// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//
#pragma once

#include <stdio.h>
#include <tchar.h>
#include <fstream>
#include <string>

#include <opencv2\opencv.hpp> //cv::VideoCapture

const int NUM_FRAMES = 10;
const int TIME_INTERVAL = 1000 / NUM_FRAMES;  // ��ʱ��ʱ����


const int FPS = NUM_FRAMES;//   ��Ƶ�洢����֡��
const int CODEC = CV_FOURCC('D', 'I', 'V', 'X');

const int REMAIN_STORAGE = 5;//5G����

const std::string LOG_FILE_NAME = "log.dat";

// TODO: reference additional headers your program requires here
