# N-Body Simulation

A 2D N-body simulation written in C++, using a quadtree for spatial partitioning and Barnes-Hut approximation to efficiently calculate gravitational forces between particles.

## naive approach @ 1000 particles

<video src="./assets/demo/brute_force_1k.mp4" controls="controls" muted="muted" width="100%"></video>

## barnes hut approximation @ 10,000 particles

<video src="./assets/demo/qtree_10k.mp4" controls="controls" muted="muted" width="100%"></video>

## quadtree visualization @ 5000 particles

<video src="./assets/demo/qtree_visual_5k.mp4" controls="controls" muted="muted" width="100%"></video>
