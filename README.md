# 42-webserv

# Webserv

Bienvenue dans le projet Webserv de l'école 42 ! Ce projet consiste à créer un serveur web HTTP du côté serveur en C++. Vous allez apprendre à gérer les requêtes HTTP, à interagir avec les clients et à servir des pages web statiques et dynamiques.

## Table des matières

- [Présentation du projet](#présentation-du-projet)
- [Installation](#installation)
- [Utilisation](#utilisation)
- [Structure du projet](#structure-du-projet)

## Présentation du projet

Le projet Webserv consiste à créer un serveur web capable de gérer des requêtes HTTP. Voici quelques-unes des fonctionnalités clés que vous devez implémenter :

- Gestion des méthodes HTTP GET et POST.
- Gestion des en-têtes HTTP, y compris les en-têtes personnalisés.
- Traitement des requêtes CGI pour les pages web dynamiques.
- Gestion des codes de réponse HTTP (200, 404, etc.).
- Support des fichiers HTML, CSS, JavaScript, etc.
- Configuration du serveur via un fichier de configuration.

## Installation

Pour installer ce projet sur votre machine, suivez ces étapes :

1. Clonez ce dépôt :
```sh
git clone https://github.com/Bubonn/webserv.git
```

2. Accédez au répertoire du projet :
```sh
cd webserv
```

3. Compilez le code source :
```sh
make
```

4. Vous êtes prêt à utiliser le serveur web !


## Utilisation

Pour lancer le serveur web, utilisez la commande suivante :

```sh
./webserv <fichier_de_configuration>
```
