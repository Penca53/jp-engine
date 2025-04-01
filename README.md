# SFML Engine

This repository contains a custom game engine and a sample 2D platfomer game demonstrating its features. The engine provides foundational components for building 2D games, including scene management, rendering, basic physics, input handling, and resource management.

## Core Engine Components

The `engine` is built upon several key modules:

* **Application (`app`):** The main entry point and core orchestrator. It manages the game loop, window creation and handling, and owns the root of the scene graph.
* **Scene Graph (`node`, `layer`):** The game world is structured as a tree of `Node` objects. Each `Node` can have a parent and multiple children, facilitating hierarchical transformations and event propagation (update, render). Nodes manage their own insertion/removal and recursively handle updates and rendering for their children. `Layer` provides a mechanism to group nodes (e.g., separating game elements from UI) for potentially different processing or rendering passes.
* **Rendering (`camera`, `sprite_sheet_animation`, `tilemap`, `tileset`, `tile`):**
    * `Camera`: Manages the viewport and view transformations, defining what part of the game world is visible.
    * Sprite & Animation: `SpriteSheetAnimation` handles frame-by-frame animation using texture sheets.
    * Tile System: `Tilemap` efficiently manages and renders large grids of `Tile` objects, defined within a `Tileset` which groups tiles from a single texture sheet.
* **Physics (`physics`, `collider`, `circle_collider`, `rectangle_collider`):** A basic physics simulation layer (`Physics`) manages collision detection. It uses an abstract `Collider` base class with concrete implementations like `CircleCollider` (radius-based) and `RectangleCollider` (rectangle-based).
* **Input (`input`):** Handles keyboard input, providing functions to query the state of keys (pressed, released, held).
* **Resource Management (`resource_manager`):** Loads and manages game assets like textures, fonts, and sounds. It acts as a cache to avoid redundant disk I/O operations when assets are requested multiple times.
* **State Management (`fsm`, `state`, `transition`):** A generic Finite State Machine (`FSM`) implementation. It uses `State` objects (with entry, update, exit logic) and `Transition` objects (defining conditions to move between states). In the sample game, this is used for managing animations.
* **Utilities (`derived`):** Includes helper components, such as `derived` for enforcing generic type constraints.

## Sample 2D Platformer Game Components

A sample platformer-style `game` is included to demonstrate the engine's usage.
It covers most of essential functionalities that the engine provides, making it a perfect fit for those who like learning hands-on.

The demo `game` features:

* **Entry Point (`main`):** Initializes the engine's `App` and starts the game.
* **Scene Setup (`default_scene`, `background`):** `default_scene` constructs the entire game level layout, including placing the `background`.
* **Player (`player`):** The controllable player character.
* **Camera Control (`follow_player`):** A component likely attached to the `Camera` node to make it follow the player (`player`).
* **Enemies (`mushroom`, `plant`, `plant_bullet`):** Different types of enemies with their specific behaviors and projectiles (`plant_bullet`).
* **Collectibles (`banana`):** Items the player can collect, represented as bananas.
* **Level Goal (`end`):** Represents the end-of-level trigger or object (e.g., the cup).
* **Game Logic (`game_manager`, `score_manager`):**
    * `GameManager`: Tracks and controls the overall state of the game (e.g., playing, paused, won, lost).
    * `ScoreManager`: Manages the player's score and updates the UI display.
* **User Interface (`win_canvas`, `lose_canvas`):** UI screens displayed on game win or game lose conditions.

### Win

![Win](https://github.com/user-attachments/assets/2aed5a89-1601-4a53-8160-eff83c9bd329)

The goal of the character is to reach the "end cup" while avoiding to fall or to take damage from the various enemies along the way.

### Loss

![DeathFall](https://github.com/user-attachments/assets/93409bb5-18d4-4a05-8e61-38836c6b7e7b)

![DeathMushroom](https://github.com/user-attachments/assets/3f3f906a-3dda-40d7-9e2d-279038d7c693)

![DeathPlantBullet](https://github.com/user-attachments/assets/5e7e30f2-cad1-4f77-a6c6-80503322d287)





