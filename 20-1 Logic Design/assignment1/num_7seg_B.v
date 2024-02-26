module num_7seg_B(out,w,x,y,z);

input w,x,y,z;
output out;

wire w,x,y,z;
wire out;

assign out = w | (x & ~y) | (x & ~z) | (~y & ~z);

endmodule