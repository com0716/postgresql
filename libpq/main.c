#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include <libpq-fe.h>

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


int main(int argc, char * argv[])
{
	const char *conn_str = "host=localhost port=8192 dbname=postgres";
	PGconn *conn = getConnection(conn_str);

	fprintf(stdout, "Successfully connected database.\n");

		
	execDemo(conn);

	if (NULL != conn)
	{
		PQfinish(conn);
		fprintf(stdout, "Successfully release the connection.\n");
	}

	return 0;
}

