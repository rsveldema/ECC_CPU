

typedef struct packed {
	int64_t[(NUMBER_OF_VECTOR_THREADS_INT64) - 1:0] data;
} VectorValue;


function int64_t get(input VectorValue v, input uint32_t ix);
begin
begin
	assert((ix < NUMBER_OF_VECTOR_THREADS_INT64));
	return v.data[ix];
end
end
endfunction


function VectorValue or_shift_left(input VectorValue self, input VectorValue other, input uint32_t shift_count);
begin
	VectorValue ret;
begin
	for (uint32_t i = 0; (i < NUMBER_OF_VECTOR_THREADS_INT64); i=(i + 1))
		begin
			ret.data[i] <= (self.data[i] | (other.data[i] << shift_count));
		end
	return ret;
end
end
endfunction


function VectorValue shift_left(input VectorValue self, input VectorValue other);
begin
	VectorValue ret;
begin
	for (uint32_t i = 0; (i < NUMBER_OF_VECTOR_THREADS_INT64); i=(i + 1))
		begin
			ret.data[i] <= (self.data[i] << other.data[i]);
		end
	return ret;
end
end
endfunction


function VectorValue add(input VectorValue self, input VectorValue other);
begin
	VectorValue ret;
begin
	for (uint32_t i = 0; (i < NUMBER_OF_VECTOR_THREADS_INT64); i=(i + 1))
		begin
			ret.data[i] <= (self.data[i] + other.data[i]);
		end
	return ret;
end
end
endfunction


function VectorValue bit_and(input VectorValue self, input VectorValue other);
begin
	VectorValue ret;
begin
	for (uint32_t i = 0; (i < NUMBER_OF_VECTOR_THREADS_INT64); i=(i + 1))
		begin
			ret.data[i] <= (self.data[i] & other.data[i]);
		end
	return ret;
end
end
endfunction


function bool all_equal(input VectorValue self);
begin
begin
	for (uint32_t i = 1; (i < NUMBER_OF_VECTOR_THREADS_INT64); i=(i + 1))
		begin
			if ((self.data[0] != self.data[i]))
			begin
				return 0;
			end
		end
	return 1;
end
end
endfunction


function int64_t get_compare_result(input int64_t value1, input int64_t value2);
begin
begin
	if ((value1 == value2))
	begin
		return FLAGS_MASK_EQ;
	end
	if ((value1 > value2))
	begin
		return FLAGS_MASK_GT;
	end
	return FLAGS_MASK_LT;
end
end
endfunction


function VectorValue compare_vecs(input VectorValue self, input VectorValue other);
begin
	VectorValue ret;
begin
	for (uint32_t i = 0; (i < NUMBER_OF_VECTOR_THREADS_INT64); i=(i + 1))
		begin
			ret.data[i] <= get_compare_result(self.data[i], other.data[i]);
		end
	return ret;
end
end
endfunction


function bool are_all_adjacent_memory_addresses(input VectorValue v, input int64_t elt_size);
begin
	int64_t first;
begin
	first <= v.data[0];
	for (uint32_t i = 1; (i < NUMBER_OF_VECTOR_THREADS_INT64); i=(i + 1))
		begin
			if ((v.data[i] != (v.data[(i - 1)] + elt_size)))
			begin
				return 0;
			end
		end
	return 1;
end
end
endfunction


function bool areAllValidMemoryAddresses(input VectorValue v);
begin
begin
	return 1;
end
end
endfunction


function uint64_t reduce_to_uint64_t(input VectorValue v);
begin
	uint64_t ret;
begin
	ret <= 0;
	for (uint32_t i = 0; (i < NUMBER_OF_VECTOR_THREADS_INT64); i=(i + 1))
		begin
			`SET_BIT(ret, i, (v.data[i] != 0));
		end
	return ret;
end
end
endfunction


function VectorValue create_vec_incrementing_values();
begin
	VectorValue ret;
begin
	for (uint32_t i = 0; (i < NUMBER_OF_VECTOR_THREADS_INT64); i=(i + 1))
		begin
			ret.data[i] <= (int64_t'(i));
		end
	return ret;
end
end
endfunction


function VectorValue create_vec_int64(input int64_t v);
begin
	VectorValue ret;
begin
	for (uint32_t i = 0; (i < NUMBER_OF_VECTOR_THREADS_INT64); i=(i + 1))
		begin
			ret.data[i] <= v;
		end
	return ret;
end
end
endfunction


function VectorValue create_vec_int8(input int8_t v);
begin
begin
	return create_vec_int64((int64_t'(v)));
end
end
endfunction


function VectorValue create_vec_int16(input int16_t v);
begin
begin
	return create_vec_int64((int64_t'(v)));
end
end
endfunction


function VectorValue create_vec_int32(input int32_t v);
begin
begin
	return create_vec_int64((int64_t'(v)));
end
end
endfunction
