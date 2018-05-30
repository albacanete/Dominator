#include "Player.hh"
#include <queue>
#include <list>
#include <map>


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME Mejor

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
	bool farmersPossiblePosition (Pos p) {
		Cell c = cell(p);
		return (!c.haunted && c.type != Wall && c.id == -1 && pos_ok(p));
	}
	
	bool knightsPossiblePosition (Pos p) {
		Cell c = cell(p);
		return (!c.haunted && c.type != Wall && (c.id == -1 || unit(c.id).player != 0));
	}
	
	bool witchesPossiblePosition (Pos p) {
		Cell c = cell(p);
		return (c.type != Wall && (c.id == -1 || unit(c.id).type != Witch || unit(c.id).player != 0));
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
			if (visPos.find(paux+Bottom) == visPos.end() && farmersPossiblePosition(paux+Bottom)) {
				q.push(paux+Bottom);
				visPos.insert(make_pair(paux+Bottom, paux));
			}
			if (visPos.find(paux+Right) == visPos.end() && farmersPossiblePosition(paux+Right)) {
				q.push(paux+Right);
				visPos.insert(make_pair(paux+Right, paux));
			}
			if (visPos.find(paux+Left) == visPos.end() && farmersPossiblePosition(paux+Left)) {
				q.push(paux+Left);
				visPos.insert(make_pair(paux+Left, paux));
			}
			if (visPos.find(paux+Top) == visPos.end() && farmersPossiblePosition(paux+Top)) {
				q.push(paux+Top);
				visPos.insert(make_pair(paux+Top, paux));
			}
			
		}
		return None;
	}
	
	/**
	 * Returns the the direction to the nearest unit 
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
				else return RT;
			}
			for (int i = 0; i < 8; ++i) {
				Pos pnext = paux+Dir(i);
				if (visPos.find(pnext) == visPos.end() && (cell(pnext).id == -1  || (cell(pnext).id != -1
					&& unit(cell(pnext).id).type != Witch)) && pos_ok(pnext) && knightsPossiblePosition(pnext)) {
					q.push(pnext);
					visPos.insert(make_pair(pnext, paux));
				}
			}
			q.pop();
		}
		return None;
	}
	
	Dir witchesNewPosition (Pos p) {
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
			}
			for (int i = 0; i < 4; ++i) {
				Pos pnext = paux+Dir(2*i);
				if (visPos.find(pnext) == visPos.end() && (cell(pnext).id == -1  || (cell(pnext).id != -1
					&& unit(cell(pnext).id).type != Witch)) && witchesPossiblePosition(pnext) && pos_ok(pnext)) {
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
			command(id, witchesNewPosition(unit(id).pos));
		}
    }
};
			


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
