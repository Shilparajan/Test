#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT "3491"       
#define MAXDATASIZE 100        

    int sockfd, numbytes;
    char buf[MAXDATASIZE];
    struct addrinfo hints, *servinfo, *p;
    int rv;
    int retVal ;
    char s[INET6_ADDRSTRLEN];
    char message[30];
    unsigned long int ledNo;
    
    FILE *fptr;
	int num[10];
	int mulp3[10];
	int mulp7[10];
  int n,x,idx,i=0,j=0,ret;
	int temp; 
	int largest,smallest;


void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void sendToSer(char option[],int num)
{
	int first;
	int second;
	
	memset(&message,0,sizeof(message));
        memset(&buf,0,strlen(buf));

        first = num/10;
	second = num%10;
	    
        sprintf(message,"%s %d %d", option, first, second);
        retVal=send(sockfd, message, sizeof(message) , 0); 
        if(retVal == -1)  
        {
            perror("send failed");
        }
    return;
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr,"usage: client hostname\n");
        exit(1);
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

       
    for(p = servinfo; p != NULL; p = p->ai_next)
    {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1)
        {
            perror("client: socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1)
        {
            close(sockfd);
            perror("client: connect");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        return 2;
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
            s, sizeof s);
    printf("client: connecting to %s", s);

    freeaddrinfo(servinfo);      

    numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0);  
    if (numbytes == -1)
    {
        perror("recv");
        exit(1);
    }

    buf[numbytes] = '\0';
    printf("client: received '%s'\n",buf);
	
	


	memset(&num, 0, sizeof num);
	memset(&mulp3, 0, sizeof mulp3);
	memset(&mulp7, 0, sizeof mulp7);
	
	fptr = fopen("somenumber.txt","w");
	if(fptr==NULL)
	{
		perror("File");
		exit(0);
	}

        printf("NumX\n");
        x=0;
	for(idx=0;idx<10;idx++)
	{
	    x+=7;
	    fprintf(fptr,"%d ",x);
	}
	fclose(fptr);
	
	fptr = fopen("somenumber.txt","r");
	if(fptr==NULL)
	{
		perror("File");
		exit(0);
	}
	for(idx=0;idx<10;idx++)
	{
	    ret=fscanf(fptr,"%d",&num[idx]);
        }
        fclose(fptr);
    
        printf("Num");
        for(idx=0;idx<10;idx++)
        {
            printf("%d",num[idx]);
    	    sendToSer("ON",num[idx]);
            sleep(1); 
	}
    
    
        largest = num[0];
        smallest = num[0];
        for(idx=0;idx<10;idx++)
        {
    	    if(largest<num[idx])
    	    {
    		largest = num[idx];
	    }
	    if(smallest>num[idx])
	    {
		smallest = num[idx];
	    }
	}
	
	printf("Largest Number is %d",largest);
	sendToSer("ON",largest);
            sleep(1); 
	printf("Smallest Number is %d",smallest);
	sendToSer("ON",smallest);
            sleep(1); 
	

        for(idx=0;idx<10;idx++)
        {
    	    if((ret=num[idx] % 3 )== 0)
    	    {
		  mulp3[i] = num[idx];
    	          i++;
    	    }
    	
  	    if((ret=num[idx] % 7 )== 0)
    	    {
		  mulp7[j] = num[idx];
    	          j++;
    	    }
	}
	
	printf("Multiples of 3");
	for(idx=0;idx<10;idx++)
	{
		printf("%d",mulp3[idx]);
		sendToSer("ON",mulp3[idx]);
                 sleep(1); 
	}
    
  printf("Multiples of 7");
	for(idx=0;idx<10;idx++)
	{
		printf("%d",mulp7[idx]);
		sendToSer("ON",mulp7[idx]);
                 sleep(1); 
	}
	

        printf("Ascending order");
        for (i=0; i <10 ; i++)
        {
            for (j=i+1; j < 10 ; j++)
            {
               if (num[i] > num[j] )
               {
                   temp = num[i];
                   num[i] = num[j];
                   num[j] = temp ;
               }
            }
        }
        for(idx=0;idx<10;idx++)
	{
		printf("%d",num[idx]);
		sendToSer("ON",num[idx]);
                 sleep(1); 
	}
	 
     
        printf("Descending order");
        for (i=0; i <10 ; i++)
        {
             for (j=i+1; j < 10 ; j++)
             {
                 if (num[i] < num[j] )
                 {
                     temp = num[i];
                     num[i] = num[j];
                     num[j] = temp ;
                 }
             }
        }
        for(idx=0;idx<10;idx++)
	{
		printf("%d",num[idx]);
		sendToSer("ON",num[idx]);
                 sleep(1); 
	}

	sendToSer("Quit",0);
    
    close(sockfd);
    return 0;
}
