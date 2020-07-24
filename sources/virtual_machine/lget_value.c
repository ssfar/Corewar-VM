/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lget_value.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssfar <samisfar.dev@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/17 02:10:29 by ssfar             #+#    #+#             */
/*   Updated: 2020/07/24 15:13:06 by ssfar            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar_vm.h"

/*
** Read (size) bytes from arena[(pc + pos) % MEM_SIZE] and return them in a big
** endian unsinged int.
*/

unsigned int	lget_mem(t_vm *vm, unsigned long long pc,
	long long pos, int size)
{
	unsigned int	ret;
	unsigned int	mult;

	ret = 0;
	mult = 1;
	while (--size >= 0)
	{
		ret = ret + vm->arena[(pc + pos + size) % MEM_SIZE] * mult;
		mult *= 256;
	}
	return (ret);
}

/*
** Read and return an unsigned int from arena[(pc + pos) % MEM_SIZE].
*/

unsigned int	lread_int(t_vm *vm, unsigned long long pc,
	long long pos)
{
	unsigned int	ret;
	int				i;
	unsigned char	*p;

	p = (unsigned char *)&ret;
	i = 0;
	while (i < 4)
	{
		p[i] = vm->arena[(pc + pos) % MEM_SIZE];
		pos++;
		i++;
	}
	return (ret);
}
