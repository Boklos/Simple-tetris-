#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <conio.h>
 
#define TERRAIN_LARGEUR 10
#define TERRAIN_HAUTEUR 20
#define MOITIE_L 15
#define MOITIE_H 10
#define QUITTER 100
 
#define LIMITE 5
#define VIDE 0
#define BLOC 1
 
#define HAUT 72
#define GAUCHE 75
#define DROIT 77
#define BAS 80
 
typedef char MData ;
 
typedef struct _locationActuelle
{
    int X;
    int Y;
} Location;
 
 
/*  Pour cacher notre curseur */
void curseurInvisible() 
{
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}
 
/* Pour déplacer notre curseur */
void curseurDeplacement(int x, int y)
{
    COORD P;
    P.X = 2*x;
    P.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), P);
}
 
/* Récupérer la touche entrée sur le clavier */
int getKeyDown()
{
    if(kbhit()) return getch();
    else return -1;
}
 
/* Dessiner le terrain */
void dessinerTerrain(MData map[TERRAIN_HAUTEUR][TERRAIN_LARGEUR])
{
    int h, L;
    HANDLE hand = GetStdHandle(STD_OUTPUT_HANDLE);
 
    for(h=0; h<TERRAIN_HAUTEUR; h++)
    {
        for(L=0; L<TERRAIN_LARGEUR; L++)
        {
            curseurDeplacement(L+2,h+2);
            if(map[h][L] == VIDE)
            {
                printf(".");
            }
            else if(map[h][L] == BLOC)
            {
                SetConsoleTextAttribute(hand, 10);
                printf("O");
                SetConsoleTextAttribute(hand, 7);
            }
        }
        printf("\n");
    }
}

/* Dessiner la limite du terrain */
void dessinerLimite(MData map[TERRAIN_HAUTEUR][TERRAIN_LARGEUR])
{
    int h, L;
    HANDLE hand = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hand, 15);
 
    for(h=0; h<=TERRAIN_HAUTEUR +1; h++)
    {
        for(L=0; L<=TERRAIN_LARGEUR +1; L++)
        {
            curseurDeplacement(L+1,h+1);
            if(h==0 || L==0 || h==TERRAIN_HAUTEUR+1 || L==TERRAIN_LARGEUR+1)
                printf("B");
        }
        printf("\n");
    }
    curseurDeplacement(MOITIE_L, MOITIE_H+2);
    printf("Score : ");
    curseurDeplacement(MOITIE_L, MOITIE_H+12);
    printf("Quitter : 't'");
    SetConsoleTextAttribute(hand, 7);
}

 
 /* Dessiner le menu */
int dessinerMenu()
{
    HANDLE hand = GetStdHandle(STD_OUTPUT_HANDLE);
    int keyInput;
    curseurDeplacement(1,2);
    SetConsoleTextAttribute(hand, 15);
    printf("===============================================");
    curseurDeplacement(1,3);
    SetConsoleTextAttribute(hand, 10);
    printf("================= T E T R I S =================");
    SetConsoleTextAttribute(hand, 15);
    curseurDeplacement(1,4);
    printf("===============================================\n");
 
    SetConsoleTextAttribute(hand, 10);
    curseurDeplacement(2,6);
    printf("Gauche : fleche gauche \n");
    curseurDeplacement(2,7);
    printf("Droite : fleche droite \n");
    curseurDeplacement(2,8);
    printf("Quitter: 't' \n");
 
 
    while(1)
    {
        keyInput = getKeyDown();
        if(keyInput == 's' || keyInput == 'S') break;
        if(keyInput == 't' || keyInput == 'T') break;
 
        curseurDeplacement(4, 15);
        SetConsoleTextAttribute(hand, 15);
        printf(" === Appuyez sur 's' pour commencer ===");
        SetConsoleTextAttribute(hand, 7);
        Sleep(1000);
        printf("                            ");
    }
 
    return keyInput;
}

/* Afficher le score */
void afficherScore(int score)
{
    HANDLE hand = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hand, 10);
    curseurDeplacement(MOITIE_L + 4, MOITIE_H+2);
    printf("%4d", score);
    SetConsoleTextAttribute(hand, 7);
}
 

 /* Créer une pièce sur le terrain*/
void formationPiece(MData map[TERRAIN_HAUTEUR][TERRAIN_LARGEUR],int forme[4][4], Location Loc_actuelle)
{
    int h, L;
 
    for(h=0; h<4;h++)
    {
        for(L=0; L<4;L++)
        {
            if(forme[h][L] ==BLOC)
            {
                map[Loc_actuelle.Y+ h][Loc_actuelle.X +L]=BLOC;
            }
        }
    }
}

/* Initialier le jeu */
void init_jeu(MData map[TERRAIN_HAUTEUR][TERRAIN_LARGEUR])
{
    int i, j=0;
    for(i=0;i<TERRAIN_HAUTEUR;i++)
    {
        for(j=0; j<TERRAIN_LARGEUR; j++)
        {
            map[i][j] = VIDE;
        }
    }
}

/* Définir la position initiale du curseur */
void locationInit(Location * Loc_actuelle)
{
    Loc_actuelle->X =3;
    Loc_actuelle->Y =0;
}

/* Copier pièce */
void copierPiece(int formePiece[4][4], int copier[4][4])
{
    int i, j;
    for(i=0;i<4;i++)
    {
        for(j=0; j<4;j++)
        {
            formePiece[i][j] = copier[i][j];
        }
    }
}

/* Dessiner la pièce */
void dessinerPiece(int formePiece[4][4])
{
 
    int forme[7][4][4] = 
    {
            // pièce I
            {{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0}},
            // pièce O
            {{0,0,0,0},{0,1,1,0},{0,1,1,0},{0,0,0,0}},
            // pièce T
            {{0,0,0,0},{0,1,0,0},{1,1,1,0},{0,0,0,0}},
            // pièce Z
            {{0,0,1,0},{0,1,1,0},{0,1,0,0},{0,0,0,0}},
            // pièce S
            {{0,1,0,0},{0,1,1,0},{0,0,1,0},{0,0,0,0}},
            // pièce J
            {{0,0,0,0},{0,1,0,0},{0,1,1,1},{0,0,0,0}},
            // pièce L 
            {{0,0,0,0},{0,1,1,1},{0,1,0,0},{0,0,0,0}}
    };

    switch(rand()%7) 
    {
        case 0:
            copierPiece(formePiece, forme[0]);
            break;
        case 1:
            copierPiece(formePiece, forme[1]);
            break;
        case 2:
            copierPiece(formePiece, forme[2]);
            break;
        case 3:
            copierPiece(formePiece, forme[3]);
            break;
        case 4:
            copierPiece(formePiece, forme[4]);
            break;
        case 5:
            copierPiece(formePiece, forme[5]);
            break;
        case 6:
            copierPiece(formePiece, forme[6]);
            break;
        default :
            break;
    }
}
 
/* Supprime l'emplacement précédent de la pièce */
void supprimerPiece(MData map[TERRAIN_HAUTEUR][TERRAIN_LARGEUR], int formePiece[4][4], Location * Loc_actuelle)
{
    int h, L;
    for(h=0; h<4;h++)
    {
        for(L=0; L<4;L++)
        {
            if(formePiece[h][L] == BLOC)
                map[Loc_actuelle->Y + h][Loc_actuelle->X + L]=VIDE;
        }
    }
}
 
 /* Gestion des collisions */
 
int bloquerGauche(int formePiece[4][4])
{
    int h, L, gauche_L=4;
    for(L=0; L<4;L++)
    {
        for(h=0; h<4;h++)
        {
            if(formePiece[h][L] == BLOC)
            {
                if(gauche_L > L)
                    gauche_L = L;
            }
        }
    }
    return gauche_L;
}

int bloquerDroite(int formePiece[4][4])
{
    int h, L, droite_L=0;
    for(L=3; L>=0;L--)
    {
        for(h=3; h>=0;h--)
        {
            if(formePiece[h][L] == BLOC)
            {
                if(droite_L < L)
                    droite_L = L;
            }
        }
    }
    return droite_L+1;
}

int bloquerBas(int formePiece[4][4])
{
    int h, L, bas_H=0;
    for(L=3; L>=0;L--)
    {
        for(h=3; h>=0;h--)
        {
            if(formePiece[h][L] == BLOC)
            {
                if(bas_H < h)
                    bas_H = h;
            }
        }
    }
    return bas_H+1;
}

int collision_bas(int formePiece[4][4], int L)
{
    int h, bas_H=-1;
    for(h=3; h>=0;h--)
    {
        if(formePiece[h][L] == BLOC)
        {
            if(bas_H < h)
                bas_H = h;
        }
    }
    return bas_H;
}
int collision_gauche(int formePiece[4][4], int h)
{
    int L, gauche_L= 5;
    for(L=0; L<4;L++)
    {
        if(formePiece[h][L] == BLOC)
        {
            if(gauche_L > L)
                gauche_L = L;
        }
    }
    return gauche_L;
}
int collision_droite(int formePiece[4][4], int h)
{
    int L, droite_L= -1;
    for(L=0; L<4;L++)
    {
        if(formePiece[h][L] == BLOC)
        {
            if(droite_L < L)
                droite_L = L;
        }
    }
    return droite_L;
}

/* Déplacer pièce */
void deplacerGauche(MData map[TERRAIN_HAUTEUR][TERRAIN_LARGEUR],int formePiece[4][4], Location *Loc_actuelle)
{
    int gauche_L = bloquerGauche(formePiece);
    int limite[4] ={0};
    int i;
    for(i=0; i<4;i++)
    {
        limite[i] = collision_gauche(formePiece, i);
    }
    if((Loc_actuelle->X) + gauche_L > 0)
    {
        if(!((limite[0] != 5 && map[Loc_actuelle->Y][Loc_actuelle->X + limite[0] -1] != VIDE)
           ||(limite[1] != 5 && map[Loc_actuelle->Y +1][Loc_actuelle->X + limite[1] -1] != VIDE)
           ||(limite[2] != 5 && map[Loc_actuelle->Y +2][Loc_actuelle->X + limite[2] -1] != VIDE)
           ||(limite[3] != 5 && map[Loc_actuelle->Y +3][Loc_actuelle->X + limite[3] -1] != VIDE)))
        {
 
            supprimerPiece(map, formePiece,Loc_actuelle);
            (Loc_actuelle->X)--;
        }
    }
}

void deplacerDroite(MData map[TERRAIN_HAUTEUR][TERRAIN_LARGEUR],int formePiece[4][4], Location *Loc_actuelle)
{
    int droite_L = bloquerDroite(formePiece);
    int limite[4] ={0};
    int i;
    for(i=0; i<4;i++)
    {
        limite[i] = collision_droite(formePiece, i);
 
    }
 
    if((Loc_actuelle->X) + droite_L < TERRAIN_LARGEUR)
    {
        if(!((limite[0] != 5 && map[Loc_actuelle->Y][Loc_actuelle->X + limite[0] +1] != VIDE)
             ||(limite[1] != 5 && map[Loc_actuelle->Y +1][Loc_actuelle->X + limite[1] +1] != VIDE)
             ||(limite[2] != 5 && map[Loc_actuelle->Y +2][Loc_actuelle->X + limite[2] +1] != VIDE)
             ||(limite[3] != 5 && map[Loc_actuelle->Y +3][Loc_actuelle->X + limite[3] +1] != VIDE)))
        {
            supprimerPiece(map, formePiece,Loc_actuelle);
            (Loc_actuelle->X)++;
        }
 
    }
}
 


/* Descendre automatiquement */
int chute_libre(MData map[TERRAIN_HAUTEUR][TERRAIN_LARGEUR], int formePiece[4][4], Location *Loc_actuelle)
{
    int bas_H = bloquerBas(formePiece);
    int limite_bas[4] = {0};
    int i;
    for(i=0; i<4; i++)
    {
        limite_bas[i] = collision_bas(formePiece, i);
    }
    if(Loc_actuelle->Y + bas_H  == TERRAIN_HAUTEUR
       ||(limite_bas[1] != -1 && map[Loc_actuelle->Y + limite_bas[1] +1][Loc_actuelle->X + 1] != VIDE)
       ||(limite_bas[0] != -1 && map[Loc_actuelle->Y + limite_bas[0] +1][Loc_actuelle->X + 0] != VIDE)
       ||(limite_bas[3] != -1 && map[Loc_actuelle->Y + limite_bas[3] +1][Loc_actuelle->X + 3] != VIDE)
       ||(limite_bas[2] != -1 && map[Loc_actuelle->Y + limite_bas[2] +1][Loc_actuelle->X + 2] != VIDE)
       )
    {
        Sleep(1500/5);
        return TRUE;
    }
 
 
    if(Loc_actuelle->Y + bas_H < TERRAIN_HAUTEUR)
    {
        supprimerPiece(map, formePiece, Loc_actuelle);
        Sleep(1500/8);
        (Loc_actuelle->Y)++;
    }
    return FALSE;
}
 
void rotate(MData map[TERRAIN_HAUTEUR][TERRAIN_LARGEUR], int formePiece[4][4], Location *Loc_actuelle)
{
    int i, j;
    int tmp[4][4];
    int droite_L, gauche_L, bas_H;

    for(i=0; i<4;i++)
    {
        for(j=0; j<4;j++)
        {
            if(formePiece[i][j] == BLOC)
            {
                tmp[j][3-i] = 1;
            }

        }
    }

    for(i=0; i<4;i++){
        for(j=0; j<4;j++)
        {
            formePiece[i][j] = VIDE;
        }
    }

    for(i=0; i<4;i++)
    {
        for(j=0; j<4;j++)
        {
            if(tmp[i][j] == 1)
            {
                formePiece[i][j] = BLOC;
            }

        }
    }

    droite_L = bloquerGauche(formePiece);
    if(Loc_actuelle->X + gauche_L <0){
        deplacerGauche(map, formePiece, Loc_actuelle);
        if(gauche_L == 0) deplacerGauche(map, formePiece, Loc_actuelle); //long shape
    }

    gauche_L = bloquerDroite(formePiece);
    if(Loc_actuelle->X + droite_L >TERRAIN_LARGEUR){
        deplacerDroite(map, formePiece, Loc_actuelle);
        if(droite_L == 4)deplacerDroite(map, formePiece, Loc_actuelle); //long shape
    }


    bas_H = bloquerBas(formePiece);
    if(Loc_actuelle->Y + bas_H > TERRAIN_HAUTEUR){
        supprimerPiece(map, formePiece, Loc_actuelle);
        (Loc_actuelle->Y)--;
        if(bas_H ==4) (Loc_actuelle->Y)--;      //long shape
    }
}

/* Supprimer une ligne pleine */
void supprimerLigne(MData map[TERRAIN_HAUTEUR][TERRAIN_LARGEUR], int h)
{
    int L;
    for(L=0 ; L < TERRAIN_LARGEUR ; L++)
    {
        map[h][L] = VIDE;
    }
}

/* Abaisser une ligne quand celle du dessous est vide */
void descendreLigne(MData map[TERRAIN_HAUTEUR][TERRAIN_LARGEUR], int h)
{
    int L;
    while(h > 1)
    {
        for(L=0; L<TERRAIN_LARGEUR;L++)
        {
            map[h][L] = map[h-1][L];
        }
        h--;
    }
 
}

/* Vérifier si la ligne est remplie */
void testLigne(MData map[TERRAIN_HAUTEUR][TERRAIN_LARGEUR], Location Loc_actuelle, int * score)
{
    int h, L, remplie, compteur =0;
 
    for(h=TERRAIN_HAUTEUR ; h >= (Loc_actuelle.Y -1); h--)
    {
        remplie =0;
        for(L=0; L<TERRAIN_LARGEUR  ;L++)
        {
            if(map[h][L] == VIDE)
            {
                break;
            }else{
                remplie++;
            }
        }
 
        if(remplie == TERRAIN_LARGEUR)
        {
            (*score) += 5;
            supprimerLigne(map, h);
            descendreLigne(map, h);
        }
    }
 
}

/* Ecran de fin de partie */
int GameOver(MData map[TERRAIN_HAUTEUR][TERRAIN_LARGEUR],int score)
{
    int L=0;
    for(L=0; L<TERRAIN_LARGEUR; L++)
    {
        if(map[0][L] == BLOC)
        {
            HANDLE hand = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(hand, 10);
            curseurDeplacement(MOITIE_L -7, MOITIE_H-2);
            printf("====== Game Over ======");
            curseurDeplacement(MOITIE_L -6, MOITIE_H-1);
            printf("Votre Score : %4d\n", score);
            SetConsoleTextAttribute(hand, 7);
            curseurDeplacement(1, TERRAIN_HAUTEUR+3);
 
            system("pause");
            return TRUE;
        }
    }
    return FALSE;
}
 
/* Démarrer la partie */
    int lancer_partie(MData map[TERRAIN_HAUTEUR][TERRAIN_LARGEUR])
    {
    int key;
    int limite = FALSE;
    int score =0;
    int formePiece[4][4] = {0};
    int ombrePiece[4][4] = {0};
    Location Loc_actuelle = {2,2};
 
    init_jeu(map);
    dessinerLimite(map);
    dessinerTerrain(map);
 
    locationInit(&Loc_actuelle);
    dessinerPiece(formePiece);
    dessinerPiece(ombrePiece);
    while(1)
    {
        if(limite == TRUE)
        {
            if(GameOver(map,score))
			{
				return QUITTER;
			}
 
            testLigne(map, Loc_actuelle, &score);
            testLigne(map, Loc_actuelle, &score);
            locationInit(&Loc_actuelle);
            copierPiece(formePiece, ombrePiece);
            dessinerPiece(ombrePiece);
            limite = FALSE;
        }
 
        afficherScore(score);
        formationPiece(map,formePiece, Loc_actuelle);
        dessinerTerrain(map);
        limite = chute_libre(map, formePiece, &Loc_actuelle);
        if(limite == TRUE) continue;
 
        key = getKeyDown();
        if(key == 't' || key =='T') break;

        if(key==224 || key ==0)
        {
            key = getch();
            
            if(key == GAUCHE)
            {
                deplacerGauche(map, formePiece, &Loc_actuelle);
            }
            else if(key == HAUT)
            {
                rotate(map, formePiece, &Loc_actuelle);
            }
            else if(key == DROIT)
            {
                deplacerDroite(map, formePiece, &Loc_actuelle);
            }
        }
    }
    return QUITTER;
}
 
 
int main() 
{
    char map[TERRAIN_HAUTEUR][TERRAIN_LARGEUR] ={0};   //map
    int key;
    curseurInvisible();
	
    system("mode con: cols=51 lines=25");   // taille de la console
 
    while(1)
    {
        key = dessinerMenu();
        if(key == 't' || key == 'T') break;
        system("cls");
        lancer_partie(map);
        Sleep(1000/3);
        system("cls");
    }
    return 0;
}
