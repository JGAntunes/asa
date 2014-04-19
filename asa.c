#include <stdio.h>
#include <stdlib.h>

#define INF -1

typedef struct nodeCon{
    int _flow;
    int _cap;
    int _id[2];
}* con;

typedef struct nodeAdj{
    int _index;
    con _connection;
    struct nodeAdj * _next;
}* adj;

typedef struct node_s{
    int _id;
    adj _adj;
    adj _adjEnd;
}* node;

typedef struct queueNode{
    node _node;
    struct queueNode * _next;
    struct queueNode * _prev;
}* queue;

node* graph;

queue endQueue;
queue headQueue;
adj* parent;

int USER_NUM;
int CN_NUM;
int CRIT_NUM;

void pushQueue(node v){
  queue queueNode = (queue) malloc(sizeof(struct queueNode));
  queueNode->_node = v;
  queueNode->_next = endQueue;
  queueNode->_prev = NULL;
  if(!headQueue){
    headQueue = endQueue = queueNode;
  }
  endQueue->_prev = queueNode;
  endQueue = queueNode;
}

node popQueue(){
  queue head = headQueue;
  node v = headQueue->_node;
  headQueue = headQueue->_prev;
  free(head);
  return v;
}


void initGraph(){
  int i;
  graph = (node *) malloc (USER_NUM * sizeof(node));
  for(i = 0; i < USER_NUM + 2; i++){
    graph[i] = (node) malloc (sizeof(struct node_s));
    graph[i]->_id = i;
    graph[i]->_adj = NULL;
    graph[i]->_adjEnd = NULL;
  }
}

int BFS(){
  int i, nextId, resCap, maxF = INF;
  adj* P =(adj*) malloc (sizeof(adj) * (USER_NUM + 2));
  node u;
  adj aux;

  for (i = 0; i < USER_NUM + 2; i++){
    P[i] = NULL;
  }

  /*P[0] = -2; (make sure source is not rediscovered)*/
  endQueue = NULL;
  headQueue = NULL;
  pushQueue(graph[0]);
  while (headQueue){
    u = popQueue();
    aux = u->_adj;

    while (aux){

      /*If there is available capacity, and v is not seen before in search*/
      nextId = aux->_connection->_id[(aux->_index + 1) %2];
      printf("nextid:%d\n", nextId);
      resCap = aux->_connection->_cap - aux->_connection->_flow;
      if ((resCap == INF || resCap > 0) && !P[nextId]){
        P[nextId] = aux;
        if(maxF == INF ||  resCap < maxF){
          maxF = resCap;
        }
        if(nextId != 1){
          pushQueue(graph[nextId]);
        }
        else{
          parent = P;
          return maxF;
        }
      }
      aux = aux->_next;
    }
  }
  parent = P;
  return 0;
}

int edmondsKarp(){
  int maxFlow = 0, nextId, augment = 0;
  node auxNode;
  adj auxAdj;
  con auxCon;
  augment = BFS();
  while(augment){
    /*(Backtrack search, and write flow)*/
    auxNode = graph[1];
    while(auxNode != graph[0]){
      printf("NodeID:%d\n", auxNode->_id);
      auxAdj = parent[auxNode->_id];
      auxCon = parent[auxNode->_id]->_connection;
      if(auxNode->_id == 1){
        auxCon->_cap = auxCon->_flow = 1;
      }
      if(augment != INF){
        auxCon->_flow++;
        maxFlow++;
      }

      nextId = auxCon->_id[auxAdj->_index];
      printf("INDEX1:%d ID1:%d\n", auxAdj->_index,  auxCon->_id[auxAdj->_index ]);
      printf("INDEX2:%d ID2:%d\n", auxAdj->_index + 1,  nextId);
      getchar();
      auxNode = graph[nextId];
    }
    augment = BFS();
  }
  return maxFlow;
}


void connect(int connectingNode, int nodeToConnect, int cap){
  node graphNode = graph[connectingNode];
  adj newNode = (adj) malloc (sizeof(struct nodeAdj));
  adj otherNode = (adj) malloc (sizeof(struct nodeAdj));

  /*Two nodes one connection*/
  newNode->_connection = (con) malloc (sizeof(struct nodeCon));

  /*First side connection*/
  newNode->_next = NULL;
  newNode->_index = 0;
  newNode->_connection->_id[0] = connectingNode;
  newNode->_connection->_id[1] = nodeToConnect;
  newNode->_connection->_flow = 0;
  newNode->_connection->_cap = cap;
  if(!(graphNode->_adjEnd)){
    graphNode->_adj = newNode;
    graphNode->_adjEnd = newNode;
  }
  else{
    graphNode->_adjEnd->_next = newNode;
    graphNode->_adjEnd = newNode;
  }

  /*Other side connection*/
  otherNode->_next = NULL;
  otherNode->_index = 1;
  otherNode->_connection = newNode->_connection;
  graphNode = graph[nodeToConnect];
  if(!(graphNode->_adjEnd)){
    graphNode->_adj = otherNode;
    graphNode->_adjEnd = otherNode;
  }
  else{
    graphNode->_adjEnd->_next = otherNode;
    graphNode->_adjEnd = otherNode;
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
    connect(user+2, link+2, 1);
  }
  scanf("%d", &CRIT_NUM);
  int res[CRIT_NUM];
  i = 0;
  while(i++ < CRIT_NUM){
    scanf("%d", &crit_sit);
    j = 0;
    while(j++ < crit_sit){
      scanf("%d", &crit_node);
      connect(0, crit_node+2, INF);
      connect(1, crit_node+2, INF);
    }
    res[i] = edmondsKarp();
  }

  for(i = 0; i < CRIT_NUM; i++){
    printf("%d\n", res[i]);
  }

  return 0;
}
