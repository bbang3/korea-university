module TOP;

wire w,x,y,z,out;
reg [3:0] num;

// instantiation of the module
num_7seg_B SEG_B(out,w,x,y,z);


// simulation ends at time 200
initial #200 $finish;


// num change from 0 to 15, incremented every 5 seconds
initial begin
	num=0;
	repeat (15)
		#5 num=num+1; 
end

// dump files
initial
begin
	$dumpfile("h1_output.vcd");
	$dumpvars;
end

// assignment of signals
assign w=num[3];
assign x=num[2];
assign y=num[1];
assign z=num[0];

endmodule
