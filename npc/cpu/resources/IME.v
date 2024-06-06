// import "DPI-C" context function int dpmem_read(input int raddr, input byte wmask);

module IME(
    input       valid,
    input      [31:0] pc,
    output reg [31:0] inst
);
    always @(posedge valid) begin
        if(valid) begin
            inst =  dpmem_read(pc);
        end
    end
endmodule