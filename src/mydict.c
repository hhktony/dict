#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define WORD_LEN 128
#define EX_LEN 1024

typedef struct node {
	char word[64];
	long pos;
	struct node *pnext;
} node_t, *pnode_t;

int menu()
{
	int choice;	
	puts("\n=================");
	puts("|    Dict\t|\n|1:search word\t|\n|2:add a word\t|\n|3:exit Dict\t|");
	puts("=================");
	printf("please enter you choose: ");
	scanf("%d", &choice);
	getchar();
	return choice;
}

void print_ex(char *ex)
{
	ex += 6;
	char buf[EX_LEN];
	strcpy(buf, ex);
	ex = buf;
	ex = strtok(ex, "@");
	int i = 0;
	while (ex != NULL) {
		printf("%d.%s\n", ++i, ex);
		ex = strtok(NULL, "@");
	}
}

void get_explanation(pnode_t p)
{
	char buffer[1024];
	FILE *fp = fopen("dict.txt", "r+");
	if (fp == NULL) {
		perror("open fail.");
		exit(1);
	}

	fseek(fp, p->pos, SEEK_SET);
	fgets(buffer, WORD_LEN, fp);
	fgets(buffer, EX_LEN, fp);
	/*printf("%s", buffer);*/
	print_ex(buffer);

	fclose(fp);
}

int search(pnode_t p, char *w)
{
	while (p != NULL) {
		if (strcmp(p->word, w) == 0) {
			get_explanation(p);
			return 0;
		}
		p = p->pnext;
	}		

	return -1;
}

void print_similar(pnode_t p, char *w)
{
	int i = 1;
	char buffer[32];
	while (p != NULL) {
		if ((strncmp(p->word, w, strlen(w) - 1)) == 0) {
			printf("%s ", p->word);

			if (i++ % 15 == 0) {
				puts("press enter to continue...(press 'quit' to exit)");				
				fgets(buffer, 31, stdin);
				if (strncasecmp(buffer, "quit", 4) == 0)
					break;
			}
		}
		p = p->pnext;
	}
}

void search_for_word(pnode_t phead)
{
	char word[WORD_LEN];
	while (1) {
		printf("enter the word:(enter 'quit' to exit)\n");
		fgets(word, WORD_LEN-1, stdin);
		if (strncasecmp(word, "quit", 4) == 0)
			break;
		if (search(phead, word))
			print_similar(phead, word);
	}
}

void print_link(pnode_t p)
{
	int i = 0;
	while (p != NULL) {
		printf("%ld:%s", p->pos, p->word);
		p = p->pnext;
		if ((i++) % 1000 == 0) 
			getchar();
	}
}

pnode_t insert_to_link(pnode_t phead, char *w, long pos)
{
	pnode_t p;
	p = (pnode_t)malloc(sizeof(node_t));
	if (p == NULL) {
		printf("malloc fail.\n");
		exit(1);
	}

	strcpy(p->word, w);
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
	char w[WORD_LEN];

	fp = fopen("dict.txt", "r+");
	if (fp == NULL) {
		perror("open fail.");
		exit(1);
	}

	while ((ch = getc(fp)) != EOF) {
		if (ch == '#') {
			pos = ftell(fp);
			fgets(w, WORD_LEN-1, fp);

			phead = insert_to_link(phead, w, pos);
		}
	}

	fclose(fp);
	
	return phead;
}


bool get_info(char *w, char *ex)
{
	char ex_buf[1024];
	w[0] = '#';
	strcpy(ex, "Trans:");

	puts("please input word:");
	fgets(w+1, WORD_LEN, stdin);
	int i = 1;
	while(1)
	{
		printf("input explanation %d(input 'quit' to quit)\n", i++);
		fgets(ex_buf,EX_LEN, stdin);

		if(strncasecmp(ex_buf, "quit", 4) == 0)
			break;

		if(i > 2)
			ex[strlen(ex)-1] = '@';

		strcat(ex, ex_buf);
	}
	return true;
}

pnode_t add_to_file(char *w, char *ex, pnode_t phead)
{
	FILE *fp = fopen("dict.txt", "a");
	if(fp == NULL)
	{
		perror("open");
		exit(1);
	}
	long pos = ftell(fp);
	fwrite(w, 1, strlen(w), fp);
	fwrite(ex, 1, strlen(ex), fp);
	pnode_t p = malloc(sizeof(pnode_t ));
	if(p == NULL)
	{
		perror("malloc");
		exit(1);
	}
	strcpy(p->word, w+1);
	p->pos = pos;
	p->pnext = phead;

	fclose(fp);

	return p;

}

void move_info(long pos, char *w, char *ex)
{
	FILE *fp = fopen("dict.txt", "r+");
	if(fp == NULL)
	{
		perror("Open file dict.txt fail:");
		exit(1);
	}
	fseek(fp, pos, SEEK_SET);
	char c;
	while((c = getc(fp)) != EOF)
		if(c == '#')
			break;

	pos = ftell(fp)-1;

	FILE *fp_temp = fopen("temp.txt", "w+");
	if(fp_temp == NULL)
	{
		perror("open file temp.txt fail:");
		exit(1);
	}
	fwrite(w, 1, strlen(w), fp_temp);
	fwrite(ex, 1, strlen(ex), fp_temp);
	fseek(fp, pos, SEEK_SET);
	while((c = getc(fp)) != EOF)
	{
		putc(c, fp_temp);
	}

	rewind(fp_temp);
	fseek(fp, pos, SEEK_SET);
	while((c = getc(fp_temp)) != EOF)
	{
		putc(c, fp);
	}
	fclose(fp_temp);
	fclose(fp);		
}

pnode_t update_link(pnode_t p)
{
	pnode_t ptr = NULL;
	while(p != NULL)
	{
		ptr = p;
		p = p->pnext;
		free(ptr);
	}

	ptr = create_link();
	return ptr;
}

pnode_t insert_to_file(char *w, char *ex, pnode_t phead)
{
	pnode_t p = phead;
	while(p != NULL)
	{
		
		if(strcmp(p->word, w+1) < 0)
		{
			move_info(p->pos, w, ex);
			phead = update_link(phead);
			break;
		}
		p = p->pnext;
	}
	return phead;
}

pnode_t add_word(pnode_t phead)
{
	char w[WORD_LEN];
	char ex[EX_LEN];

	if(get_info(w, ex))
		phead = insert_to_file(w, ex, phead);

	return phead;
}

int main(int argc, char *argv[])
{
	pnode_t phead = NULL;
	int choice = 0;

	phead = create_link();

#ifdef DEBUG
	print_link(phead);
#endif

	while (1) {
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
