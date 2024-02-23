-- cpu.vhd: Simple 8-bit CPU (BrainF*ck interpreter)
-- Copyright (C) 2019 Brno University of Technology,
--                    Faculty of Information Technology
-- Author(s): Monika Rosinkská
--

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity cpu is
 port (
   CLK   : in std_logic;  -- hodinovy signal
   RESET : in std_logic;  -- asynchronni reset procesoru
   EN    : in std_logic;  -- povoleni cinnosti procesoru
 
   -- synchronni pamet RAM
   DATA_ADDR  : out std_logic_vector(12 downto 0); -- adresa do pameti
   DATA_WDATA : out std_logic_vector(7 downto 0); -- mem[DATA_ADDR] <- DATA_WDATA pokud DATA_EN='1'
   DATA_RDATA : in std_logic_vector(7 downto 0);  -- DATA_RDATA <- ram[DATA_ADDR] pokud DATA_EN='1'
   DATA_RDWR  : out std_logic;                    -- cteni (0) / zapis (1)
   DATA_EN    : out std_logic;                    -- povoleni cinnosti
   
   -- vstupni port
   IN_DATA   : in std_logic_vector(7 downto 0);   -- IN_DATA <- stav klavesnice pokud IN_VLD='1' a IN_REQ='1'
   IN_VLD    : in std_logic;                      -- data platna
   IN_REQ    : out std_logic;                     -- pozadavek na vstup data
   
   -- vystupni port
   OUT_DATA : out  std_logic_vector(7 downto 0);  -- zapisovana data
   OUT_BUSY : in std_logic;                       -- LCD je zaneprazdnen (1), nelze zapisovat
   OUT_WE   : out std_logic                       -- LCD <- OUT_DATA pokud OUT_WE='1' a OUT_BUSY='0'
 );
end cpu;


-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of cpu is

 -- zde dopiste potrebne deklarace signalu
 signal PC: std_logic_vector (12 downto 0);
 signal PC_dec: std_logic;
 signal PC_inc: std_logic;
 
 signal PTR: std_logic_vector (12 downto 0);
 signal PTR_dec: std_logic;
 signal PTR_inc: std_logic;
 signal CNT: std_logic_vector (12 downto 0);
 signal CNT_dec: std_logic;
 signal CNT_inc: std_logic;
 
 signal sel1: std_logic;
 signal sel2: std_logic;
 signal sel3: std_logic_vector (1 downto 0);
 
 signal mx2out:  std_logic_vector (12 downto 0);
 type fsm_state is (fetch, decode, load, ptr_incr, ptr_decr, val_inc, val_inc2, val_dec, val_dec2, lbrac, lbrac2, lbrac3, lbrac4, rbrac, rbrac2, rbrac3, rbrac4, rbrac5, print, print2, readv, storeto, storeto2, storefrom, storefrom2, theend, other);
 signal present_state, nextstate : fsm_state;

begin

 -- zde dopiste vlastni VHDL kod


 -- pri tvorbe kodu reflektujte rady ze cviceni INP, zejmena mejte na pameti, ze 
 --   - nelze z vice procesu ovladat stejny signal,
 --   - je vhodne mit jeden proces pro popis jedne hardwarove komponenty, protoze pak
 --   - u synchronnich komponent obsahuje sensitivity list pouze CLK a RESET a 
 --   - u kombinacnich komponent obsahuje sensitivity list vsechny ctene signaly.
	
	OUT_DATA <= DATA_RDATA;
 
 	MX1: process(sel1, PC, mx2out)
	begin
		if sel1 = '0' then
			DATA_ADDR <= PC;
		else
			DATA_ADDR <= mx2out;
		end if;
	end process;
	
	MX2: process(sel2, PTR)
	begin
		if sel2 = '0' then
			mx2out <= PTR;
		else
			mx2out <= "1000000000000";
		end if;
	end process;
	
	MX3: process(sel3, DATA_RDATA, IN_DATA)
	begin
		if sel3 = "00" then
			DATA_WDATA <= IN_DATA;
		elsif sel3 = "01" then
			DATA_WDATA <= DATA_RDATA-1;
		elsif sel3 = "10" then
		   DATA_WDATA <= DATA_RDATA+1;
		else
			DATA_WDATA <= DATA_RDATA;
		end if;
	end process;
	
	PC_P: process(RESET, CLK)
	begin
		if RESET = '1' then
			PC <= (others => '0');
		elsif	rising_edge(CLK) then
			if PC_inc = '1' then
				PC <= PC + 1;
			end if;
			if PC_dec = '1' then
				PC <= PC - 1;
			end if;
		end if;
	end process;
	
		PTR_P: process(PTR_inc, PTR_dec, RESET, CLK)
	begin
		if RESET = '1' then
			PTR <= "1000000000000";
		elsif	rising_edge(CLK) then
			if EN = '1' then
				if PTR_inc = '1' then
					if PTR = "1111111111111" then
						PTR <= "1000000000000";
					else
						PTR <= PTR + 1;
					end if;
				end if;
			end if;
			if PTR_dec = '1' then
				if PTR = "1000000000000" then
					PTR <= "1111111111111";
				else
					PTR <= PTR - 1;
				end if;
			end if;
		end if;
	end process;
	
	CNT_P: process(CNT_inc, CNT_dec, RESET, CLK)
	begin
		if RESET = '1' then
			CNT <= (others => '0');
		elsif	rising_edge(CLK) then
			if CNT_inc = '1' then
				CNT <= CNT + 1;
			end if;
			if CNT_dec = '1' then
				CNT<= CNT - 1;
			end if;
		end if;
	end process;
	
	sync_logic : process(RESET, CLK)
	begin
		if (RESET = '1') then
			present_state <= fetch;
		elsif (CLK'event AND CLK = '1') then
			if EN = '1' then
				present_state <= nextstate;
			end if;
		end if;
	end process sync_logic;
	
	next_state_logic : process(present_state, nextstate, OUT_BUSY, IN_VLD, CNT, DATA_RDATA)
	begin
		PC_inc <= '0';
		PC_dec <= '0';
		PTR_inc <= '0';
		PTR_dec <= '0';
		CNT_inc <= '0';
		CNT_dec <= '0';
		DATA_EN <= '0';
		DATA_RDWR <= '0';
		IN_REQ <= '0';
		OUT_WE <= '0';
		sel1 <= '0';
		sel2 <= '0';
		sel3 <= "00";
		nextstate <= fetch;
		case (present_state) is
			when fetch =>
				DATA_EN <= '1';
				DATA_RDWR <= '0';
				sel1 <= '0';
				sel2 <= '0';
				sel3 <= "00";
				nextstate <= decode;
			when decode =>
				case DATA_RDATA is
					when X"3E" => nextstate <= ptr_incr;
					when X"3C" => nextstate <= ptr_decr;
					when X"2B" => nextstate <= val_inc;
					when X"2D" => nextstate <= val_dec;
					when X"5B" => nextstate <= lbrac;
					when X"5D" => nextstate <= rbrac;
					when X"2E" => nextstate <= print;
					when X"2C" => nextstate <= readv;
					when X"24" => nextstate <= storeto;
					when X"21" => nextstate <= storefrom;
					when X"00" => nextstate <= theend;
					when others => nextstate <= other;
				end case;
			-- when ">"
			when ptr_incr =>
				PTR_inc <= '1';
				PC_inc <= '1';
				nextstate <= fetch;
			-- when "<"
			when ptr_decr =>
				PTR_dec <= '1';
				PC_inc <= '1';
				nextstate <= fetch;
			-- when "+"
			when val_inc =>
				DATA_RDWR <= '0';
				DATA_EN <= '1';
				sel2 <= '0';
				sel1 <= '1';
				nextstate <= val_inc2;
			when val_inc2 =>
				sel1 <= '1';
				sel2 <= '0';
				sel3 <= "10";
				DATA_RDWR <= '1';
				DATA_EN <= '1';
				nextstate <= fetch;
				PC_inc <= '1';
			-- when "-"
			when val_dec =>
				DATA_RDWR <= '0';
				DATA_EN <= '1';
				sel2 <= '0';
				sel1 <= '1';
				nextstate <= val_dec2;
			when val_dec2 =>
				sel1 <= '1';
				sel2 <= '0';
				sel3 <= "01";
				DATA_RDWR <= '1';
				DATA_EN <= '1';
				nextstate <= fetch;
				PC_inc <= '1';
			-- when "."
			when print =>
				if OUT_BUSY = '1' then
					nextstate <= print;
				else
					DATA_EN <= '1';
					DATA_RDWR <= '0';
					sel1 <= '1';
					sel2 <= '0';
					sel3 <= "11";
					nextstate <= print2;
				end if;
			when print2 =>
				OUT_WE <= '1';
				PC_inc <= '1';
				nextstate <= fetch;
			-- when ","
			when readv =>
				IN_REQ <= '1';
				if IN_VLD = '0' then
					nextstate <= readv;
				else
					sel1 <= '1';
					sel2 <= '0';
					sel3 <= "00";
					DATA_EN <= '1';
					DATA_RDWR <= '1';
					PC_inc <= '1';
					nextstate <= fetch;
				end if;
			-- when "$"
			when storeto =>
				DATA_RDWR <= '0';
				DATA_EN <= '1';
				sel2 <= '0';
				sel1 <= '1';
				nextstate <= storeto2;
			when storeto2 =>
				DATA_RDWR <= '1';
				DATA_EN <= '1';
				sel1 <= '1';
				sel2 <= '1';
				sel3 <= "11";
				nextstate <= val_dec2;
			-- when "!"
			when storefrom =>
				sel1 <= '1';
				sel2 <= '1';
				DATA_EN <= '1';
				DATA_RDWR <= '0';
				nextstate <= storefrom2;
			when storefrom2 =>
				sel1 <= '1';
				sel2 <= '0';
				sel3 <= "11";
				DATA_EN <= '1';
				DATA_RDWR <= '1';
				PC_inc <= '1';
				nextstate <= fetch;
			-- when "["
			when lbrac =>
				PC_inc <= '1';
				sel1 <= '1';
				sel2 <= '0';
				DATA_EN <= '1';
				DATA_RDWR <= '0';
				nextstate <= lbrac2;
			when lbrac2 =>
				if DATA_RDATA = "00000000" then
					CNT_inc <= '1';
					nextstate <= lbrac3;
				else
					nextstate <= fetch;
				end if;
			when lbrac3 =>
				if CNT = "0000000000000" then
					nextstate <= fetch;
				else
					sel1 <= '0';
					DATA_EN <= '1';
					DATA_RDWR <= '0';
					nextstate <= lbrac4;
				end if;
			when lbrac4 =>
				if DATA_RDATA = X"5B" then
					CNT_inc <= '1';
				elsif DATA_RDATA = X"5D" then
					CNT_dec <= '1';
				end if;
				PC_inc <= '1';
				nextstate <= lbrac3;
			-- when "]"
			when rbrac =>
				sel1 <= '1';
				sel2 <= '0';
				DATA_EN <= '1';
				DATA_RDWR <= '0';
				nextstate <= rbrac2;
			when rbrac2 =>
				if DATA_RDATA = "00000000" then
					PC_inc <= '1';
					nextstate <= fetch;
				else
					CNT_inc <= '1';
					PC_dec <= '1';
					nextstate <= rbrac3;
				end if;
			when rbrac3 =>
				if CNT = "0000000000000" then
					nextstate <= fetch;
				else
					sel1 <= '0';
					DATA_EN <= '1';
					DATA_RDWR <= '0';
					nextstate <= rbrac4;
				end if;
			when rbrac4 =>
				if DATA_RDATA = X"5D" then
					CNT_inc <= '1';
				elsif DATA_RDATA = X"5B" then
					CNT_dec <= '1';
				end if;
				nextstate <= rbrac5;
			when rbrac5 =>
				if CNT = "0000000000000" then
					PC_inc <= '1';
				else
					PC_dec <= '1';
				end if;
				nextstate <= rbrac3;
			-- when nul
			when theend =>
				nextstate <= theend;
			when others => 
				PC_inc <= '1';
				nextstate <= fetch;
		end case;
	end process next_state_logic;
	

end behavioral;