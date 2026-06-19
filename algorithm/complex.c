/*
	ALGORITMO COMPLEX - RADIX SORT COM RANKS

	Este algoritmo ordena a stack A usando Radix Sort em binário.

	Em vez de trabalhar diretamente com os valores reais, como:
		-50, 1000, 3, 42

	o algoritmo primeiro transforma cada valor no seu "rank",
	ou seja, na posição que esse valor teria se tudo estivesse ordenado.

	Exemplo:
		valores:        40   10   30   20
		array ordenado: 10   20   30   40
		ranks:          3    0    2    1

	Assim, o algoritmo passa a ordenar os ranks:
		3, 0, 2, 1

	Em binário:
		0 = 00
		1 = 01
		2 = 10
		3 = 11

	O Radix Sort olha para um bit de cada vez.

	Primeiro olha para o bit 0:
		se o bit for 0, faz pb;
		se o bit for 1, faz ra.

	Depois devolve tudo de B para A com pa.

	Depois olha para o bit 1 e repete o processo.

	No fim de analisar todos os bits necessários,
	a stack A fica ordenada do menor para o maior.

	Porquê usar ranks?
	Porque assim evitamos problemas com:
		- números negativos;
		- números muito grandes;
		- gaps grandes entre valores.

	Complexidade:
		O número de bits é aproximadamente log2(n).
		Em cada bit, percorremos n elementos.
		Por isso, a complexidade fica aproximadamente:
			O(n log n)

#include "../includes/push_swap.h"

/*
	Esta função recebe um array já ordenado e procura o valor recebido.

	Como o array está ordenado, usa binary search, ou seja:
	em vez de procurar do início ao fim, vai sempre ao meio.

	Retorna a posição do valor no array.
	Essa posição é o "rank" do número.

	Exemplo:
	array ordenado: [10, 20, 30, 40]
	value = 30
	retorna 2

	Ou seja, 30 passa a ser tratado como índice/rank 2.
*/
static int	get_rank(int *arr, int size, int value)
{
	int	left;
	int	mid;
	int	right;

	left = 0;                     // Começa a procurar no início do array
	right = size - 1;             // Última posição válida do array
	while (left <= right)         // Enquanto ainda houver zona onde procurar
	{
		mid = left + (right - left) / 2; // Calcula o meio da zona atual
		if (arr[mid] == value)           // Se o valor no meio for o que procuramos
			return (mid);                // Retorna a posição dele, ou seja, o rank
		if (arr[mid] < value)            // Se o valor do meio for menor que value
			left = mid + 1;              // Procura só na metade da direita
		else                            // Se o valor do meio for maior que value
			right = mid - 1;             // Procura só na metade da esquerda
	}
	return (-1);                         // Se não encontrar, retorna erro
}

/*
	Esta função calcula quantos bits são necessários para representar
	o maior rank existente.

	Como os ranks vão de 0 até size - 1, o maior rank é size - 1.

	Exemplo:
	size = 5
	ranks possíveis: 0, 1, 2, 3, 4
	maior rank = 4

	4 em binário é 100, por isso precisamos de 3 bits.
*/
static int	get_max_bits(int size)
{
	int	max;
	int	bits;

	max = size - 1;              // O maior rank possível é size - 1
	bits = 0;                    // Começamos com 0 bits contados
	while ((max >> bits) != 0)   // Enquanto ainda houver bits para analisar
		bits++;                 // Conta mais 1 bit
	return (bits);               // Retorna o número total de bits necessários
}

/*
	Esta função faz uma passagem do radix sort.

	Ela analisa todos os números da stack A, usando o bit atual.

	Se o bit atual do rank for 0:
		manda o número para B com pb.

	Se o bit atual do rank for 1:
		roda A com ra, mantendo o número em A.

	No fim da passagem, tudo o que foi para B volta para A com pa.

	Isto é repetido para cada bit em complex_sort.
*/
static void	radix_pass(t_bench *bench, t_stack **a, t_stack **b,
		t_radix *radix)
{
	int	i;
	int	rank;

	i = 0;                                      // Contador de elementos analisados
	while (i < radix->size)                    // Analisa todos os elementos originais
	{
		rank = get_rank(radix->arr, radix->size, (*a)->value);
		                                      // Descobre o rank do valor que está no topo de A

		if (((rank >> radix->bit) & 1) == 0)  // Verifica se o bit atual desse rank é 0
			push_pb(bench, a, b);             // Se for 0, manda de A para B
		else                                  // Se o bit atual for 1
			rotate_ra(bench, a);              // Roda A para manter esse número em A
		i++;                                  // Conta mais um elemento analisado
	}
	while (*b)                                // Enquanto ainda existirem elementos em B
		push_pa(bench, a, b);                 // Devolve tudo de B para A
}

/*
	Esta é a função principal do algoritmo complex.

	Ela usa radix sort com ranks.

	Primeiro transforma os valores reais em posições ordenadas.
	Isto evita problemas com números negativos ou números muito grandes.

	Depois calcula quantos bits são necessários.
	A seguir faz uma passagem por cada bit.

	No fim, a stack A fica ordenada.
*/
void	complex_sort(t_bench *bench, t_stack **a, t_stack **b)
{
	t_radix	radix;
	int		max_bits;

	if (!a || !*a || !b || is_sorted(*a)) // Se A não existe, está vazia, B não existe ou A já está ordenada
		return ;                         // Não faz nada

	radix.size = stack_size(*a);          // Guarda o tamanho da stack A

	radix.arr = get_array(*a);            // Cria um array com os valores de A, normalmente já ordenado
	if (!radix.arr)                       // Se a criação do array falhar
		return ;                         // Sai da função

	radix.bit = 0;                        // Começa pelo bit 0, o menos significativo

	max_bits = get_max_bits(radix.size);  // Calcula quantos bits precisam de ser analisados

	while (radix.bit < max_bits)          // Enquanto ainda houver bits para processar
	{
		radix_pass(bench, a, b, &radix);  // Faz uma passagem do radix para o bit atual
		radix.bit++;                      // Avança para o próximo bit
	}
	free(radix.arr);                      // Liberta o array criado
}