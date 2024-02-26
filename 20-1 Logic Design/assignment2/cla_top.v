

module TOP;
	parameter WIDTH = 4;

	reg signed [WIDTH-1:0] a,b;
	reg signed [WIDTH-1:0] a_test[0:9];
	reg signed [WIDTH-1:0] b_test[0:9];
	reg signed m_test[0:9];
	wire signed [WIDTH-1:0] sum;
	wire carry;
	reg sub_not_add;
	reg [31:0] i;

	wire overflow;
	wire [WIDTH-1:0] grade_P;

	// create instance of carry lookahead adder
	addsub_cla #(.W(WIDTH)) CLA( .A(a), .B(b), .S(sum), .C(carry),.M(sub_not_add), .V(overflow));
	
	// simulate for 200 time units
	initial #200 $finish;

	// set test vectors
	initial
	begin
		a_test[0]=-1; b_test[0]=2; m_test[0]=0;
		a_test[1]= 2; b_test[1]=3; m_test[1]=1;
		a_test[2]= 1; b_test[2]=2; m_test[2]=0;
		a_test[3]= 4; b_test[3]=-1; m_test[3]=1;
		a_test[4]=-1; b_test[4]=-7; m_test[4]=0;
		a_test[5]=-2; b_test[5]=2; m_test[5]=1;
		a_test[6]=-5; b_test[6]=-4; m_test[6]=0;
		a_test[7]=-3; b_test[7]=5; m_test[7]=1;
		a_test[8]=2; b_test[8]=7; m_test[8]=0;
		a_test[9]=3; b_test[9]=3; m_test[9]=1;

	end

	initial
	begin
		// initialize variables
		a=0;
		b=0;
		sub_not_add=0;

		for(i=0;i<10;i=i+1)
		begin
			#5 
			a=a_test[i];	// read test values
			b=b_test[i];
			sub_not_add=m_test[i];
		end
	end
	
	initial
	begin
		$dumpfile("h2_output.vcd");
		$dumpvars;
	end
	
endmodule

