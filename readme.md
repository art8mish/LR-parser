
# LR Parser

## Задача

Реализовать синтаксический анализ простого арифметического языка. Операции языка: сложение, вычитание, умножение, деление, скобки. Терминалами будут являться `+`, `-`, `*`, `/`, `(`, `)`.

## Описание решения

Реализация представляет из себя конечный автомат LR(0), где L - парсинг слева направо, R - правое порождение, 0 - отсутствие предпросмотра символов. Для разрешения конфликтов используется SLR(1), использующий предпросмотр на 1 символ для принятия решений.

## Грамматика

```math
\begin{flalign*}
E &\Rightarrow E + T | E - T | T \\
T &\Rightarrow T * F | T / F | F \\
F &\Rightarrow (E) | \text{id} 
\end{flalign*}
```
[Подробное построение LR-парсера](docs/lr-parser.md)

## Запуск

Зависимости:
```shell
sudo apt-get install -y build-essential cmake flex libfl-dev graphviz graphviz-dev libgtest-dev
```

Сборка:
```shell
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

Запуск:
```shell
./build/lr-parser < end2end/tests/test1.txt 
```

Результат построения AST-дерева можно найти в `output`:

![AST](pics/ast.png)
