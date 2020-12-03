#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <errno.h>
#include <netdb.h> 
#include <arpa/inet.h>

typedef struct node {
  	char username[20];
	char password[20];
	int status;
	char homepage[200];
	struct node *next;
}node;
node *head = NULL;
void insert(char username[20], char password[20], int status, char homepage[200]){
	node *temp;
	temp=(node *)malloc(sizeof(node));
	strcpy(temp->username, username);
	strcpy(temp->password, password);
	temp->status = status;
	strcpy(temp->homepage, homepage);
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
    	printf("%20s %20s %d %200s",temp->username, temp->password, temp->status, temp->homepage);
        printf("\n");
        temp=temp->next;
    }
    printf("\n");
}
void openFile(){
	node *acc;
	char *username;
	char *password;
	char *homepage;
	int status;
	char c;
	int u = 0, p = 0, blank = 0;
	username = (char *)malloc(20);
	password = (char *)malloc(20);
	homepage = (char *)malloc(200);
    FILE *fptr;
	if((fptr=fopen("account.txt","r+"))==NULL){
		printf("Not find%s\n","account.txt");
		return;
	}
	while(1){
		fscanf(fptr,"%s",username);
		fscanf(fptr,"%s",password);
		fscanf(fptr,"%d",&status);
		fscanf(fptr,"%s",homepage);
		if(feof(fptr)) break;
		insert(username, password, status, homepage);
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
    	fprintf(fptr, "%s %s %d %s", temp->username, temp->password, temp->status, temp->homepage);
    	fprintf(fptr, "\n");
    	temp=temp->next;
    }
    fclose(fptr);
}
void registerAccount(){
	char username[20];
	char password[20];
	char homepage[200];
	printf("---------Register\n");
	printf("Username: "); scanf("%s", username);
	printf("\n");
	printf("Password: "); scanf("%s", password);
	printf("\n");
	int choice =0;
	printf("Choice Homepage(IP(choice = 1) or Domain(choice = 2)):\n");
	scanf("%d",&choice);
	
	switch (choice)
	{
	case 1:
		printf("Make IP:");
		scanf("%s",homepage);
		break;
	case 2:
		printf("Make Domain:");
		scanf("%s",homepage);
		break;
	default:
		break;
	}

	if(find(username) != NULL){
	printf("Account existed!");	
	} 
	else{
		insert(username, password, 2, homepage);
		writeFile();
		printf("Successful registration. Activation required.\n");
		// printAll();
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

//network
void GetDomain(char *ip){
   struct in_addr InAddr;

   if(inet_addr(ip) != -1){
   inet_aton(ip, &InAddr);
   struct hostent *Host = gethostbyaddr(&InAddr, sizeof(InAddr), AF_INET);
   if(Host == NULL){
      printf("Not found information\n");
   }else
   {
      printf("Homepage with domain name: %s\n", Host->h_name);
   }
   }else
   {
      printf("Homepage with domain name: %s\n", ip);
   }
   
}

void GetIP(char *name){
   struct hostent *Host;
   struct in_addr **Addr_List;
   if(inet_addr(name) == -1){
   	if((Host = gethostbyname(name)) == NULL){
    	printf("Not found information\n");
   	}else{

   		Addr_List = (struct in_addr **)Host->h_addr_list;
   		printf("Homepage with IP address: %s\n", inet_ntoa(*Addr_List[0]));
   	}	
   }else
   {
      printf("Homepage with IP address: %s\n", name);
   }   
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
	printf("7. Homepage with domain name\n"); 
	printf("8. Homepage with IP address\n");
	printf("Your choice (1-8, other to quit):\n"); scanf("%d", &c);
	return c;	
}
int main()
{
	openFile();
	// printAll();
	char newpassword[20];
	char CurrentPassword[20];
	int ch;
	node *login_acc = NULL;
	node *signout_acc = NULL;
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
			printf("Current password:");
			scanf("%s",CurrentPassword);
			if(strcmp(CurrentPassword, login_acc->password) == 0){
				printf("New password:");
				scanf("%s",newpassword);
				strcpy(login_acc->password, newpassword);
				writeFile();
				printf("Password is changed!\n");
			}else
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
		case 7:
			if(login_acc == NULL){
				printf("Sgin in, please!\n");
			}else
			{
				GetDomain(login_acc->homepage);
			}
		   break;
		case 8:
			if(login_acc == NULL){
				printf("Sgin in, please!\n");
			}else
			{
				GetIP(login_acc->homepage);
			}
			break;
	}
	}
	while(ch > 0 && ch < 9);
	return 0;
}
//giang