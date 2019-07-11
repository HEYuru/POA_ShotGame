#include <iostream>
#include <vector>
#include <time.h>
#include "Chasseur.h"


/*
 *	Tente un deplacement.
 *  try to move.
 */

bool Chasseur::move_aux( double dx, double dy )
{
	// Tester si le bon état mobile
	if ( EMPTY == _l->data( (int) ( (_x + dx) / Environnement::scale),
				            (int) ( (_y + dy) / Environnement::scale) ) )
	{
		_x	+= dx;
		_y	+= dy;
		return(true);
	}
	//cherche le tresor ,si toucher le trésor à gagner
	if ( TRESOR == (int) _l->data( (int) ( (_x + dx) / Environnement::scale),
				   (int) ( (_y + dy) / Environnement::scale) ) )
	{

		partie_terminee( true ); 
	}
	
	//si on rencontre les gardiens à perdre
	if ( 10 == (int) _l->data( (int) ( (_x + dx * Environnement::scale) / Environnement::scale),
			   (int) ( (_y + dy * Environnement::scale) / Environnement::scale) ) )
	{
		partie_terminee( false );   
	}
     
     return(false);
	
} 



/*
 *	Constructeur.
 */

Chasseur::Chasseur( Labyrinthe* l ) : Mover( 100, 80, l, 0 )
{
	_hunter_fire	= new Sound( "sons/hunter_fire.wav" );
	_hunter_hit	= new Sound( "sons/hunter_hit.wav" );
	if ( _wall_hit == 0 )
		_wall_hit = new Sound( "sons/hit_wall.wav" );
		
	ll		= l; // labrinthe
	g_life	= 2; // La vie du gardien
	
}


/*
 *	Fait bouger la boule de feu (ceci est une exemple, a vous de traiter les collisions specifiques...)
 */

bool Chasseur::process_fireball( float dx, float dy )
{
	// calculer la distance entre le chasseur et le lieu de l'explosion. 
	float	x	= (_x - _fb->get_x() ) / Environnement::scale;
	float	y	= (_y - _fb->get_y() ) / Environnement::scale;
	int	cx	= (int) ( (_fb->get_x() + dx) / Environnement::scale);
	int	cy	= (int) ( (_fb->get_y() + dy) / Environnement::scale);
	float	dist2	= x * x + y * y;
	
	
	
	// on bouge que dans le vide! 
	if ( EMPTY == _l->data( cx, cy ) )
	{
		message( "Woooshh ..... %d", (int) dist2 );
		// il y a la place. 
		return(true);
	}
    // collision...
	// calculer la distance maximum en ligne droite.
	float dmax2 = (_l->width() ) * (_l->width() ) + (_l->height() ) * (_l->height() );
	// faire exploser la boule de feu avec un bruit fonction de la distance. 
	_wall_hit->play( 1. - dist2 / dmax2 );
	message( "Booom..." );

	// detecter si un gardien est mort
	 bool flag = false; 
	// si on rencontre un ennemi 
	if ( 10 == (int) _l->data( cx, cy ) )
	{	
		//  _nguards est nombre de guard
		for ( int i = 1; i < _l->_nguards; i++ )
		{
			if ( (int) _l->_guards[i]->_x / Environnement::scale == cx && (int) _l->_guards[i]->_y / Environnement::scale == cy )
			{
				// guard perdre un vie. g_life=2
				_l->_guards[i]->g_life--;  
				for ( int j = i; j < _l->_nguards; j++ )
				{
					if ( _l->_guards[j]->g_life <= 0 )
					{
						ll->set_data((int) (_l->_guards[j]->_x / Environnement::scale),(int) ( _l->_guards[j]->_y / Environnement::scale),EMPTY);
						_l->_guards[j] = _l->_guards[j + 1];  // maj les gardien
						_l->_nguards--;  // maj le nombre de gardien
						flag = true;  
						
					}
				}
				if ( flag )
				{
					return(false);
				}
			}
		}
	}
	return(false);
}


/*
 *	Tire sur un ennemi.
 */

void Chasseur::fire( int angle_vertical )
{
	message( "Woooshh..." );
	_hunter_fire->play();
	_fb->init( /* position initiale de la boule */ _x, _y, 10.,
	           /* angles de visee */ angle_vertical, _angle );
}

/*
 *	Clic droit: par défaut fait tomber le premier gardien.
 *
 *	Inutile dans le vrai jeu, mais c'est juste pour montrer
 *	une utilisation des fonctions « tomber » et « rester_au_sol »
 */

void Chasseur::right_click (bool shift, bool control) {
	if (shift)
		_l -> _guards [1] -> rester_au_sol ();
	else
		_l -> _guards [1] -> tomber ();
}


