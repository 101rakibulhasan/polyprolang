#include <iostream>
#include <list>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
using namespace std;

using ull = unsigned long long int;

string ConvertFileToString(string filename);
void ConvertStringToFile(string data, string filename);
string RemoveSpaceNewLine(string data);

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
        string HEAD_START = "~HEAD>";
        string HEAD_END = "<HEAD~";
        string BODY_START = "~BODY>";
        string BODY_END = "<BODY~";

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
                if (temp == HEAD_START)
                {
                    headStartIndex = i + 6;
                }
                else if (temp == BODY_START)
                {
                    bodyStartIndex = i + 6;
                }
            }
            else if (x[i] == '<')
            {
                string temp = x.substr(i, 6);
                if (temp == HEAD_END)
                {
                    headEndIndex = i - 1;
                }
                else if (temp == BODY_END)
                {
                    bodyEndIndex = i - 1;
                }
            }
        }

        // cout << headStartIndex << " " << headEndIndex << " " << bodyStartIndex << " " << bodyEndIndex << endl;

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
                while(head[j] != '\n' && j<head.size())
                {
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
            }

            if(quotationMark == 1)
            {
                res +=x[i];
            }
        }

        if(quotationMark <1 || quotationMark >2)
        {
            cout << "Invalid Values" << endl;
        }
        return res;
    }

    void SetTopic(string x) { topic = x; }
    void SetExplanation(string x) { explanation = x; }
    void SetVersion(string x) { version = x; }

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
            }

            if(quotationMark == 1)
            {
                res +=x[i];
            }
        }

        if(quotationMark <1 || quotationMark >2)
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
                if (checkSyntax == "~BEGIN>")
                {
                    i += 7;
                    while (true)
                    {
                        if (i > data.size())
                        {
                            flag_err = 1;
                            cout << "No Code End Found!" << endl;
                            tempcode = "";
                            break;
                        }

                        if (data[i] == '<')
                        {
                            checkSyntax = data.substr(i, 5);
                            if (checkSyntax == "<END~")
                            {
                                break;
                            }
                        }

                        tempcode += data[i];

                        i++;
                    }
                    tempcode = RemoveSpaceNewLine(tempcode);
                    SetCode(tempcode);
                    // cout << GetCode() << endl;
                    tempcode = "";
                }
            }
        }
    }

    void SetName(string x) { name = x; }
    void SetProlang(string x) { prolang = x; }
    void SetExtension(string x) { extension = x; }
    void SetDescription(string x) { description = x; }
    void SetCode(string x) { code = x; }

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

                        if(data[i] == '<')
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
        cout << "Can not convert String to File!";
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

void GenerateProLangChunksToFile()
{
    // NEEDS WORK LATER
}

void DebugEverything(SCANCHUNKS_CLASS scc,HEADER_CLASS hc,BODYCHUNK bc)
{
    cout << "SCAN HEAD->" << endl;
    cout << scc.GetHead() << endl;
    cout << "-----------------" << endl;
    cout << "SCAN BODY->" << endl;
    cout << scc.GetBody() << endl;
    cout << "-----------------" << endl;

    cout << "GET TOPIC->" << endl;
    cout << "\"" << hc.GetTopic() << "\"" << endl;
    cout << "-----------------" << endl;
    cout << "GET EXPLANATION->" << endl;
    cout << "\"" << hc.GetExplanation() << "\"" << endl;
    cout << "-----------------" << endl;
    cout << "GET VERSION->" << endl;
    cout << "\"" << hc.GetVersion() << "\"" << endl;
    cout << "-----------------" << endl;

    list<PROLANG_CLASS> l = bc.GetListChuncks();
    for(auto it: l)
    {
        cout << "GET NAME->" << endl;
        cout << "\"" << it.GetName() << "\"" << endl;
        cout << "-----------------" << endl;
        cout << "GET PROLANG->" << endl;
        cout << "\"" << it.GetProlang() << "\"" << endl;
        cout << "-----------------" << endl;
        cout << "GET EXTENSION->" << endl;
        cout << "\"" << it.GetExtension() << "\"" << endl;
        cout << "-----------------" << endl;
        cout << "GET DESCRIPTION->" << endl;
        cout << "\"" << it.GetDescription() << "\"" << endl;
        cout << "-----------------" << endl;
        cout << "GET CODE->" << endl;
        cout << "\"" << it.GetCode() << "\"" << endl;
        cout << "-----------------" << endl;
    }
}

int main()
{
    string filename = "test.prolang";
    string filedata = ConvertFileToString(filename);

    SCANCHUNKS_CLASS scc;
    scc.readData(filedata); 

    HEADER_CLASS hc;
    hc.FindAttributes(scc.GetHead()); 

    BODYCHUNK bc;
    bc.readBody(scc.GetBody());  

    DebugEverything(scc,hc,bc);

    return 0;
}