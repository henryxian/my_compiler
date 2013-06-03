　// 11.1 ver1.1.cpp : 定义控制台应用程序的入口点。
　
　//
　
　#include "stdafx.h"
　/*
　int _tmain(int argc, _TCHAR* argv[])
　{
　 return 0;
　}
　*/
　/****************************************************/
　/*              pas.c                               */
　/*          高级语言到四元式                        */
　/****************************************************/
　
　#include  "stdio.h"
　#include  "string.h"
　#include  "time.h"
　#include  "conio.h"
#include  "stdlib.h"

　
　void my_delay(unsigned long ms)
　
{
    　    double start_time = (double)clock() / CLOCKS_PER_SEC * 1000;
    　    while ( (double)clock() / CLOCKS_PER_SEC * 1000 - start_time < ms ) ;
    　
}
　#define      ACC     -2
　/****************************************/
　#define sy_if        0
　#define sy_then      1
　#define sy_else      2
　#define sy_while 3
　#define sy_begin 4
　#define sy_do        5
　#define sy_end       6
　#define a            7
　#define semicolon    8
　#define e            9
　#define jinghao      10
　#define S            11
　#define L            12
　
　#define tempsy      15
　#define EA           18 /*E and*/
　#define EO           19 /*E or*/
　
　#define plus     34
　#define minus       35  //添加  减号的关键字
　#define times        36
　#define division    37  //添加 除号的关键字
　#define becomes      38
　#define op_and       39
　#define op_or        40
　#define op_not       41
　#define rop          42
　
　#define lparent      48
　#define rparent      49
　#define ident       56
　#define intconst    57
　/******************************************/
　char ch = '\0'; /*当前字符*/
　int     count = 0;
　static   char    spelling[10] = {""}; /*存放识别的字*/
　static   char    line[81] = {""}; /*一行字符缓冲区*/
　char *pline;/*字符缓冲区指针*/
　char ch_exit;
　static   char    ntab1[100][10];
　struct   ntab
        　
{
    　         int tc;
    　         int fc;
    　
} ntab2[200];
　int label = 0;
　     /*存放临时变量的表的定义 */
　struct   rwords
{
    　             char    sp[10];
    　             int     sy;
    　
};
　/*存放文件的结构*/
　struct rwords reswords[10] = {{"if", sy_if},
    　                         {"do", sy_do},
    　                         {"else", sy_else},
    　                         {"while", sy_while},
    　                         {"then", sy_then},
    　                         {"begin", sy_begin},
    　                         {"end", sy_end},
    　                         {"and", op_and},
    　                         {"or", op_or},
    　                         {"not", op_not}
};
　struct  aa
{
    　     int sy1;
    　     int pos;
    　
} buf[1000], /*词法分析结果缓冲区*/
　     n,/*当前字符*/
　     n1,/*当前表达式中的字符*/
　     E,/*非终结符*/
　     sstack[100],/*符号栈*/
　     ibuf[100],
　     stack[1000];
　struct  aa oth;
　struct  fourexp
{
    　             char op[10];
    　             struct aa arg1;
    　             struct aa arg2;
    　             int result;
    　
} fexp[200];
　/*四元式的结构*/
　
　int      ssp = 0; /*指向sstack[100]*/
　struct   aa  *pbuf = buf; /*指向词法分析缓冲区*/
　int     nlength = 0;
　int     lnum = 0; /*源程序长度*/
　
　int     tt1 = 0;
　FILE    *cfile;
　FILE    *mfile;
　/*********************************************************/
　int newt = 0;
　/*临时变量*/
　int nxq = 100;
　/*nxq指向下一个形成的四元式的地址*/
　int  lr;
　int lr1;
　int sp = 0;
　/* 状态栈定义*/
　int stack1[100];
　int sp1 = 0;
　/*状态栈1的定义*/
　int num = 0;
　struct ll
{
    　 int nxq1;
    　 int tc1;
    　 int fc1;
    　
} labelmark[10];
　int labeltemp[10];
　int pointmark = -1, pointtemp = -1;
　int sign = 0;
　/*sign＝1，表达式为赋值语句；sign＝2，表达式为布尔表达式。*/
　/***********************************************/
　static int   action[19][13] =
　/*0*/        {{2, -1, -1, 3, 4, -1, -1, 5, -1, -1, 10, 1, -1},
    　/*1*/        { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, ACC, -1, -1},
    　/*2*/        { -1, -1, -1, -1, -1, -1, -1, -1, -1, 6, -1, -1, -1},
    　/*3*/        { -1, -1, -1, -1, -1, -1, -1, -1, -1, 7, -1, -1, -1},
    　/*4*/        {2, -1, -1, 3, 4, -1, -1, 5, -1, -1, -1, 9, 8},
    　/*5*/        { -1, -1, 104, -1, -1, -1, 104, -1, 104, -1, 104, -1, -1},
    　/*6*/        { -1, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    　/*7*/        { -1, -1, -1, -1, -1, 11, -1, -1, -1, -1, -1, -1, -1},
    　/*8*/        { -1, -1, -1, -1, -1, -1, 12, -1, -1, -1, -1, -1, -1},
    　/*9*/        { -1, -1, -1, -1, -1, -1, 105, -1, 13, -1, -1, -1, -1},
    　/*10*/       {2, -1, -1, 3, 4, -1, -1, 5, -1, -1, -1, 14, -1},
    　/*11*/       {2, -1, -1, 3, 4, -1, -1, 5, -1, -1, -1, 15, -1},
    　/*12*/       { -1, -1, 103, -1, -1, -1, 103, -1, 103, -1, 103, -1, -1},
    　/*13*/       {2, -1, -1, 3, 4, -1, -1, 5, -1, -1, -1, 9, 16},
    　/*14*/       { -1, -1, 17, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    　/*15*/       { -1, -1, 102, -1, -1, -1, 102, -1, 102, -1, 102, -1, -1},
    　/*16*/       { -1, -1, -1, -1, -1, -1, 106, -1, -1, -1, -1, -1, -1},
    　/*17*/       {2, -1, -1, 3, 4, -1, -1, 5, -1, -1, -1, 18, -1},
    　/*18*/       { -1, -1, 101, -1, -1, -1, 101, -1, 101, -1, 101, -1, -1}
};
　
　//下面的是添加了减法和除法的算术表达式SLR矩阵
　static int action1[14][9] =
　/*0*/        {{3, -1, -1, -1, -1, 2, -1, -1, 1},
    　/*1*/        { -1, 4, 5, 6, 7, -1, -1, ACC, -1},
    　/*2*/        {3, -1, -1, -1, -1, 2, -1, -1, 8},
    　/*3*/        { -1, 106, 106, 106, 106, -1, 106, 106, -1},
    　/*4*/        {3, -1, -1, -1, -1, 2, -1, -1, 9},
    　/*5*/        {3, -1, -1, -1, -1, 2, -1, -1, 10},
    　/*6*/        {3, -1, -1, -1, -1, 2, -1, -1, 11},
    　/*7*/        {3, -1, -1, -1, -1, 2, -1, -1, 12},
    　/*8*/        { -1, 4, 5, 6, 7, -1, 13, -1, -1},
    　/*9*/        { -1, 101, 5, 101, 7, -1, 101, 101, -1},
    　/*10*/      { -1, 102, 102, 102, 102, -1, 102, 102, -1},
    　/*11*/       { -1, 103, 5, 103, 7, -1, 103, 103, -1},
    　/*12*/       { -1, 104, 104, 104, 104, -1, 104, 104, -1},
    　/*13*/       { -1, 105, 105, 105, 105, -1, 105, 105, -1}
};
　
　
　static int action2[16][11] =
　/*0*/        {{1, -1, 4, -1, 5, -1, -1, -1, 13, 7, 8},
    　/*1*/        { -1, 2, -1, 101, -1, 101, 101, 101, -1, -1, -1},
    　/*2*/        {3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    　/*3*/        { -1, -1, -1, 102, -1, 102, 102, 102, -1, -1, -1},
    　/*4*/        {1, -1, 4, -1, 5, -1, -1, -1, 11, 7, 8},
    　/*5*/        {1, -1, 4, -1, 5, -1, -1, -1, 6, 7, 8},
    　/*6*/        { -1, -1, -1, 104, -1, 9, 10, 104, -1, -1, -1},
    　/*7*/        {1, -1, 4, -1, 5, -1, -1, -1, 14, 7, 8},
    　/*8*/        {1, -1, 4, -1, 5, -1, -1, -1, 15, 7, 8},
    　/*9*/        {105, -1, 105, -1, 105, -1, -1, 105, -1, -1, -1},
    　/*10*/       {107, -1, 107, -1, 107, -1, -1, 107, -1, -1, -1},
    　/*11*/       { -1, -1, -1, 12, -1, 9, 10, -1, -1, -1, -1},
    　/*12*/       { -1, 103, -1, 103, -1, 103, 103, 103, -1, -1, -1},
    　/*13*/       { -1, -1, -1, -1, -1, 9, 10, ACC, -1, -1, -1},
    　/*14*/       { -1, -1, -1, 106, -1, 9, 10, 106, -1, -1, -1},
    　/*15*/       { -1, -1, -1, 108, -1, 9, 10, 108, -1, -1, -1}
};
　/****************从文件读一行到缓冲区***********************************/
　void readline()
　
{
    　 char ch1;
    　 pline = line;
    　 ch1 = getc(cfile);
    　 while (ch1 != '\n')
        　 {
        　////////////////////////////////////////////////////////////////////////////
        　     my_delay(50);
        　     printf("%c", ch1);
        　///////////////////////////////////////////////////////////////////////////////
        　     *pline = ch1;
        　     pline++;
        　     ch1 = getc(cfile); //取下一个字符
        　
    }
    　 *pline = '\0';
    　 pline = line;
    　 printf("\n");
    　
}
　/*********************************从缓冲区读一个字符************************/
　void readch()
　
{
    　 if (ch == '\0')
        　 {
        　
        　     readline();
        　     lnum++;
        　
        　
    }
    　 ch = *pline;
    　 pline++;
    　
    　
}
　/****************标志符和关键字的识别************************************/
　find(char spel[])
　
{
    　 int ss1 = 0;
    　 int ii = 0;
    　 while ((ss1 == 0) && (ii < nlength))
        　 {
        　     if (!strcmp(spel, ntab1[ii])) ss1 = 1;
        　     ii++;
        　
    }
    　 if (ss1 == 1) return ii - 1;
    　 else return -1;
    　
}
　void identifier()
　
{
    　 int iii = 0, j, k;
    　 int ss = 0;
    　 k = 0;
    　 do
        　 {
        　     spelling[k] = ch;
        　     k++;
        　     readch();
        　
    }
    while (((ch >= 'a') && (ch <= 'z')) || ((ch >= '0') && (ch <= '9')));
    　 pline--;
    　 spelling[k] = '\0';
    　 while ((ss == 0) && (iii < 10))
        　 {
        　     if (!strcmp(spelling, reswords[iii].sp)) ss = 1;
        　     iii++;
        　
    }
    　 /*关键字匹配*/
    　 if (ss == 1)
        　 {
        　     buf[count].sy1 = reswords[iii - 1].sy;
        　
    }
    　 else
        　 {
        　     buf[count].sy1 = ident;
        　     j = find(spelling);
        　     if (j == -1)
            　     {
            　         buf[count].pos = tt1;
            　         strcpy(ntab1[tt1], spelling);
            　         tt1++;
            　         nlength++;
            　
        }
        　     else buf[count].pos = j;
        　
    }
    　 count++;
    　 for (k = 0; k < 10; k++) spelling[k] = '\0';
    　
}
　/*****************数字的识别***********************************/
　void number()
　
{
    　 int ivalue = 0;
    　 int digit;
    　 do
        　 {
        　     digit = ch - '0';
        　     ivalue = ivalue * 10 + digit;
        　     readch();
        　
    }
    while ((ch >= '0') && (ch <= '9'));
    　 buf[count].sy1 = intconst;
    　 buf[count].pos = ivalue;
    　 count++;
    　 pline--;
    　
}
　/**************扫描主函数*************************************/
　void scan()
　
{
    　 int i;
    　 while (ch != '~')
        　 {
        　     switch (ch)
            　     {
        　     case ' ':
            　         break;
        　     case 'a':
        　     case 'b':
        　     case 'c':
        　     case 'd':
        　     case 'e':
        　     case 'f':
        　     case 'g':
        　     case 'h':
        　     case 'i':
        　     case 'j':
        　     case 'k':
        　     case 'l':
        　     case 'm':
        　     case 'n':
        　     case 'o':
        　     case 'p':
        　     case 'q':
        　     case 'r':
        　     case 's':
        　     case 't':
        　     case 'u':
        　     case 'v':
        　     case 'w':
        　     case 'x':
        　     case 'y':
        　     case 'z':
            　         identifier();
            　         break;
        　     case '0':
        　     case '1':
        　     case '2':
        　     case '3':
        　     case '4':
        　     case '5':
        　     case '6':
        　     case '7':
        　     case '8':
        　     case '9':
            　         number();
            　         break;
        　     case '<':
            　         readch();
            　         if (ch == '=')
                　         {
                　             buf[count].pos = 0;
                　
            }
            　         else
                　         {
                　             if (ch == '>') buf[count].pos = 4;
                　             else
                    　             {
                    　                 buf[count].pos = 1;
                    　                 pline--;
                    　
                }
                　
            }
            　         buf[count].sy1 = rop;
            　         count++;
            　         break;
        　     case '>':
            　         readch();
            　         if (ch == '=')
                　         {
                　             buf[count].pos = 2;
                　
            }
            　         else
                　         {
                　             buf[count].pos = 3;
                　             pline--;
                　
            }
            　         buf[count].sy1 = rop;
            　         count++;
            　         break;
        　     case '(':
            　         buf[count].sy1 = lparent;
            　         count++;
            　         break;
        　     case ')':
            　         buf[count].sy1 = rparent;
            　         count++;
            　         break;
        　     case '#':
            　         buf[count].sy1 = jinghao;
            　         count++;
            　         break;
        　     case '+':
            　         buf[count].sy1 = plus;
            　         count++;
            　         break;
        　     case '-':                   //ex
            　         buf[count].sy1 = minus;
            　         count++;
            　         break;
        　     case '/':                   //ex
            　         buf[count].sy1 = division;
            　         count++;
            　         break;
        　     case '*':
            　         buf[count].sy1 = times;
            　         count++;
            　         break;
        　     case ':':
            　         readch();
            　         if (ch == '=')
                　         buf[count].sy1 = becomes;
            　         count++;
            　         break;
        　     case '=':
            　         buf[count].sy1 = rop;
            　         buf[count].pos = 5;
            　         count++;
            　         break;
        　     case ';':
            　         buf[count].sy1 = semicolon;
            　         count++;
            　         break;
            　
        }
        　     readch();
        　
    }
    　 buf[count].sy1 = -1;
    　
}
　/******************************************************************/
　void readnu()
　
{
    　 if (pbuf->sy1 >= 0)
        　 {
        　     n.sy1 = pbuf->sy1;
        　     n.pos = pbuf->pos;
        　     pbuf++;
        　
    }
    　
}
　/******************中间变量的生成**************************/
　newtemp()
　
{
    　 newt++;
    　 return newt;
    　
}
　/**************************生成四元式****************/
　gen(char op1[], struct aa arg11, struct aa arg22, int result1)
　
{
    　 strcpy(fexp[nxq].op, op1);
    　 fexp[nxq].arg1.sy1 = arg11.sy1;
    　 fexp[nxq].arg1.pos = arg11.pos;
    　 fexp[nxq].arg2.sy1 = arg22.sy1;
    　 fexp[nxq].arg2.pos = arg22.pos;
    　 fexp[nxq].result = result1;
    　 nxq++;
    　 return nxq - 1;
    　
}
　/**********布尔表达式的匹配****************/
　merg(int p1, int p2)
　
{
    　 int p;
    　 if (p2 == 0) return p1;
    　 else
        　 {
        　     p = p2;
        　     while (fexp[p].result != 0) p = fexp[p].result;
        　     fexp[p].result = p1;
        　     return p2;
        　
    }
    　
}
　void backpatch(int p, int t)
　
{
    　 int tempq;
    　 int q;
    　 q = p;
    　 while (q != 0)
        　 {
        　     tempq = fexp[q].result;
        　     fexp[q].result = t;
        　     q = tempq;
        　
    }
    　
}
　/*******************************************/
　change1(int chan)
　
{
    　 switch (chan)
        　 {
    　 case ident:
    　 case intconst:
        　     return 0;
    　 case plus:
        　     return 1;
    　 case times:
        　     return 2;
    　 case minus:       //返回减号在 算术表达式SLR表中的 列号
        　     return 3;
    　 case division:     ///返回除号在 算术表达式SLR表中的 列号
        　     return 4;
    　 case lparent:
        　     return 5;
    　 case rparent:
        　     return 6;
    　 case jinghao:
        　     return 7;
    　 case tempsy:
        　     return 8;
        　
    }
    　
}
　change2(int chan)
　
{
    　 switch (chan)
        　 {
    　 case ident:
    　 case intconst:
        　     return 0;
    　 case rop:
        　     return 1;
    　 case lparent:
        　     return 2;
    　 case rparent:
        　     return 3;
    　 case op_not:
        　     return 4;
    　 case op_and:
        　     return 5;
    　 case op_or:
        　     return 6;
    　 case jinghao:
        　     return 7;
    　 case tempsy:
        　     return 8;
    　 case EA:
        　     return 9;
    　 case EO:
        　     return 10;
        　
    }
    　
}
　/************赋值语句的的分析***************************/
　void lrparse1(int num)
　
{
    　 lr1 = action1[stack1[sp1]][change1(n1.sy1)];
    　 if (lr1 == -1)
        　 {
        　     printf("\n算术表达式或赋值语句出错！\n");
        　     getch();
        　     exit(0);
        　
    }
    　 if (ibuf[0].sy1 != ident) //对赋值表达式的检查
        　 {
        　     printf("\n赋值语句出错！赋值号的左边不是变量名\n");
        　     getch();
        　     exit(0);
        　
    }
    　 if ((lr1 < 14) && (lr1 >= 0)) //14 ――标记算术表达式SLR表中的状态数
        　 {
        　     sp1++;
        　     stack1[sp1] = lr1;
        　     if (n1.sy1 != tempsy)
            　     {
            　         ssp++;
            　         num++;
            　         sstack[ssp].sy1 = n1.sy1;
            　         sstack[ssp].pos = n1.pos;
            　
        }
        　     n1.sy1 = ibuf[num].sy1;
        　     n1.pos = ibuf[num].pos;
        　     lrparse1(num);
        　
    }
    　 if ((lr1 >= 100) && (lr1 < 107))
        　 {
        　     switch (lr1)
            　     {
        　     case 100:
            　         break;
        　     case 101:
            　         printf("\nE->E+E 归约\n");
            　         E.pos = newtemp();
            　         gen("+", sstack[ssp - 2], sstack[ssp], E.pos + 100);
            　         ssp = ssp - 2;
            　         sstack[ssp].sy1 = tempsy;
            　         sstack[ssp].pos = E.pos;
            　         sp1 = sp1 - 3;
            　         break;
            　         /*E->E+E*/
        　     case 102:
            　         printf("\nE->E*E 归约\n");
            　         E.pos = newtemp();
            　         gen("*", sstack[ssp - 2], sstack[ssp], E.pos + 100);
            　         ssp = ssp - 2;
            　         sstack[ssp].sy1 = tempsy;
            　         sstack[ssp].pos = E.pos;
            　         sp1 = sp1 - 3;
            　         break;
            　         /*E->E*E*/
        　     case 103:          //添加的  对减法归约的语义代码
            　         printf("\nE->E-E 归约\n");
            　         E.pos = newtemp();
            　         gen("-", sstack[ssp - 2], sstack[ssp], E.pos + 100);
            　         ssp = ssp - 2;
            　         sstack[ssp].sy1 = tempsy;
            　         sstack[ssp].pos = E.pos;
            　         sp1 = sp1 - 3;
            　         break;
            　         /*E->E-E*/
        　     case 104:          //添加的  对除法归约的语义代码
            　         printf("\nE->E/E 归约\n");
            　         E.pos = newtemp();
            　         gen("/", sstack[ssp - 2], sstack[ssp], E.pos + 100);
            　         ssp = ssp - 2;
            　         sstack[ssp].sy1 = tempsy;
            　         sstack[ssp].pos = E.pos;
            　         sp1 = sp1 - 3;
            　         break;
            　         /*E->E/E*/
        　     case 105:
            　         printf("\nE->(E) 归约\n");
            　         E.pos = sstack[ssp - 1].pos;
            　         ssp = ssp - 2;
            　         sstack[ssp].sy1 = tempsy;
            　         sstack[ssp].pos = E.pos;
            　         sp1 = sp1 - 3;
            　         break;
            　         /*E->(E)*/
        　     case 106:
            　         printf("\nE->i 归约\n");
            　         E.pos = sstack[ssp].pos;
            　         sp1--;
            　         break;
            　         /*E->i*/
            　
        }
        　     n1.sy1 = tempsy; /*规约后为非终结符*/
        　     n1.pos = E.pos;
        　     lrparse1(num);
        　
    }
    　 if ((lr1 == ACC) && (stack1[sp1] == 1))
        　 {
        　     /*归约A->i:=E*/
        　     printf("\nA->i:=E 归约\n");
        　         gen(":=", sstack[ssp], oth, ibuf[0].pos);
        　     ssp = ssp - 3;
        　     sp1 = sp1 - 3;
        　
    }
    　
}
　/*********************布尔表达式的分析**************************/
　lrparse2(int num)
　
{
    　 int templabel;
    　 lr1 = action2[stack1[sp1]][change2(n1.sy1)];
    　 if (lr1 == -1)
        　 {
        　     if (sign == 2) printf("\nwhile语句出错！\n");
        　     if (sign == 3) printf("\nif语句出错！\n");
        　     getch();
        　     exit(0);
        　
    }
    　 if ((lr1 < 16) && (lr1 >= 0))
        　 {
        　     sp1++;
        　     stack1[sp1] = lr1;
        　     ssp++;
        　     sstack[ssp].sy1 = n1.sy1;
        　     sstack[ssp].pos = n1.pos;
        　     if ((n1.sy1 != tempsy) && (n1.sy1 != EA) && (n1.sy1 != EO)) num++;
        　     n1.sy1 = ibuf[num].sy1;
        　     n1.pos = ibuf[num].pos;
        　     lrparse2(num);
        　
    }
    　 if ((lr1 >= 100) && (lr1 < 109))
        　 {
        　     switch (lr1)
            　     {
        　     case 100:
            　         break;
        　     case 101:
            　         ntab2[label].tc = nxq;
            　         ntab2[label].fc = nxq + 1;
            　         gen("jnz", sstack[ssp], oth, 0);
            　         gen("j", oth, oth, 0);
            　         sp1--;
            　         ssp--;
            　         label++;
            　         n1.sy1 = tempsy;
            　         break;
            　         /*E->i*/
        　     case 102:
            　         ntab2[label].tc = nxq;
            　         ntab2[label].fc = nxq + 1;
            　         switch (sstack[ssp - 1].pos)
                　         {
            　             case 0:
                　                 gen("j<=", sstack[ssp - 2], sstack[ssp], 0);
                　                 break;
            　             case 1:
                　                 gen("j<", sstack[ssp - 2], sstack[ssp], 0);
                　                 break;
            　             case 2:
                　                 gen("j>=", sstack[ssp - 2], sstack[ssp], 0);
                　                 break;
            　             case 3:
                　                 gen("j>", sstack[ssp - 2], sstack[ssp], 0);
                　                 break;
            　             case 4:
                　                 gen("j<>", sstack[ssp - 2], sstack[ssp], 0);
                　                 break;
            　             case 5:
                　                 gen("j=", sstack[ssp - 2], sstack[ssp], 0);
                　                 break;
                　
            }
            　         gen("j", oth, oth, 0);
            　         ssp = ssp - 3;
            　         sp1 = sp1 - 3;
            　         label++;
            　         n1.sy1 = tempsy;
            　         break;
            　         /*E->i rop i*/
        　     case 103:
            　         label = label - 1;
            　         ssp = ssp - 3;
            　         sp1 = sp1 - 3;
            　         label++;
            　         n1.sy1 = tempsy;
            　         break;
            　         /*E->(E)*/
        　     case 104:
            　         label = label - 1;
            　         templabel = ntab2[label].tc;
            　         ntab2[label].tc = ntab2[label].fc;
            　         ntab2[label].fc = templabel;
            　         ssp = ssp - 2;
            　         sp1 = sp1 - 2;
            　         label++;
            　         n1.sy1 = tempsy;
            　         break;
            　         /*E->not E*/
        　     case 105:
            　         backpatch(ntab2[label - 1].tc, nxq);
            　         label = label - 1;
            　         ssp = ssp - 2;
            　         sp1 = sp1 - 2;
            　         label++;
            　         n1.sy1 = EA;
            　         break;
            　         /*EA->E(1)and*/
        　     case 106:
            　         label = label - 2;
            　         ntab2[label].tc = ntab2[label + 1].tc;
            　         ntab2[label].fc = merg(ntab2[label].fc, ntab2[label + 1].fc);
            　         ssp = ssp - 2;
            　         sp1 = sp1 - 2;
            　         label++;
            　         n1.sy1 = tempsy;
            　         break;
            　         /*E->EA E(2)*/
        　     case 107:
            　         backpatch(ntab2[label - 1].fc, nxq);
            　         label = label - 1;
            　         ssp = ssp - 2;
            　         sp1 = sp1 - 2;
            　         label++;
            　         n1.sy1 = EO;
            　         break;
        　     case 108:
            　         label = label - 2;
            　         ntab2[label].fc = ntab2[label + 1].fc;
            　         ntab2[label].tc = merg(ntab2[label].tc, ntab2[label + 1].tc);
            　         ssp = ssp - 2;
            　         sp1 = sp1 - 2;
            　         label++;
            　         n1.sy1 = tempsy;
            　         break;
            　
        }
        　     lrparse2(num);
        　
    }
    　
    　
    　 if (lr1 == ACC) return 1;
    　
}
　
　/***************测试字符是否为表达式中的值(不包括";")**************/
　test(int value)
　
{
    　 switch (value)
        　 {
    　 case intconst:
    　 case ident:
    　 case plus:
    　 case times:
    　 case minus:     //减号  是否是表达式中的符号
    　 case division:   //除号  是否是表达式中的符号
    　 case becomes:
    　 case lparent:
    　 case rparent:
    　 case rop:
    　 case op_and:
    　 case op_or:
    　 case op_not:
        　     return 1;
    　 default:
        　     return 0;
        　
    }
    　
}
　/*******************************************/
　int lrparse()
　
{
    　 int i1 = 0;
    　 int num = 0;
    　 /*指向表达式缓冲区*/
    　 if (test(n.sy1))
        　 {
        　     if (stack[sp].sy1 == sy_while) sign = 2;
        　     else
            　     {
            　         if (stack[sp].sy1 == sy_if) sign = 3;
            　         else sign = 1;
            　
        }
        　     do
            　     {
            　         ibuf[i1].sy1 = n.sy1;
            　         ibuf[i1].pos = n.pos;
            　         readnu();
            　         i1++;
            　
        }
        while (test(n.sy1));
        　     /*把表达式放入缓冲区*/
        　     ibuf[i1].sy1 = jinghao;
        　     pbuf--;
        　     /*指针后退1，需要吗？*/
        　     sstack[0].sy1 = jinghao;
        　     ssp = 0;
        　     /*符号栈底的初始化*/
        　     if (sign == 1)
            　     {
            　         sp1 = 0;
            　         stack1[sp1] = 0;
            　         /*状态栈1的栈底初始化*/
            　         num = 2;
            　         /*指向:=*/
            　         n1.sy1 = ibuf[num].sy1;
            　         n1.pos = ibuf[num].pos;
            　         lrparse1(num);
            　         /*处理赋值语句*/
            　         n.sy1 = a;
            　         /*当前文法符号置为a（赋值语句）*/
            　
        }
        　     if ((sign == 2) || (sign == 3))
            　     {
            　         pointmark++;
            　         labelmark[pointmark].nxq1 = nxq;
            　         sp1 = 0;
            　         stack1[sp1] = 0;
            　         num = 0;
            　         n1.sy1 = ibuf[num].sy1;
            　         n1.pos = ibuf[num].pos;
            　         lrparse2(num);
            　         labelmark[pointmark].tc1 = ntab2[label - 1].tc;
            　         labelmark[pointmark].fc1 = ntab2[label - 1].fc;
            　         /*处理布尔语句*/
            　         /*在处理完E，要回填真值链*/
            　         backpatch(labelmark[pointmark].tc1, nxq);
            　         n.sy1 = e;
            　         /*当前文法符号置为e（赋值语句）*/
            　
        }
        　
    }
    　 lr = action[stack[sp].pos][n.sy1];
    　 printf("stack[%d]=%d\t\tn=%d\t\tlr=%d\n", sp, stack[sp].pos, n.sy1, lr);
    　 if ((lr < 19) && (lr >= 0))
        　 {
        　     sp++;
        　     stack[sp].pos = lr;
        　     stack[sp].sy1 = n.sy1;
        　     readnu();
        　     lrparse();
        　
    }
    　     if ((lr <= 106) && (lr >= 100))
        　 {
        　     switch (lr)
            　     {
        　     case 100: break;
            　         /*S'->S*/
        　     case 101:
            　         printf("S->if e then S else S 归约\n");
            　         sp = sp - 6;
            　         n.sy1 = S;
            　         /*归约完if后，填then后面的无条件转移语句*/
            　         fexp[labeltemp[pointtemp]].result = nxq;
            　         pointtemp--;
            　         if (stack[sp].sy1 == sy_then)
                　         {
                　             gen("j", oth, oth, 0);
                　             backpatch(labelmark[pointmark].fc1, nxq);
                　             pointtemp++;
                　             labeltemp[pointtemp] = nxq - 1;
                　
            }
            　         pointmark--;
            　         if (stack[sp].sy1 == sy_do)
                　         {
                　             gen("j", oth, oth, labelmark[pointmark].nxq1);
                　             backpatch(labelmark[pointmark].fc1, nxq);
                　
            }
            　         break;
            　         /*S->if e then S then S else S*/
        　     case 102:
            　         printf("S->while e do S 归约\n");
            　         sp = sp - 4;
            　         n.sy1 = S;
            　         pointmark--;
            　         if (stack[sp].sy1 == sy_do)
                　         {
                　             gen("j", oth, oth, labelmark[pointmark].nxq1);
                　             backpatch(labelmark[pointmark].fc1, nxq);
                　
            }
            　
            　         if (stack[sp].sy1 == sy_then)
                　         {
                　             gen("j", oth, oth, 0);
                　             fexp[labelmark[pointmark].fc1].result = nxq;
                　             pointtemp++;
                　             labeltemp[pointtemp] = nxq - 1;
                　
            }
            　         break;
            　         /*S->while e do S*/
        　     case 103:
            　         printf("S->begin L end 归约\n");
            　         sp = sp - 3;
            　         n.sy1 = S;
            　         if (stack[sp].sy1 == sy_then)
                　         {
                　             gen("j", oth, oth, 0);
                　             backpatch(labelmark[pointmark].fc1, nxq);
                　             pointtemp++;
                　             labeltemp[pointtemp] = nxq - 1;
                　
            }
            　         if (stack[sp].sy1 == sy_do)
                　         {
                　             gen("j", oth, oth, labelmark[pointmark].nxq1);
                　             backpatch(labelmark[pointmark].fc1, nxq);
                　
            }
            　         getch();
            　         break;
            　         /*S->begin L end*/
        　     case 104:
            　         printf("S->a 归约\n");
            　         sp = sp - 1;
            　         n.sy1 = S;
            　         if (stack[sp].sy1 == sy_then)
                　         {
                　             gen("j", oth, oth, 0);
                　             backpatch(labelmark[pointmark].fc1, nxq);
                　             pointtemp++;
                　             labeltemp[pointtemp] = nxq - 1;
                　
            }
            　         if (stack[sp].sy1 == sy_do)
                　         {
                　             gen("j", oth, oth, labelmark[pointmark].nxq1);
                　             backpatch(labelmark[pointmark].fc1, nxq);
                　
            }
            　         break;
            　         /*S->a*/
        　     case 105:
            　         printf("L->S 归约\n");
            　         sp = sp - 1;
            　         n.sy1 = L;
            　         break;
            　         /*L->S*/
        　     case 106:
            　         printf("L->S;L 归约\n");
            　         sp = sp - 3;
            　         n.sy1 = L;
            　         break;
            　         /*L->S;L*/
            　
        }
        　     getch();
        　     pbuf--;
        　     lrparse();
        　
    }
    　 if (lr == ACC)
        　 {
        　     printf("分析成功！文法正确！\n");
        　     lr = -100;  //添加的语句 可能导致错误产生！！！！
        　     return ACC;
        　
    }
    　 else if (lr == -1)
        　 {
        　     printf("分析失败！文法错误！\n");
        　     lr = -200; //添加的语句 可能导致错误产生！！！！
        　
    }
    　
}
　/*****************************disp1*************************/
　void disp1()
　
{
    　 int temp1 = 0;
    　 printf("\n********************词法分析结果***********************\n");
    　 for (temp1 = 0; temp1 < count; temp1++)
        　 {
        　     printf("%d\t%d\n", buf[temp1].sy1, buf[temp1].pos);
        　     if (temp1 == 20)
            　     {
            　         printf("Press any key to continue......\n");
            　         getch();
            　
        }
        　
    }
    　 getch();
    　
}
　/******************************************************/
　void disp2()
　
{
    　 int temp1 = 100;
    　 printf("\n********************四元式分析结果***********************\n");
    　 for (temp1 = 100; temp1 < nxq; temp1++)
        　 {
        　                if (temp1 > 100)
            　                   fprintf(mfile, "\n");
        　     fprintf(mfile, "%d\t", temp1);
        　     fprintf(mfile, "(%s\t,", fexp[temp1].op);
        　     if (fexp[temp1].arg1.sy1 == ident)
            　         fprintf(mfile, "%s\t,", ntab1[fexp[temp1].arg1.pos]);
        　     else
            　     {
            　         if (fexp[temp1].arg1.sy1 == tempsy)
                　             fprintf(mfile, "T%d\t,", fexp[temp1].arg1.pos);
            　         else
                　         {
                　             if (fexp[temp1].arg1.sy1 == intconst)
                    　                 fprintf(mfile, "%d\t,", fexp[temp1].arg1.pos);
                　             else fprintf(mfile, "\t,");
                　
            }
            　
        }
        　     if (fexp[temp1].arg2.sy1 == ident)
            　         fprintf(mfile, "%s\t,", ntab1[fexp[temp1].arg2.pos]);
        　     else
            　     {
            　         if (fexp[temp1].arg2.sy1 == tempsy)
                　             fprintf(mfile, "T%d\t,", fexp[temp1].arg2.pos);
            　         else
                　         {
                　             if (fexp[temp1].arg2.sy1 == intconst)
                    　                 fprintf(mfile, "%d\t,", fexp[temp1].arg2.pos);
                　             else fprintf(mfile, "\t,");
                　
            }
            　
        }
        　     if (fexp[temp1].op[0] != 'j')
            　     {
            　         if (fexp[temp1].result >= 100)
                　                              fprintf(mfile, "T%d\t)", fexp[temp1].result - 100);
            　         else fprintf(mfile, "%s\t)", ntab1[fexp[temp1].result]);
            　
        }
        　     else fprintf(mfile, "%d\t)", fexp[temp1].result);
        　     if (temp1 == 20)
            　     {
            　         printf("\nPress any key to continue......\n");
            　         getch();
            　
        }
        　
    }
    　 getch();
    　
}
　void disp3()
　
{
    　 int tttt;
    　 printf("\n\n程序总 %d 共行，产生了 %d 个二元式！\n", lnum, count);
    　 getch();
    　 printf("\n*******************变量表*******************\n");
    　 for (tttt = 0; tttt < tt1; tttt++)
        　      printf("%d\t%s\n", tttt, ntab1[tttt]);
    　 getch();
    　
}
　/*****************主程序**********************/
　void main()
　
{
    　 printf("\nStart to read pas.c");
    　 cfile = fopen("pas.dat", "r");
    　 if ((cfile = fopen("pas.dat", "r")) == NULL)
        　 {
        　     printf("\n can't open file!");
        　     printf("\n press any key to quit!");
        　     scanf("%c", &ch_exit);
        　     exit(0);
        　
    }
    　 else
        　     printf("\n reading...! \n");
    　  mfile = fopen("pas.med", "w");
    　 /*打开c语言源文件*/
    　
    　 readch();
    　 /*从源文件读一个字符*/
    　
    　 scan();
    　 /*词法分析*/
    　
    　 disp1();
    　
    　 disp3();
    　
    　 stack[sp].pos = 0;
    　 stack[sp].sy1 = -1;
    　 /*初始化状态栈栈底*/
    　 stack1[sp1] = 0;
    　 /*初始化状态栈栈底*/
    　 oth.sy1 = -1;
    　 printf("\n*********状态栈变化过程及规约顺序************\n");
    　 readnu();
    　 /*从二元式读一个字符*/
    　 lrparse();
    　 getch();
    　 /*四元式的分析*/
    　 disp2();
    　 printf("\n程序结束。谢谢使用！\n");
    　 printf("COPYRIGHT BY Z.Y.P 2001.6.7");
    　 getch();
    　
}