#include <stdio.h>
#include <stdlib.h>

typedef struct nodeAdj{
    int _id;
    int _flow;
    struct nodeAdj * _next;
}* adj;

typedef struct node_s{
    int _id;
    adj _adj;
    adj _adjEnd;
}* node;

node* graph;

int maxFlow;
int USER_NUM;
int CN_NUM;
int CRIT_NUM;

void initGraph(){
  int i;
  graph = (node *) malloc (USER_NUM * sizeof(node));
  for(i = 0; i < USER_NUM; i++){
    graph[i] = (node) malloc (sizeof(struct node_s));
    graph[i]->_user = (user) malloc (sizeof( struct user_s));
    graph[i]->_id = i;
    graph[i]->_adj = NULL;
    graph[i]->_adjEnd = NULL;
  }
}

void connect(int connectingNode, int nodeToConnect){
  node graphNode = graph[connectingNode];
  adj newNode = (adj) malloc (sizeof(struct nodeAdj));
  newNode->_next = NULL;
  newNode->_id = nodeToShare;
  newNode->flow = 0:
  if(!(graphNode->_adjEnd)){
    graphNode->_adj = newNode;
    graphNode->_adjEnd = newNode;
  }
  else{
    graphNode->_adjEnd->_next = newNode;
    graphNode->_adjEnd = newNode;
  }
}



int main(){
  scanf("%d", &USER_NUM);
  scanf("%d", &SHARE_NUM);
  int user, link;
  initGraph();
  int i = 0;
  while(i++ < SHARE_NUM){
    /*printf("le utilizador %d\n", i); */
    scanf("%d", &user);
    /*printf("le link %d\n", i); */
    scanf("%d", &link);
    connect(user-1, link-1);
  }
  
  
  return 0;
}