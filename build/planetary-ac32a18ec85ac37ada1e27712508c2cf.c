#include <math.h>
#include <stdio.h>
#include <stdlib.h>

struct Body {
    int id;
    double mass;
    double position[2];
    double velocity[2];
    double acceleration[2];
};

struct Body *read_system(const char *filename, int *n) {
    FILE *input = fopen(filename, "r");
    if (input == NULL) {
        fprintf(stderr, "Cannot open %s\n", filename);
        return NULL;
    }

    int count;
    if (fscanf(input, "%d", &count) != 1 || count <= 0) {
        fprintf(stderr, "Invalid body count.\n");
        fclose(input);
        return NULL;
    }

    struct Body *bodies = calloc(count, sizeof *bodies);
    if (bodies == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        fclose(input);
        return NULL;
    }

    int valid = 1;
    for (int i = 0; i < count && valid; ++i) {
        struct Body *b = &bodies[i];
        if (fscanf(input, "%d %lf %lf %lf %lf %lf",
                   &b->id, &b->mass,
                   &b->position[0], &b->position[1],
                   &b->velocity[0], &b->velocity[1]) != 6) {
            valid = 0;
            break;
        }
        if (!isfinite(b->mass) || b->mass <= 0.0) valid = 0;
        for (int k = 0; k < 2; ++k) {
            if (!isfinite(b->position[k]) || !isfinite(b->velocity[k]))
                valid = 0;
        }
        for (int j = 0; j < i; ++j) {
            if (bodies[j].id == b->id) valid = 0;
        }
    }

    fclose(input);
    if (!valid) {
        fprintf(stderr, "Invalid body record, mass, or duplicate identifier.\n");
        free(bodies);
        return NULL;
    }

    *n = count;
    return bodies;
}

int compute_accelerations(struct Body *bodies, int n) {
    /* Reduced units: G = 1. */
    const double minimum_distance = 1e-6;

    for (int i = 0; i < n; ++i) {
        bodies[i].acceleration[0] = 0.0;
        bodies[i].acceleration[1] = 0.0;
    }

    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            double dx = bodies[j].position[0] - bodies[i].position[0];
            double dy = bodies[j].position[1] - bodies[i].position[1];
            double r2 = dx * dx + dy * dy;
            if (!isfinite(r2) || r2 < minimum_distance * minimum_distance) {
                fprintf(stderr, "Invalid separation between bodies %d and %d.\n",
                        bodies[i].id, bodies[j].id);
                return 0;
            }

            double inverse_r3 = 1.0 / (r2 * sqrt(r2));
            bodies[i].acceleration[0] += bodies[j].mass * dx * inverse_r3;
            bodies[i].acceleration[1] += bodies[j].mass * dy * inverse_r3;
            bodies[j].acceleration[0] -= bodies[i].mass * dx * inverse_r3;
            bodies[j].acceleration[1] -= bodies[i].mass * dy * inverse_r3;
        }
    }
    return 1;
}

int step_verlet(struct Body *bodies, int n, double dt) {
    /* Half kick for all bodies. */
    for (int i = 0; i < n; ++i)
        for (int k = 0; k < 2; ++k)
            bodies[i].velocity[k] += 0.5 * dt * bodies[i].acceleration[k];

    /* Drift for all bodies. */
    for (int i = 0; i < n; ++i)
        for (int k = 0; k < 2; ++k)
            bodies[i].position[k] += dt * bodies[i].velocity[k];

    if (!compute_accelerations(bodies, n)) return 0;

    /* Second half kick for all bodies. */
    for (int i = 0; i < n; ++i)
        for (int k = 0; k < 2; ++k)
            bodies[i].velocity[k] += 0.5 * dt * bodies[i].acceleration[k];

    return 1;
}

int write_snapshot(FILE *output, const struct Body *bodies, int n, double time) {
    for (int i = 0; i < n; ++i) {
        if (fprintf(output, "%.15g %d %.15g %.15g %.15g %.15g\n",
                    time, bodies[i].id,
                    bodies[i].position[0], bodies[i].position[1],
                    bodies[i].velocity[0], bodies[i].velocity[1]) < 0)
            return 0;
    }
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Usage: %s input trajectory dt steps\n", argv[0]);
        return EXIT_FAILURE;
    }

    double dt;
    int steps;
    if (sscanf(argv[3], "%lf", &dt) != 1 || !isfinite(dt) || dt <= 0.0 ||
        sscanf(argv[4], "%d", &steps) != 1 || steps <= 0) {
        fprintf(stderr, "Time step and step count must be positive.\n");
        return EXIT_FAILURE;
    }

    int n;
    struct Body *bodies = read_system(argv[1], &n);
    if (bodies == NULL) return EXIT_FAILURE;
    if (!compute_accelerations(bodies, n)) {
        free(bodies);
        return EXIT_FAILURE;
    }

    FILE *trajectory = fopen(argv[2], "w");
    if (trajectory == NULL) {
        fprintf(stderr, "Cannot open %s\n", argv[2]);
        free(bodies);
        return EXIT_FAILURE;
    }

    int ok = fprintf(trajectory, "# time id x y vx vy\n") >= 0;

    /* Save the initial state, before the first integration step. */
    if (ok) ok = write_snapshot(trajectory, bodies, n, 0.0);

    for (int step = 0; step < steps && ok; ++step) {
        if (!step_verlet(bodies, n, dt)) {
            ok = 0;
            break;
        }
        double time = (step + 1) * dt;
        ok = write_snapshot(trajectory, bodies, n, time);
    }

    if (fclose(trajectory) == EOF) ok = 0;
    free(bodies);

    if (!ok) {
        fprintf(stderr, "Simulation stopped: integration or output error.\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
