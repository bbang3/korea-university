module addsub_cla (A, B, S, C, M, V);
parameter W = 4;

input [W-1:0] A, B;
input M;
output [W-1:0] S;
output C;
output V;

wire [W-1:0] p, g;
genvar i;
generate
for(i = 0; i < W; i = i + 1)
begin
    assign p[i] = A[i] ^ (B[i] ^ M);
    assign g[i] = A[i] & (B[i] ^ M);
end
endgenerate

wire [W:0] carrys;

cla_gen #(.W(W)) CLAGEN (.P(p), .G(g), .C0(M), .C(carrys));

assign S = p ^ carrys[W-1:0];
assign C = carrys[W];
assign V = carrys[W] ^ carrys[W-1];

endmodule


module cla_gen(P, G, C0, C);
parameter W = 4;

input [W-1:0] P, G;
input C0;
output [W:0] C;

assign C[0] = C0;

genvar i;
generate
for(i = 0; i < W; i = i + 1)
begin
    assign C[i + 1] = G[i] | (P[i] & C[i]);
end
endgenerate

endmodule
