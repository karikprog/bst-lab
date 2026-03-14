### В публичную структуру войдут:
- BST* initTree() для объявления множества
- void bstInsert(BST* tree, int value) для добавления элемента в множество
- bool bstContains(BST* tree, int value) для проверки на наличие элемента в множестве
- void bstFree(BST** treeRef) для удаления множества
- int bstSize(BST* tree) для получения размера множества
- int bstMin(BST* tree), int bstMax(BST* tree) для нахождения минимума и максимума мнодества
- void bstDelete(BST* tree, int value) для удаления элемента из множества
- BST* bstMerge(BST* tree1, BST* tree2) для объединения двух множеств
- Iterator* iteratorInit(BST* tree), \ int iteratorNext(Iterator* it), \ void iteratorFree(Iterator* it) для итерации по множеству

---

### Необходимые статические функции: 
- void cleanTree(Node* root): \ нужна в void bstFree(BST** treeRef)
- Node* search(Node* node, int value): нужна в bool bstContains(BST* tree, int value)
- static void pushLeft(Iterator* iter, Node* node), \ bool iteratorHasNext(const Iterator* iter) нужны для итерации
- static void buildBalanced(BST* tree, const int* arr, const int START, const int END) нужна для BST* bstMerge(const BST* tree1, const BST* tree2)
- static Node* findParent(Node* root, Node* node) нужна в void bstDelete(BST* tree, int value)

---

### Функции, не используемые множеством:
обходы, они специфичны для деревьев, а в множестве не гарантируется порядок выведения элементов
int bstHeight(const BST* tree), bool bstIsValid(const BST* tree) так же специфичны для деревьев и множеством не поддерживаются 
int bstKthMin(BST* tree, int k) не стандартная функция для множества, могут возникнуть проблемы в случае замены реализации множества

---

Чтобы безболезненно заменять реализацию нужно поддерживать инкапсалюцию, то есть давать пользователю функции наподобие:
- Set* setCreate(void);
- bool setInsert(Set* set, int value);
- bool setContains(Set* set, int value);
- bool setRemove(Set* set, int value);
- int setSize(Set* set);
- bool setMin(Set* set, int* result);
- bool setMax(Set* set, int* result);

---

Тогда можно иметь несколько реализаций, которые скрыты за ними:
- bst_set.c, в которой эти функции написаны через BST* initTree(), int bstSize(BST* tree) и т.д.
- hash_table_set.c, в которой эти функции написаны через соответствующие функции для работы с хэш-таблицами
