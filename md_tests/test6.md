# Algoritmo di ricerca lineare

L'algoritmo di **ricerca lineare** permette la ricerca di un determinato elemento $x$ in un vettore **non ordinato** di dimensione $N$. Chiameremo la procedura `linear_search`.

La complessità di questo algoritmo è

$$
O(n) = \text{Un passo per ogni slot del vettore}
$$

Una possibile implementazione è

```c
int linear_search(int arr[], int value, int arr_size) {
    for (int i = 0; i < arr_size; i++) {
        if (arr[i] == value) {
            return i;
        }
    }
    return -1;
}
```

- Ok adesso
- questa è una lista

speriamo bene

And for my last trick

1. questa
2. è una lista
3. ordinata

speriamo...

