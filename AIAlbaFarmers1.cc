#include "Player.hh"
#include <queue>
#include <list>
#include <map>


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME AlbaFarmers
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
	typedef map<Pos, bool> pbMap;
	typedef vector <vector<Pos> > pMatrix;
	
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
	Dir farmersNewPosition(Pos p, pbMap visited_pos) {
		queue<Pos> q;
		pMatrix pm(rows(), vector<Pos>(cols()));	
		visited_pos.insert(make_pair(p, true));
		q.push(p);
		while(!q.empty()) {
			Pos paux = q.front();
			q.pop();
			Cell c = cell(paux);
			if(c.owner != 0) {
				Pos preturn = paux;
				while (pm[preturn.i][preturn.j] != p) preturn = pm[preturn.i][preturn.j];
				if (preturn+Bottom == p) return Top;
				if (preturn+Right == p) return Left;
				if (preturn+Left == p) return Right;
				else return Bottom;
			}
			c = cell(paux+Bottom);
			if (!visited_pos[paux+Bottom] && !c.haunted) {
				q.push(paux+Bottom);
				visited_pos[paux+Bottom] = true;
				pm[(paux+Bottom).i][(paux+Bottom).j] = paux;
			}
			c = cell(paux+Right);
			if (!visited_pos[paux+Right] && !c.haunted) {
				q.push(paux+Right);
				visited_pos[paux+Right] = true;
				pm[(paux+Right).i][(paux+Right).j] = paux;
			}
			c = cell(paux+Left);
			if (!visited_pos[paux+Left] && !c.haunted) {
				q.push(paux+Left);
				visited_pos[paux+Left] = true;
				pm[(paux+Left).i][(paux+Left).j] = paux;
			}
			c = cell(paux+Top);
			if (!visited_pos[paux+Top] && !c.haunted) {
				q.push(paux+Top);
				visited_pos[paux+Top] = true;
				pm[(paux+Top).i][(paux+Top).j] = paux;
			}
		}
		return None;
	}

	/*Dir newKnightPosition(Pos p) {
		map<Pos, Dir> m;
		queue<Pos> q;
		q.push(p);
		while (!q.empty()) {
			Pos pa = q.front();
			for (int i = 0; i < 8; ++i) {
				if (!c.haunted && c.type != Wall) {
					if ((Pa+Dir(i)).type
	}
	*/
	void initialVisits(pbMap& visited_pos) {
		for (int i = 0; i < rows(); ++i) {
			for (int j = 0; j < cols(); ++j) {
				Cell c = cell(Pos(i,j));
				if (c.id != -1 || c.haunted || c.type == Wall) 
					visited_pos.insert(make_pair(Pos(i,j), true));
				else visited_pos.insert(make_pair(Pos(i,j), false));
			}
		}
	}
	
	/**
	* Play method, invoked once per each round.
	*/
	virtual void play () {
		pbMap visited_pos;
		initialVisits(visited_pos);
		iv f = farmers(0);
		for (int id : f) {
			if (unit(id).health > 60)  command(id, farmersNewPosition(unit(id).pos, visited_pos));
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
