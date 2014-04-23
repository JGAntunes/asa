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

adj* parent;

int source;
int sink;
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
  for(i = 0; i < USER_NUM; i++){
    graph[i] = (node) malloc (sizeof(struct node_s));
    graph[i]->_id = i;
    graph[i]->_adj = NULL;
    graph[i]->_adjEnd = NULL;
  }
}

int BFS(){
  int i, nextId, thisId, parentId = -1, resCap, maxF = INF;
  adj* P = (adj*) malloc (sizeof(adj) * (USER_NUM));
  node u;
  adj aux;
  for (i = 0; i < USER_NUM; i++){
    P[i] = NULL;
  }
  endQueue = NULL;
  headQueue = NULL;
  pushQueue(graph[source]);
  while (headQueue){
    u = popQueue();
    aux = u->_adj;
    while (aux){
      thisId = u->_id;
      if(thisId != source){
        parentId = P[thisId]->_connection->_id[P[thisId]->_index ];
      }
      nextId = aux->_connection->_id[(aux->_index +1) %2];
      resCap = aux->_connection->_cap - aux->_connection->_flow;

      if (resCap > 0 && !P[nextId] && (parentId != nextId)){
        P[nextId] = aux;
        if(maxF == INF || resCap < maxF){
          maxF = resCap;
        }
        if(nextId != sink){
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

int edmondsKarp(int min){
  int maxFlow = 0, nextId, augment = 0, auxId;
  adj auxAdj;
  con auxCon;
  augment = BFS();
  while(augment){
    auxId = sink;
    while(auxId != source){
      auxAdj = parent[auxId];
      auxCon = parent[auxId]->_connection;
      auxCon->_flow++;
      nextId = auxCon->_id[auxAdj->_index];
      auxId = nextId;
    }
    if(augment != INF){
      maxFlow++;
    }
    if(maxFlow >= min && min != INF){
      return min;
    }
    augment = BFS();
  }
  return maxFlow;
}


void connect(int connectingNode, int nodeToConnect, int cap){
  node graphNode = graph[connectingNode];
  adj newNode = (adj) malloc (sizeof(struct nodeAdj));
  adj otherNode = (adj) malloc (sizeof(struct nodeAdj));
  newNode->_connection = (con) malloc (sizeof(struct nodeCon));
  newNode->_next = NULL;
  newNode->_prev = NULL;
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
    newNode->_prev = graphNode->_adjEnd;
    graphNode->_adjEnd->_next = newNode;
    graphNode->_adjEnd = newNode;
  }
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

int main(){
  scanf("%d", &USER_NUM);
  scanf("%d", &CN_NUM);
  int user, link, crit_sit, *crit_node, auxId;
  adj auxAdj;
  initGraph();
  int i = 0, j = 0, k = 0, n = 0;
  while(i++ < CN_NUM){
    scanf("%d", &user);
    scanf("%d", &link);
    connect(user, link, 1);
  }
  scanf("%d", &CRIT_NUM);
  int res[CRIT_NUM];
  i = 0;
  while(i++ < CRIT_NUM){
    scanf("%d", &crit_sit);
    j = 0;
    crit_node = (int *) malloc (sizeof(int) * crit_sit);
    for(j = 0; j < crit_sit; j++){
      scanf("%d", &crit_node[j]);
    }
    res[i-1] = INF;
    for(k = 0; k < crit_sit; k++){
      source = crit_node[k];
      for(j = k+1; j < crit_sit; j++){
        sink = crit_node[j];
        res[i-1] = edmondsKarp(res[i-1]);
        auxId = 0;
        auxAdj = graph[0]->_adj;
        for(n = 0; n < CN_NUM * 2; n++){
          while(!auxAdj){
            auxId++;
            auxAdj = graph[auxId]->_adj;
          }
          auxAdj->_connection->_flow = 0;
          auxAdj = auxAdj->_next;
        }
      }
    }
    printf("%d\n", res[i-1]);
    free(crit_node);
  }
  return 0;
}
