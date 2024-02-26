module TOP;
reg clk;

initial
begin
    #5 clk=0;
end

always
begin
    #10 clk = ~clk;
    #1000 $finish;
    
end
endmodule