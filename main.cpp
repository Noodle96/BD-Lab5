#include"Sequential.hpp"
#include<map>
#include<sstream>
#include<fstream>
#include<cstring>
usingnamespacestd;

classRecord{
public:
stringaccount_number;
stringbranch_name;
stringbalance;
public:
Record(){
account_number="";
branch_name="";
balance="0.0";
}
Record(stringaccount_number,stringbranch_name,stringbalance){
account_number=account_number;
branch_name=branch_name;
balance=balance;
}
~Record(){}
};


classDB{
public:
SeqBPlusTreetree;
intrec_pos;
stringdb;
map<int,int>records;

public:
DB(stringfile){
tree=SeqBPlusTree();
db=file;
rec_pos=0;
}

intstr_to_int(stringnumber){
stringstreamgeek(number);
intnum=0;
geek>>num;
returnnum;
}

voidinsert_records(){
rec_pos=0;
inti=0;
fstreamfile;
file.open(db,ios::in|ios::binary);
file.seekg(rec_pos,ios::beg);
chara_number[50];
charb_name[50];
charbalance_[50];
charbuffer[10000];
while(!file.eof()){
file.seekg(rec_pos,ios::beg);
file.getline(buffer,10000,'\n');
stringt_(buffer);
i=strlen(buffer);
if(t_[0]!=''){
file.seekg(rec_pos,ios::beg);
file.getline(a_number,50,',');
file.getline(b_name,50,',');
file.getline(balance_,50,'\n');
strings(a_number);
stringsub=s.substr(int(s.find("-"))+1);
tree.insert(str_to_int(sub),rec_pos);
records.insert(pair<int,int>(str_to_int(sub),i));
}
rec_pos=rec_pos+i+1;
}
file.close();
}

voidsearch(intkey){
rec_pos=tree.search(key);
inti=0;
fstreamfile;
file.open(db,ios::in|ios::binary);
file.seekg(rec_pos,ios::beg);
chara_number[50];
charb_name[50];
charbalance_[50];
charbuffer[10000];
while(!file.eof()){
file.seekg(rec_pos,ios::beg);
file.getline(buffer,10000,'\n');
stringt_(buffer);
i=strlen(buffer);
if(t_[0]!=''){
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

voidadd(){
ofstreamfile;
file.open(db,ios::in|ios::binary);
file.seekp(0,ios::end);
stringa,b,c;
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

voidremove(intkey){
ofstreamfile;
file.open(db,ios::in|ios::binary);
file.seekp(tree.search(key),ios::beg);
inti=0;
stringv;
map<int,int>::iteratorit;
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


intmain(){
DBBplus("records.txt");
intres=5;strings;
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
elseif(res==2){
cout<<"Key:";
cin>>s;
Bplus.remove(Bplus.str_to_int(s));
Bplus.tree.remove(Bplus.str_to_int(s));
}
elseif(res==3){
cout<<"Key:";
cin>>s;
Bplus.search(Bplus.str_to_int(s));
}
elseif(res==4){
Bplus.add();
}
elseif(res==5){
begin();
Bplus.tree.print();
end();
graph();
}
}
return0;
}