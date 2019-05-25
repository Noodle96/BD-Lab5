#include"Sequential.hpp"
#include<map>
#include<sstream>
#include<fstream>
#include<cstring>
using namespace std;

class Record{
    public:
        string account_number;
        string branch_name;
        string balance;
    public:
        Record(){
        account_number="";
        branch_name="";
        balance="0.0";
        }
        Record(string account_number,string branch_name,string balance){
        account_number=account_number;
        branch_name=branch_name;
        balance=balance;
        }
        ~Record(){}
};


class DB{
    public:
        SeqBPlusTree tree;
        int rec_pos;
        string db;
        map<int,int> records;
    public:
        DB(string file){
            tree=SeqBPlusTree();
            db=file;
            rec_pos=0;
        }

        int str_to_int(string number){
            stringstream geek(number);
            int num=0;
            geek>>num;
            return num;
        }

        void insert_records(){
            rec_pos=0;
            int i=0;
            fstream file;
            file.open(db,ios::in|ios::binary);
            file.seekg(rec_pos,ios::beg);
            char a_number[50];
            char b_name[50];
            char balance_[50];
            char buffer[10000];
            while(!file.eof()){
                file.seekg(rec_pos,ios::beg);
                file.getline(buffer,10000,'\n');
                string t_(buffer);
                i=strlen(buffer);
                if(t_[0]!=' '){
                    file.seekg(rec_pos,ios::beg);
                    file.getline(a_number,50,',');
                    file.getline(b_name,50,',');
                    file.getline(balance_,50,'\n');
                    string s(a_number);
                    string sub=s.substr(int(s.find("-"))+1);
                    tree.insert(str_to_int(sub),rec_pos);
                    records.insert(pair<int,int>(str_to_int(sub),i));
                }
                rec_pos=rec_pos+i+1;
            }
            file.close();
        }

        void search(int key){
            rec_pos=tree.search(key);
            int i=0;
            fstream file;
            file.open(db,ios::in|ios::binary);
            file.seekg(rec_pos,ios::beg);
            char a_number[50];
            char b_name[50];
            char balance_[50];
            char buffer[10000];
            while(!file.eof()){
                file.seekg(rec_pos,ios::beg);
                file.getline(buffer,10000,'\n');
                string t_(buffer);
                i=strlen(buffer);
                if(t_[0]!=' '){
                    file.seekg(rec_pos,ios::beg);
                    file.getline(a_number,50,',');
                    file.getline(b_name,50,',');
                    file.getline(balance_,50,'\n');
                    cout<<a_number<<"-"<<b_name<<"-"<<balance_<<endl;
                    break;
                }
                rec_pos=rec_pos+i+1;
            }
            file.close();
        }

        void add(){
            ofstream file;
            file.open(db,ios::in|ios::binary);
            file.seekp(0,ios::end);
            string a,b,c;
            cout<<"Accountnumber:";
            cin>>a;
            cout<<"Branchname:";
            cin>>b;
            cout<<"Balance:";
            cin>>c;
            file<<"\n"<<a<<","<<b<<","<<c;
            cout<<"Recordhasbeenadded"<<endl;
            file.close();
        }

        void remove(int key){
            ofstream file;
            file.open(db,ios::in|ios::binary);
            file.seekp(tree.search(key),ios::beg);
            int i=0;
            string v;
            map<int,int>::iterator it;
            it=records.find(key);
            while(i<it->second){
                v=v+"";
                i++;
            }
            v=v+"\n";
            file<<v;
            records.erase(it);
            file.close();
        }
};


int main(){
    DB Bplus("records.txt");
    int res=5;string s;
    while(true){
        cout<<"-----------------------------------------------"<<endl;
        cout<<"1)insertrecords"<<endl;
        cout<<"2)removerecord"<<endl;
        cout<<"3)searchrecord"<<endl;
        cout<<"4)addrecord"<<endl;
        cout<<"5)graph"<<endl;

        cin>>res;
        system("clear");
        if(res==1){
            Bplus.insert_records();
        }
        else if(res==2){
            cout<<"Key:";
            cin>>s;
            Bplus.remove(Bplus.str_to_int(s));
            Bplus.tree.remove(Bplus.str_to_int(s));
        }
        else if(res==3){
            cout<<"Key:";
            cin>>s;
            Bplus.search(Bplus.str_to_int(s));
        }
        else if(res==4){
            Bplus.add();
        }
        else if(res==5){
            begin();
            Bplus.tree.print();
            end();
            graph();
        }
    }
    return 0;
}
