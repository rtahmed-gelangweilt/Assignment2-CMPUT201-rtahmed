#include "assg.h"
#include <stdio.h>
#include <string.h>

Node nodes[100];
int node_count = 0;

Node *closed_storage[100];
Node *open_storage[100];

Node* create_node(int x, int y, int g, int h, Node *parent){
    Node *node = &nodes[node_count];
    node->x = x;
    node->y = y;
    node->g = g;
    node->h = h;
    node->f = g + h;
    node->parent = parent;

    node_count++;
    return node;
}

void closed_list_init(ClosedList *list){
    list->data = closed_storage;
    list->size = 0;
    list->capacity = 100;
}

void closed_list_add(ClosedList *list, Node *node){
    list->data[list->size] = node;
    list->size++;
}

bool closed_list_contains(const ClosedList *list, int x, int y){
    for(int i=0; i<(list->size); i++){
        if ((list->data[i]->x == x) && list->data[i]->y == y){
            return true;
        }
    }
    return false;
}

void heap_init(MinHeap *heap){
    heap->data = open_storage;
    heap->size = 0;
    heap->capacity = 100;
}

bool heap_is_empty(const MinHeap *heap){
    return heap->size == 0;
}

bool priority(const Node *node1, const Node *node2){
    if(node1->f < node2->f){
        return true;
    } 
    if(node1->f == node2->f){
        if(node1->h < node2->h){
            return true;}
        
        if(node1->h == node2->h){
            if(node1->x < node2->x){
                return true;}
            
            if(node1->x == node2->x){
                if(node1->y < node2->y){
                    return true;}    
            }
        }
    }
    return false;
        
}

void heapify_up(MinHeap *heap, int index){
    while(index>0){
       int parent_index = (index-1)/2;
       if (priority(heap->data[index], heap->data[parent_index])){
            Node *temp = heap->data[index];
            heap->data[index] = heap->data[parent_index];
            heap->data[parent_index] = temp;
            index = parent_index;
       }
       else{
        break;
    }
}
}

void heap_insert(MinHeap *heap, Node *node){
    heap->data[heap->size] = node;
    (heap->size)++;
    heapify_up(heap, heap->size - 1);
}

int heap_find(const MinHeap *heap, int x, int y){
    for(int i=0; i<(heap->size); i++){
        if(heap->data[i]->x == x &&  heap->data[i]->y ==y)
            return i;   
    }
    return -1;
}

void heap_decrease_key(MinHeap *heap, int index, int new_g, int new_h){
    Node *node = heap->data[index];
    node->g = new_g;
    node->h = new_h;
    node->f = (new_g+new_h);
    heapify_up(heap, index);
}

void heapify_down(MinHeap *heap, int index){
    while(index< heap->size){
        int left = 2*index + 1;
        int right = 2*index + 2;
        int best = index;

        if((left < heap->size) && priority(heap->data[left], heap->data[best])){
            best = left;
        }

        if((right < heap->size) && priority(heap->data[right], heap->data[best])){
            best = right;
        }

        if (best == index){
            break;
        }

        Node *temp = heap->data[index];
        heap->data[index] = heap->data[best];
        heap->data[best] = temp;

        index = best;
        
    }
}

Node* heap_extract_min(MinHeap *heap){
    Node *head = heap->data[0];
    heap->data[0] = heap -> data[heap->size-1];
    heap->size--;
    if (heap->size > 0){
    heapify_down(heap, 0);
    }

    return head;  
}

void heap_free(MinHeap *heap){}
void closed_list_free(ClosedList *list){}

int main(void){
    MinHeap open;
    ClosedList closed;
    heap_init(&open);
    closed_list_init(&closed);  
    char line[250];

    fgets(line, sizeof(line), stdin);
    while (fgets(line, sizeof(line), stdin)){
    if (strcmp(line, "done\n") == 0 || strcmp(line, "done") == 0){
        break;
    }
    int x, y, g, h;
    if (sscanf(line, "%d, %d, %d, %d", &x, &y, &g, &h) == 4){
        Node *node = create_node(x, y, g, h, NULL);
        heap_insert(&open, node);
    }
}

    while (fgets(line, sizeof(line), stdin)){
    if (strcmp(line, "[O] Extract\n") == 0 || strcmp(line, "[O] Extract") == 0){
        if (!heap_is_empty(&open)){
        Node *node = heap_extract_min(&open);
        closed_list_add(&closed, node);
        printf("Extracted Node (%d, %d), f = %d, g = %d, h = %d\n", node->x, node->y, node->f, node->g, node->h);
            }
        }

    else if (strcmp(line, "[O] IsEmpty\n") == 0 || strcmp(line, "[O] IsEmpty") == 0){
    if (heap_is_empty(&open)){
        printf("True\n");
    }
    else{
        printf("False\n");
    }
    }
    
    else if (strcmp(line, "[C] IsEmpty\n") == 0 || strcmp(line, "[C] IsEmpty") == 0) {
    if (closed.size == 0){
        printf("True\n");
    }
    else{
        printf("False\n");
    }
}

    else{
    int x, y, new_g; 
    if (sscanf(line, "[O] Find (%d, %d)", &x, &y) == 2){
        if (heap_find(&open, x, y) != -1){
            printf("Node (%d, %d) has been found\n", x, y);
        }
        else{
            printf("Node (%d, %d) has not been found\n", x, y);
        }
    }
    else if (sscanf(line, "[C] Find (%d, %d)", &x, &y) == 2){
        if (closed_list_contains(&closed, x, y)){
            printf("Node (%d, %d) has been found\n", x, y);
        }
        else{
            printf("Node (%d, %d) has not been found\n", x, y);
        }
    }
    else if (sscanf(line, "[O] DecreaseKey (%d, %d), new_g = %d", &x, &y, &new_g) == 3){
        int index = heap_find(&open, x, y); 

        if (index != -1){
            Node *node = open.data[index];  
            heap_decrease_key(&open, index, new_g, node->h); 
            printf("Decreased key for Node (%d, %d): f = %d, g = %d, h = %d\n",
                   node->x, node->y, node->f, node->g, node->h);  
        }
    }
}
    }

}