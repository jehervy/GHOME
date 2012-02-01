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
#include <string.h>
#include <mysql.h>
using namespace std;

class ghome_database {
public:
	ghome_database(string host, string user, string pw, string db);
	virtual ~ghome_database();
	int open_database();
	int close_database();
	int add_tuple(string table, int type,int room,int value);

private:
	string m_host;
	string m_user;
	string m_pw;
	string m_DB;
	MYSQL *conn;

};

#endif /* GHOME_DATABASE_H_ */
