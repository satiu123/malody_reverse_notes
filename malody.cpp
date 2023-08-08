#include<iostream>
#include<cstring>
#include <fstream>
using namespace std;
ifstream InFile;//输入文件流
fstream OutFile;//输出文件流
int Pos1;//note部分的开始位置
int Pos2;//note部分的结束位置
class note//note类
{
    public:
        int beat[3];
        int endbeat[3];
        int column,begin,end;
        string noteInformation;
        string noteInformationAll;
        note():beat{},endbeat{},column(0),noteInformation{},noteInformationAll{}{};
        string getNote()
        {
            string noteInformation;
            noteInformation="{\"beat\":["+to_string(beat[0])+","+to_string(beat[1])+","+
                            to_string(beat[2])+"],\"column\":"+to_string(column)+"}";
            return noteInformation;
        }
};
//打开文件
void OpenFile(string InFilePosition,string OutFilePosition)
{
    cout<<"请输入输入文件位置:(绝对位置)"<<endl;
    getline(cin,InFilePosition);
    OutFilePosition=InFilePosition.substr(InFilePosition.find_last_of("\\")+1);
    InFile.open(InFilePosition,ios::in);
    OutFile.open(OutFilePosition,ios::app|ios::out);
    if(!InFile.is_open()||!OutFile.is_open())
    {
        cout<<"打开文件失败！"<<endl;
        exit(0);
    }
    cout<<"默认保存到源文件目录下的："<<OutFilePosition<<endl;
}
void CloseFile()//关闭文件
{
    InFile.close();
    OutFile.close();
    cout<<"文件已保存！"<<endl;
}
//将noteInformationAll中的note部分之前的部分写入文件
void beforeNoteInformation(note&notes)
{
    Pos1=notes.noteInformationAll.find("note",0);
    Pos2=notes.noteInformationAll.find("sound",Pos1+1);
    char sb[Pos1];
    InFile.seekg(0,ios::beg);
    InFile.get(sb,Pos1);
    OutFile<<sb;
}
//获取小节数
int getNum(string n,int Pos)
{
    int i=0;
    int num;
    string ns;
    Pos+=strlen("\"beat\":[");
    while(n[Pos+i]!=',')
    {
        ns+=n[Pos+i];
        i++;
    }
    num=atoi(ns.c_str());
    return num;
}
void getNoteBegin_And_End(note&notes)//获取note部分的开始和结束位置
{
    int i=0;
    int Pos,endPos;
    string n;
    notes.begin=getNum(notes.noteInformation,strlen("\"note\":[{")-1);
    InFile.seekg(Pos2-100,ios::beg);
    getline(InFile,n);
    endPos=n.find_last_of("\"beat\":");
    Pos=n.rfind("\"beat\":",endPos-1);
    Pos=n.rfind("\"beat\":",Pos-1);
    notes.end=getNum(n,Pos);
    cout<<"note开始于第"<<notes.begin+1<<"小节"<<endl
        <<"note结束于第"<<notes.end<<"小节"<<endl;
}
//输入开始和结束小节
void inputBegin_And_End(note&notes,int&begin,int&end)
{
    while(1)
    {
        cout<<"请输入开始小节："<<endl;
        cin>>begin;
        cout<<"请输入结束小节："<<endl;
        cin>>end;
        if(begin<notes.begin||end>notes.end||begin>end)
        {
            cout<<"输入错误！,请重新输入"<<endl;
            continue;
        }
        else 
        {
            begin--;
            break;
        }
    }
}
//判断note的位置
int judgeReplace(int n)
{
    if(n==1) n=2;
    else if(n==2) n=1;
    else if(n==3) n=0;
    else if(n==0) n=3;
    return n;
}
//将noteInformationAll中的note部分反转
void reverseNote(note&notes,int begin,int end)
{
    int Pos,num,n,current_num=0;
    InFile.seekg(Pos1,ios::beg);
    getline(InFile,notes.noteInformation);
    getNoteBegin_And_End(notes);
    inputBegin_And_End(notes,begin,end);
    Pos=notes.noteInformation.find("\"beat\":["+to_string(begin),0);//获取开始小节的位置

    while(current_num<=end)
    {
        num=notes.noteInformation.find("\"column\":",Pos)+strlen("\"column\":");
        n=notes.noteInformation[num]-'0';
        notes.noteInformation=notes.noteInformation.replace(num,1,to_string(judgeReplace(n)));
        current_num=getNum(notes.noteInformation,Pos);
        Pos=notes.noteInformation.find("\"beat\":[",Pos+1);
    }
    OutFile<<"\""<<notes.noteInformation;
}

int main()
{
    int begin,end;//记录开始和结束小节
    note notes;//创建note类对象
    string InFilePosition;//输入文件位置
    string OutFilePosition;//输出文件位置
    OpenFile(InFilePosition,OutFilePosition);//打开文件
    getline(InFile,notes.noteInformationAll);//将谱面全部信息读入noteInformationAll
    beforeNoteInformation(notes);//将noteInformationAll中的note部分之前的部分写入文件
    reverseNote(notes,begin,end);//将noteInformationAll中的note部分反转同时写入
    CloseFile();//关闭文件
    return 0;
}