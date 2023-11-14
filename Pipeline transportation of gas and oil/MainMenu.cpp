#include "MainMenu.h"

using namespace std;

void MenuShowObjects(GasSupplySystem& gss)
{
	vector<string> menu = { "Show pipes", "Show stations", "Show all" };
	const string menu_information[2] =
	{ "-----------------------------------------------\n"
	"\tINFORMATION ABOUT ALL PIPES\n"
	"-----------------------------------------------\n\n",
	"-----------------------------------------------\n"
	"\tINFORMATION ABOUT ALL CS\n"
	"-----------------------------------------------\n\n" };
	switch (ChooseActionMenu(menu, true))
	{
	case 1:
	{
		cout << menu_information[0];
		gss.ShowPipes();
		break;
	}
	case 2:
	{
		cout << menu_information[1];
		gss.ShowCS();
		break;
	}
	case 3:
	{
		cout << menu_information[0];
		gss.ShowPipes();
		cout << menu_information[1];
		gss.ShowCS();
		break;
	}
	case 0:
	{
		break;
	}
	default:
		break;
	}
}

void MenuSave(GasSupplySystem& gss)
{
	cout << "Enter the file name: ";
	gss.Save(EnterLine());
}

void MenuLoad(GasSupplySystem& gss) 
{
	cout << "Enter the file name: ";
	gss.Load(EnterLine());
}

bool FoundPipesExist(GasSupplySystem& gss, unordered_set<int> found_pipes)
{
	if (ObjectsExist(found_pipes)) {
		cout << "\n~FOUND PIPES~\n\n";
		gss.ShowFoundPipes(found_pipes);
		return true;
	}
	else {
		cout << "Pipes are not found!\n";
		return false;
	}
}

void MenuChangeStatusToOpposite(GasSupplySystem& gss, unordered_set<int>& id_pipes)
{
	cout << "Change status (\"1\" - yes, \"0\" - no)?: ";
	if (GetCorrectNumber(0, 1))
		gss.ChangeStatusToOpposite(id_pipes);
}

void MenuEditPipeSubpackage(GasSupplySystem& gss,
	unordered_set<int>& id_pipes, bool for_status)
{
	if (!for_status) {
		vector<string> menu = {
		"Change the status to the opposite",
		"Change the status to the \"In repair\"",
		"Change the status to the \"In working condition\"" };
		switch (ChooseActionMenu(menu, true))
		{
		case 1:
		{
			gss.ChangeStatusToOpposite(id_pipes);
			break;
		}
		case 2:
		{
			gss.ChangeStatusToRepair(id_pipes);
			break;
		}
		case 3:
		{
			gss.ChangeStatusToWork(id_pipes);
			break;
		}
		default:
			break;
		}
	}
	else
		MenuChangeStatusToOpposite(gss, id_pipes);
}

void MenuSelectionPipesByIDs(GasSupplySystem& gss,
	unordered_set<int>& id_pipes, bool for_status)
{
	cout << "Select pipes (\"1\" - yes, \"0\" - no)?: ";
	if (GetCorrectNumber(0, 1)) {
		unordered_set<int> found_pipes = SelectByIDs(id_pipes);
		if (FoundPipesExist(gss, found_pipes))
			if (found_pipes.size() > 1)
				MenuEditPipeSubpackage(gss, found_pipes, for_status);
			else
				MenuChangeStatusToOpposite(gss, found_pipes);
	}
	else
		MenuEditPipeSubpackage(gss, id_pipes, for_status);
}

void MenuEditPipePackage(GasSupplySystem& gss)
{
	unordered_set<int> found_pipes;
	bool for_status = false;
	bool with_select = true;
	vector<string> menu = {
		"Search by kilometer mark", "Search by status",
		"Select pipes"};
	switch (ChooseActionMenu(menu, true))
	{
	case 1:
	{
		cout << "Enter the Kilometer Mark: ";
		found_pipes = gss.SearchPipesByKmMark(EnterLine());
		break;
	}
	case 2:
	{
		cout << "Enter the status (\"1\"-in repair, \"0\"-in working condition): ";
		found_pipes = gss.SearchPipesByStatus(GetCorrectNumber(0, 1));
		for_status = true;
		break;
	}
	case 3:
	{
		found_pipes = gss.SearchPipesByIDs();
		with_select = false;
		break;
	}
	case 0:
	{
		break;
	}
	default:
		break;
	}

	if (FoundPipesExist(gss, found_pipes)){
		if (found_pipes.size() > 1 and with_select)
			MenuSelectionPipesByIDs(gss, found_pipes, for_status);
		else
			MenuChangeStatusToOpposite(gss, found_pipes);
	}
		
}

void MenuEditPipes(GasSupplySystem& gss)
{
	if (!gss.IsPipeObjectsEmpty()){
		vector<string> menu = { "Edit one pipe",
			"Edit pipe package", "Edit all pipes"};
		cout << "\n~ALL PIPES~\n\n";
		gss.ShortShowPipes();
		switch (ChooseActionMenu(menu, true))
		{
		case 1:
		{
			cout << "Enter ID of pipe: ";
			/*if (gss.PipeExist())*/
			gss.EditOnePipe(GetCorrectNumber(1, INT_MAX));
			break;
		}
		case 2:
		{
			MenuEditPipePackage(gss);
			break;
		}
		case 3:
		{
			gss.EditAllPipes();
			cout << "Statuses of all pipes are changed!\n";
			break;
		}
		case 0:
		{
			break;
		}
		default:
			break;
		}
	}
	else
		cout << "System has not pipes!\n";
}

bool FoundCSExist(GasSupplySystem& gss, unordered_set<int> found_stations)
{
	if (ObjectsExist(found_stations)) {
		cout << "\n~FOUND STATIONS~\n\n";
		gss.ShowFoundCS(found_stations);
		return true;
	}
	else {
		cout << "Stations are not found!\n";
		return false;
	}
}

void MenuEditCSSubpackage(GasSupplySystem& gss,
	unordered_set<int>& id_stations)
{
	cout << "\"1\" - Increase,\"0\" - Decrease: ";
	gss.EditCSPackage(id_stations, GetCorrectNumber(0, 1));
}

void MenuSelectionCSByIDs(GasSupplySystem& gss,
	unordered_set<int>& id_stations)
{
	cout << "Select stations (\"1\" - yes, \"0\" - no)?: ";
	if (GetCorrectNumber(0, 1)) {
		unordered_set<int> found_stations = SelectByIDs(id_stations);
		if (FoundCSExist(gss, found_stations))
			MenuEditCSSubpackage(gss, found_stations);
	}
	else
		MenuEditCSSubpackage(gss, id_stations);
}

void MenuEditCSPackage(GasSupplySystem& gss)
{
	unordered_set<int> found_stations;
	vector<string> menu = {
		"Search by title", "Search by percent of unused workshops",
		"Select stations" };
	bool with_select = true;
	switch (ChooseActionMenu(menu, true))
	{
	case 1:
	{
		cout << "Enter the title: ";
		found_stations = gss.SearchCSByTitle(EnterLine());
		break;
	}
	case 2:
	{
		cout << "Enter the percent of unused workshops: ";
		found_stations = gss.SearchCSByWorkshops(GetCorrectNumber(0.0, 100.0));
		break;
	}
	case 3:
	{
		found_stations = gss.SearchCSByIDs();
		with_select = false;
		break;
	}
	case 0:
	{
		break;
	}
	default:
		break;
	}

	if (FoundCSExist(gss, found_stations)) {
		if (found_stations.size() > 1 and with_select)
			MenuSelectionCSByIDs(gss, found_stations);

		else
			MenuEditCSSubpackage(gss, found_stations);
	}

}

void MenuEditCS(GasSupplySystem& gss)
{
	if (!gss.IsCSObjectsEmpty()) {
		vector<string> menu = { "Edit one station",
			"Edit station package", "Edit all stations" };
		cout << "\n~ALL STATIONS~\n\n";
		gss.ShortShowCS();
		switch (ChooseActionMenu(menu, true))
		{
		case 1:
		{
			cout << "Enter ID of station: ";
			gss.EditOneCS(GetCorrectNumber(1, INT_MAX));
			break;
		}
		case 2:
		{
			MenuEditCSPackage(gss);
			break;
		}
		case 3:
		{

			cout << "\"1\" - Increase,\"0\" - Decrease: ";
			gss.EditAllCS(GetCorrectNumber(0, 1));
			cout << "The number of unused workshops is changed!\n";
			break;
		}
		case 0:
		{
			break;
		}
		default:
			break;
		}
	}
	else
		cout << "System has not stations!\n";
}

void MenuDelete(GasSupplySystem& gss)
{
	vector<string> menu = { "Delete pipe", "Delete station" };
	switch (ChooseActionMenu(menu, true))
	{
	case 1:
	{
		if (!gss.IsPipeObjectsEmpty()) {
			cout << "Enter id of pipe: ";
			gss.DeletePipe(GetCorrectNumber(1, INT_MAX));
		}
		else
			cout << "System has not pipes!\n";
		break;
	}
	case 2:
	{
		if (!gss.IsCSObjectsEmpty()) {
			cout << "Enter id of station: ";
			gss.DeleteCS(GetCorrectNumber(1, INT_MAX));
		}
		else
			cout << "System has not stations!\n";
		break;
	}
	default:
		break;
	}
}

