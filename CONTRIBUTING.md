# Contributing to Box2D QML

Thank you for your interest in contributing to Box2D QML! 

## How to Contribute

### Reporting Bugs

If you find a bug, please create an issue with:
- A clear description of the problem
- Steps to reproduce
- Expected vs actual behavior
- Your environment (Qt version, OS, compiler)
- Minimal code example if possible

### Suggesting Features

Feature requests are welcome! Please describe:
- The use case for the feature
- How it would work
- Why it would be useful
- Whether you're willing to implement it

### Pull Requests

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Make your changes
4. Test your changes thoroughly
5. Commit with clear messages (`git commit -m 'Add amazing feature'`)
6. Push to your fork (`git push origin feature/amazing-feature`)
7. Open a Pull Request

### Code Style

- Follow Qt/C++ conventions
- Use clear, descriptive variable names
- Comment complex logic
- Keep functions focused and small
- Use `const` and `&` appropriately

### Testing

- Test your changes with both Qt5 and Qt6 if possible
- Run the examples to ensure nothing breaks
- Add examples if introducing new features

## Development Setup

```bash
# Clone your fork
git clone https://github.com/YOUR_USERNAME/box2d.qml.git
cd box2d.qml

# Add upstream remote
git remote add upstream https://github.com/dpietruchowski/box2d.qml.git

# Create a feature branch
git checkout -b feature/my-feature

# Build and test
mkdir build && cd build
cmake ..
cmake --build .
```

## Areas for Contribution

- **Joints**: Implement joint types (revolute, prismatic, distance, etc.)
- **Particles**: Add particle system support
- **More Examples**: Add examples showing different use cases
- **Documentation**: Improve docs and tutorials
- **Testing**: Add unit tests and integration tests
- **Performance**: Optimize critical paths
- **Bug Fixes**: Fix existing issues

## Questions?

Feel free to open an issue for questions or join discussions.

Thank you for contributing! 🎉
