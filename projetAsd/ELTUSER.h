#ifndef ELTUSER_H
#define ELTUSER_H

typedef struct{
 int id;
 char nom[50];
 int age;

}UtilisateurStruct,*ELEMENT;

ELEMENT creerUtilisateur(int id,char*nom,int age);
void afficherUtilisateur(ELEMENT user);
void detruireUtilisateur(ELEMENT user);
#endif // ELTUSER_H
