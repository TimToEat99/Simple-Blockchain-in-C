#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "hmap.h"
#include "deanon.h"
#include "cs152coin.h"

deanon_t* deanon_new()
{
    deanon_t* new = calloc(1, sizeof(deanon_t));
    if(new == NULL){
        return NULL;
    }

    new->hmap = hmap_new();
    if (new->hmap == NULL){
        free(new);
        return NULL;
    }

    new->visited = hmap_new();
    if(new->visited == NULL){
        hmap_free(new->hmap);
        free(new);
        return NULL;
    }
    return new;
}

void free_clust(const char* key, void* value){
    if(value == NULL){
        return;
    }
    for(cluster_t* c = value; c != NULL;){
        cluster_t* temp = c;
        c = c->next;
        free(temp->addr);
        free(temp);
    }
}

void deanon_free(deanon_t* d)
{
    hmap_foreach(&free_clust, d->hmap);
    hmap_free(d->hmap);
    free(d);
}

void add_address(const char* addr, deanon_t* d)
{
    hmap_put(addr, cluster_new(addr), d->hmap);
}


void add_associations(dep_wd_t* trans_inputs[], unsigned int num_inputs, deanon_t* d)
{
    for(int i = 0; i < num_inputs; i++){
        cluster_t* t = hmap_get(trans_inputs[i]->addr, d->hmap);
        cluster_t* temp = t;
        for(int j = 0; j< num_inputs; j++){
            if (strcmp(trans_inputs[j]->addr, trans_inputs[i]->addr)){
                while(temp->next){
                    temp = temp->next;
                }
                temp->next = cluster_new(trans_inputs[j]->addr);
            }
            else{
                continue;
            }
        }
    }
}

// creates a recursive function for the DFS
void rechelper(const char* addr, cluster_t* daddy, cluster_t* bigdaddy, deanon_t* d)
{
    hmap_put(addr, bigdaddy, d->visited);
    cluster_t* parent = hmap_get(addr, d->hmap);
    while(parent){
        if (!(hmap_get(parent->addr, d->visited))){
            daddy->next = cluster_new(parent->addr);
            daddy = daddy->next;
            rechelper(parent->addr, daddy, bigdaddy, d);
            parent = parent->next;
        }
        else{
            parent = parent->next;
        }
    }
}


cluster_t* get_cluster(const char* addr, deanon_t* d)
{
    if (!(hmap_get(addr, d->visited))){
        cluster_t* daddy = cluster_new(addr);
        rechelper(addr, daddy, daddy, d);
        return daddy;
    }
    else {
        cluster_t* already_returned = hmap_get(addr, d->visited);
        return already_returned;
    }
}

cluster_t* cluster_new(const char* addr)
{
    cluster_t* new = malloc(sizeof(cluster_t));
    new->addr = strdup(addr);
    new->next = NULL;
    return new;
}

void cluster_free(cluster_t* c)
{
    if(c == NULL){
        return;
    }
    for(cluster_t* temp = c; c != NULL;){
        c = c->next;
        free(temp->addr);
        free(temp);
    }
}


