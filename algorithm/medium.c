/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   medium.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emda-sil <emda-sil@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/15 09:58:33 by emda-sil          #+#    #+#             */
/*   Updated: 2026/06/15 09:58:33 by emda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/push_swap.h"
/* ************************************************************************** */
/* get_chunk_size                                                             */
/*                                                                            */
/* Calculates the size of each chunk used during the distribution phase.      */
/*                                                                            */
/* The chunk size is approximately the square root of the total number of     */
/* elements. For larger inputs (>100), the chunk size is doubled to reduce    */
/* the number of chunk transitions.                                           */
/* ************************************************************************** */
static int	get_chunk_size(int size)
{
	int	chunk;		// number of elements per chunk

	chunk = 1;		// start from the smallest possible chunk

	while (chunk * chunk < size)
		chunk++;
	// find the smallest value whose square is >= size

	if (size > 100)
		chunk *= 2;
	// use larger chunks for bigger stacks

	return (chunk);
	// return calculated chunk size
}

/* ************************************************************************** */
/* push_b                                                                     */
/*                                                                            */
/* Moves all elements from A to B in chunk order.                             */
/*                                                                            */
/* A sorted array is used as a reference. Elements belonging to the current   */
/* chunk are pushed to B, while the others remain in A and are rotated until  */
/* they can be processed later.                                               */
/* ************************************************************************** */
static void	push_b(t_bench *bench, t_stack **a, t_stack **b, int *arr)
{
	int	size;		// total amount of numbers
	int	chunk;		// chunk size
	int	limit;		// highest index of current chunk in sorted array
	int	pushed;		// number of elements already pushed to B

	size = stack_size(*a);
	// save original stack size

	chunk = get_chunk_size(size);
	// calculate chunk size

	limit = chunk - 1;
	// first chunk ends at this index

	pushed = 0;
	// nothing has been pushed yet

	while (*a)
	// continue until A becomes empty
	{
		if (limit >= size)
			limit = size - 1;
		// avoid accessing outside array bounds

		if ((*a)->value <= arr[limit])
		// check if top value belongs to current chunk
		{
			push_pb(bench, a, b);
			// move value from A to B

			pushed++;
			// count pushed element

			if (pushed > limit)
				limit += chunk;
			// current chunk completed
			// advance to next chunk
		}
		else
			rotate_ra(bench, a);
		// value does not belong to current chunk
		// rotate A and check another value
	}
}

/* ************************************************************************** */
/* push_a                                                                     */
/*                                                                            */
/* Rebuilds stack A from B.                                                   */
/*                                                                            */
/* The largest value in B is always selected and moved to the top using the   */
/* cheapest rotation direction. Once at the top, it is pushed back to A.      */
/* ************************************************************************** */
static void	push_a(t_bench *bench, t_stack **a, t_stack **b)
{
	int	pos;		// position of maximum value in B
	int	size;		// current size of B

	while (*b)
	// continue until B becomes empty
	{
		pos = find_max_position(*b);
		// locate largest number in B

		size = stack_size(*b);
		// current size of stack B

		if (pos <= size / 2)
		// maximum is closer to top
		{
			while (pos--)
				rotate_rb(bench, b);
			// bring maximum to top using rb
		}
		else
		// maximum is closer to bottom
		{
			pos = size - pos;
			// calculate reverse rotations needed

			while (pos--)
				rev_rrb(bench, b);
			// bring maximum to top using rrb
		}

		push_pa(bench, a, b);
		// move maximum value back to A
	}
}

/* ************************************************************************** */
/* medium_sort                                                                */
/*                                                                            */
/* Chunk-based sorting algorithm.                                             */
/*                                                                            */
/* Steps:                                                                     */
/* 1. Create a sorted array containing all values from A.                     */
/* 2. Push values from A to B chunk by chunk.                                 */
/* 3. Rebuild A by repeatedly moving the largest value from B.                */
/*                                                                            */
/* At the end, A is sorted in ascending order.                                */
/* ************************************************************************** */
void	medium_sort(t_bench *bench, t_stack **a, t_stack **b)
{
	int	*array;		// sorted copy of all stack values

	if (!a || !*a || is_sorted(*a))
		return ;
	// invalid stack or already sorted

	array = get_array(*a);
	// create sorted array from stack values

	if (!array)
		return ;
	// allocation failed

	push_b(bench, a, b, array);
	// distribute all values from A to B using chunks

	free(array);
	// sorted array no longer needed

	push_a(bench, a, b);
	// rebuild A in sorted order
}