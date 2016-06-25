#include "CommandExecutionEngine.h"
#include "Common.h"
#include "NetworkSocket.h"
#include <ostream>

bool IsCupThere = false;

//WiringPi is een library die je de GPIO pennen in arduino stijl laat aansturen

void KoffieAanUit() 
{
	pinMode(13, OUTPUT);
	digitalWrite(13, HIGH);
	delay(100);
	digitalWrite(13,LOW);
}

void EenKopKoffie() 
{
	pinMode(6, OUTPUT);
	digitalWrite(6, HIGH);
	delay(100);
	digitalWrite(6, LOW);
}

void TweeKopKoffie() 
{
	pinMode(19, OUTPUT);
	digitalWrite(19, HIGH);
	delay(100);
	digitalWrite(19, LOW);
}

//Query'ed de DB voor RFID Checked IN
bool MayUseCoffeeDevice(CommandExecutionEngine& ref) 
{
	Query query = ref.conref.query();
	query << "SELECT * FROM USE_COFFEE_MACHINE";
	StoreQueryResult res = query.store();

	query << "SELECT * FROM RFID";
	StoreQueryResult res2 = query.store();

	for (size_t i = 0; i < res.num_rows(); ++i)
	{
		for (size_t j = 0; j < res2.num_rows(); ++j)
		{
			if (res[i]["RFID_CODE"] == res2[j]["RFID_CODE"] && static_cast<int>(res2[j]["CHECKED_IN"]) == 1)
				return true;
		}
	}
	return false;
}

CommandExecutionEngine::CommandExecutionEngine(size_t threadcount, Connection& con) : pool(threadcount - 1), conref(con)
{
}


CommandExecutionEngine::~CommandExecutionEngine()
{
}

//Voert commands uit
void CommandExecutionEngine::Execute(string command, string sender)
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
		bool DoesExist = false;
		int checked = -1;
		int iteration = -1;
		for (size_t i = 0; i < res.num_rows(); ++i)
		{
			cout << "Name: " << res[i]["RFID_CODE"] << " " << res[i]["CHECKED_IN"] << " " << std::endl;
			if (res[i]["RFID_CODE"] == RFID)
			{
				iteration = i;
				checked = res[i]["CHECKED_IN"];
				DoesExist = true;
				break;
			}
		}

		if (DoesExist)
		{
			checked = checked == 0 ? 1 : 0;
			std::ostringstream convert;
			convert << checked;
			string q = "UPDATE RFID SET CHECKED_IN='";
			q += convert.str();
			q += "' WHERE RFID_CODE ='";
			q += RFID;
			q += "'";
			query << q;
			query.execute();
		}
		string msg;
		switch (checked)
		{
		case -1:
			//red led
			msg = "-1";
			break;
		case 0:
			//orange led
			msg = "0";
			break;
		case 1:
			//green led
			msg = "1";
			break;
		}
		if(sender != "localhost")
		NetworkSocket::SendMessage(sender.c_str(), msg);
	}
	
	//Cup is placed
	if (command[0] == 'C' && command[1] == 'P' && command[2] == 'S' && command[3] == '1')
	{
		IsCupThere = true;
		cout << "A cup is placed" << endl;
		return;
	}
	//Cup is removed
	if (command[0] == 'C' && command[1] == 'P' && command[2] == 'S' && command[3] == '0')
	{
		IsCupThere = false;
		cout << "A cup is removed" << endl;
		return;
	}
	
	
	if (!MayUseCoffeeDevice(*this))
	{
		if (sender != "localhost")
		{
			//Niemand ingecheckt van security group
			NetworkSocket::SendMessage(sender, "3");
		}
		return;
	}
	if (!IsCupThere)
	{
		if (sender != "localhost")
		{
			//Geen kopje aanwezig
			NetworkSocket::SendMessage(sender, "4");
		}
		return;
	}
	
	//Koffie aan
	if(command[0] == 'K' && command[1] == 'A')
	{
		//Shared_ptr is een pointer met ref counting, delete de resource automatisch wanneer er geen refs meer zijn
		std::shared_ptr<Task> p = std::make_shared<Task>(KoffieAanUit);
		pool.Enqueue(*p);
		cout << "Koffie apparaat aan!" << endl;
		if (sender != "localhost") 
		{
			NetworkSocket::SendMessage(sender, "3");
		}
	}
	
	//1 kopje
	if (command[0] == 'K' && command[1] == '1')
	{
		cout << "Koffie zetten voor 1 persoon!" << endl;
		std::shared_ptr<Task> p = std::make_shared<Task>(EenKopKoffie);
		pool.Enqueue(*p);
		if (sender != "localhost") 
		{
			NetworkSocket::SendMessage(sender, "1");
		}
	}
	
	//2 kopjes
	if (command[0] == 'K' && command[1] == '2')
	{
		std::shared_ptr<Task> p = std::make_shared<Task>(TweeKopKoffie);
		pool.Enqueue(*p);
		cout << "Koffie zetten voor 2 personen!" << endl;	
		if (sender != "localhost") 
		{
			NetworkSocket::SendMessage(sender, "1");
		}
	}	
	
}