/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tiny.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emda-sil <emda-sil@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/15 10:00:14 by emda-sil          #+#    #+#             */
/*   Updated: 2026/06/15 10:00:14 by emda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/push_swap.h"
//Swaps the top 2 numbers if the first is bigger then the second ex 2 1 - uses sa.
void	sort_two(t_bench *bench, t_stack **a)
{
	if (!a || !*a || stack_size(*a) != 2)
		return ;
	if ((*a)->value > (*a)->next->value)
		swap_sa(bench, a);
}
//The function checks the relative order of these three values and identifies
//which of the five possible unsorted configurations is present. For each
//configuration, it applies the appropriate stack operations (sa, ra, or rra)
//to transform the stack into ascending order
void	sort_three(t_bench *bench, t_stack **a)
{
	int	a1;
	int	a2;
	int	a3;

	if (!a || !*a || stack_size(*a) != 3)
		return ;
	a1 = (*a)->value;
	a2 = (*a)->next->value;
	a3 = (*a)->next->next->value;
	if (a1 > a2 && a2 < a3 && a1 < a3)
		swap_sa(bench, a);
	else if (a1 > a2 && a2 > a3)
	{
		swap_sa(bench, a);
		rev_rra(bench, a);
	}
	else if (a1 > a2 && a2 < a3 && a1 > a3)
		rotate_ra(bench, a);
	else if (a1 < a2 && a2 > a3 && a1 < a3)
	{
		swap_sa(bench, a);
		rotate_ra(bench, a);
	}
	else if (a1 < a2 && a2 > a3 && a1 > a3)
		rev_rra(bench, a);
}
//Finds the position of the lowest number.
//Using picker_ra_rra, it rotates stack A in the most efficient direction until the
//smallest value is at the top. After that, it pushes this smallest value to stack B.
//Now stack A only has three elements, so the function calls sort_three to sort them.
//Finally, it pushes the smallest value back from stack B to stack A.
//Since the smallest value is placed back on top of the already sorted three elements,
//the whole stack becomes sorted.
void	sort_four(t_bench *bench, t_stack **a, t_stack **b)
{
	int	pos;

	if (!a || !*a || !b || stack_size(*a) != 4)
		return ;
	pos = find_min_position(*a);
	picker_ra_rra(bench, a, pos);
	push_pb(bench, a, b);
	sort_three(bench, a);
	push_pa(bench, a, b);
}
//Finds the position of the lowest number.
//Using picker_ra_rra, it rotates stack A in the most efficient direction until the
//smallest value is at the top. After that, it pushes this smallest value to stack B.
//repeats
//Now stack A only has three elements, so the function calls sort_three to sort them.
//Finally, it pushes the smallest value back from stack B to stack A.
//Since the smallest value is placed back on top of the already sorted three elements,
//the whole stack becomes sorted.
void	sort_five(t_bench *bench, t_stack **a, t_stack **b)
{
	int	pos;

	if (!a || !*a || !b || stack_size(*a) != 5)
		return ;
	pos = find_min_position(*a);
	picker_ra_rra(bench, a, pos);
	push_pb(bench, a, b);
	pos = find_min_position(*a);
	picker_ra_rra(bench, a, pos);
	push_pb(bench, a, b);
	sort_three(bench, a);
	push_pa(bench, a, b);
	push_pa(bench, a, b);
}