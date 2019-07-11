#ifndef LABYRINTHE_H
#define LABYRINTHE_H

#include "Environnement.h"


class Labyrinthe : public Environnement {
private:
	char	** _data;
	int	** _dist;
	int	** _dist_player;
	int	LAB_WIDTH;
	int	LAB_HEIGHT;
public:
	Labyrinthe( char* );
	int width () { return LAB_WIDTH;}	// retourne la largeur du labyrinthe.
	int height () { return LAB_HEIGHT;}	// retourne la longueur du labyrinthe.
	
	// retourne la case (i, j).
	char data (int i, int j)
	{
		return _data [i][j];
	}	
    
     // mettre la case (i, j) a v
	void set_data( int i, int j, int v )
	{
		_data[i][j] = v;
	}       

    
    // retourne les distance entre les cases et la tresor.
	int** get_dist()
	{
		return(_dist);
	}    

   // retourne les distance entre les cases et la play.
	int** get_dist_player()
	{
		return(_dist_player);
	}   


	void redictplayer( void );
};

#endif
