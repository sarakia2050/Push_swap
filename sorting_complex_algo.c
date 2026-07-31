/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sorting_complex_algo.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwaku <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/05 16:57:07 by kwaku             #+#    #+#             */
/*   Updated: 2026/07/05 16:57:19 by kwaku            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	get_msb_pos(int num)
{
	int	i;

	i = 15;
	while (!(num & (1 << i)))
		i--;
	return (i);
}

static void	helper_method(t_input *store, t_dll *stk_a, t_dll *stk_b, int disp)
{
	if (!(disp & (1 << 0)))
	{
		px(stk_a, stk_b);
		rec_op(store, "pb\n", disp & (1 << 1));
	}
	else
	{
		rx(stk_a);
		rec_op(store, "ra\n", disp & (1 << 1));
	}
}

int	rec_op(t_input *store, const char *op, int disp)
{
	if (ft_strncmp(op, "pa", 2) == 0)
		store->ops[0]++;
	else if (ft_strncmp(op, "pb", 2) == 0)
		store->ops[1]++;
	else if (ft_strncmp(op, "ra", 2) == 0)
		store->ops[2]++;
	else if (ft_strncmp(op, "rb", 2) == 0)
		store->ops[3]++;
	else if (ft_strncmp(op, "rra", 3) == 0)
		store->ops[5]++;
	else if (ft_strncmp(op, "rrb", 3) == 0)
		store->ops[6]++;
	else if (ft_strncmp(op, "rrr", 3) == 0)
		store->ops[7]++;
	else if (ft_strncmp(op, "rr", 2) == 0)
		store->ops[4]++;
	else if (ft_strncmp(op, "sa", 2) == 0)
		store->ops[8]++;
	else if (ft_strncmp(op, "sb", 2) == 0)
		store->ops[9]++;
	else if (ft_strncmp(op, "ss", 2) == 0)
		store->ops[10]++;
	if (disp)
		ft_putstr(op);
	return (1);
}

void	sorting_radix_sort(t_input *store, t_dll *stk_a, t_dll *stk_b,
		int disp_op)
{
	int	msb;
	int	i;
	int	j;
	int	cmb;

	store->algo_used = 3;
	i = -1;
	msb = get_msb_pos(stk_a->len - 1);
	while (++i <= msb)
	{
		j = stk_a->len;
		while (j--)
		{
			cmb = (disp_op << 1) | ((stk_a->head->val & (1 << i)) != 0);
			helper_method(store, stk_a, stk_b, cmb);
		}
		while (stk_b->len)
		{
			px(stk_b, stk_a);
			rec_op(store, "pa\n", disp_op);
		}
	}
}
