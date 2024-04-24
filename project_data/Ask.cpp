#include <bits/stdc++.h>
using namespace std;
class User;

class base
{
    protected:
    int ID;
    int From_ID;
};

class Question : base
{
int To_ID;
string question;
public:
friend class User;
friend class System;
Question(){};
Question(string _question,int from_ID,int to_ID,int id){
    question=_question;
    From_ID=from_ID;
    To_ID=to_ID;
    ID=id;
}
bool operator == (Question &question){
    return (ID==question.ID);
}
};

class Answer : base
{

string answer;
int For_ID;
public:
friend class User;
friend class System;
Answer(){};
Answer(string _answer,int from_ID,int for_ID,int id){
    answer=_answer;
    From_ID=from_ID;
    For_ID=for_ID;
    ID=id;
}
bool operator == (Answer &answer){
    return (ID==answer.ID);
}
};

vector<User>Users_List;
vector<Question>Questions_List;
vector<Answer>Answer_List;

class User
{
    public:
string User_Name;
string Password;
int User_ID;
friend class System;
User(){};
User(string user_Name,string password,int user_ID)
{
    User_Name=user_Name;
    Password=password;
    User_ID=user_ID;
}

bool operator == (User &user){
    return (User_ID==user.User_ID);
}

Question Ask_Question(){
    string to_username;
    int to_userid=-1;
    cout << "Enter user name: ";
    try
    {
        cin >> to_username;
        if(to_username==User_Name){
        string error="User name";
        throw error;
        }
        for (int i = 0; i < Users_List.size(); i++)
        {
            if(Users_List[i].User_Name==to_username){
                to_userid=Users_List[i].User_ID;
            }
        }
        if(to_userid==-1)
        {
        string error="User name";
        throw error;
        }
    }
    catch(string error)
    {
        cerr <<"ERROR: "<< error << " is incorrect" << '\n';
        cerr << "Enter number:" << endl;
        cerr <<"\t"<<"1: Try again" << endl;
        cerr <<"\t"<<"Any button: Return" << endl;
        char Error_option;
        cin >> Error_option;
        if(Error_option=='1'){
        return Ask_Question();
        }else{
        Question invalid_qus={"-1",-1,-1,-1};
        return invalid_qus;
        }
    }
    cout << "Enter Question: ";
    string question;
    cin >> std::ws;
    getline(cin,question);
    int qus_id=1;
    if(!Questions_List.empty())
    {
        qus_id=Questions_List.back().ID+1;
    }
    Question New_qus={question,User_ID,to_userid,qus_id};
    return New_qus;
}

Answer Answer_Question(){
    int Question_ID=-1;
    cout << "Enter Question ID: ";
    int Answer_ID=-1;
    try
    {
        cin >> Question_ID;
        for (int i = 0; i < Questions_List.size(); i++)
        {
            if(Questions_List[i].ID==Question_ID){
                if(Questions_List[i].From_ID!=User_ID && Questions_List[i].To_ID!=User_ID){
                    string error="This question is not from you or to you";
                    throw error;
                }
                string from_user=Users_List[Questions_List[i].From_ID-1].User_Name;
                string to_user=Users_List[Questions_List[i].To_ID-1].User_Name;
                cout << "Question From " << from_user << " to " << to_user << " :" << endl;
                cout <<"-> "<< Questions_List[i].question <<""<< endl;

                if(Answer_List.empty())
                {
                Answer_ID=1;
                }else{
                Answer_ID=Answer_List.back().ID+1;
                }
                int Answer_num=1;
                for (int j = 0; j < Answer_List.size(); j++)
                {
                if(Answer_List[j].For_ID==Question_ID)
                {
                string from=Users_List[Answer_List[j].From_ID-1].User_Name;
                cout <<"\t"<<Answer_num << "- Answer From " << from <<":"<< endl;
                cout <<"\t"<<"-> "<< Answer_List[j].answer << endl;
                Answer_num++;
                }
                }
                if(Answer_num==1)
                {
                cout << "There Is No Answer Yet!" << endl;
                }
            }
        }
        if(Answer_ID==-1)
        {
        string error="Question ID is incorrect";
        throw error;
        }
    }
    catch(string error)
    {
        cerr <<"ERROR: "<< error << '\n';
        cerr << "Enter number:" << endl;
        cerr <<"\t"<<"1: Try again" << endl;
        cerr <<"\t"<<"Any button: Return" << endl;
        char Error_option;
        cin >> Error_option;
        if(Error_option=='1'){
        return Answer_Question();
        }else{
            Answer invalid_ans={"-1",-1,-1,-1};
        return invalid_ans;
        }
    }
    cout << "Enter Answer: ";
    string answer;
    cin >> std::ws;
    getline(cin,answer);
    Answer New_ans={answer,User_ID,Question_ID,Answer_ID};
    return New_ans;
}

};

class System
{
public:
friend class User;
friend class Question;
System(){
    Users_List.clear();
    ifstream User_file("users.dat",ios::binary | ios::in );
    if(User_file.is_open()){
    while (User_file.peek() != EOF) {
        User user;
        getline(User_file, user.User_Name, '\0');
        getline(User_file, user.Password,'\0');
        User_file.read(reinterpret_cast<char*>(&user.User_ID), sizeof(user.User_ID));
        Users_List.push_back(user);
    }
    User_file.close();
    }
    Questions_List.clear();
    ifstream Qus_file("qus.dat",ios::binary | ios::in );
    if(Qus_file.is_open()){
    while (Qus_file.peek() != EOF) {
        Question qus;
        getline(Qus_file, qus.question, '\0');
        Qus_file.read(reinterpret_cast<char*>(&qus.ID), sizeof(qus.ID));
        Qus_file.read(reinterpret_cast<char*>(&qus.From_ID), sizeof(qus.From_ID));
        Qus_file.read(reinterpret_cast<char*>(&qus.To_ID), sizeof(qus.To_ID));
        Questions_List.push_back(qus);
    }
    Qus_file.close();
    }
    Answer_List.clear();
    ifstream ans_file("ans.dat",ios::binary | ios::in );
    if(ans_file.is_open()){
    while (ans_file.peek() != EOF) {
        Answer ans;
        getline(ans_file, ans.answer, '\0');
        ans_file.read(reinterpret_cast<char*>(&ans.ID), sizeof(ans.ID));
        ans_file.read(reinterpret_cast<char*>(&ans.From_ID), sizeof(ans.From_ID));
        ans_file.read(reinterpret_cast<char*>(&ans.For_ID), sizeof(ans.For_ID));
        Answer_List.push_back(ans);
    }
    ans_file.close();
    }

}

int Start_Menu()
    {
    int option;
    cout << "Menu:" << endl;
    cout <<"\t"<<"1:Login" << endl;
    cout <<"\t"<<"2:Sign Up" << endl;
    cout << "Enter number in range 1 - 2: ";
    cin.exceptions(cin.failbit);
    try
    {
        cin >> option;
        if(option!=1 && option!=2)
        throw option;
    }
    catch(int error)
    {
        cerr <<"ERROR: "<< error << " is an invalid value" << '\n';
        cerr << "Enter number: " << endl;
        cerr <<"\t"<<"1: Try again" << endl;
        cerr <<"\t"<<"Any button: Exit" << endl;
        char Error_option;
        cin >> Error_option;
        if(Error_option=='1'){
            return Start_Menu();
        }
        return 0;
    }
    return option;
    }

User Login_Menu()
    {
    string User_Name,Password;
    cout << "Enter user name: ";
    cin >> User_Name;
    cout << "Enter password: ";
    cin >> Password;
    cin.exceptions(cin.failbit);
    try
    {
        for (int i = 0; i < Users_List.size(); i++)
        {
            if(Users_List[i].User_Name==User_Name){
            if(Users_List[i].Password==Password){
            return Users_List[i];
            }else{
                string error="Password";
            throw error;
            }
            }
        }
        string error="User name";
        throw error;

    }
    catch(string error)
    {
        cerr <<"ERROR: "<< error << " is incorrect" << '\n';
        cerr << "Enter number:" << endl;
        cerr <<"\t"<<"1: Sign Up" << endl;
        cerr <<"\t"<<"2: Try again" << endl;
        cerr <<"\t"<<"Any button: Exit" << endl;
        char Error_option;
        cin >> Error_option;
        if(Error_option=='1'){
        return Sign_Up_Menu();
        }else if(Error_option=='2'){
        return Login_Menu();
        }else{
            User invalid_user={"error","error",-1};
        return invalid_user;
        }
    }
    User invalid_user={"error","error",-1};
    return invalid_user;
    }

User Sign_Up_Menu()
    {
    string User_Name,Password;
    cout << "Enter user name: ";
    cin >> User_Name;
    cout << "Enter password: ";
    cin >> Password;
    cin.exceptions(cin.failbit);
    try
    {
        for (int i = 0; i < Users_List.size(); i++)
        {
            if(Users_List[i].User_Name==User_Name){
                string error=User_Name;
            throw error;
            }
        }


    }
    catch(string error)
    {
        cerr <<"ERROR: "<< error << " is already used" << '\n';
        cerr << "Enter number:" << endl;
        cerr <<"\t"<<"1: Try again" << endl;
        cerr <<"\t"<<"Any button: Exit" << endl;
        char Error_option;
        cin >> Error_option;
        if(Error_option=='1'){
        Sign_Up_Menu();
        }else{
            User invalid_user={"error","error",-1};
        return invalid_user;
        }
    }
    int New_User_ID;
    if(Users_List.empty()){
    New_User_ID=1;
    }else{
    New_User_ID=(Users_List.back().User_ID)+1;
    }
    User New_User={User_Name,Password,New_User_ID};
    Add_User_To_System(New_User);
    return New_User;
    }

void Add_User_To_System(User New_User)
{
    New_User.User_Name.push_back('\0');
    New_User.Password.push_back('\0');
ofstream User_file;
User_file.open("users.dat",ios::app | ios::out | ios::binary);
User_file.write(New_User.User_Name.c_str(),New_User.User_Name.size());
User_file.write(New_User.Password.c_str(),New_User.Password.size());
User_file.write(reinterpret_cast<char*>(&New_User.User_ID), sizeof(New_User.User_ID));
User_file.close();
Users_List.push_back(New_User);
}

void Add_Question_To_System(Question New_Qus)
{
New_Qus.question.push_back('\0');
ofstream Qus_file;
Qus_file.open("qus.dat",ios::app | ios::out | ios::binary);
Qus_file.write(New_Qus.question.c_str(),New_Qus.question.size());
Qus_file.write(reinterpret_cast<char*>(&New_Qus.ID), sizeof(New_Qus.ID));
Qus_file.write(reinterpret_cast<char*>(&New_Qus.From_ID), sizeof(New_Qus.From_ID));
Qus_file.write(reinterpret_cast<char*>(&New_Qus.To_ID), sizeof(New_Qus.To_ID));
Questions_List.push_back(New_Qus);
Qus_file.close();
}

void Add_Answer_To_System(Answer New_ans){
New_ans.answer.push_back('\0');
ofstream ans_file;
ans_file.open("ans.dat",ios::app | ios::out | ios::binary);
ans_file.write(New_ans.answer.c_str(),New_ans.answer.size());
ans_file.write(reinterpret_cast<char*>(&New_ans.ID), sizeof(New_ans.ID));
ans_file.write(reinterpret_cast<char*>(&New_ans.From_ID), sizeof(New_ans.From_ID));
ans_file.write(reinterpret_cast<char*>(&New_ans.For_ID), sizeof(New_ans.For_ID));
Answer_List.push_back(New_ans);
ans_file.close();
}

void Update_Questions(Question New_Qus)
{
New_Qus.question.push_back('\0');
ofstream Qus_file;
Qus_file.open("qus.dat",ios::out | ios::binary);
Qus_file.clear();
Qus_file.write(New_Qus.question.c_str(),New_Qus.question.size());
Qus_file.write(reinterpret_cast<char*>(&New_Qus.ID), sizeof(New_Qus.ID));
Qus_file.write(reinterpret_cast<char*>(&New_Qus.From_ID), sizeof(New_Qus.From_ID));
Qus_file.write(reinterpret_cast<char*>(&New_Qus.To_ID), sizeof(New_Qus.To_ID));
Qus_file.close();
}

void Update_Answers(Answer New_ans){
New_ans.answer.push_back('\0');
ofstream ans_file;
ans_file.open("ans.dat",ios::out | ios::binary);
ans_file.clear();
ans_file.write(New_ans.answer.c_str(),New_ans.answer.size());
ans_file.write(reinterpret_cast<char*>(&New_ans.ID), sizeof(New_ans.ID));
ans_file.write(reinterpret_cast<char*>(&New_ans.From_ID), sizeof(New_ans.From_ID));
ans_file.write(reinterpret_cast<char*>(&New_ans.For_ID), sizeof(New_ans.For_ID));
ans_file.close();
}

void Show_System_Users(){
    int i=1;
    for(auto it : Users_List){
        cout <<i++<<"- "<<"User name: "<< it.User_Name << endl;
}
}

void Print_Qus_Form_User(User user){
    int Num_Qus=1;
for(auto it : Questions_List){
    if(it.From_ID==user.User_ID){
                string to_user=Users_List[it.To_ID-1].User_Name;
                cout <<Num_Qus<< "- Question ID ("<<it.ID<<")"<< " to " << to_user << " :" << endl;
                cout <<"-> "<< it.question << endl;
                int Answer_num=1;
                for (int j = 0; j < Answer_List.size(); j++)
                {
                if(Answer_List[j].For_ID==it.ID)
                {
                string from=Users_List[Answer_List[j].From_ID-1].User_Name;
                cout <<'\t'<<Answer_num << "- Answer From " << from <<":"<< endl;
                cout <<'\t'<<"-> "<< Answer_List[j].answer << endl;
                Answer_num++;
                }
                }
                if(Answer_num==1)
                {
                cout <<'\t'<< "There Is No Answer Yet!" << endl;
                }
                Num_Qus++;
    }
}
if(Num_Qus==1)
{
cout <<"There Is No Questions Yet!" << endl;
}
}

void Print_Qus_To_User(User user){
    int Num_Qus=1;
for(auto it : Questions_List){
    if(it.To_ID==user.User_ID){
                string from_user=Users_List[it.From_ID-1].User_Name;
                
                cout <<Num_Qus<< "- Question ID ("<<it.ID<<")"<< " from " << from_user<< " :" << endl;
                cout <<"-> "<< it.question << endl;
                int Answer_num=1;
                for (int j = 0; j < Answer_List.size(); j++)
                {
                if(Answer_List[j].For_ID==it.ID)
                {
                string from=Users_List[Answer_List[j].From_ID-1].User_Name;
                cout <<'\t'<<Answer_num << "- Answer From " << from <<":"<< endl;
                cout <<'\t'<<"-> "<< Answer_List[j].answer << endl;
                Answer_num++;
                }
                }
                if(Answer_num==1)
                {
                cout <<'\t'<< "There Is No Answer Yet!" << endl;
                }
                Num_Qus++;
    }
}
if(Num_Qus==1)
{
cout <<"There Is No Questions Yet!" << endl;
}
}

void Feed(){
    int Num_Qus=1;
for(auto it : Questions_List){
                string from_user=Users_List[it.From_ID-1].User_Name;
                string to_user=Users_List[it.To_ID-1].User_Name;
                cout <<Num_Qus<< "- Question ID ("<<it.ID<<")"<< " from " << from_user <<" to "<<to_user<< " :" << endl;
                cout <<"-> "<< it.question << endl;
                int Answer_num=1;
                for (int j = 0; j < Answer_List.size(); j++)
                {
                if(Answer_List[j].For_ID==it.ID)
                {
                string from=Users_List[Answer_List[j].From_ID-1].User_Name;
                cout <<'\t'<<Answer_num << "- Answer From " << from <<":"<< endl;
                cout <<'\t'<<"-> "<< Answer_List[j].answer << endl;
                Answer_num++;
                }
                }
                if(Answer_num==1)
                {
                cout <<'\t'<< "There Is No Answer Yet!" << endl;
                }
                Num_Qus++;
    
}
if(Num_Qus==1)
{
cout <<"There Is No Questions Yet!" << endl;
}
}

int Main_Menu(){
    int option;
    cout << "Menu:" << endl;
    cout <<"\t"<<"1: Print Questions To Me" << endl;
    cout <<"\t"<<"2: Print Questions From Me" << endl;
    cout <<"\t"<<"3: Answer Question" << endl;
    cout <<"\t"<<"4: Delete Question" << endl;
    cout <<"\t"<<"5: Ask Question" << endl;
    cout <<"\t"<<"6: List System Users" << endl;
    cout <<"\t"<<"7: Feed" << endl;
    cout <<"\t"<<"8: Logout" << endl;
    cout <<"\t"<<"9: Exit" << endl;
    cout << "Enter number in range 1 - 9: ";
    cin.exceptions(cin.failbit);
    try
    {
        cin >> option;
        if(option<1 || option>9)
        throw option;
    }
    catch(int error)
    {
        cerr <<"ERROR: "<< error << " is an invalid value" << '\n';
        cerr << "Enter number: " << endl;
        cerr <<"\t"<<"1: Try again" << endl;
        cerr <<"\t"<<"Any button: Exit" << endl;
        char Error_option;
        cin >> Error_option;
        if(Error_option=='1'){
            return Main_Menu();
        }
        return 8;
    }
    return option;
}

int Delete_Question(User user){
int Question_ID;
cout << "Enter question ID: ";
vector<Question>New_Questions_List;
vector<Answer>New_Answer_List;
try
{
        cin >> Question_ID;
        if(Question_ID==-1)
        {
            string error="Question ID is incorrect";
        throw error;
        }
        
        for (int i = 0; i < Questions_List.size(); i++)
        {
            if(Questions_List[i].ID==Question_ID){
                if(Questions_List[i].From_ID!=user.User_ID){
                    string error="This question is not from you";
                    throw error;
                }
                for (int j = 0; j < Answer_List.size();j++)
                {
                if(Answer_List[j].For_ID!=Question_ID)
                {
                New_Answer_List.push_back(Answer_List[j]);
                }
                }
                Question_ID=-1;
            }else{
            New_Questions_List.push_back(Questions_List[i]);
            }
        }
        if(Question_ID!=-1)
        {
        string error="Question ID is incorrect";
        throw error;
        }
    }
    catch(string error)
    {
        cerr <<"ERROR: "<< error << '\n';
        cerr << "Enter number:" << endl;
        cerr <<"\t"<<"1: Try again" << endl;
        cerr <<"\t"<<"Any button: Return" << endl;
        char Error_option;
        cin >> Error_option;
        if(Error_option=='1'){
        return Delete_Question(user);
        }else{
            return 0;
        }
    }
Questions_List=New_Questions_List;
Answer_List=New_Answer_List;
for (int i = 0; i < Questions_List.size(); i++){
  Update_Questions(Questions_List[i]);  
}
for (int i = 0; i < Answer_List.size(); i++){
  Update_Answers(Answer_List[i]);
}
if(Questions_List.size()==0){
ofstream Qus_file;
Qus_file.open("Qus.dat",ios::out | ios::binary);
Qus_file.clear();
Qus_file.close();
}
if(Answer_List.size()==0){
ofstream ans_file;
ans_file.open("ans.dat",ios::out | ios::binary);
ans_file.clear();
ans_file.close();
}
return 1;
}

};

int main(){
Restart:
System Sys;
int option;
option=Sys.Start_Menu();
User Current_user;
User invalid_user={"error","error",-1};
if(option==0){
return 0;
}else if(option==1){
    Current_user=Sys.Login_Menu();
}else{
    Current_user=Sys.Sign_Up_Menu();
}
if(Current_user==invalid_user){
        return 0;
}
cout << "Welcome, "<< Current_user.User_Name << endl;
Start:
option=Sys.Main_Menu();
if(option==1)
{
    System temp;
Sys.Print_Qus_To_User(Current_user);
cout <<"Enter any button to Return : ";
char Error_option;
        cin >> Error_option;
goto Start;
}else if(option==2){
    System temp;
Sys.Print_Qus_Form_User(Current_user);
cout <<"Enter any button to Return : ";
char Error_option;
        cin >> Error_option;
goto Start;
}else if(option==3){
ReAns:
    Answer invalid_ans={"-1",-1,-1,-1};
    Answer New_ans=Current_user.Answer_Question();
    if(New_ans==invalid_ans)
    {
    goto Start;
    }else{
        Sys.Add_Answer_To_System(New_ans);
        cout << "Enter number: " << endl;
        cout <<"\t"<<"1: Answer another question" << endl;
        cout <<"\t"<<"Any button: Return" << endl;
        char Error_option;
        cin >> Error_option;
        if(Error_option=='1'){
            goto ReAns;
        }else{
            goto Start;
        }
    }
}else if(option==4){
    ReDel:
    if(Sys.Delete_Question(Current_user)==1)
    {
        cout << "Enter number: " << endl;
        cout <<"\t"<<"1: Delete another question" << endl;
        cout <<"\t"<<"Any button: Return" << endl;
        char Error_option;
        cin >> Error_option;
        if(Error_option=='1'){
            goto ReDel;
        }else{
            goto Start;
        }
    }else{
    goto Start;
    }
}else if(option==5){
ReAsk:
    Question invalid_qus={"-1",-1,-1,-1};
    Question New_qus=Current_user.Ask_Question();
    if(New_qus==invalid_qus)
    {
    goto Start;
    }else{
        Sys.Add_Question_To_System(New_qus);
        cout << "Enter number: " << endl;
        cout <<"\t"<<"1: Ask another question" << endl;
        cout <<"\t"<<"Any button: Return" << endl;
        char Error_option;
        cin >> Error_option;
        if(Error_option=='1'){
            goto ReAsk;
        }else{
            goto Start;
        }
    }
}else if(option==6){
    System temp;
    Sys.Show_System_Users();
        cout <<"Enter any button to Return : ";
        char Error_option;
        cin >> Error_option;
        goto Start;
}else if(option==7){
    System temp;
    Sys.Feed();
cout <<"Enter any button to Return: ";
char Error_option;
        cin >> Error_option;
goto Start;
}else if(option==8){
    goto Restart;
}else{
    return 0;
}

}