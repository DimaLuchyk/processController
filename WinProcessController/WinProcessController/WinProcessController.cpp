#include <iostream>
#include <string>

#include "Controller.h"

int main()
{
    //Controller controller(L"WpfETTestClient.exe");
    Controller controller(L"C:\\myFiles\\processController\\WinProcessController\\WinProcessController\\ETTestClient\\WpfETTestClient.exe");
    bool res = controller.StartProcess();
    controller.ThreadEntry();
    return 0;
}