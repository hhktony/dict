#include "dict.h"

int menu()
{
	int choice;
	puts("\n=================");
	puts("|    Dict\t|\n|1: search word\t|\n|2: add a word\t|\n|3: exit Dict\t|");
	puts("=================");
	printf("please enter you choose: ");
	scanf("%d", &choice);
	getchar();
	return choice;
}

void print_explain(char *explain)
{
	explain += 6;
	char buf[EXPLAIN_LEN];
	strcpy(buf, explain);
	explain = buf;
	explain = strtok(explain, "@");
	int i = 0;

	while (explain != NULL)
	{
		printf("%d.%s\n", ++i, explain);
		explain = strtok(NULL, "@");
	}
}

void get_explanation(pnode_t p)
{
	char buffer[1024];
	FILE *fp = fopen(DICT_FILENAME, "r+");
	if (fp == NULL)
		IERROR("Open '%s'failed!", DICT_FILENAME);

	fseek(fp, p->pos, SEEK_SET);
	fgets(buffer, WORD_LEN, fp);
	fgets(buffer, EXPLAIN_LEN, fp);
	print_explain(buffer);

	fclose(fp);
}

int search(pnode_t p, char *word)
{
	while (p != NULL)
	{
		if (strcmp(p->word, word) == 0)
		{
			get_explanation(p);
			return 0;
		}
		p = p->pnext;
	}

	return -1;
}

void print_similar(pnode_t p, char *word)
{
	int i = 1;
	char buffer[32];

	while (p != NULL)
	{
		if ((strncmp(p->word, word, strlen(word) - 1)) == 0)
		{
			printf("%s ", p->word);

			if (i++ % 15 == 0)
			{
				puts("press enter to continue...(press '3' to exit)");
				fgets(buffer, 31, stdin);
				if (atoi(buffer) == 3)
					break;
			}
		}
		p = p->pnext;
	}
}

void search_for_word(pnode_t phead)
{
	char word[WORD_LEN];

	while (1)
	{
		printf("enter the word:(enter '3' to exit)\n");
		fgets(word, WORD_LEN - 1, stdin);
        if (atoi(word) == 3)
		    break;  
		if (search(phead, word))
			print_similar(phead, word);
	}
}

void print_link(pnode_t p)
{
	int i = 0;
	while (p != NULL)
	{
		IDEBUG("%ld:%s", p->pos, p->word);
		p = p->pnext;
		if ((i++) % 1000 == 0)
			getchar();
	}
}

pnode_t insert_to_link(pnode_t phead, char *word, long pos)
{
	pnode_t p;

	p = (pnode_t) malloc(sizeof(node_t));
	if (p == NULL)
	{
		printf("malloc fail.\n");
		exit(1);
	}

	strcpy(p->word, word);
	p->pos = pos;
	p->pnext = phead;

	return p;
}

pnode_t create_link(void)
{
	pnode_t phead = NULL;
	FILE *fp;
	int ch;
	long pos;
	char word[WORD_LEN];

	fp = fopen(DICT_FILENAME, "r+");
	if (fp == NULL)
	{
		IERROR("open file '%s' error", DICT_FILENAME);
		exit(1);
	}

	while ((ch = getc(fp)) != EOF)
	{
		if (ch == '#')
		{
			pos = ftell(fp);
			fgets(word, WORD_LEN - 1, fp);

			phead = insert_to_link(phead, word, pos);
		}
	}

	fclose(fp);

	return phead;
}

bool get_info(char *word, char *explain)
{
	char ex_buf[1024];
	word[0] = '#';
	strcpy(explain, "Trans:");

	puts("please input word('3' to exit): ");
	fgets(word + 1, WORD_LEN, stdin);

    if (atoi(word+1) == 3)
        return false;

	int i = 1;
	while (1)
	{
		printf("input explanation %d(input '3' to exit)\n", i++);
		fgets(ex_buf, EXPLAIN_LEN, stdin);

		if (atoi(ex_buf) == 3)
			break;

		if (i > 2)
			explain[strlen(explain) - 1] = '@';

		strcat(explain, ex_buf);
	}

	return true;
}

pnode_t add_to_file(char *word, char *explain, pnode_t phead)
{
	FILE *fp = fopen(DICT_FILENAME, "a");
	if (fp == NULL)
	{
		perror("open");
		exit(1);
	}
	long pos = ftell(fp);
	fwrite(word, 1, strlen(word), fp);
	fwrite(explain, 1, strlen(explain), fp);
	pnode_t p = malloc(sizeof(pnode_t));
	if (p == NULL)
	{
		perror("malloc");
		exit(1);
	}
	strcpy(p->word, word + 1);
	p->pos = pos;
	p->pnext = phead;

	fclose(fp);

	return p;
}

void move_info(long pos, char *word, char *explain)
{
    pid_t pid;
    char tempfile[32];
	FILE *fp;
    
    if ((fp = fopen(DICT_FILENAME, "r+")) == NULL)
		IERROR("Open file '%s' failed!", DICT_FILENAME);

	fseek(fp, pos, SEEK_SET);
	char c;
	while ((c = getc(fp)) != EOF)
		if (c == '#')
			break;

	pos = ftell(fp) - 1;

    pid = getpid();
    x_memzero(tempfile, sizeof(tempfile));
    snprintf(tempfile, sizeof(tempfile), "/tmp/tmp_%d\n", pid);

	FILE *fp_temp = fopen(tempfile, "word+");
	if (fp_temp == NULL)
		IERROR("Open file '%s' failed!", tempfile);

	fwrite(word, 1, strlen(word), fp_temp);
	fwrite(explain, 1, strlen(explain), fp_temp);
	fseek(fp, pos, SEEK_SET);
	while ((c = getc(fp)) != EOF)
	{
		putc(c, fp_temp);
	}

	rewind(fp_temp);
	fseek(fp, pos, SEEK_SET);
	while ((c = getc(fp_temp)) != EOF)
	{
		putc(c, fp);
	}

	fclose(fp_temp);
	fclose(fp);
}

pnode_t update_link(pnode_t p)
{
	pnode_t ptr = NULL;
	while (p != NULL)
	{
		ptr = p;
		p = p->pnext;
		free(ptr);
	}

	ptr = create_link();
	return ptr;
}

pnode_t insert_to_file(char *word, char *explain, pnode_t phead)
{
	pnode_t p = phead;
	while (p != NULL)
	{

		if (strcmp(p->word, word + 1) < 0)
		{
			move_info(p->pos, word, explain);
			phead = update_link(phead);
			break;
		}
		p = p->pnext;
	}
	return phead;
}

pnode_t add_word(pnode_t phead)
{
	char word[WORD_LEN];
	char explain[EXPLAIN_LEN];

	if (get_info(word, explain))
		phead = insert_to_file(word, explain, phead);

	return phead;
}

int main(int argc, char *argv[])
{
	pnode_t phead = NULL;
	int choice = 0;

	phead = create_link();

#ifndef __NODEBUG__
	print_link(phead);
#endif

	while (1)
	{
        system("clear");
		choice = menu();
		if (choice == 1)
			search_for_word(phead);
		else if (choice == 2)
			phead = add_word(phead);
		else if (choice == 3)
			break;
	}

	return 0;
}
