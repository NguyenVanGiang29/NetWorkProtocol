#include <stdio.h>
#include <string.h>
#include <stdlib.h>
typedef struct node {
  	char username[20];
	char password[20];
	int status;
	struct node *next;
}node;
node *head = NULL;
void insert(char username[20], char password[20], int status){
	node *temp;
	temp=(node *)malloc(sizeof(node));
	strcpy(temp->username, username);
	strcpy(temp->password, password);
	temp->status = status;
	temp->next = head;
	head = temp;
}
node *find(char name[20]){
   node *current = head;
   if(head == NULL)
	{
      return NULL;
    }
   while(strcmp(current->username, name) != 0){
      if(current->next == NULL){
         return NULL;
      }else {
         current = current->next;
      }
   }      
   return current;
}
node *findpass(char pass[20]){
   node *current = head;
   if(head == NULL)
	{
      return NULL;
    }
   while(strcmp(current->password, pass) != 0){
      if(current->next == NULL){
         return NULL;
      }else {
         current = current->next;
      }
   }      
   return current;
}
void printAll()
{
    node *temp;
    temp = head;
    while(temp)
    {
    	printf("%20s %20s %d",temp->username, temp->password, temp->status);
        printf("\n");
        temp=temp->next;
    }
    printf("\n");
}
void openFile(){
	node *acc;
	char *username;
	char *password;
	int status;
	char c;
	int u = 0, p = 0, blank = 0;
	username = (char *)malloc(20);
	password = (char *)malloc(20);
    FILE *fptr;
	if((fptr=fopen("account.txt","r+"))==NULL){
		printf("Not find%s\n","account.txt");
		return;
	}
	while(1){
		fscanf(fptr,"%s",username);
		fscanf(fptr,"%s",password);
		fscanf(fptr,"%d",&status);
		if(feof(fptr)) break;
		insert(username, password, status);
	}
	free(username); free(password);
	fclose(fptr);
}
void writeFile(){
	FILE *fptr;
	node *temp;
    temp = head;
    fptr=fopen("account.txt","w+");
    while(temp){
    	fprintf(fptr, "%s %s %d", temp->username, temp->password, temp->status);
    	fprintf(fptr, "\n");
    	temp=temp->next;
    }
    fclose(fptr);
}
void registerAccount(){
	char username[20];
	char password[20];
	printf("---------Register\n");
	printf("Username: "); scanf("%s", username);
	printf("\n");
	printf("Password: "); scanf("%s", password);
	printf("\n");
	if(find(username) != NULL){
	printf("Account existed!");	
	} 
	else{
		insert(username, password, 2);
		writeFile();
		printf("Successful registration. Activation required.\n");
		printAll();
	}
	printf("\n");
}
void Activate(){
	char usename[20];
	char password[20];
	char activationcode[20];
	char code[20] = "LTM121216";
	int countlogin = 0;
	printf("TO ACTIVATE ACCOUNT- PLEASE GIVE SOME INFORMATION\n");
	printf("Usename: "); scanf("%s", usename);
	printf("Password:"); scanf("%s",password);
	node *acc = find(usename);
	while (countlogin <5)
	{
		printf("Code:"); scanf("%s", activationcode);
		if(strcmp(activationcode, code) == 0){
			acc->status = 1;
			writeFile();
			printf("Account is activated\n");
			countlogin = 6;
		}else
		{
			printf("Account is not activated\n");
			countlogin +=1;
		}
	}
	if(countlogin == 5){
		acc->status = 0;
		writeFile();
		printf("Activation code is incorrect. Account is blocked\n");
	}
}

node *signin(){
	char username[20];
	char password[20];
	int login_count = 0;
	printf("---------Sign In\n");
	printf("Username: "); scanf("%s", username);
	printf("\n");
	node *acc = find(username);
	if(acc != NULL){
		if(acc->status == 0){
			printf("Account has been blocked!\n");
			return NULL;
		}
		do{
		printf("Password: "); scanf("%s", password);
		printf("\n");
		if(strcmp(acc->password,password) == 0){
			printf("Hello %s\n", acc->username);
			return acc;
			}
		else {
			printf("Password is incorrect.Account is blocked\n");
			login_count++;
			}
		}while(login_count < 3);
		if(login_count == 3) {
			printf("Account is blocked\n");
			acc->status = 0;
			writeFile();
			return NULL;
		}
	}
	else{
		printf("Cannot find account\n");
		return NULL; 
	} 
}
void search(){
	char username[20];
	printf("---------Search\n");
	printf("Username: "); scanf("%s", username);
	printf("\n");
	node *acc = find(username);
	if(acc != NULL){ 
		if(acc->status = 1){
			printf("Account is active\n");
		}else if(acc->status = 0)
		{
			printf("Account is blocked\n");
		}
		
	}
	else printf("Cannot find account\n");
}
node *Changepassword(){
	char passwordcurrent[20];
	printf("CHANGE PASSWORD\n");
	printf("Password current: \n"); scanf("%s", passwordcurrent);
	node *acc = findpass(passwordcurrent);
	if(acc == NULL){
		printf("Password incorect! Please try again!\n");
		return NULL;
	}else return acc;

	
}
node *signout(){
	char username[20];
	printf("---------Sign Out\n");
	printf("Username: "); scanf("%s", username);
	printf("\n");
	node *acc = find(username);
	if(acc == NULL){
		printf("Cannot find account\n");
		return NULL;
	}
	else return acc;
}
int choice(){
	int c = 0;
	printf("USER MANAGEMENT PROGRAM\n");
	printf("-----------------------------------\n");
	printf("1. Register\n");
	printf("2. Activate\n");
	printf("3. Sign in\n");
	printf("4. Search\n");
	printf("5. Change passowrd\n");
	printf("6. Sign out\n");
	printf("Your choice (1-6, other to quit):\n"); scanf("%d", &c);
	return c;	
}
int main()
{
	openFile();
	printAll();
	char newpassword[20];
	int ch;
	node *login_acc = NULL;
	node *signout_acc = NULL;
	node *changepassword_acc = NULL;
	do{
		ch = choice();
		switch(ch){
		case 1: registerAccount(); break;
		case 2: Activate(); break;
		case 3: login_acc = signin(); break;
		case 4: {
			if(login_acc != NULL) {
				search();
			}
			else printf("Sign in, please!\n");
		} break;
		case 5: if(login_acc == NULL) {
			printf("Sgin in, please!\n");
		}else
		{
			changepassword_acc = Changepassword();
			if(strcmp(changepassword_acc->password, login_acc->password) == 0){
				printf("New password:\n"); scanf("%s", newpassword);
				strcpy(login_acc->password, newpassword);
				writeFile();
				printf("Password is changed\n");
			}
			else
			{
				printf("Password incorect!\n");
			}
		} break;
		case 6: {
			if(login_acc == NULL) printf("Account is not sign in. Sign in, please!\n");
			else{
				signout_acc = signout();
				if(signout_acc)
				if(strcmp(signout_acc->username,login_acc->username) == 0){
					printf("Goodbye %s\n", login_acc->username);
					login_acc = NULL;
				}
				else printf("Wrong Username!\n");
			}			
		} break;
		
	}
	}
	while(ch > 0 && ch < 7);

	
	return 0;
}