#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <errno.h>
#include <netdb.h> 
#include <arpa/inet.h>

void IPToDomain();
void DomainToIP();

void IPToDomain(char *ip){
   struct in_addr InAddr;
   inet_aton(ip, &InAddr);
   struct hostent *Host = gethostbyaddr(&InAddr, sizeof(InAddr), AF_INET);
   if(Host == NULL){
      printf("Not found information\n");
   }else
   {
      printf("Official name: %s\n", Host->h_name);
      printf("Alias name:\n");
      while (*Host->h_aliases)
      {
         printf("%s\n",*Host->h_aliases);
         Host->h_aliases++;
      }
      printf("NULL\n"); 
   }
}

void DomainToIP(char *name){
   struct hostent *Host;
   struct in_addr **Addr_List;
   if((Host = gethostbyname(name)) == NULL){
      printf("Not found information\n");
   }else{

   Addr_List = (struct in_addr **)Host->h_addr_list;
   printf("Official IP: %s\n", inet_ntoa(*Addr_List[0]));
   printf("Alias IP: \n");
   if(Addr_List[1] == NULL){
      printf("NULL\n");
   }else
   {
      for (int i = 1; Addr_List[i] != NULL; i++)
      {
         printf("%s\n", inet_ntoa(*Addr_List[i]));
      }  
   }
   }
}

void main(int argc, char *argv[]){
   if(argc = 3 ){
      if(atoi(argv[1]) == 1){
         if(inet_addr(argv[2]) == -1){
            printf("Wrong parameter\n");
         }else{
         IPToDomain(argv[2]);}
      }else if(atoi(argv[1]) == 2)
      {
          if(inet_addr(argv[2]) != -1){
             printf("Wrong parameter\n");
          }else{
             DomainToIP(argv[2]);}
      }else
      {
         printf("Wrong parameter\n");
      }
      
   }else
   {
      printf("Wrong parameter\n");
   }
   
}