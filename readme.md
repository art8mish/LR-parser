
# LR Parser

## Задача

Реализовать синтаксический анализ простого арифметического языка. Операции языка: сложение, вычитание, умножение, деление, скобки. Терминалами будут являться `+`, `-`, `*`, `/`, `(`, `)`.

## Описание решения

Реализация представляет из себя конечный автомат LR(0), где L - парсинг слева направо, R - правое порождение, 0 - отсутствие предпросмотра символов. Для разрешения конфликтов используется SLR(1), использующий предпросмотр на 1 символ для принятия решений.

## Грамматика $G'$
```math
\begin{flushleft}
E' &\Rightarrow E \
E &\Rightarrow E + T \| E - T \| T \\
T &\Rightarrow T * F \| T / F \| F \\
F &\Rightarrow (E) \| \text{id} 
\end{flushleft}
```

или

$E' \Rightarrow E$
$E \Rightarrow E + T \\ $
$E \Rightarrow E - T\\ $
$E \Rightarrow T \\ $
$T \Rightarrow T * F \\ $
$T \Rightarrow T / F \\ $
$T \Rightarrow F \\ $
$F \Rightarrow (E) \\ $
$F \Rightarrow \text{id} \\ $


Расширенная грамматика $G'$ используется для того, чтобы обозначить точку останова для автомата.

<details>
<summary> ###Построение LR-парсера </summary>

## Канонический набор пунктов

Построим каноничесикй набор пунктов, состояния из набора будут являться состояниями конечного автомата.

### Начальное состояние $I_0$

$ C = \text{CLOSURE}({[E' \Rightarrow \cdot E]})\\ $

$ \text{CLOSURE}({[E' \Rightarrow \cdot E]}): \\ $
$ E' \Rightarrow \cdot E \\ $
$ E \Rightarrow \cdot E + T \\ $
$ E \Rightarrow \cdot E - T \\ $
$ E \Rightarrow \cdot T \\ $
$ T \Rightarrow \cdot T * F \\ $
$ T \Rightarrow \cdot T / F \\ $
$ T \Rightarrow \cdot F \\ $
$ F \Rightarrow \cdot (E) \\ $
$ F \Rightarrow \cdot \text{id} \\ $

### Построение GOTO

#### 1. $I_1 = \text{GOTO}(I_0, E)$

$ E' \Rightarrow E \cdot \\ $
$ E \Rightarrow E \cdot + T \\ $
$ E \Rightarrow E \cdot - T\\ $

#### 2. $I_2 = \text{GOTO}(I_0, T)$

$ E \Rightarrow T \cdot \\ $
$ T \Rightarrow T \cdot * F \\ $
$ T \Rightarrow T \cdot / F \\ $

#### 3. $I_3 = \text{GOTO}(I_0, F)$

$ T \Rightarrow F \cdot \\ $

#### 4. $I_4 = \text{GOTO}(I_0, "(")$

$ F \Rightarrow (\cdot E) \\ $

$ \text{CLOSURE}({[F \Rightarrow (\cdot E)]}): \\ $

$ E \Rightarrow \cdot E + T \\ $
$ E \Rightarrow \cdot E - T\\ $
$ E \Rightarrow \cdot T \\ $
$ T \Rightarrow \cdot T * F \\ $
$ T \Rightarrow \cdot T / F \\ $
$ T \Rightarrow \cdot F \\ $
$ F \Rightarrow \cdot (E) \\ $
$ F \Rightarrow \cdot \text{id} \\ $

#### 5. $ I_5 = \text{GOTO}(I_0, \text{id}) $

$ F \Rightarrow \text{id} \cdot \\ $

#### 6. $ I_6 = \text{GOTO}(I_1, +) $

$ E \Rightarrow E + \cdot T \\ $

$ \text{CLOSURE}({[E \Rightarrow E + \cdot T]}): \\ $

$ T \Rightarrow \cdot T * F \\ $
$ T \Rightarrow \cdot T / F \\ $
$ T \Rightarrow \cdot F \\ $
$ F \Rightarrow \cdot (E) \\ $
$ F \Rightarrow \cdot \text{id} \\ $

#### 7. $I_7 = \text{GOTO}(I_1, -)$

$ E \Rightarrow E - \cdot T \\$

$\text{CLOSURE}({[E \Rightarrow E - \cdot T]}): \\$
- Аналогично $I_6$

#### 8. $I_8 = \text{GOTO}(I_2, *)$

$ T \Rightarrow T * \cdot  F \\$

$ \text{CLOSURE}({[T \Rightarrow T * \cdot  F]}): \\$ 

$ F \Rightarrow \cdot (E) \\$
$ F \Rightarrow \cdot \text{id} \\$

#### 9. $I_9 = \text{GOTO}(I_2, /)$

$ T \Rightarrow T / \cdot  F \\$

$\text{CLOSURE}({[T \Rightarrow T / \cdot  F]}): \\$ 

$ F \Rightarrow \cdot (E) \\ $
$ F \Rightarrow \cdot \text{id} \\ $

#### 10. $I_{10} = \text{GOTO}(I_4, E)$

$ F \Rightarrow (E \cdot) \\ $
$ E \Rightarrow E \cdot + T \\ $
$ E \Rightarrow E \cdot - T\\ $

Повторяющиеся переходы: $\\$
$ \text{GOTO}(I_4, T) = I_2 \\ $
$ \text{GOTO}(I_4, F) = I_3 \\ $
$ \text{GOTO}(I_4, "(") = I_4 \\ $
$ \text{GOTO}(I_4, \text{id}) = I_5 \\ $

#### 11. $I_{11} = \text{GOTO}(I_6, T)$

$E \Rightarrow E + T \cdot \\$
$T \Rightarrow T \cdot * F \\$
$T \Rightarrow T \cdot / F \\$

Повторяющиеся переходы: $\\$
$\text{GOTO}(I_6, F) = I_3 \\$
$\text{GOTO}(I_6, "(") = I_4 \\$
$\text{GOTO}(I_6, \text{id}) = I_5 \\$

#### 12. $I_{12} = \text{GOTO}(I_7, T)$

$E \Rightarrow E - T \cdot \\$
$T \Rightarrow T \cdot * F \\$
$T \Rightarrow T \cdot / F \\$

Повторяющиеся переходы: $\\$
$\text{GOTO}(I_7, F) = I_3 \\$
$\text{GOTO}(I_7, "(") = I_4 \\$
$\text{GOTO}(I_7, \text{id}) = I_5 \\$

#### 13. $I_{13} = \text{GOTO}(I_8, F)$

$T \Rightarrow T * F \cdot \\$

Повторяющиеся переходы: $\\$
$\text{GOTO}(I_8, "(") = I_4 \\$
$\text{GOTO}(I_8, \text{id}) = I_5 \\$

#### 14. $I_{14} = \text{GOTO}(I_9, F)$

$T \Rightarrow T / F \cdot \\$

Повторяющиеся переходы: $\\$
$\text{GOTO}(I_9, "(") = I_4 \\$
$\text{GOTO}(I_9, \text{id}) = I_5 \\$

#### 15. $I_{15} = \text{GOTO}(I_{10}, ")")$

$F \Rightarrow (E) \cdot \\$

Повторяющиеся переходы: $\\$
$\text{GOTO}(I_{10}, +) = I_6 \\$
$\text{GOTO}(I_{10}, -) = I_7 \\$

### Построение FIRST/FOLLOW

$\text{FIRST}(F) = \{"(", \text{id}\} \\$
$\text{FIRST}(T) = \text{FIRST}(F)  \\$
$\text{FIRST}(E) = \text{FIRST}(T) = \text{FIRST}(F)\\$

$\text{FOLLOW}(E) = \{\$, ")", +, - \} \\$
$\text{FOLLOW}(T) = \text{FOLLOW}(E) \bigcup \{*, /\} = \{\$, ")", +, -, *, / \}  \\$
$\text{FOLLOW}(F) = \text{FOLLOW}(T) = \{\$, ")", +, -, *, / \}  \\$


</details>


<!-- $ID1      * ID2$                  SHIFT     
$F      * ID2$                  REDUCE    ID1 -> F
$T      * ID2$                  REDUCE    F -> T
$T*      ID2$                    SHIFT
$T*ID2      $                    SHIFT
$T*F     $                    REDUCE ID2->F
$T      $                    REDUCE T*F->T
$E     $                    REDUCE T->E
$E`     $                    ACCEPT -->

