#include <stdio.h>

#include "GL/glew.h"
#include "glut.h"

#pragma once

using namespace std;

class test
{
public:
	test();
	~test();

	bool init();
	void show();

	/**
	* 渲染回调函数
	*/
	static void RenderScenceCB();

	/**
	* 创建顶点缓冲器
	*/
	void CreateVertexBuffer();

	/**
	* 创建索引缓冲器
	*/
	void CreateIndexBuffer();

	/**
	* 编译着色器
	*/
	void CompileShaders();

	/**
	* 使用shader文本编译shader对象，并绑定shader都想到着色器程序中
	*/
	void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType);
};

