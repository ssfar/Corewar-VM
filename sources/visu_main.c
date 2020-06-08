#include "corewar_vm.h"

t_window    *init_window(void)
{
    t_window *window;
    WINDOW   *memory;
    WINDOW   *info1;
    WINDOW   *info2;
    WINDOW   *champions;
    WINDOW   *input;

    memory = NULL;
    info1 = NULL;
    info2 = NULL;
    champions = NULL;
    input = NULL;
    window = malloc(sizeof(t_window));
    window->memory = memory;
    window->info1 = info1;
    window->info2 = info2;
    window->champions = champions;
    window->input = input;
    return (window);
}

void	color_player(t_vm *vm, size_t act_player, size_t size, size_t nb_player)
{
	int i;
	size_t j;
	size_t start;

	i = 0;
	j = 0;
	start = MEM_SIZE / nb_player;
	start = start * (act_player -1);
	while (j < 64 && size > 0)
    {
		i = 0;
		while (i < 64 && size > 0)
		{
			if (i + j * 64 >= start && size > 0)
			{
				vm->arena_color[i + j * 64] = act_player;
				size--;
			}
			i++;
		}
		j++;
    }
}

void	init_color_arena(t_vm *vm)
{
	int i;

	i = 0;
	init_pair(0, COLOR_WHITE, COLOR_BLACK);
	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	init_pair(2, COLOR_BLUE, COLOR_BLACK);
	init_pair(3, COLOR_RED, COLOR_BLACK);
	init_pair(4, COLOR_CYAN, COLOR_BLACK);
	while (i < 4096)
	{
		vm->arena_color[i] = 0;
		i++;
	}
	i = 0;
    while (i < vm->nb_player)
    {
		color_player(vm, i + 1, vm->player[i].prog_size, vm->nb_player);
		i++;
    }
}

int    init_visu(t_vm *vm)
{
	size_t ymax;
	size_t xmax;
	size_t y;
	size_t x;

    initscr(); // start ncurses
	start_color();
    noecho();
    cbreak();
    getmaxyx(stdscr, ymax, xmax); // get screen size
    if (ymax < 64 || xmax < 250) // too short ?
    {
        printf("ERROR, SCREEN TO SHORT, ZOOM OUT");
        return (-1);
    }
    y = ymax;
	x = 192;
    vm->window = init_window();
    vm->window->memory = newwin(y + 1, x + 1, 0, 0);
    vm->window->info1 = newwin(9, 75, 0, xmax - 75);
    vm->window->champions = newwin(50 - 12.5, 75, 9, xmax - 75);
    vm->window->input = newwin(11, 75, ymax - 22, xmax - 75);
    vm->window->info2 = newwin(11, 75, ymax - 11, xmax - 75);
	init_color_arena(vm);
	display_round(vm);
	display_round(vm);
	refresh_window(vm);
	getch();
    return(0);
}

void    refresh_window(t_vm *vm)
{
    wrefresh(vm->window->memory);
    wrefresh(vm->window->info1);
    wrefresh(vm->window->champions);
    wrefresh(vm->window->info2);
    wrefresh(vm->window->input);
    refresh();
}

void	aff_memory(t_vm *vm)
{
	size_t i;
	size_t j;

	j = 0;
    while (j < 64)
    {
		i = 0;
		while (i < 64)
		{
			wattron(vm->window->memory, COLOR_PAIR(vm->arena_color[i + j * 64]));
			mvwprintw(vm->window->memory, j, i * 3, "%c%02x", ' ', vm->arena[i + j * 64]);
			wattroff(vm->window->memory, COLOR_PAIR(vm->arena_color[i + j * 64]));
			i++;
		}
		j++;
    }
	wrefresh(vm->window->memory);
}

void	aff_input(t_vm *vm)
{
	size_t j;

	j = 1;
	mvwprintw(vm->window->input, j, 1, "Play/Pause \t\t[SPACE]");
    j += 2;
    mvwprintw(vm->window->input, j, 1, "Next Turn \t\t[c]");
    j += 2;
    mvwprintw(vm->window->input, j, 1, "Leave \t\t\t[q]");
}

void	aff_info(t_vm *vm)
{
	size_t j;

	j = 1;
	mvwprintw(vm->window->info1, j, 1, "Cycle: %d", vm->cycle_counter);
    j += 2;
    mvwprintw(vm->window->info1, j, 1, "Players: %d", vm->nb_player);
    j += 2;
    mvwprintw(vm->window->info1, j, 1, "Last alive: Player %d", -1 * vm->last_reported_alive);
    j = 1;
    mvwprintw(vm->window->info2, j, 1, "Cycle to die: %d", vm->cycle_to_die);
    j += 2;
    mvwprintw(vm->window->info2, j, 1, "Checks: %d", vm->check_counter);
    j += 2;
    mvwprintw(vm->window->info2, j, 1, "Lives", vm->check_counter);
}

void	aff_champions(t_vm *vm)
{
	int i;
	size_t j;
	
	
	i = 0;
	j = 1;
	while (i < vm->nb_player)
	{
		wattron(vm->window->champions, COLOR_PAIR(i + 1));
    	mvwprintw(vm->window->champions, j, 1, "Player %d :", i + 1);
    	j += 2;
    	mvwprintw(vm->window->champions, j, 1, "%s", vm->player[i].prog_name);
    	j += 4;
		i++;
	}
}

void    display_round(t_vm *vm)
{
	aff_memory(vm);
	aff_info(vm);
	aff_champions(vm);
	aff_input(vm);
	refresh_window(vm);
}

void	color_arena(t_vm *vm, t_cursor *cursor)
{
	size_t i;

	i = 0;
	// ft_printf("reg :%d, pc %d, id %d\n", cursor->reg[REG_NUMBER], cursor->pc, cursor->player_id);
	while (i < 8)
	{
		vm->arena_color[cursor->pc + i] = cursor->player_id;
		i++;
	}
}

int		get_visu_input(t_vm *vm, int ch)
{
	nodelay(vm->window->memory, 1);
	if (ch == 'q')
	{
		system("clear");
		exit(0);
	}
	if (ch != 'c') // verif quon est pas en mode cycle
	{
		ch = wgetch(vm->window->memory);
	}
	if (ch == 'c') // avance cycle par cycle
		ch = getch();
	else if (ch == ' ') // play / pause
	{
		ch = wgetch(vm->window->memory);
		while (ch != ' ' && ch != 'c' && ch != 'q')
			ch = wgetch(vm->window->memory);
	}
	return (ch);
}