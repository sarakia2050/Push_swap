/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sorting_medium_algo.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skia <skia@student.42heilbronn.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/31 00:00:00 by skia              #+#    #+#             */
/*   Updated: 2026/07/31 00:00:00 by skia             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	int_sqrt(int n)
{
	int	i;

	i = 1;
	while (i * i < n)
		i++;
	return (i);
}

static void	rotate_b_to_top(t_dll *stk, int pos, t_input *store, int disp)
{
	if (pos <= stk->len - pos)
	{
		while (pos--)
		{
			rx(stk);
			rec_op(store, "rb\n", disp);
		}
	}
	else
	{
		pos = stk->len - pos;
		while (pos--)
		{
			rrx(stk);
			rec_op(store, "rrb\n", disp);
		}
	}
}

static void	collect_bucket(t_input *store, t_dll *stk_a, t_dll *stk_b,
		t_bucket *bk)
{
	int	steps;

	steps = stk_a->len;
	while (steps-- > 0)
	{
		if (stk_a->head->val >= bk->lo && stk_a->head->val <= bk->hi)
		{
			px(stk_a, stk_b);
			rec_op(store, "pb\n", bk->disp);
		}
		else
		{
			rx(stk_a);
			rec_op(store, "ra\n", bk->disp);
		}
	}
}

static void	flush_bucket_sorted(t_input *store, t_dll *stk_a, t_dll *stk_b,
		int disp)
{
	int		i;
	int		pos;
	int		max;
	t_node	*trav;

	while (stk_b->len)
	{
		i = 0;
		pos = 0;
		trav = stk_b->head;
		max = trav->val;
		while (i < stk_b->len)
		{
			if (trav->val > max)
			{
				max = trav->val;
				pos = i;
			}
			trav = trav->next;
			i++;
		}
		rotate_b_to_top(stk_b, pos, store, disp);
		px(stk_b, stk_a);
		rec_op(store, "pa\n", disp);
	}
}

void	sorting_chunk_sort(t_input *store, t_dll *stk_a, t_dll *stk_b,
		int disp_op)
{
	t_bucket	bk;
	int			n;
	int			chunk;

	store->algo_used = 2;
	n = stk_a->len;
	chunk = int_sqrt(2 * n);
	bk.disp = disp_op;
	bk.lo = ((n - 1) / chunk) * chunk;
	while (bk.lo >= 0)
	{
		bk.hi = bk.lo + chunk - 1;
		collect_bucket(store, stk_a, stk_b, &bk);
		flush_bucket_sorted(store, stk_a, stk_b, disp_op);
		bk.lo -= chunk;
	}
}
