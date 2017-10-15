/**
 * @file methods.h
 * @author impworks
 * Main file for method definitions.
 * Defines all methods for built-in classes.
 */

#ifndef METHODS_H
#define METHODS_H

//****************************************************************
//*                                                              *
//*                            Objects                           *
//*                                                              *
//****************************************************************

//--------------------------------
//            object
//--------------------------------

void object_op_create(rc_head *head);
void object_tainted(rc_head *head);
void object_taint_do(rc_head *head);
void object_untaint_do(rc_head *head);
void object_frozen(rc_head *head);
void object_freeze_do(rc_head *head);
void object_id(rc_head *head);
void object_exec(rc_head *head);
void object_assert(rc_head *head);
void object_class(rc_head *head);
void object_class_name(rc_head *head);
void object_has_member(rc_head *head);
void object_has_method(rc_head *head);
void object_inspect(rc_head *head);
void object_to_a(rc_head *head);
void object_to_b(rc_head *head);
void object_to_f(rc_head *head);
void object_to_i(rc_head *head);
void object_to_s(rc_head *head);

void object_print(rc_head *head);

//--------------------------------
//             undef
//--------------------------------

void undef_op_any(rc_head *head);
void undef_op_cmp_any(rc_head *head);
void undef_op_rel_any(rc_head *head);
void undef_op_index(rc_head *head);
void undef_inspect(rc_head *head);

//--------------------------------
//             bool
//--------------------------------

void bool_op_create(rc_head *head);
void bool_op_band_bool(rc_head *head);
void bool_op_bor_bool(rc_head *head);
void bool_op_bxor_bool(rc_head *head);
void bool_op_cmp_bool(rc_head *head);
void bool_op_cmp_object(rc_head *head);
void bool_inspect(rc_head *head);
void bool_toggle_do(rc_head *head);
void bool_to_b(rc_head *head);
void bool_to_f(rc_head *head);
void bool_to_i(rc_head *head);
void bool_to_s(rc_head *head);

//--------------------------------
//             int
//--------------------------------

void int_op_create(rc_head *head);
void int_op_add_int(rc_head *head);
void int_op_add_float(rc_head *head);
void int_op_sub_int(rc_head *head);
void int_op_sub_float(rc_head *head);
void int_op_mul_int(rc_head *head);
void int_op_mul_float(rc_head *head);
void int_op_div_int(rc_head *head);
void int_op_div_float(rc_head *head);
void int_op_pow_int(rc_head *head);
void int_op_pow_float(rc_head *head);
void int_op_mod_int(rc_head *head);
void int_op_shl_int(rc_head *head);
void int_op_shr_int(rc_head *head);
void int_op_band_int(rc_head *head);
void int_op_bor_int(rc_head *head);
void int_op_bxor_int(rc_head *head);
void int_op_inc(rc_head *head);
void int_op_dec(rc_head *head);
void int_op_cmp_int(rc_head *head);
void int_op_cmp_float(rc_head *head);
void int_op_cmp_string(rc_head *head);
void int_op_cmp_bool(rc_head *head);
void int_op_rel_range(rc_head *head);
void int_inspect(rc_head *head);
void int_times(rc_head *head);
void int_to(rc_head *head);
void int_upto(rc_head *head);
void int_downto(rc_head *head);
void int_char(rc_head *head);
void int_to_b(rc_head *head);
void int_to_f(rc_head *head);
void int_to_i(rc_head *head);
void int_to_s(rc_head *head);

//--------------------------------
//            float
//--------------------------------

void float_op_create(rc_head *head);
void float_op_add_int(rc_head *head);
void float_op_add_float(rc_head *head);
void float_op_sub_int(rc_head *head);
void float_op_sub_float(rc_head *head);
void float_op_mul_int(rc_head *head);
void float_op_mul_float(rc_head *head);
void float_op_div_int(rc_head *head);
void float_op_div_float(rc_head *head);
void float_op_pow_int(rc_head *head);
void float_op_pow_float(rc_head *head);
void float_op_inc(rc_head *head);
void float_op_dec(rc_head *head);
void float_op_cmp_int(rc_head *head);
void float_op_cmp_float(rc_head *head);
void float_op_cmp_string(rc_head *head);
void float_op_cmp_bool(rc_head *head);
void float_op_rel_range(rc_head *head);
void float_inspect(rc_head *head);
void float_ceil(rc_head *head);
void float_floor(rc_head *head);
void float_decimal(rc_head *head);
void float_to_b(rc_head *head);
void float_to_f(rc_head *head);
void float_to_i(rc_head *head);
void float_to_s(rc_head *head);

//--------------------------------
//            string
//--------------------------------

void string_op_create(rc_head *head);
void string_op_add_string(rc_head *head);
void string_op_sub_string(rc_head *head);
void string_op_mul_int(rc_head *head);
void string_op_mod_array(rc_head *head);
void string_op_shl_bool(rc_head *head);
void string_op_shl_int(rc_head *head);
void string_op_shl_float(rc_head *head);
void string_op_shl_string(rc_head *head);
void string_op_shl_range(rc_head *head);
void string_op_shl_object(rc_head *head);
void string_op_idx(rc_head *head);
void string_op_cmp_bool(rc_head *head);
void string_op_cmp_int(rc_head *head);
void string_op_cmp_float(rc_head *head);
void string_op_cmp_string(rc_head *head);
void string_op_rel_regex(rc_head *head);
void string_inspect(rc_head *head);
void string_length(rc_head *head);
void string_trim(rc_head *head);
void string_trim_left(rc_head *head);
void string_trim_right(rc_head *head);
void string_trim_do(rc_head *head);
void string_trim_left_do(rc_head *head);
void string_trim_right_do(rc_head *head);
void string_ord(rc_head *head);
void string_reverse(rc_head *head);
void string_reverse_do(rc_head *head);
void string_sub(rc_head *head);
void string_sub_first(rc_head *head);
void string_sub_last(rc_head *head);
void string_sub_count(rc_head *head);
void string_append(rc_head *head);
void string_prepend(rc_head *head);
void string_replace(rc_head *head);
void string_append_do(rc_head *head);
void string_prepend_do(rc_head *head);
void string_replace_do(rc_head *head);
void string_split(rc_head *head);
void string_apply(rc_head *head);
void string_apply_do(rc_head *head);
void string_insert(rc_head *head);
void string_insert_do(rc_head *head);
void string_has(rc_head *head);
void string_case_up(rc_head *head);
void string_case_down(rc_head *head);
void string_case_swap(rc_head *head);
void string_case_fix(rc_head *head); // <<< todo
void string_case_up_do(rc_head *head);
void string_case_down_do(rc_head *head);
void string_case_swap_do(rc_head *head);
void string_case_fix_do(rc_head *head); // <<< todo
void string_chars(rc_head *head);
void string_lines(rc_head *head);
void string_translate(rc_head *head);
void string_translate_do(rc_head *head);
void string_is_alnum(rc_head *head); // <<< todo
void string_is_alpha(rc_head *head); // <<< todo
void string_is_ascii(rc_head *head); // <<< todo
void string_is_digit(rc_head *head); // <<< todo
void string_is_print(rc_head *head); // <<< todo
void string_is_upper(rc_head *head); // <<< todo
void string_to_b(rc_head *head);
void string_to_f(rc_head *head);
void string_to_i(rc_head *head);
void string_to_s(rc_head *head);

//--------------------------------
//           range
//--------------------------------

void range_op_create(rc_head *head);
void range_op_cmp_bool(rc_head *head);
void range_op_cmp_range(rc_head *head);
void range_op_cmp_string(rc_head *head);
void range_op_rel_int(rc_head *head);
void range_op_rel_float(rc_head *head);
void range_inspect(rc_head *head);
void range_length(rc_head *head);
void range_set_do(rc_head *head);
void range_next(rc_head *head);
void range_rewind(rc_head *head);
void range_has(rc_head *head);
void range_each(rc_head *head);
void range_start(rc_head *head);
void range_end(rc_head *head);
void range_to_a(rc_head *head);
void range_to_b(rc_head *head);
void range_to_f(rc_head *head);
void range_to_i(rc_head *head);
void range_to_s(rc_head *head);

//--------------------------------
//            regex
//--------------------------------

void regex_op_create(rc_head *head);
void regex_op_cmp_bool(rc_head *head);
void regex_op_cmp_regex(rc_head *head);
void regex_op_cmp_string(rc_head *head);
void regex_inspect(rc_head *head);
void regex_match(rc_head *head);
void regex_study(rc_head *head);
void regex_to_b(rc_head *head);
void regex_to_s(rc_head *head);

//--------------------------------
//            match
//--------------------------------

void match_op_create(rc_head *head);
void match_op_cmp_match(rc_head *head);
void match_op_cmp_bool(rc_head *head);
void match_op_cmp_string(rc_head *head);
void match_count(rc_head *head);
void match_get(rc_head *head);
void match_bounds(rc_head *head);
void match_to_b(rc_head *head);
void match_to_i(rc_head *head);
void match_to_s(rc_head *head);

//--------------------------------
//             time
//--------------------------------

void time_op_create(rc_head *head);
void time_op_add_int(rc_head *head);
void time_op_add_time(rc_head *head);
void time_op_sub_int(rc_head *head);
void time_op_sub_time(rc_head *head);
void time_op_mul_int(rc_head *head);
void time_op_div_int(rc_head *head);
void time_op_inc(rc_head *head);
void time_op_dec(rc_head *head);
void time_op_cmp_time(rc_head *head);
void time_op_cmp_int(rc_head *head);
void time_op_cmp_bool(rc_head *head);
void time_op_cmp_string(rc_head *head);
void time_inspect(rc_head *head);
void time_now_do(rc_head *head);
void time_year(rc_head *head);
void time_month(rc_head *head);
void time_day_m(rc_head *head);
void time_day_w(rc_head *head);
void time_day_y(rc_head *head);
void time_hour(rc_head *head);
void time_min(rc_head *head);
void time_sec(rc_head *head);
void time_values(rc_head *head);
void time_format(rc_head *head);
void time_to_b(rc_head *head);
void time_to_f(rc_head *head);
void time_to_i(rc_head *head);
void time_to_s(rc_head *head);

//--------------------------------
//             file
//--------------------------------

void file_op_create(rc_head *head);
void file_inspect(rc_head *head);
void file_open(rc_head *head);
void file_close(rc_head *head);
void file_read(rc_head *head);
void file_read_all(rc_head *head);
void file_read_lines(rc_head *head);
void file_write(rc_head *head);
void file_seek(rc_head *head);
void file_tell(rc_head *head);
void file_exists(rc_head *head);
void file_copy(rc_head *head);
void file_rename(rc_head *head);
void file_remove(rc_head *head);
void file_truncate(rc_head *head);
void file_info(rc_head *head);
void file_length(rc_head *head);
void file_to_b(rc_head *head);
void file_to_i(rc_head *head);
void file_to_s(rc_head *head);

//--------------------------------
//            socket
//--------------------------------

void socket_op_create(rc_head *head);
void socket_inspect(rc_head *head);
void socket_open(rc_head *head);
void socket_close(rc_head *head);
void socket_read(rc_head *head);
void socket_read_all(rc_head *head);
void socket_read_lines(rc_head *head);
void socket_write(rc_head *head);
void socket_length(rc_head *head);
void socket_to_b(rc_head *head);
void socket_to_i(rc_head *head);
void socket_to_s(rc_head *head);

//--------------------------------
//             dir
//--------------------------------

void dir_op_create(rc_head *head);
void dir_inspect(rc_head *head);
void dir_content(rc_head *head);
void dir_files(rc_head *head);
void dir_dirs(rc_head *head);
void dir_exists(rc_head *head);
void dir_create(rc_head *head);
void dir_copy(rc_head *head);
void dir_rename(rc_head *head);
void dir_remove(rc_head *head);
void dir_to_b(rc_head *head);
void dir_to_f(rc_head *head);
void dir_to_i(rc_head *head);
void dir_to_s(rc_head *head);

//--------------------------------
//            array
//--------------------------------

void array_op_create(rc_head *head);
void array_op_idx(rc_head *head);
void array_op_add_array(rc_head *head);
void array_op_shl_object(rc_head *head);
void array_op_cmp_bool(rc_head *head);
void array_op_cmp_int(rc_head *head);
void array_op_cmp_string(rc_head *head);
void array_op_cmp_array(rc_head *head);
void array_op_splat(rc_head *head);
void array_inspect(rc_head *head);
void array_taint_all_do(rc_head *head);
void array_untaint_all_do(rc_head *head);
void array_assert_all(rc_head *head);
void array_assert_any(rc_head *head);
void array_any(rc_head *head);
void array_all(rc_head *head);
void array_has(rc_head *head);
void array_each(rc_head *head);
void array_each_key(rc_head *head);
void array_each_pair(rc_head *head);
void array_next(rc_head *head);
void array_next_key(rc_head *head);
void array_next_pair(rc_head *head);
void array_rewind(rc_head *head);
void array_find(rc_head *head);
void array_flip(rc_head *head);    // <<< todo
void array_flip_do(rc_head *head); // <<< todo
void array_sort(rc_head *head);
void array_sort_do(rc_head *head);
void array_sort_quick(rc_head *head);
void array_sort_quick_do(rc_head *head);
void array_sort_shell(rc_head *head);
void array_sort_shell_do(rc_head *head);
void array_sort_by(rc_head *head);    // <<< todo
void array_sort_by_do(rc_head *head); // <<< todo
void array_shuffle(rc_head *head);
void array_shuffle_do(rc_head *head);
void array_map_do(rc_head *head);
void array_sum(rc_head *head);
void array_mul(rc_head *head);
void array_join(rc_head *head);
void array_reindex(rc_head *head);
void array_reindex_do(rc_head *head);
void array_flatten(rc_head *head);
void array_flatten_do(rc_head *head);
void array_inflate(rc_head *head);
void array_inflate_do(rc_head *head);
void array_unique(rc_head *head);
void array_unique_do(rc_head *head);
void array_select(rc_head *head);
void array_select_do(rc_head *head);
void array_reject(rc_head *head);
void array_reject_do(rc_head *head);
void array_collect(rc_head *head);
void array_pop_do(rc_head *head);
void array_push_do(rc_head *head);
void array_clear_do(rc_head *head);
void array_reverse(rc_head *head);    // <<< todo
void array_reverse_do(rc_head *head); // <<< todo
void array_zip(rc_head *head);
void array_zip_do(rc_head *head);
void array_min(rc_head *head);
void array_max(rc_head *head);
void array_min_max(rc_head *head);
void array_length(rc_head *head);
void array_to_b(rc_head *head);
void array_to_i(rc_head *head);
void array_to_s(rc_head *head);

//--------------------------------
//            method
//--------------------------------

void method_op_cmp_method(rc_head *head);
void method_op_cmp_bool(rc_head *head);
void method_op_cmp_string(rc_head *head);
void method_inspect(rc_head *head);
void method_call(rc_head *head);
void method_class(rc_head *head);
void method_args(rc_head *head);
void method_args_min(rc_head *head);
void method_args_max(rc_head *head);
void method_to_s(rc_head *head);

//--------------------------------
//            class
//--------------------------------

void class_op_create(rc_head *head);
void class_op_cmp_class(rc_head *head);
void class_op_cmp_bool(rc_head *head);
void class_op_cmp_string(rc_head *head);
void class_inspect(rc_head *head);
void class_name(rc_head *head);
void class_methods(rc_head *head);
void class_operators(rc_head *head);
void class_members(rc_head *head);
void class_members_static(rc_head *head);
void class_count(rc_head *head);
void class_parent(rc_head *head);
void class_to_s(rc_head *head);

//--------------------------------
//          exception
//--------------------------------

void exception_op_create(rc_head *head);
void exception_op_cmp_exception(rc_head *head);
void exception_op_cmp_bool(rc_head *head);
void exception_op_cmp_string(rc_head *head);
void exception_inspect(rc_head *head);
void exception_msg(rc_head *head);
void exception_type(rc_head *head);
void exception_file(rc_head *head);
void exception_line(rc_head *head);
void exception_to_b(rc_head *head);
void exception_to_s(rc_head *head);

//****************************************************************
//*                                                              *
//*                          Namespaces                          *
//*                                                              *
//****************************************************************

//--------------------------------
//            malco
//--------------------------------

void malco_build(rc_head *head);
void malco_release(rc_head *head);
void malco_version(rc_head *head);
void malco_unicode(rc_head *head);
void malco_copyright(rc_head *head);
void malco_modules(rc_head *head); // <<< todo
void malco_error_mode(rc_head *head);
void malco_how_to_create_explosives(rc_head *head);

//--------------------------------
//            math
//--------------------------------

void math_abs(rc_head *head);
void math_sin(rc_head *head);
void math_sinh(rc_head *head);
void math_cos(rc_head *head);
void math_cosh(rc_head *head);
void math_tan(rc_head *head);
void math_tanh(rc_head *head);
void math_arcsin(rc_head *head);
void math_arcsinh(rc_head *head);
void math_arccos(rc_head *head);
void math_arccosh(rc_head *head);
void math_arctan(rc_head *head);
void math_arctanh(rc_head *head);
void math_sqrt(rc_head *head);
void math_cbrt(rc_head *head);
void math_exp(rc_head *head);
void math_log(rc_head *head);
void math_log2(rc_head *head);
void math_log10(rc_head *head);
void math_pi(rc_head *head);
void math_e(rc_head *head);
void math_random(rc_head *head);
void math_min(rc_head *head);
void math_max(rc_head *head);
void math_even(rc_head *head);
void math_base_cvt(rc_head *head);

#endif