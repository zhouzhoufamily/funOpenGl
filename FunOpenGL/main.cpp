// main.cpp: 定义控制台应用程序的入口点。
//
#include "main.h"
#include "MainControll.h"

/**
* 主函数
*/
int main(int argc, char ** argv) {

	MainControll *main = new MainControll(argc, argv);
	main->start();

	return 0;
}

