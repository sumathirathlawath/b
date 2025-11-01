#include <stdio.h>
#include <string.h>

#define MAXN 20

int N, M;
char grid[MAXN][MAXN + 1];
int visited[MAXN][MAXN];

int dx[4] = {1, -1, 0, 0};
int dy[4] = {0, 0, 1, -1};

int inside(int x, int y) {
    return x >= 0 && y >= 0 && x < N && y < M;
}

void dfs(int x, int y, char g[MAXN][MAXN + 1]) {
    visited[x][y] = 1;
    for (int dir = 0; dir < 4; dir++) {
        int nx = x + dx[dir], ny = y + dy[dir];
        if (inside(nx, ny) && !visited[nx][ny] && g[nx][ny] == 'C') {
            dfs(nx, ny, g);
        }
    }
}


int is_unwrapped(char g[MAXN][MAXN + 1]) {
    memset(visited, 0, sizeof(visited));

  
    int sx = -1, sy = -1;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (g[i][j] == 'C' && (i == 0 || j == 0 || i == N - 1 || j == M - 1)) {
                sx = i;
                sy = j;
                break;
            }
        }
        if (sx != -1) break;
    }

    if (sx == -1) return 0; 
    dfs(sx, sy, g);

    
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (g[i][j] == 'C' && !visited[i][j]) return 0;
        }
    }

    return 1;
}

int main() {
    scanf("%d %d", &N, &M);
    for (int i = 0; i < N; i++) scanf("%s", grid[i]);

    
    int flip_x[400], flip_y[400], cnt = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (grid[i][j] == 'R') {
                
                for (int d = 0; d < 4; d++) {
                    int ni = i + dx[d], nj = j + dy[d];
                    if (inside(ni, nj) && grid[ni][nj] == 'C') {
                        flip_x[cnt] = i;
                        flip_y[cnt] = j;
                        cnt++;
                        break;
                    }
                }
            }
        }
    }

    int best = 10;

    if (is_unwrapped(grid)) {
        printf("0\n");
        return 0;
    }

    
    for (int f = 0; f < cnt; f++) {
        char g2[MAXN][MAXN + 1];
        memcpy(g2, grid, sizeof(grid));
        g2[flip_x[f]][flip_y[f]] = 'C'; 
        if (is_unwrapped(g2)) {
            if (best > 1) best = 1;
        }
    }

    
    for (int f1 = 0; f1 < cnt; f1++) {
        for (int f2 = f1 + 1; f2 < cnt; f2++) {
            char g2[MAXN][MAXN + 1];
            memcpy(g2, grid, sizeof(grid));
            g2[flip_x[f1]][flip_y[f1]] = 'C';
            g2[flip_x[f2]][flip_y[f2]] = 'C';
            if (is_unwrapped(g2)) {
                if (best > 2) best = 2;
            }
        }
    }

    if (best == 10) best = 3;
    printf("%d\n", best);
    return 0;
}
