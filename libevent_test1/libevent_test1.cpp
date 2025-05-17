// libevent_test1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <event2/event.h>

int main()
{
    //std::cout << "Hello World!\n";
    struct event_base* base = event_base_new();
    if (!base) {
        fprintf(stderr, "Could not initialize libevent!\n");
        return 1;
    }
    struct sockaddr_in sin = { 0 };

    return 0;
}

