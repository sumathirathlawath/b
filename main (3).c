#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
    int x1, y1, x2, y2;
    int slope; // +1 or -1
} Slide;

int N;
Slide slides[60];

double min(double a, double b) { return a < b ? a : b; }
double max(double a, double b) { return a > b ? a : b; }

// Check if (x,y) is on a slide
int on_slide(double x, double y, Slide *s) {
    if (x < min(s->x1, s->x2) - 1e-9 || x > max(s->x1, s->x2) + 1e-9) return 0;
    if (y < min(s->y1, s->y2) - 1e-9 || y > max(s->y1, s->y2) + 1e-9) return 0;
    double expected = s->y1 + s->slope * (x - s->x1);
    return fabs(y - expected) < 1e-6;
}

// Find slide under given vertical line x=bx, below by
// returns 1 if found, sets *hx,*hy to highest intersection below by
int find_below(double bx, double by, double *hx, double *hy, int *idx) {
    double besty = -1;
    int found = 0;
    for (int i = 0; i < N; i++) {
        Slide *s = &slides[i];
        if (bx < min(s->x1, s->x2) - 1e-9 || bx > max(s->x1, s->x2) + 1e-9) continue;
        double yi = s->y1 + s->slope * (bx - s->x1);
        if (yi < by - 1e-9 && yi > besty) {
            besty = yi;
            *hx = bx;
            *hy = yi;
            *idx = i;
            found = 1;
        }
    }
    return found;
}

// Move along slide toward its lower end, reduce energy accordingly
void slide_down(double *x, double *y, int *E, Slide *s) {
    // determine direction (toward lower y)
    int targetx, targety;
    if (s->y1 < s->y2) {
        targetx = s->x1;
        targety = s->y1;
    } else {
        targetx = s->x2;
        targety = s->y2;
    }
    double dist = fabs(*x - targetx);
    if (*E <= 0) return;
    if (*E < dist) {
        // move partially
        double ratio = (double)(*E) / dist;
        *x += (targetx - *x) * ratio;
        *y += (targety - *y) * ratio;
        *E = 0;
        return;
    }
    *x = targetx;
    *y = targety;
    *E -= (int)dist;
}

int main() {
    scanf("%d", &N);
    for (int i = 0; i < N; i++) {
        scanf("%d %d %d %d", &slides[i].x1, &slides[i].y1, &slides[i].x2, &slides[i].y2);
        slides[i].slope = (slides[i].y2 - slides[i].y1) / (slides[i].x2 - slides[i].x1);
    }

    double x, y;
    int E;
    scanf("%lf %lf %d", &x, &y, &E);

    while (E > 0 && y > 0) {
        double nx, ny;
        int sid;

        // Gravity drop
        if (!find_below(x, y, &nx, &ny, &sid)) {
            // No slide below -> fall to ground
            y = 0;
            break;
        } else {
            y = ny;
            x = nx;
        }

        // Slide
        slide_down(&x, &y, &E, &slides[sid]);
        if (E <= 0 || y <= 0) break;

        // Check if now stuck (two slides meet)
        int count = 0;
        for (int i = 0; i < N; i++)
            if (on_slide(x, y, &slides[i])) count++;

        if (count > 1) { // stuck
            int cost = (int)(x * y);
            if (E < cost) {
                E = 0;
                break;
            }
            E -= cost;
        }
    }

    printf("%d %d\n", (int)(x + 0.5), (int)(y + 0.5));
    return 0;
}
