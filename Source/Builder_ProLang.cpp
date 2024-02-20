#include <iostream>
#include <conio.h> 
#include <list>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
using namespace std;

using ull = unsigned long long;

void CreateDoubleLine()
{
    cout<<"______________________________________________________"<<endl;
    cout<<"------------------------------------------------------"<<endl;
    cout << endl;
}

void cls()
{
    system("cls");
}

class PROLANG_CLASS
{
public:
    string name;
    string prolang;
    string extension;
    string description;
    string code;

    string GetName() { return name; }
    string GetProlang() { return prolang; }
    string GetExtension() { return extension; }
    string GetDescription() { return description; }
    string GetCode() { return code; }
};

class HEADER_CLASS
{
public:
    string topic;
    string explanation;
    string version;

    void AddHeaderAttributes()
    {
        cls(); 
        cout << "STEP 2: ADDING HEADER ATTRIBUTES" << endl;
        CreateDoubleLine();
        cout << "IMPORTANT: Write \" ~ \" in newline where it is required(~) to end input" << endl;
        cout << "Enter Topic: ";
        getline(cin,topic);
        cout << "Enter Explanantion (~): ";
        while(true)
        {
            char ch_expl = cin.get();
            if (ch_expl == '~') {
                break;
            } else {
                explanation += ch_expl;
            }           
        }
        cin.clear();
        cin.ignore();
        cout << "Enter Version: ";
        getline(cin,version);
    }

    string GetTopic() { return topic; }
    string GetExplanation() { return explanation; }
    string GetVersion() { return version; }
};

class BODYCHUNK
{
public:
    list<PROLANG_CLASS> codechunks;

    list<PROLANG_CLASS> GetListChuncks(){return codechunks;}

    void AddChunks()
    {
        while(1)
        {
            cls(); 
            cout << "STEP 3: ADDING BODY ATTRIBUTES" << endl;
            CreateDoubleLine();
            cout << "IMPORTANT: Write \" ~ \" in newline where it is required(~) to end input" << endl;

            PROLANG_CLASS pc;
            cout << "Enter File Name: " << endl;
            getline(cin,pc.name);
            cout << "Enter Programming Language: " << endl;
            getline(cin,pc.prolang);
            cout << "Enter Extension: " << endl;
            getline(cin,pc.extension);
            cout << "Enter Description: " << endl;
            getline(cin,pc.description);

            cout << "Write Code or Add Code from different folder? Press (Any Key for Write) / (A for Add)" << endl;
            char wa = 'W';
            cin >> wa;
            if(wa == 'A' || wa == 'a')
            {
                string filepath;
                cout << "Enter File Path:";
                cin >> filepath;
                ifstream inputfile(filepath);
                while(!inputfile.is_open())
                {
                    cout << "File can not be opened! or File not Found! " << endl;
                    cout << "Enter File Path:";
                    cin >> filepath;
                    inputfile.open(filepath);
                }

                ostringstream ss;
                ss << inputfile.rdbuf();
                pc.code = ss.str();
                inputfile.close();
                cout << "Chunk Added!" << endl;
                cin.clear();
                cin.ignore();
                
            }else
            {
                cout << "Write Code (~): " << endl;
                while(true)
                {
                    char ch_code = cin.get();
                    if (ch_code == '~') {
                        break;
                    } else {
                        pc.code += ch_code;
                    }           
                }
                cout << "Chunk Added!" << endl;
                cin.clear();
                cin.ignore();
            }

            codechunks.push_back(pc);
            
            char getInput = 'Y';
            cout << "Create another chunk? Press (Any key for YES) / (N for NO)" << endl;
            cout << ">> ";
            cin >> getInput;
            if(getInput == 'N' || getInput == 'n')
            {
                break;
            }
            cin.clear();
            cin.ignore();
        }
    }
};

bool validProlangData(list<PROLANG_CLASS> list, string data)
{
    if(!data.empty())
    {
        if(list.size() > 0)
        {
            return true;
        }else
        {
            cout << "Atleast One Chunk is required! " << endl;
        }
        return true;
    }else
    {
        cout << "Data can not be empty!" << endl;
        return false;
    }
}

string GenerateData(HEADER_CLASS hc, BODYCHUNK bc)
{
    string data = 

    "~HEAD>\n"
    "\n"
    "# topic: \""+hc.GetTopic()+"\"\n"
    "# explanation: \""+hc.GetExplanation()+"\"\n"
    "# version: \""+hc.GetVersion()+"\"\n"
    "\n"
    "<HEAD~\n"
    "\n"
    "~BODY>\n";
    list<PROLANG_CLASS> l = bc.GetListChuncks();
    for(auto it:l)
    {
        data += "~>>\n"
        "\n"
        "# name: \""+it.GetName()+"\"\n"
        "# prolang: \""+it.GetProlang()+"\"\n"
        "# extension: \""+it.GetExtension()+"\"\n"
        "# description: \""+it.GetDescription()+"\"\n"
        "~BEGIN>\n"
        +
        it.GetCode()
        +
        "\n"
        "<END~\n"
        "\n"
        "<<~\n"
        "\n";
    }
    
    
    data += "<BODY~";

    return data;
}

void SaveFile(string filename, string data)
{
    try
    {
        ofstream outputfile(filename);
        if (!outputfile.is_open())
            outputfile.open(filename);
        outputfile << data;
        outputfile.close();
    }
    catch (...)
    {
        cout << "Can save the file (Perhaps Folder Permission Issue?)" << endl;
    }
}

int main()
{
    string filename;
    cout << "STEP 1: CREATE FILE" << endl;
    CreateDoubleLine();
    cout << "Enter POLYPROLANG file name: ";
    getline(cin,filename);
    filename += ".prolang";
    string data;

    HEADER_CLASS head;
    head.AddHeaderAttributes();

    BODYCHUNK body;
    body.AddChunks();

    data = GenerateData(head, body);

    if(validProlangData(body.GetListChuncks(),data))
    {
        SaveFile(filename, data);
    }else
    {
        cout << "The data is invalid!" << endl;
    } 

    return 0;
}