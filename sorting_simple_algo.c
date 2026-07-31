/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sorting_simple_algo.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skia <skia@student.42heilbronn.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/31 00:00:00 by skia              #+#    #+#             */
/*   Updated: 2026/07/31 00:00:00 by skia             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	find_min_pos(t_dll *stk)
{
	int		i;
	int		pos;
	int		min;
	t_node	*trav;

	i = 0;
	pos = 0;
	trav = stk->head;
	min = trav->val;
	while (i < stk->len)
	{
		if (trav->val < min)
		{
			min = trav->val;
			pos = i;
		}
		trav = trav->next;
		i++;
	}
	return (pos);
}

static void	rotate_to_top(t_dll *stk, int pos, t_input *store, int disp)
{
	if (pos <= stk->len - pos)
	{
		while (pos--)
		{
			rx(stk);
			rec_op(store, "ra\n", disp);
		}
	}
	else
	{
		pos = stk->len - pos;
		while (pos--)
		{
			rrx(stk);
			rec_op(store, "rra\n", disp);
		}
	}
}

static void	move_min_to_b(t_input *store, t_dll *stk_a, t_dll *stk_b,
		int disp_op)
{
	int	pos;

	pos = find_min_pos(stk_a);
	rotate_to_top(stk_a, pos, store, disp_op);
	px(stk_a, stk_b);
	rec_op(store, "pb\n", disp_op);
}

void	sorting_turkish_sort(t_input *store, t_dll *stk_a, t_dll *stk_b,
		int disp_op)
{
	int	n;

	store->algo_used = 1;
	n = stk_a->len;
	while (n-- > 0)
		move_min_to_b(store, stk_a, stk_b, disp_op);
	while (stk_b->len)
	{
		px(stk_b, stk_a);
		rec_op(store, "pa\n", disp_op);
	}
}
