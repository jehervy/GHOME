/*
 * ghome_database.cpp
 *
 *  Created on: 29 janv. 2012
 *      Author: Pierre
 */
#include <sstream>
#include "GhomeDatabase.h"


int GhomeDatabase::OpenDatabase()
{

	m_pConn = mysql_init(NULL);

	 /* Connexion a la BD */

	 if (!mysql_real_connect(m_pConn, m_sHost.c_str(), m_sUser.c_str() , m_sPw.c_str(), m_sDb.c_str(), 0, NULL, 0))
	 {
	    return -1;
	 }

	return 0;
}

int GhomeDatabase::CloseDatabase() {

	 mysql_close(m_pConn);
	 return 0;
}

int GhomeDatabase::AddTuple(string a_sTable, int a_iRoom,int a_iMetric,int a_iValue)
{

	std::ostringstream oss;
	oss << "INSERT INTO " << a_sTable << " VALUES('', now(), '" << a_iRoom << "' , '" << a_iMetric << "' , '" << a_iValue << "')" ;

	std::string sReq = oss.str();

			   /* Execution de la requete */
			   if (mysql_query(m_pConn, sReq.c_str()))
			   {
			      return -1;
			   }
	return 0;
}

int GhomeDatabase::AddTuple(string a_sTable, string a_sAuthor, int a_iRoom,int a_iMetric,int a_iValue)
{

	std::ostringstream oss;
	oss << "INSERT INTO " << a_sTable << " VALUES('','"<< a_sAuthor << "',now(), '" << a_iRoom << "' , '" << a_iMetric << "' , '" << a_iValue << "')" ;

	std::string sReq = oss.str();
			   /* Execution de la requete */
			   if (mysql_query(m_pConn, sReq.c_str()))
			   {
			      return -1;
			   }
	return 0;
}

int GhomeDatabase::AddTuple(string a_sTable, int a_iType, string a_sMessage)
{
	std::ostringstream oss;
	oss << "INSERT INTO " << a_sTable << " VALUES('', now(), '" << a_iType << "' , '" << a_sMessage << "')" ;

	std::string sReq = oss.str();
			   /* Execution de la requete */
			   if (mysql_query(m_pConn, sReq.c_str()))
			   {
			      return -1;
			   }
	return 0;
}

GhomeDatabase::GhomeDatabase(string a_sHost, string a_sUser, string a_sPw, string a_sDb)
{
	m_sHost = a_sHost;
	m_sUser = a_sUser;
	m_sPw = a_sPw;
	m_sDb = a_sDb;

}

GhomeDatabase::~GhomeDatabase() {
	// TODO Auto-generated destructor stub
}

