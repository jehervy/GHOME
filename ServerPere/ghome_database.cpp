/*
 * ghome_database.cpp
 *
 *  Created on: 29 janv. 2012
 *      Author: remi
 */

#include "ghome_database.h"


int ghome_database::open_database()
{
	sqlite3 *db;
	int rc;
	cout<<"Tentative d'ouverture"<<endl;
	/*rc = sqlite3_open("../ghome", &db);
	if( rc ){
	cout<<"Probleme"<<endl;
	}*/
	return 0;
}

ghome_database::ghome_database() {
	// TODO Auto-generated constructor stub

}

ghome_database::~ghome_database() {
	// TODO Auto-generated destructor stub
}

