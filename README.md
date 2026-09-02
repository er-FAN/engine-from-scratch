# The Journey to a Simulation & Game Engine Library (C++)

> *The journey of building a modular C++ simulation & game engine library from scratch.*

---

## 📌 About The Project

The primary goal of this repository is to construct a practical C++ library designed for simulations (such as ecological biomes or physics systems) and game development. 

Rather than relying strictly on pre-existing frameworks, this project prioritizes **problem-solving from scratch**, facing memory and hardware performance challenges head-on, and discovering tailored architecture solutions through experimentation. While the journey begins with exploring **ECS (Entity-Component-System)** and **Data-Oriented Design**, the project remains completely flexible and unconstrained by rigid paradigms.

> 📖 **Personal Story & Philosophy:**  
> Read [MY_STORY.md](./MY_STORY.md) to discover the motivation, learning philosophy, and complete background behind this project.

---

## 🎯 Key Objectives & Approach

- **Practical Tooling:** Build a reusable, modular library applicable to real-world simulation scenarios.
- **Discovery over Predefined Tutorials:** Analyze and solve core architectural problems independently before comparing them with industry norms.
- **Hardware & Memory Awareness:** Study how CPU caching, memory layout, and RAM interactions impact performance in C++.
- **Structured Documentation:** Document every phase, benchmark, design decision, and lesson learned in dedicated stage-by-stage folders.

---

## 📂 Repository Structure

Progress and experiments are organized in sequential folders to track architectural evolution over time:

- **`docs/`**: Technical notes, hardware concepts, and research memory logs.
- **`MY_STORY.md`**: The narrative background and core development mindset.
- **`README.md`**: Overview and repository map.
- **`SETUP_SDL.md`**: One-time environment setup guide for SDL3 and SDL3_image, shared by every stage.
- **`Common/`**: The small shared rendering library (`SimpleSDL`) used across stages.
- **`step-xxxx-name/`**: Dedicated subfolders containing code, tests, and decision logs for each specific stage.

---

## 🖼️ Rendering & Windowing (SDL3)

Stages that involve any visual output (windows, drawing shapes, textures, etc.)
use **SDL3** together with **SDL3_image**, wrapped by a small internal helper
library called `SimpleSDL` (located in `Common/`). It handles window creation,
basic shape drawing, texture loading, and resource cleanup so each stage can
focus on its own logic instead of raw SDL boilerplate.

SDL3 and SDL3_image are **not included in this repository** to keep it
lightweight — they must be set up once on your own machine before building
any stage that depends on them.

➡️ **One-time setup:** see [`SETUP_SDL.md`](./SETUP_SDL.md) for downloading
SDL3/SDL3_image and pointing the project to them via an environment variable.
This only needs to be done once, regardless of how many stages you build.

---

## 🚀 Running a Stage

Each `step-xxxx-name/` folder is an independent, buildable project and has
**its own `README.md`** with:

- A description of what that specific stage demonstrates
- Any stage-specific setup notes (if applicable)
- The exact build and run commands for that folder

To get started with any stage:

1. Complete the one-time [SDL setup](./SETUP_SDL.md) (only needed once for the whole repository)
2. Open the `README.md` inside the stage folder you're interested in
3. Follow its build/run instructions

---

## 🛠 Tech Stack

- **Language:** C++
- **Focus Areas:** Systems programming, memory management, data layout, and performance optimization.
- **Rendering:** SDL3 / SDL3_image (via the internal `SimpleSDL` helper library)

---

## 🗺️ Roadmap (Dynamic & Evolving)

This project follows an iterative, experiment-driven process rather than a rigid blueprint:

- Iterative Development: Each iteration focuses on implementing a relatively complete system that addresses and resolves the limitations of the previous cycle.
- Practical Validation: The resulting system is used as a library to power one or more small demo projects.
- Feedback & Refinement: New issues, bottlenecks, and design flaws are identified, forming the groundwork for the next iteration.

---

## 📝 License

Distributed under the MIT License. Feel free to explore, fork, and learn from it.