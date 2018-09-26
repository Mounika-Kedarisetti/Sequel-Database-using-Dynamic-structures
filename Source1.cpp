#define _CRT_SECURE_NO_WARNINGS
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "Header.h"
int main()
{
	int sno = 0, schsize = 1;
	s = (struct schema*)malloc(sizeof(struct schema) * schsize);
	printf(">type ""exit"" to terminate properly\n");
	while (1)
	{
		printf(">");
		char *query = (char*)malloc(sizeof(char) * 200);
		char *sch = (char*)malloc(sizeof(char) * 50);
		char *info = (char*)malloc(sizeof(char) * 50);
		fflush(stdin);
		scanf("%[^\n]", query);
		if (strcmp(query, "exit") == 0)
			return 0;
		char *first = (char*)malloc(sizeof(char) * 50);
		char *next = (char*)malloc(sizeof(char) * 200);
		sscanf(query, "%s %s", first, next);
		if (strcmp(first, "create") == 0)
		{
			if (sno >= schsize)
			{
				s = (struct schema*)realloc(s,sizeof(struct schema) *(sno+ schsize));
			}
			createSchema(query, &sno, s);
			//sno++;
		}
		if (strcmp(first, "import") == 0)
		{
			importSchema(query,sno);
		}
		if (strcmp(first, "join") == 0)
		{
			joinSchemas(query, &sno);
			//sno++;
		}
		if (strcmp(first, "flush") == 0)
		{
			flushSchema(query, sno);
		}
		if (strcmp(first, "select") == 0)
		{
			executeQuery(query, sno);
		}
	}
}
	