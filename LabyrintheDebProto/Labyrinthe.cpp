#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <time.h>
#include "Labyrinthe.h"
#include "Chasseur.h"
#include "Gardien.h"

#define random( x ) (rand() % x)
using namespace std;

Sound	*Chasseur::_hunter_fire;        // bruit de l'arme du chasseur. 
Sound	*Chasseur::_hunter_hit;         // cri du chasseur touche. 
Sound	*Chasseur::_wall_hit;           // on a tape un mur. 

Environnement *Environnement::init( char *filename )
{
	return(new Labyrinthe( filename ) );
}

static Wall	walls[1000];  
static Wall	picts[1000]; 
static Box	boxes[1000]; 

   
// mettre les caisses et retourner le nombre de caisses
int Caisses( std::vector<std::vector<char> > data )
{
	// nombre de caisses 
	int num = 0;

	for ( size_t i = 0; i < data.size(); i++ )
	{
		for ( size_t j = 0; j < data[i].size(); j++ )
		{

			if ( data[i][j] == 'X' )
			{
				boxes[num]._x		= j;
				boxes[num]._y		= i;
				boxes[num]._ntex	= 0;
				num++;
			}
		}
	}
	return(num);
}


//mettre le tresor, retourne la position de le tresor
std::vector<int> Tresor( std::vector<std::vector<char> > data )
{
	// mettre des coordonnées de trésor dans v 
	std::vector<int> v;
	
	for ( size_t i = 0; i < data.size(); i++ )
	{
		for ( size_t j = 0; j < data[i].size(); j++ )
		{
			
			if ( data[i][j] == 'T' )
			{
				
				v.push_back( j );
				v.push_back( i );
			}
		}
	}

	return(v);
}


/*
 * max_width signifie la largeur de le labyrinthe, et max_height signifie le longeur de le labyrinthe 
 * retourner le nombre de walls
 */
 
int Walls( std::vector<std::vector<char> > data, size_t max_width, size_t max_height )
{
	
		int	num	= 0;   // le nombre de les murs
		int	counter = 0;   // la longeur de le mur

	for ( size_t i = 0; i < data.size(); i++ )
	{
		for ( size_t j = 0; j < data[i].size(); j++ )
		{
			// commencer à partir de premier caractère "+"  
			if ( data[i][j] == '+' && counter == 0 )
			{
				counter++;
				walls[num]._x1	= j;
				walls[num]._y1	= i;
				walls[num]._ntex	= 0;
				continue;
			}

			// la texture de la mur 
			if ( (data[i][j] == 'a' || data[i][j] == 'b' || data[i][j] == '-') && counter != 0 )
			{
				counter++;
				continue;
			}

			// si on rencontre le dernier element de le ligne, réinitialisre "counter" 
			if ( (j == data[i].size() - 1) && counter != 0 )
			{
				counter++;
				walls[num]._x2	= j;
				walls[num]._y2	= i;
				num++;
				counter = 0;
				continue;
			}

			// continuer 
			if ( (data[i][j + 1] == 'a' || data[i][j + 1] == '+' || data[i][j + 1] == 'b' || data[i][j + 1] == '-') && counter != 0 )
			{
				counter++;
				continue;
			}

			// si ce n'est pas le même mur,renew number et counter 
			if ( !(data[i][j + 1] == 'a' || data[i][j + 1] == 'b' || data[i][j + 1] == '-') && counter != 0 )
			{
				counter++;
				walls[num]._x2	= j;
				walls[num]._y2	= i;
				num++;
				counter = 0;
			}
		}
	}


	// lire verticalement 
	for ( size_t j = 0; j < max_width; j++ )
	{
		for ( size_t i = 0; i < max_height; i++ )
		{
			// commencer à compter à partir de premier caractère "+" 
			if ( data[i][j] == '+' && counter == 0 )
			{
				counter++;
				walls[num]._x1	= j;
				walls[num]._y1	= i;
				walls[num]._ntex	= 0;
				continue;
			}

			// la texture de le mur 
			if ( (data[i][j] == '|' || data[i][j] == 'a' || data[i][j] == 'b') && counter != 0 )
			{
				counter++;
				continue;
			}

			// le dernier element 
			if ( (i == max_height - 1) && counter == 0 )
			{
				continue;
			}

			// si on rencontre le dernier element de le ligne, réinitialisre "counter" 
			if ( (i == max_height - 1) && counter != 0 )
			{
				counter++;
				walls[num]._x2	= j;
				walls[num]._y2	= i;
				num++;
				counter = 0;
				continue;
			}

			
			// continuer à lire 
			if ( (data[i + 1][j] == '|' || data[i + 1][j] == 'a' || data[i + 1][j] == 'b' || data[i + 1][j] == '+') && counter != 0 )
			{
				counter++;
				continue;
			}

			// si ce n'est pas le même mur, màj number et counter 
			if ( !(data[i + 1][j] == '|' || data[i + 1][j] == 'a' || data[i + 1][j] == 'b' || data[i + 1][j] == '+') && counter != 0 )
			{
				counter++;
				walls[num]._x2	= j;
				walls[num]._y2	= i;
				num++;
				counter = 0;
			}
		}
	}

	
	return(num);
}

/*
   mettre des textures dans les walls, data reprensent le donnee nous avons lu, 
   b signifie le numero de le labyrinthe,retourner le nombre de texture  
*/

int makewalls( std::vector<std::vector<char> > data, int b )
{
	// le nombre de texture 
	int num = 0;

	for ( size_t i = 0; i < data.size(); i++ )
	{
		for ( size_t j = 0; j < data[i].size(); j++ )
		{

                       
			if ( data[i][j] == 'a' && (data[i][j + 1] == '-' || data[i][j + 1] == '+' || data[i][j + 1] == 'a' || data[i][j + 1] == 'b') )
			{
				picts[num]._x1	= j;
				picts[num]._y1	= i;
				picts[num]._x2	= j + 2;	// longeur de texture est 2 
				picts[num]._y2	= i;
				picts[num]._ntex	= 0;
				num++;
			}

			
			if ( data[i][j] == 'b' && (data[i][j + 1] == '-' || data[i][j + 1] == '+' || data[i][j + 1] == 'a' || data[i][j + 1] == 'b') )
			{
				picts[num]._x1	= j;
				picts[num]._y1	= i;
				picts[num]._x2	= j + 2;
				picts[num]._y2	= i;
				picts[num]._ntex= b;
				num++;
			}

			/* lire verticalement */
			if ( data[i][j] == 'a' && (data[i + 1][j] == '|' || data[i + 1][j] == '+' || data[i + 1][j] == 'a' || data[i + 1][j] == 'b') )
			{
				picts[num]._x1	= j;
				picts[num]._y1	= i;
				picts[num]._x2	= j;
				picts[num]._y2	= i + 2;
				picts[num]._ntex	= 0;
				num++;
			}

			if ( data[i][j] == 'b' && (data[i + 1][j] == '|' || data[i + 1][j] == '+' || data[i + 1][j] == 'a' || data[i + 1][j] == 'b') )
			{
				picts[num]._x1	= j;
				picts[num]._y1	= i;
				picts[num]._x2	= j;
				picts[num]._y2	= i + 2;
				picts[num]._ntex	= b;
				num++;
			}
		}
	}
	
	return(num);
}


//obtenir le nombre de gardien, retourne le nombre
int numGardiens( std::vector<std::vector<char> > data )
{

	int num = 0;
	for ( size_t i = 0; i < data.size(); i++ )
	{
		for ( size_t j = 0; j < data[i].size(); j++ )
		{

			if ( data[i][j] == 'G' )
			{
				num++;
			}
		}
	}

	return(num);
}



/* 
 * construire des gardiens, data represente des données de le labyrinthe, l reprensente le labyrinthe dans lequel il évolue, 
 * _guards represent le tableau de pointeurs de gardiens, scale represente l'échelle, il n'y a pas de valeur a retourner
*/

void Gardiens( std::vector<std::vector<char> > data, Labyrinthe* l, Mover** _guards, const int scale )
{
	srand( (int) time( 0 ) );
	std::vector<char *> names;
	names.push_back( (char *) "drfreak" );
	names.push_back( (char *) "Potator" );
	names.push_back( (char *) "garde" );
	names.push_back( (char *) "Marvin" );

	// indice de chasseur = 0, et indice de gardiens est > 0 
	int num = 1;
	for ( size_t i = 0; i < data.size(); i++ )
	{
		for ( size_t j = 0; j < data[i].size(); j++ )
		{
			// init le chasseur 
			if ( data[i][j] == 'C' )
			{
				_guards[0]	= new Chasseur( l );
				_guards[0]->_x	= j * scale;
				_guards[0]->_y	= i * scale;
			}
			// init gardiens 
			if ( data[i][j] == 'G' )
			{
				// quand nous avons lu le "G", nous construisons un gardien 
		
				_guards[num]		= new Gardien( l, names[random( 4 )] );
				// maj des coordonnée dans la carte 
				_guards[num]->_x	= j * scale;
				_guards[num]->_y	= i * scale;
				num++;
			}
		}
	}
}

/* on defini un class "Point" pour decrire la position dans le labyrinthe, et on utilise l'algorithm
 *	BFS pour calculer les distances entre les points  
 */
class Point
{
	
public:
	int x, y;
	Point(int xx = 0, int yy = 0) :x(xx), y(yy) {};
	int getX();
	int getY();

};
int Point::getX()
{
	return x;
}

int Point::getY()
{
	return y;
}
// la structure noeud 
struct Node
{
	Point node;
	Node * next;
	
};

// la structure Quene

class Quene
{
private:
	Node * before;
	Node * later;
public:
	Quene();
	~Quene();
	bool isEmpty();
	void putIn(Point);
	Point getOut();
};

Quene::Quene()
{
	before = NULL;
	later = NULL;
}

Quene::~Quene()
{
	delete before;
}

void Quene::putIn(Point p)
{
	Node *n = new Node();
	n->node = p;
	n->next = NULL;
	if (before==NULL)
	{
		before = n;
		
	}
	else
		later->next = n;
	later = n;
}

Point Quene::getOut()
{
	Node *n = new Node();
	Point p;
	if (before == NULL)
	{
		cout<<"Quene  is  empty" <<endl;
	}
	else
	{
		n = before;
		p = n->node;
		before = before->next;
		delete n;
	}
	return p;
}

bool Quene::isEmpty()
{
	if (before == NULL)
	{
		return 1;
	}
	else 
		return 0;
}

// Calculate the distance, BFS 
void pcc(Point t, int **dist)
{
	Quene q;
	q.putIn(t); 
	while(!q.isEmpty())
	{
		Point p1 = q.getOut();
		int x = p1.getX();
		int y = p1.getY();
		int distance = dist[x][y];
		for (int i = x-1; i < x+2; i++)
		{
			for (int j = y-1; j < y+2; j++)
			{
				if (dist[i][j] == -1)
				{
				    Point space(i,j);
				    q.putIn(space);
				    dist[i][j] = distance + 1;
				}
			}
		}

	}

}



/*
 * construire un labyrinthe.
 */
Labyrinthe::Labyrinthe( char *filename )
{
	ifstream			fin( filename );	// classe de flux d'entrée pour fonctionner sur les fichiers 
	string				StrLine;		// nous lisons ligne par ligne des datas dans la carte de labyrinthe
	std::vector<std::vector<char> > data;	                // créer un vecteur pour contenir des données dans la carte de labyrinthe 
	bool				begin = false;		// begin = true quand ce programme commence à lire le document 
	
	// lire ce document et mettre dans un vecteur "data" 
	while ( getline( fin, StrLine ) )
	{
		if ( begin == false )
		{
			if ( StrLine[0] == '+' )
				begin = true;
			else {
				
				if ( StrLine[0] == '#' ){
					continue;
				}
				continue;
			};
		}

		/*
		   créer un vecteur pour contenir des caractères dans chaque ligne 
		   nous allons utilise cette variable pour ajouter ligne par ligne dans le vecteur data 
		*/
		
		std::vector<char> v;
		for ( size_t i = 0; i < StrLine.size() - 1; i++ )
		{
			v.push_back( StrLine[i] );
		}
		data.push_back( v );
	}


	size_t	max_width	= 0;  // max_width contient la valeur de largeur max de labyrinthe 
	size_t	max_height	= 0;  // max_height contient la valeur de longeur max de labyrinthe 
	
	for ( size_t i = 0; i < data.size(); i++ )
	{
		for ( size_t j = 0; j < data[i].size(); j++ )
		{
			
			if ( j > max_width )
			{
				max_width = j;
			}
		}
		
		if ( i > max_height )
		{
			max_height = i;
		}
	}

	/* comme le i, j commencent par 0, nous ajoutons 1 */
	max_height++;
	max_width++;
	LAB_WIDTH	= max_width;
	LAB_HEIGHT	= max_height;
	cout<<"width : "<<LAB_WIDTH<<endl;
	cout<<"height : "<<LAB_HEIGHT<<endl;


	_data = new char * [LAB_WIDTH];	
	for ( int i = 0; i < LAB_WIDTH; i++ )
	{
		
		_data[i] = new char[LAB_HEIGHT];
		for ( int j = 0; j < LAB_HEIGHT; j++ )
		{
			
			// nous initialisons tous les _data[i][j] = 0 
			_data[i][j] = EMPTY;
		}
	}

	
	/* 
		initialiser le  tableau _dist
    */
	_dist = new int * [LAB_WIDTH];
	for ( int i = 0; i < LAB_WIDTH; i++ )
	{
		_dist[i] = new int [LAB_HEIGHT];

		// nous initialisons pour chaque _dist[i][j] -1 
		for ( int j = 0; j < LAB_HEIGHT; j++ )
		{
			_dist[i][j] = -1;
		}
	}


        /* 
		   initialiser le  tableau _dist_player
        */
	_dist_player = new int * [LAB_WIDTH];
	for ( int i = 0; i < LAB_WIDTH; i++ )
	{
		
		_dist_player[i] = new int [LAB_HEIGHT];
		for ( int j = 0; j < LAB_HEIGHT; j++ )
		{
			// nous initialisons pour chaque _dist_player[i][j] = 1 
			_dist_player[i][j] = 1;
		}
	}


	/* 
           nous mettons des "espace" dans les lignes qui ont la taille < max_width 
	       pour éviter de dépasser la taille qui sont < max_width, c'est tres important!
    */
	for ( size_t i = 0; i < data.size(); i++ )
	{
		for ( size_t j = 0; j < data[i].size(); j++ )
		{
			while ( data[i].size() < max_width )
			{
				// mettre des "espace" à la fin de ligne data[i] 
				data[i].push_back( (char) 32 );
			}
		}
	}


	
    //construire walls, texture, caisse
	// chercher l'indice de texture de voiture.jpg 
	char tmp [128];
	sprintf( tmp, "%s/%s", texture_dir, "voiture.jpg" );
	int b = wall_texture( tmp );
	
	int nwall = Walls( data, max_width, max_height );
	int npicts = makewalls( data, b );
	int nboxes = Caisses( data );
	
	// mettre des valeurs que nous avons calculé dans les variables déjà définies 
	_nwall	= nwall;
	_walls	= walls;
	_npicts = npicts;
	_picts	= picts;
	_nboxes = nboxes;
	_boxes	= boxes;

    // indiquer qu'on ne marche pas sur un mur 
	for ( int i = 0; i < nwall; i++ )
	{
		
		int	x1	= walls[i]._x1;
		int	x2	= walls[i]._x2;
		int	y1	= walls[i]._y1;
		int	y2	= walls[i]._y2;

		if ( x1 == x2 )
		{
			while ( y1 <= y2 )
			{
				_data[x1][y1] = 1;
				y1++;
			}
		}else{
			while ( x1 <= x2 )
			{
				_data[x1][y1] = 1;
				x1++;
			}
		}
	}

	// indiquer qu'on ne marche pas sur une caisse 
	for ( int i = 0; i < nboxes; i++ )
	{
		_data[boxes[i]._x][boxes[i]._y] = 1;
	}

	// retourner la position de trésor 
	std::vector<int> t = Tresor( data );
	_treasor._x			= t[0];
	_treasor._y			= t[1];
	// on ne marche pas sur le trésor 
	_data[_treasor._x][_treasor._y] = TRESOR;
	cout<<"treasor.x : "<<t[0]<<endl;
	cout<<"treasor.y : "<<t[1]<<endl;
 
    //construire gardiens	
	int nguards = numGardiens( data );

	_nguards	= nguards + 1;  // ajoute 1 pour le chasseur
	_guards		= new Mover *[_nguards];
	Gardiens( data, this, _guards, scale );

	// indiquer qu' on ne marche pas sur les gardiens. 
	for ( int i = 1; i < nguards; i++ )
	{
		_data[(int) (_guards[i]->_x / scale)][(int) (_guards[i]->_y / scale)] = 1;
	}
 
     // màj le  tableau _dist et _dist_player
	for ( size_t i = 0; i < data.size(); i++ )
	{
		for ( size_t j = 0; j < data[i].size(); j++ )
		{
			
			if ( data[i][j] == 'T' )
			{
				_dist[j][i] = 0;
				continue;
			}
	 
		 //pour des walls : + - | a b,nous mettons des distances de wall dans le tableau de _dist et _dist_player
		
			if ( data[i][j] == '+' || data[i][j] == '-' || data[i][j] == '|' || data[i][j] == 'a' || data[i][j] == 'b' )
			{
				_dist[j][i] = 1000; //le plus grand 
				_dist_player[j][i] = 1000;
				continue;
			}
		}
	}
	
// BFS pour calcule les distance 
	Point t1( t[0], t[1] );
	pcc( t1, _dist );
        
}


// regénérer la distance de tableau _dict_player, et on le utilise dans "Gardien.cpp" 
void Labyrinthe::redictplayer(void){

	// nous parcourrons le tableau _dist_player 
	for ( int i = 0; i < LAB_HEIGHT; i++ )
	{
		for ( int j = 0; j < LAB_WIDTH; j++ )
		{
			// sauf les murs, nous mettres tous les distances des autres positions en -1 
			
			if (_dist_player[j][i] == 1000) {
				continue;
			}
			_dist_player[j][i] = -1;
		}
	}
	// la position de le chausser 
	int x = (int)_guards[0]->_x/scale;
	int y = (int)_guards[0]->_y/scale;
	_dist_player[x][y] = 0;
	Point p( x, y );

	// calculer le BFS entre le chasseur et les autres positions 
        pcc( p, _dist_player );
}
