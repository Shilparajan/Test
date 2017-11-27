#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>


#define a 2
#define b 3
#define c 4
#define d 17
#define e 27
#define f 22
#define g 10

#define a1 14
#define b1 15
#define c1 18
#define d1 23
#define e1 24
#define f1 25
#define g1 8

    int zero[6] = {a,b,c,d,e,f};
    int one[2] = {b,c};
    int two[5] = {a,b,g,e,d};
    int three[5] = {a,b,c,d,g};
    int four[4] = {b,c,f,g};
    int five[5] = {a,c,d,f,g};
    int six[6] = {a,c,d,e,f,g};
    int seven[3] = {a,b,c};
    int eight[7] = {a,b,c,d,e,f,g};
    int nine[5] = {a,b,c,f,g};
    
    int zero1[6] = {a1,b1,c1,d1,e1,f1};
    int one1[2] = {b1,c1};
    int two1[5] = {a1,b1,g1,e1,d1};
    int three1[5] = {a1,b1,c1,d1,g1};
    int four1[4] = {b1,c1,f1,g1};
    int five1[5] = {a1,c1,d1,f1,g1};
    int six1[6] = {a1,c1,d1,e1,f1,g1};
    int seven1[3] = {a1,b1,c1};
    int eight1[7] = {a1,b1,c1,d1,e1,f1,g1};
    int nine1[5] = {a1,b1,c1,f1,g1};

#define PORT "3491"  
#define MAXDATASIZE 100 
#define BACKLOG 10    

//-----GPIO Constants
#define BCM2708_PERI_BASE       0x3F000000
#define GPIO_BASE               (BCM2708_PERI_BASE + 0x200000)  // GPIO controller
#define BLOCK_SIZE              (4*1024)

void sigchld_handler(int s)
{
    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;

    while(waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}
//--------GPIO Config-----------------
// IO Acces
struct bcm2835_peripheral {
    unsigned long addr_p;
    int mem_fd;
    void *map;
    volatile unsigned int *addr;
};

struct bcm2835_peripheral gpio = {GPIO_BASE};

void IN_GPIO(int h)
{
   *(gpio.addr + ((h)/10)) &= ~(7<<(((h)%10)*3));
}

void OUT_GPIO(int h)
{
     IN_GPIO(h);
    *(gpio.addr + ((h)/10)) |=  (1<<(((h)%10)*3));
}

void SetGPIO(int h)
{
   *(gpio.addr + 7) = 1 << h ;
}

void ClearGPIO(int h)
{
  *(gpio.addr + 10)  = 1 << h ;

}

//7 segment function
void Function1(char reg[],int Array[],int size)
{
        int idx;
        if(strcasecmp(reg,"ON") == 0)
        {
          printf("Inside on\n");
            for(idx=0; idx<7; idx++)
            {
                IN_GPIO(eight[idx]);
                OUT_GPIO(eight[idx]);
                SetGPIO(eight[idx]);
                printf("OFF %d\n",eight[idx]);
            }
           printf("OFF DONE\n");
           for(idx=0; idx<size; idx++)
           {
                IN_GPIO(Array[idx]);
                OUT_GPIO(Array[idx]);
                ClearGPIO(Array[idx]);
                printf("ON %d\n",Array[idx]);
           }
        }
        else if(strcasecmp(reg,"OFF") == 0)
        {
            for(idx=0; idx<size; idx++)
            {
                IN_GPIO(Array[idx]);
                OUT_GPIO(Array[idx]);
                SetGPIO(Array[idx]);
                printf("OFF %d\n",Array[idx]);
            }
        }
        return;
}

void Function2(char reg[],int Array[],int size)
{
        int idx;
        if(strcasecmp(reg,"ON") == 0)
        {
          printf("Inside on\n");
            for(idx=0; idx<7; idx++)
            {
                IN_GPIO(eight1[idx]);
                OUT_GPIO(eight1[idx]);
                SetGPIO(eight1[idx]);
                printf("OFF %d\n",eight1[idx]);
            }
           printf("OFF DONE\n");
           for(idx=0; idx<size; idx++)
           {
                IN_GPIO(Array[idx]);
                OUT_GPIO(Array[idx]);
                ClearGPIO(Array[idx]);
                printf("ON %d\n",Array[idx]);
           }
        }
        else if(strcasecmp(reg,"OFF") == 0)
        {
            for(idx=0; idx<size; idx++)
            {
                IN_GPIO(Array[idx]);
                OUT_GPIO(Array[idx]);
                SetGPIO(Array[idx]);
                printf("OFF %d\n",Array[idx]);
            }
        }
        return;
}


int map_peripheral(struct bcm2835_peripheral *p)
{
   
   if ((p->mem_fd = open("/dev/mem", O_RDWR) ) < 0)
   {
      printf("Failed to open /dev/mem, try checking permissions.\n");
      return -1;
   }

   p->map = mmap(
      NULL,
      BLOCK_SIZE,
      PROT_READ|PROT_WRITE,
      MAP_SHARED,
      p->mem_fd,      // File descriptor to physical memory virtual file '/dev/mem'
      p->addr_p       // Address in physical map that we want this memory block to expose
   );

   if (p->map == MAP_FAILED)
   {
        perror("mmap");
        return -1;
   }
   p->addr = (volatile unsigned int *)p->map;

   return 0;
}

int main(void)
{
    int sockfd, clientToken ;  // listen on sock_fd, new connection on clientToken
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    struct sigaction sa;
    int yes=1;
    char s[INET6_ADDRSTRLEN];
    int rv;
    int retVal ;
    int numbytes ;
    FILE *ptr;
    char filename[50];
    char message[30];
    char rasp[50];
    int ledStatus[26];
    int first,second;

    if(map_peripheral(&gpio) == -1)
    {
        printf("Failed to map the physical GPIO registers into the virtual memory space.\n");
        return -1;
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next)
    {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1)
        {
            perror("server: socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                sizeof(int)) == -1)
        {
            perror("setsockopt");
            exit(1);
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1)
        {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

  //freeaddrinfo(servinfo); // all done with this structure

    if (p == NULL)
    {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }

    retVal = listen(sockfd, BACKLOG);
    if (retVal == -1)
    {
        perror("listen");
        exit(1);
    }

    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    retVal = sigaction(SIGCHLD, &sa, NULL);

    if (retVal == -1)
    {
        perror("sigaction");
        exit(1);
    }

    printf("server: waiting for connections...\n");

    while(1)
    {  // main accept() loop
        sin_size = sizeof their_addr;
        clientToken = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (clientToken == -1)
        {
            perror("accept");
            continue;
        }

        inet_ntop(their_addr.ss_family,
            get_in_addr((struct sockaddr *)&their_addr),
            s, sizeof s);
        printf("server: got connection from %s\n", s);

        retVal = fork();
        if (retVal == 0)
        {
            
            close(sockfd);    
            strcpy(message, "Welcome to LED control");
            retVal = send(clientToken, message, strlen(message) , 0);
            if (retVal == -1)
            {
                perror("send");
            }

            while(1)
            {
               memset(&rasp[0], 0, sizeof(rasp));
               memset(&message[0], 0, sizeof(message));

                IN_GPIO(20);
                OUT_GPIO(20);
                SetGPIO(20);

                IN_GPIO(21);
                OUT_GPIO(21);
                SetGPIO(21);

               numbytes = recv(clientToken, rasp, sizeof(rasp), 0); 
               if (numbytes <= 0)
               {
                   printf("Receive failed \n");
               }

                sscanf(rasp,"%s %d %d",message, &first, &second);

               if(strcasecmp(message,"ON") == 0)
               {
                   printf("%d%d Displayed\n",first,second);
                   switch(first)
                   {
                      case 0:{
                          Function1(message,zero,6);
                          break;
                      }
                      case 1:{
                           Function1(message,one,2);
                           break;
                      }
                      case 2:{
                           Function1(message,two,5);
                           break;
                      }
                      case 3:{
                           Function1(message,three,5);
                           break;
                      }
                      case 4:{
                           Function1(message,four,4);
                           break;
                      }
                      case 5:{
                           Function1(message,five,5);
                           break;
                      }
                      case 6:{
                           Function1(message,six,6);
                           break;
                      }
                      case 7:{
                           Function1(message,seven,3);
                           break;
                      }
                      case 8:{
                           Function1(message,eight,7);
                           break;
                      }
                      case 9:{
                           Function1(message,nine,5);
                           break;
                      }
                   }
                   
                   switch(second)
                   {
                             case 0:{
                                Function2(message,zero1,6);
                                break;
                             }
                             case 1:{
                                Function2(message,one1,2);
                                break;
                             }
                             case 2:{
                                Function2(message,two1,5);
                                break;
                             }
                             case 3:{
                                Function2(message,three1,5);
                                break;
                             }
                             case 4:{
                                Function2(message,four1,4);
                                break;
                             }
                             case 5:{
                                Function2(message,five1,5);
                                break;
                             }
                             case 6:{
                                Function2(message,six1,6);
                                break;
                             }
                             case 7:{
                                Function2(message,seven1,3);
                                break;
                             }
                             case 8:{
                                Function2(message,eight1,7);
                                break;
                             }
                             case 9:{
                                Function2(message,nine1,5);
                                break;
                             }
                    }
                }

                else if (strcasecmp(message,"Quit") == 0)
                {
                    printf("Child:  is Quitting %s\n" ,rasp);
                    Function1("OFF",eight,7);
                    Function2("OFF",eight1,7);
                      break ;
                }
                else
                {
                    printf("Child else:  |%s|\n",rasp );
                }
            }
            close(clientToken);
            exit(0);
        }
        else if (retVal < 0)
        {
             printf("fork failed \n");
        }
        
        close(clientToken);  
   }
    return 0;
}
