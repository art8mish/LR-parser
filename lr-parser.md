

# Построение LR-парсера

## Грамматика $G'$

Расширенная грамматика $G'$ используется для того, чтобы обозначить точку останова для автомата:

```math
\begin{flalign*}
E' &\Rightarrow E \\
E &\Rightarrow E + T \\
E &\Rightarrow E - T\\
E &\Rightarrow T \\
T &\Rightarrow T * F \\
T &\Rightarrow T / F \\
T &\Rightarrow F \\
F &\Rightarrow (E) \\
F &\Rightarrow \text{id}
\end{flalign*}
```

## Канонический набор пунктов

Каноничесикй набор пунктов представляет состояния конечного автомата.

### Начальное состояние

$$ C = \text{CLOSURE}(\{[E' \Rightarrow \cdot E]\}) $$
$$\text{CLOSURE}(\{[E' \Rightarrow \cdot E]\}):$$
```math
\begin{flalign*}
E' &\Rightarrow \cdot E \\
E &\Rightarrow \cdot E + T \\
E &\Rightarrow \cdot E - T \\
E &\Rightarrow \cdot T \\
T &\Rightarrow \cdot T * F \\
T &\Rightarrow \cdot T / F \\
T &\Rightarrow \cdot F \\
F &\Rightarrow \cdot (E) \\
F &\Rightarrow \cdot \text{id}
\end{flalign*}
```

### Построение GOTO

#### $I_1 = \text{GOTO}(I_0, E)$:
```math
\begin{flalign*}
E' &\Rightarrow E \cdot \\
E &\Rightarrow E \cdot + T \\
E &\Rightarrow E \cdot - T
\end{flalign*}
```

#### $I_2 = \text{GOTO}(I_0, T)$:

```math
\begin{flalign*}
E &\Rightarrow T \cdot \\
T &\Rightarrow T \cdot * F \\
T &\Rightarrow T \cdot / F
\end{flalign*}
```

#### $I_3 = \text{GOTO}(I_0, F)$:

$$ T \Rightarrow F \cdot $$

#### $I_4 = \text{GOTO}(I_0, "(")$:

$$ F \Rightarrow (\cdot E) $$

$$\text{CLOSURE}(\{[F \Rightarrow (\cdot E)]\}):$$
```math
\begin{flalign*}
E &\Rightarrow \cdot E + T \\
E &\Rightarrow \cdot E - T \\
E &\Rightarrow \cdot T \\
T &\Rightarrow \cdot T * F \\
T &\Rightarrow \cdot T / F \\
T &\Rightarrow \cdot F \\
F &\Rightarrow \cdot (E) \\
F &\Rightarrow \cdot \text{id}
\end{flalign*}
```

#### $I_5 = \text{GOTO}(I_0, \text{id})$:

$$ F \Rightarrow \text{id} \cdot $$

#### $I_6 = \text{GOTO}(I_1, +)$:

$$ E \Rightarrow E + \cdot T $$
$$\text{CLOSURE}(\{[E \Rightarrow E + \cdot T]\}):$$
```math
\begin{flalign*}
T &\Rightarrow \cdot T * F \\
T &\Rightarrow \cdot T / F \\
T &\Rightarrow \cdot F \\
F &\Rightarrow \cdot (E) \\
F &\Rightarrow \cdot \text{id}
\end{flalign*}
```

#### $I_7 = \text{GOTO}(I_1, -)$:

$$ E \Rightarrow E - \cdot T $$

$$\text{CLOSURE}(\{[E \Rightarrow E - \cdot T]\}): \text{Аналогично} I_6$$


#### $I_8 = \text{GOTO}(I_2, *)$:

$$ T \Rightarrow T * \cdot  F $$

$$\text{CLOSURE}(\{[T \Rightarrow T * \cdot  F]\}):$$
```math
\begin{flalign*} 
F &\Rightarrow \cdot (E) \\
F &\Rightarrow \cdot \text{id}
\end{flalign*}
```

#### $I_9 = \text{GOTO}(I_2, /)$:

$$ T \Rightarrow T / \cdot  F $$

$$\text{CLOSURE}(\{[T \Rightarrow T / \cdot  F]\}):$$
```math
\begin{flalign*}
F &\Rightarrow \cdot (E) \\
F &\Rightarrow \cdot \text{id}
\end{flalign*}
```

#### $I_{10} = \text{GOTO}(I_4, E)$:

```math
\begin{flalign*} 
F &\Rightarrow (E \cdot) \\
E &\Rightarrow E \cdot + T \\
E &\Rightarrow E \cdot - T
\end{flalign*}
```

> Повторяющиеся переходы:
> ```math
> \begin{flalign*} 
> \text{GOTO}(I_4, T) &= I_2 \\
> \text{GOTO}(I_4, F) &= I_3 \\
> \text{GOTO}(I_4, "(") &= I_4 \\
> \text{GOTO}(I_4, \text{id}) &= I_5
> \end{flalign*}
> ```


#### $I_{11} = \text{GOTO}(I_6, T)$:

```math
\begin{flalign*} 
E &\Rightarrow E + T \cdot \\
T &\Rightarrow T \cdot * F \\
T &\Rightarrow T \cdot / F
\end{flalign*}
```

> Повторяющиеся переходы:
> ```math
> \begin{flalign*} 
> \text{GOTO}(I_6, F) &= I_3 \\
> \text{GOTO}(I_6, "(") &= I_4 \\
> \text{GOTO}(I_6, \text{id}) &= I_5
> \end{flalign*}
> ```

#### $I_{12} = \text{GOTO}(I_7, T)$:

```math
\begin{flalign*} 
E &\Rightarrow E - T \cdot \\
T &\Rightarrow T \cdot * F \\
T &\Rightarrow T \cdot / F 
\end{flalign*}
```

> Повторяющиеся переходы:
> ```math
> \begin{flalign*} 
> \text{GOTO}(I_7, F) &= I_3 \\
> \text{GOTO}(I_7, "(") &= I_4 \\
> \text{GOTO}(I_7, \text{id}) &= I_5
> \end{flalign*}
> ```

#### $I_{13} = \text{GOTO}(I_8, F)$:

$$ T \Rightarrow T * F \cdot $$

> Повторяющиеся переходы:
> ```math
> \begin{flalign*} 
> \text{GOTO}(I_8, "(") &= I_4 \\
> \text{GOTO}(I_8, \text{id}) &= I_5
> \end{flalign*}
> ```

#### $I_{14} = \text{GOTO}(I_9, F)$:

$$ T \Rightarrow T / F \cdot $$

> Повторяющиеся переходы:
> ```math
> \begin{flalign*}
> \text{GOTO}(I_9, "(") &= I_4 \\
> \text{GOTO}(I_9, \text{id}) &= I_5
> \end{flalign*}
> ```

#### $I_{15} = \text{GOTO}(I_{10}, ")")$:

$$ F \Rightarrow (E) \cdot $$

> Повторяющиеся переходы:
> ```math
> \begin{flalign*} 
> \text{GOTO}(I_{10}, +) &= I_6 \\
> \text{GOTO}(I_{10}, -) &= I_7
> \end{flalign*}
> ```

### Построение FIRST

```math
\begin{flalign*} 
\text{FIRST}(F) &= \{"(", \text{id}\} \\
\text{FIRST}(T) &= \text{FIRST}(F) \\
\text{FIRST}(E) &= \text{FIRST}(T) = \text{FIRST}(F)
\end{flalign*}
```

### Построение FOLLOW
```math
\begin{flalign*} 
\text{FOLLOW}(E) &= \{\$, ")", +, - \} \\
\text{FOLLOW}(T) &= \text{FOLLOW}(E) \bigcup \{*, /\} = \{\$, ")", +, -, *, / \} \\
\text{FOLLOW}(F) &= \text{FOLLOW}(T) = \{\$, ")", +, -, *, / \}
\end{flalign*}
```