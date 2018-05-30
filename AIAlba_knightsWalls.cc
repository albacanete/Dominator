#include "Player.hh"
#include <queue>
#include <list>
#include <map>


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME AlbaKWalls

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
	
	/**
	 * Returns the direction to the nearest position that 
	 * the farmer in position p can conquer 
	*/
	Dir farmersNewPosition(Pos p, pbMap visited_pos) {
		queue<Pos> q;
		pMatrix pm(rows(), vector<Pos>(cols()));	
		visited_pos.insert(make_pair(p, true));
		q.push(p);
		while(!q.empty()) {
			Pos paux = q.front();
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
			if (!visited_pos[paux+Bottom] && !c.haunted && pos_ok(paux+Bottom)) {
				q.push(paux+Bottom);
				visited_pos[paux+Bottom] = true;
				pm[(paux+Bottom).i][(paux+Bottom).j] = paux;
			}
			
			c = cell(paux+Right);
			if (!visited_pos[paux+Right] && !c.haunted && pos_ok(paux+Right)) {
				q.push(paux+Right);
				visited_pos[paux+Right] = true;
				pm[(paux+Right).i][(paux+Right).j] = paux;
			}
			
			c = cell(paux+Left);
			if (!visited_pos[paux+Left] && !c.haunted && pos_ok(paux+Left)) {
				q.push(paux+Left);
				visited_pos[paux+Left] = true;
				pm[(paux+Left).i][(paux+Left).j] = paux;
			}
			
			c = cell(paux+Top);
			if (!visited_pos[paux+Top] && !c.haunted && pos_ok(paux+Top)) {
				q.push(paux+Top);
				visited_pos[paux+Top] = true;
				pm[(paux+Top).i][(paux+Top).j] = paux;
			}
			q.pop();
		}
		return None;
	}

	/**
	 * Returns the the direction to the nearest unit 
	 * the knight in postion p can attack to
	*/
	Dir knightsNewPosition(Pos p, pbMap visited_pos) {
		queue<Pos> q;
		pMatrix pm(rows(), vector<Pos>(cols()));	
		q.push(p);
		while (!q.empty()) {
			Pos paux = q.front();
			if (cell(paux).id != -1 && 	unit(cell(paux).id).player != 0) {
				Pos preturn = paux;
				while (pm[preturn.i][preturn.j] != p) preturn = pm[preturn.i][preturn.j];
				if (preturn+Bottom == p) return Top;
				if (preturn+Right == p) return Left;
				if (preturn+Left == p) return Right;
				else return Bottom;
			}
			for (int i = 0; i < 8; ++i) {
				if (!visited_pos[paux+Dir(i)] && !cell(paux+Dir(i)).haunted && pos_ok(paux+Dir(i))) {
					q.push(paux+Dir(i));
					visited_pos[paux+Dir(i)] = true;
					pm[(paux+Dir(i)).i][(paux+Dir(i)).j] = paux;
				}
			}
			q.pop();
		}
		return None;
	}
	
	void initialVisitsFarmers(pbMap& visited_pos) {
		for (int i = 0; i < rows(); ++i) {
			for (int j = 0; j < cols(); ++j) {
				Cell c = cell(Pos(i,j));
				if (c.id != -1 || c.haunted || c.type == Wall) 
					visited_pos.insert(make_pair(Pos(i,j), true));
				else visited_pos.insert(make_pair(Pos(i,j), false));
			}
		}
	}
	
	void initialVisitsKnights(pbMap& visited_pos) {
		for (int i = 0; i < rows(); ++i) {
			for (int j = 0; j < cols(); ++j) {
				Cell c = cell(Pos(i,j));
				if (c.haunted || c.type == Wall) visited_pos[Pos(i,j)] = true;
				//else if (c.id != -1 && unit(c.id).player == 0) visited_pos[Pos(i,j)] = true;
				else if (c.id != -1 && unit(c.id).type == Witch) visited_pos[Pos(i,j)] = true;
				else visited_pos[Pos(i,j)] = false;
			}
		}
	}
	
	/**
	* Play method, invoked once per each round.
	*/
	virtual void play () {
		pbMap visited_pos;
		
		initialVisitsFarmers(visited_pos);
		iv f = farmers(0);
		for (int id : f) {
			if (unit(id).health > 60) command(id, farmersNewPosition(unit(id).pos, visited_pos));
			else command(id, None);
		}
		
		initialVisitsKnights(visited_pos);
		iv k = knights(0);
		/*if (round() == 199) {
		for (pbMap::iterator it = visited_pos.begin(); it != visited_pos.end(); ++it) 
			cerr << (*it).first << " " << (*it).second << endl;
		}*/
		for (int id : k) {
			if (unit(id).health > 60) command(id, knightsNewPosition(unit(id).pos, visited_pos));
			else command(id, None);
		}
		
		/*iv w = witches(0);
		for(int id : w) {
			if (round() < 50 && pos_ok(unit(id).pos)) command(id, Right);
			if (round() < 100 && pos_ok(unit(id).pos)) command(id, Bottom);
			if (round() < 150 && pos_ok(unit(id).pos)) command(id, Left);
			if (round() < 200 && pos_ok(unit(id).pos)) command(id, Dir(2*random(0,3)));
		}*/
	}
};
			


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
