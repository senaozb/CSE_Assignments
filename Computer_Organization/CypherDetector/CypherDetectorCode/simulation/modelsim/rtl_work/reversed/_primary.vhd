library verilog;
use verilog.vl_types.all;
entity reversed is
    port(
        fullcypher      : in     vl_logic_vector(15 downto 0);
        fullcypher_r    : out    vl_logic_vector(15 downto 0)
    );
end reversed;
