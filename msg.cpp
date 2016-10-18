#include "msg.h"

string Msg::msg = "";

void Msg::printPrompt(string msg){
	cout << msg << endl;
}

void Msg::exitPrompt(string msg,int code){
	cout << msg << ":" << code << endl;
	//exit(code);
}

