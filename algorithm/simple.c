/* ************************************************************************** */
/* GREEDY INSERTION SORT ALGORITHM                                            */
/*                                                                            */
/* This sorting strategy is based on insertion sort adapted for the           */
/* push_swap project.                                                         */
/*                                                                            */
/* Elements are first moved from stack A to stack B until only three          */
/* elements remain in A.                                                      */
/*                                                                            */
/* After sorting those three elements, values from B are reinserted into      */
/* their correct position in A.                                               */
/*                                                                            */
/* For each insertion, the algorithm evaluates every element in B and         */
/* chooses the one requiring the lowest number of movements.                  */
/*                                                                            */
/* Because the cheapest candidate is selected at every step, this is a        */
/* greedy insertion algorithm.                                                */
/* ************************************************************************** */

#include "../includes/push_swap.h"

/* ************************************************************************** */
/* find_insert_pos                                                            */
/*                                                                            */
/* Finds the position in A where a value from B should be inserted.           */
/* A is treated as a circular sorted stack, therefore the last element is     */
/* compared with the first element.                                           */
/* ************************************************************************** */
static int	find_insert_pos(t_stack *a, int value)
{
	int		pos;		// current index being examined
	t_stack	*cur;		// current node
	t_stack	*next;		// next node

	pos = 0;
	cur = a;
	while (cur)
	{
		next = cur->next;

		// If we reached the last node, compare it with the head.
		if (!next)
			next = a;

		// Normal insertion case:
		// cur < value < next
		if (value > cur->value && value < next->value)
			return (pos + 1);

		// Special case where max and min meet.
		//
		// Example:
		// 40 -> 50 -> 5 -> 10
		//
		// At 50 -> 5:
		// cur->value > next->value
		//
		// Values larger than 50 or smaller than 5
		// must be inserted here.
		if (cur->value > next->value
			&& (value > cur->value || value < next->value))
			return (pos + 1);

		cur = cur->next;
		pos++;
	}

	// Fallback (should rarely happen).
	return (0);
}

/* ************************************************************************** */
/* cost                                                                       */
/*                                                                            */
/* Calculates the minimum number of moves needed to bring a position to the   */
/* top of a stack.                                                            */
/* ************************************************************************** */
static int	cost(int pos, int size)
{
	// If the position is in the first half,
	// normal rotations are cheaper.
	if (pos <= size / 2)
		return (pos);

	// Otherwise reverse rotations are cheaper.
	return (size - pos);
}

/* ************************************************************************** */
/* combined_cost                                                              */
/*                                                                            */
/* Calculates the total cost of moving an element from B back into A.         */
/* ************************************************************************** */
static	int	combined_cost(int pos_a, int size_a, int pos_b, int size_b)
{
	int	cost_a;
	int	cost_b;

	// Cost of bringing insertion position to top in A.
	cost_a = cost(pos_a, size_a);

	// Cost of bringing target value to top in B.
	cost_b = cost(pos_b, size_b);

	// Total amount of movements required.
	return (cost_a + cost_b);
}

/* ************************************************************************** */
/* find_best                                                                  */
/*                                                                            */
/* Examines every element in B and chooses the one that can be inserted into  */
/* A with the lowest movement cost.                                           */
/* ************************************************************************** */
static void	find_best(t_stack *a, t_stack *b,
	int *best_pos_a, int *best_pos_b)
{
	int	pos_a;		// insertion position in A
	int	pos_b;		// current position in B
	int	size_a;		// size of A
	int	size_b;		// size of B
	int	best;		// lowest cost found

	size_a = stack_size(a);
	size_b = stack_size(b);

	// Start with the largest possible value.
	// Any real cost will be smaller.
	best = INT_MAX;

	// First node in B is position 0.
	pos_b = 0;

	while (b)
	{
		// Find where this value belongs in A.
		pos_a = find_insert_pos(a, b->value);

		// Calculate total cost for this candidate.
		if (combined_cost(pos_a, size_a, pos_b, size_b) < best)
		{
			// Save the new best cost.
			best = combined_cost(pos_a, size_a, pos_b, size_b);

			// Save insertion position in A.
			*best_pos_a = pos_a;

			// Save current position in B.
			*best_pos_b = pos_b;
		}

		// Continue checking next value in B.
		b = b->next;
		pos_b++;
	}
}

/* ************************************************************************** */
/* simple_sort                                                                */
/*                                                                            */
/* Sorts small and medium stacks by moving elements to B, sorting the         */
/* remaining three elements, then reinserting values in the cheapest order.   */
/* ************************************************************************** */
void	simple_sort(t_bench *bench, t_stack **a, t_stack **b)
{
	int	pos_a;		// target insertion position in A
	int	pos_b;		// target element position in B

	// Invalid stack or already sorted.
	if (!a || !*a || is_sorted(*a))
		return ;

	// Keep only 3 elements in A.
	while (stack_size(*a) > 3)
		push_pb(bench, a, b);

	// Sort the remaining 3 elements.
	sort_three(bench, a);

	// Reinsert every value from B.
	while (*b)
	{
		// Choose cheapest element to move.
		find_best(*a, *b, &pos_a, &pos_b);

		// Rotate A until insertion point reaches top.
		picker_ra_rra(bench, a, pos_a);

		// Rotate B until chosen value reaches top.
		picker_rb_rrb(bench, b, pos_b);

		// Move chosen value from B to A.
		push_pa(bench, a, b);
	}

	// Final adjustment:
	// place smallest value at top.
	move_min_top(bench, a);
}