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
    struct nodeAdj * _prev;
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

int parentHead;
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
  queue head;
  node v = headQueue->_node;
  if(headQueue == endQueue){
    head = NULL;
    endQueue = NULL;
  }
  else{
    head = headQueue->_prev;
  }
  free(headQueue);
  headQueue = head;
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
  int i, nextId, thisId, parentId = -1, resCap, maxF = INF;
  adj* P = (adj*) malloc (sizeof(adj) * (USER_NUM + 2));
  int crit [USER_NUM + 2];
  node u;
  adj aux;

  for (i = 0; i < USER_NUM + 2; i++){
    P[i] = NULL;
    crit[i] = 0;
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
      thisId = u->_id;
      if(thisId){
        parentId = P[thisId]->_connection->_id[P[thisId]->_index ];
      }
      nextId = aux->_connection->_id[(aux->_index + 1) %2];
      resCap = aux->_connection->_cap - aux->_connection->_flow;

      if ((resCap == INF || resCap > 0) && !P[nextId] && (parentId != nextId)){
        printf("nextid:%d\n", nextId);
        P[nextId] = aux;
        if(maxF == INF ||  (resCap < maxF && resCap != INF)){
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
  int maxFlow = 0, nextId, augment = 0, auxId;
  adj auxAdj;
  con auxCon;
  augment = BFS();
  while(augment){
    /*(Backtrack search, and write flow)*/
    printf("OUTSIDE WHILE\n");
    auxId = 1;
    while(auxId){
      printf("NodeID:%d\n", auxId);
      auxAdj = parent[auxId];
      auxCon = parent[auxId]->_connection;

      /*if(auxId->_id == 1){
        auxCon->_cap = auxCon->_flow = 1;
      }*/

      if(auxCon->_cap != INF){
        auxCon->_flow++;
      }

      nextId = auxCon->_id[auxAdj->_index];
      printf("INDEX1:%d ID1:%d\n", auxAdj->_index,  auxCon->_id[auxAdj->_index ]);
      printf("INDEX2:%d ID2:%d\n", auxAdj->_index + 1,  nextId);
      auxId = nextId;
    }
    if(augment != INF){
      maxFlow++;
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
  newNode->_prev = NULL;
  newNode->_index = 0;
  newNode->_connection->_id[0] = connectingNode;
  newNode->_connection->_id[1] = nodeToConnect;
  printf("CONNECT: %d %d\n", connectingNode, nodeToConnect);
  newNode->_connection->_flow = 0;
  newNode->_connection->_cap = cap;
  if(!(graphNode->_adjEnd)){
    graphNode->_adj = newNode;
    graphNode->_adjEnd = newNode;
  }
  else{
    newNode->_prev = graphNode->_adjEnd;
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
    otherNode->_prev = graphNode->_adjEnd;
    graphNode->_adjEnd->_next = otherNode;
    graphNode->_adjEnd = otherNode;
  }
}

void freeLast(int id){
  adj aux = graph[id]->_adjEnd;
  graph[id]->_adjEnd = graph[id]->_adjEnd->_prev;
  free(aux->_connection);
}

int main(){
  scanf("%d", &USER_NUM);
  scanf("%d", &CN_NUM);
  int user, link, crit_sit, *crit_node, auxId;
  adj auxAdj;
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
    crit_node = (int *) malloc (sizeof(int) * crit_sit);
    while(j++ < crit_sit){
      scanf("%d", &crit_node[j-1]);
      if(j%2){
        connect(0, crit_node[j-1]+2, INF);
      }
      else{
        connect(crit_node[j-1]+2, 1, INF);
      }
    }
    res[i-1] = edmondsKarp();
    for(j = 0; j < crit_sit; j++){
      freeLast(crit_node[j] + 2);
      if(graph[0]->_adj){
        free(graph[0]->_adjEnd);
      }
      if(graph[1]->_adj){
        free(graph[1]->_adjEnd);
      }
    }
    auxId = 2;
    auxAdj = graph[2]->_adj;
    for(j = 0; j < CN_NUM; j++){
      if(!auxAdj){
        auxId++;
        auxAdj = graph[auxId]->_adj;
      }
      printf("Free %d no %d\n", auxId, auxAdj->_connection->_id[0]);
      auxAdj->_connection->_flow = 0;
      auxAdj = auxAdj->_next;
    }
    free(crit_node);
  }

  for(i = 0; i < CRIT_NUM; i++){
    printf("%d\n", res[i]);
  }

  return 0;
}
