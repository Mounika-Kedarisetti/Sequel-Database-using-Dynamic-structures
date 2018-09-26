#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<math.h>
struct data
{
	char **theData;
};
struct schema
{
	char *sname;
	char **datatype;
	char **colname;
	int cols;
	int records;
	FILE *fp[10];
	int fpno;
	struct data *d;
}*s;
void matchCols(char *selcols, int schno, int colno, char* col, char* cond, int start, int end);
void selectFromSchema(char *selcols, int k, char * col, char *condition, char * cond);
void containsCols(char *selcols, int schno, int colno, char* col, char* cond, int start, int end);
void equals(char *selcols, int schno, int colno, char* col, char* cond, int start, int end);
void less(char *selcols, int schno, int colno, char* col, char* cond, int start, int end);
void lesseq(char *selcols, int schno, int colno, char* col, char* cond, int start, int end);
void great(char *selcols, int schno, int colno, char* col, char* cond, int start, int end);
void greateq(char *selcols, int schno, int colno, char* col, char* cond, int start, int end);
int strcomp(char *str, char *substr)
{
	if (strlen(substr)>strlen(str))
		return -1;
	int i, j, flag = 0;
	for (i = 0; i <= strlen(str) - strlen(substr); i++)
	{
		for (j = i; j < i + strlen(substr); j++)
		{
			flag = 1;
			if (str[j] != substr[j - i])
			{
				flag = 0;
				break;
			}
		}
		if (flag == 1)
			break;
	}
	if (flag == 1)
		return 0;
	return -1;
}
int toInt(char*str)
{
	int i = 0, num = 0;
	while (str[i] != '\0')
	{
		num = num * 10 + (str[i] - '0');
		i++;
	}
	return num;
}
char *toString(int val)
{
	char *str = (char *)malloc(sizeof(char) * 10);
	int i = 0, n = val;
	if (val == 0)
	{
		str = "0";
		return str;
	}
	while (val != 0)
	{
		val = val / 10;
		i++;
	}
	str[i] = '\0';
	i--;
	while (i >= 0)
	{
		str[i] = (n % 10) + '0';
		n = n / 10;
		i--;
	}
	return str;
}
void createSchema(char *query, int *sno, struct schema *s)
{
	char *sch = (char*)malloc(sizeof(char) * 50);
	char *info = (char*)malloc(sizeof(char) * 50);
	char *create, *schema;
	schema = (char*)malloc(sizeof(char) * 50);
	create = (char*)malloc(sizeof(char) * 50);
	sscanf(query, "%s %s %s %s", create, sch, schema, info);
	s[*sno].sname = (char*)malloc(sizeof(char) * 50);
	s[*sno].records = 0;
	int i, index = -1;
	for (i = 0; i < *sno; i++)
	{
		if (strcmp(s[i].sname, schema) == 0)
		{
			printf("the schema %s already exists", s[i].sname);
			return;
		}
	}
	strcpy(s[*sno].sname, schema);
	char *str = (char*)malloc(sizeof(char) * 20);
	int k = 0, col = 0, t = 0, q = 0;
	for (i = 0; info[i] != 0; i++)
	{
		if (info[i] == ':')
		{
			col++;
		}
	}
	s[*sno].cols = col;
	s[*sno].datatype = (char**)malloc(sizeof(char*)*col);
	s[*sno].colname = (char**)malloc(sizeof(char*)*col);
	for (i = 0; i < col; i++)
	{
		s[*sno].datatype[i] = (char*)malloc(sizeof(char) * 50);
		s[*sno].colname[i] = (char*)malloc(sizeof(char) * 50);
		//s[sno].d.theData[i] = (char*)malloc(sizeof(char) * 50);
	}
	//for ()
	s[*sno].d = (struct data*)malloc(sizeof(struct data)*(50));
	for (int t = 0; t < 50; t++)
	{
		s[*sno].d[t].theData = (char**)malloc(sizeof(char*) * (s[*sno].cols));
		for (int p = 0; p < s[*sno].cols; p++)
			s[*sno].d[t].theData[p] = (char*)malloc(sizeof(char) * 50);
	}
	for (i = 0; info[i] != 0; i++)
	{
		if (info[i] == ':')
		{
			str[k] = '\0';
			strcpy(s[*sno].colname[t], str);
			t++;
			k = 0;
			continue;
		}
		if (info[i] == ',')
		{
			str[k] = '\0';
			strcpy(s[*sno].datatype[q], str);
			q++;
			k = 0;
			continue;
		}
		str[k] = info[i];
		k++;

	}
	if (info[i] == '\0')
	{
		str[k] = '\0';
		strcpy(s[*sno].datatype[q], str);
		q++;
	}
	//s[*sno].fp = (FILE **)malloc(sizeof(FILE *) * 20);
	/*for (i = 0; i < 20; i++)
	{
		s[*sno].fp[i] = (FILE *)malloc(sizeof(FILE));
	}*/
	
	s[*sno].fpno = 0;
	*sno = *sno + 1;
	
}
void importSchema(char *query, int sno)
{
	char *import = (char*)malloc(sizeof(char) * 50);
	char *fname = (char*)malloc(sizeof(char) * 50);
	char *schname = (char*)malloc(sizeof(char) * 50);
	sscanf(query, "%s %s %s", import, fname, schname);
	int i = 0;
	while (i < sno)
	{
		if (strcmp(s[i].sname, schname) == 0)

			break;
		i++;
	}
	if (i == sno)
	{
		printf("enter valid schema name\n");
		return;
	}
	
	//s[sno].fp[s[sno].fpno] = fopen(fname, "r");
	FILE *fp = fopen(fname, "r");
	s[i].fp[s[i].fpno] = fp;
	if (fp == NULL)
	{
		printf("enter valid filename\n");
		return;
	}
	s[i].fpno=s[i].fpno + 1;
	int records = 0;
	char c = fgetc(fp);
	while (c != EOF)
	{
		if (c == '\n')
		{
			records++;
		}
		c = fgetc(fp);
	}
	records--;
	s[i].records = s[i].records+records;
}
void executeQuery(char *query, int sno)
{
	char  *str, *str1, *str2, *str3, *selcols, *schema, *col, *condition, *cond, *from;
	str = (char *)malloc(sizeof(char) * 30);
	str1 = (char *)malloc(sizeof(char) * 30);
	str2 = (char *)malloc(sizeof(char) * 30);
	col = (char *)malloc(sizeof(char) * 30);
	condition = (char *)malloc(sizeof(char) * 30);
	cond = (char *)malloc(sizeof(char) * 30);
	selcols = (char *)malloc(sizeof(char) * 30);
	from = (char *)malloc(sizeof(char) * 30);
	schema = (char *)malloc(sizeof(char) * 30);
	sscanf(query, "%s %s %s %s %s %s %s %s", str, selcols, from, schema, str1, col, condition, cond);
	int i = 0;
	int j = 0;
	while (1)
	{
		if (schema[i] == ',' || schema[i] == '\0')
		{
			str[j] = '\0';
			j = 0;
			for (int k = 0; k < sno; k++)
			{
				if (strcmp(s[k].sname, str) == 0)
				{
					selectFromSchema(selcols, k, col, condition, cond);
				}

			}
			//i++;
			if (schema[i] == '\0')
				break;
			i++;
			continue;
		}
		str[j] = schema[i];
		j++;
		i++;
	}

}
void selectFromSchema(char *selcols, int k, char * col, char *condition, char * cond)
{
	int i;
	char *str = (char*)malloc(sizeof(char) * 50);
	if (strcmp(selcols, "*") == 0)
	{
		for (i = 0; i < s[k].cols; i++)
			printf("%10s \t", s[k].colname[i]);
	}
	else
	{
		i = 0;
		while (1)
		{
			if (selcols[i] == ',' || selcols[i] == '\0')
			{
				str[k] = '\0';
				printf("%10s \t", str);
				k = 0;
				if (selcols[i] == '\0')
					break;
				i++;
				continue;
			}
			str[k] = selcols[i];
			i++;
			k++;
		}
	}
	printf("\n");
	int start = 0, end = 10,intval;
	int st = start;
	char *strval = (char*)malloc(sizeof(char) * 50);
	int records = 0,fnum=0;
	rewind(s[k].fp[fnum]);
	char c = fgetc(s[k].fp[fnum]);
	while (c != '\n')
	{
		c = fgetc(s[k].fp[fnum]);
	}
	int actualRecords;
	 end = 50;
	while (1)
	{
		while (st < 50&&records < s[k].records)
		{
			for (i = 0; i < s[k].cols; i++)
			{

				if (strcmp(s[k].datatype[i], "int") == 0)
				{
					if (fscanf(s[k].fp[fnum], "%d,", &intval) == EOF)
					{
						fnum = fnum + 1;
						if (fnum >= s[k].fpno)
						{
							actualRecords = records;
							break;
						}
						rewind(s[k].fp[fnum]);
						char c = fgetc(s[k].fp[fnum]);
						while (c != '\n')
						{
							c = fgetc(s[k].fp[fnum]);
						}
						st--;
						records--;
						break;
					}
					else
					{
						char *str1 = toString(intval);
						strcpy(s[k].d[st].theData[i], str1);
					}
					
				}
				if (strcmp(s[k].datatype[i], "string") == 0)
				{

					if (fscanf(s[k].fp[fnum], "%[^,\n],", strval) == EOF)
					{
						fnum = fnum + 1;
						if (fnum >= s[k].fpno)
							break;
						char c = fgetc(s[k].fp[fnum]);
						while (c != '\n')
						{
							c = fgetc(s[k].fp[fnum]);
						}
						break;
					}
					else
					{
						strcpy(s[k].d[st].theData[i], strval);

					}
					
				}
			}
				st++;
				records = records + 1;
		}
		if (end > s[k].records)
		{
			end = records;
		}
		for (i = 0; i < s[k].cols; i++)
		{
			if (strcmp(s[k].colname[i], col) == 0)
			{
				if (strcmp(condition, "matches") == 0)
					matchCols(selcols, k, i, col, cond, start, end);
				if (strcmp(condition, "contains") == 0)
					containsCols(selcols, k, i, col, cond,start,end);
					if (strcmp(condition, "EQ") == 0)
					equals(selcols, k, i, col, cond,start,end);
					if (strcmp(condition, "LT") == 0)
						less(selcols, k, i, col, cond, start, end);
					if (strcmp(condition, "GT") == 0)
						great(selcols, k, i, col, cond, start, end);
					if (strcmp(condition, "LE") == 0)
						lesseq(selcols, k, i, col, cond, start, end);
					if (strcmp(condition, "GE") == 0)
						greateq(selcols, k, i, col, cond, start, end); 

			}

		}
		start =start+ st;
		st = 0;
		if (records == s[k].records)
			break;
		if ((end + 50) < s[k].records)
			end = end + 50;
		else
		{
			end = end + (s[k].records - end);
		}
	}

}
void getCols(char *selcols, int schno, int recno)
{
	int i, k, j;
	char *str = (char*)malloc(sizeof(char) * 50);
	if (strcmp(selcols, "*") == 0)
	{
		for (i = 0; i < s[schno].cols; i++)
		{
			if (strcmp(s[schno].datatype[i], "int") == 0)
				printf("%10d \t", toInt(s[schno].d[recno].theData[i]));
			if (strcmp(s[schno].datatype[i], "string") == 0)
				printf("%10s \t", s[schno].d[recno].theData[i]);
			if (strcmp(s[schno].datatype[i], "float") == 0)
				printf("%10f \t", atof(s[schno].d[recno].theData[i]));
		}
		printf("\n");
	}
	else
	{
		i = 0, k = 0;
		while (1)
		{
			if (selcols[i] == ',' || selcols[i] == '\0')
			{
				str[k] = '\0';
				for (j = 0; j < s[schno].cols; j++)
				{
					if (strcmp(str, s[schno].colname[j]) == 0)
					{
						if (strcmp(s[schno].datatype[j], "int") == 0)
							printf("%10d \t", toInt(s[schno].d[recno].theData[j]));
						if (strcmp(s[schno].datatype[j], "string") == 0)
							printf("%10s \t", s[schno].d[recno].theData[j]);
						if (strcmp(s[schno].datatype[j], "float") == 0)
							printf("%10v \t", atof(s[schno].d[recno].theData[j]));
					}
				}
				k = 0;
				if (selcols[i] == '\0')
					break;
				i++;
				continue;
			}
			str[k] = selcols[i];
			k++;
			i++;
		}
		printf("\n");
	}
}
void matchCols(char *selcols, int schno, int colno, char* col, char* cond,int start,int end)
{
	int i, j,k=0;
	for (j = start; j < end; j++)
	{
		if (strcmp(s[schno].d[k].theData[colno], cond) == 0)
		{
			getCols(selcols, schno, k);
		}
		k++;
	}
}
void containsCols(char *selcols, int schno, int colno, char* col, char* cond, int start, int end)
{
	int i, j,k=0;
	for (j = start; j < end; j++)
	{
		if (strcomp(s[schno].d[k].theData[colno], cond) == 0)
		{
			getCols(selcols, schno, k);
		}
		k++;
	}
}
void equals(char *selcols, int schno, int colno, char* col, char* cond, int start, int end)
{
	int i, j, k = 0;
	for (j = start; j < end; j++)
	{
		if (strcmp(s[schno].datatype[colno], "int") == 0)
		{
			if (toInt(s[schno].d[k].theData[colno]) == toInt(cond))
			{
				getCols(selcols, schno, k);
			}
			k++;
		}
		else if (strcmp(s[schno].datatype[colno], "float") == 0)
		{
			if (atof(s[schno].d[k].theData[colno]) == atof(cond))
			{
				getCols(selcols, schno, j);
			}
			k++;
		}
	}
}
void less(char *selcols, int schno, int colno, char* col, char* cond, int start, int end)
{
	int i, j, k = 0;
	for (j = start; j < end; j++)
	{
		if (strcmp(s[schno].datatype[colno], "int") == 0)
		{
			if (toInt(s[schno].d[k].theData[colno]) < toInt(cond))
			{
				getCols(selcols, schno, k);
			}
			k++;
		}
		else if (strcmp(s[schno].datatype[colno], "float") == 0)
		{
			if (atof(s[schno].d[k].theData[colno]) < atof(cond))
			{
				getCols(selcols, schno, k);
			}
			k++;
		}
	}
}
void great(char *selcols, int schno, int colno, char* col, char* cond, int start, int end)
{
	int i, j,k=0;
	for (j = start; j < end; j++)
	{
		if (strcmp(s[schno].datatype[colno], "int") == 0)
		{
			if (toInt(s[schno].d[k].theData[colno]) > toInt(cond))
			{
				getCols(selcols, schno, k);
			}
			k++;
		}
		else if (strcmp(s[schno].datatype[colno], "float") == 0)
		{
			if (atof(s[schno].d[k].theData[colno]) > atof(cond))
			{
				getCols(selcols, schno, k);
			}
			k++;
		}
	}
}
void lesseq(char *selcols, int schno, int colno, char* col, char* cond, int start, int end)
{
	int i, j, k = 0;
	for (j = start; j < end; j++)
	{
		if (strcmp(s[schno].datatype[colno], "int") == 0)
		{
			if (toInt(s[schno].d[k].theData[colno]) <= toInt(cond))
			{
				getCols(selcols, schno, k);
			}
			k++;
		}
		else if (strcmp(s[schno].datatype[colno], "float") == 0)
		{
			if (atof(s[schno].d[k].theData[colno]) <= atof(cond))
			{
				getCols(selcols, schno, k);
			}
			k++;
		}
	}
}
void greateq(char *selcols, int schno, int colno, char* col, char* cond, int start, int end)
{
	int i, j, k = 0;
	for (j = start; j < end; j++)
	{
		if (strcmp(s[schno].datatype[colno], "int") == 0)
		{
			if (toInt(s[schno].d[k].theData[colno]) >= toInt(cond))
			{
				getCols(selcols, schno, k);
			}
			k++;
		}
		else if (strcmp(s[schno].datatype[colno], "float") == 0)
		{
			if (atof(s[schno].d[k].theData[colno]) >= atof(cond))
			{
				getCols(selcols, schno, k);
			}
			k++;
		}
	}
}
void flushSchema(char *schname,int sno)
{
	/*FILE *fp = fopen("flushFile.csv", "w");
	int i, index = -1;
	for (i = 0; i < sno; i++)
	{
		if (strcmp(s[i].sname, schname) == 0)
		{
			index = i;
			break;
		}
	}
	if (index == -1)
	{
		printf("schema %s not defined\n", schname);
		return;
	}

	for (i = 0; i < s[index].cols - 1; i++)
	{
		fprintf(fp, "%s,", s[index].colname[i]);
	}
	fprintf(fp, "%s\n", s[index].colname[i]);
	int fno = 0,i=0,c;
	FILE *fp1;
	while (i < fno)
	{
		fp1 = s[index].fp[fno];
		for (c = 0; c < s[index].cols; c++)
		{

		}*/
	return;

}