# Binary Search Tree

## О проекте
- Данный проект представляет собой реализацию бинарного дерева поиска (BST) на языке C.

## В рамках работы реализованы следующие возможности:

### Базовые операции
- `BST* initTree()`
- `int bstInsert(BST* tree, int value)`
  - дубликаты не добавляются
- `bool bstContains(BST* tree, int value)`
- `void bstDelete(BST* tree, int value)`
- `void bstFree(BST** treeRef)`
---

### Обходы (рекурсивные)
- `int* bstInorder(const BST* tree)`
- `int* bstPreorder(const BST* tree)`
- `int* bstPostorder(const BST* tree)`
---

### Статистика
- `int bstHeight(const BST* tree)`
- `int bstSize(const BST* tree)`
- `bool bstMin(const BST* tree, int* result)`
- `bool bstMax(const BST* tree, int* result)`
---

### Итератор
- `Iterator* iteratorInit(const BST* tree)`
- `bool iteratorHasNext(Iterator* it)`
- `bool iteratorNext(Iterator* iter, int* result)`
  - элемент возвращается через указатель `result`
- `void iteratorFree(Iterator* it)`

---

### Проверка корректности
- `bool bstIsValid(const BST* tree)`

---

### Нахождение k-ого минимального элемента
- `bool bstKthMin(const BST* tree, int k, int* result)`

---

### Объединение деревьев
- `BST* bstMerge(const BST* tree1, const BST* tree2)`

---

## Участники
- Караманов Карим (@Karimk5)
- Стариков Егор (@playboishaman)
- Кузьмищев Леонид (@qwekkh)

---

![BST demo](assets/hasbulla.mp4)