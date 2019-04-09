/*
    Program:    Mine
    Autor:  Cvijetinovic Dejan
    Datum:  13.11.2011.

    Ovaj program je uradjen prema igri minesweeper
    korisnik otvara neistrazena polja zadate matrice
    cilj je da otvori sva polja na kojima nisu postavljene mine
    kada korisnik otvori neistrazeno polje moze se desiti da
    otvori minu i time zavrsi dalje trazenje neuspesno
    ako nema mine na zadatom polju, polje ispisuje broj koji oznacava
    koliko ima mina oko tog polja
    korisnik takodje moze postavljati zastavice na neistrazena polja
    na kojima
*/

#include <stdio.h>
#include <stdlib.h>
#define MAX_VRSTA 16
#define MAX_KOLONA 76
#define MASKA_OTVORENO 0x0f
#define MASKA_MINA 0x10
#define MASKA_ZASTAVICA 0x20
#define MASKA_OTVARANJE 0x40
#define ZNAK_PRAZNO_NEOTVORENO '?'
#define ZNAK_ZASTAVICA '#'
#define ZNAK_POGRESNA_ZASTAVICA 'x'
#define ZNAK_MINA '*'
#define ZNAK_OTVORENA_MINA '%'

char **matrica; /* dinamicka matrica polja */
int br_vrsta, br_kolona, br_mina, br_otv_polja, br_zastavica, mina;

void stvaranje_matrice() {
    int i;
    if (!(matrica= malloc(br_vrsta*sizeof(char*)))) {
        printf("***Greska! Problem s memorijom!***\n"); exit(1);
    }
    for (i=0; i<br_vrsta; i++)
        if (!(matrica[i]= malloc(br_kolona*sizeof(char)))) {
            printf("***Greska! Problem s memorijom!***\n"); exit(1);
        }
}

void unistavanje_matrice() {
    int i;
    for (i=0; i<br_vrsta; i++)
        free(matrica[i]);
    free(matrica);
}

void inicijalizacija_matrice() {
    int i, j;
    for (i=0; i<br_vrsta; i++)
        for (j=0; j<br_kolona; j++)
            matrica[i][j]=MASKA_OTVORENO;
}

int ima_mina(int x, int y) {
    if ((matrica[x][y])&MASKA_MINA) return 1;
    else return 0;
}

void postavi_minu(int x, int y) {
    matrica[x][y]|=MASKA_MINA;
}

int ima_zastavica(int x, int y) {
    if ((matrica[x][y])&MASKA_ZASTAVICA) return 1;
    else return 0;
}

void promeni_zastavicu(int x, int y) {
    if (ima_zastavica(x, y)) {
        matrica[x][y]&=~MASKA_ZASTAVICA; br_zastavica--;
    } else {
        matrica[x][y]|=MASKA_ZASTAVICA; br_zastavica++;
    }
}

int ima_otvaranje(int x, int y) {
    if (matrica[x][y]&MASKA_OTVARANJE) return 1;
    else return 0;
}

void postavi_otvaranje(int x, int y) {
    matrica[x][y]|=MASKA_OTVARANJE;
}

void ukloni_otvaranje(int x, int y) {
    matrica[x][y]&=~MASKA_OTVARANJE;
}

int polje_otvoreno(int x, int y) {
    if ((matrica[x][y]&MASKA_OTVORENO)<9) return 1;
    else return 0;
}

void obelezi_polje(int x, int y, char broj) {
    broj&=MASKA_OTVORENO;
    matrica[x][y]&=~MASKA_OTVORENO;
    matrica[x][y]|=broj;
}

int procitaj_broj_polja(int x, int y) {
    return matrica[x][y]&MASKA_OTVORENO;
}

void postavljanje_mina() {
    int i, j, r, rj;
    srand((unsigned)time(NULL));
    for (i=0; i<br_mina; i++) {
        r=rand()%(br_vrsta*br_kolona);
        for (j=0; j<br_vrsta*br_kolona; j++) {
            rj= (r+j)%(br_vrsta*br_kolona);
            if (!ima_mina(rj/br_kolona, rj%br_kolona)) {
                postavi_minu(rj/br_kolona, rj%br_kolona); break;
            }
        }
    }
}

int prebroj_mine (int x, int y) {
    int i=0;
    if (x+1<br_vrsta) {
        if (ima_mina(x+1, y)) i++;
        if (y+1<br_kolona)
            if (ima_mina(x+1, y+1)) i++;
    }
    if (y+1<br_kolona) {
        if (ima_mina(x, y+1)) i++;
        if (x-1>=0)
            if (ima_mina(x-1, y+1)) i++;
    }
    if (x-1>=0) {
        if (ima_mina(x-1, y)) i++;
        if (y-1>=0)
            if (ima_mina(x-1, y-1)) i++;
    }
    if (y-1>=0) {
        if (ima_mina(x, y-1)) i++;
        if (x+1<br_vrsta)
            if (ima_mina(x+1, y-1)) i++;
    }
    return i;
}

int prebroj_zastavice (int x, int y) {
    int i=0;
    if (x+1<br_vrsta) {
        if (ima_zastavica(x+1, y)) i++;
        if (y+1<br_kolona)
            if (ima_zastavica(x+1, y+1)) i++;
    }
    if (y+1<br_kolona) {
        if (ima_zastavica(x, y+1)) i++;
        if (x-1>=0)
            if (ima_zastavica(x-1, y+1)) i++;
    }
    if (x-1>=0) {
        if (ima_zastavica(x-1, y)) i++;
        if (y-1>=0)
            if (ima_zastavica(x-1, y-1)) i++;
    }
    if (y-1>=0) {
        if (ima_zastavica(x, y-1)) i++;
        if (x+1<br_vrsta)
            if (ima_zastavica(x+1, y-1)) i++;
    }
    return i;
}

int otvaraj_okolo (int x, int y) {
    if (x+1<br_vrsta) {
        if (otvaranje_polja(x+1, y)) return 1;
        if (y+1<br_kolona)
            if (otvaranje_polja(x+1, y+1)) return 1;
    }
    if (y+1<br_kolona) {
        if (otvaranje_polja(x, y+1)) return 1;
        if (x-1>=0)
            if (otvaranje_polja(x-1, y+1)) return 1;
    }
    if (x-1>=0) {
        if (otvaranje_polja(x-1, y)) return 1;
        if (y-1>=0)
            if (otvaranje_polja(x-1, y-1)) return 1;
    }
    if (y-1>=0) {
        if (otvaranje_polja(x, y-1)) return 1;
        if (x+1<br_vrsta)
            if (otvaranje_polja(x+1, y-1)) return 1;
    }
    return 0;
}

void prikaz_matrice() {
    int i, j;
    printf("\n  y");
    for (i=1; i<=br_kolona; i++) /* horizontalni lenjir */
        if (!(i%5)) printf("%5d", i);
    printf("\n x ");
    for (i=1; i<=br_kolona; i++)
        if (i%5) putchar(' ');
        else putchar('|');
    for (i=0; i<br_vrsta; i++) {
        printf("\n%2d ", i+1); /* vertikalni lenjir */
        for (j=0; j<br_kolona; j++) {
            if (ima_zastavica(i, j)) {
                if (mina && !ima_mina(i, j)) putchar(ZNAK_POGRESNA_ZASTAVICA);
                else putchar(ZNAK_ZASTAVICA);
            } else if (polje_otvoreno(i, j))
                putchar((char)procitaj_broj_polja(i, j)+0x30);
            else { /* neotvorena polja bez zastavice */
                if (mina && ima_mina(i, j))
                    if (procitaj_broj_polja(i, j)==9) putchar(ZNAK_OTVORENA_MINA);
                    else putchar(ZNAK_MINA);
                else putchar(ZNAK_PRAZNO_NEOTVORENO);
            }
        }
    }
    putchar('\n');
}

void ulaz_koordinata(int *x, int *y) {
    do {
        printf("Unesite koordinate (0<x<=%d, 0<y<=%d): ", br_vrsta, br_kolona);
        scanf("%d%d", x, y);
    } while (*x<1 || *x>br_vrsta || *y<1 || *y>br_kolona);
    (*x)--; (*y)--;
}

/*  Funkcija vraca 1 ako se otvori mina, time se izlazi iz rekurzivnih
    poziva ove funkcije, i zavrsava se pogadjanje
    Inace, funkcija vraca 0 */
int otvaranje_polja(int x, int y) {
    int i;
    if (!ima_zastavica(x, y)) {
        if (!polje_otvoreno(x, y)) {
            if (ima_mina(x, y)) {
                obelezi_polje(x, y, 9); mina=1; return 1;
            }
            i=prebroj_mine(x, y);
            obelezi_polje(x, y, i);
            br_otv_polja++;
            if (i==0)
                if (otvaraj_okolo(x, y)) return 1;
        } else if (polje_otvoreno(x, y) && ima_otvaranje(x, y)) { /* vec otvoreno polje se moze otvoriti samo ako je naredio korisnik, a ne i rekurzivni poziv ove funkcije */
            ukloni_otvaranje(x, y);
            if (procitaj_broj_polja(x, y)==prebroj_zastavice(x, y))
                if (otvaraj_okolo(x, y)) return 1;
        }
    }
    return 0;
}

void konzola() {
    char znak;
    int x, y;
    printf("\no-otvori polje z-postavi/ukloni zastavicu l-legenda q-izlaz ");
    if ((znak=getchar())=='\n')
        znak=getchar();
    if (znak=='o' || znak=='O') { /* otvaranje */
        ulaz_koordinata(&x, &y);
        if (ima_zastavica(x, y))
            printf("To polje se ne moze otvoriti zato sto je postavljena zastavica!\n");
        else {
            if (polje_otvoreno(x, y) && (!ima_zastavica(x, y)))
                postavi_otvaranje(x, y);
            otvaranje_polja(x, y);
        }
    } else if (znak=='z' || znak=='Z') { /* postavljanje/uklanjanje zastavice */
        ulaz_koordinata(&x, &y);
        if (polje_otvoreno(x, y))
            printf("To polje je vec otvoreno. Ne moze se postaviti zastavica!\n");
        else promeni_zastavicu(x, y);
    } else if (znak=='q' || znak=='Q') {
        printf("Uneli ste komandu za izlaz. Kraj programa.\n"); exit(0);
    } else if (znak=='l' || znak=='L') {
        printf("\nLegenda\n\n");
        printf("%c\t-neotvoreno prazno polje\n", ZNAK_PRAZNO_NEOTVORENO);
        printf("0-8\t-otvoreno prazno polje\n");
        printf("%c\t-zastavica\n", ZNAK_ZASTAVICA);
        printf("%c\t-zastavica na pogresnom mestu\n", ZNAK_POGRESNA_ZASTAVICA);
        printf("%c\t-mina\n", ZNAK_MINA);
        printf("%c\t-otvorena mina\n\n", ZNAK_OTVORENA_MINA);
        printf("Za nastavak pritisnuti ENTER ");
        getchar(); getchar();
    } else printf("Uneli ste pogresnu komandu!\n");
}

int main() {
    char znak;
    do {
        mina=0; br_otv_polja=0; br_zastavica=0;
        putchar('\n');
        do {
            printf("Unesite velicinu matrice (1<m<=%d, 1<n<=%d) i broj mina(0<b<m*n): ", MAX_VRSTA, MAX_KOLONA);
            scanf("%d%d%d", &br_vrsta, &br_kolona, &br_mina);
        } while (br_vrsta<=1 || br_vrsta>MAX_VRSTA || br_kolona<=1 || br_kolona>MAX_KOLONA || br_mina<1 || br_mina>=br_vrsta*br_kolona);
        stvaranje_matrice();
        inicijalizacija_matrice();
        postavljanje_mina();
        do {
            prikaz_matrice();
            printf("\nPreostalo je jos %d neobelezenih mina.\n", br_mina-br_zastavica);
            konzola();
        } while (!mina && br_otv_polja+br_mina!=br_vrsta*br_kolona);
        prikaz_matrice();
        unistavanje_matrice();
        if (mina) printf("\nZalim, izgubili ste.\n");
        else printf("\nOtkrili ste sve mine.\n");
        printf("Zelite li nastaviti (d/n)? ");
        if ((znak=getchar())=='\n')
            znak=getchar();
    } while (znak=='d' || znak=='D');
    return 0;
}
