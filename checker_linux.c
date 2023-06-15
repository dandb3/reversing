#include <stdlib.h>

struct ark
{
	int argnb;
	int *argtable;
	int *arr;
};

int *get_argtable(int argnb, char *argv[], int deb, int num)
{
	char **split;
	int *init_int;
	int idx;

	init_int = ft_init_int(argnb);
	while (argv[deb] != NULL && init_int != NULL)
	{
		idx = 0;	//get_argtable + 41
		if (argv[deb] == NULL)
			continue ;
		split = ft_strsplit(argv[deb], ' ');
		if (split == NULL)
			continue ;
		while (split[idx] != NULL)
		{
			init_int[num] = ft_atoi(split[idx]);
			++idx;
			++num;
		}
		++deb;
		ft_tabdel(split);
	}
	return init_int;
}

int check_double(int *argtable, int argnb)
{
	for (int i = 0 ; i < argnb; ++i)
		for (int j = 0; j < argnb; ++j)
			if ([argtable + i * 4] != [argtable + j * 4] && i == j)
				return -1;
}

/* argnb 개수만큼의 int를 가지는 배열을 만든다. */
int *ft_init_int(int argnb)
{
	int *addr;
	int idx = 0;

	if (argnb <= 0)
		return NULL;
	addr = (int *) malloc(argnb * sizeof(int));
	if (addr == NULL)
		return NULL;
	for (int idx = 0; idx < argnb; ++idx)
		addr[idx] = 0;
	return addr;
}

/* 디버그 모드일 때 2 리턴, 디버그 모드 아니면 1 리턴 */
int debug(struct ark *mem, char *argv[], int argc)
{
	if (argc <= 2 || ft_strcmp(argv[1], "-d") != 0)
	{
		D[mem + 0x58] = 0;
		return 1;
	}
	D[mem + 0x58] = 1;
	return 2;
}

int parser(int argc, char *argv[], struct ark *mem)
{
	int argnb;
	int deb;
	mem->argnb = 0;	//mem->argnb가 전체 arg의 수 인듯.
	mem[8] = 0;
	mem[9] = 0;
	deb = debug(mem, argv, argc);
	while (deb < argc)
	{
		argnb = count_argnb(argv[deb]);
		if (argnb == -1)
			mem->argnb = -1;
		else
			mem->argnb += argnb;
		if (mem->argnb == -1)
			return -2;
		++deb;
	}
	deb = debug(mem, argv, argc);
	mem->argtable = get_argtable(mem->argnb, argv, deb, 0);
	if (mem->argtable != NULL)
		mem->arr = ft_init_int(mem->argnb);
	if (check_double(mem->argtable, mem->argnb) == -1)
		return -1;
	initialize(mem);
	return 0;
}

int check_order_stack(struct ark *mem)
{
	int val;
	val = mem[10];
	while (val < mem->argnb)
	{
		
	}
	return 0;
}

int main(int argc, char *argv[])
{
	struct something *mem;
	int parse;

	mem = malloc(0x68);
	if (mem == NULL)
		return -1;
	if (argc <= 1)
	{
		free(mem);
		return (0);
	}
	parse = parser(argc, argv, mem);
	if (parse == -1 || parse == -2)
		return error(mem, parse);
	visual(mem);
	if (get_instructions(mem) == -1)
		return error(mem, -3);
	if (check_order_stack(mem) == 0 && mem[0] == mem[8])
		ft_putendl("OK");
	else
		ft_putendl("KO");
	ft_free(mem);
	free(mem);
	return 0;
}
