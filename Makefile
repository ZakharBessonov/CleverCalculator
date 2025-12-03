flags=-ggdb3 -std=c++20 -O0 -Wall -Wextra -Weffc++ -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual\
-Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-security -Wformat-nonliteral -Wformat=2 -Winline\
-Wnon-virtual-dtor -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wshadow -Wsign-conversion -Wsign-promo\
-Wstrict-overflow=2 -Wsuggest-override -Wswitch-default -Wundef -Wunreachable-code -Wunused -Wvariadic-macros\
-Wno-missing-field-initializers -Wno-narrowing -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation\
-fstack-protector -fstrict-overflow -Wlarger-than=8192 -fPIE -Werror=vla -Wchar-subscripts -Wformat-signedness\
-Wredundant-decls -Wswitch-enum -Wno-old-style-cast -fno-omit-frame-pointer -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,nonnull-attribute,null,return,returns-nonnull-attribute,\shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr

CC=g++

headers=-Isources/binary_search\
		-Isources/calc_colors\
		-Isources/calc_comparators\
		-Isources/calc_dump\
		-Isources/calc_funcs_for_operations\
		-Isources/calc_funcs_for_options\
		-Isources/calc_general_funcs\
		-Isources/calc_grammar_constructions\
		-Isources/calc_hash\
		-Isources/calc_read_write_to_file\
		-Isources/calc_set_get\
		-Isources/size_of_file\
		-Isources/calc_funcs_for_writing_to_TeX\
		-Isources/calc_simplification\
		-Isources/calc_derivatives\
		-Isources/calc_other_headers\
		-Isources/calc_other_headers\
		-Isources/calc_other_headers\

sources=sources/main.cpp\
		sources/binary_search/binary_search.cpp\
		sources/calc_colors/calc_colors.cpp\
		sources/calc_comparators/calc_comparators.cpp\
		sources/calc_dump/calc_dump.cpp\
		sources/calc_funcs_for_operations/calc_funcs_for_operations.cpp\
		sources/calc_funcs_for_options/calc_funcs_for_options.cpp\
		sources/calc_general_funcs/calc_general_funcs.cpp\
		sources/calc_grammar_constructions/calc_grammar_constructions.cpp\
		sources/calc_hash/calc_hash.cpp\
		sources/calc_math_operations/calc_math_operations.cpp\
		sources/calc_read_write_to_file/calc_read_write_to_file.cpp\
		sources/calc_set_get/calc_set_get.cpp\
		sources/size_of_file/size_of_file.cpp\
		sources/calc_funcs_for_writing_to_TeX/calc_funcs_for_writing_to_TeX.cpp\
		sources/calc_simplification/calc_simplification.cpp\
		sources/calc_derivatives/calc_derivatives.cpp

all:
	$(CC) -g $(sources) -o calculator $(headers) $(flags)
