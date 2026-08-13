# The Story Behind the Project: From World Creation Dreams to Building a Library from Scratch

I have always wanted to create a simple simulation system—like a forest with animals and resources—and just sit back and watch their interactions unfold like a god.

This idea first sparked after watching a YouTube video by **Sebastian Lague** titled [Coding Adventure: Simulating an Ecosystem](https://youtu.be/r_It_X7v-1E), where he implemented that exact concept using C# and Unity.

### Early Experiments and Challenges

I immediately got to work. My first system was a basic console application simulating human marriages and births. By entering a target year, the program calculated population growth. However, simulating human behavior quickly grew overly complex.

I shifted focus toward simulating basic physics (movement, gravity, and collisions) for simple 2D shapes on screen. Since I had some familiarity with Unity, I decided to build a system inspired by Unity’s **Component System**—attaching a physics component to a GameObject to apply physical laws, or removing it to disable them.

At that time, I was unfamiliar with **ECS (Entity-Component-System)** architecture or **Data-Oriented Design (DOD)**. I attempted to implement this component system using Object-Oriented Programming (OOP) in C#. It was partially successful, and that codebase remains on my GitHub.

### Discovering ECS and Deepening Concepts

Later, while discussing code optimization with AI tools, I was introduced to the ECS architecture for the first time (though I don't recall whether I fully integrated ECS into that specific project).

Significant time has passed since then, during which my understanding of ECS, Data-Oriented Design, RAM mechanics, and CPU caching has grown substantially—along with an appreciation for how much data locality impacts simulation and game engine performance.

### The Habit of Building from Scratch

I have a strong habit of building things from scratch rather than relying on pre-built libraries or engines. Whether this is inherently good or bad is debatable, but its undeniable benefit is **deep, foundational learning**. Even if I eventually use off-the-shelf tools, building from scratch ensures I understand exactly what happens under the hood and how to leverage those tools effectively.

I could have easily built the ecosystem simulation using existing game engines or available ECS libraries. However, I wanted to accomplish something significant and deliver a strong, completed project for my portfolio, as I had left many past projects unfinished.

This motivation led to a key decision:
> **Build a functional C++ library and toolset for game engines and simulation systems. My starting point is experimenting with ECS architecture, but the ultimate goal is creating an optimized, practical tool—wherever the development process leads.**

I consciously controlled my impulse to start at an even lower level; otherwise, I might have attempted this in C or Assembly (even though I only have basic familiarity with them!).

### My Learning Philosophy: Discovery Through Experimentation

I adhere to a core principle: **When building something from scratch, it is better not to blindly follow tutorials or standard guides, but rather to discover both the problem and the solution independently.**

When breaking down the potential outcomes of this approach, two primary paths exist:
1. We **fail** to solve the problem.
2. We **succeed** in solving the problem.

If we succeed, three further scenarios arise:
* Our solution is **worse** than existing standard tools.
* Our solution is **equal** to existing standard tools.
* Our solution is **better** or yields a **novel approach**.

Across all **4 possible outcomes**, one outcome remains constant: **an exceptionally deep learning experience** (even if this mindset partly stems from my reluctance to follow conventional learning paths!).

I intend to navigate this project with that exact approach. Currently, my starting conceptual baseline rests on basic ECS principles:
* **Entity:** Identifier (ID)
* **Component:** Pure Data
* **System:** Logic & Behavior

Regarding Data-Oriented Design, my current baseline understanding is structuring data to maximize hardware efficiency. I will need to study how CPU caches and RAM process data layouts, as I have no desire to build hardware from scratch! However, these are merely baseline starting points, and the final architecture may evolve or pivot entirely based on encountered challenges.

### Boundaries and Moderation

Regarding building from scratch and avoiding rigid tutorials, I hold one caveat: **This approach must always be applied within reasonable boundaries.**

We can never start from "absolute zero" unless we are divine! Nor can we operate without any prior knowledge unless we built every underlying tool ourselves. 

The same applies to life: we must establish pragmatic boundaries and adjust them according to context. Making sound decisions depends on setting these limits properly, as nothing is absolute.

### Project Goals and the Role of AI

My goal is to document this entire journey within a **well-structured repository** where each folder represents a specific development stage. At every step, I will log my experiences, lessons learned, concepts grasped, and architectural decisions to serve as a resource for others.

I will also utilize **AI assistance** during this project, though strictly as a collaborative aid rather than a replacement for thinking:
1. **Editing and Refining Text:** Polishing documentation, `README` files, and markdown logs.
2. **C++ Syntax Assistance:** Resolving syntax errors or clarifying C++ semantics I have not yet mastered.
3. **Idea Generation & Pressure Testing:** Brainstorming concepts and challenging architectural assumptions.
4. **Learning Core Concepts:** Understanding non-negotiable concepts that cannot be built from scratch (such as hardware cache mechanics and memory alignment).
