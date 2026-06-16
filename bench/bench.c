/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bench.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emda-sil <emda-sil@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/15 10:04:21 by emda-sil          #+#    #+#             */
/*   Updated: 2026/06/15 10:04:21 by emda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/push_swap.h"
//mete a struct bench em default
void	init_bench(t_bench *bench)
{
	int	i;

	bench->strategy = STRAT_ADAPTIVE;//por defeito o programa usa adaptive
	bench->numbers_count = 0;
	bench->disorder = 0;
	bench->used_strategy = 3;
	bench->display = 0;//comeca desligado
	i = 0;
	while (i < 11) //inicia todos os contadores de operacoes a zero
		bench->moves[i++] = 0;
}
//verifica se o token e --bench
int	set_bench(t_bench *bench, char *token)
{
	if (ft_strncmp(token, "--bench", 8) != 0)//se o token nao for --bench retorna 0
		return (0);
	if (bench->display) //Se o --bench já tinha sido usado antes, dá erro.
		return (-1);
	bench->display = 1; //ativa o modo bench
	return (1);
}
