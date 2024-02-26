module huffman_decoder(x, y, clk, reset);
input x, clk, reset;
output wire[2:0] y;

reg [3:0] cur_state;
reg [3:0] next_state;

parameter SA = 1, SB = 2, SC = 3, SD = 4, SE = 5, SF = 6;
parameter S0 = 0, S1 = 7, S10 = 8, S11 = 9, S110 = 10;


always @*
begin
    case (cur_state)
    S1:
        if(x == 0) next_state = S10;
        else next_state = S11;
    S10:
        if(x == 0) next_state = SC;
        else next_state = SB;
    S11:
        if(x == 0) next_state = S110;
        else next_state = SD;
    S110:
        if(x == 0) next_state = SF;
        else next_state = SE;
    default: // for S0, SA, SB, SC, SD, SE, SF
        if(x == 0) next_state = SA;
        else next_state = S1;
    endcase
end

always @(posedge clk or posedge reset)
begin
    if (reset == 1'b1)
        cur_state <= S0;
    else
        cur_state <= next_state;
end

// assign y = (cur_state >= 1 && cur_state <= 6) ? cur_state : 4'b0000;

assign y[2] = (~cur_state[3] & cur_state[2] & ~cur_state[1]) | (~cur_state[3] & cur_state[2] & cur_state[1] & ~cur_state[0]);
assign y[1] = (~cur_state[3] & ~cur_state[2] & cur_state[1]) | (~cur_state[3] & cur_state[1] & ~cur_state[0]);
assign y[0] = (~cur_state[3] & ~cur_state[1] & cur_state[0]) | (~cur_state[3] & ~cur_state[2] & cur_state[0]);


endmodule