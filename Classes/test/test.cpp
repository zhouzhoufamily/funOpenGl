#include "test.h"

#include <string>
#include <fstream>

#include "sdk/math/math3d.h"
#include "sdk/util/util.h"

static GLuint _vbo;				// 顶点缓冲对象的句柄

static GLuint _ibo;				// 索引缓冲对象的句柄

static GLuint gScaleLocation;	// 位置中间变量

static GLuint gWorldLocation;	// 平移变换一致变量的句柄引用

// 定义要读取的顶点着色器脚本和片断着色器脚本的文件名，作为文件读取路径（这样的话shader.vs和shader.fs文件要放到工程的根目录下，保证下面定义的是这两个文件的文件路径）
const char* pVSFileName = "shader.vs";
const char* pFSFileName = "shader.fs";

test::test()
{
}


test::~test()
{
}

bool test::init()
{
	// 初始化GLUT
	//glutInit(&argc, argv);

	//// 显示模式：双缓冲、RGBA
	//glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	//// 窗口设置
	//glutInitWindowSize(480, 320);      // 窗口尺寸
	//glutInitWindowPosition(100, 100);  // 窗口位置
	//glutCreateWindow("Tutorial 03");   // 窗口标题

	return false;
}

void test::show()
{
}

/**
* 渲染回调函数
*/
void test::RenderScenceCB()
{
	// 清空颜色缓存
	glClear(GL_COLOR_BUFFER_BIT);

	// 维护一个不断慢慢增大的静态浮点数
	static float Scale = 0.0f;
	Scale += 0.0001f;
	// 将值传递给shader
	//glUniform1f(gScaleLocation, sinf(Scale));

	M3DMatrix44f World = {
		cosf(Scale), 0.0f, -sinf(Scale), 0.0f,
		sinf(Scale), 1.0f, cosf(Scale), 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
	// 将矩阵数据加载到shader中
	glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, &World[0]);

	// 开启顶点属性
	glEnableVertexAttribArray(0);
	// 绑定GL_ARRAY_BUFFER缓冲器
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	// 告诉管线怎样解析bufer中的数据
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// 每次在绘制之前绑定索引缓冲
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
	// 索引绘制图形
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

	// 开始绘制几何图形(绘制一个三角形，三个顶点)
	glDrawArrays(GL_TRIANGLES, 0, 3);

	//  禁用顶点数据
	glDisableVertexAttribArray(0);

	// 交换前后缓存
	glutSwapBuffers();

	glFlush();
}

/**
* 创建顶点缓冲器
*/
void test::CreateVertexBuffer()
{
	float scale = 0.6f;
	// 创建含有3个顶点的顶点数组

	/*M3DMatrix33f Vertices = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f 
	};*/

	float Vertices[12] = {
		-1.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	/*for (int i = 0; i < 9; i++)
	{
		Vertices[i] *= scale;
	}*/
	//M3DVector3f Vertices[3];
	// 三角形的三个顶点位置
	/*Vertices[0] = M3DVector3f(-1.0f, -1.0f, 0.0f);
	Vertices[1] = M3DVector3f(1.0f, -1.0f, 0.0f);
	Vertices[2] = M3DVector3f(0.0f, 1.0f, 0.0f);*/

	// 创建缓冲器
	glGenBuffers(1, &_vbo);
	// 绑定GL_ARRAY_BUFFER缓冲器
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	// 绑定顶点数据
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
}

void test::CreateIndexBuffer()
{
	// 四个三角形面的顶点索引集
	unsigned int Indices[] = { 0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2 };
	// 创建缓冲区
	glGenBuffers(1, &_ibo);
	// 绑定缓冲区
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
	// 添加缓冲区数据
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
}

void test::CompileShaders()
{
	// 创建着色器程序
	GLuint ShaderProgram = glCreateProgram();
	// 检查是否创建成功
	if (ShaderProgram == 0) {
		fprintf(stderr, "Error creating shader program\n");
		exit(1);
	}

	// 存储着色器文本的字符串缓冲
	std::string vs, fs;
	// 分别读取着色器文件中的文本到字符串缓冲区

	if (!util::ReadFileToString(pVSFileName, vs)) {
		exit(1);
	};
	if (!util::ReadFileToString(pFSFileName, fs)) {
		exit(1);
	};

	// 添加顶点着色器和片段着色器
	AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);
	AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

	// 链接shader着色器程序，并检查程序相关错误
	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };
	glLinkProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
	if (Success == 0) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
		exit(1);
	}

	// 检查验证在当前的管线状态程序是否可以被执行
	glValidateProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
		exit(1);
	}

	// 设置到管线声明中来使用上面成功建立的shader程序
	glUseProgram(ShaderProgram);

	// 查询获取一致变量的位置
	//gScaleLocation = glGetUniformLocation(ShaderProgram, "gScale");
	// 检查错误
	//assert(gScaleLocation != 0xFFFFFFFF);

	gWorldLocation = glGetUniformLocation(ShaderProgram, "gWorld");
	assert(gWorldLocation != 0xFFFFFFFF);
}

void test::AddShader(GLuint ShaderProgram, const char * pShaderText, GLenum ShaderType)
{
	// 根据shader类型参数定义两个shader对象
	GLuint ShaderObj = glCreateShader(ShaderType);
	// 检查是否定义成功
	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
		exit(0);
	}

	// 定义shader的代码源
	const GLchar* p[1];
	p[0] = pShaderText;
	GLint Lengths[1];
	Lengths[0] = strlen(pShaderText);
	glShaderSource(ShaderObj, 1, p, Lengths);
	glCompileShader(ShaderObj);// 编译shader对象

	// 检查和shader相关的错误
	GLint success;
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		OutputDebugString((LPCWSTR)InfoLog);
		exit(1);
	}

	// 将编译好的shader对象绑定到program object程序对象上
	glAttachShader(ShaderProgram, ShaderObj);
}
