# Fyredon Game Engine

A C++ game engine and editor featuring OpenGL rendering, an entity-component system, OBJ/FBX assets, animation, terrain, and navigation.

This repository continues the **Team Fyredon** group project originally hosted on [GitLab](https://gitlab.com/sc18ijf/comp5530m-group-project-team-fyredon-game-engine). The original team commit history and authorship are retained. Subsequent work by ShawnYangCG is maintained separately from the imported team branches.

## Branches

- `main`, `dev`, and the other original branch names preserve the imported GitLab development lines.
- `archive/local-main` and `archive/local-dev` preserve the local DEV-branch2 branch tips.
- `personal-dev` is the active continuation branch. It starts from the local `dev` revision and preserves its previously uncommitted changes as a separate commit. Newer GitLab changes have not been merged into this branch.

Large historical files are stored with Git LFS. Author and committer metadata are preserved, but affected commit IDs changed during conversion. See [migration details](docs/MIGRATION.md) and the [original-to-GitHub commit map](docs/gitlab-commit-map.csv).

## Clone

Install Git and Git LFS, then run:

```sh
git lfs install
git clone https://github.com/ShawnYangCG/Fyredon.git
cd Fyredon
git lfs pull
```

## Development Environment

- Windows, x64
- Visual Studio 2019 with C++ desktop development tools (v142)
- Windows SDK 10.0.19041.0
- GLEW, GLFW, GLM, Dear ImGui, FBX SDK, and other bundled dependencies in `Extern`

Generate the Visual Studio solution with `Setup.bat`, open `Fyredon.sln`, and build `Release | x64`. The client executable is generated under `bin/Release-windows-x86_64/Client`.

Set the client working directory to the repository's `Client` folder so that relative asset and shader paths resolve correctly.

## Structure

```text
Assets/       Models, textures, and other runtime resources
Client/       Editor application entry point and client configuration
Extern/       Third-party headers and binary dependencies
Fyredon/      Engine headers and implementation
Shaders/      OpenGL shader programs
Licenses/     Existing third-party license files
premake/      Project-generation tool
Out/          Engine output directory
docs/         Migration provenance and commit mapping
```

## Continuing Development

Use the GitHub continuation branch for new work:

```sh
git switch personal-dev
git push origin personal-dev
```

The original GitLab repository remains the source project. Its project membership, issues, and merge-request discussions are not copied by this Git history migration. Existing source notices and third-party license files remain in place.
