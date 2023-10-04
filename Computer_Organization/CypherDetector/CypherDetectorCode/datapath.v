module datapath(input wr_sum,
					input wr_counter,
					input sel_sum,
					input sel_counter,
					input read,
					input wr_comp,
					input [15:0] fullcypher,
					input [3:0] seq_input,
					input clock,
					output stop,
					output comparison,
					output check,
					output [9:0] sum);

	
	reg [3:0] reg_input;
	reg [2:0] reg_counter;
	reg [9:0] reg_sum;
	reg reg_comp;
	
	wire [3:0] w_cypher;
	wire [9:0] w_sum;
	wire [2:0] w_counter;
	
	wire [9:0] w_inp;
	wire [9:0] w_addsum;
	wire [2:0] w_addcounter;
	wire comp;
	
	assign sum		      = reg_sum;
	assign comparison		= reg_comp;
	assign w_inp 		   = {6'd0, reg_input};
	assign w_addsum	   = w_inp + reg_sum;
	assign w_addcounter	= reg_counter + 3'd1;
	assign stop 		   = reg_counter >= 4;
	
	
	/*MUXes*/
	assign w_cypher 	= reg_counter[1] ? (reg_counter[0] ? fullcypher[3:0] : fullcypher[7:4]) 
												  : (reg_counter[0] ? fullcypher[11:8] : fullcypher[15:12]);
	assign w_sum 		= sel_sum ? w_addsum : 10'd0;
	assign w_counter 	= sel_counter ? w_addcounter : 3'd0;
	
	assign comp       = ~((seq_input[3]^w_cypher[3]) || (seq_input[2]^w_cypher[2]) || 
							  (seq_input[1]^w_cypher[1]) || (seq_input[0]^w_cypher[0]));
	
	assign check         = reg_counter != 0 && !comp;
	
	/*Registers*/
	always @ (posedge clock) 
	begin
		if(wr_sum)
			reg_sum <= w_sum;
		if(wr_counter)
			reg_counter <= w_counter;
		if(read)
			reg_input <= seq_input;
		if(wr_comp && !check)
			reg_comp <= comp;
		else if(wr_comp && check)
			begin	
				reg_comp <= comp;
				reg_counter <= 3'd0;
			end
			
	end
					
endmodule