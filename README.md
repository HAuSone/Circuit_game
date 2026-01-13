Voici le code Markdown de votre projet Circuit_game à copier-coller directement :

Markdown

# Circuit_game
Jeu de course de voitures en C++

## Contexte
Dans le cadre d'un cours de **programmation orientée objet en C++**, nous avons réalisé un projet de jeu de voitures intitulé "The Racing".

## Lancement
Pour lancer le jeu, vous devez suivre cet ordre précis :
1. Exécuter le programme `circuit_game_srv`.
2. Exécuter le programme `circuit_game_client`.

> **Attention :** Le programme fonctionne sur le réseau local (**127.0.0.1**). Les deux programmes doivent donc être exécutés sur le même ordinateur.

## Fonctionnement
Une fois les deux instances lancées, vous pouvez sélectionner plusieurs véhicules.

<img width="945" height="382" alt="image" src="https://github.com/user-attachments/assets/36ee431e-876e-419e-9ba9-c365f98aee9a" />

Une fois la sélection terminée, les deux joueurs peuvent s'affronter avec des commandes différentes :

**Touches Joueur 1 :**
* **Z** : Accélérer
* **S** : Ralentir
* **Q** : Tourner à gauche
* **D** : Tourner à droite

**Touches Joueur 2 :**
* **Flèches directionnelles**

<img width="945" height="372" alt="image" src="https://github.com/user-attachments/assets/91868f8e-0acd-48e5-930d-4447798cd00d" />

## Fonctionnalités
Des **stations-service** et des zones de **changement de pneus** apparaissent à gauche ou à droite de la route. Pour changer les pneus ou refaire le plein d'essence, le véhicule doit être **complètement arrêté** sur la station.

## Conditions de fin de partie
La partie s'arrête si l'un des événements suivants survient :
* Un des véhicules sort de la route.
* Un des véhicules tombe à court de carburant.
* Un des véhicules atteint 100 % d'usure des pneus.
* Une collision survient entre les deux véhicules.
* Un des deux véhicules franchit la ligne d'arrivée.
