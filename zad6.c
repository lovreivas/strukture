#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct _article{
    char ime[50];
    int cijena;
    int kolicina;
    struct _article *next;
}article;

typedef struct _racun{
    char datum[50];
    struct _racun *next;
    article arthead;
}racun;

void insert(racun **head,char *s){
    racun *newperson=malloc(sizeof(racun));
    newperson->next=NULL;
    strcpy(newperson->datum,s);
    newperson->arthead.next=NULL; 
    if(*head==NULL){
        *head=newperson;
        return;
    }
    racun *curr=*head;
    while(curr->next!=NULL){
        curr=curr->next;
    }
    curr->next=newperson;
}

void insert_article(article **head, char *ime, int cijena, int kolicina){
    article *newarticle=malloc(sizeof(article));
    newarticle->next=NULL;
    strcpy(newarticle->ime,ime);
    newarticle->cijena = cijena;
    newarticle->kolicina = kolicina;
    if(*head==NULL){
        *head=newarticle;
        return;
    }
    article *curr=*head;
    while(curr->next!=NULL){
        curr=curr->next;
    }
    curr->next=newarticle;
}

void sorta(){
    FILE *f=fopen("racuni.txt","r");
    int i=0,j=0;
    char ss[50];
    while(fgets(ss,50,f)!=NULL){
        i++;
    }
    j=0;
    rewind(f);
    memset(ss, 0, sizeof(ss));
    char s[i][50];
    i=0;
    while(fgets(ss,50,f)){
        strcpy(s[i],ss);
        i++;
    }
    memset(ss, 0, sizeof(ss));
    for(int k=0;k<i;k++){
        for(int jj=k+1;jj<i;jj++){
            if(strcmp(s[k],s[jj])>0){
                strcpy(ss,s[k]);
                strcpy(s[k],s[jj]);
                strcpy(s[jj],ss);
            }
        }}        
    fclose(f);
    FILE *fa=fopen("racuni.txt","w");
    while(j<i){
        fprintf(fa,"%s\n",s[j]);
        j++;
    }
    fclose(fa);
}


int main(){
    racun *head=NULL;
    sorta();
    FILE *f;
    FILE *fa;
    f=fopen("racuni.txt","r");
    fa=fopen("racuni1.txt","w");
    char b[50]; 
    while (fgets(b, sizeof(b), f) != NULL) {
        if (strcmp(b, "\n") != 0 && strcmp(b, "\r\n") != 0) {
            fputs(b, fa);
        }
    }

    fclose(f);
    fclose(fa);

    remove("racuni.txt");
    rename("racuni1.txt", "racuni.txt");
    FILE *f1=fopen("racuni.txt","r");
    char s[50];
    char lines[50];
    char name[50];
    FILE *ff;
    char date[50];
    char buf[50];
    int price=0;
    int amount=0;
    racun *curr1;
    while(fgets(s,50,f1)!=NULL){
        //printf("%s",s);
        snprintf(buf, sizeof(buf), "%s",s);
        ff=fopen(buf,"r");
        while(fgets(date,50,ff)!=NULL){break;};
        //date[strcspn(date, "\n")] = 0;
        //printf("%s",date);
        insert(&head,date);
        curr1=head;
        while(curr1->next != NULL){
            curr1 = curr1->next;
        }
        while(fgets(lines,50,ff)!=NULL){
            //printf("%s",lines);
            sscanf(lines, "%s %d %d",name,&price,&amount);
            insert_article(&(curr1->arthead.next),name,price,amount);
        }
        fclose(ff);
    }
    racun *curr=head;
    while(curr!=NULL){
        printf("%s",curr->datum);
        curr=curr->next;
    }
    fclose(f1);
    
}