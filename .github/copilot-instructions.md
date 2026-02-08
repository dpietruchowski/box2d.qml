# Copilot Instructions for box2d.qml

## Code Style

Do not add obvious comments to code. Comments should explain WHY, not WHAT the code does. Let the code be self-documenting through clear naming and structure.

## Commit Guidelines

When making commits, use conventional commit format: `<type>: <short description>` where type is one of: feat (new feature), fix (bug fix), docs (documentation), style (formatting), refactor (code restructuring), test (tests), build (build system/dependencies), ci (CI/CD), or chore (other changes). Use present tense, imperative mood, don't capitalize first letter, no period at end, keep under 50 characters. Examples: `feat: add Box2D world wrapper class`, `fix: correct body positioning in QML`, `build: update CMakeLists for Box2D integration`.

### Commit Format
```
<type>: <short description>

[optional explanation - max 4 sentences, 72 chars per line]
```

Example:
```
fix: change body coordinate system

Changed from screen to world coordinates. Updated transformation
matrix. Fixed scaling issues. Added unit tests.
```

### Commit Flow
1. Check `git diff` to see what changed
2. Based on the diff, create appropriate commit message
3. Commit with `git add -A && git commit -m "message"`

**IMPORTANT**: Each line in the commit body MUST be 72 characters or less. Break long lines into multiple shorter lines.
