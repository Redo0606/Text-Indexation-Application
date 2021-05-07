#include "Text_Index_App.h"

t_listePositions * creer_liste_positions()
{
    t_listePositions * liste = malloc (sizeof(t_listePositions));
    liste->debut = NULL;
    liste->nb_elements = 0;
    return liste;
}

int ajouter_position (t_listePositions * listeP, int ligne, int ordre, int num_phrase)
{
    t_Position* pnt = listeP->debut;
    t_Position* ajout = malloc(sizeof(t_Position));
    ajout->numero_ligne = ligne;
    ajout->numero_phrase = num_phrase;
    ajout->ordre = ordre;
    ajout->suivant = NULL;
    if(pnt == NULL) //Si il s'agit d'une liste nulle, on ajoute l'�l�ment en t�te de liste
    {
       ajout->suivant = pnt;
       listeP->debut = ajout;
       listeP->nb_elements++;
       return 1;
    }
    if (listeP->nb_elements == 1) // Si il s'agit d'une liste � une position, il suffit de comparer la t�te avec l'�l�ment � ajouter
    {
        if(pnt->numero_ligne < ajout->numero_ligne) //Lignes diff�rentes ajout en fin de liste
        {
            pnt->suivant = ajout;
            listeP->nb_elements ++;
            return 1;
        }
        if(pnt->numero_ligne > ajout->numero_ligne) //Lignes diff�rentes, ajout en tete de liste
        {
            listeP->debut = ajout;
            ajout->suivant = pnt;
            listeP->nb_elements ++;
            return 1;
        }
        if(pnt->numero_ligne == ajout ->numero_ligne) //Lignes identiques, ajout en fin de liste
        {
            if (pnt->ordre < ajout->ordre)
            {
                pnt->suivant = ajout;
                listeP->nb_elements ++;
                return 1;
            }
            else if (pnt->ordre > ajout->ordre) //Lignes identiques, ajout en t�te de liste
            {
                listeP->debut = ajout;
                ajout->suivant = pnt;
                listeP->nb_elements ++;
                return 1;
            }
        }
    }
    else if (listeP->nb_elements > 1) //Si la liste comporte plus d'un element il faut rechercher
    {
        while (pnt->suivant != NULL) // On parcours notre liste de positions
        {
            if (ajout->numero_ligne == pnt->suivant->numero_ligne) // Si le numero de la ligne est bon
            {
                if (ajout->ordre < pnt->suivant->ordre) // Et que l'ordre est plus petit qu'une position deja existante, on ajoute l'element en milieu de ligne
                {
                    ajout->suivant = pnt->suivant;
                    pnt->suivant = ajout;
                    listeP->nb_elements ++;
                    return 1;
                }
                if (ajout->ordre > pnt->suivant->ordre && pnt->suivant->suivant->numero_ligne != ajout->numero_ligne) // Et que l'ordre est plus grand que tous les autrss �l�ments, on l'ajoute en fin de ligne
                {
                    ajout->suivant = pnt->suivant->suivant;
                    pnt->suivant->suivant = ajout;
                    listeP->nb_elements ++;
                    return 1;
                }
            }
            if (ajout->numero_ligne < pnt->suivant->numero_ligne && ajout->numero_ligne > pnt->numero_ligne) //Si c'est une nouvelle ligne qui se trouve entre deux ligne existante
            {
                ajout->suivant = pnt->suivant;
                pnt->suivant = ajout;
                listeP->nb_elements ++;
                return 1;
            }

            pnt = pnt->suivant;
        }
        if(pnt->suivant == NULL) // Si il s'agit d'une nouvelle ligne, on ajoute l'�l�ment en fin de liste
        {
            pnt->suivant = ajout;
            listeP->nb_elements ++;
            return 1;
        }
    }

}

t_Index* creer_index(){
    int nbr_mots=0;
    int nbr_mots_diff=0;
    t_Index* newIndex =malloc (sizeof(t_Index));
    newIndex->racine=NULL;
    newIndex->nb_mots_differents=nbr_mots_diff;
    newIndex->nb_mots_total=nbr_mots;
    return newIndex;
}

char* lower_case (char * mot)
{
    char * new_mot = malloc(sizeof(char));
    strcpy(new_mot, mot);
    for (int i = 0; i < strlen(mot); i++)
    {
        if ((mot[i] >= 65) && (mot[i] <= 90))
            {
                new_mot[i] = mot[i] + 32;
            }
    }
    return new_mot;
}

int egal (char * mot1, char * mot2)
{
    mot1 = lower_case(mot1);
    mot2 = lower_case(mot2);
    if(strlen(mot1) != strlen(mot2))
    {
        return 0;
    }
    else
    {
        int r = 0;
        for (int i=0; i< strlen(mot1); i++)
        {
            if (mot1[i] == mot2[i])
            {
                r ++;
            }
        }
        if (r == strlen(mot1))
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
}

int compare (char * mot1, char * mot2) // Compare si le mot 1 est inf�rieur au mot 2
{
    printf("   ");
    int i=0;
    while(mot1[i]==mot2[i] || mot1[i]-mot2[i]==32 || mot1[i]-mot2[i]==(-32)){
        i++;
    }
    if(mot2[i]>=65 && mot2[i]<=90 && mot1[i]>=97 && mot2[i]<=122 ){
        if(mot1[i]>mot2[i]+32 || mot2[i]=='\0')return 1;
        return 0;
    }
    if(mot1[i]>=65 && mot1[i]<=90 && mot2[i]>=97 && mot2[i]<=122 ){
        if(mot1[i]+32>mot2[i] || mot2[i]=='\0')return 1;
        return 0;
    }
    if(mot1[i]>mot2[i] || mot2[i]=='\0')return 1;
    return 0;

}

t_Noeud* recherche (t_Noeud* noeud, char* mot)
{
    if (noeud == NULL || egal(noeud->mot, mot) == 1)
    {
        return noeud;
    }
    else
    {
        if (compare(noeud->mot, mot) == 0)
        {
            return recherche(noeud->filsGauche, mot);
        }
        else if (compare(noeud->mot, mot) == 1)
        {
            return recherche(noeud->filsDroit, mot);
        }
    }

}

t_Noeud* rechercher_mot(t_Index *index, char *mot)
{
    t_Noeud* node = malloc(sizeof(t_Noeud));
    if (index->racine == NULL)
    {
        node = NULL;
        return node;
    }
    else
    {
        node = recherche(index->racine, mot);
        return node;
    }
}

int ajouter_noeud (t_Index * index, t_Noeud* noeud)
{
    t_Noeud* ajout = malloc(sizeof(t_Noeud));
    t_Noeud* pnt = index->racine;
    ajout = noeud;
    int verif = 2;
    if(index->racine == NULL)
    {
        index->racine = ajout;
        return 1;
    }
    else if (pnt!= NULL)
    {
        while (verif != 1)
        {
            if (egal(ajout->mot, pnt->mot) == 1)
            {
                return 0;
            }
            else
            {
                if(compare(ajout->mot, pnt->mot) == 1)
                {
                    if (pnt->filsGauche == NULL)
                    {
                        pnt->filsGauche = ajout;
                        verif = 1;
                    }
                    else if (pnt->filsGauche != NULL)
                    {
                        pnt = pnt->filsGauche;
                        verif = 2;
                    }
                }
                else
                {
                    if (pnt->filsDroit == NULL)
                    {
                        pnt->filsDroit = ajout;
                        verif = 1;
                    }
                    else if (pnt->filsDroit != NULL)
                    {
                        pnt = pnt->filsDroit;
                        verif = 2;
                    }
                }
            }
        }
        if (verif = 1) return 1;
    }

}

t_Phrase * creer_phrase(char* phrase, int numPhrase){
    t_Phrase * newPhrase = malloc(sizeof(t_Phrase));
    newPhrase->Phrase = malloc(sizeof(char));
    strcpy(newPhrase->Phrase,phrase);
    newPhrase->id_phrase = numPhrase;
    newPhrase->suivant = NULL;
    return newPhrase;
}

void strip(char *ligne) {
    char *ligne2 = ligne;
    while(*ligne != '\0') {
        if(*ligne != '\t' && *ligne != '\n') {
            *ligne2++ = *ligne++;
        } else {
            ++ligne;
        }
    }
    *ligne2 = '\0';
}

t_Phrase* compter_phrases (t_Index* index, char* filename)
{
  FILE * file = fopen (filename, "r+");
  if (file == NULL)
  {
    printf("fichier vide \n");
    return NULL;
  }
  else
  {
    char *mot; char ligne [1000]; char *phrase = malloc(sizeof(char*));
    int end = 0; int compteur = 1;
    t_Phrase* listeP = malloc(sizeof(t_Phrase));
    listeP->id_phrase = 0;
    while (!feof(file))
    {
      fgets(ligne, 1000, file);
      strip(ligne);
      mot = strtok(ligne, " ");
      while (mot != NULL)
      {
          if (listeP == NULL)
          {
              phrase = strcpy(phrase, mot);
          }
          else
          {
              phrase = strcat(phrase, " ");
              phrase = strcat(phrase, mot);
          }
          for (int i = 0; i<strlen(mot); i++)
          {
            if (mot[i] == '.') end = 1;
          }
          if (end == 1)
          {
            if (listeP->id_phrase == 0)
            {
                listeP = creer_phrase(phrase, compteur);
                index->debut_phrase = listeP;

            }
            else
            {
                listeP->suivant = creer_phrase(phrase, compteur);
                listeP = listeP->suivant;
            }
            phrase = strcpy(phrase, "");
            end = 0;
            compteur ++;
          }
          mot = strtok(NULL, " ");
      }
    }
    return index->debut_phrase;
  }
}

int indexer_fichier (t_Index* index, char* filename)
{
    FILE* file = fopen(filename, "r+");
    t_Noeud* node = malloc(sizeof(t_Noeud));
    t_listePositions* liste = malloc(sizeof(t_listePositions));
    if (file == NULL)
    {
        printf("Fichier vide \n");
        return 0;
    }
    else
    {
        char *mot; char phrase [10000]; const char* separateurs = " ";
        int num_ligne = 0; int ordre = 1; int num_phrase = 1; int compteur = 0; int fin_phrase = 0;
        while(!feof(file))
        {
            fgets(phrase,10000, file);
            num_ligne ++;
            mot= strtok(phrase, separateurs);
            while (mot != NULL)
            {
                for (int i=0; i<strlen(mot); i++)
                {
                    if (mot[i] == '.')
                    {
                        mot[i] = '\0';
                        fin_phrase = 1;
                    }
                }
                t_Noeud* ptr = rechercher_mot(index,mot);
                if (ptr == NULL)
                {
                    t_listePositions* liste = creer_liste_positions();
                    ajouter_position(liste, num_ligne, ordre, num_phrase);
                    t_Noeud* ajout = malloc(sizeof(t_Noeud));
                    ajout->filsDroit = NULL;
                    ajout->filsGauche = NULL;
                    ajout->mot = malloc(sizeof(char));
                    strcpy(ajout->mot, mot);
                    ajout->nb_occurences = 1;
                    ajout->positions = liste;
                    ajouter_noeud(index,ajout);
                    index->nb_mots_differents ++;
                }
                else
                {
                    ajouter_position(ptr->positions, num_ligne, ordre, num_phrase);
                    ptr->nb_occurences ++;
                }
                mot = strtok(NULL, separateurs);
                index->nb_mots_total ++;
                if (fin_phrase == 1)
                    {
                        ordre = 0;
                        num_phrase ++;
                    }
                fin_phrase = 0;
                ordre ++;
            }
        }
    }
    return 1;
}

void afficher_occurences_mot(t_Index *index, char *mot, t_Phrase* liste_phrases)
{
  t_Noeud* node = rechercher_mot(index, mot);
  if (node == NULL)
  {
    printf("Il n'y a aucun mot correspondant \n");
  }
  else
  {
    printf(" Mot = '%s' \n", mot);
    printf("Nombre d'occurrences : %d\n", node->nb_occurences);
    t_Phrase * pnt_phrase = liste_phrases;
    for (int i = 0; i<node->nb_occurences; i++)
    {
      printf("ok\n");
      while (pnt_phrase->id_phrase != node->positions->debut->numero_phrase)
      {
        pnt_phrase = pnt_phrase->suivant;
      }
      printf("| Ligne %d Mot %d : %s\n", node->positions->debut->numero_ligne, node->positions->debut->ordre, pnt_phrase->Phrase);
      node->positions->debut = node->positions->debut->suivant;
      pnt_phrase = liste_phrases;
    }
  }
}

void Affichage(t_Noeud* pnt){
    int nb_element = pnt->nb_occurences;
    t_Position* element =  pnt->positions->debut;
    int i=0;
    char* premiereLettre = pnt->mot;
    printf("\n%c\n|\n", premiereLettre[0]);
    printf("|--%s\n", pnt->mot);
    for (i==0; i<nb_element; i++){
        printf("|----(l:%d, o:%d, p:%d)\n", element->numero_ligne, element->ordre, element->numero_phrase);
        element = element ->suivant;
    }
}

void Parcours_InFixe(t_Noeud* node){
    if (node == NULL) return;
    Parcours_InFixe(node->filsDroit);
    Affichage(node);
    Parcours_InFixe(node->filsGauche);
}

void afficher_index(t_Index *index){
    Parcours_InFixe(index->racine);
}
t_Noeud** creer_tableau_vide(t_Index* index){// cr�er tableau vide d'une taille n = nombre de mots dans l'index
    t_Noeud** tabNoeud=malloc(index->nb_mots_differents*sizeof(t_Noeud*));
    if(!tabNoeud)return NULL;
    int nbr= index->nb_mots_differents;
    int i=0;
    for(i; i<nbr; i++){
        t_Noeud* Noeud=malloc(sizeof(t_Noeud));
    }
    return tabNoeud;
}
int rang = 0;

void ajouter_infixe_noeud(t_Noeud* noeud, t_Noeud** tabNoeuds){// Ajout des noeuds de mani�re tri�es
    if(noeud!=NULL){
        ajouter_infixe_noeud(noeud->filsGauche, tabNoeuds);
        tabNoeuds[rang]=noeud;
        rang++;
        ajouter_infixe_noeud(noeud->filsDroit, tabNoeuds);
    }
}

t_Noeud** tableau_tri(t_Index* index){ // Combinaison des deux fonctions pour nous renvoyer notre tableau tri�
    int i=0;
    t_Noeud* noeud=index->racine;
    t_Noeud** tableau=creer_tableau_vide(index);
    ajouter_infixe_noeud(noeud, tableau);
    return tableau;
}

t_Noeud * creer_arbre_eq (t_Noeud** tab, int start, int end)
{
      if (start > end) return;
      t_Index* index_eq = malloc(sizeof(t_Index));
      t_Noeud* current = malloc(sizeof(t_Index));
      int mid = (start + end) /2;
      current = tab [mid];
      current->filsDroit = creer_arbre_eq (tab, start, mid-1);
      current->filsGauche = creer_arbre_eq(tab, mid, start +1);
      return current;
}

t_Index * equilibrer_index (t_Index * index)
{
  int taille_tab = index->nb_mots_differents;
  t_Noeud **newTab = tableau_tri(index);
  t_Index *newIndex = malloc(sizeof(t_Index));
  index->racine = creer_arbre_eq(newTab, 0, taille_tab-1);
  printf("Fichier bien equilibre\n");
  return newIndex;
}

void menuPrincipal(t_Index * index)
{
    printf("__________________________________________________________________________________\n");
    printf("||      Bienvenue au menu principal d'une application d'indexation de texte     ||\n");
    printf("__________________________________________________________________________________\n");
    printf("||  1. Creer un index a partir d'un fichier.                                    ||\n");
    printf("||  2. Caracteristiques de l'index.                                             ||\n");
    printf("||  3. Afficher l'index.                                                        ||\n");
    printf("||  4. Rechercher un mot dans l'index.                                          ||\n");
    printf("||  5. Afficher les occurrences d'un mot.                                       ||\n");
    printf("||  6. Equilibre l'index.                                                       ||\n");
    printf("||  7. Quitter.                                                                 ||\n");
    printf("__________________________________________________________________________________\n");

    int choix, reponse = 2;
    char filename[256],mot_a_rechercher[50];
    t_Noeud * current;
    t_Phrase* liste_phrase;
    scanf("%d", &choix);
    switch (choix)
{
case 1 :
    while(reponse != 0) {
                if(reponse != 1){
                    printf("Veuillez repondre OUI : 1 ou NON : 0 ");
                    scanf("%d",&reponse);
                }
                else{
                    printf("Veuillez saisir le nom du fichier d'une instance\n");
                    scanf("%s",filename);
                    if (fopen(filename, "r+") == NULL) {
                        printf("Le fichier %s n'existe pas! Continuer? O = 1 / N = 0 ",filename);
                        scanf("%d",&reponse);
                    }else
                    {
                        indexer_fichier(index,filename);
                        index->debut_phrase = compter_phrases(index, filename);
                        reponse = 0;
                    }
                }
            }
    reponse = 1;
    menuPrincipal(index);
    break;

case 2 :
    printf("Cet index contient %d mots.\n",index->nb_mots_total);
    printf("Cet index contient %d mots differents.\n",index->nb_mots_differents);
    menuPrincipal(index);
    break;

case 3 :
    if (index->racine == NULL)
    {
        printf("L'index est vide pour le moment!\n");
    }
    else
    {
        afficher_index(index);
    }

    menuPrincipal(index);
    break;

case 4 :
    printf("Veuillez saisir le mot a rechercher : \n");
    scanf("%s",&mot_a_rechercher);
    printf("%s\n",mot_a_rechercher);
    current = rechercher_mot(index,mot_a_rechercher);
    if(current == NULL) printf("Le mot '%s' n'existe pas.\n",mot_a_rechercher);
    else
    {
        printf("Le mot '%s' existe bien!\n",mot_a_rechercher);
        while (current->positions->debut != NULL)
        {
            int i = 1;
            printf("Position %d - Ligne %d | Ordre %d | Phrase %d \n", i, current->positions->debut->numero_ligne,current->positions->debut->ordre,current->positions->debut->numero_phrase);
            current->positions->debut = current->positions->debut->suivant;
        }
    }
    menuPrincipal(index);
    break;

case 5 :
    printf("Quel mot voulez vous afficher ? \n");
    scanf("%s",&mot_a_rechercher);
    afficher_occurences_mot(index, mot_a_rechercher, index->debut_phrase);
    menuPrincipal(index);
    break;

case 6 :;
    t_Index *new_index;
    new_index = creer_index();
    new_index = equilibrer_index(index);
    menuPrincipal(index);
    break;

case 7 :
    break;
default:
            printf("Ce n'est pas un choix disponible, veuillez choisir une autre option :\n");
            menuPrincipal(index);
            break;
    }
}
