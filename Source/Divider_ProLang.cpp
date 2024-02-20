#include <iostream>
#include <list>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
using namespace std;

using ull = unsigned long long int;

string HEAD_START = "~HEAD>";
string HEAD_END = "<HEAD~";
string BODY_START = "~BODY>";
string BODY_END = "<BODY~";
string CHUNK_START = "~>>";
string CHUNK_END = "<<~";
string CODE_START = "~BEGIN>";
string CODE_END = "<END~";

bool headStartFound;
bool bodyStartFound;
bool headEndFound;
bool bodyEndFound;
bool chunkStartFound;
bool codeStartFound;
bool chunkEndFound;
bool codeEndFound;

string ConvertFileToString(string filename);
void ConvertStringToFile(string data, string filename);
string RemoveSpaceNewLine(string data);

void cls()
{
    system("cls");
}

string RefineData(string data)
{
    for(ull i = 0; i < data.size(); i++)
    {
        if(data[i] == '~')
        {
            string temp = data.substr(i, 6);
            if ((temp == HEAD_START || temp == BODY_START) && headStartFound == false && bodyStartFound == false)
            {
                if(temp == HEAD_START)
                {
                    headStartFound = true;
                }

                if(temp == BODY_START)
                {
                    bodyStartFound = true;
                }
                
                i+=6;

                for(ull j =i; j < i+2; j++)
                {
                    if(data[j] != '\n')
                    {
                        data.insert(j,1,'\n');
                    }
                }
                
            }else
            {
                string temp = data.substr(i, 3);
                if (temp == CHUNK_START)
                {
                    chunkStartFound = true;
                }
            }
        }else if(data[i] == '<')
        {
            string temp = data.substr(i, 6);
            if (temp == HEAD_END)
            {
                if(temp == HEAD_END)
                {
                    headEndFound = true;
                }

                if(temp == BODY_END)
                {
                    bodyEndFound = true;
                }
                i+=6;
                for(ull j =i; j < i+2; j++)
                {
                    if(data[j] != '\n')
                    {
                        data.insert(j,1,'\n');
                    }
                }

            }
        }
    }
    //cout << data << endl;
    return data;
}

class MetaData
{
    string TotalCode;
};

class SCANCHUNKS_CLASS
{
public:
    string head;
    string body;

    void SetHead(string x) { head = x; }
    void SetBody(string x) { body = x; }

    void readData(string x)
    {
        int headStartIndex = -1;
        int bodyStartIndex = -1;
        int headEndIndex = -1;
        int bodyEndIndex = -1;

        string headData;
        string bodyData;

        for (ull i = 0; i < x.size(); i++)
        {
            if (x[i] == '~')
            {
                string temp = x.substr(i, 6);
                if (temp == HEAD_START && headStartIndex == -1)
                {
                    headStartIndex = i + 6;
                }
                else if (temp == BODY_START &&  bodyStartIndex == -1)
                {
                    bodyStartIndex = i + 6;
                }
            }
            else if (x[i] == '<' && x[i-1] == '\n')
            {
                string temp = x.substr(i, 6);
                if (temp == HEAD_END && headEndIndex == -1)
                {
                    headEndIndex = i - 1;
                }
                else if (temp == BODY_END)
                {
                    bodyEndIndex = i - 1;
                }
            }
        }

        //cout << headStartIndex << " " << headEndIndex << " " << bodyStartIndex << " " << bodyEndIndex << endl;

        if (headStartIndex != -1 && headEndIndex != -1 && bodyStartIndex != -1 && bodyEndIndex != -1)
        {
            for (ull i = headStartIndex; i <= headEndIndex; i++)
            {
                headData += x[i];
            }

            for (ull i = bodyStartIndex; i <= bodyEndIndex; i++)
            {
                bodyData += x[i];
            }

            headData = RemoveSpaceNewLine(headData);
            bodyData = RemoveSpaceNewLine(bodyData);

            if (bodyData.size() > 0 && headData.size() > 0)
            {
                SetHead(headData);
                SetBody(bodyData);
            }

            // DEBUGGING
            // ConvertStringToFile(head, "head.txt");
            // ConvertStringToFile(body, "body.txt");
        }
        else
        {
            cout << "CODE ERROR!" << endl;
        }
    }

    string GetHead() { return head; }
    string GetBody() { return body; }
};

class HEADER_CLASS
{
public:
    string topic;
    string explanation;
    string version;

    void FindAttributes(string head)
    {
        string TOPIC_ATT = "topic";
        string EXPLANATION_ATT = "explanation";
        string VERSION_ATT = "version";
        for (ull i = 0; i < head.size(); i++)
        {
            if (head[i] == '#')
            {
                string att;
                ull j = i+1;
                while(head[j] != ':')
                {
                    att +=head[j];
                    j++;
                }

                att = RemoveSpaceNewLine(att);
                transform(att.begin(), att.end(), att.begin(), ::tolower); 
                string unfiltered;
                while(j<head.size())
                {
                    if(head[j] == '\n'&& head[j-1] == '\"' && (head[j+1] == '#' || head[j+1] == '\n'))
                    {
                        break;
                    }
                    unfiltered +=head[j];
                    j++;
                }
                i=j;

                if(att == TOPIC_ATT)
                { 
                   SetTopic(GetAttributeValue(unfiltered));
                }else if(att == EXPLANATION_ATT)
                {
                   SetExplanation(GetAttributeValue(unfiltered));
                }else if(att == VERSION_ATT)
                {
                    SetVersion(GetAttributeValue(unfiltered));
                }else
                {
                    cout << "Unknown Attribute!" << endl;
                } 
            }
        }

        // cout << GetTopic() << endl;
        // cout << GetExplanation() << endl;
        // cout << GetVersion() << endl;
    }

    string GetAttributeValue(string x)
    {
        string res;
        int quotationMark = 0;
        for(ull i=0; i < x.size(); i++)
        {
            if(x[i] == '\"')
            {
                quotationMark++;
                i++;
                while(i < x.size())
                {
                    if(x[i] == '\"')
                    {
                        quotationMark++;
                        break;
                    }
                    res += x[i];
                    i++;
                }
            }
        }

        if(quotationMark <=1)
        {
            cout << x << endl;
            cout << "Invalid Values" << endl;
        }
        return res;
    }

    void SetTopic(string x) {
        if(topic.empty()) topic = x; 
    }
    void SetExplanation(string x) { if(explanation.empty()) explanation = x; }
    void SetVersion(string x) { if(version.empty()) version = x; }

    string GetTopic() { return topic; }
    string GetExplanation() { return explanation; }
    string GetVersion() { return version; }
};

class PROLANG_CLASS
{
public:
    string name;
    string prolang;
    string extension;
    string description;
    string code;

    string GetAttributeValue(string x)
    {
        string res;
        int quotationMark = 0;
        for(ull i=0; i < x.size(); i++)
        {
            if(x[i] == '\"')
            {
                quotationMark++;
                i++;
                while(i < x.size())
                {
                    if(x[i] == '\"' || x[i+1] == '\n')
                    {
                        if(x[i] == '\"')
                        {
                            quotationMark++;
                        }
                        break;
                    }
                    res += x[i];
                    i++;
                }
            }
        }

        //cout <<"FD: " << res << endl;

        if(quotationMark <=1)
        {
            cout << "Invalid Values" << endl;
        }
        return res;
    }

    void readChunk(string data)
    {
        string tempcode = "";

        string NAME_ATT = "name";
        string PROLANG_ATT = "prolang";
        string EXTENSION_ATT = "extension";
        string DESCRIPTION_ATT = "description";
        string CODE_START_ATT = "~BEGIN>";
        string CODE_END_ATT = "<END~";
        for(ull i=0; i < data.size(); i++)
        {
            if (data[i] == '#')
            {
                string att;
                ull j = i+1;
                while(data[j] != ':')
                {
                    att +=data[j];
                    j++;
                }

                att = RemoveSpaceNewLine(att);
                transform(att.begin(), att.end(), att.begin(), ::tolower); 
                string unfiltered;
                while(data[j] != '\n' && j<data.size())
                {
                    unfiltered +=data[j];
                    j++;
                }
                i=j;

                if(att == NAME_ATT)
                {
                   SetName(GetAttributeValue(unfiltered));
                }else if(att == PROLANG_ATT)
                {
                   SetProlang(GetAttributeValue(unfiltered));
                }else if(att == EXTENSION_ATT)
                {
                    SetExtension(GetAttributeValue(unfiltered));
                }else if(att == DESCRIPTION_ATT)
                {
                    SetDescription(GetAttributeValue(unfiltered));
                }else
                {
                    cout << "Unknown Attribute!" << endl;
                } 
            }else if(data[i] == '~')
            {
                int flag_err = 0;
                if(flag_err == 1) break;
                string checkSyntax = data.substr(i, 7);
                if (checkSyntax == CODE_START_ATT)
                {
                    i += 7;
                    while (true)
                    {
                        if (i > data.size())
                        {
                            flag_err = 1;
                            cout << "No Code End Found!" << endl;
                            break;
                        }

                        if (data[i] == '<' && data[i-1] == '\n')
                        {
                            checkSyntax = data.substr(i, 5);
                            if (checkSyntax == CODE_END_ATT)
                            {
                                break;   
                            }
                        }

                        tempcode += data[i];
                        i++;
                    }
                    tempcode = RemoveSpaceNewLine(tempcode);
                    SetCode(tempcode);
                    //cout << GetCode() << endl;
                    tempcode = "";
                }
            }
        }
    }

    void SetName(string x) { if(name.empty()) name = x; }
    void SetProlang(string x) { if(prolang.empty()) prolang = x; }
    void SetExtension(string x) { if(extension.empty()) extension = x; }
    void SetDescription(string x) { if(description.empty()) description = x; }
    void SetCode(string x) { if(code.empty()) code = x; }

    string GetName() { return name; }
    string GetProlang() { return prolang; }
    string GetExtension() { return extension; }
    string GetDescription() { return description; }
    string GetCode() { return code; }
};

class BODYCHUNK
{
public:
    list<PROLANG_CLASS> codechunks;

    void readBody(string data)
    {
        string chunk;
        int flag_err = 0;
        for(ull i=0; i < data.size(); i++)
        {
            if(flag_err == 1)
            {
                break;
            }

            if(data[i] == '~')
            {
                string checkSyntax = data.substr(i,3);
                if(checkSyntax == "~>>")
                {
                    i+=3;
                    while(true)
                    {
                        if(i > data.size())
                        {
                            flag_err = 1;
                            cout << "No Chunk End Found!" << endl;
                            break;
                        }

                        if(data[i] == '<' && data[i-1] == '\n')
                        {
                            checkSyntax = data.substr(i,3);
                            if(checkSyntax == "<<~")
                            {   
                                break;
                            }    
                        }
                        
                        chunk += data[i]; 

                        i++;
                    }

                    chunk = RemoveSpaceNewLine(chunk);
                    // ConvertStringToFile(chunk, "chunk.txt");
                
                    PROLANG_CLASS pc;
                    pc.readChunk(chunk);
                    codechunks.push_back(pc);
                    chunk = "";
                }
            }
        }
    }

    list<PROLANG_CLASS> GetListChuncks()
    {
        return codechunks;
    }

    PROLANG_CLASS GetCodeChunks(int index)
    {
        if (codechunks.size() >= 2) {
            auto it = std::next(codechunks.begin(), index);
            auto val = *it;
            return val;
        } else {
            cout << "There are not enough elements in the list to access the second one." << endl;
        }
    }
};

string ConvertFileToString(string filename)
{
    string fileread;
    try
    {
        ifstream inputfile(filename);
        if (inputfile.is_open())
        {
            ostringstream ss;
            ss << inputfile.rdbuf();
            fileread = ss.str();
            inputfile.close();
        }
        else
        {
            throw "File can not be opened! or File not Found! ";
        }
    }
    catch (char const *e)
    {
        cout << e << endl;
    }

    return fileread;
}

void ConvertStringToFile(string data, string filename)
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
        cout << "Can not convert String to File!" << endl;;
    }
}

string RemoveSpaceNewLine(string data)
{
    for (ull i = 0; i < data.size(); i++)
    {
        if (data[i] == ' ' || data[i] == '\n')
        {
            //cout<<data<<endl;
            data.erase(i, 1);
            i--;
        }
        else
            break;
    }

    for (ull i = data.size() -1; i > 0; i--)
    {
        if (data[i] == ' ' || data[i] == '\n')
        {
            data.erase(i, 1);
        }
        else
            break;
    }

    return data;
}


void DebugEverything(SCANCHUNKS_CLASS scc,HEADER_CLASS hc,BODYCHUNK bc)
{
    // cout << "SCAN HEAD->" << endl;
    // cout << scc.GetHead() << endl;
    // cout << "-----------------" << endl;
    cout << "SCAN BODY->" << endl;
    cout << scc.GetBody() << endl;
    cout << "-----------------" << endl;

    // cout << "GET TOPIC->" << endl;
    // cout << "\"" << hc.GetTopic() << "\"" << endl;
    // cout << "-----------------" << endl;
    // cout << "GET EXPLANATION->" << endl;
    // cout << "\"" << hc.GetExplanation() << "\"" << endl;
    // cout << "-----------------" << endl;
    // cout << "GET VERSION->" << endl;
    // cout << "\"" << hc.GetVersion() << "\"" << endl;
    // cout << "-----------------" << endl;

    // list<PROLANG_CLASS> l = bc.GetListChuncks();
    // for(auto it: l)
    // {
    //     cout << "GET NAME->" << endl;
    //     cout << "\"" << it.GetName() << "\"" << endl;
    //     cout << "-----------------" << endl;
    //     cout << "GET PROLANG->" << endl;
    //     cout << "\"" << it.GetProlang() << "\"" << endl;
    //     cout << "-----------------" << endl;
    //     cout << "GET EXTENSION->" << endl;
    //     cout << "\"" << it.GetExtension() << "\"" << endl;
    //     cout << "-----------------" << endl;
    //     cout << "GET DESCRIPTION->" << endl;
    //     cout << "\"" << it.GetDescription() << "\"" << endl;
    //     cout << "-----------------" << endl;
    //     cout << "GET CODE->" << endl;
    //     cout << "\"" << it.GetCode() << "\"" << endl;
    //     cout << "-----------------" << endl;
    // }
}


void ExtractCode(PROLANG_CLASS pc)
{
    string filename = pc.GetName() + "." + pc.GetExtension();
    ConvertStringToFile(pc.GetCode(), filename);
}

void ViewAvailableCodes(list<PROLANG_CLASS> l)
{
    while(true)
    {
        //cls();
        cout << "Available Codes: " << endl;
        int index = 1;
        for(auto it: l)
        {
            cout << index  <<" : "<< it.GetName()<< "." << it.GetExtension() <<endl;
            index++;
        }

        cout << "-----------------" << endl;
        cout << "Type \' 0 \' for all" << endl;
        cout << "Extract Index >> ";
        int extractIndex;
        cin >> extractIndex;

        if(extractIndex != 0)
        {
            auto it = l.begin();
            advance(it, extractIndex - 1);
            ExtractCode(*it);
            cout << "\nEXTRACTING "<< extractIndex <<"...\n" << endl;

        }else
        {
            int ind = 1;
            for(auto it: l)
            {
                ExtractCode(it);
                cout << "\nEXTRACTING "<< ind <<"...\n" << endl;
                ind++;
            }
        }

        cout << "-----------------" << endl;

        cout << "Do you want to extract another code? (y/n) >> ";
        char choice; cin >> choice;
        if(choice == 'n' || choice == 'N') break;
    }
}

int main()
{
    string filename = "goodboy.prolang";
    string filedata = ConvertFileToString(filename);
    filedata = RefineData(filedata);
    //cout << filedata << endl;

    SCANCHUNKS_CLASS scc;
    scc.readData(filedata); 

    HEADER_CLASS hc;
    hc.FindAttributes(scc.GetHead()); 

    BODYCHUNK bc;
    bc.readBody(scc.GetBody());  

    // DebugEverything(scc,hc,bc);

    ViewAvailableCodes(bc.GetListChuncks());

    return 0;
}