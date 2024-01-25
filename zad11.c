#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#define N 11

struct Grad;
typedef struct Grad* PozicijaGrada;
typedef struct Grad
{
    char ime[20];
    int stanovnici;
    PozicijaGrada desno;
    PozicijaGrada lijevo;
} Grad;

struct Drzava;
typedef struct Drzava* PozicijaDrzave;
typedef struct Drzava
{
    char ime[20];
    PozicijaGrada grad;
    PozicijaDrzave sljedeca;
} Drzava;

int dodajDrzavu(PozicijaDrzave glavaDrzava, char* ime, char* datoteka);
int dodajGrad(PozicijaGrada p, char* datoteka);
PozicijaGrada umetniGrad(PozicijaGrada p, char* ime, int stanovnici);
int ispisiDrzave(PozicijaDrzave glavaDrzava);
int ispisiGradove(PozicijaGrada p);
PozicijaDrzave pronadiDrzavu(PozicijaDrzave glavaDrzava, char* ime);
int pronadiGrad(PozicijaGrada p, int brojStanovnika);

int kljucZaDrzavu(char* imeDrzave);

int main(void)
{
    Drzava glava = { .ime = "", .grad = NULL, .sljedeca = NULL };
    Drzava hash[N];

    FILE* datoteka = NULL;
    char imeDrzave[20];
    char datotekaGrada[20];
    PozicijaDrzave glavaDrzava = NULL;
    int brojStanovnika = 0;
    PozicijaGrada glavaGrada = NULL;
    int kljuc = 0;
    int i = 0;

    for (i = 0; i < N; i++)
    {
        hash[i].grad = NULL;
        hash[i].sljedeca = NULL;
        strcpy(hash[i].ime, "");
    }

    datoteka = fopen("drzave.txt", "r");
    if (datoteka == NULL)
    {
        perror("Nije moguće otvoriti datoteku\n");
        return -1;
    }
    while (!feof(datoteka))
    {
        fscanf(datoteka, "%s %s", imeDrzave, datotekaGrada);
        kljuc = kljucZaDrzavu(imeDrzave);
        dodajDrzavu(&hash[kljuc], imeDrzave, datotekaGrada);
    }
    fclose(datoteka);

    printf("Popis država:\n");
    for (i = 0; i < N; i++)
    {
        //printf("Hash[%d]:\n", i);
        ispisiDrzave(hash[i].sljedeca);
    }
    printf("\n\n");

    printf("\nUnesite državu koju želite provjeriti: ");
    scanf("%s", imeDrzave);
    kljuc = kljucZaDrzavu(imeDrzave);
    glavaDrzava = pronadiDrzavu(&hash[kljuc], imeDrzave);
    if (glavaDrzava == NULL)
    {
        printf("Ta država ne postoji\n");
        return 0;
    }
    printf("Minimalan broj stanovnika: ");
    scanf("%d", &brojStanovnika);
    printf("\nGradovi iz te države (%s) s više od %d stanovnika:\n", glavaDrzava->ime, brojStanovnika);
    pronadiGrad(glavaDrzava->grad, brojStanovnika);

    return 0;
}

int dodajDrzavu(PozicijaDrzave glavaDrzava, char* ime, char* datoteka)
{
    PozicijaDrzave noviElement = NULL;
    PozicijaDrzave prethodni = NULL;
    PozicijaGrada glavaGrada = NULL;
    noviElement = (PozicijaDrzave)malloc(sizeof(Drzava));
    if (noviElement == NULL)
    {
        perror("Nije moguće alocirati memoriju\n");
        return -1;
    }
    strcpy(noviElement->ime, ime);
    noviElement->grad = glavaGrada;

    glavaGrada = (PozicijaGrada)malloc(sizeof(Grad));
    if (glavaGrada == NULL)
    {
        perror("Nije moguće alocirati memoriju\n");
        return -1;
    }
    strcpy(glavaGrada->ime, "");
    glavaGrada->stanovnici = 0;
    glavaGrada->desno = NULL;
    glavaGrada->lijevo = NULL;
    noviElement->grad = glavaGrada;
    dodajGrad(noviElement->grad, datoteka);

    prethodni = glavaDrzava;
    while (glavaDrzava != NULL)
    {
        if (glavaDrzava->sljedeca == NULL)
        {
            glavaDrzava->sljedeca = noviElement;
            noviElement->sljedeca = NULL;
            break;
        }
        if (strcmp(noviElement->ime, prethodni->sljedeca->ime) < 0)
        {
            noviElement->sljedeca = prethodni->sljedeca;
            prethodni->sljedeca = noviElement;
            break;
        }
        prethodni = glavaDrzava;
        glavaDrzava = glavaDrzava->sljedeca;
    }
    return 0;
}

int dodajGrad(PozicijaGrada grad, char* datoteka)
{
    FILE* dat = NULL;
    char ime[20];
    int stanovnici = 0;

    dat = fopen(datoteka, "r");
    if (dat == NULL)
    {
        perror("Nije moguće otvoriti datoteku\n");
        return -1;
    }

    while (!feof(dat))
    {
        fscanf(dat, "%s %d", ime, &stanovnici);
        grad = umetniGrad(grad, ime, stanovnici);
    }
    fclose(dat);
    return 0;
}

PozicijaGrada umetniGrad(PozicijaGrada glavaGrada, char* ime, int stanovnici)
{
    PozicijaGrada noviElement = NULL;
    if (glavaGrada == NULL)
    {
        noviElement = (PozicijaGrada)malloc(sizeof(Grad));
        if (noviElement == NULL)
        {
            perror("Nije moguće alocirati memoriju\n");
            return NULL;
        }
        strcpy(noviElement->ime, ime);
        noviElement->stanovnici = stanovnici;
        noviElement->lijevo = NULL;
        noviElement->desno = NULL;
        return noviElement;
    }
    if (stanovnici > glavaGrada->stanovnici)
        glavaGrada->desno = umetniGrad(glavaGrada->desno, ime, stanovnici);
    if (stanovnici < glavaGrada->stanovnici)
        glavaGrada->lijevo = umetniGrad(glavaGrada->lijevo, ime, stanovnici);
    if (stanovnici == glavaGrada->stanovnici)
    {
        if (strcmp(ime, glavaGrada->ime) >= 0)
            glavaGrada->desno = umetniGrad(glavaGrada->desno, ime, stanovnici);
        else
            glavaGrada->lijevo = umetniGrad(glavaGrada->lijevo, ime, stanovnici);
    }

    return glavaGrada;
}

int ispisiDrzave(PozicijaDrzave glavaDrzava)
{
    while (glavaDrzava != NULL)
    {
        printf("\t%s\n", glavaDrzava->ime);
        ispisiGradove(glavaDrzava->grad->desno);
        glavaDrzava = glavaDrzava->sljedeca;
    }
    return 0;
}

int ispisiGradove(PozicijaGrada grad)
{
    if (grad == NULL)
        return 0;
    ispisiGradove(grad->lijevo);
    printf("\t\t%s\n", grad->ime);
    ispisiGradove(grad->desno);
    return 0;
}

PozicijaDrzave pronadiDrzavu(PozicijaDrzave glavaDrzava, char* ime)
{
    while (glavaDrzava != NULL)
    {
        if (strcmp(glavaDrzava->ime, ime) == 0)
            return glavaDrzava;
        glavaDrzava = glavaDrzava->sljedeca;
    }
    return glavaDrzava;
}

int pronadiGrad(PozicijaGrada grad, int brojStanovnika)
{
    if (grad == NULL)
        return 0;
    pronadiGrad(grad->lijevo, brojStanovnika);
    if (grad->stanovnici >= brojStanovnika)
        printf("\t%s %d\n", grad->ime, grad->stanovnici);
    pronadiGrad(grad->desno, brojStanovnika);
    return 0;
}

int kljucZaDrzavu(char* imeDrzave)
{
    int kljuc = 0;

    for (int i = 0; i < 5; i++)
        kljuc += (int)imeDrzave[i];

    return kljuc % N;
}
