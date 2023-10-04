library verilog;
use verilog.vl_types.all;
entity datapath is
    port(
        wr_sum          : in     vl_logic;
        wr_counter      : in     vl_logic;
        sel_sum         : in     vl_logic;
        sel_counter     : in     vl_logic;
        read            : in     vl_logic;
        wr_comp         : in     vl_logic;
        fullcypher      : in     vl_logic_vector(15 downto 0);
        seq_input       : in     vl_logic_vector(3 downto 0);
        clock           : in     vl_logic;
        stop            : out    vl_logic;
        comparison      : out    vl_logic;
        check           : out    vl_logic;
        sum             : out    vl_logic_vector(9 downto 0)
    );
end datapath;
