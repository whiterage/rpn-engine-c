// token_list.h — динамический список токенов для математического выражения
#ifndef TOKEN_LIST_H
#define TOKEN_LIST_H

// Тип токена: число, оператор, переменная, функция, левая/правая скобка
typedef enum {
    TOKEN_NUMBER,      // Число
    TOKEN_OPERATOR,    // Оператор (+, -, *, /, ^)
    TOKEN_VARIABLE,    // Переменная (x)
    TOKEN_FUNCTION,    // Функция (sin, cos, ...)
    TOKEN_LPAREN,      // Левая скобка
    TOKEN_RPAREN       // Правая скобка
} TokenType;

// Структура токена (один элемент выражения)
typedef struct {
    TokenType type;    // Тип токена
    char value[32];    // Строковое значение (для числа или функции)
    char op;           // Символ оператора (если оператор)
} Token;

// Динамический список токенов
typedef struct {
    Token* tokens;     // Массив токенов
    int size;          // Количество токенов
    int capacity;      // Ёмкость массива
} TokenList;

// Создаёт новый пустой список токенов
TokenList* create_token_list(void);
// Освобождает память, занятую списком токенов
void free_token_list(TokenList* list);
// Добавляет токен в список
void add_token(TokenList* list, Token token);

#endif