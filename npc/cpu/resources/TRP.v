import "DPI-C" context function void check_halt(input bit flag);

module TRP(
    input wire      clock,
    input wire      reset,
    input wire      halt
);

  always @(halt) begin
      check_halt(halt);
  end

endmodule