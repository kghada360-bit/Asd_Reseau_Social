#include "ELTUSER.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ELEMENT creerUtilisateur(int id,char*nom,int age){
  ELEMENT user=(ELEMENT)malloc(sizeof(UtilisateurStruct));
   if (user == NULL) {
        printf("Erreur d'allocation mémoire !\n");
        return NULL;
    }
    user->id = id;
    strcpy(user->nom, nom);
    user->age = age;
    return user;
}

void afficherUtilisateur(ELEMENT user) {
    if (user != NULL) {
        printf("ID: %d, Nom: %s, Age: %d\n", user->id, user->nom, user->age);
    }
}

void detruireUtilisateur(ELEMENT user) {
    free(user);
}
