#include "MainControll.h"

#include <stdio.h>

#include "GL/glew.h"
#include "glut.h"
#include "test/test.h"

MainControll::MainControll(int argc, char ** argv):
_argc(argc),
_argv(argv)
{
}

MainControll::~MainControll()
{
}

void MainControll::start()
{
	// 初始化GLUT
	glutInit(&_argc, _argv);

	// 显示模式：双缓冲、RGBA
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	// 窗口设置
	glutInitWindowSize(400, 400);      // 窗口尺寸
	glutInitWindowPosition(100, 100);  // 窗口位置
	glutCreateWindow("Tutorial 03");   // 窗口标题

	test *t = new test();
	// 开始渲染
	glutDisplayFunc(&test::RenderScenceCB);
	// 将渲染回调注册为全局闲置回调
	glutIdleFunc(&test::RenderScenceCB);
	glutPostRedisplay();

	// 缓存清空后的颜色值
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// 检查GLEW是否就绪，必须要在GLUT初始化之后！
	GLenum res = glewInit();
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return;
	}

	// 创建顶点缓冲器
	t->CreateVertexBuffer();
	// 创建索引缓冲器
	t->CreateIndexBuffer();

	// 编译着色器
	t->CompileShaders();

	// 通知开始GLUT的内部循环
	glutMainLoop();
}

void MainControll::stop()
{
}
