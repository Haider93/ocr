#pragma once
#include <stdio.h>
#include <iostream>

using namespace std;

class ActionInput {

public:
	//ctor
	ActionInput(string uuid = ""):m_uuid(uuid){}
	//dtor
	virtual ~ActionInput(){}
	// Disable copying and assignment
	ActionInput(const ActionInput &other) {}
private:
	string m_uuid;
};