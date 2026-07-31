/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwaku <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/04 01:05:35 by kwaku             #+#    #+#             */
/*   Updated: 2026/07/04 01:05:37 by kwaku            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PUSH_SWAP_H
# define PUSH_SWAP_H
# define SHOW_OP 1

# include <stdlib.h>
# include <unistd.h>

typedef struct s_manual_arg
{
	int				a;
	int				b;
	int				c;
	int				disp;
}					t_marg;

typedef struct s_bucket
{
	int				lo;
	int				hi;
	int				disp;
}					t_bucket;

typedef struct s_user_input
{
	int				selected_mode;
	int				bench_mode;
	int				arr[600];
	int				num_cnt;
	double			disordedness;
	int				algo_used;
	int				ops[11];
}					t_input;

typedef struct s_node
{
	int				val;
	struct s_node	*prev;
	struct s_node	*next;
}					t_node;

typedef struct s_dl_list
{
	int				len;
	t_node			*head;
	t_node			*tail;
}					t_dll;

int					ft_strncmp(const char *s1, const char *s2, size_t n);
int					ft_atoi(const char *str);
char				*ft_itoa(int n);
int					has_duplicates(int *arr, int cnt);
double				compute_disorder(int *arr, int cnt);
void				set_ranks(t_input *store);
int					ft_putstr(const char *str);
size_t				ft_strlen(const char *str);
int					lst_add_front(t_dll *lst, t_node *new_node);
t_node				*lst_pop_front(t_dll *lst);
void				lst_rotate(t_dll *lst, int dxn);
t_node				*create_node(int val);
void				lst_clear(t_dll *lst);
int					px(t_dll *from, t_dll *to);
int					rx(t_dll *stk);
int					rrx(t_dll *stk);
int					sx(t_dll *stk);
void				perform_rotations(t_dll *stk, int cnt, int dxn,
						const char *op);
void				populate_stack(t_input *store, t_dll *lst);
int					rec_op(t_input *store, const char *op, int disp);
void				sorting_manual_sort(t_input *store, t_dll *stk_a,
						t_dll *stk_b, int disp_op);
void				sorting_turkish_sort(t_input *store, t_dll *stk_a,
						t_dll *stk_b, int disp_op);
void				sorting_chunk_sort(t_input *store, t_dll *stk_a,
						t_dll *stk_b, int disp_op);
void				sorting_radix_sort(t_input *store, t_dll *stk_a,
						t_dll *stk_b, int disp_op);
void				print_bench(t_input *store);
#endif
