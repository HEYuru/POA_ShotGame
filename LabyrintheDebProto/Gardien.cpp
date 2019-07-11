#include <iostream>
#include <vector>
#include <time.h>
#include "Gardien.h"
#define _USE_MATH_DEFINES
#define random( x ) (rand() % x)


/*
 *	Constructeur sound
 */
Sound	* Gardien::_guard_fire	= new Sound( "sons/guard_fire.wav" );
Sound	* Gardien::_guard_hit	= new Sound( "sons/guard_hit.wav" );
Sound	* Gardien::_guard_die	= new Sound( "sons/guard_die.wav" );
Sound	* Gardien::_wall_hit	= new Sound( "sons/hit_wall.wav" );


/*
 *	Tente un deplacement.
 */

bool Gardien::move_aux( double dx, double dy )
{
	if ( EMPTY == _l->data( (int) ( (_x + dx * Environnement::scale) / Environnement::scale),
				            (int) ( (_y + dy * Environnement::scale) / Environnement::scale) ) )
	{
		_x	+= dx;
		_y	+= dy;
		return(true);
	}
	return(false);
}


Gardien::Gardien( Labyrinthe* l, const char* modele ) : Mover( 120, 80, l, modele )
{
	ll		= l;                   // labrinthe
	_dx		= Random();         // le prochaine etape
	_dy		= Random();         // le prochaine etape
	gf		= false;               // si le gardien en train de fire
	g_life	= 2;                   // la vie de gardien
	mode		= 1;               // mode initialiser a 1 mode guard
	s		    = 0;               // compter les etapes  
	_min_tresor	= 10000;             // le distance entre le gardien et le tresor
	_min_player	= 10000;             // le distance entre le gardien et le chausser
}

 
// Calculer le distance entre le gardien et le chausser, retourne le vecteur	
std::vector<int> Gardien::Distanceplayer( void )
{
	int	** _dist_player = ( (Labyrinthe *) _l)->get_dist_player();
	int	side[3][3]	= { 0 };
	int	x		= (int) (_x / Environnement::scale);
	int	y		= (int) (_y / Environnement::scale);

	side[0][0]	= _dist_player[x - 1][y - 1];
	side[0][1]	= _dist_player[x - 1][y];
	side[0][2]	= _dist_player[x - 1][y + 1];
	side[1][0]	= _dist_player[x][y - 1];
	side[1][1]	= 10000;  
	side[1][2]	= _dist_player[x][y + 1];
	side[2][0]	= _dist_player[x + 1][y - 1];
	side[2][1]	= _dist_player[x + 1][y];
	side[2][2]	= _dist_player[x + 1][y + 1];
   
    // garde les indices de le min de la table
	std::vector<int>	v;  
	int			min_player = 1000;
	for ( size_t i = 0; i < 3; i++ )
	{
		for ( size_t j = 0; j < 3; j++ )
		{
			if ( side[i][j] < min_player )
			{
				v.clear();
				min_player = side[i][j];
				v.push_back( i );
				v.push_back( j );
				continue;
			}
			if ( side[i][j] == min_player )
			{
				v.push_back( i );
				v.push_back( j );
				continue;
			}
		}
	}
	_min_player = min_player;
	return(v);
}


  
// Calculer le distance entre le gardien et le tresor, retourner le vecteur 
std::vector<int> Gardien::Distancetresor( void )
{
	int	** _dist	= ( (Labyrinthe *) _l)->get_dist();
	int	side[3][3]	= { 0 };
	int	x		= (int) (_x / Environnement::scale);
	int	y		= (int) (_y / Environnement::scale);

	side[0][0]	= _dist[x - 1][y - 1];
	side[0][1]	= _dist[x - 1][y];
	side[0][2]	= _dist[x - 1][y + 1];
	side[1][0]	= _dist[x][y - 1];
	side[1][1]	= 10000; 
	side[1][2]	= _dist[x][y + 1];
	side[2][0]	= _dist[x + 1][y - 1];
	side[2][1]	= _dist[x + 1][y];
	side[2][2]	= _dist[x + 1][y + 1];

	std::vector<int>	v;  
	int			min = 1000;
	for ( size_t i = 0; i < 3; i++ )
	{
		for ( size_t j = 0; j < 3; j++ )
		{
			if ( side[i][j] < min )
			{
				v.clear();
				min = side[i][j];
				v.push_back( i );
				v.push_back( j );
				continue;
			}
			if ( side[i][j] == min )
			{
				v.push_back( i );
				v.push_back( j );
				continue;
			}
		}
	}
	_min_tresor = min;
	return(v);
}

// les conditions pour changer les mode 
void Gardien::check( void )
{
	if ( _min_player < 25 )
		mode = 2; // mode 2 est attack mode
	else{
		if ( _min_tresor < 50 && _min_tresor > 5 )
			 mode = 3;  //mode 3 est defense mode
		else mode = 1; // mode 1 est guard mode
	}
}



// mode 1, le gardien se promènent dans la labyrinthe 
void Gardien::guard( void )
{
	double	dx	= _dx;
	double	dy	= _dy;
// l'angle de bouger (move)
	if ( dx > 0 )
	{
		if ( dy = 0 )
		{
			_angle = 90;
		}else{
			_angle = atan( dy / dx ) * 180 / M_PI - 90;
		}
	}else if ( dx < 0 )
	{
		if ( dy = 0 )
		{
			_angle = 270;
		}else
			_angle = atan( dy / dx ) * 180 / M_PI + 90;
	}else{
		if ( dy > 0 )
		{
			_angle = 0;
		}
		if ( dy < 0 )
		{
			_angle = 180;
		}
	}
    // mettre la postion original 0 
	ll->set_data( (int) (_x / Environnement::scale), (int) (_y / Environnement::scale), EMPTY );
	move( dx, dy );
    // mettre la nouveau postion 10 
	ll->set_data( (int) ( (_x) / Environnement::scale), (int) ( (_y) / Environnement::scale), 10 );
    // si s est tres grand, on change le vitesse 
	if ( s++ >= 50 )
	{
		_dx	= Random(); 
		_dy	= Random();
		s = 0;
	}
}



/* 
 * v(vp) est le valeur rendre par le fonction Distanceplayer
 * 2 mode, le gardiens attaque le chasseur lors qu’ils trouvent le chasseur.
 *   
 */
void Gardien::attack( std::vector<int> v )
{
	int	x		= (int) (_x / Environnement::scale);
	int	y		= (int) (_y / Environnement::scale);
	int	cx		= 0;
	int	cy		= 0;
	int	r	= random(9);  // au plus 9
	
    // etape est -1,-1 
	if ( v[r] == 0 && v[r + 1] == 0 )
	{
		cx	= x - 1;
		cy	= y - 1;
	}
    // etape est -1,0 
	if ( v[r] == 0 && v[r + 1] == 1 )
	{
		cx	= x - 1;
		cy	= y;
	}	
	// etape est 0,-1 
	if ( v[r] == 1 && v[r + 1] == 0 )
	{
		cx	= x;
		cy	= y - 1;
	}
	//  etape est -1,1 
	if ( v[r] == 0 && v[r + 1] == 2 )
	{
		cx	= x - 1;
		cy	= y + 1;
	}
	
    // etape est 1,-1 
	if ( v[r] == 2 && v[r + 1] == 0 )
	{
		cx	= x + 1;
		cy	= y - 1;
	}
    //  etape est 0,1 
	if ( v[r] == 1 && v[r + 1] == 2 )
	{
		cx	= x;
		cy	= y + 1;
	}

    //  etape est 1,0 
	if ( v[r] == 2 && v[r + 1] == 1 )
	{
		cx	= x + 1;
		cy	= y;
	}
    //etape est 1,1 
	if ( v[r] == 2 && v[r + 1] == 2 )
	{
		cx	= x + 1;
		cy	= y + 1;
	}
	
	// si s est tres grand, on change le vitesse 
	if ( s++ >= 50 )
	{
		_dx	= (cx - x) * 0.1;
		_dy	= (cy - y) * 0.1;
		s	= 0;
	}
    // calcule l'angle de move, le meme methode que avant 
	if ( _dx > 0 )
	{
		if ( _dy = 0 )
		{
			_angle = 270;
		}else{
			_angle = atan( _dy / _dx ) * 180 / M_PI - 90;
		}
	}else if ( _dx < 0 )
	{
		if ( _dy = 0 )
		{
			_angle = 90;
		}else
			_angle = atan( _dy / _dx ) * 180 / M_PI + 90;
	}else{
		if ( _dy > 0 )
		{
			_angle = 0;
		}
		if ( _dy < 0 )
		{
			_angle = 180;
		}
	}
	// mettre la postion original 0 
	ll->set_data( (int) (_x / Environnement::scale), (int) (_y / Environnement::scale), EMPTY );
	move( _dx, _dy );
	// mettre la nouveau original 10 
	ll->set_data( (int) ( (_x) / Environnement::scale), (int) ( (_y) / Environnement::scale), 10 );
}


/* 
 * 3 mode, quand le gardien est proche de la tresor,le defense Mode, v(vt) est le valeur rendre par le fonction Distancetresor 
*/
void Gardien::defense( std::vector<int> v )
{
	int	x		= (int) (_x / Environnement::scale);
	int	y		= (int) (_y / Environnement::scale);
	int	cx		= 0;
	int	cy		= 0;
	int	r	= random(9);  // au plus 9
    
    // etape est -1,-1 
	if ( v[r] == 0 && v[r + 1] == 0 )
	{
		cx	= x - 1;
		cy	= y - 1;
	}
    // etape est -1,0 
	if ( v[r] == 0 && v[r + 1] == 1 )
	{
		cx	= x - 1;
		cy	= y;
	}	
	// etape est 0,-1 
	if ( v[r] == 1 && v[r + 1] == 0 )
	{
		cx	= x;
		cy	= y - 1;
	}
	//  etape est -1,1 
	if ( v[r] == 0 && v[r + 1] == 2 )
	{
		cx	= x - 1;
		cy	= y + 1;
	}
	
    // etape est 1,-1 
	if ( v[r] == 2 && v[r + 1] == 0 )
	{
		cx	= x + 1;
		cy	= y - 1;
	}
    //  etape est 0,1 
	if ( v[r] == 1 && v[r + 1] == 2 )
	{
		cx	= x;
		cy	= y + 1;
	}

    //  etape est 1,0 
	if ( v[r] == 2 && v[r + 1] == 1 )
	{
		cx	= x + 1;
		cy	= y;
	}
    //etape est 1,1 
	if ( v[r] == 2 && v[r + 1] == 2 )
	{
		cx	= x + 1;
		cy	= y + 1;
	}
	
	// si s est tres grand, on change le vitesse 
	if ( s++ >=  50 )
	{
		_dx	= (cx - x) * 0.1;
		_dy	= (cy - y) * 0.1;
		s	= 0;
	}
	
	if ( _dx > 0 )
	{
		if ( _dy = 0 )
		{
			_angle = 270;
		}else{
			_angle = atan( _dy / _dx ) * 180 / M_PI - 90;
		}
	}else if ( _dx < 0 )
	{
		if ( _dy = 0 )
		{
			_angle = 90;
		}else
			_angle = atan( _dy / _dx ) * 180 / M_PI + 90;
	}else{
		if ( _dy > 0 )
		{
			_angle = 0;
		}
		if ( _dy < 0 )
		{
			_angle = 180;
		}
	}
        
	ll->set_data( (int) (_x / Environnement::scale), (int) (_y / Environnement::scale), EMPTY );
	move( _dx, _dy );
	ll->set_data( (int) ( (_x) / Environnement::scale), (int) ( (_y) / Environnement::scale), 10 );
}


//renew les actions de les gardiens 	
void Gardien::update( void )
{
	ll->redictplayer();
	std::vector<int>	vt	= Distancetresor();          // entre le tresor
	std::vector<int>	vp	= Distanceplayer();          // entre le chausser
	check();
        
	if ( mode == 1 )
	{
		guard();
	} else if ( mode == 3 )
	{
		defense( vt );
	}
        else
	{
		if ( gf == false )
		{

			float	x	= (_x - ll->_guards[0]->_x ) / Environnement::scale;
			float	y	= (_y - ll->_guards[0]->_y ) / Environnement::scale;
			float	dist2	= x * x + y * y;  // distance entre chausser et le gardien
			// si distance est moins de 5 
			if( dist2 < 5)
			{	int t = (int) time( 0 );  // un random
				if ( tf != t )
				{
					fire( 0 );  
					tf = t;                         
				}	
			}
			if ( _min_player > 1 )
			{
					attack( vp );
			}
			
		}
	}
}

// quand a faire bouger la boule de feu... 
bool Gardien::process_fireball( float dx, float dy )
{
	float	x	= (_x - _fb->get_x() ) / Environnement::scale;
	float	y	= (_y - _fb->get_y() ) / Environnement::scale;
	float	dist2	= x * x + y * y;
	int	cx	= (int) ( (_fb->get_x() + dx * Environnement::scale) / Environnement::scale);
	int	cy	= (int) ( (_fb->get_y() + dy * Environnement::scale) / Environnement::scale);
	/* on bouge que dans le vide! */
	if ( EMPTY == _l->data( cx, cy ) )
	{
		/* le scale de toucher */
		if ( abs( (int) (ll->_guards[0]->_x / Environnement::scale) - cx ) < 5 && abs( (int) (ll->_guards[0]->_y / Environnement::scale) - cy ) < 5 )
		{
			ll->_guards[0]->g_life--;  // 3 vie en total
			if ( ll->_guards[0]->g_life <= 0 )
			{
				partie_terminee( false );  // perdre
			}
		}
		return(true);
	}


	/*
	 * collision...
	 * calculer la distance maximum en ligne droite.
	 */
	float dmax2 = (_l->width() ) * (_l->width() ) + (_l->height() ) * (_l->height() );
	/* faire exploser la boule de feu avec un bruit fonction de la distance. */
	_wall_hit->play( 1. - dist2 / dmax2 );
	gf = false;
	return(false);
}

// obtenir le valeur entre -0.1~0.1, pour defenir le vitesse
double Gardien::Random( void )  
{
	return (rand()/double(RAND_MAX)*0.2-0.1);
}

/*
 *	Tire sur un ennemi.
 */
void Gardien::fire( int angle_vertical )
{
	message( "Woooshh..." );
	_fb->init( _x, _y, 10., angle_vertical, -(_angle) );
	gf = true;
	_guard_fire->play();
}



