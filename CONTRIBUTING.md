# Contributing Guidelines

Thank you for your interest in contributing to this project! To ensure a consistent and high-quality codebase, please follow these coding standards and workflow guidelines.

## Coding Standards

- **Language:** C (ESP-IDF/FreeRTOS style)
- **Naming Conventions:**
  - Variables: `snake_case`
  - Functions: `snake_case`
  - Macros/Constants: `UPPER_CASE`
  - File names: `snake_case.c` / `snake_case.h`
- **Indentation:** 4 spaces (no tabs)
- **Line Length:** 100 characters max
- **Braces:** K&R style (opening brace on same line)
- **Comments:**
  - Use Doxygen-style comments for public functions and structs
  - Inline comments for complex logic
- **Error Handling:**
  - Use ESP-IDF error codes and `ESP_ERROR_CHECK()` where appropriate
- **File Organization:**
  - Header files for public APIs, source files for implementation
  - Group related functions together

## Git Workflow

- **Branching:**
  - Create a new branch for each feature, bugfix, or documentation update: `feature/xyz`, `bugfix/abc`, `docs/readme-update`
- **Commits:**
  - Use [Conventional Commits](https://www.conventionalcommits.org/):
    - `feat:`, `fix:`, `docs:`, `refactor:`, `test:`, etc.
  - Reference issues in commit messages (e.g., `close #3`)
- **Pull Requests:**
  - Open a PR to `main` branch
  - Provide a clear description of changes
  - Link related issues (e.g., `Fixes #3`)
  - Ensure all checks pass before requesting review
- **Reviews:**
  - Address all review comments
  - Squash/fixup commits if needed before merging

## Issue Tracking

- Use GitHub Issues for bugs, feature requests, and questions
- Assign appropriate labels and provide clear descriptions

## Code Style Tools

- Use `clang-format` for C code formatting
- Run `idf.py lint` before submitting PRs (if available)

## Additional Notes

- Be respectful and constructive in all communications
- When in doubt, ask questions in the issue or PR comments

---

Happy coding! 🚀
