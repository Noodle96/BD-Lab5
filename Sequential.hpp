#ifndefSequential_hpp
#defineSequential_hpp

#include<algorithm>
#include<climits>
#include<iostream>
#include<cstdio>
#include<vector>
#include<stdio.h>
#include<fstream>

usingnamespacestd;

voidbegin(){
ofstreamfile("Bplus.dot");
file<<"digraphQ{"<<endl;
file<<"node[shape=record,height=.1];"<<endl;
}

voidend(){
ofstreamfile;
file.open("Bplus.dot",ios::app);
file<<"}";
file.close();
}

voidgraph(){
system("dot-TpdfBplus.dot-oBplus.pdf");
}

constintORDER=4;

enumNodeType{
INTERNAL=0,
LEAF
};

structNode{
NodeTypetype;
intsize;
Node*parent;
Node*left_sibling;
Node*right_sibling;
intid;

boolisRoot(){
returnparent==NULL;
}

boolisFull(){
returnsize>=ORDER-1;
}
virtualvoidprint()=0;//fordebug
};

structKeyValuePair{
intkey;
intvalue;
};

intk=0;

structLeaf:Node{
KeyValuePairkey_value[ORDER];

Leaf(){
type=LEAF;
size=0;
parent=left_sibling=right_sibling=NULL;
}

voidprint(){
ofstreamfile;
file.open("Bplus.dot",ios::app);

if(parent){
std::cout<<"ID:"<<id<<"size:"<<size<<"parent:"<<parent->id;
}else{
std::cout<<"ID:"<<id<<"size:"<<size<<"parent:NULL";
}
if(left_sibling){
std::cout<<"Leftsibling:"<<left_sibling->id;
}else{
std::cout<<"Leftsibling:NULL";
}
if(right_sibling){
std::cout<<"Rightsibling:"<<right_sibling->id<<'\n';
}else{
std::cout<<"Leftsibling:NULL"<<'\n';
}

file<<"node"<<id<<"[label=\"<f0>";
intj=1;

std::cout<<"values:";
for(inti=0;i<size;++i){
std::cout<<key_value[i].key<<'';
file<<"|"<<key_value[i].key<<"|<f"<<j<<">";
j++;
}

file<<"\"];"<<endl;
file<<"\"node"<<parent->id<<"\":f"<<k<<"->\"node"<<id<<"\""<<endl;
k++;

std::cout<<'\n';
}
};

structKeyReferencePair{
intkey;
Node*reference;
};

structInternalNode:Node{
KeyReferencePairkey_ref[ORDER+1];

InternalNode(){
type=INTERNAL;
size=0;
parent=left_sibling=right_sibling=NULL;
key_ref[0].key=INT_MAX;
}

voidprint(){
ofstreamfile;
file.open("Bplus.dot",ios::app);

if(parent){
std::cout<<"ID:"<<id<<"size:"<<size<<"parent:"<<parent->id;
}else{
std::cout<<"ID:"<<id<<"size:"<<size<<"parent:NULL";
}
if(left_sibling){
std::cout<<"Leftsibling:"<<left_sibling->id;
}else{
std::cout<<"Leftsibling:NULL";
}
if(right_sibling){
std::cout<<"Rightsibling:"<<right_sibling->id<<'\n';
}else{
std::cout<<"Leftsibling:NULL"<<'\n';
}

file<<"node"<<id<<"[label=\"<f0>";
intj=1;

std::cout<<"keys:";
for(inti=0;i<size;++i){
std::cout<<key_ref[i].key<<'';
file<<"|"<<key_ref[i].key<<"|<f"<<j<<">";
j++;
}

file<<"\"];"<<endl;

cout<<'\n';
std::cout<<"childsID:";
for(inti=0;i<=size;++i){
std::cout<<key_ref[i].reference->id<<'';
}
std::cout<<'\n';
}
};


classSeqBPlusTree{
private:
Node*root;
intdepth;
intnode_count;//#ofnodes
//amonotoniclyincreasingaccumulatorfornodeidassignment
//mayoverflowifthenumebrnodesevercreatedbythetreeismorethanINT_MAX
//Butthisishardlyhappendingbecausewedon'texpectthetreesizetobe
//thathuge(unfortunatelythememorywilloverflowfirst)orthetreestructures
//toexistsforthatlongtime.
//Possibleimprovement:maintainasetofidsofdeletednodesinaddition
//toid_accumulator.Onlyincreaseid_accumulatoranduseittoassignanew
//nodeidwhenthesetisempty.Otherwise,extractanidfromthesetfor
//anewlycreatednode.
intid_accumulator;

public:
SeqBPlusTree();
voidprint();
intsearch(intkey);
boolinsert(intkey,intvalue);
boolremove(intkey);
private:
Leaf*leaf_search(intkey,Node*curr_node);
voidsort_entry_by_key(Node*curr_node);
voidsplit_leaf(Leaf*curr_leaf);
voidleaf_remove(intkey);
voidparent_insert(Node*curr_node,intkey,Node*right_half);
voidsplit_internal(InternalNode*curr_node);
voidprint_recusive(vector<Node*>nodeVec);
};


//atthebeginningtherootshouldbeonlyaleaf
SeqBPlusTree::SeqBPlusTree(){
root=newLeaf();
depth=0;
node_count=1;
id_accumulator=1;
root->id=1;
}

intSeqBPlusTree::search(intkey){
Leaf*leaf=leaf_search(key,root);
for(inti=0;i<leaf->size;++i){
if(key==leaf->key_value[i].key){
returnleaf->key_value[i].value;
}
}
return-1;
}


boolSeqBPlusTree::insert(intkey,intvalue){
Leaf*leaf=leaf_search(key,root);
for(inti=0;i<leaf->size;++i){
if(key==leaf->key_value[i].key){
leaf->key_value[i].value=value;
returnfalse;
}
}
//ifthenodeisfull,needtosplitafterinsertion
boolneedSplit=leaf->isFull();
leaf->key_value[leaf->size].key=key;
leaf->key_value[leaf->size++].value=value;
sort_entry_by_key(leaf);

if(needSplit){
split_leaf(leaf);
}

returntrue;
}

boolSeqBPlusTree::remove(intkey){
intrev=-1;
Leaf*leaf=leaf_search(key,root);
InternalNode*parent=(InternalNode*)leaf->parent;
for(inti=0;i<leaf->size;++i){
if(key==leaf->key_value[i].key){
rev=i;
if(rev<0){returnfalse;}
else{
for(inti=rev+1;i<leaf->size;++i){
leaf->key_value[i-1]=leaf->key_value[i];
}
leaf->size--;
}
rev=-1;
for(inti=0;i<=parent->size;++i){
if(key==parent->key_ref[i].key){
rev=i;
if(rev<0){returntrue;}
else{
for(inti=rev+1;i<parent->size;++i){
parent->key_ref[i-1]=parent->key_ref[i];
}
parent->key_ref[i].key=leaf->key_value[0].key;
}
boolneedSplit=leaf->isFull();
sort_entry_by_key(leaf);
if(needSplit){
split_leaf(leaf);
}
cout<<"keyhasbeenremoved"<<endl;
returntrue;
}
}
cout<<"keyhasbeenremoved"<<endl;
returntrue;
}
}
returnfalse;
}

voidSeqBPlusTree::print(){
vector<Node*>rootVec;
rootVec.push_back(root);
print_recusive(rootVec);
}

Leaf*SeqBPlusTree::leaf_search(intkey,Node*curr_node){
if(LEAF==curr_node->type){
return(Leaf*)curr_node;
}
InternalNode*curr_internal=(InternalNode*)curr_node;
for(inti=0;i<curr_internal->size;++i){
if(key<curr_internal->key_ref[i].key){
returnleaf_search(key,curr_internal->key_ref[i].reference);
}
}
//ifthekeyislagerthaneveryseperator,theonlypossiblelocation
//isinthedummyreference
returnleaf_search(key,curr_internal->key_ref[curr_internal->size].reference);
}

voidSeqBPlusTree::sort_entry_by_key(Node*curr_node){
if(LEAF==curr_node->type){
Leaf*curr_leaf=(Leaf*)curr_node;
sort(curr_leaf->key_value,curr_leaf->key_value+curr_leaf->size,
[](KeyValuePaira,KeyValuePairb){
returna.key<b.key;
});
}else{
InternalNode*curr_internal=(InternalNode*)curr_node;
//needto+1becausethereisadummykeyINT_MAXatkey_ref[size]
sort(curr_internal->key_ref,curr_internal->key_ref+curr_internal->size+1,
[](KeyReferencePaira,KeyReferencePairb){
returna.key<b.key;
});
}
return;
}

voidSeqBPlusTree::split_leaf(Leaf*curr_node){
if(curr_node==NULL||LEAF!=curr_node->type||!curr_node->isFull()){
cerr<<"Notavalidleafortheleafisnotfull."<<endl;
return;
}

Leaf*right_half=newLeaf();
for(inti=curr_node->size/2,j=0;i<curr_node->size;++i,++j){
right_half->key_value[j]=curr_node->key_value[i];
right_half->size++;
}
right_half->id=++id_accumulator;
++node_count;

intmedianKey=curr_node->key_value[curr_node->size/2].key;
curr_node->size=curr_node->size/2;

//updatesiblings,fromrighttoleft
if(NULL!=curr_node->right_sibling){
curr_node->right_sibling->left_sibling=right_half;
}
right_half->right_sibling=curr_node->right_sibling;
right_half->left_sibling=curr_node;
curr_node->right_sibling=right_half;

parent_insert(curr_node,medianKey,right_half);

return;
}

voidSeqBPlusTree::parent_insert(Node*curr_node,intkey,Node*right_half){
InternalNode*parent=(InternalNode*)curr_node->parent;
//ifthesplitnodeisroot,weneedtoaddanewroot
if(parent==NULL){
parent=newInternalNode();
depth++;
parent->id=++id_accumulator;
node_count++;
root=parent;
}
//ifparentisfull,weneedtosplittheparentafterwards
boolparent_split=parent->isFull();

//++firstbecausethereisadummykeyINT_MAXatkey_ref[size]
parent->key_ref[++(parent->size)].key=key;
parent->key_ref[parent->size].reference=curr_node;
sort_entry_by_key(parent);
//Searchforthefirstkey-referencepairwhosekeyisgreaterthanthe
//insertedkey,thispairisalsopointedtothecurrentnode
//Nowredirectittotheright.
//Needtouse<=becausealsoneedtocheckthedummykeyINT_MAXatkey_ref[size]
for(inti=0;i<=parent->size;++i){
if(key<parent->key_ref[i].key){
parent->key_ref[i].reference=right_half;
break;
}
}
curr_node->parent=parent;
right_half->parent=parent;

//ifparentisfull,weneedtosplittheparent
if(parent_split){
split_internal(parent);
}

return;
}

voidSeqBPlusTree::split_internal(InternalNode*curr_node){
if(curr_node==NULL||!curr_node->isFull()){
cerr<<"Notavalidnodeorthenodeisnotfull."<<endl;
return;
}

InternalNode*right_half=newInternalNode();
//Needtouse<=becausewealsowanttocopythedummykeyINT_MAXatkey_ref[size]
for(inti=curr_node->size/2+1,j=0;i<=curr_node->size;++i,++j){
right_half->key_ref[j]=curr_node->key_ref[i];
right_half->size++;
Node*child=curr_node->key_ref[i].reference;
child->parent=right_half;
}
right_half->size--;//-1becausethereisadummykeyINT_MAXatkey_ref[size]
right_half->id=++id_accumulator;
++node_count;

intmedianKey=curr_node->key_ref[curr_node->size/2].key;
curr_node->size=curr_node->size/2;
curr_node->key_ref[curr_node->size].key=INT_MAX;

//updatesiblings,fromrighttoleft
if(NULL!=curr_node->right_sibling){
curr_node->right_sibling->left_sibling=right_half;
}
right_half->right_sibling=curr_node->right_sibling;
right_half->left_sibling=curr_node;
curr_node->right_sibling=right_half;

parent_insert(curr_node,medianKey,right_half);

return;
}
voidSeqBPlusTree::print_recusive(vector<Node*>nodeVec){
vector<Node*>nextLevel;
boolhit_leaves=LEAF==nodeVec.front()->type;
for(inti=0;i<nodeVec.size();++i){
nodeVec.at(i)->print();
//cout<<"";
cout<<endl;
if(hit_leaves)continue;
for(intj=0;j<=nodeVec.at(i)->size;++j){
nextLevel.push_back(((InternalNode*)nodeVec.at(i))->key_ref[j].reference);
}
}
cout<<endl;
if(hit_leaves)return;
print_recusive(nextLevel);
}

#endif