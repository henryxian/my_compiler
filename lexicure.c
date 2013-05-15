#include "stdio.h"
#include "string.h"
#include "ctype.h"

char prog[500], token[8];
char peek;
int syn, p, m=0, n, sum=0;
char *sysword[9]={  "begin",    "if",   "then",
                    "while",    "do",   "end", 
                    "and",      "or",   "not"};
 
void scaner()
{ 
    for(n=0;n<8;n++) 
        token[n]=NULL;
    peek=prog[p++];
    while(isspace(peek))
    {
        peek=prog[p];
        p++;
    }
    if((peek>='a'&&peek<='z')||(peek>='A'&&peek<='Z')) 
    {
        m=0;
        while((peek>='0'&&peek<='9')||(peek>='a'&&peek<='z')||(peek>='A'&&peek<='Z'))
        {
            token[m++]=peek;
            peek=prog[p++];
        }
        token[m++]='\0';
        p--;
        syn=10;
        for(n=0;n<9;n++) 
            if(strcmp(token,sysword[n])==0)
            {
                syn=n+1;
                break;
            }
    }
    else if((peek>='0'&&peek<='9')) 
    {
        {
            sum=0;
            while((peek>='0'&&peek<='9'))
            {
                sum=sum*10+peek-'0';
                peek=prog[p++];
            }
        }
        p--;
        syn=11;
    }
    else 
    switch(peek)
    {
        case'<':
            m=0;
            token[m++]=peek;
            peek=prog[p++];
            if(peek=='>')
            {
                syn=21;
                token[m++]=peek;
            }
            else if(peek=='=')
            {
                syn=22;
                token[m++]=peek;
            }
            else
            {
                syn=23;
                p--;
            }
            break;
        case'>':
            m=0;
            token[m++]=peek;
            peek=prog[p++];
            if(peek=='=')
            {
                syn=24;
                token[m++]=peek;
            }
            else
            {
                syn=20;
                p--;
            }
            break;
        case':':
            m=0;
            token[m++]=peek;
            peek=prog[p++];
            if(peek=='=')
            {
                syn=18;
                token[m++]=peek;
            }
            else
            {
                syn=17;
                p--;
            }
            break;
        case'*':
            syn=13;
            token[0]=peek;
            break;
        case'/':
            syn=14;
            token[0]=peek;
            break;
        case'+':
            syn=15;
            token[0]=peek;
            break;  
        case'-':
            syn=16;
            token[0]=peek;
            break;
        case'=':
            syn=25;
            token[0]=peek;
            break;
        case';':
            syn=26;
            token[0]=peek;
            break;
        case'(':
            syn=27;
            token[0]=peek;
            break;
        case')':
            syn=28;
            token[0]=peek;
            break;
        case'#':
            syn=0;
            token[0]=peek;
            break;
        default: 
            syn=-1;
            break;
    }
}

int main()
{
    p=0;
    do
    {
        peek=getchar();
        prog[p++]=peek;
    }
    while(peek!='#');
    p=0;
    do
    {
        scaner();
        switch(syn)
        {
        case 11: 
            printf("(%d, %d)\n", syn, sum); 
            break;  
        case -1: 
            printf("Error!\n");
            break;
        default: 
            printf("(%d, %s)\n", syn, token);
            break;
        }
    }
    while (syn!=0);
}