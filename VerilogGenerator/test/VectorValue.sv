

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
begin
	return {(self.data[0] | (other.data[0] << shift_count)), (self.data[1] | (other.data[1] << shift_count)), (self.data[2] | (other.data[2] << shift_count)), (self.data[3] | (other.data[3] << shift_count)), (self.data[4] | (other.data[4] << shift_count)), (self.data[5] | (other.data[5] << shift_count)), (self.data[6] | (other.data[6] << shift_count)), (self.data[7] | (other.data[7] << shift_count))};
end
end
endfunction


function VectorValue shift_left(input VectorValue self, input VectorValue other);
begin
begin
	return {(self.data[0] << other.data[0]), (self.data[1] << other.data[1]), (self.data[2] << other.data[2]), (self.data[3] << other.data[3]), (self.data[4] << other.data[4]), (self.data[5] << other.data[5]), (self.data[6] << other.data[6]), (self.data[7] << other.data[7])};
end
end
endfunction


function VectorValue add(input VectorValue self, input VectorValue other);
begin
begin
	return {(self.data[0] + other.data[0]), (self.data[1] + other.data[1]), (self.data[2] + other.data[2]), (self.data[3] + other.data[3]), (self.data[4] + other.data[4]), (self.data[5] + other.data[5]), (self.data[6] + other.data[6]), (self.data[7] + other.data[7])};
end
end
endfunction


function VectorValue bit_and(input VectorValue self, input VectorValue other);
begin
begin
	return {(self.data[0] & other.data[0]), (self.data[1] & other.data[1]), (self.data[2] & other.data[2]), (self.data[3] & other.data[3]), (self.data[4] & other.data[4]), (self.data[5] & other.data[5]), (self.data[6] & other.data[6]), (self.data[7] & other.data[7])};
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
begin
	return {get_compare_result(self.data[0], other.data[0]), get_compare_result(self.data[1], other.data[1]), get_compare_result(self.data[2], other.data[2]), get_compare_result(self.data[3], other.data[3]), get_compare_result(self.data[4], other.data[4]), get_compare_result(self.data[5], other.data[5]), get_compare_result(self.data[6], other.data[6]), get_compare_result(self.data[7], other.data[7])};
end
end
endfunction


function bool are_all_adjacent_memory_addresses(input VectorValue v, input int64_t elt_size);
begin
begin
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
begin
	return `PACK8_BITS((v.data[0] != 0), (v.data[1] != 0), (v.data[2] != 0), (v.data[3] != 0), (v.data[4] != 0), (v.data[5] != 0), (v.data[6] != 0), (v.data[7] != 0));
end
end
endfunction


function VectorValue create_vec_incrementing_values();
begin
begin
	return {(int64_t'(0)), (int64_t'(1)), (int64_t'(2)), (int64_t'(3)), (int64_t'(4)), (int64_t'(5)), (int64_t'(6)), (int64_t'(7))};
end
end
endfunction


function VectorValue create_vec_int64(input int64_t v);
begin
begin
	return {v, v, v, v, v, v, v, v};
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
