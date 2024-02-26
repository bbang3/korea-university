// `define INPUT_LEN 27	// for FEEDCAFE
`define INPUT_LEN 26	// for DEADBEEF
// `define INPUT_LEN 23	// for DECAFACE


module TOP;
	wire [2:0] y_out;

	reg x_in;
	reg CLOCK;
	reg t_reset;
	integer i;
	reg [7:0] ascii_out;


	reg [7:0] answer_string [0:99]; 
	integer answer_len;	
	
	wire [0:`INPUT_LEN-1] x_test;

	// INPUT_LEN: 27 answer: FEEDCAFE
	// assign x_test = `INPUT_LEN'b1100_1101_1101_111_100_0_1100_1101; 
	
	// INPUT_LEN: 26 answer: DEADBEEF
	assign x_test = `INPUT_LEN'b111_1101_0_111_101_1101_1101_1100;
	
	// INPUT_LEN: 23 answer: DECAFACE
	// assign x_test = `INPUT_LEN'b111_1101_100_0_1100_0_100_1101; 
	
	// create instance of Huffman decoder
	huffman_decoder DEC(.y(y_out),.x(x_in),.clk(CLOCK),.reset(t_reset));
	
	
	// create reset signal
	initial
	begin
		t_reset = 0;
		#1 t_reset = 1;
		#1 t_reset = 0;
	end

	// create clock signal
	initial
	begin
		CLOCK = 1; 
	end
	always
	begin
		#5 CLOCK = ~CLOCK; // at every 5 time units
	end

	// create input signal and print output
	initial 
	begin
		answer_len=0;

		for(i=0;i<`INPUT_LEN;i=i+1)
		begin
			#2;
			x_in = x_test[i];
			#8;
			answer_string[answer_len] = ascii_out[7:0];
			answer_len = answer_len+1;

		end

		#10
			answer_string[answer_len] = ascii_out[7:0];
			answer_len = answer_len+1;

			$display("The answer is");
			for(i=0;i<answer_len;i=i+1)
				$write("%s",answer_string[i]);
			$display();	

		
		$finish;
	end
	
	// convert module outputs to ASCII coded characters
	always @(*)
	begin
		if (y_out == 3'b000)	// if output is Null
			ascii_out = 45;	// ASCII '-'
		else
			ascii_out = y_out + 64; // ASCII 'A'-'F'
	end

	initial
	begin
		$dumpfile("h3_output.vcd");
		$dumpvars;
	end

endmodule

