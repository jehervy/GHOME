/*
 * AbstractModel.h
 *
 *  Created on: 22 janv. 2012
 *      Author: Xav
 */

#ifndef ABSTRACTMODEL_H_
#define ABSTRACTMODEL_H_

#include <stddef.h>
#include <pthread.h>

class AbstractModel{

private:

	static void *sCallback(void *a_pCtx) {
		((AbstractModel*)a_pCtx)->Run();
		return NULL;
	}

	virtual void Run() = 0;

public:
	AbstractModel(int a_iBal) : m_iBalCenter(a_iBal){}

	virtual ~AbstractModel(){}

	virtual void Start()
	{
		pthread_create(&m_thread, 0, AbstractModel::sCallback, (void *)this);
	}

	virtual void Stop()
	{
		pthread_cancel(m_thread);
	}

	virtual void Wait()
	{
		pthread_join(m_thread, NULL);
	}

protected:
	int m_iBalCenter;
	pthread_t m_thread;



};

#endif /* ABSTRACTMODEL_H_ */
