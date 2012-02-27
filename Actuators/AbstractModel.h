/*
 * AbstractModel.h
 *
 *  Created on: 22 janv. 2012
 *      Author: miroof
 */

#ifndef ABSTRACTMODEL_H_
#define ABSTRACTMODEL_H_

#include <pthread.h>

/*
 * Interface AbstractModel
 * Sert d'interface à tous les autres models
 */
class AbstractModel{

private:

	/*
	 * CallBack permettant de lancer le thread de façon virtual
	 */
	static void *sCallback(void *a_pCtx) {
		//AbstractModel *model = static_cast<AbstractModel*> (ctx);
		//model->run();
		((AbstractModel*)a_pCtx)->Run();
		return NULL;
	}

	/*
	 * Méthode principale du model
	 * Virtual pure -> à implémenter obligatoirement
	 */
	virtual void Run() = 0;

public:

	/*
	 * Constructeur
	 * Prend en paramètre la boîte aux lettre du SensorsCenter
	 */
	AbstractModel(int a_iBal) : m_iBalCenter(a_iBal){}

	/*
	 * Destructeur
	 */
	virtual ~AbstractModel(){}

	/*
	 * Méthode permettant de lancer le thread principal
	 */
	virtual void Start()
	{
		pthread_create(&m_thread, 0, AbstractModel::sCallback, (void *)this);
	}

	/*
	 * Méthode permettant de suspendre le thread principal
	 */
	virtual void Stop()
	{
		pthread_cancel(m_thread);
	}

	/*
	 * Méthode permettant d'attendre sur le thread principal
	 */
	virtual void Wait()
	{
		pthread_join(m_thread, NULL);
	}

protected:
	int m_iBalCenter;
	pthread_t m_thread;



};

#endif /* ABSTRACTMODEL_H_ */
