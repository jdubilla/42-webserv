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


## Configuration du Serveur

### Exemple

```
server
{
	listen					127.42.42.42:8042;
	root					www/site1;
	http_methods			GET POST DELETE HEAD;
	cgi						php /usr/bin/php-cgi;
	error_page				404 www/site1/404.html;
	client_max_body_size	0;
	autoindex				on;
	upload					/www/site1;

	location /site2 
	{
		root	www/site2;
		cgi 	py	/usr/bin/python;
		index	index.html;
	}

	location /redir {
		return	https://google.com;
	}
}
```

La configuration du serveur commence avec la directive `server`. Vous pouvez avoir plusieurs blocs `server` si vous souhaitez configurer plusieurs serveurs virtuels sur la même instance de serveur web.

### `listen`

La directive `listen` spécifie l'adresse IP et le port sur lesquels le serveur écoutera les connexions entrantes. Dans  l'exemple, le serveur écoute sur l'adresse IP `127.42.42.42` et le port `8042`.

```markdown
server
{
    listen 127.42.42.42:8042;
    // ...
}
```

### `root`

La directive root définit le répertoire racine (document root) à partir duquel le serveur web servira les fichiers demandés. Tous les fichiers seront recherchés à partir de ce répertoire. Dans l'exemple, le répertoire racine est www/site1.

`root www/site1;`

### `http_methods`

La directive http_methods spécifie les méthodes HTTP autorisées pour ce serveur. Dans votre exemple, les méthodes autorisées sont GET, POST et DELETE.

`http_methods GET POST DELETE;`

### `cgi`

La directive cgi indique comment les fichiers CGI (Common Gateway Interface) doivent être exécutés. Dans l'exemple, les fichiers PHP seront exécutés à l'aide de /usr/bin/php-cgi. De plus, les fichiers Python dans le répertoire /site2 seront exécutés à l'aide de /usr/bin/python.

`cgi php /usr/bin/php-cgi;`

### `error_page`

La directive error_page spécifie comment le serveur doit gérer les erreurs HTTP. Dans l'exemple, en cas d'erreur 404, le serveur renverra la page d'erreur située dans www/site1/404.html.


`error_page 404 www/site1/404.html;`

### `client_max_body_size`

La directive client_max_body_size limite la taille des données que les clients peuvent envoyer au serveur. Dans l'exemple, la limite est définie à 0, ce qui signifie qu'il n'y a pas de limite de taille.


`client_max_body_size 0;`

### `autoindex`

La directive autoindex contrôle si le serveur doit générer une liste automatique des fichiers dans un répertoire si aucun fichier index n'est trouvé. Dans l'exemple, l'option est activée (on).

`autoindex on;`

### `upload`

La directive upload spécifie le répertoire où les fichiers téléchargés par les clients seront stockés. Dans l'exemple, les fichiers téléchargés seront stockés dans /www/site1.

`upload /www/site1;`

### `location`

La directive location est utilisée pour définir des règles de routage spécifiques pour des chemins d'URL particuliers. Dans  l'exemple, deux blocs location sont définis : un pour le chemin /site2 et un autre pour le chemin /redir.

Le bloc location /site2 spécifie que les fichiers du chemin /site2 seront servis à partir du répertoire www/site2, les fichiers Python seront exécutés avec Python, et l'index par défaut est index.html.

```
location /site2 
{
    root    www/site2;
    cgi     py  /usr/bin/python;
    index   index.html;
}
```

Le bloc location /redir spécifie que toutes les requêtes vers /redir seront redirigées vers https://google.com.

```
location /redir {
    return  https://google.com;
}
```
Ces directives permettent de personnaliser le comportement du serveur web en fonction des chemins d'URL spécifiques et des types de fichiers à gérer.
