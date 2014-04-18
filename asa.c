#include <stdio.h>
#include <stdlib.h>

typedef struct nodeAdj{
    int _id;
    int _flow;
    int _cap;
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
int INF = -1;

void initGraph(){
  int i;
  graph = (node *) malloc (USER_NUM * sizeof(node));
  for(i = 0; i < USER_NUM + 2; i++){
    graph[i] = (node) malloc (sizeof(struct node_s));
    graph[i]->_user = (user) malloc (sizeof( struct user_s));
    graph[i]->_id = i;
    graph[i]->_adj = NULL;
    graph[i]->_adjEnd = NULL;
  }
}

void EdmondsKarp(){
    output:
        f             (Value of maximum flow)
        F             (A matrix giving a legal flow with the maximum value)
    f := 0 (Initial flow is zero)
    F := array(1..n, 1..n) (Residual capacity from u to v is C[u,v] - F[u,v])
    forever
        m, P := BreadthFirstSearch(C, E, s, t, F)
        if m = 0
            break
        f := f + m
        (Backtrack search, and write flow)
        v := t
        while v ≠ s
            u := P[v]
            F[u,v] := F[u,v] + m
            F[v,u] := F[v,u] - m
            v := u
    return (f, F)
}

void connect(int connectingNode, int nodeToConnect, int flow, int cap){
  node graphNode = graph[connectingNode];
  adj newNode = (adj) malloc (sizeof(struct nodeAdj));
  newNode->_next = NULL;
  newNode->_id = nodeToConnect;
  newNode->_flow = flow:
  newNode->_cap = cap;
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
  scanf("%d", &CN_NUM);
  int user, link, crit_sit, crit_node;
  initGraph();
  int i = 0, j = 0;
  while(i++ < CN_NUM){
    /*printf("le utilizador %d\n", i); */
    scanf("%d", &user);
    /*printf("le link %d\n", i); */
    scanf("%d", &link);
    connect(user+1, link+1, 1, 0);
  }
  scanf("%d", &CRIT_NUM);
  i = 0;
  while(i++ < CRIT_NUM){
    scanf("%d", &crit_sit);
    j = 0;
    while(j++ < crit_sit){
      scanf("%d", &crit_node);
      connect(0, crit_node+1, INF, INF);
      connect(1, crit_node+1, INF, INF);
    }
  }
  return 0;
}