#include <iostream>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <ctime>

const double PI = 3.14;

// Define the Michalewicz function
double michalewicz(double x, double y, double m) {
    return - (sin(x) * pow(sin((x * x) / PI), 2) + pow(sin(y) * pow(sin((2 * y * y) / PI), 2), 2 * m));
}

// Define the PSO parameters
const int swarm_size = 30;
const int max_iterations = 1000;
const double inertia_weight = 0.7;
const double cognitive_weight = 1.5;
const double social_weight = 1.5;

struct Particle {
    double x, y;
    double velocity_x, velocity_y;
    double best_x, best_y;
    double fitness;
};

// Initialize particles randomly
void initialize_particles(std::vector<Particle>& particles) {
    for (auto& particle : particles) {
        particle.x = (rand() % 10000) / 10000.0 * PI;
        particle.y = (rand() % 10000) / 10000.0 * PI;
        particle.velocity_x = 0.0;
        particle.velocity_y = 0.0;
        particle.best_x = particle.x;
        particle.best_y = particle.y;
        particle.fitness = michalewicz(particle.x, particle.y, 10);
    }
}

// Update particle position and velocity
void update_particle(Particle& particle, double global_best_x, double global_best_y) {
    double r1 = (rand() % 10000) / 10000.0;
    double r2 = (rand() % 10000) / 10000.0;

    // Update velocity
    particle.velocity_x = inertia_weight * particle.velocity_x
                         + cognitive_weight * r1 * (particle.best_x - particle.x)
                         + social_weight * r2 * (global_best_x - particle.x);

    particle.velocity_y = inertia_weight * particle.velocity_y
                         + cognitive_weight * r1 * (particle.best_y - particle.y)
                         + social_weight * r2 * (global_best_y - particle.y);

    // Update position
    particle.x += particle.velocity_x;
    particle.y += particle.velocity_y;

    // Update fitness
    double new_fitness = michalewicz(particle.x, particle.y, 10);
    if (new_fitness < particle.fitness) {
        particle.fitness = new_fitness;
        particle.best_x = particle.x;
        particle.best_y = particle.y;
    }
}

int main() {
    srand(static_cast<unsigned int>(time(0)));

    std::vector<Particle> particles(swarm_size);
    double global_best_x, global_best_y, global_best_fitness;

    initialize_particles(particles);

    for (int iteration = 0; iteration < max_iterations; ++iteration) {
        // Find global best particle
        global_best_fitness = michalewicz(particles[0].best_x, particles[0].best_y, 10);
        global_best_x = particles[0].best_x;
        global_best_y = particles[0].best_y;

        for (const auto& particle : particles) {
            if (michalewicz(particle.best_x, particle.best_y, 10) < global_best_fitness) {
                global_best_fitness = michalewicz(particle.best_x, particle.best_y, 10);
                global_best_x = particle.best_x;
                global_best_y = particle.best_y;
            }
        }

        // Update particles
        for (auto& particle : particles) {
            update_particle(particle, global_best_x, global_best_y);
        }
    }

    // Print the result
    std::cout << "Optimal solution: x = " << global_best_x << ", y = " << global_best_y << std::endl;
    std::cout << "Minimum value: " << -global_best_fitness << std::endl;

    return 0;
}
