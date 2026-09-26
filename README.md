# SharedPtr

Учебная реализация shared pointer на C++20.

## Реализовано

- Reference counting
- Control block
- Copy/move constructors
- Copy/move assignment
- Single objects
- Dynamic arrays
- `operator*`
- `operator->`
- `operator[]`
- `Get()`
- `UseCount()`
- Корректный `delete` / `delete[]`

## Tests

Проект использует GoogleTest.

```bash
make test
```
## AddressSanitizer:

```bash
make asan-test
```

## macOS Leaks:

```bash
make leaks-test
```

## Static Analyzer:

```bash
make analyze-test
```


