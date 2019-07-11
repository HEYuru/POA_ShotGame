#ifndef GARDIEN_H
#define GARDIEN_H

#include <stdio.h>
#include "Mover.h"
#include "Sound.h"
#include <iostream>
#include <math.h>
#include "Labyrinthe.h"
#include <random>

class Labyrinthe;

class Gardien : public Mover {
private:
    /* accepte ou non un deplacement. */
	bool move_aux( double dx, double dy );


public:
    /*
	 *	Le son...
	 */
	static Sound	* _guard_fire;
	static Sound	* _guard_hit;
	static Sound	* _guard_die;
	static Sound	* _wall_hit;
    int		mode; // 3 modes, initialiser a 1
	int		s;    // compter les etapes
	int		_min_tresor; // le plus court distance entre le gardien et le tresor
	int		_min_player; // le plus court distance entre le gardien et le chausser
    double		_dx;
	double		_dy;
	bool	gf;         // le gardien en train de marche
	int		tf = (int) time( 0 );  // pour tire correct
	Labyrinthe	* ll;  // le labyrinthe, pour maj les gardiens
	Gardien( Labyrinthe* l, const char* modele );
	// ne bouger que dans une case vide (on 'glisse' le long des obstacles) 
	bool move( double dx, double dy )
	{
		return(move_aux( dx, dy ) || move_aux( dx, 0.0 ) || move_aux( 0.0, dy ) );
	}

	// maj les gardiens 
	void update( void );
	std::vector<int> Distancetresor( void );
	std::vector<int> Distanceplayer( void ); 
    // les conditions pour changer les mode 
    void check( void );
    double Random( void ); 
	// 1 mode,
	void guard( void );
    // 2 mode
	void attack( std::vector<int> );
    // 3 mode
	void defense( std::vector<int> );
	// fait bouger la boule de feu 
	bool process_fireball( float dx, float dy );
	// tire sur un ennemi. 
	void fire( int angle_vertical );

};

#endif
