library verilog;
use verilog.vl_types.all;
entity CypherDetector is
    port(
        fullcypher      : in     vl_logic_vector(15 downto 0);
        seq_input       : in     vl_logic_vector(3 downto 0);
        clock           : in     vl_logic;
        reset           : in     vl_logic;
        sum             : out    vl_logic_vector(9 downto 0)
    );
end CypherDetector;
