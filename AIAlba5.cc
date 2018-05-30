#include "Player.hh"
#include <queue>
#include <list>
#include <map>


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME AlbaFinal

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
	typedef map<Pos, Pos> ppMap;
	
	bool farmersPossiblePosition (Pos p) {
		Cell c = cell(p);
		return (!c.haunted && c.type != Wall && c.id == -1);
	}
	
	bool knightsPossiblePosition (Pos p) {
		Cell c = cell(p);
		return (!c.haunted && c.type != Wall && (c.id == -1 || (unit(c.id).player != 0 && unit(cell(p).id).type != Witch))); 
	}
	
	bool witchesPossiblePosition (Pos p) {
		Cell c = cell(p);
		return (c.type != Wall && (c.id ==-1 || (unit(c.id).player != 0 && unit(cell(p).id).type != Witch)));
	}
	
	bool minimumEnemies (Pos p) {
		int enemies = 0;
		for (int i = 0; i < 8; ++i) {
			if (i%2 == 0) {
				if (cell(p+Dir(i)).id != -1 && unit(cell(p+Dir(i)).id).player != 0) ++enemies;
				if (pos_ok(p+Dir(i)+Dir(i)) && cell(p+Dir(i)+Dir(i)).id != -1  && unit(cell(p+Dir(i)+Dir(i)).id).player != 0) ++enemies;
			}
			else {
				if (cell(p+Dir(i)).id != -1  && unit(cell(p+Dir(i)).id).player != 0) ++enemies;
			}
		}
		return (enemies >= 3);
	}
	
	bool farmersSafePositionToGo (Pos p) {
		for (int i = 0; i < 4; ++i) {
			Cell c = cell(p+Dir(2*i));
			if (c.haunted) return false;
		}
		return true;
	}
	
	bool knightsSafePositionToGo (Pos p) {
		for (int i = 0; i < 8; ++i) {
			Cell c = cell(p+Dir(i));
			if (c.haunted) return false;
		}
		return true;
	}
	
	bool safePositionToAttack (Pos p, int id) {
		bool safe = true;
		for (int i = 0; i < 8 && safe; ++i) {
			if (i%2 == 0) {
				if (cell(p+Dir(i)).id != id && cell(p+Dir(i)).id != -1 && unit(cell(p+Dir(i)).id).player == 0) safe = false;
				if (!pos_ok(p+Dir(i)+Dir(i)) || (cell(p+Dir(i)+Dir(i)).id != id && cell(p+Dir(i)+Dir(i)).id != -1 && unit(cell(p+Dir(i)+Dir(i)).id).player == 0)) safe = false;
			}
			else {
				if (cell(p+Dir(i)).id != id && cell(p+Dir(i)).id != -1 && unit(cell(p+Dir(i)).id).player == 0) safe = false;
			}
		}
		return safe;
	}
	
	/**
	 * Returns the direction to the nearest position that 
	 * the farmer in position p can conquer 
	*/
	Dir farmersNewPosition(Pos p) {
		queue<Pos> q;
		ppMap visPos;	
		visPos.insert(make_pair(p, p));
		q.push(p);
		while(!q.empty()) {
			Pos paux = q.front();
			q.pop();
			Cell c = cell(paux);
			if(c.owner != 0) {
				Pos preturn = paux;
				while (visPos[preturn] != p) preturn = visPos[preturn];
				if (preturn+Bottom == p) return Top;
				if (preturn+Right == p) return Left;
				if (preturn+Left == p) return Right;
				return Bottom;
			}
			if (visPos.find(paux+Bottom) == visPos.end()) {
				if (farmersPossiblePosition(paux+Bottom) && farmersSafePositionToGo(paux+Bottom)) {
					q.push(paux+Bottom);
					visPos.insert(make_pair(paux+Bottom, paux));
				}
			}
			if (visPos.find(paux+Right) == visPos.end()) {
				if (farmersPossiblePosition(paux+Right) && farmersSafePositionToGo(paux+Right)) {
					q.push(paux+Right);
					visPos.insert(make_pair(paux+Right, paux));
				}
			}
			if (visPos.find(paux+Left) == visPos.end()) {
				if (farmersPossiblePosition(paux+Left) && farmersSafePositionToGo(paux+Left)) {
					q.push(paux+Left);
					visPos.insert(make_pair(paux+Left, paux));
				}
			}
			if (visPos.find(paux+Top) == visPos.end()) {
				if (farmersPossiblePosition(paux+Top) && farmersSafePositionToGo(paux+Top)) {
					q.push(paux+Top);
					visPos.insert(make_pair(paux+Top, paux));
				}
			}
			
		}
		return None;
	}
	
	/**
	 * Returns the direction to the nearest unit 
	 * the knight in postion p can attack to
	*/
	Dir knightsNewPosition(Pos p) {
		queue<Pos> q;
		ppMap visPos;
		visPos.insert(make_pair(p, p));	
		q.push(p);
		while (!q.empty()) {
			Pos paux = q.front();
			if (cell(paux).id != -1 && paux != p) {
				Pos preturn = paux;
				while (visPos[preturn] != p) preturn = visPos[preturn];
				if (preturn+Bottom == p) return Top;
				if (preturn+Right == p) return Left;
				if (preturn+Left == p) return Right;
				if (preturn+Top == p) return Bottom;
				if (preturn+BR == p) return TL;
				if (preturn+RT == p) return LB;
				if (preturn+TL == p) return BR;
				return RT;
			}
			for (int i = 0; i < 8; ++i) {
				Pos pnext = paux+Dir(i);
				if (visPos.find(pnext) == visPos.end()) {
					if (knightsPossiblePosition(pnext) && knightsSafePositionToGo(pnext)) {
						q.push(pnext);
						visPos.insert(make_pair(pnext, paux));
					}
				}
			}
			q.pop();
		}
		return None;
	}
	
	/**
	 * Returns the direction to the nearest at least three 
	 * opponent units the witch can kill 
	*/
	Dir witchesNewPosition (Pos p, int id, bool w0) {
		queue<Pos> q;
		ppMap visPos;
		vector<Dir> directions;
		if (w0) directions = {Bottom, Right, Left, Top};
		else directions = {Top, Right, Left, Bottom};
		visPos.insert(make_pair(p, p));	
		q.push(p);
		while (!q.empty()) {
			Pos paux = q.front();
			if (safePositionToAttack(paux, id) && paux != p && minimumEnemies(paux)) {
				Pos preturn = paux;
				while (visPos[preturn] != p) preturn = visPos[preturn];
				if (preturn+Bottom == p) return Top;
				if (preturn+Right == p) return Left;
				if (preturn+Left == p) return Right;
				return Bottom;
			}
			for (int i = 0; i < 4; ++i) {
				Pos pnext = paux+directions[i];
				if (visPos.find(pnext) == visPos.end() && witchesPossiblePosition(pnext)) {
					q.push(pnext);
					visPos.insert(make_pair(pnext, paux));
				}
			}
			q.pop();
		}
		return None;
	}
	
	/**
	* Play method, invoked once per each round.
	*/
	virtual void play () { 
		iv f = farmers(0);
		for (int id : f) {
			command(id, farmersNewPosition(unit(id).pos));
		}
		
		iv k = knights(0);
		for (int id : k) {
			command(id, knightsNewPosition(unit(id).pos));
		}
		
		iv w = witches(0);
		for (int id : w) {
			bool w0 = (id == w[0]);
			command(id, witchesNewPosition(unit(id).pos, id, w0));
		}
    }
};
			


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
