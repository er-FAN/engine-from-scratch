# Data-Oriented Design, Entity-Component-System Architecture, the Main Loop, and Delta Time

In this step, we want to build an initial understanding of Data-Oriented Design, the ECS architecture, how objects are defined and stored within it, the main simulation loop, and the concept of Delta Time. We will also use the SDL library to produce graphical output, which will make these concepts easier to grasp.

However, in this step — and in all subsequent steps — we do not intend to teach SDL itself. To be honest, I am not very familiar with it and rely on AI for that part. Our focus remains on the core objectives of the project.

**1. General Overview of Data-Oriented Design (DOD)**

A central idea of Data-Oriented Design (DOD) is to organize data according to how it is accessed and processed.

Suppose we have a program that needs to process a specific piece of information repeatedly. In Data-Oriented Design, we ask:

> *"How, and in what order, does the program access this data?"*

We then organize the data in a way that makes that access more efficient.

Data-Oriented Design has developed around considerations such as CPU performance, memory hierarchy, caching, and parallel processing, with the goal of structuring data to match the program's access patterns and improve performance. This means that simply knowing what Data-Oriented Design is and what it's for is not enough to produce a good design — we also need to understand some of the underlying hardware realities that motivate this approach, which is something we'll explore in more depth in later steps.

**2. What Is Entity-Component-System (ECS)?**

ECS is a programming architecture that separates data into Components and behavior into Systems, while Entities serve mainly as identifiers that tie those Components together. It broadly follows the principles of Data-Oriented Design.

In ECS, an object is divided into three main parts:

* **Entity:** Simply a unique identifier (ID) that ties together the different Components belonging to it; it holds no data or behavior of its own.
* **Component:** Holds data related to one specific aspect of an entity. An entity can have multiple Components.
* **System:** Contains the logic and behavior. Systems operate on groups of entities that share specific Components, rather than acting on a single isolated object.

As the project progresses, we'll become more familiar with ECS and Data-Oriented Design — or rather, we'll learn them together, step by step.

**3. The Difference Between Defining Objects in OOP and in ECS**

Anyone with even a little programming experience is likely familiar with OOP and how objects are defined within it. In OOP, data and behavior are typically organized around objects, most often defined through classes. Objects can also be related to one another through mechanisms such as inheritance and composition.

Defining an object in ECS, however, works quite differently — and, in my opinion, more creatively.

Defining an object in ECS is a bit like cooking by freely combining raw ingredients: as a chef, you take whatever amount of vegetables, legumes, meat, and spices you need and combine them. There are no fixed restrictions — every new combination of ingredients creates a completely new dish. Similarly, in ECS, instead of building complex inheritance trees, you simply attach various Components (such as `Position`, `Velocity`, or `Renderable`) to an Entity to form a new kind of object.

OOP, by the same analogy, is more like a fixed menu at a traditional restaurant: if you order a specific dish, its ingredients are predetermined (say, meat, vegetables, and legumes). If you then want an object that contains only "vegetables" and "legumes," without the meat, a traditional class-based design might require you to introduce an entirely new class or composition structure to represent that combination. As the number of possible combinations grows, the resulting class hierarchy or composition structure can become increasingly complex.

In ECS, by contrast, you can represent those combinations directly by attaching different Components to an Entity. If an entity no longer needs the "meat" component, you simply remove it — it's that simple.

**4. How Are Objects Stored in ECS?**

Continuing with the cooking analogy: a chef assigns a separate storage location to each type of raw ingredient depending on its nature — all the meats go on one shelf in the fridge, while the legumes go into jars on a pantry rack.

In the ECS design we're going to build, the data belonging to each Component type can be stored separately, so that similar data stays grouped together.

Take a car, for example: its position data can be stored in one place, its technical data (like velocity) in another, and its owner information somewhere else entirely. That same "owner information" storage area could then be reused for other kinds of objects too — a house, for instance — since ownership is the same concept regardless of whether the object is a car or a house.

Now — why is it done this way?

Go ask your mother — though don't phrase the question like this:

> *"Why, in ECS architecture, must an object's Components be stored in separate locations even though they all belong to the same Entity?"*

Because the answer you get might be a frying pan flying straight at your head — unless your mother happens to be a programmer!

The right way to ask would be:

> *"Why do you keep the meat in the fridge but the legumes in jars outside of it?"*

Although even then, she might start to question your sanity. So it's probably best not to ask at all.

Jokes aside, two questions arise here:

1. *Why is object storage handled this way in ECS?*
2. *What structural and hardware advantages does this storage method offer compared to the object-oriented approach?*

The answer has to do with access patterns. Since a System usually needs only one or a few specific Component types on any given run — rather than all Components at once — storing those Components separately can allow the System to access and process the data it actually needs more efficiently. The exact performance gain depends on how the data is organized and accessed. Naturally, part of the answer also has to do with CPU and memory architecture, which we'll explore further as we go deeper into Data-Oriented Design in later steps.

**5. The Main Loop**

In simulations or games, everything is "alive" and in motion — at every moment, objects react and update based on their own behavior, environmental rules, interactions with other objects, and user input.

For this to happen, we need a continuous loop that, on every iteration, gathers input, updates the program's logic, and renders the output (for example, to the screen via SDL). This loop is called the Main Loop, or Game Loop.

**6. Delta Time**

We said that a simulation needs a loop to continuously update all of its objects — but the crucial question is: by how much should each object change on every iteration?

Suppose we have a moving car, and on every loop iteration the car moves forward a little. But exactly how far should it move each time?

If we simply say "move 1 meter every frame," we run into a problem: the loop's execution speed varies across different hardware (and even under varying processing conditions on the same hardware). On a powerful computer, the loop might run 120 times per second, moving the car 120 meters in that second, while on a weaker computer it might run only 30 times, moving the car just 30 meters in the same amount of time.

The solution is to use the standard motion formulas from physics. For the simple case of constant velocity, displacement over a given time interval is:

**Δx = v · Δt**

By measuring the time that passes between loop iterations — what we call Delta Time, or **Δt** — we can calculate exactly how far the car should move during that particular frame.

This makes an object's movement depend on elapsed time rather than on the number of frames rendered, so its speed stays consistent across different frame rates.

**Final Words**

These explanations reflect my current understanding of the topic and may be refined or corrected as my own understanding deepens in later steps of the project.
