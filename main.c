#include "BmpLib/bmp_parser.h"
#include "CLI/command_parser.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	char file_to_process[256];

	enum OPERATION_TYPE query_idx = parse_user_command(argc, argv);

	void *query = get_query_structure(query_idx);

	if(QueryFunctions[query_idx](argc, argv, file_to_process, query))
	{
		BMP *image = load_image(file_to_process);

		if(image != NULL)
		{
			TaskFunctions[query_idx](&image->matrix, query);

			(void)get_file_to_unload(query_idx, query, file_to_process);

			unload_image(file_to_process, image);

			safe_free_bmp(&image);
		}
		else
		{
			fprintf(stderr, "output file not found\n");
		}
	}
	else
	{
		fprintf(stderr, "query is invalid\n");
	}

	free(query);
	return 0;
}