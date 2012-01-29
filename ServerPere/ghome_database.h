/*
 * ghome_database.h
 *
 *  Created on: 29 janv. 2012
 *      Author: remi
 */

#ifndef GHOME_DATABASE_H_
#define GHOME_DATABASE_H_
//#include "sqlite3.h"
#include <iostream>
using namespace std;

class ghome_database {
public:
	ghome_database();
	virtual ~ghome_database();
	static int open_database();
};

#endif /* GHOME_DATABASE_H_ */
