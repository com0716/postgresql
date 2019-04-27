#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include <libpq-fe.h>
#include <arpa/inet.h>

PGconn *getConnection(const char *conn_str)
{
	assert(NULL != conn_str);

	PGconn *conn = PQconnectdb(conn_str);
	if (CONNECTION_OK != PQstatus(conn))
	{
		fprintf(stderr, "Failed to connect database : %s\n", PQerrorMessage(conn));
		exit(-1);
	}

	return conn;
}

void displayResult(const PGresult *res)
{
	assert(NULL != res);

	int nfields = PQnfields(res);
	int ntuples = PQntuples(res);

	int rows = 0;
	int cols = 0;
	for (cols = 0; cols < nfields; ++cols)
	{
		fprintf(stdout, "| %s", PQfname(res, cols));
	}
	fprintf(stdout, "|\n");

	for (rows = 0; rows < ntuples; ++rows)
	{
		for (cols = 0; cols < nfields; ++cols)
		{
			fprintf(stdout, "| %s", PQgetvalue(res, rows, cols));
		}
		fprintf(stdout, "|\n");
	}
}


void execDemo(PGconn *conn)
{
	assert(NULL != conn);

	PGresult *res = PQexec(conn, "select * from student");
	if (PGRES_TUPLES_OK != PQresultStatus(res)) /*PGRES_TUPLES_OK 和 PGRES_COMMAND_OK 使用的情况不同，需要注意*/
	{
		fprintf(stderr, "query failed : %s\n", PQerrorMessage(conn));
		PQclear(res);
		return ;
	}

	displayResult(res);

	PQclear(res);
}



void execParamsDemo(PGconn *conn)
{
	/**
	PGresult *PQexecParams(PGconn *conn,
		 const char *command,
		 int nParams,
		 const Oid *paramTypes,
		 const char *const * paramValues,
		 const int *paramLengths,
		 const int *paramFormats,
		 int resultFormat);
	 */
	
	assert(NULL != conn);
	
	const char * command = "select * from student where id=$1";
	int nParams = 1;
	int student_id = htonl(2);  /*这里一定要注意大小端问题*/
	const char *paramValues[] = {(char *)&student_id};
	const int paramLengths[] = {sizeof(student_id)};
	const int paramFormats[] = {1};

	PGresult * res = PQexecParams(conn,
								command,
								nParams,
								NULL,
								paramValues,
								paramLengths,
								paramFormats,
								0);
	if (PGRES_TUPLES_OK != PQresultStatus(res))
	{
		fprintf(stderr, "query failed : %s\n", PQerrorMessage(conn));
		PQclear(res);
		return ;
	}

	displayResult(res);

	PQclear(res);
}

void prepareDemo(PGconn * conn)
{
	/*
	PGresult *PQprepare(PGconn *conn, const char *stmtName,
		  const char *query, int nParams,
		  const Oid *paramTypes);
	 */
	assert(NULL != conn);

	const char *stmtName = "findStudentById";
	const char *query = "select * from student where id=$1";
	int nParams = 1;
	const Oid paramTypes[] = {23};

	PGresult *res = PQprepare(conn,
							stmtName,
							query,
							nParams,
							paramTypes);
	if (PGRES_COMMAND_OK != PQresultStatus(res))
	{
		fprintf(stderr, "query failed : %s\n", PQerrorMessage(conn));
		PQclear(res);
		return ;
	}

	PQclear(res);
	res = NULL;
	fprintf(stderr, "Successfully prepared the statement\n");

	/*
	PGresult *PQexecPrepared(PGconn *conn,
			   const char *stmtName,
			   int nParams,
			   const char *const * paramValues,
			   const int *paramLengths,
			   const int *paramFormats,
			   int resultFormat);
	 */
	int student_id = htonl(1);
	const char * paramValues[] = {(char *)&student_id};
	const int paramLengths[] = {sizeof(student_id)};
	const int paramFormats[] = {1};

	res = PQexecPrepared(conn, 
						stmtName,
						nParams,
						paramValues,
						paramLengths,
						paramFormats,
						0);
	if (PGRES_TUPLES_OK != PQresultStatus(res))
	{
		fprintf(stderr, "query failed : %s\n", PQerrorMessage(conn));
		PQclear(res);
		return ;
	}

	displayResult(res);

	PQclear(res);
}

int main(int argc, char * argv[])
{
	const char *conn_str = "host=localhost port=8192 dbname=postgres";
	PGconn *conn = getConnection(conn_str);

	fprintf(stdout, "Successfully connected database.\n");

		
	//execDemo(conn);
	//execParamsDemo(conn);
	prepareDemo(conn);

	if (NULL != conn)
	{
		PQfinish(conn);
		fprintf(stdout, "Successfully release the connection.\n");
	}

	return 0;
}

