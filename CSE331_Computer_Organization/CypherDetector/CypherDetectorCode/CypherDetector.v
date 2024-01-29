module CypherDetector(input [15:0] fullcypher, 
							 input [3:0] seq_input, 
							 input clock, reset,
							 output [9:0] sum);
	
   wire wr_sum, wr_counter, wr_comp;
	wire sel_sum, sel_counter;
	wire read, comparison, check;
	wire stop;			
	wire [9:0] temp_sum;
	wire [15:0] fullcypher_r;
	reversed r(.fullcypher(fullcypher), .fullcypher_r(fullcypher_r));

	assign sum = temp_sum;	
							 
	datapath d0(
		.wr_sum(wr_sum),
		.wr_counter(wr_counter),
		.sel_sum(sel_sum),
		.sel_counter(sel_counter),
		.read(read),
		.wr_comp (wr_comp), 
		.fullcypher(fullcypher_r),
		.seq_input(seq_input),
		.clock(clock),
		.stop(stop),
		.comparison(comparison),
		.check(check),
		.sum(temp_sum)
	);
		
	control c0(
		.wr_sum(wr_sum),
		.wr_counter(wr_counter),
		.sel_sum(sel_sum),
		.sel_counter(sel_counter),
		.read(read),
		.wr_comp(wr_comp),
		.comparison(comparison),
		.check(check),
		.stop(stop),
		.clock(clock),
		.reset(reset)
	);
	
endmodule

module reversed(input [15:0] fullcypher, output [15:0] fullcypher_r);

	assign fullcypher_r[15:12] = fullcypher[3:0];
	assign fullcypher_r[11:8] = fullcypher[7:4];
	assign fullcypher_r[7:4] = fullcypher[11:8];
	assign fullcypher_r[3:0] = fullcypher[15:12];

endmodule

