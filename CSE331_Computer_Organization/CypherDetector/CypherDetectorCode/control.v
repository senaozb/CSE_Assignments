module control(output reg wr_sum,
					output reg wr_counter,
					output reg sel_sum,
					output reg sel_counter,
					output reg read,
					output reg wr_comp,
					input comparison,
					input check,
					input stop,
					input clock,
					input reset);

	reg [2:0] current_state, next_state;

	localparam	IDLE 		= 3'b000,
					READ     = 3'b001,
					DECIDE	= 3'b010,
					CHECK	   = 3'b011,
					EMPTY	   = 3'b100,
					SUM 		= 3'b101,
					COMP0		= 3'b110,
					COMP1		= 3'b111;
		
	//state register
	always @ (posedge clock) begin
		if(reset)
			current_state <= IDLE;
		else 
			current_state <= next_state;
	end
	
	//next state logic
	always @ (*) 
	begin
		case (current_state)
			IDLE: 
				begin
					next_state = READ;
				end
				
			READ: 
				begin
					if(stop)
						next_state = READ;
					else 
						next_state = DECIDE;
				end
				
			DECIDE: 
				begin
					if(check)
						next_state = CHECK;
					else 
						next_state = EMPTY;
				end
			
			CHECK: 
				begin
					next_state = SUM;
				end
			
			EMPTY: 
				begin
					next_state = SUM;
				end
				
			SUM: 
				begin
					if(comparison)
						next_state = COMP1;
					else 
						next_state = COMP0;
				end
				
			COMP0: 
				begin
					next_state = READ;
				end
				
			COMP1: 
				begin
					next_state = READ;
				end
		endcase
	end

	//Outputs
	always @ (*) begin
		wr_sum 			= 1'b0;
		wr_counter 		= 1'b0;
		sel_sum 		   = 1'b0;
		sel_counter		= 1'b0;
		read				= 1'b0;
		wr_comp			= 1'b0;
		
		case (current_state)
			IDLE: 
				begin
					read           = 1'b1;
					wr_sum 			= 1'b1;
					wr_counter 		= 1'b1;
				end
				
			READ: 
				begin
					read 		= 1'b1;
				end
			
			DECIDE: 
				begin
					wr_comp  = 1'b1;
				end
				
			CHECK: 
				begin
					wr_comp  = 1'b1;
				end
			
			SUM: 
				begin	
					sel_sum 	= 1'b1;
					wr_sum   = 1'b1;
				end
			
			COMP0: 
				begin
					wr_counter 	= 1'b1;
				end
				
			COMP1: 
				begin
					wr_counter 	= 1'b1;
					sel_counter = 1'b1;
				end
				
		endcase
	end

endmodule