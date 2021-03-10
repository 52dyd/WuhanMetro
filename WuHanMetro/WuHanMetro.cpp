// WuHanMetro.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include"station.h"
#include"metro.h"
#include"people.h"
#include"dest.h"
#include"edges.h"
#include"answer.h"
#include"map.h"

int main()
{
    std::freopen("out.txt", "r", stdin);
    Map map;
    map.generate_person(NULL, NULL, 1);
    std::vector<Stations*> tmp_stations;
    tmp_stations = map.get_station();
    for (int i = 0; i < 100; i++) {
        std::cout << i << std::endl;
        map.nxt_scd_thread();
        for (auto iter = tmp_stations.begin(); iter != tmp_stations.end(); iter++) 
            std::cout << (*iter)->get_name() << ':' << (*iter)->get_num_people() << std::endl;
    }
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
