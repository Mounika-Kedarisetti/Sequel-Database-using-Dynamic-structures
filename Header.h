#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<math.h>
#include<ctype.h>
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
	struct data *d;
}*s;
void selectFromSchema(char *selcols, int k, char * col, char *condition, char * cond);
void getCols(char *selcols, int schno, int recno);
void matchCols(char *selcols,  int schno, int colno, char* col, char* cond);
void containsCols(char *selcols, int schno, int colno, char *col, char *cond);
void equals(char *selcols, int schno, int colno, char *col, char *cond);
int strcomp(char *substr, char *str);
void less(char *selcols, int schno, int colno, char *col, char *cond);
void great(char *selcols, int schno, int colno, char *col, char *cond);
void lesseq(char *selcols, int schno, int colno, char *col, char *cond);
void greateq(char *selcols, int schno, int colno, char *col, char *cond);
int strccmp(char *s1, char *s2)
{
	int i = 0;
	while (s1[i] != '\0'&&s2[i] != '\0')
	{
		if (tolower(s1[i]) - tolower(s2[i]) != 0)
			return -1;
		i++;
	}
	return 0;
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
void reverse(char *str, int len)
{
	int i = 0, j = len - 1, temp;
	while (i<j)
	{
		temp = str[i];
		str[i] = str[j];
		str[j] = temp;
		i++; j--;
	}
}
int intToStr(int x, char str[], int d)
{
	int i = 0;
	while (x)
	{
		str[i++] = (x % 10) + '0';
		x = x / 10;
	}

	while (i < d)
		str[i++] = '0';

	reverse(str, i);
	str[i] = '\0';
	return i;
}
void ftoa(float n, char *res)
{
	int ipart = (int)n;
	float fpart = n - (float)ipart;
	int i = intToStr(ipart, res, 0);
		res[i] = '.'; 
		fpart = fpart * pow((double)10, 3);
		intToStr((int)fpart, res + i + 1, 3);

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
	FILE *fp = fopen(fname, "r");
	if (fp == NULL)
	{
		printf("enter valid filename\n");
		return;
	}
	int index = i, intval;
	float floatval;
	char *strval = (char*)malloc(sizeof(char) * 50);

	int j = s[index].cols;
	int records = 0;
	char *str = (char*)malloc(sizeof(char) * 200);
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
	//s[index].records = records;
	if (records <= 0)
		return;
	rewind(fp);
	// c = getc(fp);
	/*if (s[index].records == 0)
	{
		for (i = 0; i < s[index].cols; i++)
		{
			fscanf(fp, "%[^,\n],", s[index].colname[i]);

		}
	}*/
		for (i = 0; i < s[index].cols; i++)
		{
			fscanf(fp, "%[^,\n],", str);
			if (strccmp(s[index].colname[i], str) != 0)
				printf("cant import this file into schema as they dont have same column name");

		}
	if (s[index].records == 0)
	{
		s[index].d = (struct data*)malloc(sizeof(struct data)*records);
		for (i = 0; i < records; i++)
		{
			s[index].d[i].theData = (char**)malloc(sizeof(char*) * s[index].cols);
			for (j = 0; j < s[index].cols; j++)
				s[index].d[i].theData[j] = (char*)malloc(sizeof(char) * 50);
		}
	}
	else
	{
		s[index].d = (struct data*)realloc(s[index].d, sizeof(struct data)*(records + s[index].records));
		for (i = s[index].records; i < records + s[index].records; i++)
		{
			s[index].d[i].theData = (char**)malloc(sizeof(char*) * s[index].cols);
			for (j = 0; j < s[index].cols; j++)
				s[index].d[i].theData[j] = (char*)malloc(sizeof(char) * 50);
		}
	}
	j = s[index].records;
	c = fgetc(fp);
while (j<records+s[index].records)
	{

		for (i = 0; i < s[index].cols; i++)
		{
			
			if (strcmp(s[index].datatype[i], "int") == 0)
			{
				fscanf(fp, "%d,", &intval);
				char *st = toString(intval);
				strcpy(s[index].d[j].theData[i], st);
			}
			if (strcmp(s[index].datatype[i], "string") == 0)
			{
				fscanf(fp, "%[^,\n],", strval);
				strcpy(s[index].d[j].theData[i], strval);
			}

			if (strcmp(s[index].datatype[i], "float") == 0)
			{
				fscanf(fp, "%f,", &floatval);
				char res[20];
				ftoa(floatval, res);
				strcpy(s[index].d[j].theData[i],res );
			}
		}
		if (getc(fp) == '\n')
		{
			j++;
			continue;
		}
		if (fgetc(fp) == EOF)
			break;
	}
s[index].records = s[index].records + records;
fclose(fp);
}
void flushSchema(char *query, int sno)
{
	char *schema = (char*)malloc(sizeof(char) * 50);
	char *schname = (char*)malloc(sizeof(char) * 50);
	sscanf(query, "%s %s", schema, schname);
	FILE *fp = fopen("flushFile.csv", "w");
	int i,index=-1;
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
		printf("schema %s not defined\n",schname);
		return;
	}
	for (i = 0; i < s[index].cols-1; i++)
	{
		fprintf(fp, "%s,", s[index].colname[i]);
	}
	fprintf(fp, "%s\n", s[index].colname[i]);
	int j;
	for (j = 0; j < s[index].records; j++)
	{
		for (i = 0; i < s[index].cols - 1; i++)
		{
			if (strcmp(s[index].datatype[i], "int") == 0)
				fprintf(fp, "%d,", toInt(s[index].d[j].theData[i]));
			if (strcmp(s[index].datatype[i], "string") == 0)
				fprintf(fp, "%s,", s[index].d[j].theData[i]);
			if (strcmp(s[index].datatype[i], "float") == 0)
				fprintf(fp, "%f,",atof( s[index].d[j].theData[i]));

		}
		if (strcmp(s[index].datatype[i], "int") == 0)
			fprintf(fp, "%d\n", toInt(s[index].d[j].theData[i]));
		if (strcmp(s[index].datatype[i], "string") == 0)
			fprintf(fp, "%s\n", s[index].d[j].theData[i]);
		if (strcmp(s[index].datatype[i], "float") == 0)
			fprintf(fp, "%f\n", atof(s[index].d[j].theData[i]));
	}
	fclose(fp);
	free(s[index].d);

}
void joinSchemas(char*query, int *sno)
{
	char *sch = (char*)malloc(sizeof(char) * 50);
	char *s1 = (char*)malloc(sizeof(char) * 50);
	char *s2 = (char*)malloc(sizeof(char) * 50);
	char *join = (char*)malloc(sizeof(char) * 50);
	sscanf(query, "%s %s %s %s", join, sch, s1, s2);
	int index1 = -1, index2 = -1;
	int i = 0;
	while (i != *sno)
	{
		if (strcmp(s[i].sname, s1) == 0)
			index1 = i;
		if (strcmp(s[i].sname, s2) == 0)
			index2 = i;
		if (strcmp(s[i].sname, sch) == 0)
		{
			printf("schema %s already exists\n", sch);
			return;
		}
		i++;
	}

	if (index1 == -1 || index2 == -1)
	{
		printf("enter valid schemas");
		return;
	}
//	s[*sno] = (struct schema *)malloc(sizeof(struct schema)
	s[*sno].sname = (char*)malloc(sizeof(char) * 50);
	strcpy(s[*sno].sname, sch);
	s[*sno].cols = (s[index1].cols + s[index2].cols) - 1;
	s[*sno].datatype = (char**)malloc(sizeof(char*)*(s[*sno].cols));
	s[*sno].colname = (char**)malloc(sizeof(char*)*(s[*sno].cols));
	for (i = 0; i < s[*sno].cols; i++)
	{
		s[*sno].datatype[i] = (char*)malloc(sizeof(char) * 50);
		s[*sno].colname[i] = (char*)malloc(sizeof(char) * 50);
	}
	for (i = 0; i < s[index1].cols; i++)
	{
		strcpy(s[*sno].colname[i], s[index1].colname[i]);
		strcpy(s[*sno].datatype[i], s[index1].datatype[i]);
	}
	int j = i;
	for (i = 1; i < s[index2].cols; i++)
	{
		strcpy(s[*sno].colname[j], s[index2].colname[i]);
		strcpy(s[*sno].datatype[j], s[index2].datatype[i]);
		j++;
	}
	int rec = 0, k;	
	s[*sno].d = (struct data*)malloc(sizeof(struct data)*(s[index1].records+s[index2].records));
	int records = s[index1].records + s[index2].records;
	for (i = 0; i < records; i++)
	{
		s[*sno].d[i].theData = (char**)malloc(sizeof(char*) * s[*sno].cols);
		for (j = 0; j < s[*sno].cols; j++)
			s[*sno].d[i].theData[j] = (char*)malloc(sizeof(char) * 50);
	}
	for (i = 0; i < s[index1].records; i++)
	{
		for (j = 0; j < s[index2].records; j++)
		{
			if (strcmp(s[index1].d[i].theData[0], s[index2].d[j].theData[0]) == 0)
			{
				//s[*sno].d[rec].theData = (char**)malloc(sizeof(char*) * s[*sno].cols);
				for (k = 0; k < s[index1].cols; k++)
				{
					//s[*sno].d[rec].theData[k] = (char*)malloc(sizeof(char) * 50);
					strcpy(s[*sno].d[rec].theData[k], s[index1].d[i].theData[k]);
				}
				int t;
				for (t = 1; t < s[index2].cols; t++)
				{
					//s[*sno].d[rec].theData[k] = (char*)malloc(sizeof(char) * 50);
					strcpy(s[*sno].d[rec].theData[k], s[index2].d[j].theData[t]);
					k++;
				}
				rec = rec + 1;
			}
		}
	}
	for (i = 0; i < s[index1].records; i++)
	{
		for (j = 0; j < s[index2].records; j++)
		{
			if (strcmp(s[index1].d[i].theData[0], s[index2].d[j].theData[0]) == 0)
			{
				break;
			}
		}
		if (j == s[index2].records)
		{
			//s[*sno].d[rec].theData = (char**)malloc(sizeof(char*) * s[*sno].cols);
			for (k = 0; k < s[index1].cols; k++)
			{
				//s[*sno].d[rec].theData[k] = (char*)malloc(sizeof(char) * 50);
				strcpy(s[*sno].d[rec].theData[k], s[index1].d[i].theData[k]);
			}
			int t;
			for (t = 1; t < s[index2].cols; t++)
			{
			//	s[*sno].d[rec].theData[k] = (char*)malloc(sizeof(char) * 50);
				strcpy(s[*sno].d[rec].theData[k], "NA");
				k++;
			}
			rec = rec + 1;
		}

	}
	for (j = 0; j < s[index2].records; j++)
	{
		for (i = 0; i < s[index1].records; i++)
		{
			if (strcmp(s[index1].d[i].theData[0], s[index2].d[j].theData[0]) == 0)
			{
				break;
			}
		}
		if (i == s[index1].records)
		{
			//s[*sno].d[rec].theData = (char**)malloc(sizeof(char*) * s[*sno].cols);
			strcpy(s[*sno].d[rec].theData[0], s[index2].d[j].theData[0]);
			for (k = 1; k < s[index1].cols; k++)
			{
				//strcpy()
				//s[*sno].d[rec].theData[k] = (char*)malloc(sizeof(char) * 50);
				strcpy(s[*sno].d[rec].theData[k], "NA");
			}
			int t;
			for (t = 1; t < s[index2].cols; t++)
			{
			//	s[*sno].d[rec].theData[k] = (char*)malloc(sizeof(char) * 50);
				strcpy(s[*sno].d[rec].theData[k], s[index2].d[j].theData[t]);
				k++;
			}
			rec = rec + 1;

		}
	}
	s[*sno].records = rec;
	*sno = *sno + 1;
}
void createSchema(char *query,int *sno,struct schema *s)
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
	int k = 0,  col = 0, t = 0, q = 0;
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
	*sno = *sno + 1;
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
		if (schema[i] == ','||schema[i]=='\0')
		{
			str[j] = '\0';
			j = 0;
			for (int k = 0; k < sno; k++)
			{
				if (strcmp(s[k].sname, str) == 0)
				{
					selectFromSchema(selcols,k,col,condition,cond);
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
	for (i = 0; i < s[k].cols; i++)
	{
		if (strcmp(s[k].colname[i], col) == 0)
		{
			if (strcmp(condition, "matches")==0)
				matchCols(selcols, k, i,col,  cond);
			if (strcmp(condition, "contains") == 0)
				containsCols(selcols, k, i, col, cond);
			if (strcmp(condition, "EQ") == 0)
				equals(selcols, k, i, col, cond);
			if (strcmp(condition, "LT") == 0)
				less(selcols, k, i, col, cond);
			if (strcmp(condition, "GT") == 0)
				great(selcols, k, i, col, cond);
			if (strcmp(condition, "LE") == 0)
				lesseq(selcols, k, i, col, cond);
			if (strcmp(condition, "GE") == 0)
				greateq(selcols, k, i, col, cond);

		}
		
	}

}
void matchCols(char *selcols, int schno,int colno, char* col, char* cond)
{
	int i, j, k = 0;
	for (j = 0; j < s[schno].records; j++)
	{
		if (strcmp(s[schno].d[j].theData[colno], cond) == 0)
		{
			getCols(selcols, schno, j);
		}
	}
}
void getCols(char *selcols,int schno, int recno)
{
	int i,k,j;
	char *str = (char*)malloc(sizeof(char) * 50);
	if (strcmp(selcols, "*") == 0)
	{
		for (i = 0; i < s[schno].cols; i++)
		{
			if (strcmp(s[schno].datatype[i], "int") == 0)
				printf("%10d \t",toInt( s[schno].d[recno].theData[i]));
			if (strcmp(s[schno].datatype[i], "string") == 0)
				printf("%10s \t", s[schno].d[recno].theData[i]);
			if (strcmp(s[schno].datatype[i], "float") == 0)
				printf("%10f \t", atof(s[schno].d[recno].theData[i]));
		}
		printf("\n");
	}
	else
	{
		i = 0,k=0;
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
void containsCols(char *selcols, int schno, int colno, char *col, char *cond)
{
	int i, j, k = 0;
	for (j = 0; j < s[schno].records; j++)
	{
		if (strcomp(cond,s[schno].d[j].theData[colno]) == 0)
		{
			getCols(selcols, schno, j);
		}
	}
	
}
int strcomp(char *substr, char *str)
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
void equals(char *selcols, int schno, int colno, char *col, char *cond)
{
	int i, j, k = 0;
	for (j = 0; j < s[schno].records; j++)
	{
		if (strcmp(s[schno].datatype[colno], "int") == 0)
		{
			if (toInt(s[schno].d[j].theData[colno]) == toInt(cond))
			{
				getCols(selcols, schno, j);
			}
		}
		else if (strcmp(s[schno].datatype[colno], "float") == 0)
		{
			if (atof(s[schno].d[j].theData[colno]) == atof(cond))
			{
				getCols(selcols, schno, j);
			}
		}
	}
}
void less(char *selcols, int schno, int colno, char *col, char *cond)
{
	int i, j, k = 0;
	for (j = 0; j < s[schno].records; j++)
	{
		if (strcmp(s[schno].datatype[colno], "int") == 0)
		{
			if (toInt(s[schno].d[j].theData[colno]) < toInt(cond))
			{
				getCols(selcols, schno, j);
			}
		}
		else if (strcmp(s[schno].datatype[colno], "float") == 0)
		{
			if (atof(s[schno].d[j].theData[colno]) < atof(cond))
			{
				getCols(selcols, schno, j);
			}
		}
	}
}
void great(char *selcols, int schno, int colno, char *col, char *cond)
{
	int i, j, k = 0;
	for (j = 0; j < s[schno].records; j++)
	{
		if (strcmp(s[schno].datatype[colno], "int") == 0)
		{
			if (toInt(s[schno].d[j].theData[colno]) > toInt(cond))
			{
				getCols(selcols, schno, j);
			}
		}
		else if (strcmp(s[schno].datatype[colno], "float") == 0)
		{
			if (atof(s[schno].d[j].theData[colno]) > atof(cond))
			{
				getCols(selcols, schno, j);
			}
		}
	}
}
void lesseq(char *selcols, int schno, int colno, char *col, char *cond)
{
	int i, j, k = 0;
	for (j = 0; j < s[schno].records; j++)
	{
		if (strcmp(s[schno].datatype[colno], "int") == 0)
		{
			if (toInt(s[schno].d[j].theData[colno]) <= toInt(cond))
			{
				getCols(selcols, schno, j);
			}
		}
		else if (strcmp(s[schno].datatype[colno], "float") == 0)
		{
			if (atof(s[schno].d[j].theData[colno]) <= atof(cond))
			{
				getCols(selcols, schno, j);
			}
		}
	}
}
void greateq(char *selcols, int schno, int colno, char *col, char *cond)
{
	int i, j, k = 0;
	for (j = 0; j < s[schno].records; j++)
	{
		if (strcmp(s[schno].datatype[colno], "int") == 0)
		{
			if (toInt(s[schno].d[j].theData[colno]) >= toInt(cond))
			{
				getCols(selcols, schno, j);
			}
		}
		else if (strcmp(s[schno].datatype[colno], "float") == 0)
		{
			if (atof(s[schno].d[j].theData[colno]) >= atof(cond))
			{
				getCols(selcols, schno, j);
			}
		}
	}
}