//
//  main.c
//  AI_Gomoku
//
//  Created by 김동섭 on 2018. 3. 30..
//  Copyright © 2018년 김동섭. All rights reserved.
//

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))
#define MAX(X,Y) ((X) > (Y) ? (X) : (Y))
#define BLACK true
#define WHITE false

struct values{
    int x;
    int y;
    int v;
}; //MIN_VALUE, MAX_VALUE의 리턴 형태

//함수 선언
static time_t startTime;
static int mode;
static bool timeout;
static char status[20][20];
static int temp[362][2];
void print(char status[20][20]);
void myturn(bool turn);
int terminal_test(char state[20][20]);
bool endJudge();
void AI_firstTurn();
struct values utility(char state[20][20],bool turn,int terminal,int depth);
struct values bounded(char state[20][20],int depth,bool turn);
bool compareArray(char a[5],char b[5]);
int compareFive(char test[5],bool turn);
int** actions(char state[20][20],bool turn);
char (*result(char state[20][20],int action[2],bool turn))[20];
struct values MAX_VALUE(char state[20][20], int alpha, int beta,int depth, int maxDepth,int time_limit);
struct values MIN_VALUE(char state[20][20], int alpha, int beta,int depth, int maxDepth,int time_limit);
void AI_black(int time_limit);
void AI_white(int time_limit);
bool threeRule(int x, int y, char state[20][20], bool turn);
//게임 진행 함수
void print(char status[20][20]){
    for(int i=0;i<19;i++){
        for (int j=0;j<19;j++){
            printf("%c",status[i][j]);
            printf(" ");
        }
        printf("\n");
    }
    printf("\n");
}
bool threeRule(int x, int y, char state[20][20], bool turn){
    char check;
    if(turn==BLACK){
        check='X';
        state[x][y]='X';
    }
    else{
        check='O';
        state[x][y]='O';
    }
    int lb,rb,tb,bb;
    int temp;
    int tempY;
    if(turn==BLACK)
        check='X';
    else
        check='O';
    lb=y-2;
    rb=y+2;
    tb=x-2;
    bb=x+2;
    if(lb<0)
        lb=0;
    else if(rb>18)
        rb=18;
    if(tb<0)
        tb=0;
    else if(bb>18)
        bb=18;
    int count=0;
    //가로
    for(int i=lb; i<=rb-2 ;i++){
        for(int j=0; j<3; j++){
            temp=i+j;
            if(state[x][temp]==check)
                count++;
            if(count==3){
                count=0;
                if(bb==x+2){ //아래가 안막혀 있음
                    if(state[x][temp]==check && state[x+1][temp]==check && state[x+2][temp]==check)
                        return true; //temp아래
                    if(state[x][temp-2]==check && state[x+1][temp-2]==check && state[x+2][temp-2]==check)
                        return true; //temp-2아래
                    if(state[x][temp]==check && state[x+1][temp-1]==check && state[x+2][temp-2]==check)
                        return true; //temp의 '/'
                    if(state[x][temp-2]==check && state[x+1][temp-1]==check && state[x+2][temp]==check)
                        return true; //temp-2의 '\'
                    if(temp-4>=lb){
                        if(state[x][temp-2]==check && state[x+1][temp-3]==check && state[x+2][temp-4]==check)
                            return true;
                    } //temp-2 /
                    if(temp+2<=rb){
                        if(state[x][temp]==check && state[x+1][temp+1]==check && state[x+2][temp+2]==check)
                            return true;
                    } //temp '\'
                }
                if(tb==x-2){ //위가 안막혀 있음
                    if(state[x][temp]==check && state[x-1][temp]==check && state[x-2][temp]==check)
                        return true; //temp 위
                    if(state[x][temp-2]==check && state[x-1][temp-2]==check && state[x-2][temp-2]==check)
                        return true; //temp-2 위
                    if(state[x][temp-2]==check && state[x-1][temp-1]==check && state[x-2][temp]==check)
                        return true; //temp-2의 /
                    if(state[x][temp]==check && state[x-1][temp-1]==check && state[x-2][temp-2]==check)
                        return true; //temp의 '\'
                    if(temp-4>=lb){
                        if(state[x][temp-2]==check && state[x-1][temp-3]==check && state[x-2][temp-4]==check)
                            return true;
                    } //temp-2의 '\'
                    if(temp+2<=rb){
                        if(state[x][temp]==check && state[x-1][temp+1]==check && state[x-2][temp+2]==check)
                            return true;
                    } //temp의 '/'
                }
            }
        }
        count=0;
    }
    //세로
    for(int i=tb; i<=bb-2 ;i++){
        for(int j=0; j<3; j++){
            temp=i+j;
            if(state[temp][y]==check)
                count++;
            if(count==3){
                count=0;
                if(rb==y+2){ //오른쪽 안막혀 있음
                    if(state[temp][y]==check && state[temp][y+1]==check && state[temp][y+2]==check)
                        return true; //temp 오른쪽
                    if(state[temp-2][y]==check && state[temp-2][y+1]==check && state[temp-2][y+2]==check)
                        return true; //temp-2 오른쪽
                    if(state[temp][y]==check && state[temp-1][y+1]==check && state[temp-2][y+2]==check)
                        return true; //temp의 '/'
                    if(state[temp-2][y]==check && state[temp-1][y+1]==check && state[temp][y+2]==check)
                        return true; //temp-2의 '\'
                    if(temp-4>=0){
                        if(state[temp-2][y]==check && state[temp-3][y+1]==check && state[temp-4][y+2]==check)
                            return true;
                    } //temp-2의 '/'
                    if(temp+2<=18){
                        if(state[temp][y]==check && state[temp+1][y+1]==check && state[temp+2][y+2]==check)
                            return true;
                    } //temp의 '\'
                    
                }
                if(lb==y-2){ //왼쪽 안막혀 있음
                    if(state[temp][y]==check && state[temp][y-1]==check && state[temp][y-2]==check)
                        return true; //temp 왼쪽
                    if(state[temp-2][y]==check && state[temp-2][x-1]==check && state[temp-2][y-2]==check)
                        return true; //temp-2 왼쪽
                    if(state[temp][y]==check && state[temp-1][y-1]==check && state[temp-2][y-2]==check)
                        return true; //temp의 '\'
                    if(state[temp-2][y]==check && state[temp-1][y-1]==check && state[temp][y-2]==check)
                        return true; //temp-2의 '/'
                    if(temp-4>=0){
                        if(state[temp-2][y]==check && state[temp-3][y-1]==check && state[temp-4][y-2]==check)
                            return true;
                    } //temp-2의 '\'
                    if(temp+2<=18){
                        if(state[temp][y]==check && state[temp+1][y-1]==check && state[temp+2][y-2]==check)
                            return true;
                    } //temp의 '/'
                }
            }
        }
        count=0;
    }
    // '\'방향
    for(int i=-MIN(y-lb,x-tb);i<=MIN(rb-y,bb-x)-2;i++){
        for(int j=0;j<3;j++){
            temp=x+i+j;
            tempY=y+i+j;
            if(state[temp][tempY]==check)
                count++;
            if(count==3){
                //temp-2
                if(state[temp-2][tempY-2]==check && state[temp-2][tempY-1]==check && state[temp-2][tempY]==check)
                    return true; //temp-2 오른쪽
                if(state[temp-2][tempY-2]==check && state[temp-1][tempY-2]==check && state[temp][tempY-2]==check)
                    return true; //temp-2 아래
                if(0<=temp-4){
                    if(state[temp-2][tempY-2]==check && state[temp-3][tempY-2]==check && state[temp-4][tempY-2]==check)
                        return true; //temp-2 위
                    if(state[temp-2][tempY-2]==check && state[temp-3][tempY-1]==check && state[temp-4][tempY]==check)
                        return true; //temp-2 '/위'
                }
                if(0<=tempY-4){
                    if(state[temp-2][tempY-2]==check && state[temp-2][tempY-3]==check && state[temp-2][tempY-4]==check)
                        return true; //temp-2 왼쪽
                    if(state[temp-2][tempY-2]==check && state[temp-1][tempY-3]==check && state[temp][tempY-4]==check)
                        return true; //temp-2 '/아래'
                }
                //temp
                if(state[temp][tempY]==check && state[temp][tempY-1]==check && state[temp][tempY-2]==check)
                    return true; //temp 왼쪽
                if(state[temp][tempY]==check && state[temp-1][tempY]==check && state[temp-2][tempY]==check)
                    return true; //temp 위
                if(bb>=temp+2){
                    if(state[temp][tempY]==check && state[temp-1][tempY]==check && state[temp-2][tempY]==check)
                        return true; //temp 아래
                    if(state[temp][tempY]==check && state[temp+1][tempY-1]==check && state[temp+2][tempY-2]==check)
                        return true; //temp '/아래'
                }
                if(18>=tempY+2){
                    if(state[temp][tempY]==check && state[temp][tempY+1]==check && state[temp][tempY+2]==check)
                        return true; //temp 오른쪽
                    if(state[temp][tempY]==check && state[temp-1][tempY+1]==check && state[temp-2][tempY+2]==check)
                        return true; //temp '/위'
                }
            }
        }
        count=0;
    }
    //'/' 방향
    for(int i=-MIN(x-tb,rb-y);i<=MIN(y-lb,bb-x)-2;i++){
        for(int j=0;j<3;j++){
            temp=x+(i+j);
            tempY=y-(i+j);
            if(state[temp][tempY]==check)
                count++;
            if(count==3){
                tempY=i-j;
                //temp-2
                if(state[temp-2][tempY+2]==check && state[temp][tempY+1]==check && state[temp][tempY]==check)
                    return true; //temp-2 왼쪽
                if(state[temp-2][tempY+2]==check && state[temp-1][tempY+2]==check && state[temp][tempY+2]==check)
                    return true; //temp-2 아래
                if(0<=temp-4){
                    if(state[temp-2][tempY+2]==check && state[temp-3][tempY+2]==check && state[temp-4][tempY+2]==check)
                        return true; //temp-2 위
                    if(state[temp-2][tempY+2]==check && state[temp-3][tempY+1]==check && state[temp-4][tempY]==check)
                        return true; //temp-2 '\위'
                }
                if(18>=tempY+2){
                    if(state[temp-2][tempY+2]==check && state[temp-2][tempY+1]==check && state[temp-2][tempY+2]==check)
                        return true; //temp-2 오른쪽
                    if(state[temp-2][tempY+2]==check && state[temp-1][temp-3]==check && state[temp][temp-4]==check)
                        return true; //temp-2 '\아래'
                }
                //temp
                if(state[temp][tempY]==check && state[temp][tempY+1]==check && state[temp][tempY+2]==check)
                    return true; //temp 오른쪽
                if(state[temp][tempY]==check && state[temp-1][tempY]==check && state[temp-2][tempY]==check)
                    return true; //temp 위
                if(18>=temp+2){
                    if(state[temp][tempY]==check && state[temp+1][tempY]==check && state[temp+2][tempY]==check)
                        return true; //temp 아래
                    if(state[temp][tempY]==check && state[temp+1][tempY+1]==check && state[temp+2][tempY+2]==check)
                        return true; //temp '\아래'
                }
                if(0<=tempY-2){
                    if(state[temp][tempY]==check && state[temp][tempY-1]==check && state[temp][tempY-2]==check)
                        return true; //temp-2 왼쪽
                    if(state[temp][tempY]==check && state[temp-1][tempY-1]==check && state[temp-1][tempY-2]==check)
                        return true; //temp-2 '\위'
                }
            }
        }
        count=0;
    }
    state[x][y]='+';
    return false;
}

void myturn(bool turn){ //turn이 0이면 흑, 1이면 백
    int X,Y;
    printf("놓을 위치를 입력하세요 (x y 형태로)\n");
    scanf("%d %d",&X,&Y);
    while(status[X][Y]!='+'||threeRule(X, Y, status, turn)){
        printf("놓을 수 없는 위치입니다. 다시 입력하세요 (x y 형태로)\n");
        scanf("%d %d",&X,&Y);
    }
    if(turn==BLACK){
        status[X][Y]='X';
    }
    else{
        status[X][Y]='O';
    }
    print(status);
}

int terminal_test(char state[20][20]){ //-1 : 진행 중, 0 : 흑 승, 1 : 백 승, 2 : 무승부, 3 : 3-3
    int judge_count=0;
    int X=0,Y=0;
    char tracking=' ';
    int empty_count=0; //'+'의 개수
    int phase;
    int saving=0;
    for (int i=0;i<361;i++){
        temp[i][0]=-1;
        temp[i][1]=-1;
    }
    //세로 검사
    while(Y<19){
        while(X<19){
            if (state[X][Y]!='+' && state[X][Y]==tracking){ //돌이 있고 검사하던 것과 같으면
                judge_count++;
                temp[saving][0]=X;
                temp[saving][1]=Y;
                saving++;
                if (judge_count==5){
                    if (tracking=='X'){ //흑이 5개
                        return 0;
                    }
                    else{ //백이 5개
                        return 1;
                    }
                }
            }
            else if (state[X][Y]!='+' && state[X][Y]!=tracking){ //돌이 있고 검사하던 것과 다르면
                temp[saving][0]=X;
                temp[saving][1]=Y;
                saving++;
                tracking = state[X][Y];
                judge_count=1;
                if (X>14){
                    break;
                }
            }
            else{
                judge_count=0;
                empty_count++;
            }
            X++;
        }
        X=0;
        judge_count=0;
        tracking=' ';
        Y++;
    }
    if(empty_count==0){
        return 2;
    }
    //가로 검사
    Y=0;
    while(X<19){
        while(Y<19){
            if (state[X][Y]!='+' && state[X][Y]==tracking){ //돌이 있고 검사하던 것과 같으면
                judge_count++;
                if (judge_count==5){
                    if (tracking=='X'){ //흑이 5개
                        return 0;
                    }
                    else{ //백이 5개
                        return 1;
                    }
                }
            }
            else if (state[X][Y]!='+' && state[X][Y]!=tracking){ //돌이 있고 검사하던 것과 다르면
                tracking = state[X][Y];
                judge_count=1;
                if (Y>14){
                    break;
                }
            }
            else
                judge_count=0;
            Y++;
        }
        Y=0;
        judge_count=0;
        tracking=' ';
        X++;
    }
    // '/'방향 대각선 검사
    X=4;
    Y=0;
    phase=4;
    while(phase<33){
        while((-1<X && X<19) && (-1<Y && Y<19)){
            if (state[X][Y]!='+' && state[X][Y]==tracking){ //돌이 있고 검사하던 것과 같으면
                judge_count++;
                if (judge_count==5){
                    if (tracking=='X'){ //흑이 5개
                        return 0;
                    }
                    else{ //백이 5개
                        return 1;
                    }
                }
            }
            else if (state[X][Y]!='+' && state[X][Y]!=tracking){ //돌이 있고 검사하던 것과 다르면
                tracking = state[X][Y];
                judge_count=1; //더 쉽게 빠져나갈 수 있는 방법 있긴 함
            }
            else
                judge_count=0;
            X--;
            Y++;
        }
        phase++;
        if(phase>18){ //중간보다 넘어감
            X=18;
            Y=phase-18;
        }
        else{
            X=phase;
            Y=0;
        }
        judge_count=0;
        tracking=' ';
    }
    // \방향 검사
    phase=4;
    X=14;
    Y=0;
    while(phase<33){
        while((-1<X && X<19) && (-1<Y && Y<19)){
            if (state[X][Y]!='+' && state[X][Y]==tracking){ //돌이 있고 검사하던 것과 같으면
                judge_count++;
                if (judge_count==5){
                    if (tracking=='X'){ //흑이 5개
                        return 0;
                    }
                    else{ //백이 5개
                        return 1;
                    }
                }
            }
            else if (state[X][Y]!='+' && state[X][Y]!=tracking){ //돌이 있고 검사하던 것과 다르면
                tracking = state[X][Y];
                judge_count=1;
                
            }
            else
                judge_count=0;
            X++;
            Y++;
        }
        phase++;
        if(phase>18){ //중간보다 넘어감
            X=0;
            Y=phase-18;
        }
        else{
            X=18-phase;
            Y=0;
        }
        judge_count=0;
        tracking=' ';
    }
    return -1;
}

bool endJudge(){
    int end = terminal_test(status);
    if (end==0){
        printf("흑 승리!\n");
        return true;
    }
    else if(end==1){
        printf("백 승리!\n");
        return true;
    }
    else if(end==2){
        printf("무승부!\n");
        return true;
    }
    return false;
}


//alpha-beta pruning 관련 함수
void AI_firstTurn(){
    int X = rand()%19;
    int Y = rand()%19;
    while(status[X][Y]!='+'){
        X = rand()%19;
        Y = rand()%19;
    }
    status[X][Y]='X';
    print(status);
}

struct values utility(char state[20][20], bool turn, int terminal,int depth){
    struct values r;
    if(terminal==2){ //무승부
        r.v=0;
    }
    else{
        r = bounded(state,depth,turn);
        r.v-=depth;
    }
    return r;
}

int** actions(char state[20][20],bool turn){
    int** rst;
    bool stateCheck[20][20];
    rst = (int**)malloc(362*(sizeof(int*)));
    int height=0;
    int sp=0;
    int searchRange=2;
    int lb,rb,tb,bb;
    for(int i=0;i<19;i++){
        for(int j=0;j<19;j++)
            stateCheck[i][j]=false;
    }
    while(temp[sp][0]!=-1){
        lb=temp[sp][0]-searchRange;
        rb=temp[sp][0]+searchRange;
        tb=temp[sp][1]-searchRange;
        bb=temp[sp][1]+searchRange;
        if(temp[sp][0]<searchRange)
            lb=0;
        else if(temp[sp][0]>18-searchRange)
            rb=18;
        if(temp[sp][1]<searchRange)
            tb=0;
        else if(temp[sp][1]>18-searchRange)
            bb=18;
        for(int i=lb;i<=rb;i++){
            for(int j=tb;j<=bb;j++){
                if(!stateCheck[i][j]&&state[i][j]=='+'&&!threeRule(i,j,state,turn)){ //비어있으면서 아직 체크안한것, 3-3도 아닌 것
                    rst[height] = (int*)malloc(2*(sizeof(int)));
                    rst[height][0]=i;
                    rst[height][1]=j;
                    stateCheck[i][j]=true;
                    height++;
                }
            }
        }
        sp++;
    }
    rst[height] = (int*)malloc((sizeof(int)));
    rst[height][0]=-1; //끝포인트 설정
    return rst;
}

char (*result(char state[20][20],int action[2],bool turn))[20]{ //turn은 흑이냐 백이냐를 의미
    if (turn==BLACK){
        state[action[0]][action[1]]='X';
    }
    else if(turn==WHITE){
        state[action[0]][action[1]]='O';
    }
    return state;
}

bool compareArray(char a[5],char b[5]){
    for(int i=0;i<5;i++){
        if(a[i]!=b[i])
            return false;
    }
    return true;
}

int compareFive(char test[5],bool turn){
    char oneWhite[5][5] ={{'+','O','O','O','+'},{'O','O','+','O','+'},{'O','+','O','O','+'},{'+','O','O','+','O'},{'+','O','+','O','O'}};
    char oneBlack[5][5] ={{'+','X','X','X','+'},{'X','X','+','X','+'},{'X','+','X','X','+'},{'+','X','X','+','X'},{'+','X','+','X','X'}};
    char twoWhite[3][5]={{'O','O','+','O','O'},{'O','+','O','O','O'},{'O','O','O','+','O'}};
    char twoBlack[3][5]={{'X','X','+','X','X'},{'X','+','X','X','X'},{'X','X','X','+','X'}};
    char threeBlack[2][5]={{'+','X','X','X','X'},{'X','X','X','X','+'}};
    char threeWhite[2][5]={{'+','O','O','O','O'},{'O','O','O','O','+'}};
    char finalBlack[5]={'X','X','X','X','X'};
    char finalWhite[5]={'O','O','O','O','O'};
    bool g_mode;
    if(mode==1)
        g_mode=BLACK;
    else
        g_mode=WHITE;
    
    if(compareArray(finalBlack,test)&&g_mode==BLACK&&turn==BLACK){
        return 100;
    }
    else if(compareArray(finalWhite,test)&&g_mode==WHITE&&turn==WHITE){
        return -100;
    }
    else if(compareArray(finalBlack,test)&&g_mode==BLACK&&turn==BLACK){
        return 90;
    }
    else if(compareArray(finalWhite,test)&&g_mode==WHITE&&turn==WHITE){
        return -90;
    }
    else if(compareArray(finalBlack,test)&&g_mode==BLACK&&turn==BLACK){
        return 300;
    }
    else if(compareArray(finalWhite,test)&&g_mode==WHITE&&turn==WHITE){
        return -300;
    }
    else if(compareArray(finalBlack,test)&&g_mode==BLACK&&turn==BLACK){
        return 200;
    }
    else if(compareArray(finalWhite,test)&&g_mode==WHITE&&turn==WHITE){
        return -200;
    }
    for(int i=0;i<3;i++){
        //one
        if(compareArray(oneBlack[i],test)&&g_mode==BLACK&&turn==BLACK){
            if(i==0)
                return 8;
            return 5;
        }
        else if(compareArray(oneWhite[i],test)&&g_mode==WHITE&&turn==WHITE){
            if(i==0)
                return -8;
            return -5;
        }
        else if(compareArray(oneBlack[i],test)&&g_mode==BLACK&&turn==BLACK){
            if(i==0)
                return 5;
            return 3;
        }
        else if(compareArray(oneWhite[i],test)&&g_mode==WHITE&&turn==WHITE){
            if(i==0)
                return -5;
            return -3;
        }
        else if(compareArray(oneBlack[i],test)&&g_mode==BLACK&&turn==BLACK){
            if(i==0)
                return 58;
            return 50;
        }
        else if(compareArray(oneWhite[i],test)&&g_mode==WHITE&&turn==WHITE){
            if(i==0)
                return -58;
            return -50;
        }
        else if(compareArray(oneBlack[i],test)&&g_mode==BLACK&&turn==BLACK){
            if(i==0)
                return 48;
            return 40;
        }
        else if(compareArray(oneWhite[i],test)&&g_mode==WHITE&&turn==WHITE){
            if(i==0)
                return -48;
            return -40;
        }
        //two
        else if(compareArray(twoBlack[i],test)&&g_mode==BLACK&&turn==BLACK){
            return 15;
        }
        else if(compareArray(twoWhite[i],test)&&g_mode==WHITE&&turn==WHITE){
            return -15;
        }
        else if(compareArray(twoBlack[i],test)&&g_mode==BLACK&&turn==BLACK){
            return 13;
        }
        else if(compareArray(twoWhite[i],test)&&g_mode==WHITE&&turn==WHITE){
            return -13;
        }
        else if(compareArray(twoBlack[i],test)&&g_mode==BLACK&&turn==BLACK){
            return 70;
        }
        else if(compareArray(twoWhite[i],test)&&g_mode==WHITE&&turn==WHITE){
            return -70;
        }
        else if(compareArray(twoBlack[i],test)&&g_mode==BLACK&&turn==BLACK){
            return 60;
        }
        else if(compareArray(twoWhite[i],test)&&g_mode==WHITE&&turn==WHITE){
            return -60;
        }
        //three
        else if(compareArray(threeBlack[i],test)&&g_mode==BLACK&&turn==BLACK){
            return 25;
        }
        else if(compareArray(threeWhite[i],test)&&g_mode==WHITE&&turn==WHITE){
            return -25;
        }
        else if(compareArray(threeBlack[i],test)&&g_mode==BLACK&&turn==BLACK){
            return 23;
        }
        else if(compareArray(threeWhite[i],test)&&g_mode==WHITE&&turn==WHITE){
            return -23;
        }
        else if(compareArray(threeBlack[i],test)&&g_mode==BLACK&&turn==BLACK){
            return 90;
        }
        else if(compareArray(threeWhite[i],test)&&g_mode==WHITE&&turn==WHITE){
            return -90;
        }
        else if(compareArray(threeBlack[i],test)&&g_mode==BLACK&&turn==BLACK){
            return 80;
        }
        else if(compareArray(threeWhite[i],test)&&g_mode==WHITE&&turn==WHITE){
            return -80;
        }
    }
    
    
    
    
    
    /*
    //일단 흑 기준
    if(compareArray(finalBlack, test)){ //5개짜리는 무조건 두거나, 막아야 함
        if(turn==BLACK)
            return 100;
        else
            return
    }
    else if(compareArray(finalWhite,test)){
        if(turn==WHITE)
            return -200;
        else
            return -100;
    }
    for(int i=0; i<3; i++){
        if(compareArray(twoBlack[i], test)){
            return 4;
        }
        else if(compareArray(oneBlack[i], test)){
            return 1;
        }
        else if(compareArray(oneWhite[i], test)){
            if(turn==WHITE)
                return -10;
            else
                return -2;
        }
        else if(compareArray(twoWhite[i], test)){
            if(turn==WHITE)
               return -20;
            else
                return -8;
        }
    }
    for(int i=3;i<5;i++){
        if(compareArray(oneBlack[i], test))
            return 1;
        else if(compareArray(oneWhite[i], test)){
            if(turn==WHITE)
                return -10;
            else
                return -2;
        }
    }
    for(int i=0;i<2;i++){
        if(compareArray(threeBlack[i], test))
            return 15;
        else if(compareArray(threeWhite[i], test)){
            if(turn==WHITE)
                return -30;
            else
                return -10;
        }
    }
     */
    return 0;
}

struct values bounded(char state[20][20],int depth,bool turn){
    struct values rst;
    int X=0,Y=0;
    int phase;
    int point=0;
    char temp[6];
    //세로 검사
    while(Y<19){
        while(X<15){
            for(int i=0; i<5; i++){
                temp[i] = state[X+i][Y];
            }
            point+=compareFive(temp,turn);
            X++;
        }
        X=0;
        Y++;
    }
    //가로 검사
    Y=0;
    while(X<19){
        while(Y<15){
            for(int i=0; i<5; i++){
                temp[i] = state[X][Y+i];
            }
            point+=compareFive(temp,turn);
            Y++;
        }
        Y=0;
        X++;
    }
     // '/'방향 대각선 검사
     X=4;
     Y=0;
     phase=4;
     while(phase<19){
          while(X>3){ //5개이상은 되어야 측정 가능
              for(int i=0; i<5; i++)
                  temp[i] = state[X-i][Y+i];
              point+=compareFive(temp, turn);
              X--;
              Y++;
         }
         X=phase;
         Y=0;
         phase++;
     }
    while(phase<33){
        while(Y>13){ //5개이상은 되어야 측정 가능
            for(int i=0; i<5; i++)
                temp[i] = state[X-i][Y+i];
            point+=compareFive(temp, turn);
            X--;
            Y++;
        }
        X=18;
        Y=phase-18;
        phase++;
    }
    
     // \방향 검사
     phase=1;
     X=14;
     Y=0;
     while(phase<16){
         while(Y<phase){
             for(int i=0; i<5; i++)
                 temp[i] = state[X+i][Y+i];
             point+=compareFive(temp, turn);
             X++;
             Y++;
         }
         X=15-phase;
         Y=0;
         phase++;
     }
     while(phase<30){
        while(18-Y>3){
            for(int i=0; i<5; i++)
                temp[i] = state[X+i][Y+i];
            point+=compareFive(temp, turn);
            X++;
            Y++;
         }
         X=0;
         Y=phase-15;
        phase++;
    }
    rst.x=0;
    rst.y=0;
    rst.v=point;
    return rst;
}


struct values MIN_VALUE(char state[20][20], int alpha, int beta, int depth, int maxDepth,int time_limit){ //백
    depth++;
    //cut-off Test
    struct values rst;
    int terminal = terminal_test(state);
    if (terminal!=-1){
        return utility(state, WHITE, terminal, depth);
    }
    else if(depth>maxDepth){
        return bounded(state,depth,WHITE); //이제 white가 놓을 차례
    }
    char (*tmp)[20];
    struct values rs;
    rst.v=2000000000;
    int** action = actions(state,WHITE);
    int h=0;
    int same=0;
    while(action[h][0]!=-1){
        if(timeout==true||(int)((clock()-startTime)/CLOCKS_PER_SEC)>=time_limit){
            timeout=true;
            return rst;
        }
        tmp = result(state,action[h],WHITE);
        rs = MAX_VALUE(tmp,alpha,beta,depth,maxDepth,time_limit);
        if(rs.v==rst.v){
            if(rand()%(same+2)!=0) //(same+1)/(same+2)의 확률로 버팀
                same++;
            else{
                rst.v = rs.v;
                rst.x = action[h][0];
                rst.y = action[h][1];
                same=0; //rst가 바뀔 때만
            }
        }
        else if(rs.v<rst.v){ //MIN 확인 부분
            rst.v = rs.v;
            rst.x = action[h][0];
            rst.y = action[h][1];
            same=0; //rst가 바뀔 때만
        }
        state[action[h][0]][action[h][1]]='+';
        if(rst.v<=alpha){
            return rst;
        }
        beta=MIN(beta,rst.v);
        h++;
    }
    free(action);
    return rst;
}

struct values MAX_VALUE(char state[20][20], int alpha, int beta, int depth, int maxDepth,int time_limit){ //흑
    depth++;
    struct values rst;
    int terminal =terminal_test(state);
    if (terminal!=-1){
        return utility(state,BLACK,terminal,depth); //이제 흑돌차례
    }
    else if (depth>maxDepth){
        return bounded(state,depth,BLACK);
    }
    char (*tmp)[20];
    struct values rs;
    rst.v=-2000000000;
    int** action = actions(state,BLACK);
    int h=0;
    int same=0;
    while(action[h][0]!=-1){
        if(timeout==true||(int)((clock()-startTime)/CLOCKS_PER_SEC)>=time_limit){
            timeout=true;
            return rst;
        }
        tmp = result(state,action[h],BLACK);
        rs = MIN_VALUE(tmp,alpha,beta,depth,maxDepth,time_limit);
        if(rs.v==rst.v){
            if(rand()%(same+2)!=0) //(same+1)/(same+2)의 확률로 버팀ㅂ
                same++;
            else{
                rst.v = rs.v;
                rst.x = action[h][0];
                rst.y = action[h][1];
                same=0; //rst가 바뀔 때만
            }
        }
        else if(rs.v>rst.v){ //MAX 확인 부분
            rst.v = rs.v;
            rst.x = action[h][0];
            rst.y = action[h][1];
        }
       state[action[h][0]][action[h][1]]='+';
        if(rst.v>=beta){
            return rst;
        }
        alpha=MAX(alpha,rst.v);
        h++;
    }
    free(action);
    return rst;
}

void AI_black(int limit){ //Iterative Deepening
    timeout=false;
    char temp[20][20];
    for(int i=0;i<19;i++)
        for(int j=0;j<19;j++)
            temp[i][j]=status[i][j]; //temp로 원본 판의 값 복사
    int depth=1; //시간 봐서 바꿔줘야 함
    struct values temp_rst;
    struct values rst;
    startTime = clock();
    while(1){
        temp_rst = MIN_VALUE(temp,-2000000000,2000000000,0,depth,limit);
        if(timeout==true){
            break;
        }
        rst=temp_rst;
        depth+=2;
    }
    status[rst.x][rst.y]='X';
    print(status);
    printf("latest : (%d,%d)\n",rst.x,rst.y);
}

void AI_white(int limit){ //Iterative Deepening
    timeout=false;
    char temp[20][20];
    for(int i=0;i<19;i++)
        for(int j=0;j<19;j++)
            temp[i][j]=status[i][j]; //temp로 원본 판의 값 복사
    int depth=1;
    struct values temp_rst;
    struct values rst;
    startTime = clock();
    while(1){
        temp_rst = MIN_VALUE(temp,-2000000000,2000000000,0,depth,limit);
        if(timeout==true){
            break;
        }
        rst=temp_rst;
        depth+=2;
    }
    status[rst.x][rst.y]='O';
    print(status);
    printf("latest : (%d,%d)\n",rst.x,rst.y);
}
void rnd(bool turn){
    int X = rand()%19;
    int Y = rand()%19;
    while(status[X][Y]!='+'){
        X = rand()%19;
        Y = rand()%19;
    }
    if(turn==WHITE){
        status[X][Y]='O';
    }
    else{
        status[X][Y]='X';
    }
    print(status);
}
int main(int argc, const char * argv[]) {
    int time_limit;
    for(int i=0;i<19;i++){
        for (int j=0;j<19;j++){
            status[i][j]='+';
        }
    }
    printf("---------AI_gomoku---------\nset the time_limit\n");
    scanf("%d",&time_limit);
    printf("choose the mode\nYou black : 1\nYou white : 2\nTwo player : 3\n");
    scanf("%d",&mode);
    if (mode==1){ //흑 모드
        print(status);
        while(1){
            myturn(BLACK);
            if(endJudge())
                break;
            AI_white(time_limit); //시간 계산은 AI함수 안에서...
            if (endJudge())
                break;
        }
    }
    else if(mode==2){ //백 모드
        AI_firstTurn();
        while(1){
            myturn(WHITE);
            if (endJudge())
                break;
            AI_black(time_limit);
            if (endJudge())
                break;
        }
    }
    else if(mode==3){ //2P 모드
        while(1){
            myturn(BLACK);
            if (endJudge())
                break;
            myturn(WHITE);
            if (endJudge())
                break;
        }
    }
    printf("Game End\n");
}
//개선시킬 요인 : 탐색 범위, 휴리스틱


