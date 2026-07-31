/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bench.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkia <fkia@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/26 00:00:00 by skia              #+#    #+#             */
/*   Updated: 2026/07/31 19:14:18 by fkia             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static void	put_err(const char *s)
{
	write(2, s, ft_strlen(s));
}

static void	put_strategy(t_input *store)
{
	put_err("[bench] strategy: ");
	if (store->selected_mode == 1)
		put_err("Simple / ");
	else if (store->selected_mode == 2)
		put_err("Medium / ");
	else if (store->selected_mode == 3)
		put_err("Complex / ");
	else
		put_err("Adaptive / ");
	if (store->algo_used == 1)
		put_err("O(n2)\n");
	else if (store->algo_used == 2)
		put_err("O(n*sqrt(n))\n");
	else if (store->algo_used == 3)
		put_err("O(n*log(n))\n");
	else
		put_err("O(1)\n");
}

static void	put_op_counts_a(t_input *store)
{
	int	i;
	int	total;

	i = -1;
	total = 0;
	while (++i < 11)
		total += store->ops[i];
	put_err("[bench] total_ops: ");
	put_err(ft_itoa(total));
	put_err("\n[bench] sa: ");
	put_err(ft_itoa(store->ops[8]));
	put_err(" sb: ");
	put_err(ft_itoa(store->ops[9]));
	put_err(" ss: ");
	put_err(ft_itoa(store->ops[10]));
	put_err(" pa: ");
	put_err(ft_itoa(store->ops[0]));
	put_err(" pb: ");
	put_err(ft_itoa(store->ops[1]));
	put_err("\n");
}

static void	put_op_counts_b(t_input *store)
{
	put_err("[bench] ra: ");
	put_err(ft_itoa(store->ops[2]));
	put_err(" rb: ");
	put_err(ft_itoa(store->ops[3]));
	put_err(" rr: ");
	put_err(ft_itoa(store->ops[4]));
	put_err(" rra: ");
	put_err(ft_itoa(store->ops[5]));
	put_err(" rrb: ");
	put_err(ft_itoa(store->ops[6]));
	put_err(" rrr: ");
	put_err(ft_itoa(store->ops[7]));
	put_err("\n");
}

void	print_bench(t_input *store)
{
	int	whole;
	int	frac;

	whole = (int)(store->disordedness * 100.0);
	frac = (int)(store->disordedness * 10000.0) - whole * 100;
	if (frac < 0)
		frac = -frac;
	put_err("[bench] disorder: ");
	put_err(ft_itoa(whole));
	put_err(".");
	if (frac < 10)
		put_err("0");
	put_err(ft_itoa(frac));
	put_err("%\n");
	put_strategy(store);
	put_op_counts_a(store);
	put_op_counts_b(store);
}
