#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>

/* Vertex structure representing a node in a directed graph */
struct vertex
{
    int id;               // Unique identifier of the vertex
    int edge_count;       // Number of outgoing edges
    const int *edges;     // Array of IDs of vertices this vertex points to
};

/* Hash table entry for keeping track of visited vertices */
struct hash_value {
    struct vertex *vertex; // Pointer to the vertex
    bool visited;          // Whether this vertex was already visited
    bool used;             // Whether this slot in the hash table is occupied
};

/* Simple hash function for vertex IDs */
unsigned hash(int id)
{
    return id * 32413u;
}

/* Initialize hash table by marking all slots as unused */
void hash_init(struct hash_value *hash_set, int size)
{
    for (int i = 0; i < size; ++i)
        hash_set[i].used = false;
}

/* Insert a vertex into the hash table (linear probing for collisions) */
void hash_insert( struct hash_value *hash_set, int size, 
                  struct vertex *vertex )
{
    int idx = hash(vertex->id) % size;

    for (int i = 0; i < size; ++i)
    {
        if (hash_set[idx].used == false) {
            hash_set[idx].vertex = vertex;
            hash_set[idx].used = true;
            hash_set[idx].visited = false;
            return;
        }
        idx = (idx + 1) % size;
    }
}

/* Find a vertex in the hash table by its ID */
struct hash_value *hash_find(struct hash_value *hash_set, int size, int id) {
    int idx = hash(id) % size;
    int start = idx;

    while (hash_set[idx].used) {
        if (hash_set[idx].vertex->id == id)
            return &hash_set[idx];
        idx = (idx + 1) % size;
        if (idx == start)
            break;
    }
    return NULL;
}

/* 
 * Count the number of vertices reachable from the given initial vertex.
 * Uses BFS and a hash table to keep track of visited vertices.
 * Returns -1 if memory allocation fails.
 */
int count_reachable( const struct vertex *vertices,
                     int vertices_len, int initial )
{
    int *queue = malloc(sizeof(int) * vertices_len);
    if (queue == NULL)
        return -1;
    
    int hash_set_size = vertices_len * 2;
    struct hash_value *hash_set = malloc(sizeof(struct hash_value) * hash_set_size);
    if (hash_set == NULL) {
        free(queue);
        return -1;
    }

    hash_init(hash_set, hash_set_size);

    // Insert all vertices into hash table
    for (int i = 0; i < vertices_len; ++i)
        hash_insert(hash_set, hash_set_size, (struct vertex*)(&vertices[i]));

    int first = 0;
    int last = 0;

    int reachable_count = 0;
    
    queue[last++] = initial; 
    struct hash_value *val = hash_find(hash_set, hash_set_size, initial);
    val->visited = true;

    // BFS traversal
    while (first != last)
    {
        reachable_count += 1;
        int u_id = queue[first++];
        struct vertex *u = hash_find(hash_set, hash_set_size, u_id)->vertex;

        for (int i = 0; i < u->edge_count; ++i)
        {
            int v_id = u->edges[i];
            struct hash_value *v_val = hash_find(hash_set, hash_set_size, v_id);
            if (v_val->visited == true)
                continue;
            v_val->visited = true;
            queue[last++] = v_id; 
        }
    }

    free(queue);
    free(hash_set);

    return reachable_count;
}

/* Main function for tests */
int main()
{
    enum { vertices_len = 8 };

    int one[]    = { 3       },
        three[]  = { 1, 5, 7 },
        five[]   = { 1, 7    },

        nine[]   = { 11      },
        eleven[] = { 9       },

        two[]    = { 42      };

    struct vertex vertices[ vertices_len ] =
    {
        { .id =  1, .edge_count = 1, .edges = one   },
        { .id =  3, .edge_count = 3, .edges = three },
        { .id =  5, .edge_count = 2, .edges = five  },
        { .id =  7, .edge_count = 0, .edges = NULL  },

        { .id =  9, .edge_count = 1, .edges = nine   },
        { .id = 11, .edge_count = 1, .edges = eleven },

        { .id =  2, .edge_count = 1, .edges = two    },
        { .id = 42, .edge_count = 0, .edges = NULL   }
    };

    // Test reachability for individual vertices
    assert( count_reachable( vertices, vertices_len, 42 ) == 1 );
    assert( count_reachable( vertices, vertices_len,  2 ) == 2 );
    assert( count_reachable( vertices, vertices_len,  9 ) == 2 );
    assert( count_reachable( vertices, vertices_len,  1 ) == 4 );

    enum { k_len = 5, e_len = 4 };
    int e1[] =
    {
        1, 2, 3, 4,
        0, 2, 3, 4,
        0, 1, 3, 4,
        0, 1, 2, 4,
        0, 1, 2, 4
    };

    struct vertex kgraph[ k_len ] =
    {
        { .id = 0, .edge_count = e_len, .edges = e1 + e_len * 0 },
        { .id = 1, .edge_count = e_len, .edges = e1 + e_len * 1 },
        { .id = 2, .edge_count = e_len, .edges = e1 + e_len * 2 },
        { .id = 3, .edge_count = e_len, .edges = e1 + e_len * 3 },
        { .id = 4, .edge_count = e_len, .edges = e1 + e_len * 4 }
    };

    // Test reachability for fully connected graph
    assert( count_reachable( kgraph, k_len, 0 ) == k_len );
    assert( count_reachable( kgraph, k_len, 1 ) == k_len );
    assert( count_reachable( kgraph, k_len, 2 ) == k_len );
    assert( count_reachable( kgraph, k_len, 3 ) == k_len );
    assert( count_reachable( kgraph, k_len, 4 ) == k_len );

    printf("All tests passed!\n");
    return 0;
}
