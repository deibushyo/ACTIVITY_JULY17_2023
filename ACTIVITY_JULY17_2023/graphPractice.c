#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define SIZE 26

typedef char String[30];

typedef struct {
    int count;
    int max;
    String connection[5];
} Edge;

typedef struct {
    String key;
    Edge data;
} Node;

typedef struct {
    Node dict[SIZE];
} Dictionary;

int hash(String word_passed) {
    int value = word_passed[0] % SIZE;
    return value < SIZE ? value : 0;
}

void initDictionary(Dictionary *p, int size) {
    int i, j;
    for (i = 0; i < size; i++) {
        strcpy(p->dict[i].key, "");
        p->dict[i].data.count = 0;
        p->dict[i].data.max = 5;
        strcpy(p->dict[i].data.connection[0], "");
    }
}

bool addVertex(Dictionary *p, String word) {
    int index = hash(word);
    if (strcmp(p->dict[index].key, "") == 0) {
        strcpy(p->dict[index].key, word);
        return true;
    }
    return false;
}

bool addEdge(Dictionary *p, String word1, String word2) {
    int index = hash(word1);
    int *count = &p->dict[index].data.count;
    if (*count < p->dict[index].data.max) {
        strcpy(p->dict[index].data.connection[*count], word2);
        (*count)++;
        return true;
    }
    return false;
}

bool removeEdge(Dictionary *p, String vertex1, String vertex2) {
    int index1 = hash(vertex1);
    int index2 = hash(vertex2);

    // Remove vertex2 from the connection list of vertex1
    int i;
    for (i = 0; i < p->dict[index1].data.count; i++) {
        if (strcmp(p->dict[index1].data.connection[i], vertex2) == 0) {
            // Found the vertex to remove, so shift the rest of the connections to the left
            int j;
			for (j = i; j < p->dict[index1].data.count - 1; j++) {
                strcpy(p->dict[index1].data.connection[j], p->dict[index1].data.connection[j + 1]);
            }
            p->dict[index1].data.count--;
            break;
        }
    }

    // Remove vertex1 from the connection list of vertex2
    for (i = 0; i < p->dict[index2].data.count; i++) {
        if (strcmp(p->dict[index2].data.connection[i], vertex1) == 0) {
            // Found the vertex to remove, so shift the rest of the connections to the left
            int j;
			for (j = i; j < p->dict[index2].data.count - 1; j++) {
                strcpy(p->dict[index2].data.connection[j], p->dict[index2].data.connection[j + 1]);
            }
            p->dict[index2].data.count--;
            break;
        }
    }
    return true;
}

bool removeVertex(Dictionary *p, String vertex) {
    int index = hash(vertex);

    // Loop through all connections and remove vertex from their lists
    int i;
	for (i = 0; i < p->dict[index].data.count; i++) {
        String connectedVertex;
        strcpy(connectedVertex, p->dict[index].data.connection[i]);
        removeEdge(p, vertex, connectedVertex);
    }

    // Clear the data of the vertex being removed
    strcpy(p->dict[index].key, "");
    p->dict[index].data.count = 0;
    p->dict[index].data.max = 5;
    for (i = 0; i < 5; i++) {
        strcpy(p->dict[index].data.connection[i], "");
    }
    return true;
}

void displayDictionary(Dictionary *p) {
    printf("{\n");
    int i;
    for (i = 0; i < SIZE; i++) {
        if (strcmp(p->dict[i].key, "") != 0) {
            printf("  \"%s\": [", p->dict[i].key);
            int j;
			for (j = 0; j < p->dict[i].data.count; j++) {
                printf("\"%s\"", p->dict[i].data.connection[j]);
                if (j < p->dict[i].data.count - 1) {
                    printf(", ");
                }
            }
            printf("],\n");
        }
    }
    printf("}\n");
}


int main() {
    Dictionary place;
    initDictionary(&place, SIZE);
    addVertex(&place, "Tokyo");
    addVertex(&place, "Dallas");
    addVertex(&place, "Aspen");
    addVertex(&place, "Hong Kong");
    addVertex(&place, "Los Angeles");
    printf("%s\n", "VERTEX:");
    displayDictionary(&place);

    addEdge(&place, "Tokyo", "Dallas");
    addEdge(&place, "Tokyo", "Hong Kong");
    addEdge(&place, "Dallas", "Tokyo");
    addEdge(&place, "Dallas", "Aspen");
    addEdge(&place, "Dallas", "Hong Kong");
    addEdge(&place, "Dallas", "Los Angeles");
    addEdge(&place, "Aspen", "Dallas");
    addEdge(&place, "Hong Kong", "Tokyo");
    addEdge(&place, "Hong Kong", "Dallas");
    addEdge(&place, "Hong Kong", "Los Angeles");
    addEdge(&place, "Los Angeles", "Hong Kong");
    addEdge(&place, "Los Angeles", "Dallas");
    printf("\n%s\n", "EDGE:");
    displayDictionary(&place);
    
    removeEdge(&place, "Tokyo", "Dallas");
    printf("\n%s\n", "REMOVE EDGE: (Tokyo and Dallas)");
	displayDictionary(&place);
	
    removeVertex(&place, "Aspen");
    printf("\n%s\n", "REMOVE VERTEX: (Aspen)");
	displayDictionary(&place);
	
    return 0;
}

