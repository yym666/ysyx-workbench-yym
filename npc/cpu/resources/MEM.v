import "DPI-C" context function int dpmem_read(input int raddr);
import "DPI-C" context function void dpmem_write(input int waddr, input int wdata, input byte wmask);
    
module MEM(
    input clock,
    input reset,
    input valid,
    input wen,
    input [ 7:0] wmask,
    input [31:0] raddr,
    input [31:0] waddr,
    input [31:0] wdata, 
    output reg[31:0]rdata
);
    always @(posedge valid) begin
        if (valid) begin
            rdata = dpmem_read(raddr);
            if (wen) begin
                dpmem_write(waddr, wdata, wmask);
            end
        end
        else begin
            rdata = 0;
        end
    end
endmodule
