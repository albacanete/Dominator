#include "Player.hh"
#include <queue>
#include <list>
#include <map>


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME Alba
#define OTHER_COLOUR 0
#define WNOCOLOUR 1
#define MY_COLOUR 2

struct PLAYER_NAME : public Player {

	/**
	* Factory: returns a new instance of this class.
	* Do not modify this function.
	*/
	static Player* factory () {
		return new PLAYER_NAME;
	}

	/**
	* Types and attributes for your player can be defined here.
	*/
	typedef vector<int> iv;
	
	/*int playerAboveScore() {
		int pl = 1;
		int ts=total_score(1);
		for (int i = 2; i <= 3; ++i) {
			if (ts < total_score(i)) {
				ts = total_score(i);
				pl = i;
			}
		}
		return pl;
	}*/
	
	Dir farmersNewPosition(Pos p) {
		map<Pos, Dir> m;
		queue<Pos> q;
		int d = 0;
		q.push(p);
		while (!q.empty()) {
			Pos p2 = q.front();
			for (int i = 0; i < 7; i+=2) {
				Cell c = cell(p2+Dir(i));
				if (!c.haunted && c.type != Wall) {
					m.insert(make_pair(p2+Dir(i), Dir(i)));
					if(c.owner != 0) {
						//Devolver dirección
					}
					else q.push(p2+Dir(i));
				}
			}
			++d;
			q.pop();
		}
		return Dir(2*random(0,3));
	}
		
	/*Dir newKnightPosition(Pos p) {
		map<Pos, Dir> m;
		queue<Pos> q;
		q.push(p);
		while (!q.empty()) {
			Pos pa = q.front();
			for (int i = 0; i < 8; ++i) {
				Cell c = cell(
	}*/
	
	/**
	* Play method, invoked once per each round.
	*/
	virtual void play () {
		iv f = farmers(0);
		for (int id : f) {
			if (unit(id).health > 60)  command(id, farmersNewPosition(unit(id).pos));
			else command(id, None);
		}
		
		/*iv k = knights(0);
		for (int id : k) {
			if (unit(id).health > 60) {
				list<Dir> l = newKnightPosition(unit(id).pos);
				command(id, l.front());
			}
		}*/
		
		/*iv w = witches(0);
		for(int id : w) {
			command(id, Dir(random(0,7)));
		}*/
	}
};
			


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
