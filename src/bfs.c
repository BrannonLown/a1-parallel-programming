#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

typedef struct {
    int n;
    long m;
    long *offsets;
    int *adj;
} graph_t;

graph_t read_mtx(const char *path) {
    FILE *f = fopen(path, "r");
    if (!f) {
        fprintf(stderr, "cannot open %s\n", path);
        exit(1);
    }

    char line[4096];
    long n_rows = 0;
    long n_cols = 0;
    long n_edges = 0;
    while (fgets(line, sizeof(line), f)){
        if (line[0] == '%'){
            continue;
        }
        sscanf(line, "%ld %ld %ld", &n_rows, &n_cols, &n_edges);
        break;
    }

    int n = (int)n_rows;
    long *degree = calloc(n, sizeof(long));
    long *edge_u = malloc(n_edges * sizeof(long));
    long *edge_v = malloc(n_edges * sizeof(long));

    long count = 0;
    long u, v;
    while (count < n_edges && fscanf(f, "%ld %ld", &u, &v) == 2) {
        u -= 1;
        v -= 1;
        edge_u[count] = u;
        edge_v[count] = v;
        degree[u]++;
        degree[v]++;
        count++;
    }
    fclose(f);

    graph_t g;
    g.n = n;
    g.m = 2 * count;
    g.offsets = malloc((n+1) * sizeof(long));
    g.adj = malloc(g.m * sizeof(int));

    g.offsets[0] = 0;
    for(int i =0; i < n; i++){
        g.offsets[i + 1] = g.offsets[i] + degree[i];
    }

    long *cursor = malloc(n * sizeof(long));
    for(int i=0; i < n; i++){
        cursor[i] = g.offsets[i];
    }
    for (long e = 0; e < count; e++){
        g.adj[cursor[edge_u[e]]++] = (int)edge_v[e];
        g.adj[cursor[edge_v[e]]++] = (int)edge_u[e];
    }

    free(degree);
    free(edge_u);
    free(edge_v);
    free(cursor);
    return g;
}

typedef struct{
    int levels;
    int reached;
    long edges_inspected;
} bfs_result_t;

bfs_result_t bfs_run(graph_t *g, int source, int *dist, int *frontier, int *next_frontier, long *level_sizes) {
    bfs_result_t r;
    r.levels = 0;
    r.reached = 1;
    r.edges_inspected = 0;

    for (int v = 0; v < g->n; v++){
        dist[v] = -1;
    }
    dist[source] = 0;

    frontier[0] = source;
    long frontier_len = 1;
    int level = 0;

    while (frontier_len > 0) {
        level_sizes[level] = frontier_len;
        long next_len = 0;

        for (long i = 0; i < frontier_len; i++) {
            int u = frontier[i];
            long start = g->offsets[u];
            long end = g->offsets[u+1];
            for (long e = start; e < end; e++){
                r.edges_inspected++;
                int v = g->adj[e];
                if (dist[v] == -1) {
                    dist[v] = level + 1;
                    next_frontier[next_len++] = v;
                    r.reached++;
                }
            }
        }

        int *tmp = frontier;
        frontier = next_frontier;
        next_frontier = tmp;
        frontier_len = next_len;
        level++;
    }

    r.levels = level;
    return r;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s <graph.mtx>\n", argv[0]);
        return 1;
    }

    graph_t g = read_mtx(argv[1]);
    fprintf(stderr, "loaded: n=%d, m=%ld\n", g.n, g.m);

    int *candidates = malloc(g.n * sizeof(int));
    int n_candidates = 0;
    for (int v = 0; v < g.n; v++){
        if (g.offsets[v+1] > g.offsets[v]) {
            candidates[n_candidates++] = v;
        }
    }

    int num_sources = 16;
    srand(1);
    for (int i = 0; i < num_sources; i ++) {
        int j = i + rand() % (n_candidates - i);
        int tmp = candidates[i];
        candidates[i] = candidates[j];
        candidates[j] = tmp;
    }

    int *dist = malloc(g.n * sizeof(int));
    int *frontier = malloc(g.n * sizeof(int));
    int *next_frontier = malloc(g.n * sizeof(int));
    long *level_sizes = malloc(g.n * sizeof(long));
    double teps[16];

    bfs_result_t warmup_r = bfs_run(&g, candidates[0], dist, frontier, next_frontier, level_sizes);

    fprintf(stderr, "level sizes for source %d:\n", candidates[0]);
    for (int lv = 0; lv < warmup_r.levels; lv++) {
        fprintf(stderr, " level %d: %ld vertices\n", lv, level_sizes[lv]);
    }

    printf("source, levels, reached, fraction_reached, edges_inspected, elapsed_s, teps\n");
    for (int s = 0; s < num_sources; s++) {
        int src = candidates[s];

        struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC, &start);
        bfs_result_t r = bfs_run(&g, src, dist, frontier, next_frontier, level_sizes);
        clock_gettime(CLOCK_MONOTONIC, &end);

        double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
        double rate = (double)r.edges_inspected / elapsed;
        teps[s] = rate;

        double fraction = (double)r.reached / (double)g.n;
        printf("%d,%d,%d,%f,%ld,%f,%f\n", src, r.levels, r.reached, fraction, r.edges_inspected, elapsed, rate);
    }

    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            if (teps[j] > teps[j+1]){
                double t = teps[j];
                teps[j] = teps[j+1];
                teps[j+1] = t;
            }
        }
    }
    double min_teps = teps[0];
    double max_teps = teps[15];
    double median_teps = (teps[7] + teps[8]) / 2;
    fprintf(stderr, "summary: min_teps=%f median_teps=%f max_teps=%f\n", min_teps, median_teps, max_teps);

    free(candidates);
    free(dist);
    free(frontier);
    free(next_frontier);
    free(level_sizes);
    free(g.offsets);
    free(g.adj);
    return 0;
}