#ifndef _PHYSICAL_OP_H_                   // include file only once
#define _PHYSICAL_OP_H_

#include "StorageManager/Block.h"
#include "StorageManager/Config.h"
#include "StorageManager/Disk.h"
#include "StorageManager/Field.h"
#include "StorageManager/MainMemory.h"
#include "StorageManager/Relation.h"
#include "StorageManager/Schema.h"
#include "StorageManager/SchemaManager.h"
#include "StorageManager/Tuple.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <iterator>
#include <vector>
#include "DataStructures.h"

using namespace std;

class physicalOP{
private:
	MainMemory mem;
	Disk disk;
	SchemaManager   schema_manager;
	clock_t start_time;
	static physicalOP *physicalop;
    physicalOP():schema_manager(&mem, &disk) {  
		disk.resetDiskIOs();
		disk.resetDiskTimer(); }    //singleton

public:
	//add some for debug
	void displayMem();
	void displaySchema(Schema &schema);
	void displayRelation(string relation_name);
	int IO(){return disk.getDiskIOs();}

	static physicalOP *getInstance() {
        if (physicalop == NULL) {
            physicalop = new physicalOP();
			cout<<"Physical operator initialized!"<<endl;
			physicalop->displayMem();
        }
        return physicalop;
    }

	Relation * CreateTable(string relation_name,
                     vector<string> & field_names, 
                     vector <enum FIELD_TYPE> & field_types);

	void DropTable(string relation_name);

	void insert(string relation_name,
				vector<string> field_names,
				vector<string> STR,
				vector<int> INT);

	vector<Tuple> singleTableSelect(string relation_name,
									condition con);
	
	void Delete(string relation_name,
				condition con);

	void sortOnMemory(string relation_name,string field_names,
						int start_block,int num_blocks);  //sort the tuples in Mem


	vector<Tuple> Product(string relation_name1,
				       string relation_name2);   //cross join, one pass

	vector<Tuple> JoinOnePass(string relation_name1,
				       string relation_name2);   //natural join, one pass
	Tuple JoinOneTuple(Tuple t1,Tuple t2);         //return a joined tuple if can join, or an invalid tuple

	vector<Tuple> JoinTwoPass(string relation_name1, 
							string relation_name2);  //natural join, one pass
};

	



#endif