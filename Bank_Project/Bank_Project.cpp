#include <iostream>   // Include iostream for input/output operations.
#include <string>     // Include string for using std::string.
#include <cstdlib>    // Include cstdlib for rand() and srand() functions.   
#include <fstream>
#include <vector>
#include <iomanip>


using namespace std;


const string UsersFile = "Users.txt"; // ✅ غير الاسم
const string CleintFile = "Cleintfile.txt";

int ReadNumber(string Message)
{
    int Number = 0;  // Variable to store the user's input.

    cout << Message << endl; // Display the prompt message.
    cin >> Number;           // Read the number from the user.
    return Number;
}

string ReadString(string input)
{
    string Text;
    cout << "please enter a " << input << endl;


    getline(cin >> ws, Text);

    return Text;

}

struct stClientInfo {
    string account_number = "", Pincode = "", Name = "", phone = "";

    int account_balance = 0; bool MarkForDelete = false;

};

struct stUsersInfo {
    string UserName = "", Pincode = "";

    int pernissions = 0;
    bool MarkForDelete = false;

};

enum enUsersMenueOptions
{
    eListUsers = 1, eAddNewUser = 2,
    eDeleteUser = 3, eUpdateUser = 4,
    eFindUser = 5, eMainMenue = 6
};

enum enUserspermissions
{
    eAll = -1, pListClients = 1, pAddNewClient = 2, pDeleteClient = 4,
    pUpdateClients = 8, pFindClient = 16, pTranactions = 32, pManageUsers = 64
};

vector<string> SplitString(string String_1, string delimiter)
{
    vector<string> vstring;

    size_t position = 0;
    string sword;



    //القيمة string::npos تعني "لم يتم العثور".
    while ((position = String_1.find(delimiter)) != string::npos)
    {
        sword = String_1.substr(0, position);
        if (sword != "")
        {
            vstring.push_back(sword);

        }
        String_1.erase(0, position + delimiter.length());
    }
    if (String_1 != "")
    {
        vstring.push_back(String_1);


    }

    return vstring;
}

stUsersInfo User;

bool CheckPermissions(enUserspermissions permissions)
{
    if (User.pernissions == enUserspermissions::eAll)
    {
        return true;
    }
    else if ((permissions & User.pernissions) == permissions)
    {

        return true;

    }

    else
    {
        return false;
    }

}


void ShowAccessDeniedMessage()
{
    cout << "\n------------------------------------\n";
    cout << "Access Denied, \nYou dont Have Permission To Do this, \nPlease Conact Your Admin."; 
    cout << "\n------------------------------------\n";
}

//5555555555555555555


stClientInfo ConvertLineToRecord(string Line, string delimiter)
{
    stClientInfo cleint;
    vector<string>VRecord = SplitString(Line, "#//#");

    cleint.account_number = VRecord[0];
    cleint.Pincode = VRecord[1];
    cleint.Name = VRecord[2];
    cleint.phone = VRecord[3];
    cleint.account_balance = stoi(VRecord[4]);
    return cleint;

}




vector<stClientInfo> LoadDataFromFileToVector(string filename)
{
    vector< stClientInfo>vcleints;
    fstream Myfile;


    Myfile.open(filename, ios::in);//read mode 
    if (Myfile.is_open())
    {
        stClientInfo Cleint;
        string Line;

        while (getline(Myfile, Line))
        {
            Cleint = ConvertLineToRecord(Line, "#//#");
            vcleints.push_back(Cleint);

        }

        Myfile.close();

    }

    return vcleints;
}


string ConvertRecordToLine(stClientInfo client, string seperater = "#//#")
{
    string Line = "";
    Line += client.account_number + seperater;
    Line += client.Pincode + seperater;
    Line += client.Name + seperater;
    Line += client.phone + seperater;
    Line += to_string(client.account_balance);


    return Line;

}


stClientInfo ChangeClientInfo(string accountnumbersearch)
{
    stClientInfo client;
    client.account_number = accountnumbersearch;
    cout << "please Enter your Pincode \n";
    getline(cin >> ws, client.Pincode);
    cout << "please Enter your Name \n";
    getline(cin, client.Name);
    cout << "please Enter your phone \n";
    getline(cin, client.phone);
    cout << "please Enter your account_balance \n";
    cin >> client.account_balance;

    return client;
}



vector< stClientInfo> SaveVectorToFile(string FileName, vector< stClientInfo>& vcleintst)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out);
    string line;
    if (MyFile.is_open())
    {
        for (stClientInfo C : vcleintst)
        {
            if (C.MarkForDelete == false)
            {
                //we only write records that are not marked for delete.
                line = ConvertRecordToLine(C);
                MyFile << line << endl;
            }

        }
        MyFile.close();
    }
    return vcleintst;
}


bool isaccountfound(stClientInfo& Cleint, vector< stClientInfo>vcleints, string accountnumbersearch)
{

    for (stClientInfo C : vcleints)
    {
        if (C.account_number == accountnumbersearch)
        {
            Cleint = C;

            return true;
        }

    }
    return false;
}


bool MarkAsDelete(string accountnumbersearch, vector< stClientInfo>& vcleints)//FindClientByAccountNumber
{



    for (stClientInfo& C : vcleints)
    {

        if (C.account_number == accountnumbersearch)
        {

            C.MarkForDelete = true;

            return true;
        }


    }
    return false;
}
//.............................
void PrintClientCard(stClientInfo Client)
{
    cout << "\nThe following are the client details:\n";
    cout << "\nAccout Number: " << Client.account_number;
    cout << "\nPin Code : " << Client.Pincode;
    cout << "\nName : " << Client.Name;
    cout << "\nPhone : " << Client.phone;
    cout << "\nAccount Balance: " << Client.account_balance;

}


bool DeleteClientByAccountNumber(string accountnumbersearch, vector<stClientInfo>& vcleints)
{
    stClientInfo Client;
    char choice = 'n';

    if (isaccountfound(Client, vcleints, accountnumbersearch))
    {
        PrintClientCard(Client);

        cout << "\n\nAre you sure you want delete this client? y/n ? ";
        cin >> choice;

        if (toupper(choice) == 'Y')
        {
            MarkAsDelete(accountnumbersearch, vcleints);
            SaveVectorToFile(CleintFile, vcleints);

            cout << "\n\nClient Deleted Successfully.";
            return true;
        }
        else
        {
            cout << "\nDeletion cancelled.";
            return false;
        }
    }
    else
    {
        cout << "\nClient with Account Number (" << accountnumbersearch << ") is Not Found!";
        return false;
    }
}


stClientInfo ReadClientInfo()
{
    stClientInfo client;
    cout << "please Enter your Account number \n";
    getline(cin >> ws, client.account_number);
    cout << "please Enter your Pincode \n";
    getline(cin, client.Pincode);
    cout << "please Enter your Name \n";
    getline(cin, client.Name);
    cout << "please Enter your phone \n";
    getline(cin, client.phone);
    cout << "please Enter your account_balance \n";
    cin >> client.account_balance;

    return client;
}


//add functions 
void AddToFile(string FileName, string Line)
{
    fstream CleintFile;

    CleintFile.open(FileName, ios::out | ios::app);
    if (CleintFile.is_open())
    {

        CleintFile << Line << endl;

        CleintFile.close();
    }




}



void AddNewCleint()
{
    cout << "please enter cleint data\n";
    cout << "*************************\n";
    cout << endl;
    stClientInfo client = ReadClientInfo();

    AddToFile(CleintFile, ConvertRecordToLine(client, "#//#"));
}

void AddCleint()
{
    char choice = 'y';

    while (choice == 'y' || choice == 'Y')
    {
        // system("cls");
        AddNewCleint();

        cout << "\nClient Added Successfully, do  you want to addmore clients ? Y / N ? \n";
        cin >> choice;
    }
}

//-------------------------------------
// ====================================
//update client function

bool UpdateClientByAccountNumber(string accountnumbersearch, vector< stClientInfo>& vcleints)
{
    stClientInfo Client;
    char choice = 'n';
    if (isaccountfound(Client, vcleints, accountnumbersearch))
    {
        PrintClientCard(Client);

        cout << "\n\nAre you sure you want update this client? y/n ? ";
        cin >> choice;
        if (toupper(choice) == 'Y')
        {

            for (stClientInfo& C : vcleints)
            {
                if (C.account_number == accountnumbersearch)
                {

                    C = ChangeClientInfo(accountnumbersearch);
                    break;

                }


            }


            SaveVectorToFile(CleintFile, vcleints);



            cout << "\n\nClient update Successfully.";
            return true;
        }
        else
        {
            cout << "\nUpdate cancelled.\n";

            return false;

        }

    }
    else
    {
        cout << "\nClient with Account Number (" << accountnumbersearch << ") is Not Found!";

        return false;


    }


}


// =======================================
//print functions

void PrintCleintRecord(stClientInfo cleint)
{
    cout << "|" << setw(15) << left << cleint.account_number;

    cout << "|" << setw(10) << left << cleint.Pincode;
    cout << "|" << setw(40) << left << cleint.Name;
    cout << "|" << setw(12) << left << cleint.phone;
    cout << "|" << setw(12) << left << cleint.account_balance;


}

void PrintAllClientsData(vector <stClientInfo> vClients)
{
    cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";


    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n";
    cout << "|" << left << setw(15) << "Accout Number";
    cout << "|" << left << setw(10) << "Pin Code";
    cout << "|" << left << setw(40) << "Client Name";
    cout << "|" << left << setw(12) << "Phone";
    cout << "|" << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________";

    cout << "_________________________________________\n" << endl;
    for (stClientInfo Client : vClients)
    {
        PrintCleintRecord(Client);
        cout << endl;
    }
    cout <<
        "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;




}
//--------------------------------------------------------------------

//perform function:

enum enMainMenueOptions
{
    eListClients = 1, eAddNewClient = 2,
    eDeleteClient = 3, eUpdateClient = 4,
    eFindClient = 5, eTransactions = 6, eManageUsers = 7, eExit = 8
};

enum enTransitionsOptions
{
    enDeposit = 1, enWithdraw = 2,
    enTotalBalances = 3, enTransactionMenue = 4,
};


short ReadMainMenueOption(string message)
{
    short choice = 0;
    cin >> choice;
    return choice;
}



//show screens of perform function (cases functions) . >>>>

void PerfromMainMenueOption(enMainMenueOptions MainMenueOptions);

void ShowMainScreen()
{


    system("cls");
    cout << "===========================================\n";
    cout << "\t\tMain Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Show Client List.\n";
    cout << "\t[2] Add New Client.\n";
    cout << "\t[3] Delete Client.\n";
    cout << "\t[4] Update Client Info.\n";
    cout << "\t[5] Find Client.\n";
    cout << "\t[6] Transactions.\n";
    cout << "\t[7] manage users.\n";
    cout << "\t[8] Logout.\n";

    cout << "===========================================\n";
    PerfromMainMenueOption((enMainMenueOptions)ReadMainMenueOption("Choose what do you want to do? [1 to 7]? "));


}




void ShowAddNewClientsScreen()
{
    if (!CheckPermissions(enUserspermissions::pAddNewClient))
    {
        ShowAccessDeniedMessage();
        return;
    }
    cout << "\n-----------------------------------\n";
    cout << "\tAdd New Clients Screen";
    cout << "\n-----------------------------------\n";

    AddCleint();
}

void ShowDeleteClientScreen()
{
    if (!CheckPermissions(enUserspermissions::pDeleteClient))
    {
        ShowAccessDeniedMessage();
        return;
    }
    cout << "\n-----------------------------------\n";
    cout << "\tDelete Client Screen";
    cout << "\n-----------------------------------\n";
    vector <stClientInfo> vClients = LoadDataFromFileToVector(CleintFile);


    string accountnumbersearch = ReadString("account number");

    DeleteClientByAccountNumber(accountnumbersearch, vClients);
}

void ShowUpdateClientScreen()
{
    if (!CheckPermissions(enUserspermissions::pUpdateClients))
    {
        ShowAccessDeniedMessage();
        return;
    }
    vector <stClientInfo> vClients = LoadDataFromFileToVector(CleintFile);

    string accountnumbersearch = ReadString("account number");
    UpdateClientByAccountNumber(accountnumbersearch, vClients);
}

void ShowAllClientsScreen()
{
    if (!CheckPermissions(enUserspermissions::pListClients))
    {
        ShowAccessDeniedMessage();
        return;
    }
    vector <stClientInfo> vClients = LoadDataFromFileToVector(CleintFile);


    PrintAllClientsData(vClients);

}

void ShowFindClientScreen()
{
    if (!CheckPermissions(enUserspermissions::pFindClient))
    {
        ShowAccessDeniedMessage();
        return;
    }
    vector <stClientInfo> vClients = LoadDataFromFileToVector(CleintFile);

    string accountnumbersearch = ReadString("account number");
    stClientInfo Cleint;
    if (isaccountfound(Cleint, vClients, accountnumbersearch))
    {
        PrintClientCard(Cleint);
    }
    else {
        cout << "clien is not found" << endl;
    }
}
void ShowManageUsersScreen();
void ShowEndScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tProgram Ends :-)";
    cout << "\t\t\t\t\t WELCOME !\t\t\n";
    cout << "\n-----------------------------------\n";

}

void PerfromTransitionsOptions(enTransitionsOptions TransitionsOptions);



void ShowTransitionsScreen()
{
    if (!CheckPermissions(enUserspermissions::pTranactions))
    {
        ShowAccessDeniedMessage();
        return;
    }
    system("cls");    //بدل ما نحطها في ال GoBackToTarnsScreen function 

    cout << "\n-----------------------------------\n";
    cout << "\tTransitions Screen";
    cout << "\n-----------------------------------\n";
    cout << "\t[1] Deposit.\n";
    cout << "\t[2] Withdraw.\n";
    cout << "\t[3] Total Balances.\n";
    cout << "\t[4] Main Menue.\n";
    cout << "===========================================\n";

    PerfromTransitionsOptions((enTransitionsOptions)ReadMainMenueOption("Choose what do you want to do? [1 to 4]? "));


}

void GoBackToMainMenue()
{
    cout << "\n\nPress any key to go back to Main Menue...";
    system("pause>0");
    ShowMainScreen();
}

void PerfromMainMenueOption(enMainMenueOptions MainMenueOptions)
{
    switch (MainMenueOptions)
    {
    case enMainMenueOptions::eAddNewClient:
    {
        system("cls");
        ShowAddNewClientsScreen();
        GoBackToMainMenue();
        break;
    }

    case enMainMenueOptions::eDeleteClient: {
        system("cls");
        ShowDeleteClientScreen();
        GoBackToMainMenue();
        break;

    }

    case enMainMenueOptions::eUpdateClient: {
        system("cls");
        ShowUpdateClientScreen();
        GoBackToMainMenue();

        break;
    }

    case enMainMenueOptions::eListClients:
    {
        system("cls");
        ShowAllClientsScreen();
        GoBackToMainMenue();
        break;

    }

    case enMainMenueOptions::eFindClient:
    {
        system("cls");
        ShowFindClientScreen();
        GoBackToMainMenue();
        break;

    }

    //Bank Extension 1 in course 7

    case enMainMenueOptions::eTransactions:
    {
        system("cls");
        ShowTransitionsScreen();
        GoBackToMainMenue();
        break;


    }

    //Bank Extension 2 in course 8

    case enMainMenueOptions::eManageUsers:
    {
        system("cls");
        ShowManageUsersScreen();
        GoBackToMainMenue();
        break;


    }
    case enMainMenueOptions::eExit:
    {
        system("cls");
        ShowEndScreen();

        break;
    }

    




    }
}



//..............................
int CalculateDepositBalance(stClientInfo Client, int DepositAmount)
{



    // Client.account_balance = Client.account_balance + DepositAmount;

    return  Client.account_balance + DepositAmount;


}
void PrintCleintTransactions(stClientInfo cleint)
{
    cout << "|" << setw(15) << left << cleint.account_number;
    cout << "|" << setw(40) << left << cleint.Name;
    cout << "|" << setw(12) << left << cleint.account_balance;


}

void PrintAllClientsTransaction(vector <stClientInfo> vClients)
{
    cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";


    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n";
    cout << "|" << left << setw(15) << "Accout Number";
    cout << "|" << left << setw(40) << "Client Name";
    cout << "|" << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________";

    cout << "_________________________________________\n" << endl;
    for (stClientInfo Client : vClients)
    {
        PrintCleintTransactions(Client);
        cout << endl;
    }
    cout <<
        "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;




}
//--------------------------------------------------------------------
double CalculateAllBalances(vector <stClientInfo> vClients)
{
    double Sum = 0;

    for (stClientInfo& C : vClients)
    {

        Sum = C.account_balance + Sum;


    }
    return Sum;
}

bool DepositeBalance(string accountnumbersearch, int DepositAmount, vector <stClientInfo>& vClients)
{
    char choice = 'N';

    cout << "\n\nAre you sure to perform amount to this account ? y/n ? ";
    cin >> choice;

    if (toupper(choice) == 'Y')
    {

        for (stClientInfo& C : vClients)
        {
            if (C.account_number == accountnumbersearch)
            {

                C.account_balance = CalculateDepositBalance(C, DepositAmount);
                break;

            }


        }


        SaveVectorToFile(CleintFile, vClients);



        cout << "\n\nbalance proccess Successfully.";
        return true;
    }
    else
    {
        cout << "\proccess cancelled.\n";

        return false;

    }

}

void ShowDepositScreen()
{


    cout << "\n-----------------------------------\n";
    cout << "\Deposit Screen";
    cout << "\n-----------------------------------\n";
    vector <stClientInfo> vClients = LoadDataFromFileToVector(CleintFile);
    stClientInfo Client;
    string accountnumbersearch = ReadString("account number");

    while (!isaccountfound(Client, vClients, accountnumbersearch))
    {
        cout << "client who has the account number : " << accountnumbersearch << " is not found" << endl;

        accountnumbersearch = ReadString("account number");
    }

    PrintClientCard(Client);
    cout << endl;

    DepositeBalance(accountnumbersearch, ReadNumber("\n\nplease enter Deposit Amount\n"), vClients);


}

void ShowWithdrawScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\Withdraw Screen";
    cout << "\n-----------------------------------\n";
    vector <stClientInfo> vClients = LoadDataFromFileToVector(CleintFile);
    stClientInfo Client;
    string accountnumbersearch = ReadString("account number");

    while (!isaccountfound(Client, vClients, accountnumbersearch))
    {
        cout << "client who has the account number : " << accountnumbersearch << " is not found" << endl;

        accountnumbersearch = ReadString("account number");
    }

    PrintClientCard(Client);
    cout << endl;
    int  Amount = ReadNumber("\n\nplease enter an amount to with draw it\n");
    // validating

    while (Amount > Client.account_balance)
    {

        cout << "\nAmount Exceeds the balance, you can withdraw up to : " << Client.account_balance << endl;
        cout << "Please enter another amount\n ";
        cin >> Amount;


    }
    DepositeBalance(accountnumbersearch, Amount * -1, vClients);



}

void ShowAllTranscAtionsScreen()
{
    vector <stClientInfo> vClients = LoadDataFromFileToVector(CleintFile);
    PrintAllClientsTransaction(vClients);
    cout << "Sum of all balances is = " << CalculateAllBalances(vClients) << endl;

}

void GoBackToTransitionsScreen()
{

    cout << "\n\nPress any key to go back to Main Menue...";
    system("pause>0");
    ShowTransitionsScreen();
}

void PerfromTransitionsOptions(enum enTransitionsOptions TransitionsOptions)
{
    switch (TransitionsOptions)
    {
    case enTransitionsOptions::enDeposit:
    {
        system("cls");
        ShowDepositScreen();

        GoBackToTransitionsScreen();
        break;
    }

    case enTransitionsOptions::enWithdraw: {
        system("cls");
        ShowWithdrawScreen();
        GoBackToTransitionsScreen();
        break;

    }

    case enTransitionsOptions::enTotalBalances: {
        system("cls");
        ShowAllTranscAtionsScreen();
        GoBackToTransitionsScreen();

        break;
    }

    case enTransitionsOptions::enTransactionMenue:
    {
        system("cls");
        ShowMainScreen();
        break;

    }



    }
}




//777777777777777777777777777777777





//---------extention 2 for bank project functions ------------------- :

string ConvertRecordToLineUsers(stUsersInfo User, string seperater = "#//#")
{
    string Line = "";
    Line += User.UserName + seperater;
    Line += User.Pincode + seperater;
    Line += to_string(User.pernissions);


    return Line;

}

bool MarkAsDeleteUsers(string accountnumbersearch, vector<stUsersInfo>& vUser)//FindClientByAccountNumber
{



    for (stUsersInfo& C : vUser)
    {

        if (C.UserName == accountnumbersearch)
        {

            C.MarkForDelete = true;

            return true;
        }


    }
    return false;
}
//.............................


vector< stUsersInfo> SaveVectorToFileUsers(string FileName, vector<stUsersInfo>& vUser)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out);
    string line;
    if (MyFile.is_open())
    {
        for (stUsersInfo C : vUser)
        {
            if (C.MarkForDelete == false)
            {
                //we only write records that are not marked for delete.
                line = ConvertRecordToLineUsers(C);
                MyFile << line << endl;
            }

        }
        MyFile.close();
    }
    return vUser;
}

bool isaccountfoundUsers(stUsersInfo& User, vector< stUsersInfo>vUser, string username, string pincode = "")
{

    for (stUsersInfo U : vUser)
    {
        if (pincode == "")
        {
            if (U.UserName == username)
            {
                User = U;
                return true;
            }
        }
        else if (U.UserName == username && U.Pincode == pincode)
        {
            User = U;

            return true;
        }

    }
    return false;
}


void PrintUserCard(stUsersInfo User)
{
    cout << "\nThe following are the User details:\n";
    cout << "\nUserName : " << User.UserName;
    cout << "\nPin Code : " << User.Pincode;


}



bool DeleteUserByUserName(string UserNameSearch, vector<stUsersInfo>& vUser)
{
    stUsersInfo User;

    char choice = 'n';

    if (isaccountfoundUsers(User, vUser, UserNameSearch))
    {
        PrintUserCard(User);

        cout << "\n\nAre you sure you want delete this client? y/n ? ";
        cin >> choice;

        if (toupper(choice) == 'Y')
        {
            MarkAsDeleteUsers(UserNameSearch, vUser);
            SaveVectorToFileUsers(UsersFile, vUser);

            cout << "\n\nUser Deleted Successfully./n";
            return true;
        }
        else
        {
            cout << "\nDeletion cancelled.";
            return false;
        }
    }
    else
    {
        cout << "\nuser with Account Number (" << UserNameSearch << ") is Not Found!";
        return false;
    }
}


stUsersInfo ChangeUserInfo(string UserNameSearch)
{
    stUsersInfo User;

    cout << "please Enter your username \n";
    getline(cin >> ws, User.UserName);

    cout << "please Enter your Pincode \n";
    getline(cin, User.Pincode);

    cout << "please Enter your permissions \n";
    cin >> User.pernissions;

    return User;
}


bool UpdateUserByUserName(string UserNameSearch, vector<stUsersInfo>& vUser)
{
    stUsersInfo User;
    char choice = 'n';
    if (isaccountfoundUsers(User, vUser, UserNameSearch))
    {
        PrintUserCard(User);

        cout << "\n\nAre you sure you want update this client? y/n ? ";
        cin >> choice;
        if (toupper(choice) == 'Y')
        {

            for (stUsersInfo& C : vUser)
            {
                if (C.UserName == UserNameSearch)
                {

                    C = ChangeUserInfo(UserNameSearch);
                    break;

                }


            }


            SaveVectorToFileUsers(UsersFile, vUser);



            cout << "\n\nClient update Successfully.";
            return true;
        }
        else
        {
            cout << "\nUpdate cancelled.\n";

            return false;

        }

    }
    else
    {
        cout << "\nClient with Account Number (" << UserNameSearch << ") is Not Found!";

        return false;


    }


}


/*bool isaccountfound(stClientInfo& Cleint, vector< stClientInfo>vcleints,string accountnumbersearch)
{

    for (stClientInfo C : vcleints)
    {
        if (C.account_number == accountnumbersearch)
        {
            Cleint = C;

            return true;
        }

    }
    return false;
}

*/


stUsersInfo ConvertLineToRecordUsers(string Line, string delimiter)
{
    stUsersInfo User;
    vector<string>VRecord = SplitString(Line, "#//#");

    User.UserName = VRecord[0];
    User.Pincode = VRecord[1];
    User.pernissions = stoi(VRecord[2]);
    return User;

}

vector <stUsersInfo> LoadDataFromFileToVectorUsers(string filename)
{
    vector< stUsersInfo>vUsers;
    fstream Myfile;


    Myfile.open(filename, ios::in);//read mode 
    if (Myfile.is_open())
    {
        stUsersInfo User;
        string Line;

        while (getline(Myfile, Line))
        {
            User = ConvertLineToRecordUsers(Line, "#//#");
            vUsers.push_back(User);

        }

        Myfile.close();

    }

    return vUsers;



}




bool isUserExist(stUsersInfo& User, string username, string pincode="")
{
    vector <stUsersInfo> vUser = LoadDataFromFileToVectorUsers(UsersFile);

    if (isaccountfoundUsers(User, vUser, username, pincode))
    {
        //PrintUserCard(User);
        return true;
    }
    else {
        // cout << "clien is not found" << endl;
        return false;

    }

}

//print functions

void PrintUserRecord(stUsersInfo User)
{
    cout << "|" << setw(40) << left << User.UserName;
    cout << "|" << setw(10) << left << User.Pincode;
    cout << "|" << setw(10) << left << User.pernissions;



}

void PrintAllUsersData(vector <stUsersInfo> vUsers)
{
    cout << "\n\t\t\t\t\tClient List (" << vUsers.size() << ") User(s).";


    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n";
    cout << "|" << left << setw(40) << "User Name";
    cout << "|" << left << setw(10) << "Pin Code";
    cout << "|" << left << setw(10) << "permissions";

    cout << "\n_______________________________________________________";

    cout << "_________________________________________\n" << endl;
    for (stUsersInfo User : vUsers)
    {
        PrintUserRecord(User);
        cout << endl;
    }
    cout <<
        "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;




}
//------------------
//add user functions
int ReadUserPermissions()
{
    int permissions = 0;
    char Answer = 'y';
       
    cout << "\nDo you want to give full access? y/n? ";
    cin >> Answer;
    if (toupper(Answer) == 'Y')
    {
        return -1;
    }


    cout << "\nDo you want to give access to : \n ";
    cout << "\nShow Client List? y/n? ";
    cin >> Answer;
    if (toupper(Answer) == 'Y')
    {

        permissions = permissions + enUserspermissions::pListClients;

    }

    cout << "\nDelete Client ? y/n? ";
    cin >> Answer;
    if (toupper(Answer) == 'Y')
    {

        permissions = permissions + enUserspermissions::pDeleteClient;

    }

    cout << "\nAdd New Client ? y/n? ";
    cin >> Answer;
    if (toupper(Answer) == 'Y')
    {

        permissions = permissions + enUserspermissions::pAddNewClient;

    }

    cout << "\nUpdate Client ? y/n? ";
    cin >> Answer;
    if (toupper(Answer) == 'Y')
    {

        permissions = permissions + enUserspermissions::pUpdateClients;

    }

   
    cout << "\nFind Client ? y/n";
    cin >> Answer;
    if (toupper(Answer) == 'Y')
    {

        permissions = permissions + enUserspermissions::pFindClient;

    }

    cout << "\nTransactions Clients ? y/n";
    cin >> Answer;
    if (toupper(Answer) == 'Y')
    {

        permissions = permissions + enUserspermissions::pTranactions;

    }

    cout << "\nManage Users ? y/n";
    cin >> Answer;
    if (toupper(Answer) == 'Y')
    {

        permissions = permissions + enUserspermissions::pManageUsers;

    }

    return permissions;
}

stUsersInfo ReadUserInfo()
{
    stUsersInfo User;

    cout << "please Enter your UserName \n";
    getline(cin >> ws, User.UserName);
    while (isUserExist(User, User.UserName))
    {
        cout << "\n\n user with " << User.UserName << "username is already exsit try anothor username \n\n";
    }
    cout << "please Enter your Pincode \n";
    getline(cin, User.Pincode);
    cout << "please Enter your permissions \n";
    //cin >>User.pernissions;
    User.pernissions = ReadUserPermissions();

    return User;
}



void AddToFileUsers(string FileName, string Line)
{
    fstream UsersFile;

    UsersFile.open(FileName, ios::out | ios::app);
    if (UsersFile.is_open())
    {

        UsersFile << Line << endl;

        UsersFile.close();
    }




}

void AddNewUser()
{
    cout << "please enter cleint data\n";
    cout << "*************************\n";
    cout << endl;
    stUsersInfo User = ReadUserInfo();

    AddToFileUsers(UsersFile, ConvertRecordToLineUsers(User, "#//#"));
}


void AddUser()
{
    char choice = 'y';

    while (choice == 'y' || choice == 'Y')
    {
        // system("cls");
        AddNewUser();

        cout << "\nUser Added Successfully, do  you want to addmore clients ? Y / N ? \n";
        cin >> choice;
    }
}





//--------------------------------------------
void ShowAllUsersScreen()
{
    vector <stUsersInfo> vUsers = LoadDataFromFileToVectorUsers(UsersFile);


    PrintAllUsersData(vUsers);

}

void ShowAddNewUsersScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tAdd New Users Screen";
    cout << "\n-----------------------------------\n";

    AddUser();
}

void ShowDeleteUserScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tDelete Client Screen";
    cout << "\n-----------------------------------\n";
    vector <stUsersInfo> vUsers = LoadDataFromFileToVectorUsers(UsersFile);


    string usernamesearch = ReadString("user name");

    DeleteUserByUserName(usernamesearch, vUsers);
}

void ShowUpdateUserScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tUpdate User Screen";
    cout << "\n-----------------------------------\n";
    vector <stUsersInfo> vUsers = LoadDataFromFileToVectorUsers(UsersFile);

    string usernamesearch = ReadString("user name");
    UpdateUserByUserName(usernamesearch, vUsers);
}

void ShowFindUserScreen()
{
    vector <stUsersInfo> vUsers = LoadDataFromFileToVectorUsers(UsersFile);

    string usernamesearch = ReadString("user name");
    stUsersInfo User;
    if (isaccountfoundUsers(User, vUsers, usernamesearch))
    {
        PrintUserCard(User);
    }
    else 
    {
        cout << "User is not found" << endl;
    }
}


void GoBackToManageUsersScreen()
{

    cout << "\n\nPress any key to go back to Main Menue...";
    system("pause>0");
    ShowManageUsersScreen();
}

void PerfromUsersMenueOption(enUsersMenueOptions UsersMenueOptions)
{
    switch (UsersMenueOptions)

    {
    case enUsersMenueOptions::eListUsers:
    {
        system("cls");
        ShowAllUsersScreen();
        GoBackToManageUsersScreen();
        break;

    }

    case enUsersMenueOptions::eAddNewUser: {

        system("cls");
        ShowAddNewUsersScreen();
        GoBackToManageUsersScreen();
        break;
    }

    case enUsersMenueOptions::eDeleteUser: {
        system("cls");
        ShowDeleteUserScreen();
        GoBackToManageUsersScreen();
        break;
    }

    case enUsersMenueOptions::eUpdateUser:
    {
        system("cls");
        ShowUpdateUserScreen();
        GoBackToManageUsersScreen();
        break;

    }
                                        
    case enUsersMenueOptions::eFindUser: {
        system("cls");
        ShowFindUserScreen();
        GoBackToManageUsersScreen();
        break;

    }
                                         
    case enUsersMenueOptions::eMainMenue: {
        system("cls");
        GoBackToMainMenue();
        break;

    }




                                        cout << "\t\t\t\t\t WELCOME !\t\t\n";

                                        



    }
}

void ShowManageUsersScreen()
{
    if (!CheckPermissions(enUserspermissions::pManageUsers))
    {
        ShowAccessDeniedMessage();
        return;
    }

    system("cls");
    cout << "===========================================\n";
    cout << "\t\tMain Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Show Users List.\n";
    cout << "\t[2] Add New User.\n";
    cout << "\t[3] Delete User.\n";
    cout << "\t[4] Update User Info.\n";
    cout << "\t[5] Find User.\n";
    cout << "\t[6] Back To Main Menue.\n";
    cout << "===========================================\n";
    PerfromUsersMenueOption((enUsersMenueOptions)ReadMainMenueOption("Choose what do you want to do? [1 to 6]? "));


}


void Login()
{
    bool check = true;
    string username;
    string pincode;
    while (check)
    {
        cout << "plaese enter your username : \n";
        cin >> username;
        cout << "plaese enter your Pin code : \n";
        cin >> pincode;

        if (!isUserExist(User, username, pincode))
        {

            cout << "invalid username/password  \n";
            check = true;   // استمر في الloop
        }
        else
        {
            cout << "\n valid User  \n";
            check = false;  // ✅ لما ينجح اللوقين، وقف الloop
        }

    }
    ShowMainScreen();
    
}

int main()
{



    Login();

    system("pause>0");


    return 0;
}

//*******************************************************


