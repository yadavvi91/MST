#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_INT 512

struct q_node
{
	int val;
	struct q_node *next, *prev;
} q_node;

struct queue
{
	struct q_node *head, *tail;
} queue;

int graph[MAX_INT][MAX_INT] = {{0}};
int parent[MAX_INT] = {-1}, key[MAX_INT] = {-1}, adj[MAX_INT] = {-1};
int adj_top = 0;

struct queue *init_q()
{
	struct queue *q = malloc(sizeof(struct queue));
	q->head = NULL; q->tail = NULL;
	return q;
}

void free_q(struct queue **q_ptr)
{
    struct q_node *q_temp = (*q_ptr)->head, *q_temp2;
    while (q_temp) {
        q_temp2 = q_temp;
        q_temp = q_temp->next;
        free(q_temp2);
    }
    free(*q_ptr);
}

struct queue *enqueue(struct queue *q, int i)
{
	struct q_node *q_temp = malloc(sizeof(struct q_node));
	q_temp->prev = NULL;
	q_temp->next = NULL;
	q_temp->val = i;
	if (q->head == NULL && q->tail == NULL) {
		q->head = q_temp;
		q->tail = q_temp;
	} else {
		q_temp->prev = q->tail;
		q->tail->next = q_temp;
		q->tail = q_temp;
	}
	return q;
}

int is_empty(struct queue *q)
{
	return (q->head == NULL && q->tail == NULL) ? 1 : 0;
}

void traverse(struct queue *q)
{
    struct q_node *q_inst = q->head;
    if (q->head) {
        do {
            printf("%d ", q_inst->val);
            q_inst = q_inst->next;
        } while (q_inst != NULL);
    }
    printf("\n");
}

int extract_min(struct queue **q_ptr, int no_vert)
{
	struct queue *q = *q_ptr;
	struct q_node *q_inst = q->head, *q_temp = NULL;
	int i, min_val = MAX_INT, min_head;

	// i = q_inst->val;
	while (q_inst != NULL) {
		i = q_inst->val;
		if (key[i] < min_val) {
			min_val = key[i];
			min_head = i;
			q_temp = q_inst;
		}
		q_inst = q_inst->next;
		// i = q_inst->val;
	}
	if (q_temp == q->head && q_temp == q->tail) {
        q->head = NULL;
        q->tail = NULL;
        free(q_temp);
	} else if (q_temp == q->head && q_temp != NULL) {
		if (q_temp->next)
			q_temp->next->prev = NULL;
		(*q_ptr)->head = q_temp->next;
		free(q_temp);
	} else if (q_temp == q->tail && q_temp != NULL) {
		if (q_temp->prev)
			q_temp->prev->next = NULL;
		(*q_ptr)->tail = q_temp->prev;
		free(q_temp);
	} else {
		q_temp->prev->next = q_temp->next;
		q_temp->next->prev = q_temp->prev;
		free(q_temp);
	}
	return min_head;
}

int isin_q(struct queue *q, int v)
{
	struct q_node *q_inst = q->head;
	while (q_inst) {
		if (q_inst->val == v)
			return 1;
		q_inst = q_inst->next;
	}
	return 0;
}

void find_adj(int u, int no_vert)
{
	int i;
	for (i = 0; i < no_vert; i++) {
		if (graph[u][i] != 0) {
			adj[adj_top++] = i;
		}
	}
}

void min_spantree(int no_vert, int no_edge, int root)
{
	int u, v, i;
	struct queue *q = init_q();
	for (i = 0; i < no_vert; i++) {
		key[i] = MAX_INT;
		parent[i] = -1;
	}

	key[root] = 0;
	for (i = 0; i < no_vert; i++) {
		q = enqueue(q, i);
	}
    /* printf("   Key Parent\n");
    for (i = 0; i < no_vert; i++) {
        printf("%6d%6d\n", key[i], parent[i]);
    }*/
	while (!is_empty(q)) {
        // traverse(q);
        u = extract_min(&q, no_vert);
        // printf("U: %d\n", u);
		find_adj(u, no_vert);
		for (i = 0; i < adj_top; i++) {
			v = adj[i];
			if (isin_q(q, v) && graph[u][v] < key[v]) {
				parent[v] = u;
				key[v] = graph[u][v];
			}
		}
		adj_top = 0;
	}

	free_q(&q);
}

int main(int argc, char *argv[])
{
	int no_vert, no_edge, root_vert;
	int i, from, to;

	scanf("%d %d", &no_vert, &no_edge);
	scanf("%d", &root_vert);
	for (i = 0; i < no_edge; i++) {
		scanf("%d %d", &from, &to);
		scanf("%d", &graph[from][to]);
		graph[to][from] = graph[from][to];
	}

	min_spantree(no_vert, no_edge, root_vert);

	printf("   Node Parent    key\n");
	// printf("%6d%6d\n", parent[0], key[0]);
	for (i = 0; i < no_vert; i++) {
		printf("%7d%7d%7d\n", i, parent[i], key[i]);
	}

	return 0;
}
