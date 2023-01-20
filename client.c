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

int checkCards(struct card* hand);
int* inswapsort(struct card* hand);

int cmpfunc (const void * a, const void * b)
{
   return ( *(int*)a - *(int*)b );
}

int main()
{
    int coins = 100;
    int wins = 0;
    int losses = 0;
    int matches = 0;
    int bet;
    
    int boo;
    printf("Its time to play the game\nHere are the bet multipliers:\n4 of a Kind:  20,825 times the bet + the bet back\nStraight Flush :  6,153 times the bet + the bet back\n3 of a Kind:  108 times the bet + the bet back\nStraight :  98 times the bet + the bet back\n2 Pair:  96 times the bet + the bet back\nFlush:  96 times the bet + the bet back\nPair:  3 times the bet + the bet back\nanything else  :  loss of bet\n You start with 100 coins\n");
    
    struct sockaddr name2 = {AF_UNIX, "server1"};
    socklen_t namelen2 = sizeof(struct sockaddr) + 8;
    char BUFF[2048];
    struct card* hand;
    
    do
    {
    
    printf("\nCoins: %d", coins);
    printf("\nhow much would you like to bet?\n");
    scanf("%d", &bet);
    while(bet>coins)
    {
        printf("\nnot enough coins\nTry again with a new bet:");
        scanf("%d", &bet);
    }
    matches++;
        
    int s0 = socket(AF_UNIX, SOCK_STREAM, 0);
    int con = connect(s0, &name2, namelen2);
    
    read(s0, BUFF, 2048);
    printf("\n");
    
    hand = (struct card*)malloc(4*sizeof(struct card));
        
    char* token = strtok(BUFF, ".");
    
    for(int i = 0; i < 4; i++)
    {
        hand[i].value = token;
        token = strtok(NULL, ".");
        hand[i].suit = token;
        token = strtok(NULL, ".");
    }
    
    printf("your four card hand is: \n");
    for(int y = 0; y < 4; y++)
    {
        
        printf("%s of %s\n", hand[y].value, hand[y].suit);
    }
        
    int mult = checkCards(hand);
    if(mult != 1)
    {
        bet = bet*mult;
        coins += bet;
        wins++;
    }
    else
    {
        coins -= bet;
        losses++;
    }
    
    if (coins == 0)
    {
        printf("\nYou are out of coins\n");
        boo = 0;
    }
    else
    {
        printf("\nYou now have %d coins.\n", coins);
        printf("\nwanna play again(press 1 to play again/anything else to term the game)?\n");
        scanf("%d", &boo);
    }
    } while(boo == 1);
    
    printf("\nGAME OVER\nCoins: %d\nMatches Played: %d\nWins: %d\nLosses: %d\n", coins, matches, wins, losses);
    free(hand);
    return 0;
    
}

int checkCards(struct card* hand)
{
    char* s0 = strdup(hand[0].suit);
    char* s1 = strdup(hand[1].suit);
    char* s2 = strdup(hand[2].suit);
    char* s3 = strdup(hand[3].suit);

    int* valint = inswapsort(hand);
    
    int v0 = valint[0];
    int v1 = valint[1];
    int v2 = valint[2];
    int v3 = valint[3];

    if (v0 == v1 && v0 == v2 && v0 == v3)
    {
        printf("\n4 of a kind!\n");
        return 20825;
    }
    else if (strcmp(s0, s1) == 0 && strcmp(s0, s2) == 0 && strcmp(s0, s3) == 0)
    {
        if (v0+3 == v3 && v1+2 == v3 && v2+1 == v3)
        {
            printf("\nstraight flush!\n");
            free(valint);
            return 6135;
        }
    }
    else if ((v0 == v1 && v1 == v2) || (v0 == v1 && v1 == v3) || (v0 == v2 && v2 == v3) || (v1 == v2 && v2 == v3))
    {
        printf("\n3 of a kind!\n");
        free(valint);
        return 108;
    }
    else if (v0+3 == v3 && v1+2 == v3 && v2+1 == v3)
    {
        printf("\nstraight!\n");
        free(valint);
        return 98;
    }
    else if ((v0 == v1 && v2 == v3) || (v0 == v2 && v1 == v3) || (v0 == v3 && v2 == v1))
    {
        printf("\n2 pair!\n");
        free(valint);
        return 96;
    }
    else if (strcmp(s0, s1) == 0 && strcmp(s0, s2) == 0 && strcmp(s0, s3) == 0)
    {
        printf("\nflush!\n");
        free(valint);
        return 96;
    }
    else if (v0 == v1 || v0 == v2 || v0 == v3 || v1 == v2 || v1 == v3 || v2 == v3)
    {
        printf("\npair!\n");
        free(valint);
        return 3;
    }
    
    printf("\nHigh Card. No matches.\n");
    free(valint);
    return 1;
}

int* inswapsort(struct card* hand)
{
    int* tmp = malloc(sizeof(int)*4);
    for(int i = 0; i < 4; i++)
    {
        if (strcmp(hand[i].value, "Two") == 0)
        {
            tmp[i] = 2;
        }
        else if(strcmp(hand[i].value, "Three") == 0)
        {
            tmp[i] = 3;
        }
        else if(strcmp(hand[i].value, "Four") == 0)
        {
            tmp[i] = 4;
        }
        else if(strcmp(hand[i].value, "Five") == 0)
        {
            tmp[i] = 5;
        }
        else if(strcmp(hand[i].value, "Six") == 0)
        {
            tmp[i] = 6;
        }
        else if(strcmp(hand[i].value, "Seven") == 0)
        {
            tmp[i] = 7;
        }
        else if(strcmp(hand[i].value, "Eight") == 0)
        {
            tmp[i] = 2;
        }
        else if(strcmp(hand[i].value, "Nine") == 0)
        {
            tmp[i] = 9;
        }
        else if(strcmp(hand[i].value, "Ten") == 0)
        {
            tmp[i] = 10;
        }
        else if(strcmp(hand[i].value, "Jack") == 0)
        {
            tmp[i] = 11;
        }
        else if(strcmp(hand[i].value, "Queen") == 0)
        {
            tmp[i] = 12;
        }
        else if(strcmp(hand[i].value, "King") == 0)
        {
            tmp[i] = 13;
        }
        else if(strcmp(hand[i].value, "Ace") == 0)
        {
            tmp[i] = 14;
        }
    }
    qsort(tmp, 4, sizeof(int), cmpfunc);
    return tmp;
}

