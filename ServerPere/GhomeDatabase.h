/*
 * ghome_database.h
 *
 *  Created on: 29 janv. 2012
 *      Author: Pierre
 */

#ifndef GHOME_DATABASE_H_
#define GHOME_DATABASE_H_
#include <iostream>
#include <string.h>
#include <mysql/mysql.h>
using namespace std;

//Classe gérant l'acces a une base de données
class GhomeDatabase {
public:
	GhomeDatabase(string a_sHost, string a_sUser, string a_sPw, string a_sDb);
	virtual ~GhomeDatabase();
	//Ouvre la base de donnée associée a l'objet
	int OpenDatabase();
	//Ferme la base de données associée à l'objet
	int CloseDatabase();
	//Permet l'insertion d'un tuple dans les bases de données sensors_values et actuators_commands
	int AddTuple(string a_sTable, int a_iRoom,int a_iMetric,int a_iValue);
	int AddTuple(string a_sTable, string a_sAuthor, int a_iRoom,int a_iMetric,int a_iValue);
	//Permet l'insertion d'un tuple dans la base de données system_logs
	int AddTuple(string a_sTable, int a_iType, string a_sMessage);

private:
	string m_sHost;
	string m_sUser;
	string m_sPw;
	string m_sDb;
	MYSQL *m_pConn;

};

#endif /* GHOME_DATABASE_H_ */
