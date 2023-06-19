#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LINE_SIZE 300

struct Dir;
struct File;

typedef struct Dir{
	char *name;
	struct Dir* parent;
	struct File* head_children_files;
	struct Dir* head_children_dirs;
	struct Dir* next;
} Dir;

typedef struct File {
	char *name;
	struct Dir* parent;
	struct File* next;
} File;

File* mallocFile(Dir* parent,char* name){
	File* new_file = (File*)calloc(1, sizeof(File));
	new_file->name = (char*)calloc(1, strlen(name) + 1);

	strcpy(new_file->name, name);
	new_file->parent = parent;
	new_file->next = NULL;
	return new_file;
}

void touch (Dir* parent, char* name) {
	for(File* contor = parent->head_children_files; contor != NULL; contor = contor->next){
		if(strcmp(contor->name, name) == 0){
			printf("File already exists\n");
			return;
		}
	}
	File* new_file = mallocFile(parent,name);

	if(parent->head_children_files == NULL){
		parent->head_children_files = new_file;
		return;
	}
	for(File* contor = parent->head_children_files; contor != NULL; contor = contor->next){
		if(contor->next == NULL){
			contor->next = new_file;
			return;
		}
}
	return;
}

Dir* mallocDir(Dir* parent,char* name){
	Dir* new_dir = (Dir*)calloc(1, sizeof(Dir));
	new_dir->name = (char*)calloc(strlen(name) + 1, sizeof(char));
	strcpy(new_dir->name, name);
	new_dir->parent = parent;
	new_dir->head_children_dirs = NULL;
	new_dir->head_children_files = NULL;
	new_dir->next = NULL;
	return new_dir;
}

void mkdir (Dir* parent, char* name) {
	if(parent) {
		for(Dir* contor = parent->head_children_dirs; contor != NULL; contor = contor->next){
			if(strcmp(contor->name, name) == 0){
				printf("Directory already exists\n");
				return;
			}
		}
		
		Dir* new_dir = mallocDir(parent, name);
		if(parent->head_children_dirs == NULL){
			parent->head_children_dirs = new_dir;
			return;
		}
		for(Dir* contor = parent->head_children_dirs; contor != NULL; contor = contor->next){
			if(contor->next == NULL){
				contor->next = new_dir;
				return;
			}
		}
		return;
	}
}

void ls (Dir* parent) {
	for(Dir* contor = parent->head_children_dirs; contor != NULL; contor = contor->next){
		printf("%s\n", contor->name);
	}
	for(File* contor = parent->head_children_files; contor != NULL; contor = contor->next){
		printf("%s\n", contor->name);
	}
}

void rm (Dir* parent, char* name) {
	int found = 0;
	
	for(File* contor = parent->head_children_files; contor != NULL; contor = contor->next){
		if(strcmp(contor->name, name) == 0){
			found = 1;
			break;
		}
}
	if(!found){
		printf("Could not find the file\n");
		return;
	}
	if(strcmp(parent->head_children_files->name, name) == 0){
		File* aux = parent->head_children_files;
		parent->head_children_files = parent->head_children_files->next;
		free(aux->name);
		free(aux);
		return;
	}
	for(File* contor = parent->head_children_files; contor != NULL; contor = contor->next){
		if(strcmp(contor->next->name, name) == 0){
			File* aux = contor->next;
			contor->next = contor->next->next;
			free(aux->name);
			free(aux);
			return;
		}
	}
}

void rmdir (Dir* parent, char* name) {
	Dir* deleted;
	int found = 0;
	for(Dir* contor = parent->head_children_dirs; contor != NULL; contor = contor->next){
		if(strcmp(contor->name, name) == 0){
			found = 1;
			deleted = contor;
			break;
		}
	}
	if(!found){
		printf("Could not find the dir\n");
		return;
	}
	
	File* contor = deleted->head_children_files;
	while(contor != NULL){
		File* aux = contor;
		contor = contor->next;
		free(aux->name);
		free(aux);
	}

	for(Dir* contor = deleted->head_children_dirs; contor != NULL; contor = contor->next){
		rmdir(deleted, contor->name);
	}

	Dir* contor1 = parent->head_children_dirs;
	if(strcmp(contor1->name, name) == 0){
		parent->head_children_dirs = parent->head_children_dirs->next;
		free(contor1->name);
		free(contor1);
		return;
	}
	for(Dir* contor = parent->head_children_dirs; contor != NULL; contor = contor->next){
		if(strcmp(contor->next->name, name) == 0) {
			Dir* aux = contor->next;
			contor->next = contor->next->next;
			free(aux->name);
			free(aux);
			return;
		}
	}
	free(deleted->name);
	free(deleted);
}

void cd(Dir** target, char *name) {
	if(strcmp(name, "..") == 0){
		if((*target)->parent != NULL){
		(*target) = (*target)->parent;
		return;
		}
		return;
	}
	int found = 0;
	for(Dir* contor = (*target)->head_children_dirs; contor != NULL; contor = contor->next){
		if(strcmp(contor->name, name) == 0){
			found = 1;
			break;
		}
	}
	if(!found){
		printf("No directories found!\n");
		return;
	}
	for(Dir* contor = (*target)->head_children_dirs; contor != NULL; contor = contor->next){
		if(strcmp(contor->name, name) == 0){
			(*target) = contor;
			return;
		}
	}
	return;
}

char *pwd (Dir* target) {
	char* curent = malloc(sizeof(char) * MAX_INPUT_LINE_SIZE);
	char* cale = malloc(sizeof(char) * MAX_INPUT_LINE_SIZE);
	
	for(Dir* contor = target; contor != NULL; contor = contor->parent)
	{	
		strcpy(curent, "/");
		strcat(curent, contor->name); // ex: /c /b /a /home
		strcat(curent, cale); 
		strcpy(cale, curent);
	}
	free(curent);
	return cale; // ex: /home/a/b/c/d   
} 
 
void stop (Dir* target) {
}

void tree (Dir* target, int level) {
	int i;
	for(Dir* contor = target->head_children_dirs; contor != NULL; contor = contor->next){
		for(i = 1; i <= level; i++)
		{ printf("    ");}
		printf("%s\n", contor->name);
		tree(contor, level + 1);
	}
	for(File* contor = target->head_children_files; contor != NULL; contor = contor->next){
		for(i = 1; i <= level; i++)
		{ printf("    ");}
		printf("%s\n", contor->name);
	}
}

// void mv(Dir* parent, char *oldname, char *newname) {}

void removeDirList(Dir* list) {
	//trebuie sa dau free la directorul in cauza 
	if (!list) //daca lista de directoare este nula, ma opresc (cazul de baza)
	return;
	free(list->name);
	//eliberez fisierele din directorul curent
	while (list->head_children_files) {
		File* aux = list->head_children_files;
		list->head_children_files = list->head_children_files->next;
		free(aux->name);
		free(aux);
	}
	//apelez recursiv functia pentru a-mi sterge directoarele din interiorul directorului curent
	removeDirList(list->head_children_dirs);
	//sterg directorul urmator
	removeDirList(list->next);
	free(list);
}

void removeDir(Dir* root) {
	if (!root)
		return;
	free(root->name);
	while (root->head_children_files) {
		File* aux = root->head_children_files;
		root->head_children_files = root->head_children_files->next;
		free(aux->name);
		free(aux);
	}
	removeDirList(root->head_children_dirs);
	free(root);
}

int main () {

	Dir* root = mallocDir(NULL, "home");
	Dir *directory = root;

	char *line = (char*)malloc(MAX_INPUT_LINE_SIZE * sizeof(char));
	char *command;

	fgets(line, MAX_INPUT_LINE_SIZE, stdin);
	do{
		command = strtok(line, " \n");
		if(strcmp(command, "touch") == 0) 
			touch(directory, strtok(NULL, " \n"));
		else
			if(strcmp(command, "mkdir") == 0) {
				mkdir(directory, strtok(NULL, " \n"));
			}
		else
			if(strcmp(command, "ls") == 0)
				ls(directory);
		else
			if(strcmp(command, "rm") == 0)
				rm(directory, strtok(NULL, " \n"));
		else
			if(strcmp(command, "rmdir") == 0)
				rmdir(directory,strtok(NULL, " \n"));
		else
			if(strcmp(command, "cd") == 0)
				cd(&directory,strtok(NULL, " \n"));
		else
			if(strcmp(command, "stop") == 0) {
				break;
			}
		else 
			if (strcmp(command, "pwd") == 0) {
				char *str = pwd(directory);
				printf("%s\n", str);
				free(str);
			}
		else 
			if (strcmp(command, "tree") == 0) {
				tree(directory, 0);
			}
	} while(fgets(line, MAX_INPUT_LINE_SIZE, stdin));

	removeDir(root);
	free(line);

	return 0;
}
