/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwaku <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/04 01:05:17 by kwaku             #+#    #+#             */
/*   Updated: 2026/07/04 01:05:19 by kwaku            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	parse_number_arg(t_input *store, char *arg)
{
	if (arg[0] == '\0')
		return (0);
	store->arr[store->num_cnt++] = ft_atoi(arg);
	if (ft_strncmp(arg, ft_itoa(store->arr[store->num_cnt - 1]),
			ft_strlen(arg)))
		return (0);
	return (1);
}

int	process_user_input(t_input *store, int ac, char **av)
{
	int	i;

	i = 0;
	while (++i < ac)
	{
		if (av[i][0] == '-' && av[i][1] == '-')
		{
			if (ft_strncmp("simple", av[i] + 2, 7) == 0)
				store->selected_mode = 1;
			else if (ft_strncmp("medium", av[i] + 2, 6) == 0)
				store->selected_mode = 2;
			else if (ft_strncmp("complex", av[i] + 2, 7) == 0)
				store->selected_mode = 3;
			else if (ft_strncmp("bench", av[i] + 2, 5) == 0)
				store->bench_mode = 1;
		}
		else if (!parse_number_arg(store, av[i]))
			return (0);
	}
	return (1);
}

void	init_variables(t_input *store, t_dll *stack_a, t_dll *stack_b)
{
	int	i;

	i = -1;
	while (++i < 11)
		store->ops[i] = 0;
	store->selected_mode = 0;
	store->bench_mode = 0;
	store->num_cnt = 0;
	store->algo_used = 0;
	store->disordedness = 0.0;
	stack_a->len = 0;
	stack_a->head = NULL;
	stack_a->tail = NULL;
	stack_b->len = 0;
	stack_b->head = NULL;
	stack_b->tail = NULL;
}

void	run_push_swap(t_input *store, t_dll *stk_a, t_dll *stk_b)
{
	populate_stack(store, stk_a);
	if (store->disordedness < 0.001)
	{
	}
	else if (stk_a->len < 6)
		sorting_manual_sort(store, stk_a, stk_b, SHOW_OP);
	else if (store->selected_mode == 1)
		sorting_turkish_sort(store, stk_a, stk_b, SHOW_OP);
	else if (store->selected_mode == 2)
		sorting_chunk_sort(store, stk_a, stk_b, SHOW_OP);
	else if (store->selected_mode == 3)
		sorting_radix_sort(store, stk_a, stk_b, SHOW_OP);
	else
	{
		if (store->disordedness < 0.2)
			sorting_turkish_sort(store, stk_a, stk_b, SHOW_OP);
		else if (store->disordedness < 0.5)
			sorting_chunk_sort(store, stk_a, stk_b, SHOW_OP);
		else
			sorting_radix_sort(store, stk_a, stk_b, SHOW_OP);
	}
	if (store->bench_mode)
		print_bench(store);
	lst_clear(stk_a);
	lst_clear(stk_b);
}

int	main(int ac, char **av)
{
	t_input	user_input;
	t_dll	stack_a;
	t_dll	stack_b;

	init_variables(&user_input, &stack_a, &stack_b);
	if (!process_user_input(&user_input, ac, av)
		|| has_duplicates(user_input.arr, user_input.num_cnt))
	{
		write(2, "Error\n", 6);
		return (1);
	}
	user_input.disordedness = compute_disorder(user_input.arr,
			user_input.num_cnt);
	set_ranks(&user_input);
	run_push_swap(&user_input, &stack_a, &stack_b);
	return (0);
}
