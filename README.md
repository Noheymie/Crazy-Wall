# Crazy Wall

## Objectif
En binôme, réaliser un jeu consistant à faire passer une main virtuelle à travers un trou dans un mur avançant vers elle. Les trous possibles correspondent à un certain nombre de doigts relevés, et la main virtuelle agit selon la position prise par la main réelle devant la caméra. Le projet doit inclure un retour caméra du joueur et de sa main et une représentation visuelle de la position de main détectée par le système.

## Utilisation
L’interface utilisateur se compose d’une partie _Caméra_ à gauche et d’une partie _Jeu_ à droite. La partie _Caméra_ diffuse des images issues de la webcam de l’ordinateur, et un bouton __Capture__ permet de calculer la pose de la main à un instant donné. Un tracé de la position détectée est réalisé sous l’espace _Caméra_. 

L’interface de jeu située à droite recrée la position de la main de l’utilisateur avec un modèle 3D composé de cubes. Un mur généré de façon similaire se rapproche chaque seconde du modèle de main. Ce mur possède un trou reprenant en négatif une des positions possibles de la main de l’utilisateur, que ce dernier doit alors imiter pour passer le mur. Le modèle de main s’adapte après chaque capture d’image. L’utilisateur peut le modifier en changeant sa main de position et en capturant l’image, et ce jusqu’à ce que le mur se trouve à un dernier espace de la main. Le décompte des points est affiché sous l’interface de jeu, ainsi qu'un "High Score", meilleur score jusqu'ici.

Si l’utilisateur imite la position correspondant au trou dans le mur, une image de victoire s’affiche au-dessus du mur, il gagne un point et le jeu reprend avec un nouveau trou généré aléatoirement. Dans le cas contraire, une image de défaite s’affiche au même endroit et le jeu s'arrête.

## Elements techniques 
C++, Qt, multithreading

## Equipe
Quentin CRETIER, Noémie GERARD

## Annexe : diagramme de classe pour la conception du projet
![Diagramme de classes (3)](https://github.com/user-attachments/assets/2f88b6dd-b4c1-4710-9b86-41af0d8c2749)
