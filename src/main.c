/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smortier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/24 23:20:52 by smortier          #+#    #+#             */
/*   Updated: 2018/03/11 11:05:14 by smortier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"
#include <stdio.h>

t_coord		*ft_newcoords(float x, float y, char *zstr)
{
	t_coord		*newcoords;

	if (!(newcoords = (t_coord *)ft_memalloc(sizeof(t_coord))))
		return (NULL);
	newcoords->x = (float)x;
	newcoords->y = (float)y;
	if (ft_strchr(zstr, 'x'))
	{
		ft_putendl("An error occured with you'r file. Please check it");
		exit(1);
	}
	else
		newcoords->z = (float)(ft_atoi(zstr)) ? ft_atoi(zstr) : 0;
	newcoords->next = NULL;
	newcoords->prev = NULL;
	ft_memdel((void **)&zstr);
	return (newcoords);
}

t_coord		*ft_create_lst(t_coord *coords, int x, int y, char *zstr)
{
	t_coord	*temp;

	if (coords == NULL)
		return (ft_newcoords(x, y, zstr));
	else
	{
		temp = coords;
		while (temp->next)
			temp = temp->next;
		temp->next = ft_newcoords(x, y, zstr);
		temp->next->prev = temp;
		return (coords);
	}
}

t_coord		*norm(t_coord *coords, char **map, int y)
{
	int		x;
	int		save_pos;
	int		index;

	index = 0;
	x = 0;
	while (map[(int)y][index])
	{
		save_pos = index;
		while (ft_isalnum(map[(int)y][save_pos]))
			save_pos++;
		coords = ft_create_lst(coords, x, y, ft_strsub(map[(int)y], index,
													save_pos - index));
		x++;
		index = save_pos;
		while (map[(int)y][index] == ' ' || map[(int)y][index] == ',')
			index++;
	}
	return (coords);
}

t_coord		*ft_fill_lst(t_coord *coords, char **map, int endofthis)
{
	float		y;
	float		x;
	int			index;
	int			counter;

	y = 0;
	counter = 0;
	while (map[(int)y])
	{
		index = 0;
		x = 0;
		coords = norm(coords, map, y);
		y++;
		counter++;
		where_m_i(counter, endofthis);
	}
	return (coords);
}

int			main(int ac, char **av)
{
	char		*line;
	char		*map[10000];
	t_coord		*coords;
	int			fd;
	int			i;

	coords = NULL;
	if (ac != 2)
	{
		ft_putendl("usage : ./fdf file");
		exit(0);
	}
	i = -1;
	fd = open(av[1], O_RDONLY);
	while (get_next_line(fd, &line) == 1)
		map[++i] = line;
	if (!i)
	{
		ft_putendl("File is not correct, Please retry with a good one");
		exit(1);
	}
	map[i + 1] = NULL;
	close(fd);
	coords = ft_fill_lst(coords, map, i + 1);
	init_drawing(fill_params(coords));
}
