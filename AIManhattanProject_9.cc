#include "Player.hh"


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME CatStevens


struct PLAYER_NAME : public Player {

  /**
   * Factory: returns a new instance of this class.
   * Do not modify this function.
   */
  static Player* factory () {
    return new PLAYER_NAME;
  }
  
  
  bool pos_that_can_be_haunted(Pos p,int own){
	  if(!pos_ok(p)) return true;
	  Cell c=cell(p);
	  return(c.id == own or c.id==-1 or (unit(c.id).player!=0 and unit(c.id).type!=Witch));
  }
  
  bool pos_to_haunt(Pos p){
	  if(!pos_ok(p)) return false;
	  Cell c=cell(p);
	  return(c.id!=-1 and unit(c.id).player!=0 and unit(c.id).type!=Witch);
  }
  
  bool pos_to_stay(Pos p){
	  return(!cell(p).haunted);
  }
  
  bool pos_to_pass_around(Pos p,int own){
	  if(!pos_ok(p)) return true;
	  Cell c=cell(p);
	  return(c.id==-1 or c.id==own or unit(c.id).player!=0);
  }
  
  bool pos_to_pass(Pos p){
	  Cell c=cell(p);
	  return(c.type!=Wall and (c.id==-1 or (unit(c.id).player!=0 and unit(c.id).type!=Witch)));
  }
  
  
  bool way_ok(Pos newpos){
	  bool b=true;
	  
	  if(!pos_to_pass(newpos)) b=false;
	 
	  return b;
  }
  
  bool safe_way(Pos p){
	  bool b = true;
	  for (int i=0; i<8 and b; i+=2){
		  if (cell(p+Dir(i)).haunted) b=false;
	  }
	  return b;
  }
  
    
  bool safe_pos(Pos p){
	  bool b=true;
	  
	  for(int i=0; i<8 and b; ++i){
		  if(cell(p+Dir(i)).haunted) b=false;
	  }
	  
	  return b;
  }
  
  
  bool fix_way(Pos newpos,int own){
	  bool safe=true;
	  int kill=0;
	  
	  if(!pos_to_stay(newpos)) safe=false;
	  
	  else{
		  for(int i=0;i<8;++i){
			  if (i%2){
				  if(!pos_that_can_be_haunted(newpos+Dir(i),own)) safe=false;
			  }
			  else{
				  if(!pos_that_can_be_haunted(newpos+Dir(i)+Dir(i),own)) safe=false;
				  if(!pos_that_can_be_haunted(newpos+Dir(i),own)) safe=false;
			  }
		  }
	  }
	  
	  if(!safe) return false;
	  
	  for(int i=0;i<8;++i){
		  if (i%2){
			  if(pos_to_haunt(newpos+Dir(i))) ++kill;
		  }
		  else{
			  if(pos_to_haunt(newpos+Dir(i)+Dir(i))) ++kill;
			  if(pos_to_haunt(newpos+Dir(i))) ++kill;
		  }
	  }
	  return (kill>=3);
  }

  
  Dir best_dir_w(int id){
	  map<Pos,Dir> the_map;
	  queue<Pos> the_queue;
	  Pos actpos=unit(id).pos;
	  the_queue.push(actpos);
	  the_map.insert(make_pair(actpos,None));
	  bool first=true;
	  while(not the_queue.empty()){
		  Pos p=the_queue.front();
		  the_queue.pop();
		  
		  if(!first and fix_way(p,id)){
			  map<Pos,Dir>::iterator it;
			  it=the_map.find(p);
			  return it->second;
		  }
		  
		  vector<Dir> vdir = {Right, Bottom, Left, Top};
		  
		  for(int i=0; i<4; ++i){
			  if(the_map.find(p+vdir[i])==the_map.end()){
				  if(way_ok(p+vdir[i])){
					if(first) the_map.insert(make_pair(p+vdir[i],vdir[i]));
					else the_map.insert(make_pair(p+vdir[i],the_map.find(p)->second));
					the_queue.push(p+vdir[i]);
				  }
			  }
		  }
		  
		  first=false;
	  }
	  return None;
  }
  
  
  
  Dir best_dir_k(int id){
	  map<Pos,Dir> the_map;
	  queue<Pos> the_queue;
	  Pos actpos=unit(id).pos;
	  the_queue.push(actpos);
	  the_map.insert(make_pair(actpos,None));
	  bool first=true;
	  while(not the_queue.empty()){
		  Pos p=the_queue.front();
		  the_queue.pop();  
		  
		  if(!first and !cell(p).haunted and cell(p).id!=-1 and unit(cell(p).id).type!=Witch and unit(cell(p).id).player!=0){
			  map<Pos,Dir>::iterator it;
			  it=the_map.find(p);
			  return it->second;
		  }
		  
		  vector<Dir> vdir = {Bottom, BR, Right, RT, Left, TL, LB, Top};
		  
		  for (int i=0;i<8;++i){
			  if(the_map.find(p+vdir[i])==the_map.end()){
				if(cell(p+vdir[i]).type!=Wall and !cell(p+vdir[i]).haunted and
				(cell(p+vdir[i]).id==-1 or (unit(cell(p+vdir[i]).id).type!=Witch and unit(cell(p+vdir[i]).id).player!=0)) and safe_pos(p+vdir[i])){
					if(first) the_map.insert(make_pair(p+vdir[i],vdir[i]));
					else the_map.insert(make_pair(p+vdir[i],the_map.find(p)->second));
					the_queue.push(p+vdir[i]);
				}
			}
		}
		  first=false;
	  }
	  return None;
  }
  
  
  Dir best_dir_f(int id){
	  map<Pos,Dir> the_map;
	  queue<Pos> the_queue;
	  Pos actpos=unit(id).pos;
	  the_queue.push(actpos);
	  the_map.insert(make_pair(actpos,None));
	  bool first=true;
	  while(not the_queue.empty()){
		  Pos p=the_queue.front();
		  the_queue.pop();  
		  
		  if(!first and cell(p).owner!=0){
			  map<Pos,Dir>::iterator it;
			  it=the_map.find(p);
			  return it->second;
		  }
		  
		  vector<Dir> vdir = {Bottom, Right, Left, Top};
		  
		  for(int i=0; i<4; ++i){
			  if(the_map.find(p+vdir[i])==the_map.end()){
				  if(cell(p+vdir[i]).type!=Wall and !cell(p+vdir[i]).haunted and cell(p+vdir[i]).id==-1 and safe_pos(p+vdir[i])){
						if(first) the_map.insert(make_pair(p+vdir[i],vdir[i]));
						else the_map.insert(make_pair(p+vdir[i],the_map.find(p)->second));
						the_queue.push(p+vdir[i]);
					}
			}
		  }
		  first=false;
	  }
	  return None;
  }
	  

  /**
   * Types and attributes for your player can be defined here.
   */
	typedef vector<int> vint;
  /**
   * Play method, invoked once per each round.
   */
  virtual void play () {
	  vint f = farmers(0);
	  
	  /**
	   * Analiza las posibles direcciones a las que se puede mover y les asigna una prioridad.
	   * Si hay más de una posible dirección con la misma prioridad, la elige de forma random.
	   */
	  for (int id : f) {
		 command(id,best_dir_f(id));
	 }
    
    vint k = knights(0);
	  
	  /**
	   * Analiza las posibles direcciones a las que se puede mover y les asigna una prioridad.
	   * Si hay más de una posible dirección con la misma prioridad, la elige de forma random.
	   */
	  for (int id : k) {
		  command(id,best_dir_k(id));
	  }
	  
	  
	  
	  vint w = witches(0);
	  
	  /**
	   * Analiza las posibles direcciones a las que se puede mover y les asigna una prioridad.
	   * Si hay más de una posible dirección con la misma prioridad, la elige de forma random.
	   */
	  for (int id : w) {
		  command(id,best_dir_w(id));
	  }
  }
};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
