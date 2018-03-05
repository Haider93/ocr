#pragma once
#include <stdio.h>
#include <iostream>
#include "ActionInput.h"
#include "ActionOutput.h"
#include <time.h>
using namespace std;

template<class ActionInp, class ActionOut>
class Action {
public:
	virtual ActionOut* execute(ActionInp *actionInput)= 0;

	void setTimeToExecute(double timetoExecute){
		this->m_timetoExecute = timetoExecute;
	}

	void startClock(){
		this->m_start_time = clock();
	}
	void endClock()
	{
		this->m_end_time = clock();
		double time_taken=(m_end_time-m_start_time)/double(CLOCKS_PER_SEC);
		this->setTimeToExecute(time_taken);	
	}

	double getExecutionTime()
	{
		return this->m_timetoExecute;
	}

private:
	double m_timetoExecute;
	int m_start_time;
	int m_end_time;
};
