#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#define MAXLINE 100
#define MAX_LISTEN_QUEUE 100

int count = 0, check = 0;
char message[200] = "Goodbye ", code[200];

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

void sendMess(char *content, int sockfd, struct sockaddr *servaddr){
	int len, sendBytes;
	  
	len = sizeof(*servaddr);

	sendBytes = send(sockfd, content, strlen(content) +1, 0);

	if(sendBytes < 0){
		perror("Error: \n");
	}
	
}

int CheckString(char a[100]){		
	for(int i = 0; i < strlen(a); i++){
		if(a[i] >= 'A' && a[i] <= 'Z' || a[i] >= 'a' && a[i] <= 'z' || a[i] >= '0' && a[i] <= '9'){
		}else
		{
			return 0;
		}
	}
	return 1;
}

char* Encode(char Str[200]){
	char NewStr[200];
	int count = 0;

	for(int i = 0; i < strlen(Str); i++)
		if(Str[i] >= '0' && Str[i] <= '9'){
			NewStr[count] = Str[i];
			count++;
		}

	for(int i = 0; i < strlen(Str); i++){
		if(Str[i] < '0' || Str[i] > '9'){
			NewStr[count] = Str[i];
			count++;
		}
	}

	NewStr[count] = '\0';
    return strdup(NewStr);
}

int main(int argc, char* argv[]){

    openFile();


    int login = 0;
    char server_mesg[200];
    int listenfd, connfd, n, portNumber;
    pid_t childpid;
	fd_set currentSockets, readySockets;
    socklen_t clilen;
    char buff[MAXLINE];
    struct sockaddr_in cliaddr, servaddr;

    portNumber = atoi(argv[1]);


    // construct socket
    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    // bind socket to ip address
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(portNumber);

    bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

    listen(listenfd, MAX_LISTEN_QUEUE);

    printf("%s\n", "Server running...waiting for connections.");

    node *acc;

    // communicate with client
    while(1){
        clilen = sizeof(cliaddr);
        connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &clilen);
        printf("%s\n", "Received request...");
        
        while(n = recv(connfd, buff, MAXLINE, 0) > 0){

			if(buff[0] == '\0')
			{
				exit(0);
			}else
			{
				printf("\'%s\'\n", buff);
				switch (login)
				{
				case 0:
					acc = find(buff);
					if(acc == NULL){
						sendMess("Wrong account", connfd, (struct sockaddr*) &cliaddr);
					}else
					{
						sendMess("Insert password", connfd, (struct sockaddr*) &cliaddr);
						login = 1;
					}
					break;

				case 1:
					if(strcmp(acc->password, buff) == 0){
						if(acc->status ==1){
							sendMess("OK", connfd, (struct sockaddr*) &cliaddr);
							login = 2;
						}else
						{
							sendMess("Account not ready", connfd, (struct sockaddr*) &cliaddr);
							login = 0;
						}

						count = 0;
						
					}else
					{
						count = count +1;
						if(count >= 3){
							sendMess("Account is blocked", connfd, (struct sockaddr*) &cliaddr );
							count = 0;
							acc->status = 0;
							writeFile();
							login = 0;
						}else
						{
							sendMess("NOT OK", connfd, (struct sockaddr*) &cliaddr);
						}
						
					}
					break;
				
				case 2:
					if(strcmp(buff, "bye") == 0){
						strcat(message, acc->username);
						sendMess(message, connfd, (struct sockaddr*) &cliaddr);
						login = 0;
						count = 0;
					}else{
						check = CheckString(buff);
						if(check == 1){
						strcpy(acc->password, buff);
						writeFile();
						code[0] = '\0';
						strcat(code, Encode(acc->password));
						sendMess(code, connfd, (struct sockaddr*) &cliaddr);
						}else
						{
							sendMess("Error", connfd, (struct sockaddr*) &cliaddr);
						}
					}
					break;
					break;
				default:
					break;
				}
		}      
        }

        if(n < 0){
            perror("Read error: ");
            exit(-1);
        }

        close(connfd);
    }

    close(listenfd);

    return 0;
}