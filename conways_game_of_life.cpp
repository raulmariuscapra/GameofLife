#include<cstdio>
#include<graphics.h>
#include<windows.h>
#include<stdlib.h>
#include<time.h>

using namespace std;

struct matrice
{
    int mat[250][250],n;
};

int xmax=1000,ymax=600,width,height,sleepval,state=0,culoare=COLOR(42,47,43),bgcolor=COLOR(231,227,226),scene=1,achievements[12],culori[10]={0,0,0,0,0,0,0,0,0,0},boards[6]={0,0,0,0,0,0};
matrice board,boardcitit;
bool medal;

void rules();void set_color();void run();void menu();

//poztiile posinile in ordinea N-W,N,N-E,W,E,S-W,S,S-E/
int pozitiix[50]={-1,-1,-1,0,0,1,1,1},pozitiiy[50]={-1,0,1,-1,1,-1,0,1};

void citire_board()
{
    FILE *f=fopen("board.in","r");
    fscanf(f,"%i",&boardcitit.n);
    for(int i=1;i<=boardcitit.n;i++)
    {
        for(int j=1;j<=boardcitit.n;j++)
        {
            fscanf(f,"%i",&boardcitit.mat[i][j]);
        }
    }
    fclose(f);
}

void scriere_board()
{
    FILE *f=fopen("board.in","w");
    fprintf(f,"%i\n",boardcitit.n);
    for(int i=1;i<=boardcitit.n;i++)
    {
        for(int j=1;j<=boardcitit.n;j++)
        {
            fprintf(f,"%i ",boardcitit.mat[i][j]);
        }
        fprintf(f,"\n");
    }
    fclose(f);
}

//Fereastra care apare de fiecare data cand completam un achievement
void achievement_unlocked(char* text)
{
    cleardevice();
    setcolor(culoare);
    setfillstyle(0,culoare);
    bar(xmax/5,ymax/5,(xmax/5)*4,(ymax/5)*4);
    rectangle(xmax/5,ymax/5,(xmax/5)*4,(ymax/5)*4);
    outtextxy((xmax/100)*42,(ymax/100)*30,(char*)"Achievement Unlocked!");
    outtextxy((xmax/100)*42,(ymax/100)*45,text);
    outtextxy((xmax/100)*42,(ymax/100)*70,(char*)"Left Click to continue");
    swapbuffers();
    int x,y;
    while(!ismouseclick(WM_LBUTTONDOWN));
    getmouseclick(WM_LBUTTONDOWN,x,y);
}

//citire achievementuri pentru a vedea ce achievementuri sunt facute
void citire_achievements()
{
    FILE *f=fopen("achievements.in","r");
    for(int i=1;i<=11;i++)
    {
        fscanf(f,"%i",&achievements[i]);
    }
    int x;
    fscanf(f,"%i",&x);
    if(x==0)
        medal=false;
    else
        medal=true;
    fclose(f);
}

//se scriu achievementurile primite pentru a fi folosite pentru jocurile urmatoare
void scriere_achievements()
{
    FILE *f=fopen("achievements.in","w");
    for(int i=1;i<=11;i++)
    {
        fprintf(f,"%i ",achievements[i]);
    }
    if(medal)
        fprintf(f,"1 ");
    else
        fprintf(f,"0 ");
    fclose(f);
}

//updatarea boardului
matrice update(int x,int y,matrice board2)
{
    int s=0;
    //se calculeaza suma pozitiilor vecinilor din jur/
    for(int i=0;i<8;i++)
    {
        if(x+pozitiix[i]>=0&&y+pozitiiy[i]>=0&&x+pozitiix[i]<board.n&&y+pozitiiy[i]<board.n)
        {
            s=s+board.mat[x+pozitiix[i]][y+pozitiiy[i]];

        }
    }
    //daca in board pe pozitia x y celula e nepopulata si aceasta are mai multi de 2 vecini populati aceasta devine populata in generatia urmatoare/
    if(board.mat[x][y]==0&&s>=3)
    {
        board2.mat[x][y]=1;
    }
    //daca in boardpe pozitia x y celula e populata si aceasta are mai putini de 2 vecini sau mai multi de 3 aceasta va debeni nepopulata in generatia urmatoare/
    if(board.mat[x][y]==1&&s<2||s>3)
    {
        board2.mat[x][y]=0;
    }
    //se returneaza matricea noua(din noua generatie)/
    return board2;

}

//desenarea boardului
void draw_board()
{
    //se sterge ce era inainte
    cleardevice();
    int i,j;
    for(i=0;i<board.n;i++)
    {
        for(j=0;j<board.n;j++)
        {
            //daca este populata atunci se va desena un patrat plin, altfel se va desena un patrat gol
            setfillstyle(board.mat[i][j],culoare);
            bar(j*width,i*height,(j+1)*width,(i+1)*height);
            rectangle(j*width,i*height,(j+1)*width,(i+1)*height);
        }
    }
    //se deseneaza totul nu pe rand fiecare casuta
    swapbuffers();
}

//o=programul unde se arata contribuitori jocului si unde exista multumirile
void thank_you_scrolled()
{
    int i,j;
    cleardevice();
    j=(ymax/5)*4;
    while((j+1600>=ymax/15)&&(!ismouseclick(WM_LBUTTONDOWN)))
    {
        j--;
        cleardevice();
        setfillstyle(1,bgcolor);
        outtextxy((xmax/100)*50-textwidth((char*)"Conway's Game of Life")/2,j,(char*)"Conway's Game of Life");
        outtextxy((xmax/100)*50-textwidth((char*)"Credits")/2,j+100,(char*)"Credits");
        outtextxy((xmax/100)*20,j+200,(char*)"Who took part in the making of this game?");
        outtextxy((xmax/100)*50-textwidth((char*)"Project Manager")/2,j+300,(char*)"Project Manager");
        outtextxy((xmax/100)*20,j+350,(char*)"Capra Marius Raul");
        outtextxy((xmax/100)*50-textwidth((char*)"Programming")/2,j+450,(char*)"Programming");
        outtextxy((xmax/100)*20,j+500,(char*)"Capra Marius Raul");
        outtextxy((xmax/100)*50-textwidth((char*)"Designer")/2,j+600,(char*)"Designer");
        outtextxy((xmax/100)*20,j+650,(char*)"Capra Marius Raul");
        outtextxy((xmax/100)*50-textwidth((char*)"Sound")/2,j+750,(char*)"Sound");
        outtextxy((xmax/100)*20,j+800,(char*)"Nobody(There is no sound)");
        outtextxy((xmax/100)*50-textwidth((char*)"Marketing Campaign")/2,j+900,(char*)"Marketing Campaign");
        outtextxy((xmax/100)*20,j+950,(char*)"Capra Marius Raul");
        outtextxy((xmax/100)*50-textwidth((char*)"Thank you to our beta testers")/2,j+1050,(char*)"Thank you to our beta testers");
        outtextxy((xmax/100)*20,j+1100,(char*)"Andrei Iulia Elena");
        outtextxy((xmax/100)*20,j+1150,(char*)"Capra Marius Raul");
        outtextxy((xmax/100)*20,j+1200,(char*)"Cristea Ioana Alexia");
        outtextxy((xmax/100)*20,j+1250,(char*)"Ilie Dumitru");
        outtextxy((xmax/100)*20,j+1300,(char*)"Ion Daniel");
        outtextxy((xmax/100)*20,j+1350,(char*)"Nita Eduard Eugen");
        outtextxy((xmax/100)*20,j+1400,(char*)"Vasioiu Stefan Cristian");
        outtextxy((xmax/100)*20,j+1500,(char*)"Special thanks to Ceognire Studios and our investor Aurel for making this game possible.");
        outtextxy((xmax/100)*20,j+1600,(char*)"But most of all, thank you for playing this lovely game!");
        bar(0,0,xmax,ymax/10);
        bar(0,(ymax/100)*90,xmax,ymax);
        rectangle((xmax/100)*30,(ymax/100)*93,(xmax/100)*70,(ymax/100)*98);
        outtextxy((xmax/100)*50-textwidth((char*)"Press The Left Mouse Button To Skip")/2,(ymax/100)*95,(char*)"Press The Left Mouse Button To Skip");
        swapbuffers();
        Sleep(3);
    }
    int x=0,y;
    if(ismouseclick(WM_LBUTTONDOWN))
    {
        getmouseclick(WM_LBUTTONDOWN,x,y);
    }
    if(x==0)
    {
        cleardevice();
        settextstyle(3,HORIZ_DIR,10);
        outtextxy((xmax/100)*23,ymax/3,(char*)"Conway's Game of Life");
        settextstyle(3,HORIZ_DIR,2);
        outtextxy((xmax/100)*60,(ymax/100)*50,(char*)"Powered by Ceognire Studios");
        rectangle((xmax/100)*30,(ymax/100)*93,(xmax/100)*70,(ymax/100)*98);
        outtextxy((xmax/100)*50-textwidth((char*)"Press The Left Mouse Button To End")/2,(ymax/100)*95,(char*)"Press The Left Mouse Button To End");
        swapbuffers();
        bool gata=false;
        if(achievements[11]==1&&medal)
            gata=true;
        while(!ismouseclick(WM_LBUTTONDOWN)&&x>=0)
        {
            if(!gata)
            {
                x++;
                if(x==2000)
                {
                    cleardevice();
                    outtextxy((xmax/100)*20,ymax/2,(char*)"What are you doing here?");
                    swapbuffers();
                }
                else if(x==3000)
                {
                    cleardevice();
                    outtextxy((xmax/100)*20,ymax/2,(char*)"You know you can just left click to go, right?");
                    swapbuffers();
                }
                else if(x==4000)
                {
                    cleardevice();
                    outtextxy((xmax/100)*20,ymax/2,(char*)"This is really awkward...");
                    swapbuffers();
                }
                else if(x==5000)
                {
                    cleardevice();
                    outtextxy((xmax/100)*20,ymax/2,(char*)"There is absolutely nothing, just click away!");
                    swapbuffers();
                }
                else if(x==6000)
                {
                    cleardevice();
                    outtextxy((xmax/100)*20,ymax/5,(char*)"Fine because you are so insistent here, you get a medal.");
                    int points1[]={(xmax/100)*40,(ymax/100)*40,(xmax/100)*50,(ymax/100)*42,(xmax/100)*50,(ymax/100)*70,(xmax/100)*40,(ymax/100)*40};
                    int points2[]={(xmax/100)*60,(ymax/100)*40,(xmax/100)*50,(ymax/100)*42,(xmax/100)*50,(ymax/100)*70,(xmax/100)*60,(ymax/100)*40};
                    setcolor(BLACK);
                    setfillstyle(1,BLUE);
                    fillpoly(4,points1);
                    setfillstyle(1,LIGHTBLUE);
                    fillpoly(4,points2);
                    setfillstyle(1,YELLOW);
                    fillellipse((xmax/100)*50,(ymax/100)*70,xmax/20,xmax/20);
                    swapbuffers();
                    if(!medal)
                        medal=true;
                }
                else if(x==9000 && achievements[11]==0)
                {
                    setcolor(culoare);
                    cleardevice();
                    outtextxy((xmax/100)*20,ymax/5,(char*)"Wow you can't just be happy with a medal huh?");
                    outtextxy((xmax/100)*20,ymax/4,(char*)"Fine then, get your achievement and go already!");
                    rectangle((xmax/100)*18,(ymax/100)*40,(xmax/100)*82,(ymax/100)*65);
                    outtextxy((xmax/100)*25,(ymax/100)*42,(char*)"Achievment unlocked!");
                    outtextxy((xmax/100)*30,(ymax/100)*52,(char*)"You are really annoying.");
                    outtextxy((xmax/100)*30,(ymax/100)*58,(char*)"Congratulations!");
                    swapbuffers();
                    achievements[11]=1;
                }
                Sleep(1);
            }
        }
    }
}

//aceste este ecranul pentru pauza care are 2 optiuni si anume de reluare a jocului si de intoarcere la meniu
void stop()
{
    setcolor(culoare);
    setfillstyle(0,culoare);
    bar(xmax/5,ymax/5,(xmax/5)*4,(ymax/5)*4);
    rectangle(xmax/5,ymax/5,(xmax/5)*4,(ymax/5)*4);
    outtextxy(((xmax/5)+(xmax/5)*4)/2-textwidth((char*)"Return to main menu?")/2,(ymax/100)*40,(char*)"Return to main menu?");
    rectangle((xmax/100)*25,(ymax/100)*65,(xmax/100)*35,(ymax/100)*75);
    outtextxy(((xmax/100)*25+(xmax/100)*35)/2-textwidth((char*)"Yes")/2,(ymax/100)*69,(char*)"Yes");
    rectangle((xmax/100)*65,(ymax/100)*65,(xmax/100)*75,(ymax/100)*75);
    outtextxy(((xmax/100)*65+(xmax/100)*75)/2-textwidth((char*)"No")/2,(ymax/100)*69,(char*)"No");
    swapbuffers();
    int x,y,gotox=0;
    bool selectat=false;
    while(!selectat)
    {
        while(!ismouseclick(WM_LBUTTONDOWN));
        getmouseclick(WM_LBUTTONDOWN,x,y);
        if(x>(xmax/100)*25 && x<(xmax/100)*35 && y>(ymax/100)*65 && y<(ymax/100)*75)
        {
            scene=1;
            boardcitit=board;
            selectat=true;
        }
        else if(x>(xmax/100)*65 && x<(xmax/100)*75 && y>(ymax/100)*65 && y<(ymax/100)*75)
        {
            scene=6;
            selectat=true;
        }
    }
}

//programul de rulare unde ruleaza jocul
void run()
{
    int c=1;
    width=xmax/board.n;
    height=ymax/board.n;
    matrice board2=board;
    int i,j;
    while(!ismouseclick(WM_RBUTTONDOWN))
    {
        draw_board();
        for(i=0;i<board.n;i++)
            for(j=0;j<board.n;j++)
                //aici urmatoarea generatie se va schimba de la celula la celula prin metoda update care primeste prima oara geneartia curenta, pozitia x(rand),pozitia y(coloana),si matricea urmatoare care va insemna generatia urmatoare/
                board2=update(i,j,board2);
        //dupa foruri se schimba generatia curenta in generatia noua/
        board=board2;
        Sleep(sleepval);
        c++;
        if(c==100 && achievements[6]==0)
        {
            achievements[6]=1;
            achievement_unlocked((char*)"Live or die?");
        }
    }
    int x,y;
    getmouseclick(WM_RBUTTONDOWN,x,y);
    draw_board();
    stop();
}

//aici vedem daca boardul e gol
bool ghostcity()
{
    for(int i=0;i<board.n;i++)
    {
        for(int j=0;j<board.n;j++)
        {
            if(board.mat[i][j]==1)
                return false;
        }
    }
    return true;
}

//aici vedem daca boardul este plin
bool overpopulation()
{
    for(int i=0;i<board.n;i++)
    {
        for(int j=0;j<board.n;j++)
        {
            if(board.mat[i][j]==0)
                return false;
        }
    }
    return true;
}

//locul unde se fac boardurile daca state e 1 atunci e un board random unde te intreaba marimea boardului dac e 2 e boardul cu glider daca e boardul 3 e boardul cu nava spatiala daca e boardul 4 este un board personalizat prin clip pe celula pe care dorim sa o schimbam
void boardsetup()
{
    cleardevice();
    if(state==1)
    {
        boards[1]=1;
        outtextxy(xmax/2-textwidth((char*)"Conway's Game of Life")/2,height*2,(char*)"Conway's Game of Life");
        outtextxy(5*width,5*height,(char*)"What dimensions do you want the board to have?");
        outtextxy((10*width+35*width)/2-textwidth((char*)"5x5")/2,(10*height+35*height)/2-height,(char*)"5x5");
        rectangle(10*width,10*height,35*width,35*height);
        outtextxy((40*width+65*width)/2-textwidth((char*)"10x10")/2,(10*height+35*height)/2-height,(char*)"10x10");
        rectangle(40*width,10*height,65*width,35*height);
        outtextxy((70*width+95*width)/2-textwidth((char*)"20x20")/2,(10*height+35*height)/2-height,(char*)"20x20");
        rectangle(70*width,10*height,95*width,35*height);
        outtextxy((10*width+35*width)/2-textwidth((char*)"30x30")/2,(40*height+65*height)/2-height,(char*)"30x30");
        rectangle(10*width,40*height,35*width,65*height);
        outtextxy((40*width+65*width)/2-textwidth((char*)"40x40")/2,(40*height+65*height)/2-height,(char*)"40x40");
        rectangle(40*width,40*height,65*width,65*height);
        outtextxy((70*width+95*width)/2-textwidth((char*)"50x50")/2,(40*height+65*height)/2-height,(char*)"50x50");
        rectangle(70*width,40*height,95*width,65*height);
        outtextxy((10*width+35*width)/2-textwidth((char*)"100x100")/2,(70*height+95*height)/2-height,(char*)"100x100");
        rectangle(10*width,70*height,35*width,95*height);
        outtextxy((40*width+65*width)/2-textwidth((char*)"150x150")/2,(70*height+95*height)/2-height,(char*)"150x150");
        rectangle(40*width,70*height,65*width,95*height);
        outtextxy((70*width+95*width)/2-textwidth((char*)"200x200")/2,(70*height+95*height)/2-height,(char*)"200x200");
        rectangle(70*width,70*height,95*width,95*height);
        swapbuffers();
        bool selectat=false;
        int x,y;
        while(!selectat)
        {
            while(!ismouseclick(WM_LBUTTONDOWN));
            getmouseclick(WM_LBUTTONDOWN, x, y);
            if(x>10*width && x<35*width && y>10*height && y<35*height)
            {
                board.n=5;
                selectat=true;
            }
            else if(x>40*width && x<65*width && y>10*height && y<35*height)
            {
                board.n=10;
                selectat=true;
            }
            else if(x>70*width && x<95*width && y>10*height && y<35*height)
            {
                board.n=20;
                selectat=true;
            }
            else if(x>10*width && x<35*width && y>40*height && y<65*height)
            {
                board.n=30;
                selectat=true;
            }
            else if(x>40*width && x<65*width && y>40*height && y<65*height)
            {
                board.n=40;
                selectat=true;
            }
            else if(x>70*width && x<95*width && y>40*height && y<65*height)
            {
                board.n=50;
                selectat=true;
            }
            else if(x>10*width && x<35*width && y>70*height && y<95*height)
            {
                board.n=100;
                selectat=true;
            }
            else if(x>40*width && x<65*width && y>70*height && y<95*height)
            {
                board.n=150;
                selectat=true;
            }
            else if(x>70*width && x<95*width && y>70*height && y<95*height)
            {
                if(achievements[8]==0)
                {
                    achievements[8]=1;
                    achievement_unlocked((char*)"Big dreams High complexity");
                }
                board.n=200;
                selectat=true;
            }
        }
        int i,j;
        srand(time(0));
        for(i=0;i<board.n;i++)
        {
            for(j=0;j<board.n;j++)
            {
                board.mat[i][j]=rand()%2;
            }
        }
    }
    else if(state==2)
    {
        boards[2]=1;
        board.n=20;
        int i,j;
        FILE*f=fopen("conways_game_of_life_glider.in",(char*)"r");
        for(i=0;i<board.n;i++)
        {
            for(j=0;j<board.n;j++)
            {
                fscanf(f,(char*)"%i",&board.mat[i][j]);
            }
        }
        fclose(f);
    }
    else if(state==3)
    {
        boards[3]=1;
        board.n=20;
        int i,j;
        FILE*f=fopen("conways_game_of_life_spaceship.in",(char*)"r");
        for(i=0;i<board.n;i++)
        {
            for(j=0;j<board.n;j++)
            {
                fscanf(f,(char*)"%i",&board.mat[i][j]);
            }
        }
        fclose(f);
    }
    else if(state==4)
    {
        boards[4]=1;
        outtextxy(xmax/2-textwidth((char*)"Conway's Game of Life")/2,height*2,(char*)"Conway's Game of Life");
        outtextxy(5*width,5*height,(char*)"What dimensions do you want the board to have?");
        outtextxy((10*width+35*width)/2-textwidth((char*)"5x5")/2,(10*height+35*height)/2-height,(char*)"5x5");
        rectangle(10*width,10*height,35*width,35*height);
        outtextxy((40*width+65*width)/2-textwidth((char*)"10x10")/2,(10*height+35*height)/2-height,(char*)"10x10");
        rectangle(40*width,10*height,65*width,35*height);
        outtextxy((70*width+95*width)/2-textwidth((char*)"15x15")/2,(10*height+35*height)/2-height,(char*)"15x15");
        rectangle(70*width,10*height,95*width,35*height);
        outtextxy((10*width+35*width)/2-textwidth((char*)"20x20")/2,(40*height+65*height)/2-height,(char*)"20x20");
        rectangle(10*width,40*height,35*width,65*height);
        outtextxy((40*width+65*width)/2-textwidth((char*)"30x30")/2,(40*height+65*height)/2-height,(char*)"30x30");
        rectangle(40*width,40*height,65*width,65*height);
        outtextxy((70*width+95*width)/2-textwidth((char*)"40x40")/2,(40*height+65*height)/2-height,(char*)"40x40");
        rectangle(70*width,40*height,95*width,65*height);
        outtextxy((10*width+35*width)/2-textwidth((char*)"50x50")/2,(70*height+95*height)/2-height,(char*)"50x50");
        rectangle(10*width,70*height,35*width,95*height);
        outtextxy((40*width+65*width)/2-textwidth((char*)"70x70")/2,(70*height+95*height)/2-height,(char*)"70x70");
        rectangle(40*width,70*height,65*width,95*height);
        outtextxy((70*width+95*width)/2-textwidth((char*)"80x80")/2,(70*height+95*height)/2-height,(char*)"80x80");
        rectangle(70*width,70*height,95*width,95*height);
        swapbuffers();
        bool selectat=false;
        int x,y;
        while(!selectat)
        {
            while(!ismouseclick(WM_LBUTTONDOWN));
            getmouseclick(WM_LBUTTONDOWN, x, y);
            if(x>10*width && x<35*width && y>10*height && y<35*height)
            {
                board.n=5;
                selectat=true;
            }
            else if(x>40*width && x<65*width && y>10*height && y<35*height)
            {
                board.n=10;
                selectat=true;
            }
            else if(x>70*width && x<95*width && y>10*height && y<35*height)
            {
                board.n=15;
                selectat=true;
            }
            else if(x>10*width && x<35*width && y>40*height && y<65*height)
            {
                board.n=20;
                selectat=true;
            }
            else if(x>40*width && x<65*width && y>40*height && y<65*height)
            {
                board.n=30;
                selectat=true;
            }
            else if(x>70*width && x<95*width && y>40*height && y<65*height)
            {
                board.n=40;
                selectat=true;
            }
            else if(x>10*width && x<35*width && y>70*height && y<95*height)
            {
                board.n=50;
                selectat=true;
            }
            else if(x>40*width && x<65*width && y>70*height && y<95*height)
            {
                board.n=70;
                selectat=true;
            }
            else if(x>70*width && x<95*width && y>70*height && y<95*height)
            {
                board.n=80;
                selectat=true;
            }
        }
        int i,j;
        for(i=0;i<board.n;i++)
        {
            for(j=0;j<board.n;j++)
            {
                board.mat[i][j]=0;
            }
        }
        cleardevice();
        outtextxy(xmax/2-textwidth((char*)"Conway's Game of Life")/2,height*2,(char*)"Conway's Game of Life");
        outtextxy(15*width,30*height,(char*)"You will be presented with a board of cells.");
        outtextxy(15*width,35*height,(char*)"You can change each cell from dead to alive or vice versa with the left mouse button.");
        outtextxy(15*width,40*height,(char*)"When you are ready to start press the right mouse button.");
        rectangle(10*width,10*height,90*width,65*height);
        outtextxy((40*width+60*width)/2-textwidth((char*)"Continue!")/2,(70*height+95*height)/2-height,(char*)"Continue!");
        rectangle(40*width,70*height,60*width,95*height);
        swapbuffers();
        selectat=false;
        while(!selectat)
        {
            while(!ismouseclick(WM_LBUTTONDOWN));
            getmouseclick(WM_LBUTTONDOWN, x, y);
            if(x>40*width && x<60*width && y>70*height && y<95*height)
            {
                selectat=true;
            }
        }
        width=xmax/board.n;
        height=ymax/board.n;
        int xmare,ymare,x2=0,y2=0;
        while(!ismouseclick(WM_RBUTTONDOWN))
        {
            draw_board();
            while(!ismouseclick(WM_LBUTTONDOWN))
                if(ismouseclick(WM_RBUTTONDOWN))
                {
                    x=board.n*width;
                    y=board.n*height;
                    break;
                }
            if(ismouseclick(WM_RBUTTONDOWN))
            {
                x=board.n*width;
                y=board.n*height;
                break;
            }
            getmouseclick(WM_LBUTTONDOWN,x,y);
            xmare=x/width;
            ymare=y/height;
            if(xmare<board.n && ymare<board.n)
            {
                board.mat[ymare][xmare]=(board.mat[ymare][xmare]+1)%2;
            }
        }
        getmouseclick(WM_RBUTTONDOWN,x,y);
        if(achievements[4]==0 && ghostcity())
        {
            achievements[4]=1;
            achievement_unlocked((char*)"Ghost city");
        }
        if(achievements[5]==0 && overpopulation())
        {
            achievements[5]=1;
            achievement_unlocked((char*)"Overpopulation");
        }
    }
    else if(state==5)
    {
        boards[5]=1;
        board=boardcitit;
    }
    if(achievements[1]==0)
    {
        achievements[1]=1;
        achievement_unlocked((char*)"Birth of a legend");
    }
    if(achievements[9]==0)
    {
        int s=0;
        for(int i=1;i<=5;i++)
        {
            s+=boards[i];
        }
        if(s==5)
        {
            achievements[9]=1;
            achievement_unlocked((char*)"Master of all");
        }
    }
    scene=6;
}

//program ce face xmax si ymax dimensiunile ecranului
void GetDesktopResolution(int& horizontal, int& vertical)
{
   RECT desktop;
   // Get a handle to the desktop window
   const HWND hDesktop = GetDesktopWindow();
   // Get the size of screen to the variable desktop
   GetWindowRect(hDesktop, &desktop);
   // The top left corner will have coordinates (0,0)
   // and the bottom right corner will have coordinates
   // (horizontal, vertical)
   horizontal = desktop.right;
   vertical = desktop.bottom;
}

//programul ce prezinta meniul cu boarduri unde putem alege mai multe boarduri
void menu()
{
    cleardevice();
    height=ymax/100;
    width=xmax/100;
    outtextxy(xmax/2-textwidth((char*)"Conway's Game of Life")/2,height*2,(char*)"Conway's Game of Life");
    outtextxy(width*10,height*5,(char*)"Chose from the presets, a random generated board or make your own board.");
    outtextxy(width*70,height*5,(char*)"Choose a sleep value.");
    outtextxy((10*width+35*width)/2-textwidth((char*)"Random Board")/2,(10*height+35*height)/2-height,(char*)"Random Board");
    rectangle(10*width,10*height,35*width,35*height);
    outtextxy((40*width+65*width)/2-textwidth((char*)"Glider")/2,(10*height+35*height)/2-height,(char*)"Glider");
    rectangle(40*width,10*height,65*width,35*height);
    outtextxy((10*width+35*width)/2-textwidth((char*)"Spaceship")/2,(40*height+65*height)/2-height,(char*)"Spaceship");
    rectangle(10*width,40*height,35*width,65*height);
    outtextxy((40*width+65*width)/2-textwidth((char*)"Personalize!")/2,(40*height+65*height)/2-height,(char*)"Personalize!");
    rectangle(40*width,40*height,65*width,65*height);
    outtextxy(((10*width+35*width)/2+(40*width+(40*width+65*width)/2)/2)/2-textwidth((char*)"Continue")/2,(70*height+95*height)/2-height,(char*)"Continue");
    rectangle((10*width+35*width)/2,70*height,(40*width+(40*width+65*width)/2)/2,95*height);
    outtextxy(((65*width+(40*width+65*width)/2)/2+(70*width+95*width)/2)/2-textwidth((char*)"Return to Main Menu")/2,(70*height+95*height)/2-height,(char*)"Return to Main Menu");
    rectangle((65*width+(40*width+65*width)/2)/2,70*height,(70*width+95*width)/2,95*height);
    outtextxy((70*width+95*width)/2-textwidth((char*)"0,3 secunde")/2,(10*height+35*height)/2-height,(char*)"0,3 secunde");
    rectangle(70*width,10*height,95*width,35*height);
    outtextxy((70*width+95*width)/2-textwidth((char*)"0,5 secunde")/2,(40*height+65*height)/2-height,(char*)"0,5 secunde");
    rectangle(70*width,40*height,95*width,65*height);
    swapbuffers();
    bool selectat=false,selectatarena=false,selectatsleep=false;
    int x,y;
    while(!selectat)
    {
        while(!ismouseclick(WM_LBUTTONDOWN));
        getmouseclick(WM_LBUTTONDOWN, x, y);
        if(x>10*width && x<35*width && y>10*height && y<35*height)
        {
            cleardevice();
            outtextxy(xmax/2-textwidth((char*)"Conway's Game of Life")/2,height*2,(char*)"Conway's Game of Life");
            outtextxy(width*10,height*5,(char*)"Chose from the presets, a random generated board or make your own board.");
            outtextxy(width*70,height*5,(char*)"Choose a sleep value.");
            setfillstyle(1,culoare);
            bar(10*width,10*height,35*width,35*height);
            setbkcolor(culoare);
            setcolor(bgcolor);
            outtextxy((10*width+35*width)/2-textwidth((char*)"Random Board")/2,(10*height+35*height)/2-height,(char*)"Random Board");
            setbkcolor(bgcolor);
            setcolor(culoare);
            rectangle(10*width,10*height,35*width,35*height);
            outtextxy((40*width+65*width)/2-textwidth((char*)"Glider")/2,(10*height+35*height)/2-height,(char*)"Glider");
            rectangle(40*width,10*height,65*width,35*height);
            outtextxy((10*width+35*width)/2-textwidth((char*)"Spaceship")/2,(40*height+65*height)/2-height,(char*)"Spaceship");
            rectangle(10*width,40*height,35*width,65*height);
            outtextxy((40*width+65*width)/2-textwidth((char*)"Personalize!")/2,(40*height+65*height)/2-height,(char*)"Personalize!");
            rectangle(40*width,40*height,65*width,65*height);
            outtextxy(((10*width+35*width)/2+(40*width+(40*width+65*width)/2)/2)/2-textwidth((char*)"Continue")/2,(70*height+95*height)/2-height,(char*)"Continue");
            rectangle((10*width+35*width)/2,70*height,(40*width+(40*width+65*width)/2)/2,95*height);
            outtextxy(((65*width+(40*width+65*width)/2)/2+(70*width+95*width)/2)/2-textwidth((char*)"Return to Main Menu")/2,(70*height+95*height)/2-height,(char*)"Return to Main Menu");
            rectangle((65*width+(40*width+65*width)/2)/2,70*height,(70*width+95*width)/2,95*height);
            outtextxy((70*width+95*width)/2-textwidth((char*)"0,3 secunde")/2,(10*height+35*height)/2-height,(char*)"0,3 secunde");
            rectangle(70*width,10*height,95*width,35*height);
            outtextxy((70*width+95*width)/2-textwidth((char*)"0,5 secunde")/2,(40*height+65*height)/2-height,(char*)"0,5 secunde");
            rectangle(70*width,40*height,95*width,65*height);
            outtextxy(46*width,5*height,(char*)"Choose a sleep value to start!");
            swapbuffers();
            selectatarena=true;
            state=1;
        }
        else if(x>40*width && x<65*width && y>10*height && y<35*height)
        {
            cleardevice();
            outtextxy(xmax/2-textwidth((char*)"Conway's Game of Life")/2,height*2,(char*)"Conway's Game of Life");
            outtextxy(width*10,height*5,(char*)"Chose from the presets, a random generated board or make your own board.");
            outtextxy(width*70,height*5,(char*)"Choose a sleep value.");
            outtextxy((10*width+35*width)/2-textwidth((char*)"Random Board")/2,(10*height+35*height)/2-height,(char*)"Random Board");
            rectangle(10*width,10*height,35*width,35*height);
            setfillstyle(1,culoare);
            bar(40*width,10*height,65*width,35*height);
            setbkcolor(culoare);
            setcolor(bgcolor);
            outtextxy((40*width+65*width)/2-textwidth((char*)"Glider")/2,(10*height+35*height)/2-height,(char*)"Glider");
            setbkcolor(bgcolor);
            setcolor(culoare);
            rectangle(40*width,10*height,65*width,35*height);
            outtextxy((10*width+35*width)/2-textwidth((char*)"Spaceship")/2,(40*height+65*height)/2-height,(char*)"Spaceship");
            rectangle(10*width,40*height,35*width,65*height);
            outtextxy((40*width+65*width)/2-textwidth((char*)"Personalize!")/2,(40*height+65*height)/2-height,(char*)"Personalize!");
            rectangle(40*width,40*height,65*width,65*height);
            outtextxy(((10*width+35*width)/2+(40*width+(40*width+65*width)/2)/2)/2-textwidth((char*)"Continue")/2,(70*height+95*height)/2-height,(char*)"Continue");
            rectangle((10*width+35*width)/2,70*height,(40*width+(40*width+65*width)/2)/2,95*height);
            outtextxy(((65*width+(40*width+65*width)/2)/2+(70*width+95*width)/2)/2-textwidth((char*)"Return to Main Menu")/2,(70*height+95*height)/2-height,(char*)"Return to Main Menu");
            rectangle((65*width+(40*width+65*width)/2)/2,70*height,(70*width+95*width)/2,95*height);
            outtextxy((70*width+95*width)/2-textwidth((char*)"0,3 secunde")/2,(10*height+35*height)/2-height,(char*)"0,3 secunde");
            rectangle(70*width,10*height,95*width,35*height);
            outtextxy((70*width+95*width)/2-textwidth((char*)"0,5 secunde")/2,(40*height+65*height)/2-height,(char*)"0,5 secunde");
            rectangle(70*width,40*height,95*width,65*height);
            outtextxy(46*width,5*height,(char*)"Choose a sleep value to start!");
            swapbuffers();
            selectatarena=true;
            state=2;
        }
        else if(x>10*width && x<35*width && y>40*height && y<65*height)
        {
            cleardevice();
            outtextxy(xmax/2-textwidth((char*)"Conway's Game of Life")/2,height*2,(char*)"Conway's Game of Life");
            outtextxy(width*10,height*5,(char*)"Chose from the presets, a random generated board or make your own board.");
            outtextxy(width*70,height*5,(char*)"Choose a sleep value.");
            outtextxy((10*width+35*width)/2-textwidth((char*)"Random Board")/2,(10*height+35*height)/2-height,(char*)"Random Board");
            rectangle(10*width,10*height,35*width,35*height);
            outtextxy((40*width+65*width)/2-textwidth((char*)"Glider")/2,(10*height+35*height)/2-height,(char*)"Glider");
            rectangle(40*width,10*height,65*width,35*height);
            setfillstyle(1,culoare);
            bar(10*width,40*height,35*width,65*height);
            setbkcolor(culoare);
            setcolor(bgcolor);
            outtextxy((10*width+35*width)/2-textwidth((char*)"Spaceship")/2,(40*height+65*height)/2-height,(char*)"Spaceship");
            setbkcolor(bgcolor);
            setcolor(culoare);
            rectangle(10*width,40*height,35*width,65*height);
            outtextxy((40*width+65*width)/2-textwidth((char*)"Personalize!")/2,(40*height+65*height)/2-height,(char*)"Personalize!");
            rectangle(40*width,40*height,65*width,65*height);
            outtextxy(((10*width+35*width)/2+(40*width+(40*width+65*width)/2)/2)/2-textwidth((char*)"Continue")/2,(70*height+95*height)/2-height,(char*)"Continue");
            rectangle((10*width+35*width)/2,70*height,(40*width+(40*width+65*width)/2)/2,95*height);
            outtextxy(((65*width+(40*width+65*width)/2)/2+(70*width+95*width)/2)/2-textwidth((char*)"Return to Main Menu")/2,(70*height+95*height)/2-height,(char*)"Return to Main Menu");
            rectangle((65*width+(40*width+65*width)/2)/2,70*height,(70*width+95*width)/2,95*height);
            outtextxy((70*width+95*width)/2-textwidth((char*)"0,3 secunde")/2,(10*height+35*height)/2-height,(char*)"0,3 secunde");
            rectangle(70*width,10*height,95*width,35*height);
            outtextxy((70*width+95*width)/2-textwidth((char*)"0,5 secunde")/2,(40*height+65*height)/2-height,(char*)"0,5 secunde");
            rectangle(70*width,40*height,95*width,65*height);
            outtextxy(46*width,5*height,(char*)"Choose a sleep value to start!");
            swapbuffers();
            selectatarena=true;
            state=3;
        }
        else if(x>40*width && x<65*width && y>40*height && y<65*height)
        {
            cleardevice();
            outtextxy(xmax/2-textwidth((char*)"Conway's Game of Life")/2,height*2,(char*)"Conway's Game of Life");
            outtextxy(width*10,height*5,(char*)"Chose from the presets, a random generated board or make your own board.");
            outtextxy(width*70,height*5,(char*)"Choose a sleep value.");
            outtextxy((10*width+35*width)/2-textwidth((char*)"Random Board")/2,(10*height+35*height)/2-height,(char*)"Random Board");
            rectangle(10*width,10*height,35*width,35*height);
            outtextxy((40*width+65*width)/2-textwidth((char*)"Glider")/2,(10*height+35*height)/2-height,(char*)"Glider");
            rectangle(40*width,10*height,65*width,35*height);
            outtextxy((10*width+35*width)/2-textwidth((char*)"Spaceship")/2,(40*height+65*height)/2-height,(char*)"Spaceship");
            rectangle(10*width,40*height,35*width,65*height);
            setfillstyle(1,culoare);
            bar(40*width,40*height,65*width,65*height);
            setbkcolor(culoare);
            setcolor(bgcolor);
            outtextxy((40*width+65*width)/2-textwidth((char*)"Personalize!")/2,(40*height+65*height)/2-height,(char*)"Personalize!");
            setbkcolor(bgcolor);
            setcolor(culoare);
            rectangle(40*width,40*height,65*width,65*height);
            outtextxy(((10*width+35*width)/2+(40*width+(40*width+65*width)/2)/2)/2-textwidth((char*)"Continue")/2,(70*height+95*height)/2-height,(char*)"Continue");
            rectangle((10*width+35*width)/2,70*height,(40*width+(40*width+65*width)/2)/2,95*height);
            outtextxy(((65*width+(40*width+65*width)/2)/2+(70*width+95*width)/2)/2-textwidth((char*)"Return to Main Menu")/2,(70*height+95*height)/2-height,(char*)"Return to Main Menu");
            rectangle((65*width+(40*width+65*width)/2)/2,70*height,(70*width+95*width)/2,95*height);
            outtextxy((70*width+95*width)/2-textwidth((char*)"0,3 secunde")/2,(10*height+35*height)/2-height,(char*)"0,3 secunde");
            rectangle(70*width,10*height,95*width,35*height);
            outtextxy((70*width+95*width)/2-textwidth((char*)"0,5 secunde")/2,(40*height+65*height)/2-height,(char*)"0,5 secunde");
            rectangle(70*width,40*height,95*width,65*height);
            outtextxy(46*width,5*height,(char*)"Choose a sleep value to start!");
            swapbuffers();
            selectatarena=true;
            state=4;
        }
        else if(x>70*width && x<95*width && y>10*height && y<35*height)
        {
            cleardevice();
            outtextxy(xmax/2-textwidth((char*)"Conway's Game of Life")/2,height*2,(char*)"Conway's Game of Life");
            outtextxy(width*10,height*5,(char*)"Chose from the presets, a random generated board or make your own board.");
            outtextxy(width*70,height*5,(char*)"Choose a sleep value.");
            outtextxy((10*width+35*width)/2-textwidth((char*)"Random Board")/2,(10*height+35*height)/2-height,(char*)"Random Board");
            rectangle(10*width,10*height,35*width,35*height);
            outtextxy((40*width+65*width)/2-textwidth((char*)"Glider")/2,(10*height+35*height)/2-height,(char*)"Glider");
            rectangle(40*width,10*height,65*width,35*height);
            outtextxy((10*width+35*width)/2-textwidth((char*)"Spaceship")/2,(40*height+65*height)/2-height,(char*)"Spaceship");
            rectangle(10*width,40*height,35*width,65*height);
            outtextxy((40*width+65*width)/2-textwidth((char*)"Personalize!")/2,(40*height+65*height)/2-height,(char*)"Personalize!");
            rectangle(40*width,40*height,65*width,65*height);
            outtextxy(((10*width+35*width)/2+(40*width+(40*width+65*width)/2)/2)/2-textwidth((char*)"Continue")/2,(70*height+95*height)/2-height,(char*)"Continue");
            rectangle((10*width+35*width)/2,70*height,(40*width+(40*width+65*width)/2)/2,95*height);
            outtextxy(((65*width+(40*width+65*width)/2)/2+(70*width+95*width)/2)/2-textwidth((char*)"Return to Main Menu")/2,(70*height+95*height)/2-height,(char*)"Return to Main Menu");
            rectangle((65*width+(40*width+65*width)/2)/2,70*height,(70*width+95*width)/2,95*height);
            setfillstyle(1,culoare);
            bar(70*width,10*height,95*width,35*height);
            setbkcolor(culoare);
            setcolor(bgcolor);
            outtextxy((70*width+95*width)/2-textwidth((char*)"0,3 secunde")/2,(10*height+35*height)/2-height,(char*)"0,3 secunde");
            setbkcolor(bgcolor);
            setcolor(culoare);
            rectangle(70*width,10*height,95*width,35*height);
            outtextxy((70*width+95*width)/2-textwidth((char*)"0,5 secunde")/2,(40*height+65*height)/2-height,(char*)"0,5 secunde");
            rectangle(70*width,40*height,95*width,65*height);
            outtextxy(47*width,5*height,(char*)"Choose a board to start!");
            swapbuffers();
            selectatsleep=true;
            sleepval=300;
        }
        else if(x>70*width && x<95*width && y>40*height && y<65*height)
        {
            cleardevice();
            outtextxy(xmax/2-textwidth((char*)"Conway's Game of Life")/2,height*2,(char*)"Conway's Game of Life");
            outtextxy(width*10,height*5,(char*)"Chose from the presets, a random generated board or make your own board.");
            outtextxy(width*70,height*5,(char*)"Choose a sleep value.");
            outtextxy((10*width+35*width)/2-textwidth((char*)"Random Board")/2,(10*height+35*height)/2-height,(char*)"Random Board");
            rectangle(10*width,10*height,35*width,35*height);
            outtextxy((40*width+65*width)/2-textwidth((char*)"Glider")/2,(10*height+35*height)/2-height,(char*)"Glider");
            rectangle(40*width,10*height,65*width,35*height);
            outtextxy((10*width+35*width)/2-textwidth((char*)"Spaceship")/2,(40*height+65*height)/2-height,(char*)"Spaceship");
            rectangle(10*width,40*height,35*width,65*height);
            outtextxy((40*width+65*width)/2-textwidth((char*)"Personalize!")/2,(40*height+65*height)/2-height,(char*)"Personalize!");
            rectangle(40*width,40*height,65*width,65*height);
            outtextxy(((10*width+35*width)/2+(40*width+(40*width+65*width)/2)/2)/2-textwidth((char*)"Continue")/2,(70*height+95*height)/2-height,(char*)"Continue");
            rectangle((10*width+35*width)/2,70*height,(40*width+(40*width+65*width)/2)/2,95*height);
            outtextxy(((65*width+(40*width+65*width)/2)/2+(70*width+95*width)/2)/2-textwidth((char*)"Return to Main Menu")/2,(70*height+95*height)/2-height,(char*)"Return to Main Menu");
            rectangle((65*width+(40*width+65*width)/2)/2,70*height,(70*width+95*width)/2,95*height);
            outtextxy((70*width+95*width)/2-textwidth((char*)"0,3 secunde")/2,(10*height+35*height)/2-height,(char*)"0,3 secunde");
            rectangle(70*width,10*height,95*width,35*height);
            setfillstyle(1,culoare);
            bar(70*width,40*height,95*width,65*height);
            setbkcolor(culoare);
            setcolor(bgcolor);
            outtextxy((70*width+95*width)/2-textwidth((char*)"0,5 secunde")/2,(40*height+65*height)/2-height,(char*)"0,5 secunde");
            setbkcolor(bgcolor);
            setcolor(culoare);
            rectangle(70*width,40*height,95*width,65*height);
            outtextxy(47*width,5*height,(char*)"Choose a board to start!");
            swapbuffers();
            selectatsleep=true;
            sleepval=500;
            if(achievements[7]==0)
            {
                achievements[7]=1;
                achievement_unlocked((char*)"ZzZz");
            }
        }
        else if(x>(10*width+35*width)/2 && x<(40*width+(40*width+65*width)/2)/2 && y>70*height && y<95*height)
        {
            state=5;
            selectatarena=true;
            cleardevice();
            outtextxy(xmax/2-textwidth((char*)"Conway's Game of Life")/2,height*2,(char*)"Conway's Game of Life");
            outtextxy(width*10,height*5,(char*)"Chose from the presets, a random generated board or make your own board.");
            outtextxy(width*70,height*5,(char*)"Choose a sleep value.");
            outtextxy((10*width+35*width)/2-textwidth((char*)"Random Board")/2,(10*height+35*height)/2-height,(char*)"Random Board");
            rectangle(10*width,10*height,35*width,35*height);
            outtextxy((40*width+65*width)/2-textwidth((char*)"Glider")/2,(10*height+35*height)/2-height,(char*)"Glider");
            rectangle(40*width,10*height,65*width,35*height);
            outtextxy((10*width+35*width)/2-textwidth((char*)"Spaceship")/2,(40*height+65*height)/2-height,(char*)"Spaceship");
            rectangle(10*width,40*height,35*width,65*height);
            outtextxy((40*width+65*width)/2-textwidth((char*)"Personalize!")/2,(40*height+65*height)/2-height,(char*)"Personalize!");
            rectangle(40*width,40*height,65*width,65*height);
            setfillstyle(1,culoare);
            bar((10*width+35*width)/2,70*height,(40*width+(40*width+65*width)/2)/2,95*height);
            setbkcolor(culoare);
            setcolor(bgcolor);
            outtextxy(((10*width+35*width)/2+(40*width+(40*width+65*width)/2)/2)/2-textwidth((char*)"Continue")/2,(70*height+95*height)/2-height,(char*)"Continue");
            setbkcolor(bgcolor);
            setcolor(culoare);
            rectangle((10*width+35*width)/2,70*height,(40*width+(40*width+65*width)/2)/2,95*height);
            outtextxy(((65*width+(40*width+65*width)/2)/2+(70*width+95*width)/2)/2-textwidth((char*)"Return to Main Menu")/2,(70*height+95*height)/2-height,(char*)"Return to Main Menu");
            rectangle((65*width+(40*width+65*width)/2)/2,70*height,(70*width+95*width)/2,95*height);
            outtextxy((70*width+95*width)/2-textwidth((char*)"0,3 secunde")/2,(10*height+35*height)/2-height,(char*)"0,3 secunde");
            rectangle(70*width,10*height,95*width,35*height);
            outtextxy((70*width+95*width)/2-textwidth((char*)"0,5 secunde")/2,(40*height+65*height)/2-height,(char*)"0,5 secunde");
            rectangle(70*width,40*height,95*width,65*height);
            outtextxy(46*width,5*height,(char*)"Choose a sleep value to start!");
            swapbuffers();
        }
        else if(x>(65*width+(40*width+65*width)/2)/2 && x<(70*width+95*width)/2 && y>70*height && y<95*height)
        {
            scene=1;
            selectat=true;
        }
        if(selectatarena==true&&selectatsleep==true)
        {
            selectat=true;
            scene=5;
        }
    }
}

//programul ce explica regulile
void rules()
{
    if(achievements[2]==0)
    {
        achievements[2]=1;
        achievement_unlocked((char*)"Knowledge is King");
    }
    cleardevice();
    outtextxy(xmax/2-textwidth((char*)"Conway's Game of Life")/2,height*2,(char*)"Conway's Game of Life");
    outtextxy(15*width,15*height,(char*)"The game of life was invented by the mathematician John Horton Conway in 1970.");
    outtextxy(15*width,18*height,(char*)"Someone can interact with the game by creating an initial configuration and observing how it evolves.");
    outtextxy(15*width,25*height,(char*)"This is a 0 player game which follows 5 simple rules.");
    outtextxy(20*width,30*height,(char*)"1. Any cell can be either dead or alive.");
    outtextxy(20*width,35*height,(char*)"2. Any alive cell with fewer than two alive neighbours dies, as if by underpopulation.");
    outtextxy(20*width,40*height,(char*)"3. Any alive cell with more than three alive neighbours dies, as if by overpopulation.");
    outtextxy(20*width,45*height,(char*)"4. Any alive cell with two or three alive neighbours lives on to the next generation.");
    outtextxy(20*width,50*height,(char*)"5. Any dead cell with three or more alive neighbours becomes a live cell, as if by reproduction.");
    outtextxy(15*width,60*height,(char*)"When you are presented with the board and the program is running you can press the right mouse button to stop.");
    rectangle(10*width,10*height,90*width,65*height);
    outtextxy((40*width+60*width)/2-textwidth((char*)"Back to menu")/2,(70*height+95*height)/2-height,(char*)"Back to menu");
    rectangle(40*width,70*height,60*width,95*height);
    swapbuffers();
    bool backk=false;
    int x,y;
    while(!backk)
    {
        while(!ismouseclick(WM_LBUTTONDOWN));
        getmouseclick(WM_LBUTTONDOWN, x, y);
        if(x>40*width && x<60*width && y>70*height && y<95*height)
        {
            backk=true;
        }
    }
    scene=1;
}

//programul ce te pune sa alegi o culoare principala
void set_color()
{
    cleardevice();
    height=ymax/100;
    width=xmax/100;
    setcolor(COLOR(42,47,43));
    outtextxy(xmax/2-textwidth((char*)"Conway's Game of Life")/2,height*2,(char*)"Conway's Game of Life");
    outtextxy(xmax/2-textwidth((char*)"What color do you want the game to have?")/2,5*height,(char*)"What color do you want the game to have?");
    outtextxy((10*width+35*width)/2-textwidth((char*)"Black")/2,(10*height+35*height)/2-height,(char*)"Black");
    rectangle(10*width,10*height,35*width,35*height);
    setcolor(COLOR(84,129,194));
    outtextxy((40*width+65*width)/2-textwidth((char*)"Blue")/2,(10*height+35*height)/2-height,(char*)"Blue");
    rectangle(40*width,10*height,65*width,35*height);
    setcolor(COLOR(81,156,96));
    outtextxy((70*width+95*width)/2-textwidth((char*)"Green")/2,(10*height+35*height)/2-height,(char*)"Green");
    rectangle(70*width,10*height,95*width,35*height);
    setcolor(COLOR(59,170,150));
    outtextxy((10*width+35*width)/2-textwidth((char*)"Turquoise")/2,(40*height+65*height)/2-height,(char*)"Turquoise");
    rectangle(10*width,40*height,35*width,65*height);
    setcolor(COLOR(190,81,79));
    outtextxy((40*width+65*width)/2-textwidth((char*)"Red")/2,(40*height+65*height)/2-height,(char*)"Red");
    rectangle(40*width,40*height,65*width,65*height);
    setcolor(COLOR(166,84,194));
    outtextxy((70*width+95*width)/2-textwidth((char*)"Purple")/2,(40*height+65*height)/2-height,(char*)"Purple");
    rectangle(70*width,40*height,95*width,65*height);
    setcolor(COLOR(193,113,54));
    outtextxy((10*width+35*width)/2-textwidth((char*)"Orange")/2,(70*height+95*height)/2-height,(char*)"Orange");
    rectangle(10*width,70*height,35*width,95*height);
    setcolor(COLOR(100,52,184));
    outtextxy((40*width+65*width)/2-textwidth((char*)"Indigo")/2,(70*height+95*height)/2-height,(char*)"Indigo");
    rectangle(40*width,70*height,65*width,95*height);
    setcolor(COLOR(205,90,189));
    outtextxy((70*width+95*width)/2-textwidth((char*)"Pink")/2,(70*height+95*height)/2-height,(char*)"Pink");
    rectangle(70*width,70*height,95*width,95*height);
    swapbuffers();
    bool selectat=false;
    int x,y;
        while(!selectat)
        {
            while(!ismouseclick(WM_LBUTTONDOWN));
            getmouseclick(WM_LBUTTONDOWN, x, y);
            if(x>10*width && x<35*width && y>10*height && y<35*height)
            {
                culori[1]=1;
                culoare=COLOR(42,47,43);
                selectat=true;
            }
            else if(x>40*width && x<65*width && y>10*height && y<35*height)
            {
                culori[2]=1;
                culoare=COLOR(84,129,194);
                selectat=true;
            }
            else if(x>70*width && x<95*width && y>10*height && y<35*height)
            {
                culori[3]=1;
                culoare=COLOR(81,156,96);
                selectat=true;
            }
            else if(x>10*width && x<35*width && y>40*height && y<65*height)
            {
                culori[4]=1;
                culoare=COLOR(79,190,168);
                selectat=true;
            }
            else if(x>40*width && x<65*width && y>40*height && y<65*height)
            {
                culori[5]=1;
                culoare=COLOR(190,81,79);
                selectat=true;
            }
            else if(x>70*width && x<95*width && y>40*height && y<65*height)
            {
                culori[6]=1;
                culoare=COLOR(166,84,194);
                selectat=true;
            }
            else if(x>10*width && x<35*width && y>70*height && y<95*height)
            {
                culori[7]=1;
                culoare=COLOR(193,113,54);
                selectat=true;
            }
            else if(x>40*width && x<65*width && y>70*height && y<95*height)
            {
                culori[8]=1;
                culoare=COLOR(100,52,184);
                selectat=true;
            }
            else if(x>70*width && x<95*width && y>70*height && y<95*height)
            {
                culori[9]=1;
                culoare=COLOR(205,90,189);
                selectat=true;
            }
        }
    if(achievements[3]==0)
    {
        int s=1;
        for(int i=2;i<=10;i++)
        {
            s+=culori[i];
        }
        if(s==9)
        {
            achievements[3]=1;
            achievement_unlocked((char*)"Color maniac");
        }
    }
    setcolor(culoare);
    scene=1;
}

//programul ce returneaza culoarea alb daca achievementul nu este completat sau culoarea principala daca este completat
int achievementsc(int n)
{
    if(achievements[n]==0)
        return 15;
    return culoare;
}

//meniul unde se explica cum se iau achievemnturi
void achievements_guide()
{
    cleardevice();
    outtextxy(xmax/2-textwidth((char*)"Conway's Game of Life")/2,height*2,(char*)"Conway's Game of Life");
    rectangle(10*width,30*height,90*width,65*height);
    outtextxy(15*width,35*height,(char*)"1.The birth of a legend!->Obtained when starting a board. | 2.Knowledge is King->Knowing the rules of the game");
    outtextxy(15*width,40*height,(char*)"3.Color maniac->For the people who like a lot of color. | 4.Ghost city->Obtained when a board is extinct");
    outtextxy(15*width,45*height,(char*)"5.Overpopulation->The more the merrier. | 6.Live or die->For the people who live long generations");
    outtextxy(15*width,50*height,(char*)"7.ZzZz->Sleeping for long is a gift. | 8.Big Dreams High Complexity->It takes longer to load bigger boards.");
    outtextxy(15*width,55*height,(char*)"9.Master of all->The master of the boards. | 10.Detective->Find life in the main menu.");
    outtextxy(15*width,60*height,(char*)"11.?->It is a secret for a reason.");
    outtextxy((40*width+60*width)/2-textwidth((char*)"Back to achievements")/2,(70*height+95*height)/2-height,(char*)"Back to achievements");
    rectangle(40*width,70*height,60*width,95*height);
    swapbuffers();
    bool backk=false;
    int x,y;
    while(!backk)
    {
        while(!ismouseclick(WM_LBUTTONDOWN));
        getmouseclick(WM_LBUTTONDOWN, x, y);
        if(x>40*width && x<60*width && y>70*height && y<95*height)
        {
            backk=true;
        }
    }
    scene=8;
}

//meniul cu achievementuri unde vedem cat de "complet" este jocul
void achievements_menu()
{
    cleardevice();
    height=ymax/100;
    width=xmax/100;
    outtextxy(xmax/2-textwidth((char*)"Conway's Game of Life")/2,height*2,(char*)"Conway's Game of Life");
    setcolor(achievementsc(1));
    rectangle(width*10,height*10,width*25,height*25);
    outtextxy((width*10+width*25)/2-textwidth((char*)"Birth of a legend!")/2,height*17,(char*)"Birth of a legend!");
    setcolor(achievementsc(2));
    rectangle(width*30,height*10,width*45,height*25);
    outtextxy((width*30+width*45)/2-textwidth((char*)"Knowledge is King")/2,height*17,(char*)"Knowledge is King");
    setcolor(achievementsc(3));
    rectangle(width*50,height*10,width*65,height*25);
    outtextxy((width*50+width*65)/2-textwidth((char*)"Color maniac")/2,height*17,(char*)"Color maniac");
    setcolor(achievementsc(4));
    rectangle(width*70,height*10,width*85,height*25);
    outtextxy((width*70+width*85)/2-textwidth((char*)"Ghost city")/2,height*17,(char*)"Ghost city");
    setcolor(achievementsc(5));
    rectangle(width*10,height*30,width*25,height*45);
    outtextxy((width*10+width*25)/2-textwidth((char*)"Overpopulation")/2,height*37,(char*)"Overpopulation");
    setcolor(achievementsc(6));
    rectangle(width*30,height*30,width*45,height*45);
    outtextxy((width*30+width*45)/2-textwidth((char*)"Live or Die?")/2,height*37,(char*)"Live or Die?");
    setcolor(achievementsc(7));
    rectangle(width*50,height*30,width*65,height*45);
    outtextxy((width*50+width*65)/2-textwidth((char*)"ZzZz")/2,height*37,(char*)"ZzZz");
    setcolor(achievementsc(8));
    rectangle(width*70,height*30,width*85,height*45);
    outtextxy((width*70+width*85)/2-textwidth((char*)"Big dreams High complexity")/2,height*37,(char*)"Big dreams High complexity");
    setcolor(achievementsc(9));
    rectangle(width*10,height*50,width*25,height*65);
    outtextxy((width*10+width*25)/2-textwidth((char*)"Master of all")/2,height*57,(char*)"Master of all");
    setcolor(achievementsc(10));
    rectangle(width*30,height*50,width*45,height*65);
    outtextxy((width*30+width*45)/2-textwidth((char*)"Detective")/2,height*57,(char*)"Detective");
    setcolor(achievementsc(11));
    rectangle(width*50,height*50,width*65,height*65);
    outtextxy((width*50+width*65)/2-textwidth((char*)"?")/2,height*57,(char*)"?");
    setcolor(culoare);
    rectangle(width*70,height*50,width*85,height*65);
    outtextxy((width*70+width*85)/2-textwidth((char*)"Completion")/2,height*55,(char*)"Completion");
    char completion[50];
    int completionnumber=0;
    double precentage;
    for(int i=1;i<=11;i++)
    {
        completionnumber+=achievements[i];
    }
    precentage=(completionnumber*100.0)/11.0;
    sprintf(completion,"%i/11 ->%3.f%%",completionnumber,precentage);
    outtextxy((width*70+width*85)/2-textwidth(completion)/2,height*60,completion);
    rectangle(width*10,height*70,width*25,height*85);
    outtextxy((width*10+width*25)/2-textwidth((char*)"Reset Achievements")/2,height*77,(char*)"Reset Achievements");
    rectangle(width*30,height*70,width*65,height*85);
    outtextxy((width*30+width*65)/2-textwidth((char*)"Left click to return to menu")/2,height*77,(char*)"Left click to return to menu");
    rectangle(width*70,height*70,width*85,height*85);
    outtextxy((width*70+width*85)/2-textwidth((char*)"Achievement Guide")/2,height*77,(char*)"Achievement Guide");
    swapbuffers();
    bool selectat=false;
    int x,y;
    while(!selectat)
    {
        while(!ismouseclick(WM_LBUTTONDOWN));
        getmouseclick(WM_LBUTTONDOWN,x,y);
        if(x>width*10 && x<width*25 && y>height*70 &&y<height*85)
        {
            cleardevice();
            setcolor(culoare);
            setfillstyle(0,culoare);
            bar(xmax/5,ymax/5,(xmax/5)*4,(ymax/5)*4);
            rectangle(xmax/5,ymax/5,(xmax/5)*4,(ymax/5)*4);
            outtextxy(((xmax/5)+(xmax/5)*4)/2-textwidth((char*)"Are you sure you want to reset achievements?")/2,(ymax/100)*40,(char*)"Are you sure you want to reset achievements?");
            outtextxy(((xmax/5)+(xmax/5)*4)/2-textwidth((char*)"You will lose all your progress.")/2,(ymax/100)*50,(char*)"You will lose all your progress.");
            rectangle((xmax/100)*25,(ymax/100)*65,(xmax/100)*35,(ymax/100)*75);
            outtextxy(((xmax/100)*25+(xmax/100)*35)/2-textwidth((char*)"Yes")/2,(ymax/100)*69,(char*)"Yes");
            rectangle((xmax/100)*65,(ymax/100)*65,(xmax/100)*75,(ymax/100)*75);
            outtextxy(((xmax/100)*65+(xmax/100)*75)/2-textwidth((char*)"No")/2,(ymax/100)*69,(char*)"No");
            swapbuffers();
            int x,y;
            bool selectatnou=false;
            while(!selectatnou)
            {
                while(!ismouseclick(WM_LBUTTONDOWN));
                getmouseclick(WM_LBUTTONDOWN,x,y);
                if(x>(xmax/100)*25 && x<(xmax/100)*35 && y>(ymax/100)*65 && y<(ymax/100)*75)
                {
                    for(int i=1;i<=11;i++)
                    {
                        achievements[i]=0;
                        scene=8;
                    }
                    selectat=true;
                    medal=false;
                    selectatnou=true;
                }
                else if(x>(xmax/100)*65 && x<(xmax/100)*75 && y>(ymax/100)*65 && y<(ymax/100)*75)
                {
                    selectatnou=true;
                    selectat=true;
                }
            }
        }
        else if(x>width*30 && x<width*65 && y>height*70 &&y<height*85)
        {
            selectat=true;
            scene=1;
        }
        else if(x>width*70 && x<width*85 && y>height*70 &&y<height*85)
        {
            selectat=true;
            scene=9;
        }
    }
}

//meniul principal
void main_menu()
{
    int number_ac=0;
    cleardevice();
    setcolor(culoare);
    height=ymax/100;
    width=xmax/100;
    outtextxy(xmax/2-textwidth((char*)"Conway's Game of Life")/2,height*8,(char*)"Conway's Game of Life");
    outtextxy((40*width+60*width)/2-textwidth((char*)"Play")/2,(20*height+30*height)/2-height,(char*)"Play");
    rectangle(40*width,20*height,60*width,30*height);
    outtextxy((40*width+60*width)/2-textwidth((char*)"Rules")/2,(35*height+45*height)/2-height,(char*)"Rules");
    rectangle(40*width,35*height,60*width,45*height);
    outtextxy((40*width+60*width)/2-textwidth((char*)"Change Color")/2,(50*height+60*height)/2-height,(char*)"Change Color");
    rectangle(40*width,50*height,60*width,60*height);
    outtextxy((40*width+60*width)/2-textwidth((char*)"Achievements")/2,(65*height+75*height)/2-height,(char*)"Achievements");
    rectangle(40*width,65*height,60*width,75*height);
    outtextxy((40*width+60*width)/2-textwidth((char*)"Quit")/2,(80*height+90*height)/2-height,(char*)"Quit");
    rectangle(40*width,80*height,60*width,90*height);
    if(medal)
    {
        int points1[]={(xmax/100)*83,(ymax/100)*84,(xmax/100)*85,(ymax/100)*849/10,(xmax/100)*85,(ymax/100)*90,(xmax/100)*83,(ymax/100)*84},points2[]={(xmax/100)*87,(ymax/100)*84,(xmax/100)*85,(ymax/100)*849/10,(xmax/100)*85,(ymax/100)*90,(xmax/100)*87,(ymax/100)*84};
        setcolor(BLACK);
        setfillstyle(1,BLUE);
        fillpoly(4,points1);
        setfillstyle(1,LIGHTBLUE);
        fillpoly(4,points2);
        setfillstyle(1,YELLOW);
        fillellipse((xmax/100)*85,(ymax/100)*90,xmax/99,xmax/99);
        setcolor(culoare);
    }
    int points[]={(xmax/100)*12,(ymax/100)*84,(xmax/100)*135/10,(ymax/100)*86,(xmax/100)*13,(ymax/100)*89,(xmax/100)*15,(ymax/100)*87,(xmax/100)*17,(ymax/100)*89,(xmax/100)*165/10,(ymax/100)*86,(xmax/100)*18,(ymax/100)*84,(xmax/100)*16,(ymax/100)*835/10,(xmax/100)*15,(ymax/100)*805/10,(xmax/100)*14,(ymax/100)*835/10,(xmax/100)*12,(ymax/100)*84};
    for(int i=1;i<=11;++i)
    {
        number_ac+=achievements[i];
    }
    if(number_ac==11)
    {
        setcolor(BLACK);
        setfillstyle(1,YELLOW);
        fillpoly(11,points);
        setcolor(culoare);
    }
    swapbuffers();
    bool selectat=false;
    int x,y;
    while(!selectat)
    {
        while(!ismouseclick(WM_LBUTTONDOWN));
        getmouseclick(WM_LBUTTONDOWN, x, y);
        if(x>40*width && x<60*width && y>20*height && y<30*height)
        {
            scene=4;
            selectat=true;
        }
        else if(x>40*width && x<60*width && y>35*height && y<45*height)
        {
            scene=3;
            selectat=true;
        }
        else if(x>40*width && x<60*width && y>50*height && y<60*height)
        {
            scene=2;
            selectat=true;
        }
        else if(x>40*width && x<60*width && y>65*height && y<75*height)
        {
            scene=8;
            selectat=true;
        }
        else if(x>40*width && x<60*width && y>80*height && y<90*height)
        {
            cleardevice();
            setcolor(culoare);
            setfillstyle(0,culoare);
            bar(xmax/5,ymax/5,(xmax/5)*4,(ymax/5)*4);
            rectangle(xmax/5,ymax/5,(xmax/5)*4,(ymax/5)*4);
            outtextxy(((xmax/5)+(xmax/5)*4)/2-textwidth((char*)"Are you sure you want to quit?")/2,(ymax/100)*40,(char*)"Are you sure you want to quit?");
            rectangle((xmax/100)*25,(ymax/100)*65,(xmax/100)*35,(ymax/100)*75);
            outtextxy(((xmax/100)*25+(xmax/100)*35)/2-textwidth((char*)"Yes")/2,(ymax/100)*69,(char*)"Yes");
            rectangle((xmax/100)*65,(ymax/100)*65,(xmax/100)*75,(ymax/100)*75);
            outtextxy(((xmax/100)*65+(xmax/100)*75)/2-textwidth((char*)"No")/2,(ymax/100)*69,(char*)"No");
            swapbuffers();
            int x,y;
            bool selectatnou=false;
            while(!selectatnou)
            {
                while(!ismouseclick(WM_LBUTTONDOWN));
                getmouseclick(WM_LBUTTONDOWN,x,y);
                if(x>(xmax/100)*25 && x<(xmax/100)*35 && y>(ymax/100)*65 && y<(ymax/100)*75)
                {
                        scene=7;
                        selectat=true;
                        selectatnou=true;
                }
                else if(x>(xmax/100)*65 && x<(xmax/100)*75 && y>(ymax/100)*65 && y<(ymax/100)*75)
                {
                    selectatnou=true;
                    selectat=true;
                }
            }
        }
        else if(x>45*width && x<65*width && y>2*width && y<8*width && achievements[10]==0)
        {
            achievements[10]=1;
            achievement_unlocked((char*)"Detecive");
            scene=1;
            selectat=true;
        }
    }
}

//intro cu titlul jocului
void intro()
{
    cleardevice();
    setcolor(0);
    settextstyle(3,HORIZ_DIR,10);
    outtextxy((xmax/100)*23,ymax/3,(char*)"Conway's Game of Life");
    settextstyle(3,HORIZ_DIR,2);
    outtextxy((xmax/100)*60,(ymax/100)*50,(char*)"Powered by Ceognire Studios");
    rectangle((xmax/100)*30,(ymax/100)*93,(xmax/100)*70,(ymax/100)*98);
    outtextxy((xmax/100)*50-textwidth((char*)"Press The Left Mouse Button To Start")/2,(ymax/100)*95,(char*)"Press The Left Mouse Button To Start");
    swapbuffers();
    int x,y;
    while(!ismouseclick(WM_LBUTTONDOWN));
    getmouseclick(WM_LBUTTONDOWN,x,y);
    scene=1;
}

void setup_window()
{
    citire_achievements();
    citire_board();
    GetDesktopResolution(xmax,ymax);
    //se initializeaza desenul , se face backgroundul alb si se face culoarea de scris neagra
    initwindow(xmax,ymax);
    setbkcolor(bgcolor);
    cleardevice();
    intro();
}

int main()
{
    setup_window();
    //se alegea scena astfel 1 meniu principal 2 selectarea culroii 3 regulile 4 meniul cu boarduri 5 setup-uri cu boarduri 6 rularea programului 7 quit 8 achievementuri 9 guide la achievement
    while(scene!=7)
    {
        switch(scene)
        {
            case 1:
                main_menu();
                break;
            case 2:
                set_color();
                break;
            case 3:
                rules();
                break;
            case 4:
                menu();
                break;
            case 5:
                boardsetup();
                break;
            case 6:
                run();
                break;
            case 8:
                achievements_menu();
                break;
            case 9:
                achievements_guide();
        }
    }
    thank_you_scrolled();
    closegraph();
    scriere_achievements();
    scriere_board();
    return 0;
}
