#include <iostream>
#include <fstream>
#include <string>
#include "ecriture_hit_box.h"

std::vector<int> positionMouse(){
    while(sf::Mouse::isButtonPressed(sf::Mouse::Left)==0){
    }
    std::cout<<sf::Mouse::getPosition().x<<" "<<sf::Mouse::getPosition().y<<"\n";
        return std::vector<int>(sf::Mouse::getPosition().x,sf::Mouse::getPosition().y);
}


void creeHitBox(std::vector<std::vector<int>> *listeHitBox){
    int type;
    std::vector<int> supGauche,infDroit,laHitBox;
    std::cout<<"saisi le type, 1=plateforme, 2=porte, 3=piége:   ";std::cin>>type;
    std::cout<<"clique sur l'angle supérieur gauche\n";supGauche=positionMouse();
    std::cout<<"clique sur l'angle inférieur droit\n";infDroit=positionMouse();
    switch (type){
        case 1:
            {
            laHitBox={infDroit[0]-supGauche[0],infDroit[1]-supGauche[1],supGauche[0],supGauche[1],type};
            break;
            }
        case 2:
            {
            int i,o;
            std::vector<int> reap;
            std::cout<<"saisi l'indice de la carte suivante':   ";std::cin>>i;
            std::cout<<"saisi l'orientation de la carte suivante':   ";std::cin>>o;
            std::cout<<"clique sur le point de réaparition";reap=positionMouse();
            laHitBox={infDroit[0]-supGauche[0],infDroit[1]-supGauche[1],supGauche[0],supGauche[1],type,i,o,reap[0],reap[1]};
            break;
            }
        case 3:
            {
            std::vector<int> app;
            std::cout<<"clique sur le point de réaparition";app=positionMouse();            
            laHitBox={infDroit[0]-supGauche[0],infDroit[1]-supGauche[1],supGauche[0],supGauche[1],type,app[0],app[1]};
            break;
            }
    }
    (*listeHitBox).push_back(laHitBox);
}
//afficher prend un rectangle dans la liste dont on prend l'indice dans le terminal et remplace le 5éme élément du vecteur de la liste par son type + 100
//mettre une boucle while pour laisser le truc type 100 tant que une touche n'a pas été cliqué puis remettre le bon type
void affichRect(std::vector<std::vector<int>> *listeHitBox){
    int i;
    std::cout<<"entre l'indice du rectangle a afficher en blanc:   ";std::cin>>i;
    (*listeHitBox)[i][4]=(*listeHitBox)[i][4]+100;
    std::cout<<"fait un clique gauche pour que tout revienne à la normale\n";
    while(sf::Mouse::isButtonPressed(sf::Mouse::Left)==0){
    }
    (*listeHitBox)[i][4]=(*listeHitBox)[i][4]-100;
}


//fonction suppr qui demande l'indice du truc à supprimé(fonction au dessus sert à trouver l'indice)
void supppr(std::vector<std::vector<int>> *listeHitBox){
    int i;
    std::cout<<"entre l'indice du rectangle a supprimer:   ";std::cin>>i;
    (*listeHitBox).erase((*listeHitBox).begin()+i-1);
}

//modif carré qui avec indice donne les x y l h du carré et te permet de les modifier.
void modifHitBox(std::vector<std::vector<int>> *listeHitBox){
    int i,m;
    std::cout<<"entre l'indice du rectangle a modifier:   ";std::cin>>i;
    std::cout<<"son abcysse est: "<<(*listeHitBox)[i][2]<<"\nrentre ce que tu veut ajouter à l'abcysse:   ";std::cin>>m;
    (*listeHitBox)[i][2]=(*listeHitBox)[i][2]+m;
    std::cout<<"son ordonnée est: "<<(*listeHitBox)[i][3]<<"\nrentre ce que tu veut ajouter à l'ordonnée:   ";std::cin>>m;
    (*listeHitBox)[i][3]=(*listeHitBox)[i][3]+m;
    std::cout<<"sa largeur est: "<<(*listeHitBox)[i][0]<<"\nrentre ce que tu veut ajouter à la largeur:   ";std::cin>>m;
    (*listeHitBox)[i][0]=(*listeHitBox)[i][0]+m;
    std::cout<<"sa hauteur est: "<<(*listeHitBox)[i][1]<<"\nrentre ce que tu veut ajouter à la hauteur:   ";std::cin>>m;
    (*listeHitBox)[i][1]=(*listeHitBox)[i][1]+m;
}

