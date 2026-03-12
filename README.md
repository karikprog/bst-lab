# Binary Search Tree

## О проекте
- Данный проект представляет собой реализацию бинарного дерева поиска (BST) на языке C.

## В рамках работы реализованы следующие возможности:
### Базовые операции
- int bstInsert(BST* tree, int value) 
- bool bstContains(BST* tree, int value)
- void bstFree(BST** treeRef)

### Обходы (рекурсивные)
- void bstInorder(BST* tree)
- void bstPreorder(BST* tree) 
- void bstPostorder(BST* tree)

### Статистика
- int bstHeight(BST* tree) 
- int bstSize(BST* tree)
- int bstMin(BST* tree) 
  *Поведение на пустом дереве: возвращает 0*
- int bstMax(BST* tree)
  *Аналогично bstMin.*

### Итератор
- `Iterator* iteratorInit(BST* tree)`
  иницилизация
- `bool iteratorHasNext(Iterator* it)`
  проверка на наличие элемента в стеке
- `bool iteratorNext(Iterator* iter, int* result)`
  элемент из стека возвращается через адрес переменной в вызывающем коде. сигнатура `bool` выбрана ради обработки ошибок.
- `void iteratorFree(Iterator* it)`
  очистка итератора

### Проверка коректности
- bool bstIsValid(const BST* tree)

### Нахождение k-ого минимального элемента
- bool bstKthMin(const BST* tree, int k, int* result)

## Участники
- Караманов Карим (@Karimk5)
- Стариков Егор (@playboishaman)
- Кузьмищев Леонид(@qwekkh)