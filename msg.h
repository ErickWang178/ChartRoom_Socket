#ifndef MSG_H
#define MSG_H

#include <iostream>
#include <string>

using namespace std;

class Msg{
private:
	static string msg;

public:
	static void printPrompt(string msg);
	static void exitPrompt(string msg,int code);

};

#endif