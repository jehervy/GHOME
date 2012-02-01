/*
 * ghome_database.cpp
 *
 *  Created on: 29 janv. 2012
 *      Author: Pierre
 */

#include "ghome_database.h"
#include <sstream>


int ghome_database::open_database()
{	  //on initialise les pointeurs
	// MYSQL_RES *res;
	// MYSQL_ROW row;

	 conn = mysql_init(NULL);

	 /* Connexion a la BD */

	 if (!mysql_real_connect(conn, m_host.c_str(), m_user.c_str() , m_pw.c_str(), m_DB.c_str(), 0, NULL, 0))
	 {
	    return -1;
	 }

	return 0;
}

int ghome_database::close_database() {

	 mysql_close(conn);
	 return 0;
}

int ghome_database::add_tuple(string table, int type,int room,int value) {
	std::ostringstream oss;
	oss << "INSERT INTO " << table << " VALUES('','" << type << "' , '" << room << "' , '" << value << "')" ;

	std::string req = oss.str();
			   /* Execution de la requete */
			   if (mysql_query(conn, req.c_str()))
			   {
			      return -1;
			   }
	return 0;
}

ghome_database::ghome_database(string host, string user, string pw, string db) {
	m_host = host;
	m_user = user;
	m_pw = pw;
	m_DB = db;

}

ghome_database::~ghome_database() {
	// TODO Auto-generated destructor stub
}

