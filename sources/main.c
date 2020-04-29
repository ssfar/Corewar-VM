/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssfar <ssfar@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/17 13:27:02 by ssfar             #+#    #+#             */
/*   Updated: 2020/04/17 13:27:02 by ssfar            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar_vm.h"

int	main(int ac, char **av)
{
	corewar_vm(ac, av);
}

void	corewar_vm(int ac, char **av)
{
	t_vm	vm;

	if (ac < 2)
		exit_failure("Usage : ./corewar "
		"[-dump nbr_cycles] [[-n number] champion1.cor] ...", NULL, false);
	init(ac, av, &vm);
	parsing(ac, av, &vm);
	complete_player_id(ac, av, &vm);
	read_player_file(&vm);
}

void	init(int ac, char **av, t_vm *vm)
{
	unsigned int	i;

	vm->option_id = 0;
	vm->cursor = NULL;
	vm->visualiser = false;
	count_player(ac, av, vm);
	if (!(vm->player = malloc(sizeof((*vm->player)) * vm->nb_player)))
		exit_failure("Malloc error", NULL, false);
	i = 0;
	while (i < vm->nb_player)
	{
		vm->player[i].file = NULL;
		i++;
	}
}

void	complete_player_id(int ac, char **av, t_vm *vm)
{
	unsigned int	i;
	unsigned int	j;

	if (vm->option_id != 0)
		exit_failure("Usage : ./corewar "
		"[-dump nbr_cycles] [[-n number] champion1.cor] ...", NULL, false);
	i = 0;
	while (i < ac)
	{
		if (ft_strlen(av[i]) > 4
			&& ft_strcmp(&(av[i][ft_strlen(av[i]) - 4]), ".cor" == 0))
		{
			j = 0;
			while (j < vm->nb_player && vm->player[j].file != av[i])
				j++;
			if (j == vm->nb_player)
			{
				j = 0;
				while (vm->player[j].file != NULL)
					j++;
				vm->player[j].file = av[i];
			}
		}
		i++;
	}
}

void	exit_failure(char *error_message, char *file, t_bool call_perror)
{
	if (call_perror == true)
		perror(error_message);
	else if (file != NULL)
		ft_dprintf(2, error_message, file);
	else if (error_message)
	{
		write(2, error_message, ft_strlen(error_message));
		write(2, "\n", 1);
	}
	exit(-1);
}
