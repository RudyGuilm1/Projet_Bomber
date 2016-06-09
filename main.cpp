#include <allegro.h>
#include <string.h>

BITMAP*page;

class point
{
protected :
    float x,y;
public :
    point(float=100,float=100);
    ~point() { }
    void init(float abs,float ord)
    {
        x=abs;
        y=ord;
    }
    void deplace(float=1,float=1);
};


class rectangl:public point
{
protected :
    float haut,larg;
    int couleur_int;
    int couleur_ext;
    int coul_fond;
    float x1,y1,x2,y2;
public :
    rectangl(float=100,float=100,float=20,float=20,float=4,float=2);
    ~rectangl() { }
    void affiche();
    void init(float, float, float, float,int , int );
    void init();
    int collision(const rectangl&);
};

class eltlaby:public rectangl
{
protected:
    int coulfond, coulcadre;
public:
    eltlaby(float, float, float, float, int, int);
    ~eltlaby();
    eltlaby(const eltlaby&);
    //  eltlaby & operator =(eltlaby &);
    void init(float, float, float, float, int, int);
    void affiche();
};



class labyrinthe
{
protected:
    int dim_bomber, nbrect, coulf, coulc;
    int * tabx, *taby;
    eltlaby** tab;
public:
    labyrinthe(int, int, int, int);
    ~labyrinthe();
    void affiche();
    void init(int, int, int, int);
    int getnbrect(){return nbrect;}
    eltlaby* gettab(int i){return tab[i];}
};

class bomber:public rectangl
{
protected:
    int coulfond, coulcadre;
    float xprec,yprec,coef_speed=0.5;
    int pas;
    //bombe *tab;
public:
    bomber(float, float, float, int, int, int);
    ~bomber();
    bomber(const bomber&);
    //bomber& operator=(bomber &);
    void init(float, float, float, int, int);
    void affiche();
    //int collision(const labyrinthe&);
    int collision(labyrinthe*);
    void mouvement();
    void deplacement();
    void annule();
};

class bombe:public rectangl
{
protected:
    int coulfond;
    float xb,yb;
    int temps;
public:
    bombe(float,float,int,int);
    ~bombe();
    bombe(const bombe&);
    void affiche();
    int collision(labyrinthe*);
    int explosion();
};

point::point(float abs,float ord)
{
    x=abs;
    y=ord;
}

void point::deplace(float dx,float dy)
{
    x+=dx;
    y+=dy;
}


void rectangl::affiche()
{
    couleur_ext=makecol(0,255,0);
//    rectfill(screen,(x-larg/2.), (y-haut/2.), (x+larg/2.), (y+haut/2.), couleur_int); //Remplit le rectangle construit à partir des points des deux coins opposés haut-gauche et bas-droite de la couleur rouge.
    rectfill(page,(x-larg/2.), (y-haut/2.), (x+larg/2.), (y+haut/2.), couleur_int); //Remplit le rectangle construit à partir des points des deux coins opposés haut-gauche et bas-droite de la couleur rouge.
//    rect(screen,(x-larg/2.), (y-haut/2.), (x+larg/2.), (y+haut/2.), couleur_ext); // Trace le tour d'un rectangle à partir des points des deux coins opposés haut-//gauche(20,20) et bas-droite(50,50).
    rect(page,(x-larg/2.), (y-haut/2.), (x+larg/2.), (y+haut/2.), couleur_ext); // Trace le tour d'un rectangle à partir des points des deux coins opposés haut-//gauche(20,20) et bas-droite(50,50).
}

rectangl::rectangl(float abs,float ord,float plarg,float phaut,
                   float cext,float cint):point(abs,ord)
{
    couleur_int=cint;
    couleur_ext=cext;
    larg = plarg;
    haut = phaut;
    coul_fond = makecol(0,0,0);
    x1 = x-larg/2.;
    y1 = y-haut/2.;
    x2 = x+larg/2.;
    y2 = y+haut/2.;
}


int rectangl::collision(const rectangl& f2)
{
    int res=1;

    if ( 	(x1 > f2.x2 ) ||   	// f1 à droite de f2
            (y1 > f2.y2 ) ||   	// f1 en dessous
            (x2  < f2.x1) ||   	// f1 à gauche de f2
            (y2 < f2.y1 )   )    // f1 au dessus
    {
        res=0;	 // pas de collision
    }
    return res;
}


void rectangl::init(float abs,float ord,float plarg,float phaut,int cint, int cext)
{
    x=abs;
    y=ord;
    couleur_int=cint;
    couleur_ext=cext;
    larg = plarg;
    haut = phaut;
    coul_fond = makecol(0,0,0);
    x1 = x-larg/2.;
    y1 = y-haut/2.;
    x2 = x+larg/2.;
    y2 = y+haut/2.;
}

void rectangl::init()
{
    x1 = x-larg/2.;
    y1 = y-haut/2.;
    x2 = x+larg/2.;
    y2 = y+haut/2.;
}


eltlaby::eltlaby(float px, float py, float pl, float ph, int pcf, int pci):rectangl(px, py, pl, ph)
{
    coulfond=pcf;
    coulcadre=pci;
}

eltlaby::~eltlaby() {}

void eltlaby::init(float px, float py, float pl, float ph, int pcf, int pci)
{
    coulfond=pcf;
    coulcadre=pci;
}

void eltlaby::affiche()
{
    rectfill(page, x1, y1, x2, y2, coulfond);
    rect(page, x1, y1, x2, y2, coulcadre);

}


bomber::bomber(float pdim,float px, float py, int pcf, int pci,int ppas):
    rectangl(px+pdim/2, py+pdim/2, pdim, pdim)
{
    xprec = x;
    yprec = y;
    coulfond=pcf;
    coulcadre=pci;
    pas=ppas;
//                     tab=NULL;
}

bomber::~bomber() {}

void bomber::init(float px, float py, float pl, int pcf, int pci)
{
    rectangl::init(px, py, pl, pl, pcf, pci);
}

void bomber::affiche()
{
    rectfill(page, x1, y1, x2, y2, coulfond);
    rect(page, x1, y1, x2, y2, coulcadre);
}


void bomber::deplacement()
{
    xprec = x;
    yprec = y;
    if (key[KEY_UP]) y-=coef_speed*pas;
    if (key[KEY_DOWN]) y+=coef_speed*pas;
    if (key[KEY_LEFT]) x-=coef_speed*pas;
    if (key[KEY_RIGHT]) x+=coef_speed*pas;
    //if (key[KEY_SPACE])

    rectangl::init();
}

void bomber::annule()
{
    x=xprec;
    y=yprec;
    rectangl::init();
}


int bomber::collision(labyrinthe* l)
{
    int i,nb;
    nb=(l->getnbrect())*(l->getnbrect());

    for (i=0; i<nb; i++)
        if(rectangl::collision(*(l->gettab(i)))==1) return 1;


    if ( (x1<0) || (x2 > SCREEN_W) || ( y1<0) || (y2>SCREEN_H)) return 1; //bordures de fenetres

    return 0;
}


void bomber::mouvement()
{
    affiche();
    //textprintf(page, font, 200, 360, 10, "x=%f et y=%f x1 = %f y1=%f",x,y,x1,y1);
}


labyrinthe::labyrinthe(int pnb, int pdim, int pcf, int pcc)
{
    float cumulrect, yinit, xinit, largeur, hauteur;
    int i, j , k ;
    coulf=pcf;
    coulc=pcc;
    dim_bomber=pdim;
    nbrect=pnb;
    cumulrect = SCREEN_H -(nbrect+1) * dim_bomber;
    hauteur = cumulrect / nbrect;
    yinit = dim_bomber + hauteur/2;

    taby = new int[nbrect];
    taby[0] = yinit;
    for (int i=1; i<nbrect; i++)
        taby[i] = taby[i-1] + hauteur + dim_bomber;

    cumulrect = SCREEN_W - (nbrect+1)*dim_bomber;
    largeur = cumulrect/nbrect;
    xinit=dim_bomber + largeur/2;

    tabx = new int[nbrect];
    tabx[0]=xinit;
    for (int i=1; i<nbrect; i++)
        tabx[i]=tabx[i-1]+dim_bomber+largeur;

    tab = new eltlaby*[nbrect*nbrect];

    k=0;
    for (i=0; i<nbrect; i++)
        for (j=0; j<nbrect; j++)
            tab[k++] = new eltlaby(tabx[i], taby[j], largeur, hauteur, pcf, pcc);

}

labyrinthe::~labyrinthe()
{
    delete tabx;
    delete taby;
    for (int i=0; i<nbrect*nbrect; i++)
        delete tab[i];
    delete tab;
}


void labyrinthe::affiche()
{
    for(int i=0; i<nbrect*nbrect; i++)
        tab[i]->affiche();
}

void jeu()
{
    labyrinthe* l;
    bomber *b;
    int d;

    l = new labyrinthe(8,40,makecol(255,0,0),makecol(0,255,0));  //4,40,makecol(255,0,0),makecol(0,255,0));
    b = new bomber(25,1,1,makecol(0,0,255),makecol(200,10,20),1);

    while (!key[KEY_ESC])
    {
        clear(page);
        b->deplacement();

        d = b->collision(l);
        if (!d)
        {
            b->mouvement();
            l->affiche();
            blit(page,screen,0,0,0,0,page->w,page->h);
        }
        else
            b->annule();
    }

    readkey();
    clear(page);
    delete b;
    delete l;
}


int main()
{
    allegro_init();
    install_keyboard();   //installe le clavier
    install_mouse();      //installe la souris

    if (set_gfx_mode(GFX_AUTODETECT_WINDOWED,640,640,0,0)!=0)    // sélectionner un driver graphique approprié +initialisation de la résolution de l'écran (tailles 640-640)
    {
        allegro_message(allegro_error);
        allegro_exit();
        return 1;
    }

    set_mouse_sprite(NULL);
    show_mouse(screen);
    set_keyboard_rate(0,0);

    page=create_bitmap(SCREEN_W,SCREEN_H);

    if (!page)
    {
        allegro_message(allegro_error);
        allegro_exit();
        return 1;
    }

    clear_bitmap(page);

    jeu();

    readkey();

    set_gfx_mode(GFX_TEXT,0,0,0,0);

    exit(EXIT_SUCCESS) ;
}
END_OF_MAIN();
