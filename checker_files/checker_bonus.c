#include "../includes/checker_bonus.h"

/*
	read_moves()

	Reads instructions from standard input, one line at a time,
	using get_next_line(0).

	Each instruction is sent to checker_apply_instruction(),
	which applies the correct operation to stack A or stack B.

	If an invalid instruction is found, the function frees the current
	line and returns 0.

	If all instructions are valid until the end of input, it returns 1.
*/
static int	read_moves(t_stack **a, t_stack **b)
{
	char	*instruction;                           // stores one instruction line

	instruction = get_next_line(0);                // reads first line from stdin
	while (instruction)                            // loops while there is a line
	{
		if (!checker_apply_instruction(instruction, a, b)) // applies instruction
			return (free(instruction), 0);         // frees line and returns error
		free(instruction);                         // frees valid instruction line
		instruction = get_next_line(0);            // reads next instruction line
	}
	return (1);                                    // all instructions were valid
}

/*
	checker_result()

	Checks the final state of the stacks.

	If stack A is sorted and stack B is empty, it prints "OK".
	Otherwise, it prints "KO".
*/
static void	checker_result(t_stack *a, t_stack *b)
{
	if (is_sorted(a) && !b)                         // A sorted and B empty
		write(1, "OK\n", 3);                       // prints OK
	else                                           // otherwise
		write(1, "KO\n", 3);                       // prints KO
}

/*
	is_flag()

	Checks if string s is exactly equal to the given flag.

	ft_strlen(flag) + 1 is used so the comparison also includes
	the final '\0'. This prevents partial matches.
*/
static int	is_flag(char *s, char *flag)
{
	return (ft_strncmp(s, flag, ft_strlen(flag) + 1) == 0); // returns 1 if equal
}

/*
	has_flags()

	Checks if the program arguments contain forbidden push_swap flags.

	The checker should only receive numbers.
	It should not accept flags like --bench, --simple, --medium,
	--complex or --adaptive.

	Returns 1 if a forbidden flag is found.
	Returns 0 if no forbidden flag is found.
*/
static int	has_flags(int argc, char **argv)
{
	int	i;                                         // argument index

	i = 1;                                         // skips program name argv[0]
	while (i < argc)                               // loops through all arguments
	{
		if (is_flag(argv[i], "--bench")            // checks --bench
			|| is_flag(argv[i], "--simple")        // checks --simple
			|| is_flag(argv[i], "--medium")        // checks --medium
			|| is_flag(argv[i], "--complex")       // checks --complex
			|| is_flag(argv[i], "--adaptive"))     // checks --adaptive
			return (1);                            // forbidden flag found
		i++;                                      // moves to next argument
	}
	return (0);                                    // no forbidden flags found
}

/*
	main()

	Main function of the checker bonus.

	Steps:
	1. Initializes all variables.
	2. Rejects forbidden flags.
	3. Uses parser() to validate the arguments and convert them into numbers.
	4. Builds stack A with those numbers.
	5. Reads instructions from standard input.
	6. Applies each instruction to the stacks.
	7. Prints OK if A is sorted and B is empty.
	8. Prints KO otherwise.
	9. Frees all allocated memory before exiting.
*/
int	main(int argc, char **argv)
{
	int			status;                              // stores parser result
	int			*numbers;                            // stores parsed integers
	t_stack		*a;                                  // stack A
	t_stack		*b;                                  // stack B
	t_bench		bench;                               // stores parser/bench data

	numbers = NULL;                                // initializes numbers pointer
	a = NULL;                                      // initializes stack A
	b = NULL;                                      // initializes stack B
	if (has_flags(argc, argv))                     // checks forbidden flags
		return (print_error());                    // prints Error and exits
	status = parser(argc, argv, &numbers, &bench); // parses and validates input
	if (status == 0)                               // no arguments case
		return (0);                                // exits silently
	if (status == -1)                              // parser found an error
		return (print_error());                    // prints Error and exits
	a = stack_builder(numbers, bench.numbers_count); // builds stack A
	free(numbers);                                 // frees numbers array
	if (!a)                                        // checks if stack creation failed
		return (print_error());                    // prints Error and exits
	if (!read_moves(&a, &b))                       // reads and applies instructions
		return (stack_free(&a), stack_free(&b), print_error()); // frees and errors
	return (checker_result(a, b), stack_free(&a), stack_free(&b), 0); // result + cleanup
}