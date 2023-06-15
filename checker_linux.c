#include <stdlib.h>

struct ark
{
	int argnb;
	int min;
	int max;
	
	int *a;			//0x10
	int *b;
	int a_size;		//0x20
	int b_size;
	...
	int a_last;		//0x30
	...
	int *sorted_arr;	//0x50
	...
	char *name;		//0x60
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

/* argtable에 중첩된 input이 있는지 확인! */
int check_double(int *argtable, int argnb)
{
	for (int i = 0 ; i < argnb; ++i)
		for (int j = 0; j < argnb; ++j)
			if (argtable[i] == argtable[j] && i != j)
				return -1;
}

/* argnb 개수만큼의 int를 가지는 배열을 만든다. */
int *ft_init_int(int argnb)
{
	int *arr;
	int idx = 0;

	if (argnb <= 0)
		return NULL;
	arr = (int *) malloc(argnb * sizeof(int));
	if (arr == NULL)
		return NULL;
	for (int idx = 0; idx < argnb; ++idx)
		arr[idx] = 0;
	return arr;
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

int count_argnb(char *str)
{
	char **split;
	int cnt = 0;
	int idx = 0;

	split = ft_strsplit(str, ' ');
	while (split[idx] != NULL)
	{
		if (check_atoi(split[idx]) == -1) {
			ft_tabdel(split);
			return -1;
		}
		++cnt;
		++idx;
	}
	ft_tabdel(split);
	if (cnt != 0)
		return cnt;
	else
		return -1;
}

int *copy_inttab(int *src, int *dst, int start, int end)
{
	int idx = 0;

	while (idx < end - start + 1)
	{
		dst[idx] = src[idx + start];
		++idx;
	}
	return dst;
}

char *ft_strnew(size_t num)
{
	int *arr;

	arr = malloc(num + 1);
	if (arr == NULL)
		return NULL;
	++num;
	while (num-- != 0)
		arr[num] = 0;
	return arr;
}

void initialize(struct ark *mem)
{
	mem->sorted_arr = ft_init_int(mem->argnb);
	mem->sorted_arr = copy_inttab(mem->a, mem->sorted_arr, 0, mem->argnb - 1);
	mem->sorted_arr = bubble_sort(mem->sorted_arr, mem->argnb);
	mem->min = mem->sorted_arr[0];
	mem->max = mem->sorted_arr[mem->argnb - 1];
	edx = mem->argnb;
	mem->a_size = mem->argnb;
	mem->b_size = 0;
	if (mem->a != NULL)
		mem->a_last = mem->a[mem->argnb - 1];
	Q[mem + 0x38] = ft_init_int(1);
	D[mem + 0x48] = 1;
	D[[mem + 0x38]] = mem->min;
	D[mem + 0x44] = 0;
	D[mem + 0x28] = top(mem, 'a');
	D[mem + 0x2c] = top(mem, 'b');
	mem->name = ft_strnew(1);
}

int parser(int argc, char *argv[], struct ark *mem)
{
	int argnb;
	int deb;
	mem->argnb = 0;	//mem->argnb가 전체 arg의 수 인듯.
	mem->a_size = 0;
	mem->b_size = 0;
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
	mem->a = get_argtable(mem->argnb, argv, deb, 0);
	if (mem->a != NULL)
		mem->arr = ft_init_int(mem->argnb);
	if (check_double(mem->a, mem->argnb) == -1)
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

int top(struct ark *mem, char stack)
{
	if (stack == 'a')
		return (mem->argnb - mem->a_size);
	else if (stack == 'b')
		return (mem->argnb - mem->b_size);
	else
		return 0;
}

void swap_operations(struct ark *mem, char *line)
{
	int tmp;

	if (ft_strcmp(line, "sa\n") == 0 && mem->a_size > 1) {
		tmp = mem->a[top(mem, 'a')];
		mem->a[top(mem, 'a')] = mem->a[top(mem, 'a') + 1];
		mem->a[top(mem, 'a') + 1] = tmp;
	}
	else if (ft_strcmp(line, "sb\n") == 0 && mem->b_size > 1) {
		tmp = mem->b[top(mem, 'b')];
		mem->b[top(mem, 'b')] = mem->b[top(mem, 'b') + 1];
		mem->b[top(mem, 'b') + 1] = tmp;
	}
	else if (ft_strcmp(line, "ss\n") == 0) {
		swap_operations(mem, "sa\n");
		swap_operations(mem, "sb\n");
	}
	visual(mem);
}

void push_operations(struct ark *mem, char *line)
{
	if (ft_strcmp(line, "pa\n") == 0 && mem->b_size > 0) {
		mem->a[top(mem, 'a') - 1] = mem->b[top(mem, 'b')];
		mem->b[top(mem, 'b')] = 0;
		--mem->b_size;
		++mem->a_size;
	}	//+212
	else if (ft_strcmp(line, "pb\n") == 0 && mem->a_size > 0) {
		mem->b[top(mem, 'b') - 1] = mem->a[top(mem, 'a')];
		mem->a[top(mem, 'a')] = 0;
		--mem->a_size;
		++mem->b_size;
	}
}

int get_instructions(struct ark *mem)
{
	char *line;
	int status;
	int cnt = 0;

	while (cnt != -1 && (status = get_next_line(0, &line) == 1)) {
		++cnt;
		if (ft_strcmp(line, "sa\n") == 0 || ft_strcmp(line, "sb\n") == 0 || ft_strcmp(line, "ss\n") == 0)
			swap_operations(mem, line);
		else if (ft_strcmp(line, "pa\n") == 0 || ft_strcmp(line, "pb\n") == 0)
			push_operations(mem, line);
		else if (ft_strcmp(line, "ra\n") == 0 || ft_strcmp(line, "rb\n") == 0 || ft_strcmp(line, "rr\n") == 0)
			rotate_operations(mem, line);
		else if (ft_strcmp(line, "rra\n") == 0 || ft_strcmp(line, "rrb\n") == 0 || ft_strcmp(line, "rrr\n") == 0)
			reverse_operations(mem, line);
		else
			cnt = -1;
		ft_strdel(line);
	}
	if (cnt == -1 || status == -1)
		return -1;
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
	if (check_order_stack(mem) == 0 && mem->argnb == mem[8])
		ft_putendl("OK");
	else
		ft_putendl("KO");
	ft_free(mem);
	free(mem);
	return 0;
}
