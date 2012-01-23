/*
 * AbstractModel.h
 *
 *  Created on: 22 janv. 2012
 *      Author: miroof
 */

#ifndef ABSTRACTMODEL_H_
#define ABSTRACTMODEL_H_

#include <pthread.h>
#include <iostream>
#include <string>
#include <cstdio>

class AbstractModel{

private:

	static void *callback(void *ctx) {
		//AbstractModel *model = static_cast<AbstractModel*> (ctx);
		//model->run();
		((AbstractModel*)ctx)->run();
		return NULL;
	}

	virtual void run() = 0;

public:
	AbstractModel(int bal) : balCenter(bal){}

	virtual ~AbstractModel(){}

	virtual void start()
	{
		pthread_create(&thread, 0, AbstractModel::callback, (void *)this);
	}

	virtual void stop()
	{
		pthread_cancel(thread);
	}

protected:
	int balCenter;
	pthread_t thread;



};

#endif /* ABSTRACTMODEL_H_ */
