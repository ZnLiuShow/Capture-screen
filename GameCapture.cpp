// GameCapture.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include<Windows.h>
#include <opencv2/opencv.hpp>
#include "W_Screenshot.h"

Screenshot tsv;
int main()
{
    HWND hwndDesktop = GetDesktopWindow();
    cv::namedWindow("output", cv::WINDOW_NORMAL);
    int key = 0;

    while (key != 27)
    {
        int r = GetTickCount64();
        //cv::Mat src = hwnd2mat(hwndDesktop);
        cv::Mat src = tsv.getScreenshot();
        // you can do some image processing here
        cv::imshow("output", src);
        std::cout << GetTickCount64() - r << std::endl;
        key = cv::waitKey(60); // you can change wait time
    }

    std::cout << "Hello World!\n";
    return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
