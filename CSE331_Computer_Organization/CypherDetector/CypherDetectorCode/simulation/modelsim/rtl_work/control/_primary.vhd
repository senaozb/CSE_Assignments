library verilog;
use verilog.vl_types.all;
entity control is
    port(
        wr_sum          : out    vl_logic;
        wr_counter      : out    vl_logic;
        sel_sum         : out    vl_logic;
        sel_counter     : out    vl_logic;
        read            : out    vl_logic;
        wr_comp         : out    vl_logic;
        comparison      : in     vl_logic;
        check           : in     vl_logic;
        stop            : in     vl_logic;
        clock           : in     vl_logic;
        reset           : in     vl_logic
    );
end control;
