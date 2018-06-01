#pragma once
class MainControll
{
public:
	MainControll(int argc, char ** argv);
	~MainControll();

	void start();
	void stop();

private:
	int _argc;
	char ** _argv;
};