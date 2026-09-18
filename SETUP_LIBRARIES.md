# Setting up SDL3, SDL3_image and ImGui for this project

This project depends on **SDL3**, **SDL3_image**, and **Dear ImGui**. None of
these are included in the repository (to keep its size small), so before
building any stage of the project, you need to point your system to their
location once. This setup is **shared across all stages of the project** and
only needs to be done a single time.

---

## Step 1: Download SDL3

Download the prebuilt development libraries from here:

🔗 https://github.com/libsdl-org/SDL/releases

- **Windows:** grab the file named `SDL3-devel-x.x.x-VC.zip` (for Visual Studio / MSVC)
- **Linux/macOS:** grab the file for your OS, or install it via your system's package manager (`apt`, `brew`, ...)

Extract the zip and place its contents (the `include`, `lib`, and on Windows
`bin` folders) anywhere you like on your system. Example:

```
C:\Libs\SDL3
```

## Step 2: Download SDL3_image

From the same kind of releases page, but for the SDL_image repository:

🔗 https://github.com/libsdl-org/SDL_image/releases

Download and extract the version for your OS (`SDL3_image-devel-x.x.x-VC.zip`
on Windows). **It's recommended to place this folder right next to (at the
same level as) the SDL3 folder**, since the project's default configuration
looks for it there:

```
C:\Libs\
├── SDL3\
├── SDL3_image\
└── imgui\
```

> If you placed the SDL3_image folder somewhere else, that's fine too — see
> the "Advanced settings" section below for how to point to a custom path.

## Step 3: Download Dear ImGui

Unlike SDL3 and SDL3_image, ImGui is **not distributed as a prebuilt
library** — it's used as raw source code that gets compiled as part of the
project. You get it by cloning (or downloading) the repository:

🔗 https://github.com/ocornut/imgui

**Option A — clone with git (recommended):**
```bash
git clone https://github.com/ocornut/imgui.git
```

**Option B — download as a zip:**
Go to the repository page, click "Code" → "Download ZIP", and extract it.
Make sure the extracted folder is named `imgui` (not `imgui-master` or
similar) and directly contains `imgui.h`, `imgui.cpp`, and a `backends/`
subfolder.

**Just like SDL3_image, place this folder right next to your SDL3 folder**,
since the project's default configuration looks for it there:

```
C:\Libs\
├── SDL3\
├── SDL3_image\
└── imgui\
```

> No separate build step is needed for ImGui — the project compiles its
> source files automatically as part of your build. If you placed it
> somewhere else, see "Advanced settings" below.

---

## Step 4: Point the project to SDL3 (Environment Variable)

You need to create an environment variable named `SDL3_ROOT` that points to
the SDL3 folder.

### Windows

Pick one of these two approaches:

**Option 1 (recommended) — via terminal:**
Open PowerShell or Command Prompt and run:
```
setx SDL3_ROOT "C:\Libs\SDL3"
```
After seeing `SUCCESS: Specified value was saved`, close the terminal
completely and reopen it (and if you're using an IDE like Visual Studio or
VS Code, close and reopen that too).

**Option 2 — via the Windows GUI:**
1. Search for "Environment Variables" in the Start Menu and open "Edit the system environment variables"
2. Click the "Environment Variables..." button
3. Under "User variables", click "New..."
4. Variable name: `SDL3_ROOT` — Variable value: the SDL3 folder path (e.g. `C:\Libs\SDL3`)
5. Click OK, close all windows, then reopen your terminal/IDE

### Linux / macOS

Add the following line to your `~/.bashrc` or `~/.zshrc` (depending on your shell):
```bash
export SDL3_ROOT=/home/username/libs/SDL3
```
Then close and reopen your terminal, or run `source ~/.bashrc`.

### Verifying it's set correctly

**Windows (Command Prompt):**
```
echo %SDL3_ROOT%
```
**Windows (PowerShell):**
```
$env:SDL3_ROOT
```
**Linux/macOS:**
```
echo $SDL3_ROOT
```
If you see the path you entered, it's set correctly.

---

## Step 5: Point the project to SDL3_image and ImGui (optional in most cases)

If you placed **SDL3_image** and **imgui** right next to SDL3 (as in Steps 2
and 3), **you don't need to do anything else** — the project automatically
resolves both paths from `SDL3_ROOT`.

If either one is located somewhere else, create the matching environment
variable the same way as `SDL3_ROOT` above:
```
setx SDL3_IMAGE_ROOT "path/to/SDL3_image"
setx IMGUI_ROOT "path/to/imgui"
```

---

## Alternative: without Environment Variables (temporary)

If you'd rather not create environment variables, you can pass the paths
directly when running `cmake`. This only applies to that single build and
must be repeated every time you delete the `build` folder:

```
cmake -S . -B build -DSDL3_ROOT="C:\Libs\SDL3" -DSDL3_IMAGE_ROOT="C:\Libs\SDL3_image" -DIMGUI_ROOT="C:\Libs\imgui"
```

---

## If you get a "library not found" error

If you hit a CMake configuration error like this:

```
SDL3.lib not found at this path: ...
```

Check the following:

1. **Correct path:** make sure `SDL3_ROOT` points exactly to the folder that
   directly contains the `include` and `lib` subfolders (not a folder
   above or below it)
2. **Architecture:** some SDL3 distributions have separate subfolders inside
   `lib` for `x64`, `x86`, and `arm64`. Make sure the version you downloaded
   matches your build architecture (usually x64)
3. **Stale CMake cache:** if you just set `SDL3_ROOT` but had already
   configured the `build` folder once before (without it set), delete it
   and reconfigure:
   ```
   rmdir /s /q build
   cmake -S . -B build
   ```
4. **Old terminal/IDE session:** after running `setx`, any terminal or IDE
   that was already open won't see the new value — it needs to be fully
   closed and reopened

### ImGui-specific errors

If you hit an error like:

```
imgui path not found or incorrect: ...
(imgui.h not found there)
```
or
```
SDL3 backend not found at: .../backends/imgui_impl_sdl3.h
```

Check the following:

1. **Correct folder, not a subfolder:** `IMGUI_ROOT` (or its default next to
   `SDL3_ROOT`) must point to the folder that directly contains `imgui.h`
   and a `backends/` subfolder — not `imgui-master`, not a folder one level
   too deep or too shallow.
2. **Full clone, not a partial download:** make sure the `backends/` folder
   was actually included (a zip download from GitHub includes it by
   default, but a shallow or filtered clone might not).
3. **Stale CMake cache:** same as above — delete `build` and reconfigure
   after fixing the path.