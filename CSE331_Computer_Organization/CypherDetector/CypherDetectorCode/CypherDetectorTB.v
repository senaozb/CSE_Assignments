module CypherDetectorTB(input d);


	reg [15:0] fullcypher;
	reg [3:0] seq_input;
	reg clock = 1'b0, reset;
	wire [9:0] sum;

	CypherDetector cd0(
		.fullcypher(fullcypher), 
		.seq_input(seq_input),
		.clock(clock), 
		.reset(reset),
		.sum(sum));
		
	always 
	begin
		#1
		clock = ~clock;
	end
	

	initial 
	begin
		reset = 1'b1;
		fullcypher = 16'b0010011000000001;
		seq_input = 4'b0000;
		
		#2
		reset = 1'b0;
		
		#10
		seq_input = 4'b0001;
		
		#10
		seq_input = 4'b0011;
		
		#10
		seq_input = 4'b0000;
		
		#10
		seq_input = 4'b0011;
		
		#10
		seq_input = 4'b0100;
		
		#10
		seq_input = 4'b0001;
		
		#10
		seq_input = 4'b0000;
		
		#10
		seq_input = 4'b0010;
		
		#10
		seq_input = 4'b0001;
		
		#10
		seq_input = 4'b0001;
		
		#10
		seq_input = 4'b0000;
		
		#10
		seq_input = 4'b0110;
		
		#10
		seq_input = 4'b0010;
		
		#10
		seq_input = 4'b0010;
		
		#10
		seq_input = 4'b0110;
		
		#10
		seq_input = 4'b0000;
		
	end 

endmodule