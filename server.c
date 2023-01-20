#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>

typedef struct card{
  char* value;
  char* suit;
} card;

void ctrlc();

int main()
{
    struct card* deck = (struct card*)malloc(52*sizeof(struct card));
    char* values[13] = {"Ace", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", "Jack", "Queen", "King"};
    char* suits[4] = {"Clubs", "Diamonds", "Hearts", "Spades"};
    
    int s1 = socket(AF_UNIX, SOCK_STREAM, 0);
    if(s1 == -1)
    {
        return -1;
    }
    struct sockaddr name1 = {AF_UNIX, "server1"};
    socklen_t namelen1 = sizeof(struct sockaddr) + 8;
    bind(s1, &name1, namelen1);
    
    if(listen(s1, 10) == -1)
    {
        printf("\nlisten fail\n");
    }

    fflush(stdout);
    printf("listening....\n");
    char* buf;
    
    struct sigaction news;
    struct sigaction olds;
    
    void ctrlc()
    {
        unlink("server1");
        if(buf != NULL)
            free(buf);
        if(deck)
            free(deck);
        sigaction( SIGINT, &olds, NULL);
        printf("\n\nserver term\n\n");
        exit(1);
    }
             
    news.sa_handler = ctrlc;
    news.sa_flags = 0;
    sigaction(SIGINT, &news, &olds);
    
    while(1)
    {
        
        int conn = accept(s1, NULL, NULL);

        for (int i = 0; i < 52; i++)
        {
            deck[i].value = values[i%13];
            deck[i].suit = suits[i%4];
        }
        
        int swap = 0;
        struct card tmp = {"", ""};
        srand(time(NULL));
        
        for (int j = 0; j < 52; j++)
        {
            swap = rand() % 52;
            tmp = deck[j];
            deck[j] = deck[swap];
            deck[swap] = tmp;
        }
        
        printf("The Deck:\n");
        for(int y = 0; y < 52; y++)
        {
            printf("%5s of %-12s", deck[y].value, deck[y].suit);
            if(0==((y+1)%3))
            {
                printf("\n");
            }
        }
        
        buf = (char*)malloc(4*sizeof(struct card));
        for(int i = 0; i < 4; i++)
        {
            strcat(buf, deck[i].value);
            strcat(buf, ".");
            strcat(buf, deck[i].suit);
            strcat(buf, ".");
        }
        write(conn, buf, strlen(buf));
        close(conn);
        sleep(1);
    }
    return 0;
}
