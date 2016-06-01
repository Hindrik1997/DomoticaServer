#include "CommandExecutionEngine.h"
#include "Common.h"

CommandExecutionEngine::CommandExecutionEngine(size_t threadcount, Connection& con) : pool(threadcount - 1), conref(con)
{
}


CommandExecutionEngine::~CommandExecutionEngine()
{
}

void CommandExecutionEngine::Execute(string command)
{
	//Commands!
	if (command[0] == 'R' && command[1] == 'F' && command[2] == 'I' && command[3] == 'D' && command[4] == ':' && command[5] == ' ')
	{
		string RFID;
		//RFID TAG!
		for (int i = 6; i < command.length(); ++i)
		{
			RFID += command[i];
		}
		Query query = conref.query();
		query << "SELECT * FROM RFID";
		StoreQueryResult res = query.store();
		for (size_t i = 0; i < res.num_rows(); ++i)
		{
			cout << "Name: " << res[i]["RFID_CODE"] << " " << res[i]["CHECKED_IN"] << " " << std::endl;			
		}
		
		
		
		
		
	}
	
	if(command[0] == 'K' && command[1] == 'A')
	{
		cout << "Koffie apparaat aan!" << endl;		
	}
	
	if (command[0] == 'K' && command[1] == 'Z')
	{
		cout << "Koffie zetten!" << endl;		
	}
	
}