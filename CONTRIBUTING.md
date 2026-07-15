# Contributing to FrameBridge

Thank you for your interest in contributing to FrameBridge! This document provides guidelines for contributing code, reporting issues, and submitting pull requests.

## Getting Started

### Prerequisites

- **Visual Studio 2022** with C++ Desktop Development workload
- **Windows SDK 10.0+**
- **Git** with submodule support
- **Inno Setup 6.2.0** (for installer builds)

### Setting Up the Development Environment

```powershell
# Fork the repository on GitHub, then clone your fork
git clone --recurse-submodules https://github.com/<your-username>/FrameBridge.git
cd FrameBridge

# Add upstream remote
git remote add upstream https://github.com/dreyvinixz/FrameBridge.git

# Open the solution in Visual Studio
start OptiScaler.sln
```

## Code Guidelines

### Precompiled Header (PCH) Usage

We use Precompiled Headers to significantly reduce compile times:

- **Source Files (`.cpp`):** Every source file must include `"pch.h"` as the **very first** non-comment line.
- **Header Files (`.h`):** **Never** include `"pch.h"` inside a header file.
- **Utility Code:** Do not add general-purpose utilities, macros, or global declarations to `pch.h`. Use `SysUtils.h` or create appropriate header files.

### Code Style

- Follow the `.clang-format` configuration included in the repository
- Use meaningful variable and function names
- Add comments for non-obvious logic, especially around DirectX hooks and GPU spoofing

### Commit Messages

Use clear, descriptive commit messages:

```
feat: add support for new game quirk (Game Name)
fix: resolve crash when using XeFG with exclusive fullscreen
docs: update build instructions for VS 2022 17.x
refactor: simplify upscaler input detection logic
```

## Pull Request Process

1. **Create a branch** from `master` for your changes
2. **Make your changes** following the code guidelines above
3. **Test locally** — build and verify the DLL works with at least one game
4. **Submit the PR** with a clear description of what changed and why
5. **Respond to feedback** from maintainers

### PR Checklist

- [ ] Code compiles without warnings on x64 Release
- [ ] PCH rules are followed (pch.h as first include in .cpp files)
- [ ] No hardcoded paths or user-specific values
- [ ] Game-specific quirks include the game name in comments
- [ ] New features include INI configuration options where applicable

## Reporting Issues

When reporting issues, please include:

- Game name and version
- GPU model and driver version
- FrameBridge version (from the overlay or installer)
- Steps to reproduce the issue
- Relevant log files (`OptiScaler.log`)

## License

By contributing to FrameBridge, you agree that your contributions will be licensed under the **GNU General Public License v3.0**.
