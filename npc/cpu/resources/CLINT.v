import "DPI-C" context function void diff_skip();
module CLINT(
    input clock,
    input reset,
    input [ 3:0]    axi_arid,
    input [ 7:0]    axi_arlen,
    input [ 2:0]    axi_arsize,
    input [ 1:0]    axi_arburst,
    input [31:0]    axi_araddr,
    input           axi_arvalid,
    output          axi_arready,
    //R
    output reg [63:0]   axi_rdata,
    output     [ 1:0]   axi_rresp,
    output              axi_rvalid,
    output              axi_rlast,
    output     [ 3:0]   axi_rid,
    input               axi_rready,
    //AW
    input [ 3:0]    axi_awid,
    input [ 7:0]    axi_awlen,
    input [ 2:0]    axi_awsize,
    input [ 1:0]    axi_awburst,
    input [31:0]    axi_awaddr,
    input           axi_awvalid,
    output          axi_awready,
    //W
    input [63:0]    axi_wdata,
    input [ 7:0]    axi_wstrb,
    input           axi_wvalid,
    input           axi_wlast,
    output          axi_wready,
    //B
    output [ 3:0]   axi_bid,
    output [ 1:0]   axi_bresp,
    output          axi_bvalid,
    input           axi_bready
);
//DontCare
    assign axi_bid = 0;
    assign axi_rid = 0;
    assign axi_rlast = 0;
    
    reg [63:0]  mtime;
    always @(posedge clock) begin
        if(reset) mtime <= 64'h0;
        else mtime <= mtime + 1;
    end

//LFSR
    reg [3:0]   lfsr;
    always @(posedge clock) begin
        if (reset) lfsr <= 4'b1111;
        else lfsr <= {lfsr[2:0], lfsr[3] ^ lfsr[2]};
    end

//READ
    reg         Rstate;
    reg         nxtRstate;
    reg [3:0]   delayR;
    reg [1:0]   axi_rresp_r;
    reg         axi_rvalid_r;
    reg         axi_arready_r;
    assign      axi_rresp   = axi_rresp_r;
    assign      axi_rvalid  = axi_rvalid_r;
    assign      axi_arready = axi_arready_r;
    parameter   Rstate1 = 1'b0;
    parameter   Rstate2 = 1'b1;

    always @(posedge clock) begin
        if(reset) begin
            Rstate <= Rstate1;
            delayR <= lfsr;
        end
        else 
            Rstate <= nxtRstate;
    end

    always @(posedge clock) begin
        case(Rstate)
            Rstate1:
                nxtRstate = (axi_arvalid & axi_arready_r) ? Rstate2 : Rstate1;
            Rstate2:
                nxtRstate = (axi_rvalid_r & axi_rready)   ? Rstate1 : Rstate2;
            default:
                nxtRstate = Rstate1;
        endcase
    end

    always @(posedge clock) begin
        case(nxtRstate)
            Rstate1: begin
                axi_arready_r   <= 1'b1;
                axi_rresp_r     <= 2'b00;
                axi_rvalid_r    <= 1'b0;
                delayR          <= lfsr;
            end
            Rstate2: begin
                axi_arready_r   <= 1'b0;
                axi_rresp_r     <= 2'b00;
                delayR          <= delayR - 1;
                if(delayR == 0)begin
                    diff_skip();
                    if(axi_araddr == 32'ha0000048) begin
                        axi_rdata   <= {32'b0, mtime[31:0]};
                        axi_rresp_r <= 2'b00;
                        axi_rvalid_r<= 1'b1;
                    end
                    else if(axi_araddr == 32'ha000004c) begin
                        axi_rdata   <= {32'b0, mtime[63:32]};
                        axi_rresp_r <= 2'b00;
                        axi_rvalid_r<= 1'b1;
                    end
                    else begin
                        axi_rvalid_r<= 1'b1;
                        axi_rresp_r <= 2'b01;
                        $error("UART read error");
                    end
                end
                else
                    axi_rvalid_r <= 1'b0;
            end
            default: begin
                axi_arready_r   <= 1'b1;
                axi_rresp_r     <= 2'b00;
                axi_rvalid_r    <= 1'b0;
            end
        endcase
    end
 
//WRITE 
    reg         Wstate;
    reg         nxtWstate;
    reg [3:0]   delayW;
    reg [1:0]   axi_bresp_r;
    reg         axi_wready_r;
    reg         axi_bvalid_r;
    reg         axi_awready_r;
    assign axi_bresp   = axi_bresp_r;
    assign axi_wready  = axi_wready_r;
    assign axi_bvalid  = axi_bvalid_r;
    assign axi_awready = axi_awready_r;
    parameter Wstate1  = 1'b0;
    parameter Wstate2  = 1'b1;

    always @(posedge clock) begin
        if(reset) begin
            Wstate <= Wstate1;
            delayW <= lfsr;
        end
        else
            Wstate <= nxtWstate;
    end


    always @(posedge clock) begin
        case(Wstate)
            Wstate1:
                nxtWstate = (axi_awvalid & axi_awready_r) & (axi_wvalid & axi_wready_r) ? Wstate2 : Wstate1;
            Wstate2:
                nxtWstate = (axi_bvalid_r & axi_bready) ? Wstate1 : Wstate2;
        endcase
    end

    always @(posedge clock)begin
        case(nxtWstate)
            Wstate1:begin
                axi_awready_r   <= 1'b1;
                axi_wready_r    <= 1'b1;
                axi_bresp_r     <= 2'b00;
                axi_bvalid_r    <= 1'b0;
                delayW      <= lfsr;
            end
            Wstate2:begin
                axi_awready_r   <= 1'b0;
                axi_wready_r    <= 1'b0;
                axi_bresp_r     <= 2'b00;
                delayW      <= delayW - 1;
                if(delayW == 0) begin
                    diff_skip();
                    axi_bvalid_r <= 1'b1;
                    $error("CLINT write error");
                end
                else
                    axi_bvalid_r <= 1'b0;
            end
        endcase
    end
endmodule
