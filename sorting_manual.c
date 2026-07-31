/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sorting_manual.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkia <fkia@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/05 16:55:37 by kwaku             #+#    #+#             */
/*   Updated: 2026/07/31 20:40:48 by fkia             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static void	sorting_manual_sort_2(t_input *store, t_dll *stk_a, t_dll *stk_b,
		int disp)
{
	(void)stk_b;
	if (stk_a->len != 2)
		return ;
	if (stk_a->head->val > stk_a->head->next->val)
	{
		rec_op(store, "sa\n", disp);
		sx(stk_a);
	}
}

static void	sorting_manual_sort_3(t_input *store, t_dll *stk_a, t_dll *stk_b,
		int disp)
{
	t_marg	args;

	(void)stk_b;
	if (stk_a->len != 3)
		return ;
	args.a = stk_a->head->val;
	args.b = stk_a->head->next->val;
	args.c = stk_a->tail->val;
	args.disp = disp;
	if (args.a < args.c && args.b > args.c)
	{
		rec_op(store, "rra\n", disp);
		rec_op(store, "sa\n", disp);
	}
	else if (args.a > args.b && args.c > args.a)
		rec_op(store, "sa\n", disp);
	else if (args.a < args.b && args.c < args.a)
		rec_op(store, "rra\n", disp);
	else if (args.c > args.b && args.c < args.a)
		rec_op(store, "ra\n", disp);
	else if (args.c < args.b && args.b < args.a)
	{
		rec_op(store, "ra\n", disp);
		rec_op(store, "sa\n", disp);
	}
}

int	find_target(t_dll *stk, int target)
{
	int		i;
	t_node	*trav;

	i = 0;
	trav = stk->head;
	while (i < stk->len)
	{
		if (trav->val == target)
			return (i);
		trav = trav->next;
		i++;
	}
	return (-1);
}

static int	push_excess_to_b(t_input *store, t_dll *stk_a, t_dll *stk_b)
{
	int	mn;
	int	pos;
	int	moved;

	mn = 0;
	moved = 0;
	while (stk_a->len > 3)
	{
		pos = find_target(stk_a, mn++);
		if (pos < stk_a->len - pos)
			perform_rotations(stk_a, pos, SHOW_OP, "ra\n");
		else
			perform_rotations(stk_a, stk_a->len - pos, SHOW_OP, "rra\n");
		px(stk_a, stk_b);
		rec_op(store, "pb\n", SHOW_OP);
		moved++;
	}
	return (moved);
}

void	sorting_manual_sort(t_input *store, t_dll *stk_a, t_dll *stk_b,
		int disp_op)
{
	int	moved;

	if (stk_a->len == 2)
	{
		sorting_manual_sort_2(store, stk_a, stk_b, disp_op);
		return ;
	}
	if (stk_a->len == 3)
	{
		sorting_manual_sort_3(store, stk_a, stk_b, disp_op);
		return ;
	}
	moved = push_excess_to_b(store, stk_a, stk_b);
	sorting_manual_sort_3(store, stk_a, stk_b, SHOW_OP);
	while (moved-- > 0)
		rec_op(store, "pa\n", SHOW_OP);
}
