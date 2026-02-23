

# Построение LR-парсера

## Грамматика $G'$

Расширенная грамматика $G'$ используется для того, чтобы обозначить точку останова для автомата. Добавляется правило $E' \Rightarrow E$:

```math
\begin{align*}
 E' &\Rightarrow E &&\text{(0)}\\
E &\Rightarrow E + T &&\text{(1)}\\
E &\Rightarrow E - T &&\text{(2)}\\
E &\Rightarrow T &&\text{(3)}\\
T &\Rightarrow T * F &&\text{(4)}\\
T &\Rightarrow T / F &&\text{(5)}\\
T &\Rightarrow F &&\text{(6)}\\
F &\Rightarrow (E) &&\text{(7)}\\
F &\Rightarrow \text{id} &&\text{(8)}
\end{align*}
```

## Канонический набор пунктов LR(0)

При построение будут использоваться **пункты** - правила, показывающие в каком состоянии парсинга находится автомат. Обозначаются с использованием $"\cdot"$: $A \Rightarrow a\cdot b c$ значит, что считан символ `a`, а следующий `b`. 

Канонический набор пунктов представляет состояния конечного автомата.

### Инструменты

I представляет из себя множество пунктов. Над этим множеством можно совершать некоторые операции.

#### $\text{CLOSURE}(I)$

Алгоритм:
1. Все пункты из $I$ добавляются $\text{CLOSURE}(I)$.
2. Если в $\text{CLOSURE}(I)$ добавляется пункт вида $A \Rightarrow a \cdot B b$, где B - нетерминал, то для каждого правила $B \Rightarrow c$ в $\text{CLOSURE}(I)$ добавляется пункт $B \Rightarrow \cdot c$.
3. Повторяется шаг 2, пока происходят изменения.

#### $\text{GOTO}(I, X)$

Алгоритм:
1. Для всеx пунктов $I$, вида $A \Rightarrow a \cdot X b$ $\cdot$ перемещается в позицию после $X$: $A \Rightarrow a X \cdot b$
2. Применяется $\text{CLOSURE}(I')$ к полученному множеству пунктов

Алгоритм построения кононического набора пунктов

#### $\text{FIRST}(X)$

Представляет из себя множество терминалов, с которых могут начинаться строки, выводимые из $X$. Например, $\text{FIRST}(F) = \{ "(", \text{id} \}$

#### $\text{FOLLOW}(X)$

Представляет из себя множество терминалов, которые могут встретиться сразу после ннетерминала $X$. Например, $\text{FOLLOW}(E) = \{ \$, +, -, ")" \}$, где $ - маркер конца строки.

### Алгоритм построения канонического набора пунктов

1. Начальное состояние представляет $\text{CLOSURE}$ от начального пункта $I_0 = \text{CLOSURE}({[E' \Rightarrow \cdot E]})$. Добавляются все соответствующие состояния по алгоритму $\text{CLOSURE}$.

2. Для каждого существующего состояния $I$ и каждого символа $X$ вычисляется $\text{GOTO}(I, X)$.

3. Повторяется шаг 2, пока происходят изменения.

### Начальное состояние $I_0$

$$ C = \text{CLOSURE}(\{[E' \Rightarrow \cdot E]\}) $$
$$\text{CLOSURE}(\{[E' \Rightarrow \cdot E]\}):$$
```math
\begin{align*}
E' &\Rightarrow \cdot E \\
E &\Rightarrow \cdot E + T \\
E &\Rightarrow \cdot E - T \\
E &\Rightarrow \cdot T \\
T &\Rightarrow \cdot T * F \\
T &\Rightarrow \cdot T / F \\
T &\Rightarrow \cdot F \\
F &\Rightarrow \cdot (E) \\
F &\Rightarrow \cdot \text{id}
\end{align*}
```

### Построение GOTO

#### $I_1 = \text{GOTO}(I_0, E)$:
```math
\begin{align*}
E' &\Rightarrow E \cdot \\
E &\Rightarrow E \cdot + T \\
E &\Rightarrow E \cdot - T
\end{align*}
```

#### $I_2 = \text{GOTO}(I_0, T)$:

```math
\begin{align*}
E &\Rightarrow T \cdot \\
T &\Rightarrow T \cdot * F \\
T &\Rightarrow T \cdot / F
\end{align*}
```

#### $I_3 = \text{GOTO}(I_0, F)$:

$$ T \Rightarrow F \cdot $$

#### $I_4 = \text{GOTO}(I_0, "(")$:

$$ F \Rightarrow (\cdot E) $$

$$\text{CLOSURE}(\{[F \Rightarrow (\cdot E)]\}):$$
```math
\begin{align*}
E &\Rightarrow \cdot E + T \\
E &\Rightarrow \cdot E - T \\
E &\Rightarrow \cdot T \\
T &\Rightarrow \cdot T * F \\
T &\Rightarrow \cdot T / F \\
T &\Rightarrow \cdot F \\
F &\Rightarrow \cdot (E) \\
F &\Rightarrow \cdot \text{id}
\end{align*}
```

#### $I_5 = \text{GOTO}(I_0, \text{id})$:

$$ F \Rightarrow \text{id} \cdot $$

#### $I_6 = \text{GOTO}(I_1, +)$:

$$ E \Rightarrow E + \cdot T $$
$$\text{CLOSURE}(\{[E \Rightarrow E + \cdot T]\}):$$
```math
\begin{align*}
T &\Rightarrow \cdot T * F \\
T &\Rightarrow \cdot T / F \\
T &\Rightarrow \cdot F \\
F &\Rightarrow \cdot (E) \\
F &\Rightarrow \cdot \text{id}
\end{align*}
```

#### $I_7 = \text{GOTO}(I_1, -)$:

$$ E \Rightarrow E - \cdot T $$

$$\text{CLOSURE}(\{[E \Rightarrow E - \cdot T]\}): \text{Аналогично} I_6$$


#### $I_8 = \text{GOTO}(I_2, *)$:

$$ T \Rightarrow T * \cdot  F $$

$$\text{CLOSURE}(\{[T \Rightarrow T * \cdot  F]\}):$$
```math
\begin{align*} 
F &\Rightarrow \cdot (E) \\
F &\Rightarrow \cdot \text{id}
\end{align*}
```

#### $I_9 = \text{GOTO}(I_2, /)$:

$$ T \Rightarrow T / \cdot  F $$

$$\text{CLOSURE}(\{[T \Rightarrow T / \cdot  F]\}):$$
```math
\begin{align*}
F &\Rightarrow \cdot (E) \\
F &\Rightarrow \cdot \text{id}
\end{align*}
```

#### $I_{10} = \text{GOTO}(I_4, E)$:

```math
\begin{align*} 
F &\Rightarrow (E \cdot) \\
E &\Rightarrow E \cdot + T \\
E &\Rightarrow E \cdot - T
\end{align*}
```

> Повторяющиеся переходы:
> ```math
> \begin{align*} 
> \text{GOTO}(I_4, T) &= I_2 \\
> \text{GOTO}(I_4, F) &= I_3 \\
> \text{GOTO}(I_4, "(") &= I_4 \\
> \text{GOTO}(I_4, \text{id}) &= I_5
> \end{align*}
> ```


#### $I_{11} = \text{GOTO}(I_6, T)$:

```math
\begin{align*} 
E &\Rightarrow E + T \cdot \\
T &\Rightarrow T \cdot * F \\
T &\Rightarrow T \cdot / F
\end{align*}
```

> Повторяющиеся переходы:
> ```math
> \begin{align*} 
> \text{GOTO}(I_6, F) &= I_3 \\
> \text{GOTO}(I_6, "(") &= I_4 \\
> \text{GOTO}(I_6, \text{id}) &= I_5
> \end{align*}
> ```

#### $I_{12} = \text{GOTO}(I_7, T)$:

```math
\begin{align*} 
E &\Rightarrow E - T \cdot \\
T &\Rightarrow T \cdot * F \\
T &\Rightarrow T \cdot / F 
\end{align*}
```

> Повторяющиеся переходы:
> ```math
> \begin{align*} 
> \text{GOTO}(I_7, F) &= I_3 \\
> \text{GOTO}(I_7, "(") &= I_4 \\
> \text{GOTO}(I_7, \text{id}) &= I_5
> \end{align*}
> ```

#### $I_{13} = \text{GOTO}(I_8, F)$:

$$ T \Rightarrow T * F \cdot $$

> Повторяющиеся переходы:
> ```math
> \begin{align*} 
> \text{GOTO}(I_8, "(") &= I_4 \\
> \text{GOTO}(I_8, \text{id}) &= I_5
> \end{align*}
> ```

#### $I_{14} = \text{GOTO}(I_9, F)$:

$$ T \Rightarrow T / F \cdot $$

> Повторяющиеся переходы:
> ```math
> \begin{align*}
> \text{GOTO}(I_9, "(") &= I_4 \\
> \text{GOTO}(I_9, \text{id}) &= I_5
> \end{align*}
> ```

#### $I_{15} = \text{GOTO}(I_{10}, ")")$:

$$ F \Rightarrow (E) \cdot $$

> Повторяющиеся переходы:
> ```math
> \begin{align*} 
> \text{GOTO}(I_{10}, +) &= I_6 \\
> \text{GOTO}(I_{10}, -) &= I_7
> \end{align*}
> ```

### Построение FIRST

```math
\begin{align*} 
\text{FIRST}(F) &= \{"(", \text{id}\} \\
\text{FIRST}(T) &= \text{FIRST}(F) \\
\text{FIRST}(E) &= \text{FIRST}(T) = \text{FIRST}(F)
\end{align*}
```

### Построение FOLLOW
```math
\begin{align*} 
\text{FOLLOW}(E) &= \{\$, ")", +, - \} \\
\text{FOLLOW}(T) &= \text{FOLLOW}(E) \bigcup \{*, /\} = \{\$, ")", +, -, *, / \} \\
\text{FOLLOW}(F) &= \text{FOLLOW}(T) = \{\$, ")", +, -, *, / \}
\end{align*}
```

## Таблица SLR(1)

В основе разрешения конфликтов лежит предпросмотр на один символ с помощью множеств $\text{FOLLOW}$. Таблица состоит из двуз колонок: $\text{ACTION}$ и $\text{GOTO}$. Заполнение таблицы для состояния $I_i$​:

1. Shift: Если в $I_i$​ есть пункт $A \Rightarrow \alpha \cdot a \beta$ (a - терминал) и $\text{GOTO}(I_i​, a)=I_j$​, то в ячейку $\text{ACTION}[i, a]$ записывается Shift $j$ ($Sj$).

2. Reduce: Если в $I_i$​ есть пункт $A \Rightarrow a \cdot$, то для каждого терминала $a$ из множества $\text{FOLLOW}(A)$, в ячейку $\text{ACTION}[i, a]$ записывается Reduce $A \Rightarrow a$ ($Rk$, где $k$ - номер правила).

3. Accept: Если в $I_i$​​ есть пункт $E' \Rightarrow E \cdot$, то в ячейку $\text{ACTION}[i, \$]$ записывается Accept.

4. Заполнение GOTO: Если $\text{GOTO}(I_i​ ,A)=I_j$​ (A - нетерминал), то в ячейку $\text{GOTO}[i, A]$ записывается номер состояния $j$.

5. Все пустые ячейки считаются ошибками синтаксиса.


| I | id | + | - | * | / | ( | ) | $ | E | T | F |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| **0** | S5 | | | | | S4 | | | 1 | 2 | 3 |
| **1** | | S6 | S7 | | | | | Acc | | | |
| **2** | | R3 | R3 | S8 | S9 | | R3 | R3 | | | |
| **3** | | R6 | R6 | R6 | R6 | | R6 | R6 | | | |
| **4** | S5 | | | | | S4 | | | 10 | 2 | 3 |
| **5** | | R8 | R8 | R8 | R8 | | R8 | R8 | | | |
| **6** | S5 | | | | | S4 | | | | 11 | 3 |
| **7** | S5 | | | | | S4 | | | | 12 | 3 |
| **8** | S5 | | | | | S4 | | | | | 13 |
| **9** | S5 | | | | | S4 | | | | | 14 |
| **10** | | S6 | S7 | | | | S15 | | | | |
| **11** | | R1 | R1 | S8 | S9 | | R1 | R1 | | | |
| **12** | | R2 | R2 | S8 | S9 | | R2 | R2 | | | |
| **13** | | R4 | R4 | R4 | R4 | | R4 | R4 | | | |
| **14** | | R5 | R5 | R5 | R5 | | R5 | R5 | | | |
| **15** | | R7 | R7 | R7 | R7 | | R7 | R7 | | | |

- **Sn**: Shift в состояние $n$;
- **Rn**: Reduce по правилу $n$;
- **Acc**: Accept;
- Колонки $E$, $T$, $F$: Переходы $\text{GOTO}$.
